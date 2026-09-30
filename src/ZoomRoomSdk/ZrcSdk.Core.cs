using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;

namespace PepperDash.ZoomRoom.Sdk;

/// <summary>
/// C# P/Invoke wrapper for the Zoom Rooms Controller SDK (ZRC SDK).
/// </summary>
/// <remarks>
/// <para>
/// This class loads the native <c>libzrcsdkwrapperpdt.so</c> C wrapper at runtime using
/// a dedicated SDK thread that drives both initialization and the 150 ms HeartBeat loop,
/// matching the SDK's single-threaded libuv event loop requirement.
/// </para>
/// <para>
/// The native SDK is one singleton per process. Each <see cref="ZrcSdk"/> instance controls one
/// Zoom Room on it, selected by the room ID passed to <see cref="Initialize(string, string?)"/>,
/// and every instance is initialized, pumped and uninitialized on the same shared SDK thread.
/// </para>
/// <para>
/// On Crestron systems all writable application paths are mounted <c>noexec</c>. The
/// native library is therefore loaded via <c>memfd_create</c> + <c>dlopen(/proc/self/fd/N)</c>
/// to bypass the restriction. The proprietary <c>libZRCSdk.so</c> must already be installed
/// on the device at <c>/usr/lib/</c>.
/// </para>
/// </remarks>
public partial class ZrcSdk : IDisposable
{
    private IntPtr _handle;
    private bool _disposed;

    // Set when this instance was uninitialized while other rooms kept the native SDK alive. Its
    // native handle (and the callback delegates it points at) must outlive the SDK, so it is freed
    // by the last instance to uninitialize rather than by Dispose.
    private volatile bool _retired;

    // ── Shared SDK thread ─────────────────────────────────────────────────────
    // The native SDK is one singleton, created, pumped and destroyed on a single thread. That
    // thread is shared by every instance: it runs queued lifecycle work, then one HeartBeat per
    // tick, and exits once no instance is initialized.
    private static readonly object s_sdkThreadLock = new();
    private static readonly Queue<Action> s_sdkWork = new();
    private static Thread? s_sdkThread;
    // Both lists are touched only on the SDK thread.
    private static readonly List<ZrcSdk> s_initialized = new();
    private static readonly List<ZrcSdk> s_retired = new();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    internal delegate void SdkEventCallbackDelegate(
        [MarshalAs(UnmanagedType.LPStr)] string message,
        int errorCode,
        IntPtr userData);

    // ── Core callback delegate held here ─────────────────────────────────────
    private SdkEventCallbackDelegate? _initializedCallbackDelegate;

    // ── Partial method declarations — implemented in each domain partial file ─
    partial void InitializeConnectionCallbacks();
    partial void InitializePairingCallbacks();
    partial void InitializeMeetingCallbacks();
    partial void InitializeAudioCallbacks();
    partial void InitializeVideoCallbacks();
    partial void InitializeRecordingCallbacks();
    partial void InitializeParticipantCallbacks();
    partial void InitializeContactsCallbacks();
    partial void InitializeMeetingListCallbacks();
    partial void InitializeLayoutCallbacks();
    partial void InitializeShareCallbacks();
    partial void InitializeZrcsCallbacks();
    partial void InitializeCameraCallbacks();
    partial void InitializeChatCallbacks();
    partial void InitializeBreakoutRoomCallbacks();
    partial void InitializeWaitingRoomCallbacks();
    partial void InitializePollingCallbacks();
    partial void InitializeQACallbacks();
    partial void InitializeReactionsCallbacks();
    partial void InitializePhoneCallbacks();
    partial void InitializeSettingsCallbacks();
    partial void InitializeProAVCallbacks();
    partial void InitializePromptCallbacks();
    partial void InitializeBreakoutAdminCallbacks();
    partial void InitializeWebinarCallbacks();

    #region Native glue

    private const string DllName = "zrcsdkwrapperpdt";

    private static IntPtr _cachedWrapperHandle = IntPtr.Zero;
    private static readonly object _wrapperHandleLock = new();

    private const int    RTLD_NOW    = 2;
    private const int    RTLD_LAZY   = 1;
    private const int    RTLD_GLOBAL = 0x100;
    private const uint   MFD_CLOEXEC = 1;
    private const int    SYS_memfd_create = 385; // ARM32 syscall number

    [DllImport("libdl.so.2")]
    private static extern IntPtr dlopen(string filename, int flags);

    [DllImport("libdl.so.2")]
    private static extern IntPtr dlerror();

    [DllImport("libc.so.6", SetLastError = true)]
    private static extern int syscall(int number, string name, uint flags);

    [DllImport("libc.so.6", SetLastError = true)]
    private static extern IntPtr write(int fd, byte[] buf, IntPtr count);

    [DllImport("libc.so.6", SetLastError = true)]
    private static extern int close(int fd);

    static ZrcSdk()
    {
        try
        {
            dlerror();
            var handle = dlopen("libZRCSdk.so", RTLD_NOW | RTLD_GLOBAL);
            if (handle == IntPtr.Zero)
            {
                var errPtr = dlerror();
                var errMsg = errPtr != IntPtr.Zero ? Marshal.PtrToStringAnsi(errPtr) : "unknown";
                throw new InvalidOperationException($"Failed to load libZRCSdk.so: {errMsg}");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Warning: Could not preload ZRC SDK: {ex.Message}");
        }

        NativeLibrary.SetDllImportResolver(typeof(ZrcSdk).Assembly, DllImportResolver);
    }

    private static IntPtr DllImportResolver(string libraryName, Assembly assembly, DllImportSearchPath? searchPath)
    {
        if (libraryName != DllName)
            return IntPtr.Zero;

        if (_cachedWrapperHandle != IntPtr.Zero)
            return _cachedWrapperHandle;

        lock (_wrapperHandleLock)
        {
            if (_cachedWrapperHandle != IntPtr.Zero)
                return _cachedWrapperHandle;

            var libPath = ResolveWrapperPath();
            if (!File.Exists(libPath))
            {
                // Report every location that was searched so a misconfigured SetLibraryPath is obvious.
                var searched = string.IsNullOrEmpty(_overrideLibraryPath)
                    ? Path.Combine(DefaultWrapperDirectory, WrapperFileName)
                    : $"{Path.Combine(_overrideLibraryPath, WrapperFileName)} and {Path.Combine(DefaultWrapperDirectory, WrapperFileName)}";
                throw new DllNotFoundException(
                    $"{WrapperFileName} not found. Searched: {searched}. " +
                    "Set the wrapper directory with ZrcSdk.SetLibraryPath(dir) (the directory must contain the file).");
            }

            var libBytes = File.ReadAllBytes(libPath);
            int memfd = syscall(SYS_memfd_create, "zrcsdkwrapperpdt", MFD_CLOEXEC);
            if (memfd < 0)
                throw new DllNotFoundException($"memfd_create failed (errno={Marshal.GetLastWin32Error()})");

            try
            {
                var written = write(memfd, libBytes, (IntPtr)libBytes.Length);
                if (written.ToInt64() != libBytes.Length)
                    throw new DllNotFoundException($"memfd write incomplete: {written}/{libBytes.Length}");

                var procPath = $"/proc/self/fd/{memfd}";
                dlerror();
                var handle = dlopen(procPath, RTLD_LAZY | RTLD_GLOBAL);
                if (handle == IntPtr.Zero)
                {
                    var errPtr = dlerror();
                    var errMsg = errPtr != IntPtr.Zero ? Marshal.PtrToStringAnsi(errPtr) : "unknown";
                    throw new DllNotFoundException($"Failed to dlopen via memfd ({procPath}): {errMsg}");
                }

                _cachedWrapperHandle = handle;
                return handle;
            }
            finally
            {
                close(memfd);
            }
        }
    }

    private static string? _overrideLibraryPath;

    private const string WrapperFileName = "libzrcsdkwrapperpdt.so";
    private const string DefaultWrapperDirectory = "/usr/lib";

    /// <summary>
    /// Resolves the full path to <c>libzrcsdkwrapperpdt.so</c>. Honors the directory set via
    /// <see cref="SetLibraryPath"/> (if it contains the wrapper); otherwise falls back to
    /// <c>/usr/lib</c>. The host can stage the wrapper in a writable location and point the
    /// SDK at it, which is required on firmware where <c>/usr/lib</c> is read-only.
    /// </summary>
    private static string ResolveWrapperPath()
    {
        if (!string.IsNullOrEmpty(_overrideLibraryPath))
        {
            var overridePath = Path.Combine(_overrideLibraryPath, WrapperFileName);
            if (File.Exists(overridePath))
                return overridePath;
        }

        return Path.Combine(DefaultWrapperDirectory, WrapperFileName);
    }

    /// <summary>
    /// Overrides the directory searched for <c>libzrcsdkwrapperpdt.so</c>. When set, the
    /// resolver looks here first and falls back to <c>/usr/lib</c> if the wrapper is not present.
    /// Call before constructing any <see cref="ZrcSdk"/> instance.
    /// </summary>
    public static void SetLibraryPath(string directoryPath) =>
        _overrideLibraryPath = directoryPath;

    // ── P/Invoke declarations ─────────────────────────────────────────────────

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr ZrcSdk_Create();
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_Destroy(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_InitializeRoom(IntPtr handle, string configPath, string? roomID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_Uninitialize(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_HeartBeat(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GetActiveRoomCount();
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_GetSDKVersion(IntPtr handle, StringBuilder buffer, int bufferSize);

    // Pairing
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_PairRoomWithActivationCode(IntPtr handle, string activationCode);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_CanRetryToPairLastRoom(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RetryToPairRoom(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_UnpairRoom(IntPtr handle);

    // Connection
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GetConnectionState(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_WakeZoomRoomUp(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_LogoutZoomRoomDevice(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RestartZoomRoomOS(IntPtr handle);

    // Meeting
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_StartMeeting(IntPtr handle, string meetingNumber);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StartInstantMeeting(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_StartMeetingWithHostKey(IntPtr handle, string hostKey);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_JoinMeeting(IntPtr handle, string meetingNumber);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_JoinMeetingWithURL(IntPtr handle, string url);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_LeaveMeeting(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_EndMeeting(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AnswerMeetingInvite(IntPtr handle, int accept);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_SendMeetingPassword(IntPtr handle, string password);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_CancelEnteringMeetingPassword(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_CancelWaitingForHost(IntPtr handle);

    // Audio / Video
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetAudioMute(IntPtr handle, int mute);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetVideoState(IntPtr handle, int start);

    // Recording
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StartRecording(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StopRecording(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_PauseRecording(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ResumeRecording(IntPtr handle);

    // Participants
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GetParticipantCount(IntPtr handle);

    // ZRCS
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_IsZRCSEnabled(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_OpenRoomControls(IntPtr handle, int open);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ControlZRCSDevice(IntPtr handle, string deviceID, string methodID, string paramID, string value);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ExecuteZRCSScene(IntPtr handle, string sceneID);

    // ── Callback setters ──────────────────────────────────────────────────────
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetInitializedCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetConnectionStateChangedCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetErrorCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetPairRoomResultCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMeetingStatusCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetStartPmiResultCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetExitMeetingCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMeetingNeedsPasswordCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMeetingInviteCallback(IntPtr handle, ZrcMeetingInviteCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetAudioStatusCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMuteOnEntryCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetParticipantCountCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetHostChangedCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetRecordingStatusCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetControlSystemEnabledCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetInstantMeetingStartedCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);

    #endregion

    // ── Core events ───────────────────────────────────────────────────────────

    /// <summary>Fired when the SDK has initialized. <see cref="SdkEventArgs.ErrorCode"/> is 0 on success.</summary>
    public event EventHandler<SdkEventArgs>? Initialized;

    // ── Constructor ───────────────────────────────────────────────────────────

    /// <summary>
    /// Creates the native SDK instance and registers all event callbacks.
    /// </summary>
    public ZrcSdk()
    {
        _handle = ZrcSdk_Create();
        if (_handle == IntPtr.Zero)
            throw new InvalidOperationException("Failed to create ZRC SDK instance");

        _initializedCallbackDelegate = OnInitializedCallback;
        ZrcSdk_SetInitializedCallback(_handle, _initializedCallbackDelegate, IntPtr.Zero);

        InitializeConnectionCallbacks();
        InitializePairingCallbacks();
        InitializeMeetingCallbacks();
        InitializeAudioCallbacks();
        InitializeVideoCallbacks();
        InitializeRecordingCallbacks();
        InitializeParticipantCallbacks();
        InitializeContactsCallbacks();
        InitializeMeetingListCallbacks();
        InitializeLayoutCallbacks();
        InitializeShareCallbacks();
        InitializeZrcsCallbacks();
        InitializeCameraCallbacks();
        InitializeChatCallbacks();
        InitializeBreakoutRoomCallbacks();
        InitializeWaitingRoomCallbacks();
        InitializePollingCallbacks();
        InitializeQACallbacks();
        InitializeReactionsCallbacks();
        InitializePhoneCallbacks();
        InitializeSettingsCallbacks();
        InitializeProAVCallbacks();
        InitializePromptCallbacks();
        InitializeBreakoutAdminCallbacks();
        InitializeWebinarCallbacks();
    }

    // ── Lifecycle ─────────────────────────────────────────────────────────────

    /// <summary>
    /// Initializes the SDK and starts the internal HeartBeat loop.
    /// Blocks until the native <c>IZRCSDK::CreateInstance()</c> call returns, then returns.
    /// </summary>
    /// <param name="configPath">
    /// Directory used by the SDK for persistent state. On Crestron use <c>/user/zrcsdk</c>.
    /// </param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool Initialize(string configPath) => Initialize(configPath, null);

    /// <summary>
    /// Initializes the SDK for one Zoom Room and starts the shared HeartBeat loop if it is not
    /// already running. Blocks until the native initialization returns.
    /// </summary>
    /// <param name="configPath">
    /// Directory used by the SDK for persistent state. On Crestron use <c>/user/zrcsdk</c>. The
    /// directory belongs to the shared native SDK: the first instance to initialize sets it and
    /// later instances use that same directory.
    /// </param>
    /// <param name="roomId">
    /// Identifies this instance's Zoom Room to the SDK. Pairing is stored per ID, and the ID is
    /// shown as the controller's serial number in the Zoom web portal. <see langword="null"/> or
    /// empty uses the SDK's default ID, which is what a single-room program was paired under.
    /// Each instance in a process needs its own ID; a repeated one fails.
    /// </param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool Initialize(string configPath, string? roomId)
    {
        ThrowIfDisposed();
        var ok = false;
        RunOnSdkThread(() =>
        {
            ok = ZrcSdk_InitializeRoom(_handle, configPath, roomId) == 0;
            if (ok && !s_initialized.Contains(this)) s_initialized.Add(this);
        }, Timeout.InfiniteTimeSpan);
        return ok;
    }

    /// <summary>
    /// Runs <paramref name="work"/> on the shared SDK thread, starting that thread if needed, and
    /// waits up to <paramref name="timeout"/> for it to finish. Work queued from the SDK thread
    /// itself (an event handler calling back in) runs inline.
    /// </summary>
    private static bool RunOnSdkThread(Action work, TimeSpan timeout)
    {
        if (Thread.CurrentThread == s_sdkThread)
        {
            work();
            return true;
        }

        var done = new ManualResetEventSlim(false);
        lock (s_sdkThreadLock)
        {
            s_sdkWork.Enqueue(() =>
            {
                try { work(); }
                finally { done.Set(); }
            });

            if (s_sdkThread == null)
            {
                s_sdkThread = new Thread(SdkThreadProc)
                {
                    IsBackground = true,
                    Name = "ZrcSdkThread",
                };
                s_sdkThread.Start();
            }
        }

        // Not disposed: on a timeout the SDK thread still sets it when the work eventually runs.
        return done.Wait(timeout);
    }

    private static void SdkThreadProc()
    {
        while (true)
        {
            while (true)
            {
                Action? work;
                lock (s_sdkThreadLock)
                {
                    if (s_sdkWork.Count == 0)
                    {
                        if (s_initialized.Count == 0)
                        {
                            s_sdkThread = null;
                            return;
                        }
                        break;
                    }
                    work = s_sdkWork.Dequeue();
                }

                try { work(); }
                catch { /* lifecycle work reports through its own result; keep the pump alive */ }
            }

            Thread.Sleep(150);

            // One HeartBeat pumps the shared native SDK for every room.
            var pumped = s_initialized.Find(sdk => sdk._handle != IntPtr.Zero);
            if (pumped != null)
                ZrcSdk_HeartBeat(pumped._handle);
        }
    }

    // SDK thread only.
    private void UninitializeOnSdkThread()
    {
        if (!s_initialized.Remove(this))
            return;

        // The handle is already gone if Dispose gave up waiting for this work and destroyed it.
        if (_handle != IntPtr.Zero)
            ZrcSdk_Uninitialize(_handle);

        if (ZrcSdk_GetActiveRoomCount() > 0)
        {
            // Other rooms keep the native SDK alive, and this room's sinks are still registered with
            // it: hold the handle and the delegates it calls until the SDK is destroyed.
            if (_handle != IntPtr.Zero)
            {
                _retired = true;
                s_retired.Add(this);
            }
            return;
        }

        // That was the last room, so the native SDK is gone and the retired handles can go too.
        foreach (var retired in s_retired)
        {
            ZrcSdk_Destroy(retired._handle);
            retired._handle = IntPtr.Zero;
        }
        s_retired.Clear();
    }

    /// <summary>Returns the ZRC SDK version string.</summary>
    public string GetSDKVersion()
    {
        ThrowIfDisposed();
        var buffer = new StringBuilder(256);
        ZrcSdk_GetSDKVersion(_handle, buffer, buffer.Capacity);
        return buffer.ToString();
    }

    private void OnInitializedCallback(string message, int errorCode, IntPtr userData) =>
        Initialized?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });

    // ── IDisposable ───────────────────────────────────────────────────────────

    private void ThrowIfDisposed()
    {
        if (_disposed) throw new ObjectDisposedException(nameof(ZrcSdk));
    }

    /// <summary>Stops the HeartBeat loop and uninitializes the SDK on its owner thread.</summary>
    public void Uninitialize()
    {
        if (!_disposed && _handle != IntPtr.Zero)
            RunOnSdkThread(UninitializeOnSdkThread, TimeSpan.FromSeconds(2));
    }

    /// <inheritdoc/>
    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            // An initialized instance is rooted by the shared SDK thread's lists, so the finalizer
            // only ever sees one that never initialized or has already been uninitialized.
            if (disposing && _handle != IntPtr.Zero)
                RunOnSdkThread(UninitializeOnSdkThread, TimeSpan.FromSeconds(2));

            // A retired handle is freed by the last room to uninitialize (see UninitializeOnSdkThread).
            if (_handle != IntPtr.Zero && !_retired)
            {
                ZrcSdk_Destroy(_handle);
                _handle = IntPtr.Zero;
            }

            _disposed = true;
        }
    }

    /// <summary>Finalizer.</summary>
    ~ZrcSdk() => Dispose(false);

    /// <summary>
    /// Stops the HeartBeat loop, flushes SDK state to disk, and releases all native resources.
    /// </summary>
    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    /// <summary>
    /// Marshals a native contiguous array of <typeparamref name="TNative"/> structs (as delivered by an
    /// SDK callback) into a managed <typeparamref name="TManaged"/> array via <paramref name="project"/>.
    /// Single source of the null/zero-count guard and stride/PtrToStructure loop shared by the
    /// contact-list and meeting-list marshalers.
    /// </summary>
    private static TManaged[] MarshalNativeArray<TNative, TManaged>(
        IntPtr ptr, int count, Func<TNative, TManaged> project) where TNative : struct
    {
        if (ptr == IntPtr.Zero || count <= 0)
            return Array.Empty<TManaged>();

        var result = new TManaged[count];
        int stride = Marshal.SizeOf<TNative>();
        for (int i = 0; i < count; i++)
            result[i] = project(Marshal.PtrToStructure<TNative>(ptr + i * stride));
        return result;
    }
}

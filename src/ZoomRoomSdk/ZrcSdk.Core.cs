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
    private Thread? _sdkThread;
    private volatile bool _sdkRunning;
    private bool _initResult;
    private ManualResetEventSlim? _initDone;

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

    #region Native glue

    private const string DllName = "zrcsdkwrapperpdt";

    private static IntPtr _cachedWrapperHandle = IntPtr.Zero;

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
    private static extern int ZrcSdk_Initialize(IntPtr handle, string configPath);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_Uninitialize(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_HeartBeat(IntPtr handle);
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
    private static extern void ZrcSdk_SetMeetingInviteCallback(IntPtr handle, SdkEventCallbackDelegate cb, IntPtr userData);
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
    public bool Initialize(string configPath)
    {
        ThrowIfDisposed();
        _initDone = new ManualResetEventSlim(false);
        _sdkRunning = true;
        _sdkThread = new Thread(() => SdkThreadProc(configPath))
        {
            IsBackground = true,
            Name = "ZrcSdkThread",
        };
        _sdkThread.Start();
        _initDone.Wait();
        _initDone.Dispose();
        _initDone = null;
        return _initResult;
    }

    private void SdkThreadProc(string configPath)
    {
        int result = ZrcSdk_Initialize(_handle, configPath);
        _initResult = result == 0;
        _initDone!.Set();

        if (_initResult)
        {
            while (_sdkRunning)
            {
                Thread.Sleep(150);
                if (_handle != IntPtr.Zero && _sdkRunning)
                    ZrcSdk_HeartBeat(_handle);
            }
            ZrcSdk_Uninitialize(_handle);
        }
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
        {
            _sdkRunning = false;
            _sdkThread?.Join(TimeSpan.FromSeconds(2));
            _sdkThread = null;
        }
    }

    /// <inheritdoc/>
    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            _sdkRunning = false;
            if (disposing)
            {
                _sdkThread?.Join(TimeSpan.FromSeconds(2));
                _sdkThread = null;
            }

            if (_handle != IntPtr.Zero)
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
}

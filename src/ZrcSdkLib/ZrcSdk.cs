using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using Path = System.IO.Path;

namespace PepperDash.Zoom.ZrcSdk;

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
/// <para>
/// Typical usage:
/// <code>
/// var sdk = new ZrcSdk();
/// sdk.PairRoomResult += (s, e) => Console.WriteLine(ZrcSdkCodes.GetPairRoomResultDescription(e.ErrorCode));
/// sdk.Initialize("/user/zrcsdk");
/// if (sdk.CanRetryToPairLastRoom())
///     sdk.RetryToPairRoom();
/// </code>
/// </para>
/// </remarks>
public class ZrcSdk : IDisposable
{
    private IntPtr _handle;
    private bool _disposed;
    // Single dedicated thread drives both Initialize() and HeartBeat() — matches SDK's libuv model
    private Thread? _sdkThread;
    private volatile bool _sdkRunning;
    private bool _initResult;
    private ManualResetEventSlim? _initDone;

    // Delegate types matching the native callbacks
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void SdkEventCallbackDelegate(
        [MarshalAs(UnmanagedType.LPStr)] string message,
        int errorCode,
        IntPtr userData);

    // Keep references to prevent GC collection of callback delegates
    private SdkEventCallbackDelegate _initializedCallbackDelegate;
    private SdkEventCallbackDelegate _meetingStateChangedCallbackDelegate;
    private SdkEventCallbackDelegate _errorCallbackDelegate;
    private SdkEventCallbackDelegate _instantMeetingStartedCallbackDelegate;
    private SdkEventCallbackDelegate _pairRoomResultCallbackDelegate;
    private SdkEventCallbackDelegate _meetingStatusCallbackDelegate;
    private SdkEventCallbackDelegate _startPmiResultCallbackDelegate;
    private SdkEventCallbackDelegate _exitMeetingCallbackDelegate;
    private SdkEventCallbackDelegate _meetingNeedsPasswordCallbackDelegate;
    private SdkEventCallbackDelegate _meetingInviteCallbackDelegate;
    private SdkEventCallbackDelegate _audioStatusCallbackDelegate;
    private SdkEventCallbackDelegate _muteOnEntryCallbackDelegate;
    private SdkEventCallbackDelegate _participantCountCallbackDelegate;
    private SdkEventCallbackDelegate _hostChangedCallbackDelegate;
    private SdkEventCallbackDelegate _recordingStatusCallbackDelegate;
    private SdkEventCallbackDelegate _controlSystemEnabledCallbackDelegate;

    #region Events

    /// <summary>Fired when the SDK has initialized successfully. <see cref="SdkEventArgs.ErrorCode"/> is 0 on success.</summary>
    public event EventHandler<SdkEventArgs> Initialized;

    /// <summary>
    /// Fired when the ZRC connection state changes.
    /// <see cref="SdkEventArgs.ErrorCode"/>: 0=Established, 1=Connected, 2=Disconnected.
    /// Use <see cref="ZrcSdkCodes.GetConnectionStateDescription"/> to get a description.
    /// </summary>
    /// <remarks>
    /// A brief Disconnected→Established→Connected cycle during meeting start/end is normal ZRC behavior.
    /// </remarks>
    public event EventHandler<SdkEventArgs> ConnectionStateChanged;

    /// <summary>Fired when the SDK reports an error. <see cref="SdkEventArgs.ErrorCode"/> is a <c>ZRCSDKError</c> value.</summary>
    public event EventHandler<SdkEventArgs> Error;

    /// <summary>
    /// Fired when a meeting is started via <see cref="StartInstantMeeting"/>.
    /// <see cref="SdkEventArgs.Message"/> is the meeting number; <see cref="SdkEventArgs.ErrorCode"/> is the result.
    /// </summary>
    public event EventHandler<SdkEventArgs> InstantMeetingStarted;

    /// <summary>
    /// Fired when a room pairing attempt completes (via <see cref="PairRoomWithActivationCode"/>,
    /// <see cref="RetryToPairRoom"/>, or automatic reconnect).
    /// <see cref="SdkEventArgs.ErrorCode"/> is 0 on success. Use
    /// <see cref="ZrcSdkCodes.GetPairRoomResultDescription"/> for other codes.
    /// </summary>
    public event EventHandler<SdkEventArgs> PairRoomResult;

    /// <summary>
    /// Fired when the meeting status changes.
    /// <see cref="SdkEventArgs.ErrorCode"/>: 0=NotInMeeting, 1=Connecting, 2=InMeeting, 3=LoggedOut.
    /// Use <see cref="ZrcSdkCodes.GetMeetingStatusDescription"/> to get a description.
    /// </summary>
    public event EventHandler<SdkEventArgs> MeetingStatus;

    /// <summary>
    /// Fired when a PMI or scheduled meeting starts.
    /// <see cref="SdkEventArgs.Message"/> is the meeting number; <see cref="SdkEventArgs.ErrorCode"/> is the result.
    /// </summary>
    public event EventHandler<SdkEventArgs> StartPmiResult;

    /// <summary>
    /// Fired when a meeting ends.
    /// <see cref="SdkEventArgs.ErrorCode"/> encodes result (low byte) and reason (high byte).
    /// Decode with: <c>result = e.ErrorCode &amp; 0xFF; reason = (e.ErrorCode >> 8) &amp; 0xFF</c>.
    /// Use <see cref="ZrcSdkCodes.GetExitMeetingReasonDescription"/> for the reason.
    /// </summary>
    public event EventHandler<SdkEventArgs> ExitMeeting;

    /// <summary>
    /// Fired when the meeting requires a password.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if the previous password attempt was wrong.
    /// Respond with <see cref="SendMeetingPassword"/> or <see cref="CancelEnteringMeetingPassword"/>.
    /// </summary>
    public event EventHandler<SdkEventArgs> MeetingNeedsPassword;

    /// <summary>
    /// Fired when an incoming meeting invitation arrives.
    /// <see cref="SdkEventArgs.Message"/> contains the caller's name.
    /// </summary>
    public event EventHandler<SdkEventArgs> MeetingInvite;

    /// <summary>
    /// Fired when the local audio mute state changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if muted, 0 if unmuted.
    /// </summary>
    public event EventHandler<SdkEventArgs> AudioStatus;

    /// <summary>
    /// Fired when the mute-on-entry meeting setting changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if enabled, 0 if disabled.
    /// </summary>
    public event EventHandler<SdkEventArgs> MuteOnEntry;

    /// <summary>
    /// Fired when the participant count changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is the current participant count.
    /// </summary>
    public event EventHandler<SdkEventArgs> ParticipantCount;

    /// <summary>
    /// Fired when the meeting host changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if this device is now the host, 0 otherwise.
    /// </summary>
    public event EventHandler<SdkEventArgs> HostChanged;

    /// <summary>
    /// Fired when the cloud recording status changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if recording is in progress, 0 if stopped.
    /// </summary>
    public event EventHandler<SdkEventArgs> RecordingStatus;

    /// <summary>
    /// Fired when the Zoom Room Control System (ZRCS) enabled state changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if ZRCS is enabled, 0 if disabled.
    /// </summary>
    public event EventHandler<SdkEventArgs> ControlSystemEnabled;

    #endregion

    #region Native Imports

    private const string DllName = "zrcsdkwrapperpdt";

    private static IntPtr _cachedWrapperHandle = IntPtr.Zero;

    private const int RTLD_NOW    = 2;
    private const int RTLD_LAZY   = 1;
    private const int RTLD_GLOBAL = 0x100;
    private const uint MFD_CLOEXEC = 1;

    // ARM32 syscall number for memfd_create
    private const int SYS_memfd_create = 385;

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
        // Preload the proprietary ZRC SDK library with RTLD_GLOBAL so the wrapper can resolve its symbols
        try
        {
            dlerror();
            var handle = dlopen("libZRCSdk.so", RTLD_NOW | RTLD_GLOBAL);
            if (handle == IntPtr.Zero)
            {
                var errPtr = dlerror();
                var errMsg = errPtr != IntPtr.Zero ? Marshal.PtrToStringAnsi(errPtr) : "Unknown error";
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

        // Resolve library path: try Crestron app directory first, then /usr/lib
        var appDir = _overrideLibraryPath
            ?? Crestron.SimplSharp.CrestronIO.Directory.GetApplicationDirectory();
        var libPath = Path.Combine(appDir, "libzrcsdkwrapperpdt.so");

        if (!System.IO.File.Exists(libPath))
            libPath = "/usr/lib/libzrcsdkwrapperpdt.so";

        if (!System.IO.File.Exists(libPath))
            throw new DllNotFoundException($"libzrcsdkwrapperpdt.so not found in app directory or /usr/lib");

        // All writable Crestron paths are mounted noexec. Load via memfd_create so the anonymous
        // fd is never subject to noexec enforcement.
        var libBytes = System.IO.File.ReadAllBytes(libPath);
        Console.WriteLine($"Loading {libPath} via memfd ({libBytes.Length} bytes)");

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
                var errMsg = errPtr != IntPtr.Zero ? Marshal.PtrToStringAnsi(errPtr) : "Unknown error";
                throw new DllNotFoundException($"Failed to dlopen via memfd ({procPath}): {errMsg}");
            }

            Console.WriteLine($"Successfully loaded {libPath} via memfd");
            _cachedWrapperHandle = handle;
            return handle;
        }
        finally
        {
            close(memfd);
        }
    }

    private static string? _overrideLibraryPath;

    /// <summary>
    /// Overrides the directory searched for <c>libzrcsdkwrapperpdt.so</c>.
    /// Call before constructing any <see cref="ZrcSdk"/> instance.
    /// When not set the Crestron application directory is used, with <c>/usr/lib</c> as fallback.
    /// </summary>
    /// <param name="directoryPath">Absolute path to the directory containing the native library.</param>
    public static void SetLibraryPath(string directoryPath) =>
        _overrideLibraryPath = directoryPath;

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
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_PairRoomWithActivationCode(IntPtr handle, string activationCode);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_CanRetryToPairLastRoom(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RetryToPairRoom(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_UnpairRoom(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_StartMeeting(IntPtr handle, string meetingNumber);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_EndMeeting(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetAudioMute(IntPtr handle, int mute);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetVideoState(IntPtr handle, int start);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetInitializedCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetConnectionStateChangedCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetErrorCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StartInstantMeeting(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetInstantMeetingStartedCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetPairRoomResultCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GetConnectionState(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_WakeZoomRoomUp(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_LogoutZoomRoomDevice(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RestartZoomRoomOS(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_JoinMeeting(IntPtr handle, string meetingNumber);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_JoinMeetingWithURL(IntPtr handle, string url);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_StartMeetingWithHostKey(IntPtr handle, string hostKey);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_LeaveMeeting(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_SendMeetingPassword(IntPtr handle, string password);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_CancelEnteringMeetingPassword(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_CancelWaitingForHost(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StartRecording(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StopRecording(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_PauseRecording(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ResumeRecording(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GetParticipantCount(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_IsZRCSEnabled(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_OpenRoomControls(IntPtr handle, int open);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ControlZRCSDevice(IntPtr handle, string deviceID, string methodID, string paramID, string value);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ExecuteZRCSScene(IntPtr handle, string sceneID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMeetingStatusCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetStartPmiResultCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetExitMeetingCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMeetingNeedsPasswordCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMeetingInviteCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetAudioStatusCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMuteOnEntryCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetParticipantCountCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetHostChangedCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetRecordingStatusCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetControlSystemEnabledCallback(IntPtr handle, SdkEventCallbackDelegate callback, IntPtr userData);

    #endregion

    /// <summary>
    /// Initializes a new instance of <see cref="ZrcSdk"/>, creating the native SDK instance
    /// and registering all event callbacks.
    /// </summary>
    /// <exception cref="InvalidOperationException">Thrown if the native library cannot create an SDK instance.</exception>
    public ZrcSdk()
    {
        _handle = ZrcSdk_Create();
        if (_handle == IntPtr.Zero)
            throw new InvalidOperationException("Failed to create ZRC SDK instance");

        _initializedCallbackDelegate          = OnInitializedCallback;
        _meetingStateChangedCallbackDelegate  = OnMeetingStateChangedCallback;
        _errorCallbackDelegate                = OnErrorCallback;
        _instantMeetingStartedCallbackDelegate = OnInstantMeetingStartedCallback;
        _pairRoomResultCallbackDelegate       = OnPairRoomResultCallback;
        _meetingStatusCallbackDelegate        = OnMeetingStatusCallback;
        _startPmiResultCallbackDelegate       = OnStartPmiResultCallback;
        _exitMeetingCallbackDelegate          = OnExitMeetingCallback;
        _meetingNeedsPasswordCallbackDelegate = OnMeetingNeedsPasswordCallback;
        _meetingInviteCallbackDelegate        = OnMeetingInviteCallback;
        _audioStatusCallbackDelegate          = OnAudioStatusCallback;
        _muteOnEntryCallbackDelegate          = OnMuteOnEntryCallback;
        _participantCountCallbackDelegate     = OnParticipantCountCallback;
        _hostChangedCallbackDelegate          = OnHostChangedCallback;
        _recordingStatusCallbackDelegate      = OnRecordingStatusCallback;
        _controlSystemEnabledCallbackDelegate = OnControlSystemEnabledCallback;

        ZrcSdk_SetInitializedCallback(_handle,              _initializedCallbackDelegate,          IntPtr.Zero);
        ZrcSdk_SetConnectionStateChangedCallback(_handle,   _meetingStateChangedCallbackDelegate,  IntPtr.Zero);
        ZrcSdk_SetErrorCallback(_handle,                    _errorCallbackDelegate,                IntPtr.Zero);
        ZrcSdk_SetInstantMeetingStartedCallback(_handle,    _instantMeetingStartedCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetPairRoomResultCallback(_handle,           _pairRoomResultCallbackDelegate,       IntPtr.Zero);
        ZrcSdk_SetMeetingStatusCallback(_handle,            _meetingStatusCallbackDelegate,        IntPtr.Zero);
        ZrcSdk_SetStartPmiResultCallback(_handle,           _startPmiResultCallbackDelegate,       IntPtr.Zero);
        ZrcSdk_SetExitMeetingCallback(_handle,              _exitMeetingCallbackDelegate,          IntPtr.Zero);
        ZrcSdk_SetMeetingNeedsPasswordCallback(_handle,     _meetingNeedsPasswordCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetMeetingInviteCallback(_handle,            _meetingInviteCallbackDelegate,        IntPtr.Zero);
        ZrcSdk_SetAudioStatusCallback(_handle,              _audioStatusCallbackDelegate,          IntPtr.Zero);
        ZrcSdk_SetMuteOnEntryCallback(_handle,              _muteOnEntryCallbackDelegate,          IntPtr.Zero);
        ZrcSdk_SetParticipantCountCallback(_handle,         _participantCountCallbackDelegate,     IntPtr.Zero);
        ZrcSdk_SetHostChangedCallback(_handle,              _hostChangedCallbackDelegate,          IntPtr.Zero);
        ZrcSdk_SetRecordingStatusCallback(_handle,          _recordingStatusCallbackDelegate,      IntPtr.Zero);
        ZrcSdk_SetControlSystemEnabledCallback(_handle,     _controlSystemEnabledCallbackDelegate, IntPtr.Zero);
    }

    #region Lifecycle

    /// <summary>
    /// Initializes the SDK and starts the internal HeartBeat loop.
    /// Blocks until the native <c>IZRCSDK::CreateInstance()</c> call returns, then returns.
    /// The HeartBeat loop continues on a background thread until <see cref="Dispose"/> is called.
    /// </summary>
    /// <param name="configPath">
    /// Directory used by the SDK for persistent state (credentials, logs, SQLite DB).
    /// On Crestron devices use <c>/user/zrcsdk</c>.
    /// </param>
    /// <returns><see langword="true"/> on success, <see langword="false"/> if initialization failed.</returns>
    public bool Initialize(string configPath)
    {
        ThrowIfDisposed();
        _initDone = new ManualResetEventSlim(false);
        _sdkRunning = true;
        _sdkThread = new Thread(() => SdkThreadProc(configPath))
        {
            IsBackground = true,
            Name = "ZrcSdkThread"
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

    #endregion

    #region Pairing

    /// <summary>
    /// Pairs the Zoom Room using an activation code obtained from the Zoom admin portal.
    /// Result is returned asynchronously via the <see cref="PairRoomResult"/> event.
    /// </summary>
    /// <param name="activationCode">The activation code from the Zoom admin portal.</param>
    /// <returns><see langword="true"/> if the request was submitted successfully.</returns>
    public bool PairRoomWithActivationCode(string activationCode)
    {
        ThrowIfDisposed();
        return ZrcSdk_PairRoomWithActivationCode(_handle, activationCode) == 0;
    }

    /// <summary>
    /// Returns <see langword="true"/> if the SDK has stored credentials from a previous pairing
    /// and can reconnect without a new activation code.
    /// </summary>
    public bool CanRetryToPairLastRoom()
    {
        ThrowIfDisposed();
        return ZrcSdk_CanRetryToPairLastRoom(_handle) == 1;
    }

    /// <summary>
    /// Reconnects to the last paired Zoom Room using stored credentials.
    /// No activation code is required. Result arrives via the <see cref="PairRoomResult"/> event.
    /// </summary>
    /// <returns><see langword="true"/> if the request was submitted successfully.</returns>
    public bool RetryToPairRoom()
    {
        ThrowIfDisposed();
        return ZrcSdk_RetryToPairRoom(_handle) == 0;
    }

    /// <summary>
    /// Unpairs the SDK from the current Zoom Room and clears stored credentials.
    /// </summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool UnpairRoom()
    {
        ThrowIfDisposed();
        return ZrcSdk_UnpairRoom(_handle) == 0;
    }

    #endregion

    #region Connection State

    /// <summary>
    /// Returns the current ZRC connection state.
    /// </summary>
    /// <returns>0=Established, 1=Connected, 2=Disconnected, -1=None/error.
    /// Use <see cref="ZrcSdkCodes.GetConnectionStateDescription"/> for a description.</returns>
    public int GetConnectionState()
    {
        ThrowIfDisposed();
        return ZrcSdk_GetConnectionState(_handle);
    }

    /// <summary>Sends a wake command to the Zoom Room display.</summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool WakeZoomRoomUp()
    {
        ThrowIfDisposed();
        return ZrcSdk_WakeZoomRoomUp(_handle) == 0;
    }

    /// <summary>Logs out the Zoom Room device from its Zoom account.</summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool LogoutZoomRoomDevice()
    {
        ThrowIfDisposed();
        return ZrcSdk_LogoutZoomRoomDevice(_handle) == 0;
    }

    /// <summary>Sends a restart command to the Zoom Room OS.</summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool RestartZoomRoomOS()
    {
        ThrowIfDisposed();
        return ZrcSdk_RestartZoomRoomOS(_handle) == 0;
    }

    #endregion

    #region Meeting Control

    /// <summary>Starts a meeting by meeting number or personal link name.</summary>
    /// <param name="meetingNumber">The meeting number or personal link name.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool StartMeeting(string meetingNumber)
    {
        ThrowIfDisposed();
        return ZrcSdk_StartMeeting(_handle, meetingNumber) == 0;
    }

    /// <summary>
    /// Starts an instant (PMI) meeting. The assigned meeting number is returned via the
    /// <see cref="InstantMeetingStarted"/> event.
    /// </summary>
    /// <returns><see langword="true"/> if the request was submitted successfully.</returns>
    public bool StartInstantMeeting()
    {
        ThrowIfDisposed();
        return ZrcSdk_StartInstantMeeting(_handle) == 0;
    }

    /// <summary>Joins a meeting by meeting number or personal link name.</summary>
    /// <param name="meetingNumber">The meeting number or personal link name.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool JoinMeeting(string meetingNumber)
    {
        ThrowIfDisposed();
        return ZrcSdk_JoinMeeting(_handle, meetingNumber) == 0;
    }

    /// <summary>Joins a meeting using a Zoom invite URL.</summary>
    /// <param name="url">The full Zoom meeting URL.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool JoinMeetingWithURL(string url)
    {
        ThrowIfDisposed();
        return ZrcSdk_JoinMeetingWithURL(_handle, url) == 0;
    }

    /// <summary>Starts a scheduled meeting using the host key.</summary>
    /// <param name="hostKey">The host key for the scheduled meeting.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool StartMeetingWithHostKey(string hostKey)
    {
        ThrowIfDisposed();
        return ZrcSdk_StartMeetingWithHostKey(_handle, hostKey) == 0;
    }

    /// <summary>
    /// Ends the current meeting for all participants. Must be host.
    /// </summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool EndMeeting()
    {
        ThrowIfDisposed();
        return ZrcSdk_EndMeeting(_handle) == 0;
    }

    /// <summary>
    /// Leaves the current meeting (self only). The meeting continues for other participants.
    /// </summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool LeaveMeeting()
    {
        ThrowIfDisposed();
        return ZrcSdk_LeaveMeeting(_handle) == 0;
    }

    /// <summary>
    /// Sends the meeting password when prompted by the <see cref="MeetingNeedsPassword"/> event.
    /// </summary>
    /// <param name="password">The meeting password.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool SendMeetingPassword(string password)
    {
        ThrowIfDisposed();
        return ZrcSdk_SendMeetingPassword(_handle, password) == 0;
    }

    /// <summary>Cancels meeting password entry and aborts joining.</summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool CancelEnteringMeetingPassword()
    {
        ThrowIfDisposed();
        return ZrcSdk_CancelEnteringMeetingPassword(_handle) == 0;
    }

    /// <summary>Cancels waiting for the host and aborts joining.</summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool CancelWaitingForHost()
    {
        ThrowIfDisposed();
        return ZrcSdk_CancelWaitingForHost(_handle) == 0;
    }

    #endregion

    #region Audio / Video

    /// <summary>Sets the local audio mute state.</summary>
    /// <param name="mute"><see langword="true"/> to mute, <see langword="false"/> to unmute.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool SetAudioMute(bool mute)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetAudioMute(_handle, mute ? 1 : 0) == 0;
    }

    /// <summary>Starts or stops the local camera.</summary>
    /// <param name="start"><see langword="true"/> to start video, <see langword="false"/> to stop.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool SetVideoState(bool start)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetVideoState(_handle, start ? 1 : 0) == 0;
    }

    #endregion

    #region Cloud Recording

    /// <summary>Starts cloud recording. Must be host or have recording permission.</summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool StartRecording()
    {
        ThrowIfDisposed();
        return ZrcSdk_StartRecording(_handle) == 0;
    }

    /// <summary>Stops the active cloud recording.</summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool StopRecording()
    {
        ThrowIfDisposed();
        return ZrcSdk_StopRecording(_handle) == 0;
    }

    /// <summary>Pauses the active cloud recording.</summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool PauseRecording()
    {
        ThrowIfDisposed();
        return ZrcSdk_PauseRecording(_handle) == 0;
    }

    /// <summary>Resumes a paused cloud recording.</summary>
    /// <returns><see langword="true"/> on success.</returns>
    public bool ResumeRecording()
    {
        ThrowIfDisposed();
        return ZrcSdk_ResumeRecording(_handle) == 0;
    }

    #endregion

    #region Participants

    /// <summary>
    /// Returns the last-known participant count. This value is kept up to date by the
    /// <see cref="ParticipantCount"/> event.
    /// </summary>
    /// <returns>Current participant count, or 0 if not in a meeting.</returns>
    public int GetParticipantCount()
    {
        ThrowIfDisposed();
        return ZrcSdk_GetParticipantCount(_handle);
    }

    #endregion

    #region Zoom Room Control System (ZRCS)

    /// <summary>Returns <see langword="true"/> if the Zoom Room Control System (ZRCS) is enabled.</summary>
    public bool IsZRCSEnabled()
    {
        ThrowIfDisposed();
        return ZrcSdk_IsZRCSEnabled(_handle) == 1;
    }

    /// <summary>Opens or closes the room controls panel on the Zoom Room display.</summary>
    /// <param name="open"><see langword="true"/> to open, <see langword="false"/> to close.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool OpenRoomControls(bool open)
    {
        ThrowIfDisposed();
        return ZrcSdk_OpenRoomControls(_handle, open ? 1 : 0) == 0;
    }

    /// <summary>Sends a control command to a ZRCS-managed device.</summary>
    /// <param name="deviceID">The ZRCS device ID.</param>
    /// <param name="methodID">The method ID for the action.</param>
    /// <param name="paramID">The parameter ID.</param>
    /// <param name="value">The value to set.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool ControlZRCSDevice(string deviceID, string methodID, string paramID, string value)
    {
        ThrowIfDisposed();
        return ZrcSdk_ControlZRCSDevice(_handle, deviceID, methodID, paramID, value) == 0;
    }

    /// <summary>Executes a ZRCS automation scene by scene ID.</summary>
    /// <param name="sceneID">The ZRCS scene ID.</param>
    /// <returns><see langword="true"/> on success.</returns>
    public bool ExecuteZRCSScene(string sceneID)
    {
        ThrowIfDisposed();
        return ZrcSdk_ExecuteZRCSScene(_handle, sceneID) == 0;
    }

    #endregion

    #region Uninitialize

    /// <summary>
    /// Stops the HeartBeat loop and uninitializes the SDK on its owner thread.
    /// Equivalent to calling <see cref="Dispose"/>.
    /// </summary>
    public void Uninitialize()
    {
        if (!_disposed && _handle != IntPtr.Zero)
        {
            _sdkRunning = false;
            _sdkThread?.Join(TimeSpan.FromSeconds(2));
            _sdkThread = null;
        }
    }

    #endregion

    #region Callbacks

    private void OnInitializedCallback(string message, int errorCode, IntPtr userData) =>
        Initialized?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });

    private void OnMeetingStateChangedCallback(string message, int errorCode, IntPtr userData) =>
        ConnectionStateChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });

    private void OnErrorCallback(string message, int errorCode, IntPtr userData) =>
        Error?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });

    private void OnInstantMeetingStartedCallback(string meetingNumber, int result, IntPtr userData) =>
        InstantMeetingStarted?.Invoke(this, new SdkEventArgs { Message = meetingNumber, ErrorCode = result });

    private void OnPairRoomResultCallback(string message, int result, IntPtr userData) =>
        PairRoomResult?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = result });

    private void OnMeetingStatusCallback(string message, int statusCode, IntPtr userData) =>
        MeetingStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = statusCode });

    private void OnStartPmiResultCallback(string message, int result, IntPtr userData)
    {
        StartPmiResult?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = result });
        if (result == 0)
            InstantMeetingStarted?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = result });
    }

    private void OnExitMeetingCallback(string message, int errorCode, IntPtr userData) =>
        ExitMeeting?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });

    private void OnMeetingNeedsPasswordCallback(string message, int wrongAndRetry, IntPtr userData) =>
        MeetingNeedsPassword?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = wrongAndRetry });

    private void OnMeetingInviteCallback(string message, int errorCode, IntPtr userData) =>
        MeetingInvite?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });

    private void OnAudioStatusCallback(string message, int isMuted, IntPtr userData) =>
        AudioStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = isMuted });

    private void OnMuteOnEntryCallback(string message, int enabled, IntPtr userData) =>
        MuteOnEntry?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = enabled });

    private void OnParticipantCountCallback(string message, int count, IntPtr userData) =>
        ParticipantCount?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = count });

    private void OnHostChangedCallback(string message, int amIHost, IntPtr userData) =>
        HostChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = amIHost });

    private void OnRecordingStatusCallback(string message, int isRecording, IntPtr userData) =>
        RecordingStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = isRecording });

    private void OnControlSystemEnabledCallback(string message, int enabled, IntPtr userData) =>
        ControlSystemEnabled?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = enabled });

    #endregion

    #region IDisposable

    private void ThrowIfDisposed()
    {
        if (_disposed)
            throw new ObjectDisposedException(nameof(ZrcSdk));
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

    /// <summary>Finalizer — calls <see cref="Dispose(bool)"/> with <see langword="false"/>.</summary>
    ~ZrcSdk() => Dispose(false);

    /// <summary>
    /// Stops the HeartBeat loop, flushes SDK credentials to disk, and releases all native resources.
    /// </summary>
    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    #endregion
}

using System.Runtime.InteropServices;
using System.Text;
using System.Reflection;
using Path = System.IO.Path;

namespace ZrcSdkTest;

  /// <summary>
  /// P/Invoke wrapper for the native ZRC SDK
  /// </summary>
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

      // Keep references to prevent garbage collection
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

      // Events
      public event EventHandler<SdkEventArgs> Initialized;
      public event EventHandler<SdkEventArgs> ConnectionStateChanged;
      public event EventHandler<SdkEventArgs> Error;
      /// <summary>Fired when a meeting start attempt completes. Message=meetingNumber (may be empty), ErrorCode=result.</summary>
      public event EventHandler<SdkEventArgs> InstantMeetingStarted;
      public event EventHandler<SdkEventArgs> PairRoomResult;
      /// <summary>Meeting status changed. Message=status name, ErrorCode=MeetingStatus enum (0=NotInMeeting,1=Connecting,2=InMeeting,3=LoggedOut).</summary>
      public event EventHandler<SdkEventArgs> MeetingStatus;
      /// <summary>PMI/instant meeting started. Message=meetingNumber, ErrorCode=result.</summary>
      public event EventHandler<SdkEventArgs> StartPmiResult;
      /// <summary>Meeting exit notification. ErrorCode encodes result (low byte) and reason (high byte).</summary>
      public event EventHandler<SdkEventArgs> ExitMeeting;
      /// <summary>Meeting needs password. ErrorCode=1 if previous password was wrong.</summary>
      public event EventHandler<SdkEventArgs> MeetingNeedsPassword;
      /// <summary>Incoming meeting invite. Message=callerName.</summary>
      public event EventHandler<SdkEventArgs> MeetingInvite;
      /// <summary>Local audio mute state changed. ErrorCode=1 if muted.</summary>
      public event EventHandler<SdkEventArgs> AudioStatus;
      /// <summary>Mute-on-entry setting changed. ErrorCode=1 if enabled.</summary>
      public event EventHandler<SdkEventArgs> MuteOnEntry;
      /// <summary>Participant count changed. ErrorCode=current count.</summary>
      public event EventHandler<SdkEventArgs> ParticipantCount;
      /// <summary>Host changed. ErrorCode=1 if self is now host.</summary>
      public event EventHandler<SdkEventArgs> HostChanged;
      /// <summary>Recording status changed. ErrorCode=1 if meeting is being recorded.</summary>
      public event EventHandler<SdkEventArgs> RecordingStatus;
      /// <summary>Control system enabled state changed. ErrorCode=1 if enabled.</summary>
      public event EventHandler<SdkEventArgs> ControlSystemEnabled;

      #region Native Imports

      // Use library name without path - resolver will find it
      private const string DllName = "zrcsdkwrapperpdt";

      // Cache the wrapper handle to prevent multiple loads per assembly (DllImportResolver fires per P/Invoke call)
      private static IntPtr _cachedWrapperHandle = IntPtr.Zero;

      // dlopen constants for Linux
      private const int RTLD_NOW = 2;
      private const int RTLD_LAZY = 1;
      private const int RTLD_GLOBAL = 0x100;

      // memfd_create flags
      private const uint MFD_CLOEXEC = 1;

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

      // ARM32 syscall number for memfd_create
      private const int SYS_memfd_create = 385;

      // Static constructor to set up native library resolver
      static ZrcSdk()
      {
          // Preload ZRC SDK library with RTLD_GLOBAL so wrapper can find it
          try
          {
              // Clear any previous errors
              dlerror();
              
              // Use just the library name - the dynamic linker will find it via ld.so.cache
              var handle = dlopen("libZRCSdk.so", RTLD_NOW | RTLD_GLOBAL);
              if (handle == IntPtr.Zero)
              {
                  // Get the error message
                  var errorPtr = dlerror();
                  var errorMsg = errorPtr != IntPtr.Zero ? Marshal.PtrToStringAnsi(errorPtr) : "Unknown error";
                  throw new InvalidOperationException($"Failed to load libZRCSdk.so: {errorMsg}");
              }
          }
          catch (Exception ex)
          {
              // Log but don't throw - let it fail later with more context
              Console.WriteLine($"Warning: Could not preload ZRC SDK: {ex.Message}");
          }

          NativeLibrary.SetDllImportResolver(typeof(ZrcSdk).Assembly, DllImportResolver);
      }

      private static IntPtr DllImportResolver(string libraryName, Assembly assembly, DllImportSearchPath? searchPath)
      {
          if (libraryName == DllName)
          {
              // Return cached handle if already loaded
              if (_cachedWrapperHandle != IntPtr.Zero)
                  return _cachedWrapperHandle;

              // Try loading from Crestron application directory, fall back to /usr/lib
              var appDir = Crestron.SimplSharp.CrestronIO.Directory.GetApplicationDirectory();
              var libPath = Path.Combine(appDir, "libzrcsdkwrapperpdt.so");

              if (!System.IO.File.Exists(libPath))
              {
                  libPath = "/usr/lib/libzrcsdkwrapperpdt.so";
              }

              if (!System.IO.File.Exists(libPath))
              {
                  throw new DllNotFoundException($"libzrcsdkwrapperpdt.so not found in app directory or /usr/lib");
              }

              // All writable paths (/simpl/app, /var/volatile, /user) are mounted noexec.
              // Use memfd_create to load the library into an anonymous memory fd,
              // then dlopen via /proc/self/fd/<n> which is never noexec.
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
                      var errorPtr = dlerror();
                      var errorMsg = errorPtr != IntPtr.Zero ? Marshal.PtrToStringAnsi(errorPtr) : "Unknown error";
                      throw new DllNotFoundException($"Failed to dlopen via memfd ({procPath}): {errorMsg}");
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
          return IntPtr.Zero;
      }

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
      private static extern void ZrcSdk_SetInitializedCallback(
          IntPtr handle, 
          SdkEventCallbackDelegate callback, 
          IntPtr userData);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern void ZrcSdk_SetConnectionStateChangedCallback(
          IntPtr handle, 
          SdkEventCallbackDelegate callback, 
          IntPtr userData);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern void ZrcSdk_SetErrorCallback(
          IntPtr handle, 
          SdkEventCallbackDelegate callback, 
          IntPtr userData);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_StartInstantMeeting(IntPtr handle);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern void ZrcSdk_SetInstantMeetingStartedCallback(
          IntPtr handle,
          SdkEventCallbackDelegate callback,
          IntPtr userData);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern void ZrcSdk_SetPairRoomResultCallback(
          IntPtr handle,
          SdkEventCallbackDelegate callback,
          IntPtr userData);

      // ── Pre-meeting / connection ──────────────────────────────────────────────
      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_GetConnectionState(IntPtr handle);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_WakeZoomRoomUp(IntPtr handle);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_LogoutZoomRoomDevice(IntPtr handle);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_RestartZoomRoomOS(IntPtr handle);

      // ── Meeting join / exit ───────────────────────────────────────────────────
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

      // ── Recording ─────────────────────────────────────────────────────────────
      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_StartRecording(IntPtr handle);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_StopRecording(IntPtr handle);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_PauseRecording(IntPtr handle);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_ResumeRecording(IntPtr handle);

      // ── Participants ──────────────────────────────────────────────────────────
      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_GetParticipantCount(IntPtr handle);

      // ── Control System ────────────────────────────────────────────────────────
      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_IsZRCSEnabled(IntPtr handle);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
      private static extern int ZrcSdk_OpenRoomControls(IntPtr handle, int open);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
      private static extern int ZrcSdk_ControlZRCSDevice(IntPtr handle, string deviceID, string methodID, string paramID, string value);

      [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
      private static extern int ZrcSdk_ExecuteZRCSScene(IntPtr handle, string sceneID);

      // ── New event callback setters ────────────────────────────────────────────
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
      /// Creates a new instance of the ZRC SDK wrapper
      /// </summary>
      public ZrcSdk()
      {
          _handle = ZrcSdk_Create();
          if (_handle == IntPtr.Zero)
          {
              throw new InvalidOperationException("Failed to create ZRC SDK instance");
          }

          // Set up callback delegates
          _initializedCallbackDelegate = OnInitializedCallback;
          _meetingStateChangedCallbackDelegate = OnMeetingStateChangedCallback;
          _errorCallbackDelegate = OnErrorCallback;
          _instantMeetingStartedCallbackDelegate = OnInstantMeetingStartedCallback;
          _pairRoomResultCallbackDelegate = OnPairRoomResultCallback;
          _meetingStatusCallbackDelegate = OnMeetingStatusCallback;
          _startPmiResultCallbackDelegate = OnStartPmiResultCallback;
          _exitMeetingCallbackDelegate = OnExitMeetingCallback;
          _meetingNeedsPasswordCallbackDelegate = OnMeetingNeedsPasswordCallback;
          _meetingInviteCallbackDelegate = OnMeetingInviteCallback;
          _audioStatusCallbackDelegate = OnAudioStatusCallback;
          _muteOnEntryCallbackDelegate = OnMuteOnEntryCallback;
          _participantCountCallbackDelegate = OnParticipantCountCallback;
          _hostChangedCallbackDelegate = OnHostChangedCallback;
          _recordingStatusCallbackDelegate = OnRecordingStatusCallback;
          _controlSystemEnabledCallbackDelegate = OnControlSystemEnabledCallback;

          ZrcSdk_SetInitializedCallback(_handle, _initializedCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetConnectionStateChangedCallback(_handle, _meetingStateChangedCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetErrorCallback(_handle, _errorCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetInstantMeetingStartedCallback(_handle, _instantMeetingStartedCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetPairRoomResultCallback(_handle, _pairRoomResultCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetMeetingStatusCallback(_handle, _meetingStatusCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetStartPmiResultCallback(_handle, _startPmiResultCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetExitMeetingCallback(_handle, _exitMeetingCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetMeetingNeedsPasswordCallback(_handle, _meetingNeedsPasswordCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetMeetingInviteCallback(_handle, _meetingInviteCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetAudioStatusCallback(_handle, _audioStatusCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetMuteOnEntryCallback(_handle, _muteOnEntryCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetParticipantCountCallback(_handle, _participantCountCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetHostChangedCallback(_handle, _hostChangedCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetRecordingStatusCallback(_handle, _recordingStatusCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetControlSystemEnabledCallback(_handle, _controlSystemEnabledCallbackDelegate, IntPtr.Zero);
      }

      /// <summary>
      /// Initializes the SDK with the specified configuration path.
      /// Initialize() and HeartBeat() both run on a dedicated thread to match the SDK's
      /// single-threaded libuv event loop requirement.
      /// </summary>
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
          _initDone.Wait();   // block until ZrcSdk_Initialize returns
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
              // Drive the SDK event loop from this thread — same thread that called CreateInstance()
              while (_sdkRunning)
              {
                  Thread.Sleep(150);
                  if (_handle != IntPtr.Zero && _sdkRunning)
                      ZrcSdk_HeartBeat(_handle);
              }
              // Uninitialize on the SDK owner thread for clean DestroyInstance() shutdown
              ZrcSdk_Uninitialize(_handle);
          }
      }

      /// <summary>
      /// Gets the SDK version
      /// </summary>
      public string GetSDKVersion()
      {
          ThrowIfDisposed();
          var buffer = new StringBuilder(256);
          ZrcSdk_GetSDKVersion(_handle, buffer, buffer.Capacity);
          return buffer.ToString();
      }

      /// <summary>
      /// Pairs the room with an activation code
      /// </summary>
      public bool PairRoomWithActivationCode(string activationCode)
      {
          ThrowIfDisposed();
          int result = ZrcSdk_PairRoomWithActivationCode(_handle, activationCode);
          return result == 0;
      }

      /// <summary>
      /// Returns true if the SDK has stored credentials from a previous pairing and can reconnect without an activation code.
      /// </summary>
      public bool CanRetryToPairLastRoom()
      {
          ThrowIfDisposed();
          return ZrcSdk_CanRetryToPairLastRoom(_handle) == 1;
      }

      /// <summary>
      /// Reconnects to the last paired Zoom Room using stored credentials. No activation code required.
      /// The PairRoomResult event fires with the result.
      /// </summary>
      public bool RetryToPairRoom()
      {
          ThrowIfDisposed();
          return ZrcSdk_RetryToPairRoom(_handle) == 0;
      }

      /// <summary>
      /// Unpairs the SDK from the current Zoom Room and clears stored credentials.
      /// </summary>
      public bool UnpairRoom()
      {
          ThrowIfDisposed();
          return ZrcSdk_UnpairRoom(_handle) == 0;
      }

      /// <summary>
      /// Starts a meeting with the specified meeting number
      /// </summary>
      public bool StartMeeting(string meetingNumber)
      {
          ThrowIfDisposed();
          int result = ZrcSdk_StartMeeting(_handle, meetingNumber);
          return result == 0;
      }

      /// <summary>
      /// Ends the current meeting
      /// </summary>
      public bool EndMeeting()
      {
          ThrowIfDisposed();
          int result = ZrcSdk_EndMeeting(_handle);
          return result == 0;
      }

      /// <summary>
      /// Starts an instant (PMI) meeting. The meeting number is returned via the InstantMeetingStarted event.
      /// </summary>
      public bool StartInstantMeeting()
      {
          ThrowIfDisposed();
          int result = ZrcSdk_StartInstantMeeting(_handle);
          return result == 0;
      }

      /// <summary>
      /// Sets the audio mute state
      /// </summary>
      public bool SetAudioMute(bool mute)
      {
          ThrowIfDisposed();
          int result = ZrcSdk_SetAudioMute(_handle, mute ? 1 : 0);
          return result == 0;
      }

      /// <summary>
      /// Sets the video state
      /// </summary>
      public bool SetVideoState(bool start)
      {
          ThrowIfDisposed();
          int result = ZrcSdk_SetVideoState(_handle, start ? 1 : 0);
          return result == 0;
      }

      // ── Pre-meeting / connection ───────────────────────────────────────────────

      /// <summary>Returns the current ConnectionState (0=Established, 1=Connected, 2=Disconnected, -1=None/error).</summary>
      public int GetConnectionState()
      {
          ThrowIfDisposed();
          return ZrcSdk_GetConnectionState(_handle);
      }

      public bool WakeZoomRoomUp()
      {
          ThrowIfDisposed();
          return ZrcSdk_WakeZoomRoomUp(_handle) == 0;
      }

      public bool LogoutZoomRoomDevice()
      {
          ThrowIfDisposed();
          return ZrcSdk_LogoutZoomRoomDevice(_handle) == 0;
      }

      public bool RestartZoomRoomOS()
      {
          ThrowIfDisposed();
          return ZrcSdk_RestartZoomRoomOS(_handle) == 0;
      }

      // ── Meeting join / exit ────────────────────────────────────────────────────

      /// <summary>Join a meeting by meeting number or personal link name.</summary>
      public bool JoinMeeting(string meetingNumber)
      {
          ThrowIfDisposed();
          return ZrcSdk_JoinMeeting(_handle, meetingNumber) == 0;
      }

      /// <summary>Join a meeting by URL.</summary>
      public bool JoinMeetingWithURL(string url)
      {
          ThrowIfDisposed();
          return ZrcSdk_JoinMeetingWithURL(_handle, url) == 0;
      }

      /// <summary>Start a scheduled meeting using the host key.</summary>
      public bool StartMeetingWithHostKey(string hostKey)
      {
          ThrowIfDisposed();
          return ZrcSdk_StartMeetingWithHostKey(_handle, hostKey) == 0;
      }

      /// <summary>Leave the meeting (self only). Host leaving does not end the meeting.</summary>
      public bool LeaveMeeting()
      {
          ThrowIfDisposed();
          return ZrcSdk_LeaveMeeting(_handle) == 0;
      }

      /// <summary>Send the meeting password when prompted.</summary>
      public bool SendMeetingPassword(string password)
      {
          ThrowIfDisposed();
          return ZrcSdk_SendMeetingPassword(_handle, password) == 0;
      }

      /// <summary>Cancel the meeting password entry and abort joining.</summary>
      public bool CancelEnteringMeetingPassword()
      {
          ThrowIfDisposed();
          return ZrcSdk_CancelEnteringMeetingPassword(_handle) == 0;
      }

      /// <summary>Cancel waiting for host and abort joining.</summary>
      public bool CancelWaitingForHost()
      {
          ThrowIfDisposed();
          return ZrcSdk_CancelWaitingForHost(_handle) == 0;
      }

      // ── Cloud Recording ────────────────────────────────────────────────────────

      public bool StartRecording()
      {
          ThrowIfDisposed();
          return ZrcSdk_StartRecording(_handle) == 0;
      }

      public bool StopRecording()
      {
          ThrowIfDisposed();
          return ZrcSdk_StopRecording(_handle) == 0;
      }

      public bool PauseRecording()
      {
          ThrowIfDisposed();
          return ZrcSdk_PauseRecording(_handle) == 0;
      }

      public bool ResumeRecording()
      {
          ThrowIfDisposed();
          return ZrcSdk_ResumeRecording(_handle) == 0;
      }

      // ── Participants ───────────────────────────────────────────────────────────

      /// <summary>Returns the last-known participant count (updated by the ParticipantCount event).</summary>
      public int GetParticipantCount()
      {
          ThrowIfDisposed();
          return ZrcSdk_GetParticipantCount(_handle);
      }

      // ── Control System (ZRCS) ──────────────────────────────────────────────────

      /// <summary>Returns true if the Zoom Room Control System is enabled.</summary>
      public bool IsZRCSEnabled()
      {
          ThrowIfDisposed();
          return ZrcSdk_IsZRCSEnabled(_handle) == 1;
      }

      /// <summary>Open or close the room controls panel on the Zoom Room.</summary>
      public bool OpenRoomControls(bool open)
      {
          ThrowIfDisposed();
          return ZrcSdk_OpenRoomControls(_handle, open ? 1 : 0) == 0;
      }

      /// <summary>Send a control command to a ZRCS device.</summary>
      public bool ControlZRCSDevice(string deviceID, string methodID, string paramID, string value)
      {
          ThrowIfDisposed();
          return ZrcSdk_ControlZRCSDevice(_handle, deviceID, methodID, paramID, value) == 0;
      }

      /// <summary>Execute a ZRCS scene by scene ID.</summary>
      public bool ExecuteZRCSScene(string sceneID)
      {
          ThrowIfDisposed();
          return ZrcSdk_ExecuteZRCSScene(_handle, sceneID) == 0;
      }

      /// <summary>
      /// Uninitializes the SDK
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

      #region Callback Methods

      private void OnInitializedCallback(string message, int errorCode, IntPtr userData)
      {
          Initialized?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });
      }

      private void OnMeetingStateChangedCallback(string message, int errorCode, IntPtr userData)
      {
          ConnectionStateChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });
      }

      private void OnErrorCallback(string message, int errorCode, IntPtr userData)
      {
          Error?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });
      }

      private void OnInstantMeetingStartedCallback(string meetingNumber, int result, IntPtr userData)
      {
          Console.WriteLine($"Instant meeting started - Meeting number: {meetingNumber}, Result: {result}");
          InstantMeetingStarted?.Invoke(this, new SdkEventArgs { Message = meetingNumber, ErrorCode = result });
      }

      private void OnPairRoomResultCallback(string message, int result, IntPtr userData)
      {
          var status = result == 0 ? "success" : $"failed (code {result})";
          Console.WriteLine($"Pair room result: {status}");
          PairRoomResult?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = result });
      }

      private void OnMeetingStatusCallback(string message, int statusCode, IntPtr userData)
      {
          Console.WriteLine($"Meeting status: {message} ({statusCode})");
          MeetingStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = statusCode });
      }

      private void OnStartPmiResultCallback(string message, int result, IntPtr userData)
      {
          Console.WriteLine($"PMI meeting started: number={message}, result={result}");
          StartPmiResult?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = result });
          // Also fire the legacy event so existing handlers work
          if (result == 0)
              InstantMeetingStarted?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = result });
      }

      private void OnExitMeetingCallback(string message, int errorCode, IntPtr userData)
      {
          Console.WriteLine($"Exit meeting: result={errorCode & 0xFF}, reason={(errorCode >> 8) & 0xFF}");
          ExitMeeting?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });
      }

      private void OnMeetingNeedsPasswordCallback(string message, int wrongAndRetry, IntPtr userData)
      {
          Console.WriteLine($"Meeting needs password (wrongAndRetry={wrongAndRetry})");
          MeetingNeedsPassword?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = wrongAndRetry });
      }

      private void OnMeetingInviteCallback(string message, int errorCode, IntPtr userData)
      {
          Console.WriteLine($"Meeting invite from: {message}");
          MeetingInvite?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });
      }

      private void OnAudioStatusCallback(string message, int isMuted, IntPtr userData)
      {
          Console.WriteLine($"Audio status: muted={isMuted}");
          AudioStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = isMuted });
      }

      private void OnMuteOnEntryCallback(string message, int enabled, IntPtr userData)
      {
          Console.WriteLine($"Mute on entry: {enabled}");
          MuteOnEntry?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = enabled });
      }

      private void OnParticipantCountCallback(string message, int count, IntPtr userData)
      {
          Console.WriteLine($"Participant count: {count}");
          ParticipantCount?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = count });
      }

      private void OnHostChangedCallback(string message, int amIHost, IntPtr userData)
      {
          Console.WriteLine($"Host changed: amIHost={amIHost}");
          HostChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = amIHost });
      }

      private void OnRecordingStatusCallback(string message, int isRecording, IntPtr userData)
      {
          Console.WriteLine($"Recording status: {isRecording}");
          RecordingStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = isRecording });
      }

      private void OnControlSystemEnabledCallback(string message, int enabled, IntPtr userData)
      {
          Console.WriteLine($"Control system enabled: {enabled}");
          ControlSystemEnabled?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = enabled });
      }

      #endregion

      #region IDisposable Implementation

      private void ThrowIfDisposed()
      {
          if (_disposed)
          {
              throw new ObjectDisposedException(nameof(ZrcSdk));
          }
      }

      protected virtual void Dispose(bool disposing)
      {
          if (!_disposed)
          {
              // Signal SDK thread to stop and call ZrcSdk_Uninitialize on its own thread
              _sdkRunning = false;
              if (disposing)
              {
                  _sdkThread?.Join(TimeSpan.FromSeconds(2));
                  _sdkThread = null;
              }

              // Clean up unmanaged instance (sinks + handle); Uninitialize was called on SDK thread
              if (_handle != IntPtr.Zero)
              {
                  ZrcSdk_Destroy(_handle);
                  _handle = IntPtr.Zero;
              }

              _disposed = true;
          }
      }

      ~ZrcSdk()
      {
          Dispose(false);
      }

      public void Dispose()
      {
          Dispose(true);
          GC.SuppressFinalize(this);
      }

      #endregion
  }

  /// <summary>
  /// Event arguments for SDK events
  /// </summary>
  public class SdkEventArgs : EventArgs
  {
      public string Message { get; set; }
      public int ErrorCode { get; set; }
  }

  /// <summary>
  /// Translates ZRC SDK integer codes to human-readable strings.
  /// Values sourced directly from ZRCSDKTypes.h and IMeetingService.h.
  /// </summary>
  public static class ZrcSdkCodes
  {
      /// <summary>
      /// Translates a ZRCSDKError value to a description string.
      /// This is the general error code returned by most API calls.
      /// </summary>
      public static string GetErrorDescription(int code) => code switch
      {
          // General
          0   => "Success",
          1   => "Internal error",
          2   => "Service not initialized",
          3   => "No permission",
          4   => "Feature disabled",
          5   => "Zoom Room has no capability for this action",
          6   => "Invalid parameter",
          7   => "Evaluation build expired",
          8   => "API not supported in webinar",
          9   => "Wrong API usage",
          10  => "Already in this state",
          11  => "Not connected to Zoom Room",
          12  => "Host has no capability to respond",
          13  => "Host not in meeting",
          14  => "Cannot perform action (ZR in special mode)",
          15  => "API not supported in webinar backstage",
          16  => "Request already responded",
          17  => "Settings locked on web",
          // API context restrictions
          50  => "API not supported in normal meeting",
          51  => "API not supported in breakout room",
          52  => "API not supported in webinar breakout room",
          53  => "API not supported in webinar debrief session",
          54  => "API not supported while switching meeting",
          55  => "API not supported in simulive webinar",
          // Device
          100 => "Device does not exist",
          101 => "Microphone is disabled",
          102 => "Camera is disabled",
          103 => "No CEC screen",
          104 => "Microphone audio is muted",
          105 => "No local network audio device",
          106 => "Zoom Room is not an IoT device",
          107 => "Device does not support identify",
          108 => "Network audio device channel not found",
          109 => "Device not connected",
          110 => "Selected microphone is a stethoscope",
          111 => "Device has no capability",
          112 => "Microphone is troubleshooting",
          // Camera
          120 => "Camera cannot be controlled",
          121 => "Camera cannot move",
          122 => "Camera cannot zoom",
          123 => "Camera cannot switch",
          124 => "Camera cannot change pan/tilt speed",
          125 => "Camera does not support smart mode",
          126 => "Camera is not active",
          127 => "Camera control is not enabled",
          128 => "Camera is not sharing",
          129 => "Camera does not support preset",
          130 => "Speaker switching not supported",
          131 => "Companion ZR camera cannot be enabled",
          132 => "Director calibration not supported",
          133 => "Camera cannot director calibrate",
          134 => "Camera does not support calibration",
          135 => "Calibration in wrong order",
          136 => "Multi-camera not enabled",
          137 => "Max multi-camera count reached",
          138 => "Camera cannot be selected for multi-camera",
          139 => "Camera not found",
          140 => "Camera cannot set COM ID",
          141 => "Camera cannot set display name",
          142 => "Camera boundary not supported",
          143 => "Camera boundary configuration in wrong order",
          144 => "NDI camera is being shared and cannot be selected",
          // Virtual Background
          180 => "Cannot change virtual background — setting locked on web",
          181 => "Cannot change virtual background — in immersive cutout",
          182 => "Cannot set virtual background to none — force-enabled on web",
          183 => "Blur algorithm not supported",
          // Meeting
          200 => "Invalid meeting",
          201 => "Invalid meeting number",
          202 => "Not a third-party meeting",
          210 => "No unencrypted attendee in webinar",
          211 => "Not an AES-256-GCM encrypted meeting",
          212 => "Meeting has third-party audio",
          220 => "Already in a meeting",
          221 => "Not in a meeting",
          222 => "Not in a webinar",
          223 => "Not in a PSTN call-out meeting",
          224 => "Not in an end-to-end encrypted meeting",
          225 => "Not in an integration meeting",
          226 => "Not in webinar practice session",
          227 => "Meeting does not support waiting room",
          228 => "Meeting waiting room is locked",
          229 => "Cannot join via web client",
          230 => "In an end-to-end encrypted meeting",
          231 => "In waiting room",
          232 => "In webinar backstage debrief session",
          233 => "Self or user not in proper webinar position",
          234 => "Meeting chat not supported or enabled",
          235 => "Chat privilege control not supported",
          236 => "Incoming meeting call does not exist",
          237 => "Cannot join via SIP",
          238 => "Cannot join via phone",
          239 => "Current integration meeting not supported",
          // View Layout
          250 => "View layout following host order",
          251 => "Screen cannot be pinned",
          252 => "User video already pinned on screen",
          253 => "User already spotlighted",
          254 => "Meeting does not support multi-pin",
          255 => "No user pinned",
          256 => "Max pinned users reached",
          257 => "No user spotlighted",
          258 => "Max spotlighted users reached",
          260 => "Not enough users for spotlight",
          261 => "NDI not enabled",
          262 => "NDI source already pinned",
          263 => "Not in dynamic layout",
          264 => "No share source while in live feed",
          265 => "Cannot adjust screen — only one screen available",
          266 => "Cannot use main screen as confidence monitor",
          267 => "Screen position adjustment out of range",
          268 => "NDI index not persisted",
          279 => "Wrong number of spotlighted users",
          // Share
          300 => "Meeting share is disabled",
          301 => "Not sharing",
          302 => "Blackmagic not sharing",
          303 => "Blackmagic not viewing locally",
          304 => "Blackmagic share data not ready",
          305 => "Blackmagic local view data not ready",
          306 => "No camera is sharing",
          307 => "Pin share needs double-confirmation",
          308 => "Share audio control not supported on single screen",
          309 => "Share has no audio",
          310 => "Feature disabled while in multi-share",
          311 => "Screen cannot pin share",
          312 => "Cannot pin backstage share in webinar",
          313 => "NDI camera is selected and cannot be shared",
          314 => "Share cannot be pinned on confidence monitor screen",
          // Recording
          350 => "Recording not available in current meeting",
          351 => "Must confirm recording disclaimer before starting",
          352 => "Must set recording notification email before starting",
          353 => "No need to prompt recording disclaimer",
          // User
          400 => "Participant not found by user ID",
          401 => "User info not found by GUID",
          402 => "Multi-stream video user — action not supported",
          403 => "Phone call-in user — action not supported",
          404 => "User is a panelist",
          405 => "User is an attendee",
          406 => "User is in backstage",
          407 => "User cannot be allowed to record",
          408 => "User cannot be allowed to multi-pin",
          409 => "User cannot be spotlighted",
          410 => "User cannot be add-pinned",
          411 => "User cannot be add-spotlighted",
          412 => "User is a simulive user",
          413 => "User action not supported",
          // Caption / Transcription
          450 => "Closed caption not available",
          451 => "Closed caption disabled — use new LTT caption",
          452 => "Live transcription not enabled",
          453 => "Live transcription disabled — use new LTT caption",
          460 => "New LTT caption not enabled",
          461 => "New LTT translated caption not enabled",
          462 => "New LTT caption speaking language not available",
          463 => "New LTT caption translation language not available",
          // Breakout Room
          501 => "No permission to manage breakout room",
          502 => "Cannot create breakout room after it has started",
          503 => "Cannot edit breakout room after it has started",
          504 => "Breakout room count exceeds max",
          505 => "Web pre-assign breakout room not enabled",
          506 => "Broadcast message to breakout room is disabled",
          507 => "Cannot delete main session",
          508 => "No breakout rooms added",
          509 => "Breakout room not found by session ID",
          511 => "Breakout room not in progress",
          514 => "No permission to join unassigned breakout room",
          515 => "No permission to leave breakout room",
          516 => "Not in a breakout room",
          517 => "Not in main session",
          518 => "You are the breakout room manager",
          519 => "User not assigned to any breakout room",
          520 => "Breakout room is stopping",
          521 => "Breakout room at capacity",
          // BYOD
          550 => "BYOD mode not supported",
          551 => "BYOD camera control not supported",
          552 => "BYOD microphone control not supported",
          553 => "BYOD speaker control not supported",
          554 => "BYOD auto framing not supported",
          555 => "BYOD mode does not support manual operation",
          556 => "Not in correct BYOD status",
          557 => "BYOD HDMI share not supported or not ready",
          // Polling
          600 => "Polling session is inactive",
          601 => "Another device already has polling control",
          602 => "Not the polling in-charge device",
          603 => "Invalid polling question type",
          604 => "Polling question ID missing",
          605 => "Polling question index missing",
          606 => "Polling question answers missing",
          607 => "Polling answer ID missing",
          608 => "Polling answers count error",
          609 => "Single-choice question allows only one answer",
          610 => "Multi-choice question requires at least one answer",
          611 => "Polling sub-questions missing",
          612 => "Polling sub-question ID missing",
          613 => "Polling sub-question index missing",
          614 => "Polling sub-question answers missing",
          615 => "Polling sub-question answer ID missing",
          616 => "Polling sub-question answers count error",
          617 => "Polling sub-question allows only one answer",
          618 => "Text answer is empty",
          619 => "Text answer below minimum length",
          620 => "Text answer exceeds maximum length",
          621 => "Not all blanks filled in",
          622 => "ZR does not support showing poll overlay",
          623 => "Show poll overlay not enabled",
          // SIP
          650 => "Incoming call not in incoming calls list",
          651 => "Already have a SIP call",
          652 => "No SIP call currently",
          653 => "SIP call not in SIP calls list",
          654 => "SIP call not in correct state",
          655 => "Foreground call is not in-call",
          656 => "Not in SIP call activity",
          657 => "SIP service not available",
          658 => "Zoom Phone not available",
          659 => "Number of calls exceeds limit",
          660 => "Already have a merged call",
          661 => "This call cannot be merged",
          662 => "Call is transferring",
          663 => "Already have a warm-transfer call",
          664 => "Cannot transfer a merged call",
          665 => "Call has no related call ID",
          666 => "Related call does not exist",
          667 => "No foreground call",
          668 => "Call is an emergency call",
          669 => "Cannot change location permission during meeting",
          670 => "No emergency info",
          671 => "No unhold call",
          672 => "Not all SIP calls are in-call",
          // AI Companion
          700 => "AI Companion meeting summary disabled",
          701 => "AI Companion meeting query disabled",
          702 => "AI Companion: no privilege setting item",
          703 => "AI Companion: invalid privilege setting item",
          704 => "AI Companion: meeting summary notification email already set",
          705 => "AI Companion: meeting summary notification email not set",
          // Meeting Chat
          750 => "Meeting chat disabled",
          751 => "Not using new meeting chat experience",
          752 => "Must confirm CMC tip",
          753 => "No need to confirm CMC tip",
          754 => "Message content is empty",
          755 => "Message content exceeds max size",
          756 => "Chat thread not found",
          757 => "Chat message not found",
          758 => "Emoji not found",
          759 => "Cannot send message",
          760 => "Cannot reply to thread",
          761 => "Cannot delete message",
          762 => "Cannot edit message",
          763 => "Cannot react to message with emoji",
          764 => "Please use new chat APIs",
          765 => "Reply type must match thread type",
          766 => "Private chat user ID error",
          767 => "Can only react with selected emojis",
          // HWIO
          780 => "HWIO service unavailable",
          // Q&A
          800 => "Not the main controller",
          801 => "Not allowed to send question",
          802 => "Not allowed to send anonymous question",
          803 => "Not allowed to answer question",
          804 => "Not allowed to comment on question",
          805 => "Not allowed to upvote question",
          // Pro AV
          830 => "NDI and HWIO both disabled",
          // Reaction
          850 => "Reactions disabled",
          // Smart Tag
          870 => "Smart tag disabled",
          871 => "Smart tag not supported",
          _   => $"Unknown error ({code})"
      };

      /// <summary>
      /// Translates an OnPairRoomResult result code.
      /// These are NOT ZRCSDKError values — they are pair-specific codes returned by the Zoom server.
      /// </summary>
      public static string GetPairRoomResultDescription(int code) => code switch
      {
          0        => "Success",
          100      => "Failed to connect to room",
          101      => "Room cannot verify connection",
          102      => "Timeout waiting for room verify response",
          5002     => "Stored credentials rejected by server — token may have expired or room was re-provisioned",
          30055016 => "Invalid activation code",
          _        => $"Pairing failed (server code {code})"
      };

      /// <summary>
      /// Translates a MeetingStatus value (from the MeetingStatus event ErrorCode).
      /// 0=NotInMeeting, 1=ConnectingToMeeting, 2=InMeeting, 3=LoggedOut
      /// </summary>
      public static string GetMeetingStatusDescription(int code) => code switch
      {
          0 => "Not in meeting",
          1 => "Connecting to meeting",
          2 => "In meeting",
          3 => "Logged out",
          _ => $"Unknown meeting status ({code})"
      };

      /// <summary>
      /// Translates a ConnectionState value (from GetConnectionState()).
      /// -1=None, 0=Established, 1=Connected, 2=Disconnected
      /// </summary>
      public static string GetConnectionStateDescription(int code) => code switch
      {
          -1 => "None (invalid)",
          0  => "Established",
          1  => "Connected and verified",
          2  => "Disconnected",
          _  => $"Unknown connection state ({code})"
      };

      /// <summary>
      /// Translates the exit reason portion of the ExitMeeting event's ErrorCode.
      /// The reason is in the upper byte: (errorCode >> 8) &amp; 0xFF.
      /// </summary>
      public static string GetExitMeetingReasonDescription(int reason) => reason switch
      {
          0 => "Default",
          1 => "Joining breakout room",
          2 => "Leaving breakout room",
          3 => "Rejoining new meeting",
          _ => $"Unknown exit reason ({reason})"
      };

      /// <summary>
      /// Translates a WaitingHostReason value from the MeetingStateChanged event.
      /// 0=WaitingForHostToStart, 1=WaitingForHostToEndAnotherMeeting
      /// </summary>
      public static string GetWaitingHostReasonDescription(int code) => code switch
      {
          0 => "Waiting for host to start meeting",
          1 => "Waiting for host to end another meeting",
          _ => $"Unknown waiting reason ({code})"
      };

      /// <summary>
      /// Translates a RoomUnpairedReason value.
      /// Fired when the device becomes unpaired from the Zoom Room.
      /// </summary>
      public static string GetRoomUnpairedReasonDescription(int code) => code switch
      {
          1 => "Token is invalid",
          2 => "Internal token refresh failed",
          3 => "Device removed by admin",
          4 => "Evaluation build expired",
          5 => "Zoom Room is in E2E-encrypted meeting",
          _ => $"Unknown unpaired reason ({code})"
      };
  }

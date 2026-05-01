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
      private System.Threading.Timer? _heartbeatTimer;

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

      // Events
      public event EventHandler<SdkEventArgs> Initialized;
      public event EventHandler<SdkEventArgs> MeetingStateChanged;
      public event EventHandler<SdkEventArgs> Error;
      public event EventHandler<SdkEventArgs> InstantMeetingStarted;
      public event EventHandler<SdkEventArgs> PairRoomResult;

      #region Native Imports

      // Use library name without path - resolver will find it
      private const string DllName = "zrcsdkwrapperpdt";

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

      // Cached handle — the resolver is called once per P/Invoke call, not once per library.
      // Without caching, each call creates a new memfd and loads a fresh library instance.
      private static IntPtr _cachedWrapperHandle = IntPtr.Zero;

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
      private static extern void ZrcSdk_SetMeetingStateChangedCallback(
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

          ZrcSdk_SetInitializedCallback(_handle, _initializedCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetMeetingStateChangedCallback(_handle, _meetingStateChangedCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetErrorCallback(_handle, _errorCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetInstantMeetingStartedCallback(_handle, _instantMeetingStartedCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetPairRoomResultCallback(_handle, _pairRoomResultCallbackDelegate, IntPtr.Zero);
      }

      /// <summary>
      /// Initializes the SDK with the specified configuration path
      /// </summary>
      public bool Initialize(string configPath)
      {
          ThrowIfDisposed();
          int result = ZrcSdk_Initialize(_handle, configPath);
          if (result == 0)
          {
              // Pump the SDK event loop every 150ms — required on Linux for any callbacks to fire
              _heartbeatTimer = new System.Threading.Timer(
                  _ => ZrcSdk_HeartBeat(_handle),
                  null,
                  dueTime: System.TimeSpan.FromMilliseconds(150),
                  period: System.TimeSpan.FromMilliseconds(150));
          }
          return result == 0;
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

      /// <summary>
      /// Uninitializes the SDK
      /// </summary>
      public void Uninitialize()
      {
          if (!_disposed && _handle != IntPtr.Zero)
          {
              _heartbeatTimer?.Dispose();
              _heartbeatTimer = null;
              ZrcSdk_Uninitialize(_handle);
          }
      }

      #region Callback Methods

      private void OnInitializedCallback(string message, int errorCode, IntPtr userData)
      {
          Initialized?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });
      }

      private void OnMeetingStateChangedCallback(string message, int errorCode, IntPtr userData)
      {
          MeetingStateChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });
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
              if (disposing)
              {
                  _heartbeatTimer?.Dispose();
                  _heartbeatTimer = null;
              }

              // Dispose unmanaged resources
              if (_handle != IntPtr.Zero)
              {
                  ZrcSdk_Uninitialize(_handle);
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

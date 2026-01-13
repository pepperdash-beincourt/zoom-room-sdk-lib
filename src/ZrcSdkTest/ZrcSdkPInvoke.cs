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

      // Events
      public event EventHandler<SdkEventArgs> Initialized;
      public event EventHandler<SdkEventArgs> MeetingStateChanged;
      public event EventHandler<SdkEventArgs> Error;

      #region Native Imports

      // Use library name without path - resolver will find it
      private const string DllName = "ZrcSdkWrapper";

      // dlopen constants for Linux
      private const int RTLD_NOW = 2;
      private const int RTLD_LAZY = 1;
      private const int RTLD_GLOBAL = 0x100;

      [DllImport("libdl.so.2")]
      private static extern IntPtr dlopen(string filename, int flags);

      [DllImport("libdl.so.2")]
      private static extern IntPtr dlerror();

      // Static constructor to set up native library resolver
      static ZrcSdk()
      {
          // Preload ZRC SDK library with RTLD_GLOBAL so wrapper can find it
          try
          {
              var appDir = Crestron.SimplSharp.CrestronIO.Directory.GetApplicationDirectory();
              var zrcSdkPath = Path.Combine(appDir, "libZRCSdk.so");
              
              if (!System.IO.File.Exists(zrcSdkPath))
              {
                  throw new InvalidOperationException($"libZRCSdk.so not found at {zrcSdkPath}. Make sure it's deployed alongside the application.");
              }

              // Clear any previous errors
              dlerror();
              
              // Use dlopen with RTLD_GLOBAL to make symbols available
              var handle = dlopen(zrcSdkPath, RTLD_NOW | RTLD_GLOBAL);
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
              // Try loading from Crestron application directory
              var appDir = Crestron.SimplSharp.CrestronIO.Directory.GetApplicationDirectory();
              var libPath = Path.Combine(appDir, "libZrcSdkWrapper.so");
              
              if (!System.IO.File.Exists(libPath))
              {
                  throw new DllNotFoundException($"libZrcSdkWrapper.so not found at {libPath}");
              }

              // Check file size to ensure it's not corrupted
              var fileInfo = new System.IO.FileInfo(libPath);
              Console.WriteLine($"Loading {libPath} (size: {fileInfo.Length} bytes)");
              
              // Clear any previous errors
              dlerror();
              
              // Try RTLD_LAZY first to see if it's a linking issue
              var handle = dlopen(libPath, RTLD_LAZY | RTLD_GLOBAL);
              if (handle == IntPtr.Zero)
              {
                  var errorPtr = dlerror();
                  var errorMsg = errorPtr != IntPtr.Zero ? Marshal.PtrToStringAnsi(errorPtr) : "Unknown error";
                  
                  // Try to provide more context
                  throw new DllNotFoundException(
                      $"Failed to load {libPath}: {errorMsg}\n" +
                      $"File size: {fileInfo.Length} bytes\n" +
                      $"Make sure the library was built for ARM Linux and transferred correctly.\n" +
                      $"On the processor, try: file {libPath} && ldd {libPath}");
              }
              
              Console.WriteLine($"Successfully loaded {libPath}");
              return handle;
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

          ZrcSdk_SetInitializedCallback(_handle, _initializedCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetMeetingStateChangedCallback(_handle, _meetingStateChangedCallbackDelegate, IntPtr.Zero);
          ZrcSdk_SetErrorCallback(_handle, _errorCallbackDelegate, IntPtr.Zero);
      }

      /// <summary>
      /// Initializes the SDK with the specified configuration path
      /// </summary>
      public bool Initialize(string configPath)
      {
          ThrowIfDisposed();
          int result = ZrcSdk_Initialize(_handle, configPath);
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
                  // Dispose managed resources
              }

              // Dispose unmanaged resources
              if (_handle != IntPtr.Zero)
              {
                  Uninitialize();
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

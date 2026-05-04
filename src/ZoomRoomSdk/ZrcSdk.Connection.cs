namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    private SdkEventCallbackDelegate? _connectionStateChangedCallbackDelegate;
    private SdkEventCallbackDelegate? _errorCallbackDelegate;

    /// <summary>
    /// Fired when the ZRC connection state changes.
    /// <see cref="SdkEventArgs.ErrorCode"/>: use <see cref="ConnectionState"/> enum.
    /// 0=Established, 1=Connected, 2=Disconnected.
    /// </summary>
    public event EventHandler<SdkEventArgs>? ConnectionStateChanged;

    /// <summary>Fired when the SDK reports an error. <see cref="SdkEventArgs.ErrorCode"/> is a <c>ZRCSDKError</c> value.</summary>
    public event EventHandler<SdkEventArgs>? Error;

    partial void InitializeConnectionCallbacks()
    {
        _connectionStateChangedCallbackDelegate = OnConnectionStateChangedCallback;
        _errorCallbackDelegate                  = OnErrorCallback;

        ZrcSdk_SetConnectionStateChangedCallback(_handle, _connectionStateChangedCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetErrorCallback(_handle, _errorCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>
    /// Returns the current ZRC connection state.
    /// </summary>
    /// <returns>A <see cref="ConnectionState"/> value cast to int.
    /// Use <see cref="ZrcSdkCodes.GetConnectionStateDescription"/> for a description.</returns>
    public int GetConnectionState()
    {
        ThrowIfDisposed();
        return ZrcSdk_GetConnectionState(_handle);
    }

    /// <summary>Sends a wake command to the Zoom Room display.</summary>
    public bool WakeZoomRoomUp()
    {
        ThrowIfDisposed();
        return ZrcSdk_WakeZoomRoomUp(_handle) == 0;
    }

    /// <summary>Logs out the Zoom Room device from its Zoom account.</summary>
    public bool LogoutZoomRoomDevice()
    {
        ThrowIfDisposed();
        return ZrcSdk_LogoutZoomRoomDevice(_handle) == 0;
    }

    /// <summary>Sends a restart command to the Zoom Room OS.</summary>
    public bool RestartZoomRoomOS()
    {
        ThrowIfDisposed();
        return ZrcSdk_RestartZoomRoomOS(_handle) == 0;
    }

    private void OnConnectionStateChangedCallback(string message, int errorCode, IntPtr userData) =>
        ConnectionStateChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });

    private void OnErrorCallback(string message, int errorCode, IntPtr userData) =>
        Error?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });
}

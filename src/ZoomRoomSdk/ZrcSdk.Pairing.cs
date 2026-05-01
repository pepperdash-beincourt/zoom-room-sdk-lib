namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    private SdkEventCallbackDelegate? _pairRoomResultCallbackDelegate;

    /// <summary>
    /// Fired when a room pairing attempt completes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 0 on success.
    /// Use <see cref="ZrcSdkCodes.GetPairRoomResultDescription"/> for other codes.
    /// </summary>
    public event EventHandler<SdkEventArgs>? PairRoomResult;

    partial void InitializePairingCallbacks()
    {
        _pairRoomResultCallbackDelegate = OnPairRoomResultCallback;
        ZrcSdk_SetPairRoomResultCallback(_handle, _pairRoomResultCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>
    /// Pairs the Zoom Room using an activation code from the Zoom admin portal.
    /// Result arrives asynchronously via <see cref="PairRoomResult"/>.
    /// </summary>
    public bool PairRoomWithActivationCode(string activationCode)
    {
        ThrowIfDisposed();
        return ZrcSdk_PairRoomWithActivationCode(_handle, activationCode) == 0;
    }

    /// <summary>
    /// Returns <see langword="true"/> if stored credentials exist and
    /// <see cref="RetryToPairRoom"/> can reconnect without a new activation code.
    /// </summary>
    public bool CanRetryToPairLastRoom()
    {
        ThrowIfDisposed();
        return ZrcSdk_CanRetryToPairLastRoom(_handle) == 1;
    }

    /// <summary>
    /// Reconnects to the last paired Zoom Room using stored credentials.
    /// Result arrives via <see cref="PairRoomResult"/>.
    /// </summary>
    public bool RetryToPairRoom()
    {
        ThrowIfDisposed();
        return ZrcSdk_RetryToPairRoom(_handle) == 0;
    }

    /// <summary>Unpairs the SDK from the current Zoom Room and clears stored credentials.</summary>
    public bool UnpairRoom()
    {
        ThrowIfDisposed();
        return ZrcSdk_UnpairRoom(_handle) == 0;
    }

    private void OnPairRoomResultCallback(string message, int result, IntPtr userData) =>
        PairRoomResult?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = result });
}

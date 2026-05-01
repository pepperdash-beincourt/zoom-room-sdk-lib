namespace PepperDash.Zoom.ZrcSdk;

public partial class ZrcSdk
{
    private SdkEventCallbackDelegate? _controlSystemEnabledCallbackDelegate;

    /// <summary>
    /// Fired when the Zoom Room Control System (ZRCS) enabled state changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if enabled, 0 if disabled.
    /// </summary>
    public event EventHandler<SdkEventArgs>? ControlSystemEnabled;

    partial void InitializeZrcsCallbacks()
    {
        _controlSystemEnabledCallbackDelegate = OnControlSystemEnabledCallback;
        ZrcSdk_SetControlSystemEnabledCallback(_handle, _controlSystemEnabledCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Returns <see langword="true"/> if the Zoom Room Control System (ZRCS) is enabled.</summary>
    public bool IsZRCSEnabled()
    {
        ThrowIfDisposed();
        return ZrcSdk_IsZRCSEnabled(_handle) == 1;
    }

    /// <summary>Opens or closes the room controls panel on the Zoom Room display.</summary>
    public bool OpenRoomControls(bool open)
    {
        ThrowIfDisposed();
        return ZrcSdk_OpenRoomControls(_handle, open ? 1 : 0) == 0;
    }

    /// <summary>Sends a control command to a ZRCS-managed device.</summary>
    public bool ControlZRCSDevice(string deviceID, string methodID, string paramID, string value)
    {
        ThrowIfDisposed();
        return ZrcSdk_ControlZRCSDevice(_handle, deviceID, methodID, paramID, value) == 0;
    }

    /// <summary>Executes a ZRCS automation scene by scene ID.</summary>
    public bool ExecuteZRCSScene(string sceneID)
    {
        ThrowIfDisposed();
        return ZrcSdk_ExecuteZRCSScene(_handle, sceneID) == 0;
    }

    private void OnControlSystemEnabledCallback(string message, int enabled, IntPtr userData) =>
        ControlSystemEnabled?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = enabled });
}

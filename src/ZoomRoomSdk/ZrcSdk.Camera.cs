using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ControlCamera(IntPtr handle, string deviceID, int action, int type, string? panTilt);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ControlUserCamera(IntPtr handle, int userID, int action, int type);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RespondRemoteCameraControl(IntPtr handle, int userID, int accept);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetFarEndCameraControlRequestCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    private SdkEventCallbackDelegate? _farEndCameraControlCallbackDelegate;

    /// <summary>
    /// Far-end camera control was requested.
    /// <see cref="SdkEventArgs.ErrorCode"/> is the requesting userID.
    /// Respond with <see cref="RespondRemoteCameraControl"/>.
    /// </summary>
    public event EventHandler<SdkEventArgs>? FarEndCameraControlRequest;

    partial void InitializeCameraCallbacks()
    {
        _farEndCameraControlCallbackDelegate = OnFarEndCameraControlCallback;
        ZrcSdk_SetFarEndCameraControlRequestCallback(_handle, _farEndCameraControlCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>
    /// Controls a local camera.
    /// </summary>
    /// <param name="deviceID">Camera device ID string.</param>
    /// <param name="action">CameraControlAction enum value.</param>
    /// <param name="type">CameraControlType enum value.</param>
    /// <param name="panTilt">Pan/tilt value string (may be null).</param>
    public bool ControlCamera(string deviceID, int action, int type, string? panTilt = null)
    {
        ThrowIfDisposed();
        return ZrcSdk_ControlCamera(_handle, deviceID, action, type, panTilt) == 0;
    }

    /// <summary>
    /// Controls a far-end (participant) camera. The target participant must have granted
    /// far-end camera control to this room first.
    /// </summary>
    /// <param name="userID">Target participant userID.</param>
    /// <param name="action">CameraControlAction enum value (MoveUp=0, MoveDown, MoveLeft, MoveRight, ZoomIn, ZoomOut).</param>
    /// <param name="type">CameraControlType enum value (Start=0, Continue, Stop).</param>
    public bool ControlUserCamera(int userID, int action, int type)
    {
        ThrowIfDisposed();
        return ZrcSdk_ControlUserCamera(_handle, userID, action, type) == 0;
    }

    /// <summary>
    /// Accepts or rejects a remote camera control request from another participant.
    /// </summary>
    public bool RespondRemoteCameraControl(int userID, bool accept)
    {
        ThrowIfDisposed();
        return ZrcSdk_RespondRemoteCameraControl(_handle, userID, accept ? 1 : 0) == 0;
    }

    private void OnFarEndCameraControlCallback(string message, int userID, IntPtr userData) =>
        FarEndCameraControlRequest?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = userID });
}

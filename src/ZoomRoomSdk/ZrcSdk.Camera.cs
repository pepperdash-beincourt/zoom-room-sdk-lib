using System.Runtime.InteropServices;
using PepperDash.ZoomRoom.Sdk.EventArgs;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ControlCamera(IntPtr handle, string deviceID, int action, int type, string? panTilt);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ChangeSmartCameraMode(IntPtr handle, int mask, string deviceID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ControlUserCamera(IntPtr handle, int userID, int action, int type);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RespondRemoteCameraControl(IntPtr handle, int userID, int accept);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetFarEndCameraControlRequestCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GetCameraList(IntPtr handle, [Out] ZrcDeviceNative[]? outDevices, int maxCount);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GetCurrentCamera(IntPtr handle, out ZrcDeviceNative outDevice);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_SetCurrentCamera(IntPtr handle, string deviceID);

    // Flat camera/device struct (must mirror ZrcDevice in ZrcSdkWrapper_C.h exactly).
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcDeviceNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string id;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string name;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string displayName;
        public int isSelected;
    }

    private const int MaxCameraList = 16;

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
    /// Sets the smart/auto camera framing mode for a local camera.
    /// </summary>
    /// <param name="mask">SmartCameraMask enum value (Manual=1, SpeakerFocus=2, GroupFocus=4, MultiFocus=8, SmartGallery=16, Director=32, PresenterFocus=64).</param>
    /// <param name="deviceID">Camera device ID; empty string targets the main (near-end) camera.</param>
    public bool ChangeSmartCameraMode(int mask, string deviceID = "")
    {
        ThrowIfDisposed();
        return ZrcSdk_ChangeSmartCameraMode(_handle, mask, deviceID ?? string.Empty) == 0;
    }

    /// <summary>
    /// Enumerates the room's local cameras (device list). Synchronous getter on the setting service.
    /// </summary>
    /// <returns>The cameras, or an empty array on error / no cameras.</returns>
    public CameraDevice[] GetCameras()
    {
        ThrowIfDisposed();
        var buffer = new ZrcDeviceNative[MaxCameraList];
        int count = ZrcSdk_GetCameraList(_handle, buffer, MaxCameraList);
        if (count <= 0) return Array.Empty<CameraDevice>();
        if (count > MaxCameraList) count = MaxCameraList;
        var result = new CameraDevice[count];
        for (int i = 0; i < count; i++) result[i] = ToCameraDevice(buffer[i]);
        return result;
    }

    /// <summary>
    /// Gets the currently selected/active camera.
    /// </summary>
    /// <param name="camera">The active camera when this returns <see langword="true"/>.</param>
    /// <returns><see langword="true"/> if a current camera was retrieved.</returns>
    public bool TryGetCurrentCamera(out CameraDevice? camera)
    {
        ThrowIfDisposed();
        if (ZrcSdk_GetCurrentCamera(_handle, out var native) == 0)
        {
            camera = ToCameraDevice(native);
            return true;
        }
        camera = null;
        return false;
    }

    /// <summary>
    /// Selects the active local camera by device ID. Returns <see langword="false"/> if the
    /// device ID is not found or the SDK rejects the request.
    /// </summary>
    public bool SetCurrentCamera(string deviceID)
    {
        ThrowIfDisposed();
        if (string.IsNullOrEmpty(deviceID)) return false;
        return ZrcSdk_SetCurrentCamera(_handle, deviceID) == 0;
    }

    private static CameraDevice ToCameraDevice(ZrcDeviceNative n) => new CameraDevice
    {
        Id          = n.id ?? string.Empty,
        Name        = n.name ?? string.Empty,
        DisplayName = string.IsNullOrEmpty(n.displayName) ? (n.name ?? string.Empty) : n.displayName,
        IsSelected  = n.isSelected != 0,
    };

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

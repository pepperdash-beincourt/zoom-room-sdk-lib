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

    // Camera presets (index range [0,1,2]; empty deviceID = main/near camera).
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_SetCameraPreset(IntPtr handle, uint index, string deviceID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_GoToCameraPreset(IntPtr handle, uint index, string deviceID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_NameCameraPreset(IntPtr handle, uint index, string name, string deviceID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetCameraPresetInfoCallback(IntPtr handle, ZrcCameraPresetInfoCallbackDelegate? cb, IntPtr userData);

    // Flat camera/device struct (must mirror ZrcDevice in ZrcSdkWrapper_C.h exactly).
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcDeviceNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string id;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string name;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string displayName;
        public int isSelected;
    }

    // Camera preset flat structs (must mirror ZrcCameraPreset / ZrcCameraPresetInfo in ZrcSdkWrapper_C.h).
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcCameraPresetNative
    {
        public int index;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string name;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcCameraPresetInfoNative
    {
        public int defaultIndex;
        public int supportedPresetCount;
        public int presetCount;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)] public ZrcCameraPresetNative[] presets;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcCameraPresetInfoCallbackDelegate(IntPtr infoPtr, IntPtr userData);

    private const int MaxCameraList = 16;

    private SdkEventCallbackDelegate? _farEndCameraControlCallbackDelegate;
    private ZrcCameraPresetInfoCallbackDelegate? _cameraPresetInfoCallbackDelegate;

    /// <summary>
    /// Camera preset info changed (saved presets, supported count). See <see cref="CameraPresetInfoEventArgs"/>.
    /// </summary>
    public event EventHandler<CameraPresetInfoEventArgs>? CameraPresetInfoChanged;

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
        _cameraPresetInfoCallbackDelegate = OnCameraPresetInfoCallback;
        ZrcSdk_SetCameraPresetInfoCallback(_handle, _cameraPresetInfoCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Saves the current camera position to a preset slot (index 0–2). Empty deviceID = main camera.</summary>
    public bool SetCameraPreset(uint index, string deviceID = "")
    {
        ThrowIfDisposed();
        return ZrcSdk_SetCameraPreset(_handle, index, deviceID ?? string.Empty) == 0;
    }

    /// <summary>Recalls a camera preset slot (index 0–2). Empty deviceID = main camera.</summary>
    public bool GoToCameraPreset(uint index, string deviceID = "")
    {
        ThrowIfDisposed();
        return ZrcSdk_GoToCameraPreset(_handle, index, deviceID ?? string.Empty) == 0;
    }

    /// <summary>Names a camera preset slot (index 0–2). Empty deviceID = main camera.</summary>
    public bool NameCameraPreset(uint index, string name, string deviceID = "")
    {
        ThrowIfDisposed();
        return ZrcSdk_NameCameraPreset(_handle, index, name ?? string.Empty, deviceID ?? string.Empty) == 0;
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
        if (count > MaxCameraList)
        {
            // The buffer is bounded at MaxCameraList; surface the truncation rather than silently
            // dropping cameras so an over-capacity room is diagnosable.
            Console.WriteLine($"Warning: ZRC SDK reported {count} cameras; truncating to {MaxCameraList}.");
            count = MaxCameraList;
        }
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

    private void OnCameraPresetInfoCallback(IntPtr infoPtr, IntPtr userData)
    {
        if (infoPtr == IntPtr.Zero) return;
        var n = Marshal.PtrToStructure<ZrcCameraPresetInfoNative>(infoPtr);
        var presets = new List<CameraPreset>();
        int count = Math.Max(0, Math.Min(n.presetCount, 3));
        for (int i = 0; i < count && n.presets != null && i < n.presets.Length; i++)
            presets.Add(new CameraPreset { Index = n.presets[i].index, Name = n.presets[i].name ?? string.Empty });
        CameraPresetInfoChanged?.Invoke(this, new CameraPresetInfoEventArgs
        {
            DefaultIndex         = n.defaultIndex,
            SupportedPresetCount = n.supportedPresetCount,
            Presets              = presets,
        });
    }
}

/// <summary>A saved camera preset slot.</summary>
public sealed class CameraPreset
{
    /// <summary>Preset slot index (0–2).</summary>
    public int Index { get; init; }
    /// <summary>Preset name (empty if unnamed).</summary>
    public string Name { get; init; } = string.Empty;
}

/// <summary>Event args for <see cref="ZrcSdk.CameraPresetInfoChanged"/>.</summary>
public sealed class CameraPresetInfoEventArgs : System.EventArgs
{
    /// <summary>Default preset index (-1 if none).</summary>
    public int DefaultIndex { get; init; }
    /// <summary>Number of preset slots the current camera supports (≤3).</summary>
    public int SupportedPresetCount { get; init; }
    /// <summary>The saved presets.</summary>
    public IReadOnlyList<CameraPreset> Presets { get; init; } = System.Array.Empty<CameraPreset>();
}

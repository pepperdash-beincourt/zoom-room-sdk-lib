using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    // ── ZRCS device / scene list structs ─────────────────────────────────────

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcZRCSDeviceNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string deviceID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string deviceName;
        public int deviceStatus;
        public int updateType;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcZRCSSceneNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string sceneID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string sceneName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string sceneIcon;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcZRCSDeviceListCallbackDelegate(IntPtr devicesPtr, int count, int updateType, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcZRCSSceneListCallbackDelegate(IntPtr scenesPtr, int count, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetZRCSDeviceListCallback(IntPtr handle, ZrcZRCSDeviceListCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetZRCSSceneListCallback(IntPtr handle, ZrcZRCSSceneListCallbackDelegate? cb, IntPtr userData);

    private SdkEventCallbackDelegate?       _controlSystemEnabledCallbackDelegate;
    private ZrcZRCSDeviceListCallbackDelegate? _zrcsDeviceListCallbackDelegate;
    private ZrcZRCSSceneListCallbackDelegate?  _zrcsSceneListCallbackDelegate;

    /// <summary>ZRCS enabled state changed. <see cref="SdkEventArgs.ErrorCode"/> is 1 if enabled.</summary>
    public event EventHandler<SdkEventArgs>? ControlSystemEnabled;
    /// <summary>ZRCS device list updated.</summary>
    public event EventHandler<ZRCSDevice[]>? ZRCSDeviceListUpdated;
    /// <summary>ZRCS scene list updated.</summary>
    public event EventHandler<ZRCSScene[]>? ZRCSSceneListUpdated;

    partial void InitializeZrcsCallbacks()
    {
        _controlSystemEnabledCallbackDelegate = OnControlSystemEnabledCallback;
        _zrcsDeviceListCallbackDelegate       = OnZRCSDeviceListCallback;
        _zrcsSceneListCallbackDelegate        = OnZRCSSceneListCallback;
        ZrcSdk_SetControlSystemEnabledCallback(_handle, _controlSystemEnabledCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetZRCSDeviceListCallback(_handle, _zrcsDeviceListCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetZRCSSceneListCallback(_handle,  _zrcsSceneListCallbackDelegate,  IntPtr.Zero);
    }

    /// <summary>Returns <see langword="true"/> if the Zoom Room Control System (ZRCS) is enabled.</summary>
    public bool IsZRCSEnabled() { ThrowIfDisposed(); return ZrcSdk_IsZRCSEnabled(_handle) == 1; }

    /// <summary>Opens or closes the room controls panel on the Zoom Room display.</summary>
    public bool OpenRoomControls(bool open) { ThrowIfDisposed(); return ZrcSdk_OpenRoomControls(_handle, open ? 1 : 0) == 0; }

    /// <summary>Sends a control command to a ZRCS-managed device.</summary>
    public bool ControlZRCSDevice(string deviceID, string methodID, string paramID, string value)
    {
        ThrowIfDisposed();
        return ZrcSdk_ControlZRCSDevice(_handle, deviceID, methodID, paramID, value) == 0;
    }

    /// <summary>Executes a ZRCS automation scene by scene ID.</summary>
    public bool ExecuteZRCSScene(string sceneID) { ThrowIfDisposed(); return ZrcSdk_ExecuteZRCSScene(_handle, sceneID) == 0; }

    private void OnControlSystemEnabledCallback(string message, int enabled, IntPtr userData) =>
        ControlSystemEnabled?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = enabled });

    private void OnZRCSDeviceListCallback(IntPtr devicesPtr, int count, int updateType, IntPtr userData)
    {
        var devices = MarshalZRCSDevices(devicesPtr, count);
        ZRCSDeviceListUpdated?.Invoke(this, devices);
    }

    private void OnZRCSSceneListCallback(IntPtr scenesPtr, int count, IntPtr userData)
    {
        var scenes = MarshalZRCSScenes(scenesPtr, count);
        ZRCSSceneListUpdated?.Invoke(this, scenes);
    }

    private static ZRCSDevice[] MarshalZRCSDevices(IntPtr ptr, int count)
    {
        if (ptr == IntPtr.Zero || count <= 0) return Array.Empty<ZRCSDevice>();
        var result = new ZRCSDevice[count];
        int stride = Marshal.SizeOf<ZrcZRCSDeviceNative>();
        for (int i = 0; i < count; i++)
        {
            var n = Marshal.PtrToStructure<ZrcZRCSDeviceNative>(ptr + i * stride);
            result[i] = new ZRCSDevice { DeviceID = n.deviceID ?? string.Empty, DeviceName = n.deviceName ?? string.Empty, DeviceStatus = n.deviceStatus, UpdateType = n.updateType };
        }
        return result;
    }

    private static ZRCSScene[] MarshalZRCSScenes(IntPtr ptr, int count)
    {
        if (ptr == IntPtr.Zero || count <= 0) return Array.Empty<ZRCSScene>();
        var result = new ZRCSScene[count];
        int stride = Marshal.SizeOf<ZrcZRCSSceneNative>();
        for (int i = 0; i < count; i++)
        {
            var n = Marshal.PtrToStructure<ZrcZRCSSceneNative>(ptr + i * stride);
            result[i] = new ZRCSScene { SceneID = n.sceneID ?? string.Empty, SceneName = n.sceneName ?? string.Empty, SceneIcon = n.sceneIcon ?? string.Empty };
        }
        return result;
    }
}

/// <summary>Represents a ZRCS-managed (Room Controls) device.</summary>
public class ZRCSDevice
{
    /// <summary>Unique device identifier string.</summary>
    public string DeviceID     { get; set; } = string.Empty;
    /// <summary>Human-readable display name of the device.</summary>
    public string DeviceName   { get; set; } = string.Empty;
    /// <summary>Current device status code reported by the ZRCS platform.</summary>
    public int    DeviceStatus { get; set; }
    /// <summary>Update type code indicating why this device entry was sent (add, update, remove, etc.).</summary>
    public int    UpdateType   { get; set; }
}

/// <summary>Represents a ZRCS automation scene that can be executed.</summary>
public class ZRCSScene
{
    /// <summary>Unique scene identifier string.</summary>
    public string SceneID   { get; set; } = string.Empty;
    /// <summary>Human-readable display name of the scene.</summary>
    public string SceneName { get; set; } = string.Empty;
    /// <summary>Icon identifier or URL for the scene, used by display UIs.</summary>
    public string SceneIcon { get; set; } = string.Empty;
}

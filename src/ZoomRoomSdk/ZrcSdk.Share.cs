using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [StructLayout(LayoutKind.Sequential)]
    private struct ZrcSharingStatusNative
    {
        public int sharingState;
        public int canShareToBO;
        public int isSharingToBO;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcAirPlayStatusNative
    {
        public int instructionDisplayState;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string wifiName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string serverName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]  public string password;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]  public string directPresentationPairingCode;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string directPresentationSharingKey;
        public int isAirHostClientConnected;
        public int isBlackMagicConnected;
        public int isBlackMagicDataAvailable;
        public int isSharingBlackMagic;
        public int isDirectPresentationConnected;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcSharingStatusCallbackDelegate(IntPtr statusPtr, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcAirPlayStatusCallbackDelegate(IntPtr statusPtr, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StopShare(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_LaunchSharingMeeting(IntPtr handle, int isInLocalShare, int displayState);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SwitchFromLocalPresentationToNormalMeeting(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ShowSharingInstruction(IntPtr handle, int show, int instructionState);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ShareBlackMagic(IntPtr handle, int isStart, int isViewLocally);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetSharingStatusCallback(IntPtr handle, ZrcSharingStatusCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetAirPlayStatusCallback(IntPtr handle, ZrcAirPlayStatusCallbackDelegate? cb, IntPtr userData);

    private ZrcSharingStatusCallbackDelegate? _sharingStatusCallbackDelegate;
    private ZrcAirPlayStatusCallbackDelegate? _airPlayStatusCallbackDelegate;

    /// <summary>Sharing status changed. See <see cref="SharingStatusEventArgs"/> for decoded fields.</summary>
    public event EventHandler<SharingStatusEventArgs>? SharingStatusChanged;

    /// <summary>AirPlay / HDMI sharing instruction status changed. See <see cref="AirPlayStatusEventArgs"/> for decoded fields.</summary>
    public event EventHandler<AirPlayStatusEventArgs>? AirPlayStatusChanged;

    partial void InitializeShareCallbacks()
    {
        _sharingStatusCallbackDelegate = OnSharingStatusCallback;
        ZrcSdk_SetSharingStatusCallback(_handle, _sharingStatusCallbackDelegate, IntPtr.Zero);
        _airPlayStatusCallbackDelegate = OnAirPlayStatusCallback;
        ZrcSdk_SetAirPlayStatusCallback(_handle, _airPlayStatusCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Stops the current share from this Zoom Room.</summary>
    public bool StopShare()
    {
        ThrowIfDisposed();
        return ZrcSdk_StopShare(_handle) == 0;
    }

    /// <summary>
    /// Launches a sharing-only ("local presentation") meeting.
    /// </summary>
    /// <param name="isInLocalShare"><see langword="true"/> to start a local presentation; <see langword="false"/> to start a sharing meeting.</param>
    /// <param name="displayState">SharingInstructionDisplayState enum value (None=0, Desktop, IOS, WhiteboardCamera).</param>
    public bool LaunchSharingMeeting(bool isInLocalShare, int displayState)
    {
        ThrowIfDisposed();
        return ZrcSdk_LaunchSharingMeeting(_handle, isInLocalShare ? 1 : 0, displayState) == 0;
    }

    /// <summary>Switches an active local presentation into a normal Zoom meeting.</summary>
    public bool SwitchFromLocalPresentationToNormalMeeting()
    {
        ThrowIfDisposed();
        return ZrcSdk_SwitchFromLocalPresentationToNormalMeeting(_handle) == 0;
    }

    /// <summary>
    /// Shows or hides the wireless-share instruction overlay (AirPlay / HDMI / direct-share codes).
    /// </summary>
    /// <param name="show"><see langword="true"/> to show the instruction overlay; <see langword="false"/> to hide it.</param>
    /// <param name="instructionState">SharingInstructionDisplayState enum value (None=0, Desktop, IOS, WhiteboardCamera).</param>
    public bool ShowSharingInstruction(bool show, int instructionState)
    {
        ThrowIfDisposed();
        return ZrcSdk_ShowSharingInstruction(_handle, show ? 1 : 0, instructionState) == 0;
    }

    /// <summary>
    /// Starts or stops an HDMI ("black magic") cable share from this Zoom Room.
    /// </summary>
    /// <param name="isStart"><see langword="true"/> to start the HDMI share; <see langword="false"/> to stop it.</param>
    /// <param name="isViewLocally"><see langword="true"/> to also display the HDMI source locally.</param>
    public bool ShareBlackMagic(bool isStart, bool isViewLocally)
    {
        ThrowIfDisposed();
        return ZrcSdk_ShareBlackMagic(_handle, isStart ? 1 : 0, isViewLocally ? 1 : 0) == 0;
    }

    private void OnSharingStatusCallback(IntPtr statusPtr, IntPtr userData)
    {
        if (statusPtr == IntPtr.Zero) return;
        var native = Marshal.PtrToStructure<ZrcSharingStatusNative>(statusPtr);
        SharingStatusChanged?.Invoke(this, new SharingStatusEventArgs
        {
            SharingState  = native.sharingState,
            CanShareToBO  = native.canShareToBO != 0,
            IsSharingToBO = native.isSharingToBO != 0,
        });
    }

    private void OnAirPlayStatusCallback(IntPtr statusPtr, IntPtr userData)
    {
        if (statusPtr == IntPtr.Zero) return;
        var native = Marshal.PtrToStructure<ZrcAirPlayStatusNative>(statusPtr);
        AirPlayStatusChanged?.Invoke(this, new AirPlayStatusEventArgs
        {
            InstructionDisplayState        = native.instructionDisplayState,
            WifiName                       = native.wifiName ?? string.Empty,
            ServerName                     = native.serverName ?? string.Empty,
            Password                       = native.password ?? string.Empty,
            DirectPresentationPairingCode  = native.directPresentationPairingCode ?? string.Empty,
            DirectPresentationSharingKey   = native.directPresentationSharingKey ?? string.Empty,
            IsAirHostClientConnected       = native.isAirHostClientConnected != 0,
            IsBlackMagicConnected          = native.isBlackMagicConnected != 0,
            IsBlackMagicDataAvailable      = native.isBlackMagicDataAvailable != 0,
            IsSharingBlackMagic            = native.isSharingBlackMagic != 0,
            IsDirectPresentationConnected  = native.isDirectPresentationConnected != 0,
        });
    }
}

/// <summary>Event args for <see cref="ZrcSdk.SharingStatusChanged"/>.</summary>
public class SharingStatusEventArgs : System.EventArgs
{
    /// <summary>Current sharing state code (0 = not sharing, non-zero = sharing active).</summary>
    public int  SharingState  { get; set; }
    /// <summary><see langword="true"/> if content can be shared into a breakout room.</summary>
    public bool CanShareToBO  { get; set; }
    /// <summary><see langword="true"/> if content is currently being shared to a breakout room.</summary>
    public bool IsSharingToBO { get; set; }
}

/// <summary>Event args for <see cref="ZrcSdk.AirPlayStatusChanged"/>. Maps to <c>AirplayBlackMagicStatus</c> in the ZRC SDK.</summary>
public class AirPlayStatusEventArgs : System.EventArgs
{
    /// <summary>SharingInstructionDisplayState enum value from the SDK.</summary>
    public int    InstructionDisplayState        { get; set; }
    /// <summary>Wi-Fi network name (SSID) for the AirPlay server.</summary>
    public string WifiName                       { get; set; } = string.Empty;
    /// <summary>AirPlay server/room name.</summary>
    public string ServerName                     { get; set; } = string.Empty;
    /// <summary>AirPlay password / share code.</summary>
    public string Password                       { get; set; } = string.Empty;
    /// <summary>Direct presentation pairing code.</summary>
    public string DirectPresentationPairingCode  { get; set; } = string.Empty;
    /// <summary>Laptop direct-share key.</summary>
    public string DirectPresentationSharingKey   { get; set; } = string.Empty;
    /// <summary><see langword="true"/> if an AirHost client is currently connected.</summary>
    public bool IsAirHostClientConnected         { get; set; }
    /// <summary><see langword="true"/> if an HDMI cable is connected.</summary>
    public bool IsBlackMagicConnected            { get; set; }
    /// <summary><see langword="true"/> if HDMI data is available and ready to share.</summary>
    public bool IsBlackMagicDataAvailable        { get; set; }
    /// <summary><see langword="true"/> if HDMI content is currently being shared.</summary>
    public bool IsSharingBlackMagic              { get; set; }
    /// <summary><see langword="true"/> if a direct-presentation (laptop) client is connected.</summary>
    public bool IsDirectPresentationConnected    { get; set; }
}

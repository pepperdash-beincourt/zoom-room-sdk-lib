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

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcSharingStatusCallbackDelegate(IntPtr statusPtr, IntPtr userData);

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

    private ZrcSharingStatusCallbackDelegate? _sharingStatusCallbackDelegate;

    /// <summary>Sharing status changed. See <see cref="SharingStatusEventArgs"/> for decoded fields.</summary>
    public event EventHandler<SharingStatusEventArgs>? SharingStatusChanged;

    partial void InitializeShareCallbacks()
    {
        _sharingStatusCallbackDelegate = OnSharingStatusCallback;
        ZrcSdk_SetSharingStatusCallback(_handle, _sharingStatusCallbackDelegate, IntPtr.Zero);
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

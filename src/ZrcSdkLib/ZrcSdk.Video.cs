using System.Runtime.InteropServices;

namespace PepperDash.Zoom.ZrcSdk;

public partial class ZrcSdk
{
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_MuteUserVideo(IntPtr handle, int userID, int mute);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AllowAttendeesStartVideo(IntPtr handle, int allow);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_PinUserOnScreen(IntPtr handle, int userID, int screenIndex);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_UnpinUserFromScreen(IntPtr handle, int userID, int screenIndex);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SpotlightUser(IntPtr handle, int userID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_CancelSpotlightUser(IntPtr handle, int userID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetAllowAttendeesVideoCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    private SdkEventCallbackDelegate? _allowAttendeesVideoCallbackDelegate;

    /// <summary>Host changed whether attendees may start their video. <see cref="SdkEventArgs.ErrorCode"/> is 1 if allowed.</summary>
    public event EventHandler<SdkEventArgs>? AllowAttendeesVideoChanged;

    partial void InitializeVideoCallbacks()
    {
        _allowAttendeesVideoCallbackDelegate = OnAllowAttendeesVideoCallback;
        ZrcSdk_SetAllowAttendeesVideoCallback(_handle, _allowAttendeesVideoCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Starts or stops the local camera.</summary>
    public bool SetVideoState(bool start)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetVideoState(_handle, start ? 1 : 0) == 0;
    }

    /// <summary>Mutes or unmutes a participant's video. Host only.</summary>
    public bool MuteUserVideo(int userID, bool mute)
    {
        ThrowIfDisposed();
        return ZrcSdk_MuteUserVideo(_handle, userID, mute ? 1 : 0) == 0;
    }

    /// <summary>Allows or prevents attendees from starting their video. Host only.</summary>
    public bool AllowAttendeesStartVideo(bool allow)
    {
        ThrowIfDisposed();
        return ZrcSdk_AllowAttendeesStartVideo(_handle, allow ? 1 : 0) == 0;
    }

    /// <summary>Pins a participant to a specific screen index.</summary>
    public bool PinUserOnScreen(int userID, int screenIndex = 0)
    {
        ThrowIfDisposed();
        return ZrcSdk_PinUserOnScreen(_handle, userID, screenIndex) == 0;
    }

    /// <summary>Unpins a participant from a specific screen index.</summary>
    public bool UnpinUserFromScreen(int userID, int screenIndex = 0)
    {
        ThrowIfDisposed();
        return ZrcSdk_UnpinUserFromScreen(_handle, userID, screenIndex) == 0;
    }

    /// <summary>Spotlights a participant for all attendees. Host only.</summary>
    public bool SpotlightUser(int userID)
    {
        ThrowIfDisposed();
        return ZrcSdk_SpotlightUser(_handle, userID) == 0;
    }

    /// <summary>Removes spotlight from a participant. Host only.</summary>
    public bool CancelSpotlightUser(int userID)
    {
        ThrowIfDisposed();
        return ZrcSdk_CancelSpotlightUser(_handle, userID) == 0;
    }

    private void OnAllowAttendeesVideoCallback(string message, int allow, IntPtr userData) =>
        AllowAttendeesVideoChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = allow });
}

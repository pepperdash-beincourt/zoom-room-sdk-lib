using System.Runtime.InteropServices;

namespace PepperDash.Zoom.ZrcSdk;

public partial class ZrcSdk
{
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AdmitUserFromWaitingRoom(IntPtr handle, int userID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AdmitAllFromWaitingRoom(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_PutUserInWaitingRoom(IntPtr handle, int userID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetInSilentModeCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    private SdkEventCallbackDelegate? _inSilentModeCallbackDelegate;

    /// <summary>Silent mode (attendees muted and on hold) changed. <see cref="SdkEventArgs.ErrorCode"/> is 1 if active.</summary>
    public event EventHandler<SdkEventArgs>? InSilentModeChanged;

    partial void InitializeWaitingRoomCallbacks()
    {
        _inSilentModeCallbackDelegate = OnInSilentModeCallback;
        ZrcSdk_SetInSilentModeCallback(_handle, _inSilentModeCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Admits a participant from the waiting room. Host only.</summary>
    public bool AdmitUserFromWaitingRoom(int userID) { ThrowIfDisposed(); return ZrcSdk_AdmitUserFromWaitingRoom(_handle, userID) == 0; }

    /// <summary>Admits all participants from the waiting room. Host only.</summary>
    public bool AdmitAllFromWaitingRoom() { ThrowIfDisposed(); return ZrcSdk_AdmitAllFromWaitingRoom(_handle) == 0; }

    /// <summary>Sends a participant back to the waiting room. Host only.</summary>
    public bool PutUserInWaitingRoom(int userID) { ThrowIfDisposed(); return ZrcSdk_PutUserInWaitingRoom(_handle, userID) == 0; }

    private void OnInSilentModeCallback(string message, int active, IntPtr userData) =>
        InSilentModeChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = active });
}

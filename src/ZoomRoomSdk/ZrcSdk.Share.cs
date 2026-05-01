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

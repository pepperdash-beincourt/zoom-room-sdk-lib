using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetScreenLayout(IntPtr handle, int screen, int layoutSourceType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetVideoOrder(IntPtr handle, int videoOrderType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetFollowingHostOrder(IntPtr handle, int follow);

    partial void InitializeLayoutCallbacks() { /* no native callbacks for layout */ }

    /// <summary>
    /// Sets the video layout for a screen.
    /// </summary>
    /// <param name="screen">MeetingScreen enum value (0 = primary).</param>
    /// <param name="layoutSourceType">ScreenLayoutSourceType enum value.</param>
    public int SetScreenLayout(int screen, int layoutSourceType)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetScreenLayout(_handle, screen, layoutSourceType);
    }

    /// <summary>Sets the video order type for gallery/speaker view.</summary>
    /// <param name="videoOrderType">VideoOrderType enum value.</param>
    public int SetVideoOrder(int videoOrderType)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetVideoOrder(_handle, videoOrderType);
    }

    /// <summary>Toggles following the host's video order. Returns -2 if not supported by this SDK version.</summary>
    public int SetFollowingHostOrder(bool follow)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetFollowingHostOrder(_handle, follow ? 1 : 0);
    }
}

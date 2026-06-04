using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetScreenLayout(IntPtr handle, int screen, int layoutSourceType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetVideoOrder(IntPtr handle, int videoOrderType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_UpdateVideoLayoutStyle(IntPtr handle, int style);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetFollowingHostOrder(IntPtr handle, int follow);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ControlVideoPosition(IntPtr handle, int position, int size);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_TurnVideoPage(IntPtr handle, int forward, int pageVideoType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ChangeThumbnailsPosition(IntPtr handle, int type);

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

    /// <summary>
    /// Sets the meeting video layout style (Gallery / Speaker / Thumbnail / Content-only / Dynamic).
    /// This is distinct from <see cref="SetVideoOrder"/>, which only changes participant tile ordering.
    /// </summary>
    /// <param name="videoLayoutStyle">VideoLayoutStyle enum value (Gallery=1, Speaker=2, Thumbnail=3, ContentOnly=4, DynamicLayout=6).</param>
    public int UpdateVideoLayoutStyle(int videoLayoutStyle)
    {
        ThrowIfDisposed();
        return ZrcSdk_UpdateVideoLayoutStyle(_handle, videoLayoutStyle);
    }

    /// <summary>Toggles following the host's video order. Returns -2 if not supported by this SDK version.</summary>
    public int SetFollowingHostOrder(bool follow)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetFollowingHostOrder(_handle, follow ? 1 : 0);
    }

    /// <summary>
    /// Sets the self-view PiP position and size.
    /// </summary>
    /// <param name="position">VideoThumbPosition enum value (Center=0, Up, Right, UpRight, Down, DownRight, Left, UpLeft, DownLeft).</param>
    /// <param name="size">VideoThumbSize enum value (Off=0 hides the PiP, 1x=1, 2x=2, 3x=3, Stripe=4).</param>
    public int ControlVideoPosition(int position, int size)
    {
        ThrowIfDisposed();
        return ZrcSdk_ControlVideoPosition(_handle, position, size);
    }

    /// <summary>Pages the video gallery/thumbnail/dynamic view forward or backward.</summary>
    /// <param name="forward"><see langword="true"/> to page to the next page; <see langword="false"/> for the previous page.</param>
    /// <param name="pageVideoType">PageVideoType enum value (GalleryView=0, ThumbnailView=1, DynamicLayoutView=2).</param>
    public int TurnVideoPage(bool forward, int pageVideoType)
    {
        ThrowIfDisposed();
        return ZrcSdk_TurnVideoPage(_handle, forward ? 1 : 0, pageVideoType);
    }

    /// <summary>Changes the thumbnail strip position (used to swap content with the participant thumbnails).</summary>
    /// <param name="type">ThumbnailsPositionType enum value.</param>
    public int ChangeThumbnailsPosition(int type)
    {
        ThrowIfDisposed();
        return ZrcSdk_ChangeThumbnailsPosition(_handle, type);
    }
}

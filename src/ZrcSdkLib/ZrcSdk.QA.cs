using System.Runtime.InteropServices;

namespace PepperDash.Zoom.ZrcSdk;

public partial class ZrcSdk
{
    // ── Caption / Closed Caption structs ──────────────────────────────────────

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcCaptionNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]  public string messageID;
        public int userID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]  public string userName;
        public long messageTime;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2048)] public string content;
        public int isFinal;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcCaptionCallbackDelegate(IntPtr captionPtr, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ShowCaption(IntPtr handle, int show);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_EnableLiveTranscription(IntPtr handle, int enable);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AssignClosedCaption(IntPtr handle, int userID, int assign);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetCaptionCallback(IntPtr handle, ZrcCaptionCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetQAEnabledCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    private ZrcCaptionCallbackDelegate? _captionCallbackDelegate;
    private SdkEventCallbackDelegate?   _qaEnabledCallbackDelegate;

    /// <summary>A live transcription / closed caption message was received.</summary>
    public event EventHandler<CaptionMessage>? CaptionReceived;
    /// <summary>Q&amp;A enabled state changed. <see cref="SdkEventArgs.ErrorCode"/> is 1 if enabled.</summary>
    public event EventHandler<SdkEventArgs>? QAEnabled;

    partial void InitializeQACallbacks()
    {
        _captionCallbackDelegate  = OnCaptionCallback;
        _qaEnabledCallbackDelegate = OnQAEnabledCallback;
        ZrcSdk_SetCaptionCallback(_handle,   _captionCallbackDelegate,   IntPtr.Zero);
        ZrcSdk_SetQAEnabledCallback(_handle, _qaEnabledCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Shows or hides the closed caption overlay on the Zoom Room display.</summary>
    public bool ShowCaption(bool show) { ThrowIfDisposed(); return ZrcSdk_ShowCaption(_handle, show ? 1 : 0) == 0; }

    /// <summary>Enables or disables live transcription (LTT). Host only.</summary>
    public bool EnableLiveTranscription(bool enable) { ThrowIfDisposed(); return ZrcSdk_EnableLiveTranscription(_handle, enable ? 1 : 0) == 0; }

    /// <summary>Assigns or revokes the closed captioner role for a participant. Host only.</summary>
    public bool AssignClosedCaption(int userID, bool assign) { ThrowIfDisposed(); return ZrcSdk_AssignClosedCaption(_handle, userID, assign ? 1 : 0) == 0; }

    private void OnCaptionCallback(IntPtr captionPtr, IntPtr userData)
    {
        if (captionPtr == IntPtr.Zero) return;
        var n = Marshal.PtrToStructure<ZrcCaptionNative>(captionPtr);
        CaptionReceived?.Invoke(this, new CaptionMessage
        {
            MessageID   = n.messageID ?? string.Empty,
            UserID      = n.userID,
            UserName    = n.userName ?? string.Empty,
            MessageTime = n.messageTime,
            Content     = n.content ?? string.Empty,
            IsFinal     = n.isFinal != 0,
        });
    }

    private void OnQAEnabledCallback(string message, int enabled, IntPtr userData) =>
        QAEnabled?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = enabled });
}

/// <summary>A live transcription / closed caption segment.</summary>
public class CaptionMessage
{
    public string MessageID   { get; set; } = string.Empty;
    public int    UserID      { get; set; }
    public string UserName    { get; set; } = string.Empty;
    public long   MessageTime { get; set; }
    public string Content     { get; set; } = string.Empty;
    public bool   IsFinal     { get; set; }
}

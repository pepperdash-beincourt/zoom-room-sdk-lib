using System.Runtime.InteropServices;

namespace PepperDash.Zoom.ZrcSdk;

public partial class ZrcSdk
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcChatMessageNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string messageID;
        public int senderMeetingUserID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string senderName;
        public int receiverMeetingUserID;
        public int sendToType;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2048)] public string content;
        public long messageTime;
        public int isDeleted;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcChatMessageCallbackDelegate(IntPtr msgPtr, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_SendChatMessage(IntPtr handle, string message, int receiverUserID, int sendToType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetChatPrivilegeCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetChatMessageCallback(IntPtr handle, ZrcChatMessageCallbackDelegate? cb, IntPtr userData);

    private SdkEventCallbackDelegate?     _chatPrivilegeCallbackDelegate;
    private ZrcChatMessageCallbackDelegate? _chatMessageCallbackDelegate;

    /// <summary>Chat privilege changed. <see cref="SdkEventArgs.ErrorCode"/> is the MeetingChatPrivilegeType enum value.</summary>
    public event EventHandler<SdkEventArgs>? ChatPrivilegeChanged;
    /// <summary>A chat message was received or updated.</summary>
    public event EventHandler<ChatMessage>? ChatMessageReceived;

    partial void InitializeChatCallbacks()
    {
        _chatPrivilegeCallbackDelegate = OnChatPrivilegeCallback;
        _chatMessageCallbackDelegate   = OnChatMessageCallback;
        ZrcSdk_SetChatPrivilegeCallback(_handle, _chatPrivilegeCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetChatMessageCallback(_handle,   _chatMessageCallbackDelegate,   IntPtr.Zero);
    }

    /// <summary>
    /// Sends a chat message.
    /// </summary>
    /// <param name="message">Message text.</param>
    /// <param name="receiverUserID">Recipient userID, or 0 for everyone.</param>
    /// <param name="sendToType">MeetingChatMSGSendToType enum value (0 = everyone, 1 = specific user).</param>
    public bool SendChatMessage(string message, int receiverUserID = 0, int sendToType = 0)
    {
        ThrowIfDisposed();
        return ZrcSdk_SendChatMessage(_handle, message, receiverUserID, sendToType) == 0;
    }

    private void OnChatPrivilegeCallback(string message, int type, IntPtr userData) =>
        ChatPrivilegeChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = type });

    private void OnChatMessageCallback(IntPtr msgPtr, IntPtr userData)
    {
        if (msgPtr == IntPtr.Zero) return;
        var n = Marshal.PtrToStructure<ZrcChatMessageNative>(msgPtr);
        ChatMessageReceived?.Invoke(this, new ChatMessage
        {
            MessageID            = n.messageID ?? string.Empty,
            SenderMeetingUserID  = n.senderMeetingUserID,
            SenderName           = n.senderName ?? string.Empty,
            ReceiverMeetingUserID = n.receiverMeetingUserID,
            SendToType           = n.sendToType,
            Content              = n.content ?? string.Empty,
            MessageTime          = n.messageTime,
            IsDeleted            = n.isDeleted != 0,
        });
    }
}

/// <summary>A chat message received from the SDK.</summary>
public class ChatMessage
{
    public string MessageID             { get; set; } = string.Empty;
    public int    SenderMeetingUserID   { get; set; }
    public string SenderName            { get; set; } = string.Empty;
    public int    ReceiverMeetingUserID { get; set; }
    public int    SendToType            { get; set; }
    public string Content               { get; set; } = string.Empty;
    public long   MessageTime           { get; set; }
    public bool   IsDeleted             { get; set; }
}

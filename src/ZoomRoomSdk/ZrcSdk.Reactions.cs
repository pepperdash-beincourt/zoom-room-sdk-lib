using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RaiseHand(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_LowerHand(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_LowerHandForUser(IntPtr handle, int userID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_SendReactionEmoji(IntPtr handle, string emoji);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SendReactionFeedback(IntPtr handle, int feedbackType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ClearAllFeedback(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetReactionStatusCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    private SdkEventCallbackDelegate? _reactionStatusCallbackDelegate;

    /// <summary>Reaction / feedback status changed. <see cref="SdkEventArgs.ErrorCode"/> is the ReactionFeedback enum value.</summary>
    public event EventHandler<SdkEventArgs>? ReactionStatusChanged;

    partial void InitializeReactionsCallbacks()
    {
        _reactionStatusCallbackDelegate = OnReactionStatusCallback;
        ZrcSdk_SetReactionStatusCallback(_handle, _reactionStatusCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Raises the local user's hand.</summary>
    public bool RaiseHand() { ThrowIfDisposed(); return ZrcSdk_RaiseHand(_handle) == 0; }

    /// <summary>Lowers the local user's hand.</summary>
    public bool LowerHand() { ThrowIfDisposed(); return ZrcSdk_LowerHand(_handle) == 0; }

    /// <summary>Lowers another participant's hand. Returns false if not supported.</summary>
    public bool LowerHandForUser(int userID) { ThrowIfDisposed(); return ZrcSdk_LowerHandForUser(_handle, userID) == 0; }

    /// <summary>Sends a reaction emoji (e.g. "\uD83D\uDC4D").</summary>
    public bool SendReactionEmoji(string emoji) { ThrowIfDisposed(); return ZrcSdk_SendReactionEmoji(_handle, emoji) == 0; }

    /// <summary>Sends a reaction feedback. <paramref name="feedbackType"/> is a ReactionFeedback enum value.</summary>
    public bool SendReactionFeedback(int feedbackType) { ThrowIfDisposed(); return ZrcSdk_SendReactionFeedback(_handle, feedbackType) == 0; }

    /// <summary>Clears all non-verbal feedback for this participant.</summary>
    public bool ClearAllFeedback() { ThrowIfDisposed(); return ZrcSdk_ClearAllFeedback(_handle) == 0; }

    private void OnReactionStatusCallback(string message, int feedbackType, IntPtr userData) =>
        ReactionStatusChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = feedbackType });
}

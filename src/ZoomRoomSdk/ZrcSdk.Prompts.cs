using System.Runtime.InteropServices;
using PepperDash.ZoomRoom.Sdk.EventArgs;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    // Mirrors ZrcPrompt in ZrcSdkWrapper_C.h: the two 64-bit fields lead so no padding depends on the platform.
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcPromptNative
    {
        public long type64;
        public long autoEndTime;
        public int  kind;
        public int  type;
        public int  isShowing;
        public int  userId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]  public string consentId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 512)]  public string title;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2048)] public string message;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]  public string positiveText;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]  public string negativeText;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 512)]  public string linkUrl;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]  public string linkText;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]  public string fromUser;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]  public string sessionBID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]  public string sessionName;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcPromptCallbackDelegate(IntPtr promptPtr, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetPromptCallback(IntPtr handle, ZrcPromptCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ConfirmMeetingReminder(IntPtr handle, int agree, int reminderType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ConfirmCustomizedMeetingReminder(IntPtr handle, int agree, int customizedType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ConfirmConsent(IntPtr handle, int agree, int consentType, string? consentId);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ConfirmCombinedConsent(IntPtr handle, int agree, long consentType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_HandlePrivacyAlert(IntPtr handle, int action, int type);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ContinueMeetingOnInactivity(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AnswerHostRequestUnmuteVideo(IntPtr handle, int accepted);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ResponseHostInviteToMainSession(IntPtr handle, int accept);

    private ZrcPromptCallbackDelegate? _promptCallbackDelegate;

    /// <summary>
    /// A dialog-style notification during a call: reminders and consents (recording disclaimer,
    /// panelist promotion, live streaming, archiving, AI Companion, focus mode, generic consent),
    /// privacy alerts, the inactive-room auto-end warning, the host asking this room to start video,
    /// breakout-room switch / return-to-main invites, and webinar role changes.
    /// <see cref="PromptEventArgs.Kind"/> selects the matching answer method below.
    /// </summary>
    public event EventHandler<PromptEventArgs>? PromptReceived;

    partial void InitializePromptCallbacks()
    {
        _promptCallbackDelegate = OnPromptCallback;
        ZrcSdk_SetPromptCallback(_handle, _promptCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Answers a <see cref="ZrcPromptKind.MeetingReminder"/> (e.g. join webinar as panelist, recording disclaimer).</summary>
    public bool ConfirmMeetingReminder(bool agree, MeetingReminderType reminderType)
    {
        ThrowIfDisposed();
        return ZrcSdk_ConfirmMeetingReminder(_handle, agree ? 1 : 0, (int)reminderType) == 0;
    }

    /// <summary>Answers a <see cref="ZrcPromptKind.CustomizedReminder"/>.</summary>
    public bool ConfirmCustomizedMeetingReminder(bool agree, int customizedType)
    {
        ThrowIfDisposed();
        return ZrcSdk_ConfirmCustomizedMeetingReminder(_handle, agree ? 1 : 0, customizedType) == 0;
    }

    /// <summary>Answers a <see cref="ZrcPromptKind.Consent"/>; pass the prompt's ConsentId for <see cref="ConsentType.Common"/>.</summary>
    public bool ConfirmConsent(bool agree, ConsentType type, string? consentId = null)
    {
        ThrowIfDisposed();
        return ZrcSdk_ConfirmConsent(_handle, agree ? 1 : 0, (int)type, consentId ?? string.Empty) == 0;
    }

    /// <summary>Answers a <see cref="ZrcPromptKind.CombinedConsent"/> using the prompt's Type64.</summary>
    public bool ConfirmCombinedConsent(bool agree, long consentType)
    {
        ThrowIfDisposed();
        return ZrcSdk_ConfirmCombinedConsent(_handle, agree ? 1 : 0, consentType) == 0;
    }

    /// <summary>Handles a <see cref="ZrcPromptKind.PrivacyAlert"/> (close it, or open / close its disclaimer).</summary>
    public bool HandlePrivacyAlert(PrivacyAlertAction action, PrivacyAlertType type)
    {
        ThrowIfDisposed();
        return ZrcSdk_HandlePrivacyAlert(_handle, (int)action, (int)type) == 0;
    }

    /// <summary>Keeps the meeting running after a <see cref="ZrcPromptKind.InactiveDetection"/> warning.</summary>
    public bool ContinueMeetingOnInactivity()
    {
        ThrowIfDisposed();
        return ZrcSdk_ContinueMeetingOnInactivity(_handle) == 0;
    }

    /// <summary>Answers the host's request that this room start its video (<see cref="ZrcPromptKind.AskStartVideo"/>).</summary>
    public bool AnswerHostRequestUnmuteVideo(bool accepted)
    {
        ThrowIfDisposed();
        return ZrcSdk_AnswerHostRequestUnmuteVideo(_handle, accepted ? 1 : 0) == 0;
    }

    /// <summary>Answers the host's invite back to the main session (<see cref="ZrcPromptKind.BOReturnToMainInvite"/>).</summary>
    public bool ResponseHostInviteToMainSession(bool accept)
    {
        ThrowIfDisposed();
        return ZrcSdk_ResponseHostInviteToMainSession(_handle, accept ? 1 : 0) == 0;
    }

    private void OnPromptCallback(IntPtr promptPtr, IntPtr userData)
    {
        if (promptPtr == IntPtr.Zero) return;
        var n = Marshal.PtrToStructure<ZrcPromptNative>(promptPtr);
        PromptReceived?.Invoke(this, new PromptEventArgs
        {
            Kind         = (ZrcPromptKind)n.kind,
            Type         = n.type,
            Type64       = n.type64,
            IsShowing    = n.isShowing != 0,
            UserId       = n.userId,
            AutoEndTime  = n.autoEndTime,
            ConsentId    = n.consentId ?? string.Empty,
            Title        = n.title ?? string.Empty,
            Message      = n.message ?? string.Empty,
            PositiveText = n.positiveText ?? string.Empty,
            NegativeText = n.negativeText ?? string.Empty,
            LinkUrl      = n.linkUrl ?? string.Empty,
            LinkText     = n.linkText ?? string.Empty,
            FromUser     = n.fromUser ?? string.Empty,
            SessionBID   = n.sessionBID ?? string.Empty,
            SessionName  = n.sessionName ?? string.Empty,
        });
    }
}

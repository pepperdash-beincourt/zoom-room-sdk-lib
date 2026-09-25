namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// A dialog-style notification raised by the Zoom Room during a call (reminder, consent, a request
/// from the host, a breakout-room invite, a webinar role change), delivered on
/// <see cref="PepperDash.ZoomRoom.Sdk.ZrcSdk.PromptReceived"/>. <see cref="Kind"/> says which native
/// notification it came from and which <c>ZrcSdk</c> answer method applies (see <see cref="ZrcPromptKind"/>).
/// </summary>
public sealed class PromptEventArgs : System.EventArgs
{
    public ZrcPromptKind Kind { get; init; }
    /// <summary>Native sub-type enum value for the kind (MeetingReminderType, ConsentType, PrivacyAlertType, MessageEvent, WebinarRoleChangedState...).</summary>
    public int Type { get; init; }
    /// <summary>64-bit sub-type (combined consent).</summary>
    public long Type64 { get; init; }
    /// <summary>True while the Zoom Room is showing the dialog; false when it closed or was resolved elsewhere.</summary>
    public bool IsShowing { get; init; }
    /// <summary>Requesting user (AskStartVideo) or the <see cref="PrivacyAlertAction"/> (PrivacyAlert).</summary>
    public int UserId { get; init; }
    /// <summary>Inactive detection: UTC seconds at which the meeting auto-ends.</summary>
    public long AutoEndTime { get; init; }
    public string ConsentId { get; init; } = string.Empty;
    public string Title { get; init; } = string.Empty;
    public string Message { get; init; } = string.Empty;
    /// <summary>Positive button text supplied by the Zoom Room; empty when no positive action exists.</summary>
    public string PositiveText { get; init; } = string.Empty;
    /// <summary>Negative button text supplied by the Zoom Room; empty when no negative action exists.</summary>
    public string NegativeText { get; init; } = string.Empty;
    public string LinkUrl { get; init; } = string.Empty;
    public string LinkText { get; init; } = string.Empty;
    /// <summary>Breakout switch / return-to-main: who sent it.</summary>
    public string FromUser { get; init; } = string.Empty;
    /// <summary>Breakout switch: target room id.</summary>
    public string SessionBID { get; init; } = string.Empty;
    /// <summary>Breakout switch: target room name.</summary>
    public string SessionName { get; init; } = string.Empty;
}

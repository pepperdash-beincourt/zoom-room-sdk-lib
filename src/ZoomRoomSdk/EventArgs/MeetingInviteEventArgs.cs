namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Details of an incoming meeting invitation (a contact/room inviting this room into a meeting),
/// delivered on <see cref="PepperDash.ZoomRoom.Sdk.ZrcSdk.MeetingInvite"/>. Answer it with
/// <see cref="PepperDash.ZoomRoom.Sdk.ZrcSdk.AnswerMeetingInvite"/>.
/// </summary>
public sealed class MeetingInviteEventArgs : System.EventArgs
{
    /// <summary>Display name of the inviting caller.</summary>
    public string CallerName      { get; init; } = string.Empty;
    /// <summary>Caller's contact ID, if available.</summary>
    public string CallerContactId { get; init; } = string.Empty;
    /// <summary>Meeting ID of the invited meeting.</summary>
    public string MeetingId       { get; init; } = string.Empty;
    /// <summary>Meeting number of the invited meeting (stringified).</summary>
    public string MeetingNumber   { get; init; } = string.Empty;
}

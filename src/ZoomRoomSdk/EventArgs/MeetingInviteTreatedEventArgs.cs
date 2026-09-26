namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Fired when a pending <see cref="PepperDash.ZoomRoom.Sdk.ZrcSdk.MeetingInvite"/> is resolved --
/// answered here, answered elsewhere, declined, or expired/cancelled by the caller -- via
/// <see cref="PepperDash.ZoomRoom.Sdk.ZrcSdk.MeetingInviteTreated"/>.
/// </summary>
public sealed class MeetingInviteTreatedEventArgs : System.EventArgs
{
    /// <summary>Display name of the inviting caller.</summary>
    public string CallerName      { get; init; } = string.Empty;
    /// <summary>Caller's contact ID, if available.</summary>
    public string CallerContactId { get; init; } = string.Empty;
    /// <summary>Meeting ID of the invited meeting.</summary>
    public string MeetingId       { get; init; } = string.Empty;
    /// <summary>Meeting number of the invited meeting (stringified).</summary>
    public string MeetingNumber   { get; init; } = string.Empty;
    /// <summary><see langword="true"/> if the invite was accepted; <see langword="false"/> if declined/expired/cancelled.</summary>
    public bool   Accepted        { get; init; }
}

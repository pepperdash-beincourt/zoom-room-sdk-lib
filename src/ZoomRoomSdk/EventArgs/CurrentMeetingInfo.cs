namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Managed representation of the meeting currently in progress on the Zoom Room (a flattened
/// subset of the SDK <c>MeetingInfo</c> struct). Use <see cref="ZrcSdk.TryGetMeetingInfo"/> to
/// retrieve this synchronously, e.g. to display the "Room Meeting ID" once a meeting has started.
/// </summary>
public sealed class CurrentMeetingInfo
{
    /// <summary>Meeting ID (matches the "Room Meeting ID" shown in the web Room Controller).</summary>
    public string MeetingID        { get; init; } = string.Empty;
    /// <summary>Meeting number used to dial/join the meeting.</summary>
    public string MeetingNumber    { get; init; } = string.Empty;
    /// <summary>Display name / topic of the meeting.</summary>
    public string MeetingName      { get; init; } = string.Empty;
    /// <summary>Alphanumeric meeting password, if any.</summary>
    public string MeetingPassword  { get; init; } = string.Empty;
    /// <summary>Numeric meeting password (for phone/room-system dial-in), if any.</summary>
    public string NumericPassword  { get; init; } = string.Empty;
    /// <summary>Join URL for the meeting, if available.</summary>
    public string JoinMeetingUrl   { get; init; } = string.Empty;
    /// <summary>Raw MeetingType enum value. See <see cref="MeetingType"/>.</summary>
    public MeetingType MeetingType { get; init; }
    /// <summary>True if this is a webinar.</summary>
    public bool   IsWebinar        { get; init; }
    /// <summary>True if this is a waiting-room meeting.</summary>
    public bool   IsWaitingRoom    { get; init; }
    /// <summary>This Zoom Room's user ID in the current meeting.</summary>
    public int    MyUserId         { get; init; }
    /// <summary>True if this Zoom Room is the original host of the meeting.</summary>
    public bool   AmIOriginalHost  { get; init; }
}

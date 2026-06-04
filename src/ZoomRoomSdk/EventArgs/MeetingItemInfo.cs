namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Managed representation of a Zoom Room scheduled meeting (calendar booking).
/// Mirrors the <c>ZrcMeetingItem</c> C struct with the fields the host needs to populate a
/// schedule/bookings list.
/// </summary>
public sealed class MeetingItemInfo
{
    /// <summary>Meeting number used to dial/join the meeting.</summary>
    public string MeetingNumber   { get; init; } = string.Empty;
    /// <summary>Display name / topic of the meeting.</summary>
    public string MeetingName     { get; init; } = string.Empty;
    /// <summary>Name of the meeting host/organizer.</summary>
    public string HostName        { get; init; } = string.Empty;
    /// <summary>Start time as an ISO-8601 string (e.g. "2017-03-15T11:30:00-07:00").</summary>
    public string StartTime       { get; init; } = string.Empty;
    /// <summary>End time as an ISO-8601 string (e.g. "2017-03-15T11:30:00-07:00").</summary>
    public string EndTime         { get; init; } = string.Empty;
    /// <summary>Meeting domain, if reported.</summary>
    public string MeetingDomain   { get; init; } = string.Empty;
    /// <summary>Raw MeetingScheduleFrom enum value (-1 if not applicable).</summary>
    public int    ScheduledFrom   { get; init; }
    /// <summary>True if this is a private event (details hidden).</summary>
    public bool   IsPrivate       { get; init; }
    /// <summary>True if this is an all-day event.</summary>
    public bool   IsAllDayEvent   { get; init; }
    /// <summary>True if this event has been checked in.</summary>
    public bool   IsCheckedIn     { get; init; }
    /// <summary>True if this is an instant meeting.</summary>
    public bool   IsInstantMeeting { get; init; }
}

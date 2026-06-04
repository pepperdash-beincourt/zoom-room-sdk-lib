namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Event arguments for scheduled-meeting (bookings/schedule) list updates received from the
/// Zoom Room. Raised in response to a meeting list request (<see cref="ZrcSdk.ListMeeting"/>).
/// </summary>
public sealed class MeetingListEventArgs : System.EventArgs
{
    /// <summary>
    /// Raw ListMeetingResult enum value reported by the SDK (0 = success).
    /// </summary>
    public int Result { get; init; }

    /// <summary>
    /// The scheduled meetings for this update.
    /// </summary>
    public MeetingItemInfo[] Meetings { get; init; } = Array.Empty<MeetingItemInfo>();
}

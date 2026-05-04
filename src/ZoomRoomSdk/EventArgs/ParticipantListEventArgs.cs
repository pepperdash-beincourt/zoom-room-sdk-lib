namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Event arguments for participant list updates received from the Zoom Room.
/// Raised for both the initial list and incremental add/remove/update events.
/// </summary>
public sealed class ParticipantListEventArgs : System.EventArgs
{
    /// <summary>
    /// The participant data for this update batch.
    /// </summary>
    public ParticipantInfo[] Participants { get; init; } = Array.Empty<ParticipantInfo>();

    /// <summary>
    /// Total participant count as reported by the SDK for this session.
    /// </summary>
    public int TotalCount { get; init; }

    /// <summary>
    /// When <see langword="true"/> the consumer should replace its entire local list
    /// with <see cref="Participants"/> rather than merging.
    /// </summary>
    public bool NeedCleanUp { get; init; }

    /// <summary>
    /// The conference session this update applies to.
    /// </summary>
    public ConfSessionType Session { get; init; }
}

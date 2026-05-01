namespace PepperDash.Zoom.ZrcSdk;

/// <summary>
/// Event arguments passed to all ZRC SDK events.
/// </summary>
public class SdkEventArgs : System.EventArgs
{
    /// <summary>
    /// A human-readable message describing the event. May be empty for numeric-only events.
    /// </summary>
    public string Message { get; set; } = string.Empty;

    /// <summary>
    /// An integer code whose meaning is event-specific. Use <see cref="ZrcSdkCodes"/>
    /// helper methods to translate common codes to descriptions.
    /// </summary>
    public int ErrorCode { get; set; }
}

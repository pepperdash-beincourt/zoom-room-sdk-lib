namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Managed representation of a Zoom Room meeting participant.
/// Mirrors the <c>ZrcParticipant</c> C struct with fully-typed properties.
/// </summary>
public sealed class ParticipantInfo
{
    /// <summary>Zoom meeting user ID for this participant in the current session.</summary>
    public int    UserID                        { get; init; }
    /// <summary>User ID of the parent participant (non-zero for companion-mode or sub-session users).</summary>
    public int    ParentUserID                  { get; init; }
    /// <summary>Globally unique user GUID string assigned by the Zoom platform.</summary>
    public string UserGUID                      { get; init; } = string.Empty;
    /// <summary>Participant category (normal, H.323, or PSTN call-in).</summary>
    public UserType UserType                    { get; init; }
    /// <summary>Display name shown in the Zoom Room.</summary>
    public string UserName                      { get; init; } = string.Empty;
    /// <summary>Optional pronouns string set by the participant.</summary>
    public string Pronouns                      { get; init; } = string.Empty;
    /// <summary><see langword="true"/> if this entry represents the local Zoom Room device itself.</summary>
    public bool   IsMySelf                      { get; init; }
    /// <summary><see langword="true"/> if this participant is the meeting host.</summary>
    public bool   IsHost                        { get; init; }
    /// <summary><see langword="true"/> if this participant is the original or an alternative host.</summary>
    public bool   IsOriginalOrAlternativeHost   { get; init; }
    /// <summary><see langword="true"/> if this participant has co-host privileges.</summary>
    public bool   IsCohost                      { get; init; }
    /// <summary><see langword="true"/> if this participant is an external (guest) user.</summary>
    public bool   IsGuest                       { get; init; }
    /// <summary><see langword="true"/> if this participant joined in view-only (webinar attendee) mode.</summary>
    public bool   IsViewOnlyUser                { get; init; }
    /// <summary><see langword="true"/> if a view-only participant has been granted talk permission.</summary>
    public bool   IsViewOnlyUserCanTalk         { get; init; }
    /// <summary><see langword="true"/> if the participant is allowed to record the meeting.</summary>
    public bool   CanRecord                     { get; init; }
    /// <summary><see langword="true"/> if the participant is currently recording.</summary>
    public bool   IsRecording                   { get; init; }
    /// <summary><see langword="true"/> if recording has been disabled for this participant by the host.</summary>
    public bool   RecordingDisabled             { get; init; }
    /// <summary><see langword="true"/> if the participant is in a breakout room silent mode.</summary>
    public bool   IsInSilentMode                { get; init; }

    // ── Audio ─────────────────────────────────────────────────────────────────
    /// <summary>Audio connection type (VoIP or phone call-in).</summary>
    public AudioType AudioType                  { get; init; }
    /// <summary><see langword="true"/> if the participant's audio is muted.</summary>
    public bool   AudioMuted                    { get; init; }

    // ── Video ─────────────────────────────────────────────────────────────────
    /// <summary><see langword="true"/> if the participant's camera has a video source available.</summary>
    public bool   VideoHasSource               { get; init; }
    /// <summary><see langword="true"/> if the local Zoom Room is receiving this participant's video.</summary>
    public bool   VideoReceiving               { get; init; }
    /// <summary><see langword="true"/> if the participant is sending video.</summary>
    public bool   VideoSending                 { get; init; }
    /// <summary><see langword="true"/> if the local device can control this participant's video state.</summary>
    public bool   VideoCanControl              { get; init; }

    // ── Camera control ────────────────────────────────────────────────────────
    /// <summary><see langword="true"/> if a far-end camera control request can be sent to this participant.</summary>
    public bool   CameraCanRequestControl      { get; init; }
    /// <summary><see langword="true"/> if the local device is currently controlling this participant's camera.</summary>
    public bool   CameraAmIControlling         { get; init; }
    /// <summary><see langword="true"/> if the participant's camera supports switching (multiple camera inputs).</summary>
    public bool   CameraCanSwitch              { get; init; }
    /// <summary><see langword="true"/> if the participant's camera supports pan/tilt movement.</summary>
    public bool   CameraCanMove                { get; init; }
    /// <summary><see langword="true"/> if the participant's camera supports optical zoom.</summary>
    public bool   CameraCanZoom                { get; init; }

    // ── Hand / reaction ───────────────────────────────────────────────────────
    /// <summary><see langword="true"/> if the participant has raised their hand.</summary>
    public bool          HandRaised             { get; init; }
    /// <summary>Skin tone modifier applied to the participant's raised-hand emoji.</summary>
    public HandSkinTone  HandSkinTone           { get; init; }
    /// <summary>Unicode emoji string of the participant's active reaction, or empty if none.</summary>
    public string        ReactionEmoji          { get; init; } = string.Empty;
    /// <summary>Non-verbal feedback status (yes/no/fast/slow/coffee/hand).</summary>
    public ReactionFeedback ReactionFeedback    { get; init; }

    // ── Interpretation ────────────────────────────────────────────────────────
    /// <summary><see langword="true"/> if this participant is acting as a language interpreter.</summary>
    public bool              IsInterpreter             { get; init; }
    /// <summary>Interpretation channel this participant is assigned to.</summary>
    public InterpretLanguage InterpreterLanguage        { get; init; }
    /// <summary>Opaque language ID string for custom interpretation channels.</summary>
    public string            InterpreterLanguageID      { get; init; } = string.Empty;
    /// <summary>Human-readable name of the participant's interpretation language.</summary>
    public string            InterpreterLanguageName    { get; init; } = string.Empty;

    // ── Misc ──────────────────────────────────────────────────────────────────
    /// <summary><see langword="true"/> if this participant has remote-control administrator rights.</summary>
    public bool   IsRemoteControlAdmin          { get; init; }
    /// <summary><see langword="true"/> if this participant is an AI virtual assistant.</summary>
    public bool   IsVirtualAssistant            { get; init; }
    /// <summary><see langword="true"/> if this participant joined from a Companion Mode device.</summary>
    public bool   IsCompanionModeUser           { get; init; }
    /// <summary><see langword="true"/> if this is a Companion Zoom Room user.</summary>
    public bool   IsCompanionZRUser             { get; init; }

    // ── Breakout room ─────────────────────────────────────────────────────────
    /// <summary>Breakout room session BID that this participant is currently assigned to. Empty if in the main session.</summary>
    public string       BoSessionBID                { get; init; } = string.Empty;
    /// <summary>Breakout room assignment status for this participant.</summary>
    public BO_USER_STATUS BoUserStatus              { get; init; }
    /// <summary><see langword="true"/> if the host can force this participant to join or leave breakout rooms.</summary>
    public bool         BoSupportForceJoinLeave     { get; init; }
    /// <summary><see langword="true"/> if this participant can self-select a breakout room.</summary>
    public bool         BoSupportSelfChoose         { get; init; }
    /// <summary><see langword="true"/> if a co-host can start/stop breakout rooms for this participant.</summary>
    public bool         BoSupportCohostStartStop    { get; init; }

    // ── Streaming / webinar ───────────────────────────────────────────────────
    /// <summary><see langword="true"/> if this participant can be pinned in multi-video layout.</summary>
    public bool   CanPinMultiVideo              { get; init; }
    /// <summary><see langword="true"/> if this participant supports the green room feature.</summary>
    public bool   IsSupportGreenRoom            { get; init; }
    /// <summary><see langword="true"/> if this participant is currently in the green room.</summary>
    public bool   IsInGreenRoom                 { get; init; }

    // ── Audio far-end control ─────────────────────────────────────────────────
    /// <summary><see langword="true"/> if the local device can request audio control of this participant.</summary>
    public bool   AudioCanRequestControl        { get; init; }
    /// <summary><see langword="true"/> if this participant's audio can be controlled by a remote request.</summary>
    public bool   AudioCanBeRequested           { get; init; }
    /// <summary><see langword="true"/> if the local device is currently controlling this participant's audio.</summary>
    public bool   AudioAmIControlling           { get; init; }

    // ── RTMP / special ────────────────────────────────────────────────────────
    /// <summary><see langword="true"/> if this participant is an RTMP streaming user.</summary>
    public bool   IsRTMPUser                    { get; init; }
    /// <summary><see langword="true"/> if this participant is an active (currently streaming) RTMP user.</summary>
    public bool   IsActiveRTMPUser              { get; init; }
    /// <summary><see langword="true"/> if this participant is a Simulive (pre-recorded webinar) user.</summary>
    public bool   IsSimuliveUser                { get; init; }

    // ── Timezone ──────────────────────────────────────────────────────────────
    /// <summary>Participant's local time zone offset from UTC, in minutes.</summary>
    public int    TimeZoneOffsetMinutes         { get; init; }
    /// <summary><see langword="true"/> if the Zoom Room can display this participant's local time.</summary>
    public bool   IsSupportDisplayLocalTime     { get; init; }
    /// <summary>Jabber ID (JID) of this attendee, used for messaging.</summary>
    public string AttendeeJid                   { get; init; } = string.Empty;
}

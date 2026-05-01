namespace PepperDash.Zoom.ZrcSdk.EventArgs;

/// <summary>
/// Managed representation of a Zoom Room meeting participant.
/// Mirrors the <c>ZrcParticipant</c> C struct with fully-typed properties.
/// </summary>
public sealed class ParticipantInfo
{
    public int    UserID                        { get; init; }
    public int    ParentUserID                  { get; init; }
    public string UserGUID                      { get; init; } = string.Empty;
    public UserType UserType                    { get; init; }
    public string UserName                      { get; init; } = string.Empty;
    public string Pronouns                      { get; init; } = string.Empty;
    public bool   IsMySelf                      { get; init; }
    public bool   IsHost                        { get; init; }
    public bool   IsOriginalOrAlternativeHost   { get; init; }
    public bool   IsCohost                      { get; init; }
    public bool   IsGuest                       { get; init; }
    public bool   IsViewOnlyUser                { get; init; }
    public bool   IsViewOnlyUserCanTalk         { get; init; }
    public bool   CanRecord                     { get; init; }
    public bool   IsRecording                   { get; init; }
    public bool   RecordingDisabled             { get; init; }
    public bool   IsInSilentMode                { get; init; }

    // Audio
    public AudioType AudioType                  { get; init; }
    public bool   AudioMuted                    { get; init; }

    // Video
    public bool   VideoHasSource               { get; init; }
    public bool   VideoReceiving               { get; init; }
    public bool   VideoSending                 { get; init; }
    public bool   VideoCanControl              { get; init; }

    // Camera control
    public bool   CameraCanRequestControl      { get; init; }
    public bool   CameraAmIControlling         { get; init; }
    public bool   CameraCanSwitch              { get; init; }
    public bool   CameraCanMove                { get; init; }
    public bool   CameraCanZoom                { get; init; }

    // Hand / reaction
    public bool          HandRaised             { get; init; }
    public HandSkinTone  HandSkinTone           { get; init; }
    public string        ReactionEmoji          { get; init; } = string.Empty;
    public ReactionFeedback ReactionFeedback    { get; init; }

    // Interpretation
    public bool              IsInterpreter             { get; init; }
    public InterpretLanguage InterpreterLanguage        { get; init; }
    public string            InterpreterLanguageID      { get; init; } = string.Empty;
    public string            InterpreterLanguageName    { get; init; } = string.Empty;

    // Misc
    public bool   IsRemoteControlAdmin          { get; init; }
    public bool   IsVirtualAssistant            { get; init; }
    public bool   IsCompanionModeUser           { get; init; }
    public bool   IsCompanionZRUser             { get; init; }

    // Breakout room
    public string       BoSessionBID                { get; init; } = string.Empty;
    public BO_USER_STATUS BoUserStatus              { get; init; }
    public bool         BoSupportForceJoinLeave     { get; init; }
    public bool         BoSupportSelfChoose         { get; init; }
    public bool         BoSupportCohostStartStop    { get; init; }

    // Streaming / webinar
    public bool   CanPinMultiVideo              { get; init; }
    public bool   IsSupportGreenRoom            { get; init; }
    public bool   IsInGreenRoom                 { get; init; }

    // Audio control
    public bool   AudioCanRequestControl        { get; init; }
    public bool   AudioCanBeRequested           { get; init; }
    public bool   AudioAmIControlling           { get; init; }

    // RTMP / special
    public bool   IsRTMPUser                    { get; init; }
    public bool   IsActiveRTMPUser              { get; init; }
    public bool   IsSimuliveUser                { get; init; }

    // Timezone
    public int    TimeZoneOffsetMinutes         { get; init; }
    public bool   IsSupportDisplayLocalTime     { get; init; }
    public string AttendeeJid                   { get; init; } = string.Empty;
}

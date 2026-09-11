namespace PepperDash.ZoomRoom.Sdk;

// ── Connection ────────────────────────────────────────────────────────────────

/// <summary>SDK connection state with the Zoom Room.</summary>
public enum ConnectionState
{
    /// <summary>Not connected / uninitialized.</summary>
    None = -1,
    /// <summary>TCP connection established, handshake in progress.</summary>
    Established = 0,
    /// <summary>Fully connected and verified with the Zoom Room.</summary>
    Connected = 1,
    /// <summary>Disconnected.</summary>
    Disconnected = 2,
}

/// <summary>Reason the device was unpaired from the Zoom Room.</summary>
public enum RoomUnpairedReason
{
    /// <summary>The pairing token is no longer valid.</summary>
    TokenInvalid = 1,
    /// <summary>Refresh token renewal failed.</summary>
    RefreshTokenFail = 2,
    /// <summary>Removed by an administrator.</summary>
    RemovedByAdmin = 3,
    /// <summary>Beta build has expired.</summary>
    BetaBuildExpired = 4,
    /// <summary>The Zoom Room is in an end-to-end encrypted meeting.</summary>
    ZRInE2EMeeting = 5,
}

// ── Meeting ───────────────────────────────────────────────────────────────────

/// <summary>Current Zoom Room meeting status.</summary>
public enum MeetingStatus
{
    /// <summary>Not in a meeting.</summary>
    NotInMeeting = 0,
    /// <summary>Connecting to a meeting.</summary>
    ConnectingToMeeting = 1,
    /// <summary>Actively in a meeting.</summary>
    InMeeting = 2,
    /// <summary>The Zoom Room is logged out.</summary>
    LoggedOut = 3,
}

/// <summary>Meeting type.</summary>
public enum MeetingType
{
    /// <summary>No meeting type.</summary>
    None = 0,
    /// <summary>Standard video meeting.</summary>
    Meeting = 1,
    /// <summary>Content-sharing-only session.</summary>
    Sharing = 2,
    /// <summary>PSTN call-out session.</summary>
    PSTNCallout = 3,
    /// <summary>Third-party integration meeting.</summary>
    Integration = 4,
}

/// <summary>Meeting encryption algorithm.</summary>
public enum MeetingEncryptionAlgorithm
{
    /// <summary>No encryption.</summary>
    None = 0,
    /// <summary>AES-256 encryption.</summary>
    AES256 = 1,
    /// <summary>AES-256-GCM encryption.</summary>
    AES256GCM = 2,
}

/// <summary>General encryption algorithm.</summary>
public enum EncryptionAlgorithm
{
    /// <summary>Unknown encryption type.</summary>
    Unknown = -1,
    /// <summary>No encryption.</summary>
    None = 0,
    /// <summary>AES-256 encryption.</summary>
    AES256 = 1,
    /// <summary>AES-256-GCM encryption.</summary>
    AES256GCM = 2,
    /// <summary>AES-256-GCM encryption with third-party key.</summary>
    AES256GCM3rd = 3,
}

/// <summary>Meeting exit reason.</summary>
public enum ExitMeetingReason
{
    /// <summary>No specific reason.</summary>
    None = 0,
    /// <summary>Meeting ended by the host.</summary>
    ByHost = 1,
    /// <summary>This participant was removed by the host.</summary>
    RemovedByHost = 2,
    /// <summary>Joined from another device, forcing this session to close.</summary>
    JoinFromAnotherDevice = 3,
    /// <summary>Joined from another location, forcing this session to close.</summary>
    JoinFromAnotherLocation = 4,
    /// <summary>The meeting reached its scheduled end time.</summary>
    MeetingOver = 5,
    /// <summary>Free meeting time limit was exceeded.</summary>
    FreeMeetingOver = 6,
    /// <summary>Unknown exit reason.</summary>
    Unknown = 100,
}

// ── Participants ──────────────────────────────────────────────────────────────

/// <summary>Meeting user type.</summary>
public enum UserType
{
    /// <summary>Standard video meeting participant.</summary>
    Normal = 0,
    /// <summary>H.323 / SIP room system participant.</summary>
    H323 = 1,
    /// <summary>Pure PSTN call-in participant (audio only).</summary>
    PureCallIn = 2,
}

/// <summary>Conference session type (for multi-session scenarios like breakout rooms).</summary>
public enum ConfSessionType
{
    /// <summary>The specific session in which you are actively participating.</summary>
    CurrentSession = 1,
    /// <summary>The overarching meeting that encompasses all sub-sessions.</summary>
    MasterSession = 2,
}

/// <summary>Conference instance type indicating data source.</summary>
public enum ConfInstType
{
    /// <summary>Unknown instance type.</summary>
    Unknown = -1,
    /// <summary>The current active conference.</summary>
    CurrentConf = 0,
    /// <summary>The master (main) conference.</summary>
    MasterConf = 1,
    /// <summary>Webinar backstage area.</summary>
    Backstage = 2,
    /// <summary>A new breakout room session.</summary>
    NewBO = 3,
}

/// <summary>Recording type carried by <see cref="ZrcSdk.RecordingRequest"/> (native RecordingType).</summary>
public enum RecordingType
{
    Unknown = -1,
    Local = 0,
    Cloud = 1,
}

/// <summary>Claim host result.</summary>
public enum ClaimHostResult
{
    /// <summary>Host key accepted; role claimed.</summary>
    Success = 0,
    /// <summary>The supplied host key was incorrect.</summary>
    InvalidHostKey = 1,
    /// <summary>Claim failed for an unspecified reason.</summary>
    UnknownError = 2,
}

/// <summary>Report issue type flags.</summary>
[Flags]
public enum ReportIssueType
{
    /// <summary>Content is offensive, illegal, or abusive.</summary>
    OffensiveIllegalAbusive = 0x40,
    /// <summary>Content involves suicide or self-harm.</summary>
    SuicideSelfHarm = 0x80,
    /// <summary>Private information was shared without consent.</summary>
    PrivateInformation = 0x100,
    /// <summary>Spam content.</summary>
    Spam = 0x200,
    /// <summary>Copyright or trademark infringement.</summary>
    CopyrightTrademarkInfringement = 0x400,
    /// <summary>Impersonation of another person or entity.</summary>
    Impersonation = 0x800,
    /// <summary>Other issue type not covered above.</summary>
    Other = 0x1,
}

/// <summary>Zoom Room Whiteboard user change type.</summary>
public enum ZRWUserChangeType
{
    /// <summary>User joined the whiteboard session.</summary>
    Join = 0,
    /// <summary>User left the whiteboard session.</summary>
    Leave = 1,
}

/// <summary>Breakout room user status.</summary>
public enum BO_USER_STATUS
{
    /// <summary>Invalid or uninitialized status.</summary>
    Invalid = -1,
    /// <summary>User is in the main (master) meeting session.</summary>
    InMaster = 1,
    /// <summary>User is in a breakout room.</summary>
    InBO = 2,
    /// <summary>User has left the breakout room.</summary>
    Leave = 3,
}

// ── Audio ─────────────────────────────────────────────────────────────────────

/// <summary>Audio connection type.</summary>
public enum AudioType
{
    /// <summary>No audio connection.</summary>
    None = 0,
    /// <summary>Voice over IP (computer audio).</summary>
    VoIP = 1,
    /// <summary>PSTN telephone call-in.</summary>
    Phone = 2,
}

/// <summary>Reason the host asked ZR to unmute its audio.</summary>
public enum AskUnmuteAudioByHostType
{
    /// <summary>Host asked to unmute audio for speaking.</summary>
    UnmuteAudio = 0,
    /// <summary>Host spotlighted this participant, requiring unmute.</summary>
    Spotlight = 1,
    /// <summary>View-only participant was granted talk permission.</summary>
    ViewOnlyTalk = 2,
}

// ── Video ─────────────────────────────────────────────────────────────────────

/// <summary>Video sending source type.</summary>
public enum VideoSendingSourceType
{
    /// <summary>Video captured from a physical camera device.</summary>
    Device = 0,
    /// <summary>Video rendered via lip-sync (AI-generated).</summary>
    Lipsync = 1,
}

// ── Screen / Layout ───────────────────────────────────────────────────────────

/// <summary>Screen index on the Zoom Room.</summary>
public enum MeetingScreen
{
    /// <summary>Screen index is unknown.</summary>
    Unknown = -1,
    /// <summary>Primary (first) display.</summary>
    First = 0,
    /// <summary>Secondary (second) display.</summary>
    Second = 1,
    /// <summary>Tertiary (third) display.</summary>
    Third = 2,
    /// <summary>Confidence monitor display.</summary>
    Confidence = 100,
}

/// <summary>What is shown on a screen panel.</summary>
public enum ScreenLayoutSourceType
{
    /// <summary>No source assigned.</summary>
    None = -1,
    /// <summary>Active speaker video.</summary>
    ActiveVideo = 0,
    /// <summary>Local self-view video.</summary>
    SelfVideo = 1,
    /// <summary>A pinned participant's video.</summary>
    PinnedVideo = 2,
    /// <summary>Spotlighted participant's video.</summary>
    Spotlight = 3,
    /// <summary>Gallery grid of participant videos.</summary>
    Gallery = 4,
    /// <summary>Shared screen / content.</summary>
    SharedContent = 5,
    /// <summary>Static background image or color.</summary>
    Background = 6,
    /// <summary>Local room camera view.</summary>
    LocalView = 7,
    /// <summary>Immersive Scene view.</summary>
    ImmersiveView = 8,
    /// <summary>Zoom Apps embedded view.</summary>
    ZoomAppsView = 9,
    /// <summary>Dynamic layout managed by Zoom Rooms intelligence.</summary>
    DynamicView = 10,
    /// <summary>Thumbnail strip of participant videos.</summary>
    ThumbnailView = 11,
    /// <summary>Thumbnail strip alongside shared content.</summary>
    ThumbnailShareView = 12,
}

// ── Share ─────────────────────────────────────────────────────────────────────

/// <summary>Share source type.</summary>
public enum ShareSourceType
{
    /// <summary>Unknown share source.</summary>
    Unknown = -1,
    /// <summary>Normal screen or application share.</summary>
    Normal = 0,
    /// <summary>Zoom Cloud Whiteboard share.</summary>
    CloudWhiteboard = 1,
    /// <summary>Collaborative Zoom Apps share.</summary>
    CollaborationZapps = 2,
}

/// <summary>Sharing instruction display state.</summary>
public enum SharingInstructionDisplayState
{
    /// <summary>No instruction displayed.</summary>
    None = 0,
    /// <summary>Desktop sharing instructions are shown.</summary>
    Desktop = 1,
    /// <summary>iOS AirPlay sharing instructions are shown.</summary>
    IOS = 2,
    /// <summary>Whiteboard camera instructions are shown.</summary>
    WhiteboardCamera = 3,
}

// ── Breakout Rooms ────────────────────────────────────────────────────────────

/// <summary>Breakout room lifecycle status.</summary>
public enum BO_STATUS
{
    /// <summary>Invalid or uninitialized status.</summary>
    Invalid = 0,
    /// <summary>Breakout rooms are being configured (pre-start).</summary>
    Edit = 1,
    /// <summary>Breakout rooms are open and participants can join.</summary>
    Started = 2,
    /// <summary>Breakout rooms are in the process of closing.</summary>
    Stopping = 3,
    /// <summary>Breakout rooms have ended.</summary>
    Ended = 4,
}

// ── Camera ────────────────────────────────────────────────────────────────────

/// <summary>Camera control event type.</summary>
public enum CameraControlType
{
    /// <summary>Control session started.</summary>
    Start = 0,
    /// <summary>Control command continues (hold).</summary>
    Continue = 1,
    /// <summary>Control session stopped.</summary>
    Stop = 2,
    /// <summary>Remote end requested camera control.</summary>
    RequestedByRemote = 3,
    /// <summary>Remote end gave up camera control.</summary>
    GaveUpByRemote = 4,
    /// <summary>Local device sent a control request to the remote.</summary>
    ControlRequestToRemote = 5,
    /// <summary>Local device gave up control of the remote camera.</summary>
    ControlGaveUpToRemote = 6,
    /// <summary>Opens the remote camera control panel on the display.</summary>
    ControlOpenCtrlRemotePanel = 7,
    /// <summary>Adjusts the pan/tilt moving speed.</summary>
    ControlMovingSpeed = 8,
}

/// <summary>Camera PTZ action.</summary>
public enum CameraControlAction
{
    /// <summary>Tilt camera upward.</summary>
    MoveUp = 0,
    /// <summary>Tilt camera downward.</summary>
    MoveDown = 1,
    /// <summary>Pan camera left.</summary>
    MoveLeft = 2,
    /// <summary>Pan camera right.</summary>
    MoveRight = 3,
    /// <summary>Zoom in (telephoto).</summary>
    ZoomIn = 4,
    /// <summary>Zoom out (wide angle).</summary>
    ZoomOut = 5,
}

/// <summary>Camera calibration layout.</summary>
public enum CalibrationCameraLayout
{
    /// <summary>Unknown layout.</summary>
    Unknown = -1,
    /// <summary>Left-top, mid-bottom, right-top arrangement.</summary>
    LeftTopMidBottomRightTop = 0,
    /// <summary>Left-bottom, mid-top, right-bottom arrangement.</summary>
    LeftBottomMidTopRightBottom = 1,
    /// <summary>Left-top, mid-top, right-top arrangement.</summary>
    LeftTopMidTopRightTop = 2,
    /// <summary>Left-bottom, mid-bottom, right-bottom arrangement.</summary>
    LeftBottomMidBottomRightBottom = 3,
}

/// <summary>Camera calibration result.</summary>
public enum CalibrationResult
{
    /// <summary>Calibration initialized but not yet started.</summary>
    Init = 0,
    /// <summary>Calibration stopped.</summary>
    Stop = 1,
    /// <summary>Calibration completed successfully.</summary>
    CalibrateSuccess = 2,
    /// <summary>Calibration failed.</summary>
    CalibrateFailed = 3,
    /// <summary>Camera position was updated during calibration.</summary>
    CameraPositionUpdate = 4,
}

/// <summary>Smart camera mode flags.</summary>
[Flags]
public enum SmartCameraMask
{
    /// <summary>Manual PTZ control only.</summary>
    Manual = 1 << 0,
    /// <summary>Single-person focus tracking.</summary>
    SingleFocus = 1 << 1,
    /// <summary>Group focus (auto-frame for entire room).</summary>
    GroupFocus = 1 << 2,
    /// <summary>Multi-person focus tracking.</summary>
    MultiFocus = 1 << 3,
    /// <summary>Multi-stream (individual video feeds per speaker).</summary>
    MultiStream = 1 << 4,
    /// <summary>Intelligent Director multi-camera switching.</summary>
    Director = 1 << 5,
    /// <summary>Presenter focus tracking.</summary>
    PresenterFocus = 1 << 6,
}

/// <summary>Camera boundary adjust field.</summary>
public enum CameraBoundaryAdjustField
{
    /// <summary>Unknown field.</summary>
    Unknown = -1,
    /// <summary>Left boundary.</summary>
    Left = 0,
    /// <summary>Right boundary.</summary>
    Right = 1,
    /// <summary>Depth boundary.</summary>
    Depth = 2,
}

// ── Reactions ─────────────────────────────────────────────────────────────────

/// <summary>Non-verbal reaction feedback.</summary>
public enum ReactionFeedback
{
    /// <summary>No feedback active.</summary>
    None = 0,
    /// <summary>Raised hand.</summary>
    Hand = 1,
    /// <summary>"Yes" response.</summary>
    Yes = 2,
    /// <summary>"No" response.</summary>
    No = 3,
    /// <summary>"Go faster" response.</summary>
    Fast = 4,
    /// <summary>"Go slower" response.</summary>
    Slow = 5,
    /// <summary>"Coffee break" response.</summary>
    Coffee = 6,
}

/// <summary>Hand raise skin tone.</summary>
public enum HandSkinTone
{
    /// <summary>Default / no skin tone modifier.</summary>
    Unknown = 0,
    /// <summary>Fitzpatrick skin type 1 (lightest).</summary>
    Tone1 = 1,
    /// <summary>Fitzpatrick skin type 2.</summary>
    Tone2 = 2,
    /// <summary>Fitzpatrick skin type 3.</summary>
    Tone3 = 3,
    /// <summary>Fitzpatrick skin type 4.</summary>
    Tone4 = 4,
    /// <summary>Fitzpatrick skin type 5.</summary>
    Tone5 = 5,
    /// <summary>Fitzpatrick skin type 6 (darkest).</summary>
    Tone6 = 6,
}

/// <summary>Reaction on-screen intensity.</summary>
public enum ReactionIntensity
{
    /// <summary>Reaction emoji is hidden.</summary>
    Hidden = 0,
    /// <summary>Reaction emoji shown at full size.</summary>
    Full = 1,
    /// <summary>Reaction emoji shown at medium size.</summary>
    Medium = 2,
}

// ── Interpretation ────────────────────────────────────────────────────────────

/// <summary>Language interpretation channel.</summary>
public enum InterpretLanguage
{
    /// <summary>Original meeting audio (no interpretation).</summary>
    OriginalAudio = -1,
    /// <summary>English interpretation channel.</summary>
    English = 0,
    /// <summary>Chinese interpretation channel.</summary>
    Chinese = 1,
    /// <summary>Japanese interpretation channel.</summary>
    Japanese = 2,
    /// <summary>German interpretation channel.</summary>
    German = 3,
    /// <summary>French interpretation channel.</summary>
    French = 4,
    /// <summary>Russian interpretation channel.</summary>
    Russian = 5,
    /// <summary>Portuguese interpretation channel.</summary>
    Portuguese = 6,
    /// <summary>Spanish interpretation channel.</summary>
    Spanish = 7,
    /// <summary>Korean interpretation channel.</summary>
    Korean = 8,
}

// ── Devices ───────────────────────────────────────────────────────────────────

/// <summary>Audio/video device type.</summary>
public enum DeviceType
{
    /// <summary>Microphone (audio input) device.</summary>
    Microphone = 0,
    /// <summary>Speaker (audio output) device.</summary>
    Speaker = 1,
    /// <summary>Camera (video capture) device.</summary>
    Camera = 2,
}

/// <summary>Virtual audio device type.</summary>
public enum VirtualAudioDeviceType
{
    /// <summary>Unknown virtual audio device type.</summary>
    Unknown = -1,
    /// <summary>Virtual microphone input.</summary>
    Microphone = 0,
    /// <summary>Virtual speaker output.</summary>
    Speaker = 1,
}

/// <summary>Virtual audio device vendor.</summary>
public enum VirtualAudioDeviceVendor
{
    /// <summary>Audinate Dante audio-over-IP device.</summary>
    Dante = 0,
}

// ── Phone / SIP ───────────────────────────────────────────────────────────────

/// <summary>PSTN call-out status.</summary>
public enum PSTNCallOutStatus
{
    /// <summary>Status is unknown.</summary>
    Unknown = 0,
    /// <summary>Call is dialing out.</summary>
    Calling = 1,
    /// <summary>Remote end is ringing.</summary>
    Ringing = 2,
    /// <summary>Call accepted by the remote end.</summary>
    Accepted = 3,
    /// <summary>Remote end is busy.</summary>
    Busy = 4,
    /// <summary>Remote end is not available.</summary>
    NotAvailable = 5,
    /// <summary>Remote end hung up.</summary>
    UserHangUp = 6,
    /// <summary>Call failed for another reason.</summary>
    OtherFail = 7,
    /// <summary>Call connected and participant joined the meeting.</summary>
    JoinSuccess = 8,
    /// <summary>Call timed out.</summary>
    Timeout = 9,
    /// <summary>Zoom initiated a cancellation of the call.</summary>
    ZoomStartCancelCall = 10,
    /// <summary>Zoom has canceled the call.</summary>
    ZoomCallCanceled = 11,
    /// <summary>Zoom failed to cancel the call.</summary>
    ZoomCancelCallFail = 12,
    /// <summary>No answer from the remote end.</summary>
    NoAnswer = 13,
    /// <summary>Call blocked because host has not yet joined.</summary>
    BlockNoHost = 14,
    /// <summary>Call blocked due to high call-out rate.</summary>
    BlockHighRate = 15,
    /// <summary>Call blocked for calling too frequently.</summary>
    BlockTooFrequent = 16,
}

/// <summary>Room system protocol type.</summary>
public enum RoomSystemProtocolType
{
    /// <summary>Unknown protocol.</summary>
    Unknown = 0,
    /// <summary>H.323 video conferencing protocol.</summary>
    H323 = 1,
    /// <summary>Session Initiation Protocol (SIP).</summary>
    SIP = 2,
}

/// <summary>Legacy room system type.</summary>
public enum LegacyRoomSystemType
{
    /// <summary>No legacy system.</summary>
    None = 0,
    /// <summary>H.323 legacy system.</summary>
    H323 = 1,
    /// <summary>SIP legacy system.</summary>
    SIP = 2,
    /// <summary>Both H.323 and SIP.</summary>
    Both = 3,
}

/// <summary>Legacy room system encryption type.</summary>
public enum LegacyRoomSystemEncryptType
{
    /// <summary>Encryption disabled.</summary>
    No = 0,
    /// <summary>Encryption enabled.</summary>
    Yes = 1,
    /// <summary>Auto-negotiate encryption.</summary>
    Auto = 2,
}

// ── Calendar / Scheduling ─────────────────────────────────────────────────────

/// <summary>Origin of a meeting schedule.</summary>
public enum MeetingScheduleFrom
{
    /// <summary>Unknown origin.</summary>
    Unknown = -1,
    /// <summary>Scheduled via calendar integration.</summary>
    Calendar = 0,
    /// <summary>Scheduled from the Zoom Room scheduling display.</summary>
    ScheduleDisplay = 1,
    /// <summary>Started as an instant (ad-hoc) meeting.</summary>
    InstantMeeting = 2,
}

/// <summary>Zoom meeting item type.</summary>
public enum ZoomMeetingItemType
{
    /// <summary>Default meeting type.</summary>
    Default = 0,
    /// <summary>Zoom Events single-session.</summary>
    ZESingleSession = 1,
    /// <summary>Zoom Events multi-session.</summary>
    ZEMultiSession = 2,
    /// <summary>Zoom Events sub-session.</summary>
    ZESubSession = 3,
}

/// <summary>Schedule calendar event result.</summary>
public enum ScheduleCalendarEventResult
{
    /// <summary>Scheduling succeeded.</summary>
    Success = 0,
    /// <summary>Scheduling failed — unknown reason.</summary>
    FailUnknown = 1,
    /// <summary>Scheduling failed — meeting password is too weak.</summary>
    FailWeakPassword = 2,
}

/// <summary>Delete calendar event result.</summary>
public enum DeleteCalendarEventResult
{
    /// <summary>Deletion succeeded.</summary>
    Success = 0,
    /// <summary>Deletion failed because the calendar itself was deleted.</summary>
    FailByDeleteCalendar = 1,
    /// <summary>Deletion failed because the Zoom Room is in a meeting.</summary>
    FailByZRInMeeting = 2,
    /// <summary>Deletion failed — unknown reason.</summary>
    FailUnknown = 3,
}

/// <summary>List meetings result.</summary>
public enum ListMeetingResult
{
    /// <summary>Meeting list retrieved successfully.</summary>
    Success = 0,
    /// <summary>Unknown error retrieving meeting list.</summary>
    ErrorUnknown = 1,
    /// <summary>Google Calendar credentials are invalid.</summary>
    GoogleCalendarInvalidCredential = 2,
    /// <summary>Google Calendar daily API quota exceeded.</summary>
    GoogleCalendarDailyLimitExceeded = 3,
    /// <summary>Exchange Web Services credentials are invalid.</summary>
    EWSInvalidCredential = 4,
    /// <summary>Exchange Web Services authentication method is unsupported.</summary>
    EWSAuthMethodUnsupported = 5,
    /// <summary>Exchange mailbox folder not found.</summary>
    EWSFolderNotFound = 6,
    /// <summary>Exchange impersonation of the target user was denied.</summary>
    EWSImpersonateUserDenied = 7,
    /// <summary>Exchange mailbox does not exist.</summary>
    EWSNonExistentMailbox = 8,
    /// <summary>Calendar service is disconnected.</summary>
    CalendarServiceDisconnected = 9,
}

// ── Third-Party Meetings ──────────────────────────────────────────────────────

/// <summary>Third-party meeting service provider.</summary>
public enum ThirdPartyMeetingServiceProvider
{
    /// <summary>Invalid / none.</summary>
    Invalid = 0,
    /// <summary>Cisco Webex.</summary>
    Webex = 1,
    /// <summary>Microsoft Skype for Business.</summary>
    Skype = 2,
    /// <summary>LogMeIn GoToMeeting.</summary>
    GoToMeeting = 3,
    /// <summary>Microsoft Teams.</summary>
    Teams = 4,
    /// <summary>BlueJeans by Verizon.</summary>
    Bluejeans = 5,
    /// <summary>Google Meet (formerly Hangouts).</summary>
    GoogleHangouts = 6,
    /// <summary>Fuze conferencing.</summary>
    Fuze = 7,
    /// <summary>Meta Workrooms.</summary>
    MetaWorkrooms = 8,
    /// <summary>Tencent Meeting.</summary>
    Tencent = 9,
}


/// <summary>Which native notification a <see cref="EventArgs.PromptEventArgs"/> came from, and which answer applies.</summary>
public enum ZrcPromptKind
{
    None = 0,
    /// <summary>Type is <see cref="MeetingReminderType"/>; answer with ConfirmMeetingReminder.</summary>
    MeetingReminder = 1,
    /// <summary>Type is the customized disclaimer type; answer with ConfirmCustomizedMeetingReminder.</summary>
    CustomizedReminder = 2,
    /// <summary>Type64 is the combined consent type; answer with ConfirmCombinedConsent.</summary>
    CombinedConsent = 3,
    /// <summary>Type is <see cref="ConsentType"/> (ConsentId for Common); answer with ConfirmConsent.</summary>
    Consent = 4,
    /// <summary>Type is <see cref="PrivacyAlertType"/>, UserId is the <see cref="PrivacyAlertAction"/>; answer with HandlePrivacyAlert.</summary>
    PrivacyAlert = 5,
    /// <summary>Room empty for 30 minutes; AutoEndTime says when the meeting ends. Keep it with ContinueMeetingOnInactivity.</summary>
    InactiveDetection = 6,
    /// <summary>Type is <see cref="MeetingMessageEvent"/>. Informational.</summary>
    MessageEvent = 7,
    /// <summary>The host asked this room to start video; answer with AnswerHostRequestUnmuteVideo.</summary>
    AskStartVideo = 8,
    /// <summary>The host moved this room to breakout room SessionName; accept with JoinBreakoutRoom.</summary>
    BOSwitchRequest = 9,
    /// <summary>The host invited this room back to the main session; answer with ResponseHostInviteToMainSession.</summary>
    BOReturnToMainInvite = 10,
    /// <summary>Type is <see cref="WebinarRoleChangedState"/>. Informational.</summary>
    WebinarRoleChanged = 11,
}

/// <summary>Native MeetingReminderType.</summary>
public enum MeetingReminderType
{
    None = -1,
    StartOrJoinMeeting = 0,
    JoinExternalMeeting = 1,
    RecordingReminder = 2,
    RecordingDisclaimer = 3,
    ArchivingFail = 4,
    JoinWebinarAsPanelist = 5,
}

/// <summary>Native ConsentType.</summary>
public enum ConsentType
{
    None = -1,
    LiveStreaming = 0,
    PromotedToPanelist = 1,
    Archiving = 2,
    Ndi = 3,
    FocusModeStart = 4,
    FocusModeEnding = 5,
    AdminPayRemind = 6,
    PersonalAudioConference = 7,
    ZoomPhoneAcr = 8,
    HdmiConnected = 9,
    MeetingSummary = 10,
    MeetingQuery = 11,
    CustomAiCompanion = 12,
    Common = 13,
    SimuliveWebinar = 14,
    CustomRecording = 15,
}

/// <summary>Native PrivacyAlertType.</summary>
public enum PrivacyAlertType
{
    LiveTranscription = 0,
    NewLttCaption = 1,
}

/// <summary>Native PrivacyAlertAction.</summary>
public enum PrivacyAlertAction
{
    None = 0,
    Show = 1,
    Close = 2,
    ShowDisclaimer = 3,
    CloseDisclaimer = 4,
}

/// <summary>Native MessageEvent (video could not be started, and why).</summary>
public enum MeetingMessageEvent
{
    Unknown = 0,
    OpenVideoFailForHostStop = 1,
    OpenVideoFailForForceVBEnabledButUserOptionDisabled = 2,
    OpenVideoFailForForceVBEnabledButUserNoGreenScreen = 3,
    OpenVideoFailForForceVBEnabledButDeviceNotSupport = 4,
}

/// <summary>Native WebinarRoleChangedState.</summary>
public enum WebinarRoleChangedState
{
    None = 0,
    Promote = 1,
    Demote = 2,
}

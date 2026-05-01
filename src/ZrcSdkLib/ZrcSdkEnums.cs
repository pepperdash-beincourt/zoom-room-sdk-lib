namespace PepperDash.Zoom.ZrcSdk;

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
    TokenInvalid = 1,
    RefreshTokenFail = 2,
    RemovedByAdmin = 3,
    BetaBuildExpired = 4,
    ZRInE2EMeeting = 5,
}

// ── Meeting ───────────────────────────────────────────────────────────────────

/// <summary>Current Zoom Room meeting status.</summary>
public enum MeetingStatus
{
    NotInMeeting = 0,
    ConnectingToMeeting = 1,
    InMeeting = 2,
    LoggedOut = 3,
}

/// <summary>Meeting type.</summary>
public enum MeetingType
{
    None = 0,
    Meeting = 1,
    Sharing = 2,
    PSTNCallout = 3,
    Integration = 4,
}

/// <summary>Meeting encryption algorithm.</summary>
public enum MeetingEncryptionAlgorithm
{
    None = 0,
    AES256 = 1,
    AES256GCM = 2,
}

/// <summary>General encryption algorithm.</summary>
public enum EncryptionAlgorithm
{
    Unknown = -1,
    None = 0,
    AES256 = 1,
    AES256GCM = 2,
    AES256GCM3rd = 3,
}

/// <summary>Meeting exit reason.</summary>
public enum ExitMeetingReason
{
    None = 0,
    ByHost = 1,
    RemovedByHost = 2,
    JoinFromAnotherDevice = 3,
    JoinFromAnotherLocation = 4,
    MeetingOver = 5,
    FreeMeetingOver = 6,
    Unknown = 100,
}

// ── Participants ──────────────────────────────────────────────────────────────

/// <summary>Meeting user type.</summary>
public enum UserType
{
    Normal = 0,
    H323 = 1,
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
    Unknown = -1,
    CurrentConf = 0,
    MasterConf = 1,
    Backstage = 2,
    NewBO = 3,
}

/// <summary>Claim host result.</summary>
public enum ClaimHostResult
{
    Success = 0,
    InvalidHostKey = 1,
    UnknownError = 2,
}

/// <summary>Report issue type flags.</summary>
[Flags]
public enum ReportIssueType
{
    OffensiveIllegalAbusive = 0x40,
    SuicideSelfHarm = 0x80,
    PrivateInformation = 0x100,
    Spam = 0x200,
    CopyrightTrademarkInfringement = 0x400,
    Impersonation = 0x800,
    Other = 0x1,
}

/// <summary>Zoom Room Whiteboard user change type.</summary>
public enum ZRWUserChangeType
{
    Join = 0,
    Leave = 1,
}

/// <summary>Breakout room user status.</summary>
public enum BO_USER_STATUS
{
    Invalid = -1,
    InMaster = 1,
    InBO = 2,
    Leave = 3,
}

// ── Audio ─────────────────────────────────────────────────────────────────────

/// <summary>Audio connection type.</summary>
public enum AudioType
{
    None = 0,
    VoIP = 1,
    Phone = 2,
}

/// <summary>Reason the host asked ZR to unmute its audio.</summary>
public enum AskUnmuteAudioByHostType
{
    UnmuteAudio = 0,
    Spotlight = 1,
    ViewOnlyTalk = 2,
}

// ── Video ─────────────────────────────────────────────────────────────────────

/// <summary>Video sending source type.</summary>
public enum VideoSendingSourceType
{
    Device = 0,
    Lipsync = 1,
}

// ── Screen / Layout ───────────────────────────────────────────────────────────

/// <summary>Screen index on the Zoom Room.</summary>
public enum MeetingScreen
{
    Unknown = -1,
    First = 0,
    Second = 1,
    Third = 2,
    Confidence = 100,
}

/// <summary>What is shown on a screen panel.</summary>
public enum ScreenLayoutSourceType
{
    None = -1,
    ActiveVideo = 0,
    SelfVideo = 1,
    PinnedVideo = 2,
    Spotlight = 3,
    Gallery = 4,
    SharedContent = 5,
    Background = 6,
    LocalView = 7,
    ImmersiveView = 8,
    ZoomAppsView = 9,
    DynamicView = 10,
    ThumbnailView = 11,
    ThumbnailShareView = 12,
}

// ── Share ─────────────────────────────────────────────────────────────────────

/// <summary>Share source type.</summary>
public enum ShareSourceType
{
    Unknown = -1,
    Normal = 0,
    CloudWhiteboard = 1,
    CollaborationZapps = 2,
}

/// <summary>Sharing instruction display state.</summary>
public enum SharingInstructionDisplayState
{
    None = 0,
    Desktop = 1,
    IOS = 2,
    WhiteboardCamera = 3,
}

// ── Breakout Rooms ────────────────────────────────────────────────────────────

/// <summary>Breakout room lifecycle status.</summary>
public enum BO_STATUS
{
    Invalid = 0,
    Edit = 1,
    Started = 2,
    Stopping = 3,
    Ended = 4,
}

// ── Camera ────────────────────────────────────────────────────────────────────

/// <summary>Camera control event type.</summary>
public enum CameraControlType
{
    Start = 0,
    Continue = 1,
    Stop = 2,
    RequestedByRemote = 3,
    GaveUpByRemote = 4,
    ControlRequestToRemote = 5,
    ControlGaveUpToRemote = 6,
    ControlOpenCtrlRemotePanel = 7,
    ControlMovingSpeed = 8,
}

/// <summary>Camera PTZ action.</summary>
public enum CameraControlAction
{
    MoveUp = 0,
    MoveDown = 1,
    MoveLeft = 2,
    MoveRight = 3,
    ZoomIn = 4,
    ZoomOut = 5,
}

/// <summary>Camera calibration layout.</summary>
public enum CalibrationCameraLayout
{
    Unknown = -1,
    LeftTopMidBottomRightTop = 0,
    LeftBottomMidTopRightBottom = 1,
    LeftTopMidTopRightTop = 2,
    LeftBottomMidBottomRightBottom = 3,
}

/// <summary>Camera calibration result.</summary>
public enum CalibrationResult
{
    Init = 0,
    Stop = 1,
    CalibrateSuccess = 2,
    CalibrateFailed = 3,
    CameraPositionUpdate = 4,
}

/// <summary>Smart camera mode flags.</summary>
[Flags]
public enum SmartCameraMask
{
    Manual = 1 << 0,
    SingleFocus = 1 << 1,
    GroupFocus = 1 << 2,
    MultiFocus = 1 << 3,
    MultiStream = 1 << 4,
    Director = 1 << 5,
    PresenterFocus = 1 << 6,
}

/// <summary>Camera boundary adjust field.</summary>
public enum CameraBoundaryAdjustField
{
    Unknown = -1,
    Left = 0,
    Right = 1,
    Depth = 2,
}

// ── Reactions ─────────────────────────────────────────────────────────────────

/// <summary>Non-verbal reaction feedback.</summary>
public enum ReactionFeedback
{
    None = 0,
    Hand = 1,
    Yes = 2,
    No = 3,
    Fast = 4,
    Slow = 5,
    Coffee = 6,
}

/// <summary>Hand raise skin tone.</summary>
public enum HandSkinTone
{
    Unknown = 0,
    Tone1 = 1,
    Tone2 = 2,
    Tone3 = 3,
    Tone4 = 4,
    Tone5 = 5,
    Tone6 = 6,
}

/// <summary>Reaction on-screen intensity.</summary>
public enum ReactionIntensity
{
    Hidden = 0,
    Full = 1,
    Medium = 2,
}

// ── Interpretation ────────────────────────────────────────────────────────────

/// <summary>Language interpretation channel.</summary>
public enum InterpretLanguage
{
    OriginalAudio = -1,
    English = 0,
    Chinese = 1,
    Japanese = 2,
    German = 3,
    French = 4,
    Russian = 5,
    Portuguese = 6,
    Spanish = 7,
    Korean = 8,
}

// ── Devices ───────────────────────────────────────────────────────────────────

/// <summary>Audio/video device type.</summary>
public enum DeviceType
{
    Microphone = 0,
    Speaker = 1,
    Camera = 2,
}

/// <summary>Virtual audio device type.</summary>
public enum VirtualAudioDeviceType
{
    Unknown = -1,
    Microphone = 0,
    Speaker = 1,
}

/// <summary>Virtual audio device vendor.</summary>
public enum VirtualAudioDeviceVendor
{
    Dante = 0,
}

// ── Phone / SIP ───────────────────────────────────────────────────────────────

/// <summary>PSTN call-out status.</summary>
public enum PSTNCallOutStatus
{
    Unknown = 0,
    Calling = 1,
    Ringing = 2,
    Accepted = 3,
    Busy = 4,
    NotAvailable = 5,
    UserHangUp = 6,
    OtherFail = 7,
    JoinSuccess = 8,
    Timeout = 9,
    ZoomStartCancelCall = 10,
    ZoomCallCanceled = 11,
    ZoomCancelCallFail = 12,
    NoAnswer = 13,
    BlockNoHost = 14,
    BlockHighRate = 15,
    BlockTooFrequent = 16,
}

/// <summary>Room system protocol type.</summary>
public enum RoomSystemProtocolType
{
    Unknown = 0,
    H323 = 1,
    SIP = 2,
}

/// <summary>Legacy room system type.</summary>
public enum LegacyRoomSystemType
{
    None = 0,
    H323 = 1,
    SIP = 2,
    Both = 3,
}

/// <summary>Legacy room system encryption type.</summary>
public enum LegacyRoomSystemEncryptType
{
    No = 0,
    Yes = 1,
    Auto = 2,
}

// ── Calendar / Scheduling ─────────────────────────────────────────────────────

/// <summary>Origin of a meeting schedule.</summary>
public enum MeetingScheduleFrom
{
    Unknown = -1,
    Calendar = 0,
    ScheduleDisplay = 1,
    InstantMeeting = 2,
}

/// <summary>Zoom meeting item type.</summary>
public enum ZoomMeetingItemType
{
    Default = 0,
    ZESingleSession = 1,
    ZEMultiSession = 2,
    ZESubSession = 3,
}

/// <summary>Schedule calendar event result.</summary>
public enum ScheduleCalendarEventResult
{
    Success = 0,
    FailUnknown = 1,
    FailWeakPassword = 2,
}

/// <summary>Delete calendar event result.</summary>
public enum DeleteCalendarEventResult
{
    Success = 0,
    FailByDeleteCalendar = 1,
    FailByZRInMeeting = 2,
    FailUnknown = 3,
}

/// <summary>List meetings result.</summary>
public enum ListMeetingResult
{
    Success = 0,
    ErrorUnknown = 1,
    GoogleCalendarInvalidCredential = 2,
    GoogleCalendarDailyLimitExceeded = 3,
    EWSInvalidCredential = 4,
    EWSAuthMethodUnsupported = 5,
    EWSFolderNotFound = 6,
    EWSImpersonateUserDenied = 7,
    EWSNonExistentMailbox = 8,
    CalendarServiceDisconnected = 9,
}

// ── Third-Party Meetings ──────────────────────────────────────────────────────

/// <summary>Third-party meeting service provider.</summary>
public enum ThirdPartyMeetingServiceProvider
{
    Invalid = 0,
    Webex = 1,
    Skype = 2,
    GoToMeeting = 3,
    Teams = 4,
    Bluejeans = 5,
    GoogleHangouts = 6,
    Fuze = 7,
    MetaWorkrooms = 8,
    Tencent = 9,
}

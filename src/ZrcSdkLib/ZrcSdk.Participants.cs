using System.Runtime.InteropServices;
using PepperDash.Zoom.ZrcSdk.EventArgs;

namespace PepperDash.Zoom.ZrcSdk;

public partial class ZrcSdk
{
    // ── P/Invoke: ZrcParticipant flat struct (must mirror ZrcSdkWrapper_C.h exactly) ──

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcParticipantNative
    {
        public int userID;
        public int parentUserID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string userGUID;
        public int userType;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string userName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string pronouns;
        public int isMySelf;
        public int isHost;
        public int isOriginalOrAlternativeHost;
        public int isCohost;
        public int isGuest;
        public int isViewOnlyUser;
        public int isViewOnlyUserCanTalk;
        public int canRecord;
        public int isRecording;
        public int recordingDisabled;
        public int isInSilentMode;
        // AudioStatus
        public int audioType;
        public int audioMuted;
        // VideoStatus
        public int videoHasSource;
        public int videoReceiving;
        public int videoSending;
        public int videoCanControl;
        // CameraControlStatus
        public int cameraCanRequestControl;
        public int cameraAmIControlling;
        public int cameraCanSwitch;
        public int cameraCanMove;
        public int cameraCanZoom;
        // HandStatus
        public int handRaised;
        public int handSkinTone;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]  public string reactionEmoji;
        public int reactionFeedback;
        // Interpretation
        public int isInterpreter;
        public int interpreterLanguage;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]  public string interpreterLanguageID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]  public string interpreterLanguageName;
        // Misc
        public int isRemoteControlAdmin;
        public int isVirtualAssistant;
        public int isCompanionModeUser;
        public int isCompanionZRUser;
        // BreakoutRoomStatus
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string boSessionBID;
        public int boUserStatus;
        public int boSupportForceJoinLeave;
        public int boSupportSelfChoose;
        public int boSupportCohostStartStop;
        // Streaming / webinar
        public int canPinMultiVideo;
        public int isSupportGreenRoom;
        public int isInGreenRoom;
        // AudioControlStatus
        public int audioCanRequestControl;
        public int audioCanBeRequested;
        public int audioAmIControlling;
        // RTMP / special
        public int isRTMPUser;
        public int isActiveRTMPUser;
        public int isSimuliveUser;
        // Timezone
        public int timeZoneOffsetMinutes;
        public int isSupportDisplayLocalTime;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string attendeeJid;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcParticipantListCallbackDelegate(
        IntPtr participantsPtr,
        int count,
        int needCleanUp,
        int sessionType,
        IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetParticipantListCallback(
        IntPtr handle, ZrcParticipantListCallbackDelegate? cb, IntPtr userData);

    // ── Delegates ──────────────────────────────────────────────────────────────

    private SdkEventCallbackDelegate?          _participantCountCallbackDelegate;
    private SdkEventCallbackDelegate?          _hostChangedCallbackDelegate;
    private ZrcParticipantListCallbackDelegate? _participantListCallbackDelegate;

    // ── Events ─────────────────────────────────────────────────────────────────

    /// <summary>
    /// Fired when the participant count changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is the current count.
    /// </summary>
    public event EventHandler<SdkEventArgs>? ParticipantCount;

    /// <summary>
    /// Fired when the meeting host changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if this device is now the host, 0 otherwise.
    /// </summary>
    public event EventHandler<SdkEventArgs>? HostChanged;

    /// <summary>
    /// Fired when the full participant list is initialised at meeting join.
    /// <see cref="ParticipantListEventArgs.NeedCleanUp"/> is <see langword="true"/>;
    /// replace your local list with <see cref="ParticipantListEventArgs.Participants"/>.
    /// </summary>
    public event EventHandler<ParticipantListEventArgs>? ParticipantsInitialized;

    /// <summary>Fired when one or more participants join the meeting.</summary>
    public event EventHandler<ParticipantListEventArgs>? UserJoined;

    /// <summary>Fired when one or more participants leave the meeting.</summary>
    public event EventHandler<ParticipantListEventArgs>? UserLeft;

    /// <summary>Fired when participant properties (audio, video, hand, etc.) change.</summary>
    public event EventHandler<ParticipantListEventArgs>? UserUpdated;

    // ── Initializer ────────────────────────────────────────────────────────────

    partial void InitializeParticipantCallbacks()
    {
        _participantCountCallbackDelegate = OnParticipantCountCallback;
        _hostChangedCallbackDelegate      = OnHostChangedCallback;
        _participantListCallbackDelegate  = OnParticipantListCallback;

        ZrcSdk_SetParticipantCountCallback(_handle, _participantCountCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetHostChangedCallback(_handle,      _hostChangedCallbackDelegate,      IntPtr.Zero);
        ZrcSdk_SetParticipantListCallback(_handle,  _participantListCallbackDelegate,  IntPtr.Zero);
    }

    // ── Public API ─────────────────────────────────────────────────────────────

    /// <summary>
    /// Returns the last-known participant count, kept up to date by the <see cref="ParticipantCount"/> event.
    /// </summary>
    public int GetParticipantCount()
    {
        ThrowIfDisposed();
        return ZrcSdk_GetParticipantCount(_handle);
    }

    // ── Callback handlers ──────────────────────────────────────────────────────

    private void OnParticipantCountCallback(string message, int count, IntPtr userData) =>
        ParticipantCount?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = count });

    private void OnHostChangedCallback(string message, int amIHost, IntPtr userData) =>
        HostChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = amIHost });

    private void OnParticipantListCallback(IntPtr participantsPtr, int count,
                                           int needCleanUp, int sessionType, IntPtr userData)
    {
        var participants = MarshalParticipants(participantsPtr, count);
        var args = new ParticipantListEventArgs
        {
            Participants = participants,
            TotalCount   = count,
            NeedCleanUp  = needCleanUp != 0,
            Session      = (ConfSessionType)sessionType,
        };

        if (needCleanUp != 0)
            ParticipantsInitialized?.Invoke(this, args);
        else if (count > 0 && participants.Length > 0)
            UserJoined?.Invoke(this, args); // may also be leave/update — routed by needCleanUp pattern
    }

    private static ParticipantInfo[] MarshalParticipants(IntPtr ptr, int count)
    {
        if (ptr == IntPtr.Zero || count <= 0)
            return Array.Empty<ParticipantInfo>();

        var result = new ParticipantInfo[count];
        int stride = Marshal.SizeOf<ZrcParticipantNative>();

        for (int i = 0; i < count; i++)
        {
            var native = Marshal.PtrToStructure<ZrcParticipantNative>(ptr + i * stride);
            result[i] = new ParticipantInfo
            {
                UserID                      = native.userID,
                ParentUserID                = native.parentUserID,
                UserGUID                    = native.userGUID ?? string.Empty,
                UserType                    = (UserType)native.userType,
                UserName                    = native.userName ?? string.Empty,
                Pronouns                    = native.pronouns ?? string.Empty,
                IsMySelf                    = native.isMySelf != 0,
                IsHost                      = native.isHost != 0,
                IsOriginalOrAlternativeHost = native.isOriginalOrAlternativeHost != 0,
                IsCohost                    = native.isCohost != 0,
                IsGuest                     = native.isGuest != 0,
                IsViewOnlyUser              = native.isViewOnlyUser != 0,
                IsViewOnlyUserCanTalk       = native.isViewOnlyUserCanTalk != 0,
                CanRecord                   = native.canRecord != 0,
                IsRecording                 = native.isRecording != 0,
                RecordingDisabled           = native.recordingDisabled != 0,
                IsInSilentMode              = native.isInSilentMode != 0,
                AudioType                   = (AudioType)native.audioType,
                AudioMuted                  = native.audioMuted != 0,
                VideoHasSource             = native.videoHasSource != 0,
                VideoReceiving             = native.videoReceiving != 0,
                VideoSending               = native.videoSending != 0,
                VideoCanControl            = native.videoCanControl != 0,
                CameraCanRequestControl    = native.cameraCanRequestControl != 0,
                CameraAmIControlling       = native.cameraAmIControlling != 0,
                CameraCanSwitch            = native.cameraCanSwitch != 0,
                CameraCanMove              = native.cameraCanMove != 0,
                CameraCanZoom              = native.cameraCanZoom != 0,
                HandRaised                 = native.handRaised != 0,
                HandSkinTone               = (HandSkinTone)native.handSkinTone,
                ReactionEmoji              = native.reactionEmoji ?? string.Empty,
                ReactionFeedback           = (ReactionFeedback)native.reactionFeedback,
                IsInterpreter              = native.isInterpreter != 0,
                InterpreterLanguage        = (InterpretLanguage)native.interpreterLanguage,
                InterpreterLanguageID      = native.interpreterLanguageID ?? string.Empty,
                InterpreterLanguageName    = native.interpreterLanguageName ?? string.Empty,
                IsRemoteControlAdmin       = native.isRemoteControlAdmin != 0,
                IsVirtualAssistant         = native.isVirtualAssistant != 0,
                IsCompanionModeUser        = native.isCompanionModeUser != 0,
                IsCompanionZRUser          = native.isCompanionZRUser != 0,
                BoSessionBID               = native.boSessionBID ?? string.Empty,
                BoUserStatus               = (BO_USER_STATUS)native.boUserStatus,
                BoSupportForceJoinLeave    = native.boSupportForceJoinLeave != 0,
                BoSupportSelfChoose        = native.boSupportSelfChoose != 0,
                BoSupportCohostStartStop   = native.boSupportCohostStartStop != 0,
                CanPinMultiVideo           = native.canPinMultiVideo != 0,
                IsSupportGreenRoom         = native.isSupportGreenRoom != 0,
                IsInGreenRoom              = native.isInGreenRoom != 0,
                AudioCanRequestControl     = native.audioCanRequestControl != 0,
                AudioCanBeRequested        = native.audioCanBeRequested != 0,
                AudioAmIControlling        = native.audioAmIControlling != 0,
                IsRTMPUser                 = native.isRTMPUser != 0,
                IsActiveRTMPUser           = native.isActiveRTMPUser != 0,
                IsSimuliveUser             = native.isSimuliveUser != 0,
                TimeZoneOffsetMinutes      = native.timeZoneOffsetMinutes,
                IsSupportDisplayLocalTime  = native.isSupportDisplayLocalTime != 0,
                AttendeeJid                = native.attendeeJid ?? string.Empty,
            };
        }

        return result;
    }
}


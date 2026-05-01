#include "ZrcSdkWrapper_C.h"
#include <cstdint>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include "IZRCSDK.h"
#include "IZoomRoomsService.h"
#include "IMeetingService.h"
#include "IPreMeetingService.h"
#include "ServiceComponents/IMeetingAudioHelper.h"
#include "ServiceComponents/IMeetingVideoHelper.h"
#include "ServiceComponents/IParticipantHelper.h"
#include "ServiceComponents/IRecordingHelper.h"
#include "ServiceComponents/IControlSystemHelper.h"

using namespace ZRCSDK;

// Sink classes contain many empty pure-virtual stubs that intentionally ignore their parameters.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ─── Forward declarations ──────────────────────────────────────────────────────
struct ZrcSdkInstance;

// ─── IZRCSDKSink — device/app identity ───────────────────────────────────────
class ZrcSDKSink : public IZRCSDKSink
{
public:
    std::string contentDir;
    explicit ZrcSDKSink(const std::string& dir) : contentDir(dir) {}

    std::string OnGetDeviceManufacturer() override { return "PepperDash"; }
    std::string OnGetDeviceModel() override { return "CP4N"; }
    std::string OnGetDeviceSerialNumber() override { return "CP4N-001"; }
    std::string OnGetDeviceMacAddress() override { return "00:00:00:00:00:00"; }
    std::string OnGetDeviceIP() override { return "127.0.0.1"; }
    std::string OnGetFirmwareVersion() override { return "1.0.0"; }
    std::string OnGetAppName() override { return "ZrcSdkTest"; }
    std::string OnGetAppVersion() override { return "1.0.0"; }
    std::string OnGetAppDeveloper() override { return "PepperDash"; }
    std::string OnGetAppContact() override { return "support@pepperdash.com"; }
    std::string OnGetAppContentDirPath() override { return contentDir; }
    bool OnPromptToInputUserNamePasswordForProxyServer(
        const std::string&, uint32_t, const std::string&) override { return true; }
};

// ─── IZoomRoomsServiceSink — pair result ──────────────────────────────────────
class ZrcZoomRoomsServiceSink : public IZoomRoomsServiceSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcZoomRoomsServiceSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnPairRoomResult(int32_t result) override;
    void OnRoomUnpairedReason(RoomUnpairedReason reason) override {}
};

// ─── IPreMeetingServiceSink — connection state ────────────────────────────────
class ZrcPreMeetingServiceSink : public IPreMeetingServiceSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcPreMeetingServiceSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnZRConnectionStateChanged(ConnectionState connectionState) override;
    void OnShutdownOSNot(bool restartOS) override {}
};

// ─── IMeetingServiceSink — meeting lifecycle ──────────────────────────────────
class ZrcMeetingServiceSink : public IMeetingServiceSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcMeetingServiceSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnStartMeetingResult(int32_t result) override;
    void OnStartPmiResult(int32_t result, const std::string& meetingNumber, MeetingType meetingType) override;
    void OnStartPmiNotification(bool success) override {}
    void OnUpdateMeetingStatus(MeetingStatus meetingStatus) override;
    void OnConfReadyNotification() override {}
    void OnUpdateMeetingInfoNotification(const MeetingInfo& meetingInfo) override {}
    void OnExitMeetingNotification(int32_t result, ExitMeetingReason reason) override;
    void OnMeetingErrorNotification(const MeetingErrorInfo& errorInfo) override;
    void OnMeetingEndedNotification(const MeetingErrorInfo& errorInfo) override;
    void OnReceiveMeetingInviteNotification(const MeetingInvitationInfo& invitation) override;
    void OnAnswerMeetingInviteResponse(int32_t result, const MeetingInvitationInfo& invitation, bool accepted) override {}
    void OnTreatedMeetingInviteNotification(const MeetingInvitationInfo& invitation, bool accepted) override {}
    void OnStartMeetingWithHostKeyResult(int32_t result) override;
    void OnUpdateDataCenterRegionNotification(const DataCenterRegion& dcRegion) override {}
    void OnUpdateE2ESecurityCode(const E2ESecurityCode& code) override {}
    void OnBandwidthLimitNotification(const BandwidthLimitInfo& info) override {}
    void OnSendMeetingInviteEmailNotification(int32_t result) override {}
    void OnSetRoomTempDisplayNameNotification(bool isShow) override {}
    void OnMeetingNeedsPasswordNotification(bool showPasswordDialog, bool wrongAndRetry, const ConfDeviceLockStatus& lockStatus) override;
    void OnConfDeviceLockStatusNotification(const ConfDeviceLockStatus& status) override {}
    void OnJBHWaitingHostNotification(bool showWaitForHostDialog, WaitingHostReason reason) override;
    void OnE2eeMeetingStatusNotification(const E2eeMeetingStatus& e2eeMeetingStatus) override {}
    void OnMeshInfoNotification(const MeshInfoNotification& meshInfo) override {}
    void OnMeetingWillStopAutomatically() override {}
    void OnExtendMeetingResult(int32_t extendMins) override {}
    void OnConfirmPersonalLink(const std::string& personalLink) override {}
};

// ─── IMeetingAudioHelperSink — audio status ───────────────────────────────────
class ZrcMeetingAudioHelperSink : public IMeetingAudioHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcMeetingAudioHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnUpdateMyAudioStatus(const AudioStatus& audioStatus) override;
    void OnMuteUserAudioNotification(int32_t userID, const AudioStatus& audioStatus) override {}
    void OnMuteOnEntryNotification(bool isMuteOnEntry) override;
    void OnAskUnmuteAudioByHostNotification(bool show, AskUnmuteAudioByHostType type) override {}
    void OnAllowAttendeesUnmuteThemselvesNotification(bool canAttendeesUnmuteThemselves) override {}
    void OnEnablePlayJoinOrLeaveChimeNotification(bool enable) override {}
    void OnUpdateAudioTroubleShootingStatus(const AudioTroubleShootingStatus& status) override {}
    void OnFEACApproveNotification(int32_t farEndUserID, const std::string& farEndUserName) override {}
    void OnFEACDeclineNotification(int32_t farEndUserID, const std::string& farEndUserName) override {}
    void OnFEACTakeOverNotification(int32_t farEndUserID, const std::string& farEndUserName, int32_t controllingUserID, const std::string& controllingUserName) override {}
    void OnFEACMicListChangedNotification(int32_t farEndUserID, const std::vector<FarEndAudioDeviceInfo>& micList) override {}
    void OnFEACSpeakerListChangedNotification(int32_t farEndUserID, const std::vector<FarEndAudioDeviceInfo>& speakerList) override {}
    void OnFEACMuteStateChangedNotification(int32_t farEndUserID, bool muteState) override {}
    void OnFEACUnmuteDisabledByHostNotification(int32_t farEndUserID) override {}
    void OnFEACRequestNotification(int32_t requesterUserID, const std::string& requesterUserName) override {}
    void OnFEACGiveUpNotification(int32_t requesterUserID, const std::string& requesterUserName) override {}
    void OnFEACApproveControlRequestNotification(int32_t requesterUserID) override {}
    void OnFEACDeclineControlRequestNotification(int32_t requesterUserID) override {}
};

// ─── IParticipantHelperSink — participant events ──────────────────────────────
class ZrcParticipantHelperSink : public IParticipantHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcParticipantHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnInitMeetingParticipants(const std::vector<MeetingParticipant>& participants, int32_t totalParticipantsCount,
                                   bool needCleanUpUserList, ConfSessionType session) override;
    void OnUserJoin(const std::vector<MeetingParticipant>& participants, ConfSessionType session) override;
    void OnUserLeave(const std::vector<MeetingParticipant>& participants, ConfSessionType session) override;
    void OnUserUpdate(const std::vector<MeetingParticipant>& participants, ConfSessionType session) override;
    void OnHostChangedNotification(int32_t hostUserID, bool amIHost, ConfSessionType session) override;
    void OnMeetingParticipantsChanged(ConfSessionType session) override {}
    void OnUpdateHideProfilePictures(bool isHideProfilePictures) override {}
    void OnHideFullRoomViewNotification(const std::vector<int32_t>& userIDs) override {}
    void OnClaimHostNotification(ClaimHostResult result) override {}
    void OnUpdateSharingAnnotationInfo(bool support, bool enable) override {}
    void OnAllowAttendeesRenameThemselvesNotification(bool allow) override {}
    void OnAllowAttendeesShareWhiteboardsNotification(bool isSupported, bool isAllowed) override {}
    void OnAllowRaiseHandForAttendeeNotification(bool canRaiseHandForAttendee) override {}
    void OnUpdateOnZRWUserChangeNotification(ZRWUserChangeType type, int32_t zrwUserID) override {}
    void OnUpdateHasRemoteControlAdmin(bool isAdminExist) override {}
    void OnUpdateHasRemoteControlAssistant(bool isAssistantExist) override {}
    void OnDownloadingFinished(const std::string& localFilePath, uint32_t result) override {}
    void OnShowParticipantLocalTimeNotification(bool isShowing) override {}
};

// ─── IRecordingHelperSink — recording status ─────────────────────────────────
class ZrcRecordingHelperSink : public IRecordingHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcRecordingHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnUpdateMeetingRecordingInfo(const MeetingRecordingInfo& recordingInfo) override;
    void OnMeetingCloudRecordingErrorNotification(bool show, MeetingRecordingError errorCode, bool hasCMREdit, uint64_t gracePeriodDate) override {}
    void OnMeetingCloudRecordingErrorNotification(bool result, const std::string& reason) override {}
    void OnNeedPromptStartRecordingDisclaimerUpdate(bool need) override {}
    void OnQueryMeetingCloudRecordingNotification(MeetingRecordingError errorCode, bool hasCMREdit) override {}
    void OnUpdateMeetingUserRecordingStatus(int32_t userID, bool canRecord, bool isRecording, bool isLocalRecordingDisabled) override {}
    void OnSetRecordingNotificationEmailNotification(int32_t result) override {}
    void OnSetMeetingRecordingResult(int32_t result, const std::string& recordingNotificationEmail, RecordingRequestType type) override;
    void OnUpdateRecordingPermission(const std::vector<RecordPermissionInfo>& info) override {}
    void OnReceiveRecordingRequest(const RecordingRequestInfo& info) override {}
};

// ─── IControlSystemHelperSink — ZRCS events ──────────────────────────────────
class ZrcControlSystemHelperSink : public IControlSystemHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcControlSystemHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnEnableZRCSNotification(bool enable) override;
    void OnUpdateZRCSDeviceList(ControlSystemUpdateDeviceType type, const ControlSystemDeviceList& list) override {}
    void OnUpdateZRCSSceneList(const std::vector<ControlSystemSceneInfo>& scenes) override {}
};

#pragma GCC diagnostic pop

// ─── ZrcSdkInstance — holds all SDK state ────────────────────────────────────
struct ZrcSdkInstance
{
    IZRCSDK*             pNativeSDK;
    bool                 bInitialized;

    ZrcSDKSink*                 pSdkSink;
    IZoomRoomsService*          pZoomRoomsService;
    ZrcZoomRoomsServiceSink*    pZoomRoomsServiceSink;
    IPreMeetingService*         pPreMeetingService;
    ZrcPreMeetingServiceSink*   pPreMeetingServiceSink;
    IMeetingService*            pMeetingService;
    ZrcMeetingServiceSink*      pMeetingServiceSink;
    ZrcMeetingAudioHelperSink*  pAudioHelperSink;
    ZrcParticipantHelperSink*   pParticipantHelperSink;
    ZrcRecordingHelperSink*     pRecordingHelperSink;
    ZrcControlSystemHelperSink* pControlSystemHelperSink;

    int participantCount;

    SdkEventCallback initializedCallback;           void* initializedUserData;
    SdkEventCallback meetingStateChangedCallback;   void* meetingStateChangedUserData;
    SdkEventCallback errorCallback;                 void* errorUserData;
    SdkEventCallback instantMeetingStartedCallback; void* instantMeetingStartedUserData;
    SdkEventCallback pairRoomResultCallback;        void* pairRoomResultUserData;
    SdkEventCallback meetingStatusCallback;         void* meetingStatusUserData;
    SdkEventCallback startPmiResultCallback;        void* startPmiResultUserData;
    SdkEventCallback exitMeetingCallback;           void* exitMeetingUserData;
    SdkEventCallback meetingNeedsPasswordCallback;  void* meetingNeedsPasswordUserData;
    SdkEventCallback meetingInviteCallback;         void* meetingInviteUserData;
    SdkEventCallback audioStatusCallback;           void* audioStatusUserData;
    SdkEventCallback muteOnEntryCallback;           void* muteOnEntryUserData;
    SdkEventCallback participantCountCallback;      void* participantCountUserData;
    SdkEventCallback hostChangedCallback;           void* hostChangedUserData;
    SdkEventCallback recordingStatusCallback;       void* recordingStatusUserData;
    SdkEventCallback controlSystemEnabledCallback;  void* controlSystemEnabledUserData;
    ZrcParticipantListCallback participantListCallback; void* participantListUserData;

    ZrcSdkInstance()
        : pNativeSDK(nullptr), bInitialized(false)
        , pSdkSink(nullptr), pZoomRoomsService(nullptr), pZoomRoomsServiceSink(nullptr)
        , pPreMeetingService(nullptr), pPreMeetingServiceSink(nullptr)
        , pMeetingService(nullptr), pMeetingServiceSink(nullptr)
        , pAudioHelperSink(nullptr), pParticipantHelperSink(nullptr)
        , pRecordingHelperSink(nullptr), pControlSystemHelperSink(nullptr)
        , participantCount(0)
        , initializedCallback(nullptr), initializedUserData(nullptr)
        , meetingStateChangedCallback(nullptr), meetingStateChangedUserData(nullptr)
        , errorCallback(nullptr), errorUserData(nullptr)
        , instantMeetingStartedCallback(nullptr), instantMeetingStartedUserData(nullptr)
        , pairRoomResultCallback(nullptr), pairRoomResultUserData(nullptr)
        , meetingStatusCallback(nullptr), meetingStatusUserData(nullptr)
        , startPmiResultCallback(nullptr), startPmiResultUserData(nullptr)
        , exitMeetingCallback(nullptr), exitMeetingUserData(nullptr)
        , meetingNeedsPasswordCallback(nullptr), meetingNeedsPasswordUserData(nullptr)
        , meetingInviteCallback(nullptr), meetingInviteUserData(nullptr)
        , audioStatusCallback(nullptr), audioStatusUserData(nullptr)
        , muteOnEntryCallback(nullptr), muteOnEntryUserData(nullptr)
        , participantCountCallback(nullptr), participantCountUserData(nullptr)
        , hostChangedCallback(nullptr), hostChangedUserData(nullptr)
        , recordingStatusCallback(nullptr), recordingStatusUserData(nullptr)
        , controlSystemEnabledCallback(nullptr), controlSystemEnabledUserData(nullptr)
        , participantListCallback(nullptr), participantListUserData(nullptr)
    {}

    void Raise(SdkEventCallback cb, void* ud, const char* msg, int code) { if (cb) cb(msg, code, ud); }
    void RaiseInitializedEvent(const char* msg, int code)         { Raise(initializedCallback, initializedUserData, msg, code); }
    void RaiseMeetingStateChangedEvent(const char* msg, int code) { Raise(meetingStateChangedCallback, meetingStateChangedUserData, msg, code); }
    void RaiseErrorEvent(const char* msg, int code)               { Raise(errorCallback, errorUserData, msg, code); }
    void RaiseInstantMeetingStartedEvent(const char* num, int r)  { Raise(instantMeetingStartedCallback, instantMeetingStartedUserData, num, r); }
    void RaisePairRoomResultEvent(int result)                     { Raise(pairRoomResultCallback, pairRoomResultUserData, "", result); }
    void RaiseMeetingStatusEvent(const char* msg, int status)     { Raise(meetingStatusCallback, meetingStatusUserData, msg, status); }
    void RaiseStartPmiResultEvent(const char* num, int result)    { Raise(startPmiResultCallback, startPmiResultUserData, num, result); }
    void RaiseExitMeetingEvent(int result, int reason)            { Raise(exitMeetingCallback, exitMeetingUserData, "", result | (reason << 8)); }
    void RaiseMeetingNeedsPasswordEvent(int wrongAndRetry)        { Raise(meetingNeedsPasswordCallback, meetingNeedsPasswordUserData, "", wrongAndRetry); }
    void RaiseMeetingInviteEvent(const char* callerName)          { Raise(meetingInviteCallback, meetingInviteUserData, callerName, 0); }
    void RaiseAudioStatusEvent(int isMuted)                       { Raise(audioStatusCallback, audioStatusUserData, "", isMuted); }
    void RaiseMuteOnEntryEvent(int enabled)                       { Raise(muteOnEntryCallback, muteOnEntryUserData, "", enabled); }
    void RaiseParticipantCountEvent(int count)                    { Raise(participantCountCallback, participantCountUserData, "", count); }
    void RaiseHostChangedEvent(int amIHost)                       { Raise(hostChangedCallback, hostChangedUserData, "", amIHost); }
    void RaiseRecordingStatusEvent(int isRecording)               { Raise(recordingStatusCallback, recordingStatusUserData, "", isRecording); }
    void RaiseControlSystemEnabledEvent(int enabled)              { Raise(controlSystemEnabledCallback, controlSystemEnabledUserData, "", enabled); }

    void RaiseParticipantListEvent(const std::vector<MeetingParticipant>& participants,
                                   int total, bool needCleanUp, ConfSessionType session);
};

// ─── Sink implementations ─────────────────────────────────────────────────────

// Helper: copy std::string into a fixed char array, always null-terminated.
static void strncpy_safe(char* dst, const std::string& src, size_t maxLen)
{
    size_t len = src.size() < maxLen - 1 ? src.size() : maxLen - 1;
    memcpy(dst, src.c_str(), len);
    dst[len] = '\0';
}

// Helper: flatten a C++ MeetingParticipant to the C ZrcParticipant struct.
static void FlattenParticipant(const MeetingParticipant& src, ZrcParticipant& dst)
{
    memset(&dst, 0, sizeof(ZrcParticipant));
    dst.userID       = src.userID;
    dst.parentUserID = src.parentUserID;
    strncpy_safe(dst.userGUID, src.userGUID, sizeof(dst.userGUID));
    dst.userType     = (int32_t)src.userType;
    strncpy_safe(dst.userName, src.userName, sizeof(dst.userName));
    strncpy_safe(dst.pronouns, src.pronouns, sizeof(dst.pronouns));
    dst.isMySelf                      = src.isMySelf ? 1 : 0;
    dst.isHost                        = src.isHost ? 1 : 0;
    dst.isOriginalOrAlternativeHost   = src.isOriginalOrAlternativeHost ? 1 : 0;
    dst.isCohost                      = src.isCohost ? 1 : 0;
    dst.isGuest                       = src.isGuest ? 1 : 0;
    dst.isViewOnlyUser                = src.isViewOnlyUser ? 1 : 0;
    dst.isViewOnlyUserCanTalk         = src.isViewOnlyUserCanTalk ? 1 : 0;
    dst.canRecord                     = src.canRecord ? 1 : 0;
    dst.isRecording                   = src.isRecording ? 1 : 0;
    dst.recordingDisabled             = src.recordingDisabled ? 1 : 0;
    dst.isInSilentMode                = src.isInSilentMode ? 1 : 0;
    // AudioStatus
    dst.audioType   = (int32_t)src.audioStatus.audioType;
    dst.audioMuted  = src.audioStatus.isMuted ? 1 : 0;
    // VideoStatus
    dst.videoHasSource  = src.videoStatus.hasSource ? 1 : 0;
    dst.videoReceiving  = src.videoStatus.receiving ? 1 : 0;
    dst.videoSending    = src.videoStatus.sending ? 1 : 0;
    dst.videoCanControl = src.videoStatus.canControl ? 1 : 0;
    // CameraControlStatus
    dst.cameraCanRequestControl = src.cameraControlStatus.canIRequestControl ? 1 : 0;
    dst.cameraAmIControlling    = src.cameraControlStatus.amIControlling ? 1 : 0;
    dst.cameraCanSwitch         = src.cameraControlStatus.canSwitchCamera ? 1 : 0;
    dst.cameraCanMove           = src.cameraControlStatus.canMoveCamera ? 1 : 0;
    dst.cameraCanZoom           = src.cameraControlStatus.canZoomCamera ? 1 : 0;
    // HandStatus
    dst.handRaised  = src.handStatus.handRaised ? 1 : 0;
    dst.handSkinTone = (int32_t)src.handStatus.skinTone;
    strncpy_safe(dst.reactionEmoji, src.reactionEmoji, sizeof(dst.reactionEmoji));
    dst.reactionFeedback = (int32_t)src.reactionFeedback;
    // Interpretation
    dst.isInterpreter    = src.isInterpreter ? 1 : 0;
    dst.interpreterLanguage = (int32_t)src.activeInterpreterLanguage.language;
    strncpy_safe(dst.interpreterLanguageID, src.activeInterpreterLanguage.languageID, sizeof(dst.interpreterLanguageID));
    strncpy_safe(dst.interpreterLanguageName, src.activeInterpreterLanguage.displayName, sizeof(dst.interpreterLanguageName));
    // Misc
    dst.isRemoteControlAdmin = src.isRemoteControlAdmin ? 1 : 0;
    dst.isVirtualAssistant   = src.isVirtualAssistant ? 1 : 0;
    dst.isCompanionModeUser  = src.isCompanionModeUser ? 1 : 0;
    dst.isCompanionZRUser    = src.isCompanionZRUser ? 1 : 0;
    // BreakoutRoomStatus
    strncpy_safe(dst.boSessionBID, src.breakoutRoomStatus.sessionBID, sizeof(dst.boSessionBID));
    dst.boUserStatus              = (int32_t)src.breakoutRoomStatus.userStatus;
    dst.boSupportForceJoinLeave   = src.breakoutRoomStatus.supportForceJoinLeave ? 1 : 0;
    dst.boSupportSelfChoose       = src.breakoutRoomStatus.supportSelfChooseRoom ? 1 : 0;
    dst.boSupportCohostStartStop  = src.breakoutRoomStatus.supportCohostStartStopBO ? 1 : 0;
    // Streaming / webinar
    dst.canPinMultiVideo    = src.canPinMultiVideo ? 1 : 0;
    dst.isSupportGreenRoom  = src.isSupportGreenRoom ? 1 : 0;
    dst.isInGreenRoom       = src.isInGreenRoom ? 1 : 0;
    // AudioControlStatus
    dst.audioCanRequestControl = src.audioControlStatus.canIRequestAudioControl ? 1 : 0;
    dst.audioCanBeRequested    = src.audioControlStatus.canBeRequestedAudioControl ? 1 : 0;
    dst.audioAmIControlling    = src.audioControlStatus.amIControllingAudio ? 1 : 0;
    // RTMP / special
    dst.isRTMPUser        = src.isRTMPUser ? 1 : 0;
    dst.isActiveRTMPUser  = src.isActiveRTMPUser ? 1 : 0;
    dst.isSimuliveUser    = src.isSimuliveUser ? 1 : 0;
    // Timezone
    dst.timeZoneOffsetMinutes    = src.timeZoneOffsetMinutes;
    dst.isSupportDisplayLocalTime = src.isSupportDisplayLocalTime ? 1 : 0;
    strncpy_safe(dst.attendeeJid, src.attendeeJid, sizeof(dst.attendeeJid));
}

void ZrcSdkInstance::RaiseParticipantListEvent(const std::vector<MeetingParticipant>& participants,
                                               int total, bool needCleanUp, ConfSessionType session)
{
    if (!participantListCallback) return;
    std::vector<ZrcParticipant> flat(participants.size());
    for (size_t i = 0; i < participants.size(); ++i)
        FlattenParticipant(participants[i], flat[i]);
    participantListCallback(flat.empty() ? nullptr : flat.data(),
                            (int)flat.size(), needCleanUp ? 1 : 0,
                            (int)session, participantListUserData);
}

void ZrcZoomRoomsServiceSink::OnPairRoomResult(int32_t result)
{
    if (owner) owner->RaisePairRoomResultEvent((int)result);
}

void ZrcPreMeetingServiceSink::OnZRConnectionStateChanged(ConnectionState connectionState)
{
    if (!owner) return;
    const char* stateStr;
    switch (connectionState)
    {
        case ConnectionStateEstablished:   stateStr = "ZR connection established"; break;
        case ConnectionStateConnected:     stateStr = "ZR connection connected and verified"; break;
        case ConnectionStateDisconnected:  stateStr = "ZR connection disconnected"; break;
        default:                           stateStr = "ZR connection state unknown"; break;
    }
    owner->RaiseMeetingStateChangedEvent(stateStr, (int)connectionState);
}

void ZrcMeetingServiceSink::OnStartMeetingResult(int32_t result)
{
    if (owner) owner->RaiseInstantMeetingStartedEvent("", (int)result);
}

void ZrcMeetingServiceSink::OnStartPmiResult(int32_t result, const std::string& meetingNumber, MeetingType /*meetingType*/)
{
    if (owner) owner->RaiseStartPmiResultEvent(meetingNumber.c_str(), (int)result);
}

void ZrcMeetingServiceSink::OnUpdateMeetingStatus(MeetingStatus meetingStatus)
{
    if (!owner) return;
    const char* statusStr;
    switch (meetingStatus)
    {
        case MeetingStatusNotInMeeting:        statusStr = "NotInMeeting"; break;
        case MeetingStatusConnectingToMeeting: statusStr = "ConnectingToMeeting"; break;
        case MeetingStatusInMeeting:           statusStr = "InMeeting"; break;
        case MeetingStatusLoggedOut:           statusStr = "LoggedOut"; break;
        default:                               statusStr = "Unknown"; break;
    }
    owner->RaiseMeetingStatusEvent(statusStr, (int)meetingStatus);
}

void ZrcMeetingServiceSink::OnExitMeetingNotification(int32_t result, ExitMeetingReason reason)
{
    if (owner) owner->RaiseExitMeetingEvent((int)result, (int)reason);
}

void ZrcMeetingServiceSink::OnMeetingErrorNotification(const MeetingErrorInfo& errorInfo)
{
    if (owner) owner->RaiseErrorEvent(errorInfo.errorInfo.c_str(), errorInfo.errorCode);
}

void ZrcMeetingServiceSink::OnMeetingEndedNotification(const MeetingErrorInfo& errorInfo)
{
    // errorInfo.errorCode can be a large server code — send it as the full error code, reason=0
    if (owner) owner->RaiseExitMeetingEvent(errorInfo.errorCode & 0xFF, 0);
}

void ZrcMeetingServiceSink::OnReceiveMeetingInviteNotification(const MeetingInvitationInfo& invitation)
{
    if (owner) owner->RaiseMeetingInviteEvent(invitation.callerName.c_str());
}

void ZrcMeetingServiceSink::OnStartMeetingWithHostKeyResult(int32_t result)
{
    if (owner) owner->RaiseInstantMeetingStartedEvent("", (int)result);
}

void ZrcMeetingServiceSink::OnMeetingNeedsPasswordNotification(bool showPasswordDialog, bool wrongAndRetry,
                                                                const ConfDeviceLockStatus& /*lockStatus*/)
{
    if (owner && showPasswordDialog) owner->RaiseMeetingNeedsPasswordEvent(wrongAndRetry ? 1 : 0);
}

void ZrcMeetingServiceSink::OnJBHWaitingHostNotification(bool showWaitForHostDialog, WaitingHostReason reason)
{
    if (owner && showWaitForHostDialog)
        owner->RaiseMeetingStateChangedEvent("Waiting for host", (int)reason);
}

void ZrcMeetingAudioHelperSink::OnUpdateMyAudioStatus(const AudioStatus& audioStatus)
{
    if (owner) owner->RaiseAudioStatusEvent(audioStatus.isMuted ? 1 : 0);
}

void ZrcMeetingAudioHelperSink::OnMuteOnEntryNotification(bool isMuteOnEntry)
{
    if (owner) owner->RaiseMuteOnEntryEvent(isMuteOnEntry ? 1 : 0);
}

void ZrcParticipantHelperSink::OnInitMeetingParticipants(const std::vector<MeetingParticipant>& participants,
                                                          int32_t totalParticipantsCount,
                                                          bool needCleanUpUserList, ConfSessionType session)
{
    if (!owner) return;
    if (session == CurrentSession)
    {
        owner->participantCount = totalParticipantsCount;
        owner->RaiseParticipantCountEvent(totalParticipantsCount);
    }
    owner->RaiseParticipantListEvent(participants, totalParticipantsCount, needCleanUpUserList, session);
}

void ZrcParticipantHelperSink::OnUserJoin(const std::vector<MeetingParticipant>& participants, ConfSessionType session)
{
    if (!owner) return;
    if (session == CurrentSession)
    {
        owner->participantCount += (int)participants.size();
        owner->RaiseParticipantCountEvent(owner->participantCount);
    }
    owner->RaiseParticipantListEvent(participants, owner->participantCount, false, session);
}

void ZrcParticipantHelperSink::OnUserLeave(const std::vector<MeetingParticipant>& participants, ConfSessionType session)
{
    if (!owner) return;
    if (session == CurrentSession)
    {
        owner->participantCount -= (int)participants.size();
        if (owner->participantCount < 0) owner->participantCount = 0;
        owner->RaiseParticipantCountEvent(owner->participantCount);
    }
    owner->RaiseParticipantListEvent(participants, owner->participantCount, false, session);
}

void ZrcParticipantHelperSink::OnUserUpdate(const std::vector<MeetingParticipant>& participants, ConfSessionType session)
{
    if (owner) owner->RaiseParticipantListEvent(participants, owner->participantCount, false, session);
}

void ZrcParticipantHelperSink::OnHostChangedNotification(int32_t /*hostUserID*/, bool amIHost, ConfSessionType session)
{
    if (owner && session == CurrentSession) owner->RaiseHostChangedEvent(amIHost ? 1 : 0);
}

void ZrcRecordingHelperSink::OnUpdateMeetingRecordingInfo(const MeetingRecordingInfo& recordingInfo)
{
    if (owner) owner->RaiseRecordingStatusEvent(recordingInfo.isMeetingBeingRecorded ? 1 : 0);
}

void ZrcRecordingHelperSink::OnSetMeetingRecordingResult(int32_t result, const std::string& /*recordingNotificationEmail*/,
                                                          RecordingRequestType /*type*/)
{
    if (owner) owner->RaiseRecordingStatusEvent(result == 0 ? 1 : 0);
}

void ZrcControlSystemHelperSink::OnEnableZRCSNotification(bool enable)
{
    if (owner) owner->RaiseControlSystemEnabledEvent(enable ? 1 : 0);
}

// ─── Helper macros ────────────────────────────────────────────────────────────
#define GET_MEETING_SERVICE(inst, var)                                      \
    IMeetingService* var = (inst)->pMeetingService                          \
        ? (inst)->pMeetingService                                           \
        : ((inst)->pZoomRoomsService                                        \
            ? (inst)->pZoomRoomsService->GetMeetingService() : nullptr);    \
    if (!(var)) {                                                            \
        (inst)->RaiseErrorEvent("Meeting Service not available", -1);       \
        return -1;                                                          \
    }

#define GET_PREMEETING_SERVICE(inst, var)                                   \
    IPreMeetingService* var = (inst)->pPreMeetingService                    \
        ? (inst)->pPreMeetingService                                        \
        : ((inst)->pZoomRoomsService                                        \
            ? (inst)->pZoomRoomsService->GetPreMeetingService() : nullptr); \
    if (!(var)) {                                                            \
        (inst)->RaiseErrorEvent("PreMeeting Service not available", -1);    \
        return -1;                                                          \
    }

// ─── SDK Lifecycle ────────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API ZrcSdkHandle ZRCSDKWRAPPER_CALL ZrcSdk_Create()
{
    try { return (ZrcSdkHandle)(new ZrcSdkInstance()); }
    catch (...) { return nullptr; }
}

ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_Destroy(ZrcSdkHandle handle)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (inst->bInitialized) ZrcSdk_Uninitialize(handle);
    delete inst->pControlSystemHelperSink;
    delete inst->pRecordingHelperSink;
    delete inst->pParticipantHelperSink;
    delete inst->pAudioHelperSink;
    delete inst->pMeetingServiceSink;
    delete inst->pPreMeetingServiceSink;
    delete inst->pZoomRoomsServiceSink;
    delete inst->pSdkSink;
    delete inst;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_Initialize(ZrcSdkHandle handle, const char* configPath)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;

    try
    {
        std::string contentDir = (configPath && configPath[0] != '\0')
            ? std::string(configPath) : std::string("/tmp/zrcsdk/");
        mkdir(contentDir.c_str(), 0755);

        inst->pSdkSink = new ZrcSDKSink(contentDir);
        inst->pNativeSDK = IZRCSDK::CreateInstance(inst->pSdkSink);
        if (!inst->pNativeSDK)
        {
            inst->RaiseErrorEvent("IZRCSDK::CreateInstance returned null", -1);
            return -1;
        }

        inst->pZoomRoomsService = inst->pNativeSDK->CreateZoomRoomsService();
        if (!inst->pZoomRoomsService)
        {
            inst->RaiseErrorEvent("CreateZoomRoomsService returned null", -1);
            return -1;
        }

        inst->pZoomRoomsServiceSink = new ZrcZoomRoomsServiceSink(inst);
        inst->pZoomRoomsService->RegisterSink(inst->pZoomRoomsServiceSink);

        inst->pPreMeetingService = inst->pZoomRoomsService->GetPreMeetingService();
        if (inst->pPreMeetingService)
        {
            inst->pPreMeetingServiceSink = new ZrcPreMeetingServiceSink(inst);
            inst->pPreMeetingService->RegisterSink(inst->pPreMeetingServiceSink);

            IControlSystemHelper* pCS = inst->pPreMeetingService->GetControlSystemHelper();
            if (pCS)
            {
                inst->pControlSystemHelperSink = new ZrcControlSystemHelperSink(inst);
                pCS->RegisterSink(inst->pControlSystemHelperSink);
            }
        }

        inst->pMeetingService = inst->pZoomRoomsService->GetMeetingService();
        if (inst->pMeetingService)
        {
            inst->pMeetingServiceSink = new ZrcMeetingServiceSink(inst);
            inst->pMeetingService->RegisterSink(inst->pMeetingServiceSink);

            IMeetingAudioHelper* pAudio = inst->pMeetingService->GetMeetingAudioHelper();
            if (pAudio)
            {
                inst->pAudioHelperSink = new ZrcMeetingAudioHelperSink(inst);
                pAudio->RegisterSink(inst->pAudioHelperSink);
            }

            IParticipantHelper* pPart = inst->pMeetingService->GetParticipantHelper();
            if (pPart)
            {
                inst->pParticipantHelperSink = new ZrcParticipantHelperSink(inst);
                pPart->RegisterSink(inst->pParticipantHelperSink);
            }

            IRecordingHelper* pRec = inst->pMeetingService->GetRecordingHelper();
            if (pRec)
            {
                inst->pRecordingHelperSink = new ZrcRecordingHelperSink(inst);
                pRec->RegisterSink(inst->pRecordingHelperSink);
            }
        }

        inst->bInitialized = true;
        inst->RaiseInitializedEvent("SDK initialized successfully", 0);
        return 0;
    }
    catch (...)
    {
        inst->RaiseErrorEvent("Exception during initialization", -1);
        return -1;
    }
}

ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_Uninitialize(ZrcSdkHandle handle)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (inst->pNativeSDK && inst->bInitialized)
    {
        try
        {
            inst->bInitialized = false;
            inst->pNativeSDK->ForceFlushLog();  // flush credential cache and logs to disk
            IZRCSDK::DestroyInstance();          // cleanly shut down SDK singleton
            inst->pNativeSDK = nullptr;
        }
        catch (...) {}
    }
}

ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_HeartBeat(ZrcSdkHandle handle)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (inst->pNativeSDK) inst->pNativeSDK->HeartBeat();
}

// ─── SDK Information ──────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetSDKVersion(ZrcSdkHandle handle, char* buffer, int bufferSize)
{
    if (!handle || !buffer || bufferSize <= 0) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->pNativeSDK)
    {
        strncpy(buffer, "Unknown - SDK not initialized", bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
        return -1;
    }
    try
    {
        std::string version;
        inst->pNativeSDK->GetSDKVersion(version);
        strncpy(buffer, version.c_str(), bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
        return 0;
    }
    catch (...)
    {
        strncpy(buffer, "Unknown", bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
        return -1;
    }
}

// ─── Room Pairing ─────────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PairRoomWithActivationCode(ZrcSdkHandle handle, const char* activationCode)
{
    if (!handle || !activationCode) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->pNativeSDK || !inst->bInitialized || !inst->pZoomRoomsService)
    {
        inst->RaiseErrorEvent("SDK not initialized", -1);
        return -1;
    }
    try
    {
        ZRCSDKError err = inst->pZoomRoomsService->PairRoomWithActivationCode(std::string(activationCode));
        if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("Failed to pair room", (int)err); return (int)err; }
        return 0;
    }
    catch (...) { inst->RaiseErrorEvent("Exception during pairing", -1); return -1; }
}

// ─── Pre-Meeting / Connection ─────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CanRetryToPairLastRoom(ZrcSdkHandle handle)
{
    if (!handle) return 0;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->pZoomRoomsService) return 0;
    bool canRetry = false;
    inst->pZoomRoomsService->CanRetryToPairLastRoom(canRetry);
    return canRetry ? 1 : 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RetryToPairRoom(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->pNativeSDK || !inst->bInitialized || !inst->pZoomRoomsService)
    {
        inst->RaiseErrorEvent("SDK not initialized", -1);
        return -1;
    }
    try
    {
        ZRCSDKError err = inst->pZoomRoomsService->RetryToPairRoom();
        if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("Failed to retry pairing", (int)err); return (int)err; }
        return 0;
    }
    catch (...) { inst->RaiseErrorEvent("Exception during retry pairing", -1); return -1; }
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UnpairRoom(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->pNativeSDK || !inst->bInitialized || !inst->pZoomRoomsService)
    {
        inst->RaiseErrorEvent("SDK not initialized", -1);
        return -1;
    }
    try
    {
        ZRCSDKError err = inst->pZoomRoomsService->UnpairRoom();
        if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("Failed to unpair room", (int)err); return (int)err; }
        return 0;
    }
    catch (...) { inst->RaiseErrorEvent("Exception during unpair", -1); return -1; }
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetConnectionState(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->pPreMeetingService) return -1;
    ConnectionState state = ConnectionStateNone;
    inst->pPreMeetingService->GetConnectionState(state);
    return (int)state;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_WakeZoomRoomUp(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_PREMEETING_SERVICE(inst, pPM);
    return (int)pPM->WakeZoomRoomUp();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LogoutZoomRoomDevice(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_PREMEETING_SERVICE(inst, pPM);
    return (int)pPM->LogoutZoomRoomDevice();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RestartZoomRoomOS(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_PREMEETING_SERVICE(inst, pPM);
    return (int)pPM->RestartZoomRoomOS();
}

// ─── Meeting Control ──────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartMeeting(ZrcSdkHandle handle, const char* meetingNumber)
{
    return ZrcSdk_JoinMeeting(handle, meetingNumber);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartInstantMeeting(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ZRCSDKError err = pMS->StartInstantMeeting();
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("StartInstantMeeting failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartMeetingWithHostKey(ZrcSdkHandle handle, const char* hostKey)
{
    if (!handle || !hostKey) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ZRCSDKError err = pMS->StartMeetingWithHostKey(std::string(hostKey));
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("StartMeetingWithHostKey failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_JoinMeeting(ZrcSdkHandle handle, const char* meetingNumber)
{
    if (!handle || !meetingNumber) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ZRCSDKError err = pMS->JoinMeetingWithMeetingNumber(std::string(meetingNumber));
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("JoinMeeting failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_JoinMeetingWithURL(ZrcSdkHandle handle, const char* url)
{
    if (!handle || !url) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ZRCSDKError err = pMS->JoinMeetingWithURL(std::string(url));
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("JoinMeetingWithURL failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LeaveMeeting(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ZRCSDKError err = pMS->ExitMeeting(ExitMeetingCmdLeave);
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("LeaveMeeting failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_EndMeeting(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ZRCSDKError err = pMS->ExitMeeting(ExitMeetingCmdEnd);
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("EndMeeting failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendMeetingPassword(ZrcSdkHandle handle, const char* password)
{
    if (!handle || !password) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ZRCSDKError err = pMS->SendMeetingPassword(std::string(password));
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("SendMeetingPassword failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CancelEnteringMeetingPassword(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    return (int)pMS->CancelEnteringMeetingPassword();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CancelWaitingForHost(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    return (int)pMS->CancelWaitingForHost();
}

// ─── Audio / Video ────────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetAudioMute(ZrcSdkHandle handle, int mute)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingAudioHelper* pAudio = pMS->GetMeetingAudioHelper();
    if (!pAudio) { inst->RaiseErrorEvent("Audio Helper not available", -1); return -1; }
    ZRCSDKError err = pAudio->UpdateMyAudioStatus(mute);
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("SetAudioMute failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoState(ZrcSdkHandle handle, int start)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingVideoHelper* pVideo = pMS->GetMeetingVideoHelper();
    if (!pVideo) { inst->RaiseErrorEvent("Video Helper not available", -1); return -1; }
    ZRCSDKError err = pVideo->UpdateMyVideo(!start);
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("SetVideoState failed", (int)err); return (int)err; }
    return 0;
}

// ─── Cloud Recording ──────────────────────────────────────────────────────────

static IRecordingHelper* GetRecordingHelper(ZrcSdkInstance* inst)
{
    return inst->pMeetingService ? inst->pMeetingService->GetRecordingHelper() : nullptr;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartRecording(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    IRecordingHelper* pRec = GetRecordingHelper(inst);
    if (!pRec) { inst->RaiseErrorEvent("Recording Helper not available", -1); return -1; }
    ZRCSDKError err = pRec->StartMeetingCloudRecording();
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("StartRecording failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StopRecording(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    IRecordingHelper* pRec = GetRecordingHelper(inst);
    if (!pRec) { inst->RaiseErrorEvent("Recording Helper not available", -1); return -1; }
    ZRCSDKError err = pRec->StopMeetingCloudRecording();
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("StopRecording failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PauseRecording(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    IRecordingHelper* pRec = GetRecordingHelper(inst);
    if (!pRec) { inst->RaiseErrorEvent("Recording Helper not available", -1); return -1; }
    ZRCSDKError err = pRec->PauseMeetingCloudRecording();
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("PauseRecording failed", (int)err); return (int)err; }
    return 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ResumeRecording(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    IRecordingHelper* pRec = GetRecordingHelper(inst);
    if (!pRec) { inst->RaiseErrorEvent("Recording Helper not available", -1); return -1; }
    ZRCSDKError err = pRec->ResumeMeetingCloudRecording();
    if (err != ZRCSDKERR_SUCCESS) { inst->RaiseErrorEvent("ResumeRecording failed", (int)err); return (int)err; }
    return 0;
}

// ─── Participants ─────────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetParticipantCount(ZrcSdkHandle handle)
{
    if (!handle) return 0;
    return ((ZrcSdkInstance*)handle)->participantCount;
}

// ─── Control System (ZRCS) ────────────────────────────────────────────────────

static IControlSystemHelper* GetCSHelper(ZrcSdkInstance* inst)
{
    return inst->pPreMeetingService ? inst->pPreMeetingService->GetControlSystemHelper() : nullptr;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_IsZRCSEnabled(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    IControlSystemHelper* pCS = GetCSHelper((ZrcSdkInstance*)handle);
    if (!pCS) return -1;
    bool enabled = false;
    return pCS->IsZRCSEnabled(enabled) == ZRCSDKERR_SUCCESS ? (enabled ? 1 : 0) : -1;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_OpenRoomControls(ZrcSdkHandle handle, int open)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    IControlSystemHelper* pCS = GetCSHelper(inst);
    if (!pCS) { inst->RaiseErrorEvent("Control System Helper not available", -1); return -1; }
    return (int)pCS->OpenRoomControls(open != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ControlZRCSDevice(ZrcSdkHandle handle,
    const char* deviceID, const char* methodID, const char* paramID, const char* value)
{
    if (!handle || !deviceID || !methodID || !paramID || !value) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    IControlSystemHelper* pCS = GetCSHelper(inst);
    if (!pCS) { inst->RaiseErrorEvent("Control System Helper not available", -1); return -1; }
    return (int)pCS->ControlZRCSDevice(std::string(deviceID), std::string(methodID),
                                        std::string(paramID), std::string(value));
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ExecuteZRCSScene(ZrcSdkHandle handle, const char* sceneID)
{
    if (!handle || !sceneID) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    IControlSystemHelper* pCS = GetCSHelper(inst);
    if (!pCS) { inst->RaiseErrorEvent("Control System Helper not available", -1); return -1; }
    return (int)pCS->ExecuteZRCSScene(std::string(sceneID));
}

// ─── Event Callback Setters ───────────────────────────────────────────────────

#define SET_CB(field, cb, ud) \
    do { if (!handle) return; ZrcSdkInstance* inst = (ZrcSdkInstance*)handle; \
         inst->field##Callback = cb; inst->field##UserData = ud; } while(0)

ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetInitializedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(initialized, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetConnectionStateChangedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(meetingStateChanged, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetErrorCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(error, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetInstantMeetingStartedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(instantMeetingStarted, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetPairRoomResultCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(pairRoomResult, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(meetingStatus, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetStartPmiResultCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(startPmiResult, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetExitMeetingCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(exitMeeting, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingNeedsPasswordCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(meetingNeedsPassword, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingInviteCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(meetingInvite, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAudioStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(audioStatus, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMuteOnEntryCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(muteOnEntry, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetParticipantCountCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(participantCount, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetHostChangedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(hostChanged, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetRecordingStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(recordingStatus, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetControlSystemEnabledCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(controlSystemEnabled, callback, userData); }

ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetParticipantListCallback(ZrcSdkHandle handle, ZrcParticipantListCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->participantListCallback = callback;
    inst->participantListUserData = userData;
}

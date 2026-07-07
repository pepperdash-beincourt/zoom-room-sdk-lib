#include "ZrcSdkWrapper_C.h"
#include <cstdint>
#include <string>
#include <cstring>
#include <map>
#include <mutex>
#include <sys/stat.h>
#include "IZRCSDK.h"
#include "IZoomRoomsService.h"
#include "IMeetingService.h"
#include "IPreMeetingService.h"
#include "ISettingService.h"
#include "IPhoneCallService.h"
#include "IProAVService.h"
#include "ServiceComponents/IMeetingAudioHelper.h"
#include "ServiceComponents/IMeetingVideoHelper.h"
#include "ServiceComponents/IParticipantHelper.h"
#include "ServiceComponents/IRecordingHelper.h"
#include "ServiceComponents/IControlSystemHelper.h"
#include "ServiceComponents/IMeetingShareHelper.h"
#include "ServiceComponents/IMeetingViewLayoutHelper.h"
#include "ServiceComponents/IMeetingControlHelper.h"
#include "ServiceComponents/IMeetingReactionEmojiHelper.h"
#include "ServiceComponents/IWaitingRoomHelper.h"
#include "ServiceComponents/IBreakoutRoomHelper.h"
#include "ServiceComponents/IMeetingChatHelper.h"
#include "ServiceComponents/IClosedCaptionHelper.h"
#include "ServiceComponents/ICameraControlHelper.h"
#include "ServiceComponents/IThirdPartyMeetingHelper.h"
#include "ServiceComponents/IContactHelper.h"
#include "ServiceComponents/IMeetingListHelper.h"

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
    void OnTreatedMeetingInviteNotification(const MeetingInvitationInfo& invitation, bool accepted) override;
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
    void OnAskUnmuteAudioByHostNotification(bool show, AskUnmuteAudioByHostType type) override;
    void OnAllowAttendeesUnmuteThemselvesNotification(bool canAttendeesUnmuteThemselves) override;
    void OnEnablePlayJoinOrLeaveChimeNotification(bool enable) override {}
    void OnUpdateAudioTroubleShootingStatus(const AudioTroubleShootingStatus& status) override {}
    void OnFEACApproveNotification(int32_t farEndUserID, const std::string& farEndUserName) override;
    void OnFEACDeclineNotification(int32_t farEndUserID, const std::string& farEndUserName) override;
    void OnFEACTakeOverNotification(int32_t farEndUserID, const std::string& farEndUserName, int32_t controllingUserID, const std::string& controllingUserName) override {}
    void OnFEACMicListChangedNotification(int32_t farEndUserID, const std::vector<FarEndAudioDeviceInfo>& micList) override {}
    void OnFEACSpeakerListChangedNotification(int32_t farEndUserID, const std::vector<FarEndAudioDeviceInfo>& speakerList) override {}
    void OnFEACMuteStateChangedNotification(int32_t farEndUserID, bool muteState) override {}
    void OnFEACUnmuteDisabledByHostNotification(int32_t farEndUserID) override {}
    void OnFEACRequestNotification(int32_t requesterUserID, const std::string& requesterUserName) override;
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
    void OnUpdateZRCSDeviceList(ControlSystemUpdateDeviceType type, const ControlSystemDeviceList& list) override;
    void OnUpdateZRCSSceneList(const std::vector<ControlSystemSceneInfo>& scenes) override;
};

// ─── IContactHelperSink — directory / phonebook contact events ───────────────
class ZrcContactHelperSink : public IContactHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcContactHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnImUpdateContactNotification(const std::vector<Contact>& contacts) override;
    void OnDynamicContactBasicInfoNotification(const std::vector<ContactTypeInfo>& typeInfos, uint32_t numberOfContacts, uint32_t numberOfZoomRooms) override {}
    void OnDynamicContactListNotification(const DynamicContactListInfo& info) override;
    void OnUpdateFavoritesList(const FavoritesList& list) override {}
    void OnStartReceivingLegacyRoomList(bool isSelfRooms) override {}
    void OnAddLegacyRooms(const std::vector<LegacyRoomSystem>& roomList, bool isSelfRooms) override {}
    void OnFinishReceivingLegacyRoomList(bool isSelfRooms) override {}
};

// ─── IMeetingListHelperSink — bookings / schedule events ─────────────────────
class ZrcMeetingListHelperSink : public IMeetingListHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcMeetingListHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnUpdateMeetingList(ListMeetingResult result, const std::vector<MeetingItem>& meetingList) override;
    void OnUpdatedScheduleCalendarEventNotification(ScheduleCalendarEventResult scheduleResult) override {}
    void OnUpdatedDeleteCalendarEventNotification(DeleteCalendarEventResult deleteResult) override {}
    void OnShowUpcomingMeetingAlertResult(int32_t result, const MeetingItem& meetingItem) override {}
    void OnCloseUpcomingMeetingAlertResult(int32_t result) override {}
    void OnMeetingWillReleaseAutomatically(const MeetingItem& meetingItem) override {}
};

// ─── IMeetingVideoHelperSink ──────────────────────────────────────────────────
class ZrcMeetingVideoHelperSink : public IMeetingVideoHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcMeetingVideoHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnUpdateMyVideoNotification(const VideoStatus& videoStatus) override {}
    void OnMuteUserVideoNotification(int32_t userID, const VideoStatus& videoStatus) override {}
    void OnAskStartVideoByHostNotification(int32_t userID) override {}
    void OnUpdateScreenStatusForPinNotification(const std::vector<ScreenStatusForPin>& screens, PinShareWarningType warning) override {}
    void OnSpotlightStatusNotification(const SpotlightStatus& status) override {}
    void OnUpdateAllowAttendeesStartVideo(bool allow) override;
    void OnMyVideoSettingsNotification(const MyVideoSettings& settings) override {}
    void OnMyMeetingVideoSettingsNotification(const MyMeetingVideoSettings& settings) override {}
};

// ─── IMeetingShareHelperSink ──────────────────────────────────────────────────
class ZrcMeetingShareHelperSink : public IMeetingShareHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcMeetingShareHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnStartLocalPresentNotification(const LocalPresentationInfo& info) override {}
    void OnStartLocalPresentResult(bool success, SharingInstructionDisplayState state) override {}
    void OnSwitchToNormalMeetingResult(int result) override {}
    void OnShowSharingInstructionResult(int result, bool show, SharingInstructionDisplayState state) override {}
    void OnShareSettingNotification(const ShareSetting& setting) override {}
    void OnSharingStatusNotification(const SharingStatus& status) override;
    void OnUpdateAirPlayBlackMagicStatus(const AirplayBlackMagicStatus& status) override;
    void OnUpdateCameraSharingStatus(const CameraSharingStatus& status) override {}
    void OnSharingSourceNotification(const std::vector<ShareSource>& sources, const std::vector<ShareSource>& sources2) override {}
    void OnHDMI60FPSShareInfoNotification(bool a, bool b, HDMI60FPSShareDisableReason r) override {}
    void OnHDMIShareResolutionAndFrameRateNotification(const std::vector<HDMIShareResolutionAndFrameRateOption>& opts, uint32_t sel) override {}
    void OnLocalHDMIShareAudioPlaybackNotification(bool enable) override {}
    void OnUpdateClassicWhiteboardShareStatusNotification(const ClassicWhiteboardShareStatus& s) override {}
    void OnZRWSharingStatusNotification(const ZRWSharingStatus& s) override {}
    void OnUpdateLocalViewStatus(bool show) override {}
    void OnIncomingMeetingShareNotification(const IncomingMeetingShareNot& n) override {}
    void OnSlideControlNotification(const std::vector<SlideControlInfo>& s) override {}
    void OnDocsShareSettingsNotification(const DocsShareSettingsInfo& s) override {}
};

// ─── IMeetingViewLayoutHelperSink ─────────────────────────────────────────────
class ZrcMeetingViewLayoutHelperSink : public IMeetingViewLayoutHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcMeetingViewLayoutHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnUpdateWallviewStyleNotification(const WallViewStyleStatus& status) override {}
    void OnUpdateVideoThumbInfo(const VideoThumbInfo& info) override {}
    void OnUpdateVideoPageStatusNotification(const VideoPageStatus& status) override;
    void OnUpdateIsNonVideoParticipantsShowedNotification(bool show) override {}
    void OnUpdateShowUpTo49PerPageInGallery(bool show) override {}
    void OnAutoSwitchSpeakerNotification(bool a, bool b) override {}
    void OnVideoOrderNotification(const VideoOrderInfo& info) override {}
    void OnDynamicLayoutOptionNotification(DynamicLayoutType type) override {}
    void OnConfidenceMonitorNotification(const ConfidenceMonitorInfo& info) override {}
    void OnChangeAttendeeViewNotification(AttendeeViewLayoutType type) override {}
    void OnAttendeeViewLayoutEnableShareContentOnlyNotification(bool a, bool b) override {}
    void OnUpdateGalleryGridSelectionNotification(bool a, uint32_t b, uint32_t c) override {}
    void OnUpdateScreenLayoutStatus(const ScreenLayoutStatus& status) override;
    void OnThumbnailsPositionNotification(ThumbnailsPositionType type) override {}
};

// ─── IMeetingControlHelperSink ────────────────────────────────────────────────
class ZrcMeetingControlHelperSink : public IMeetingControlHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcMeetingControlHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnUpdateIsDisplayTopBannerNotification(bool show) override {}
    void OnUpdateMeetingLockStatus(bool locked) override;
    void OnUpdateFocusModeOptionsNotification(bool a, FocusModeStatus b) override {}
    void OnUpdateLiveStreamStatus(const LiveStreamStatus& s) override {}
    void OnHiFiMusicModeNotification(bool a, bool b) override {}
    void OnHasAppSignalingChanged(bool a) override {}
    void OnUpdateSignalingApps(const SignalingAppList& s) override {}
    void OnUpdateAccessedUsers(const SignalingAppAccessedUserList& s) override {}
    void OnUpdateAppPermissionLink(const SignalingAppPermissionLink& s) override {}
    void OnArchivingStatusNotification(bool a) override {}
    void OnShowArchivingStatusFailNotification(bool a) override {}
    void OnZoomPhoneACRStatusNotification(bool a) override {}
    void OnSmartSummaryOn(bool a, bool b) override {}
    void OnSetMeetingSummaryNotificationEmailNotification(int32_t r) override {}
    void OnUpdateMeetingQueryBaseInfo(const MeetingQueryInfo& i) override {}
    void OnChangeMeetingQueryPrivilegeSettingID(int32_t id) override {}
    void OnReceiveAICompanionRequest(const AICompanionRequestInfo& i) override {}
    void OnAICompanionStatusNeedConfirm(const AICompanionStatusInfo& i) override {}
    void OnShowSidePanel(bool show, PanelType type) override {}
};

// ─── IMeetingReactionEmojiHelperSink ─────────────────────────────────────────
class ZrcReactionEmojiHelperSink : public IMeetingReactionEmojiHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcReactionEmojiHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnUpdateReactionStatus(const ReactionStatus& status) override;
    void OnUpdateReactionStatisticsInfoNotification(const ReactionStatisticsInfo& info) override {}
};

// ─── IWaitingRoomHelperSink ───────────────────────────────────────────────────
class ZrcWaitingRoomHelperSink : public IWaitingRoomHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcWaitingRoomHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnInSilentModeNotification(const InSilentModeInfo& info) override;
    void OnEnableWaitingRoomOnEntryNotification(bool isEnable) override {}
    void OnUpdateAdmitGuestEnableNotification(bool isEnabled) override {}
};

// ─── IBreakoutRoomHelperSink ──────────────────────────────────────────────────
class ZrcBreakoutRoomHelperSink : public IBreakoutRoomHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcBreakoutRoomHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnBOStatusChanged(BO_STATUS boStatus) override;
    void OnBOSwitchRequestReceived(const std::string& fromUserName, const BreakoutRoomInfo& newBORoomInfo) override {}
    void OnHostInviteReturnToMainSession(const std::string& fromUserName) override {}
    void OnBOStopCountDown(uint64_t remainingSeconds) override {}
    void OnStartJoinBONotification() override {}
};

// ─── IMeetingChatHelperSink ───────────────────────────────────────────────────
class ZrcMeetingChatHelperSink : public IMeetingChatHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcMeetingChatHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnChatDisplaySettingsNotification(bool a, bool b) override {}
    void OnMeetingChatPrivilegeNotification(const MeetingChatPrivilege& priv) override;
    void OnChatMessageResponse(int32_t result, int32_t action) override {}
    void OnMeetingChatMessagesNotification(const std::vector<MeetingChatMessage>& messages) override;
    void OnUpdateCMCInfo(bool a, bool b) override {}
    void OnUpdateCMCTip(bool need, const std::string& title, const std::string& msg) override {}
    void OnIsReadyForLoadMessageChanged() override {}
    void OnThreadReceived(const NewMeetingChatMessage& thread) override;
    void OnCommentReceived(const NewMeetingChatMessage& comment, const NewMeetingChatMessage& thread) override {}
    void OnThreadSent(const NewMeetingChatMessage& thread) override {}
    void OnCommentSent(const NewMeetingChatMessage& comment, const NewMeetingChatMessage& thread) override {}
    void OnThreadDeleted(const NewMeetingChatMessage& thread) override {}
    void OnCommentDeleted(const NewMeetingChatMessage& comment, const NewMeetingChatMessage& thread) override {}
    void OnMessagesUpdated(const std::vector<NewMeetingChatMessage>& messages) override {}
    void OnEmojiUpdated(const std::vector<NewMeetingChatMessage>& messages) override {}
    void OnMessageLoaded(const ChatMessageSyncFilter& filter, bool result,
                         const std::vector<NewMeetingChatMessage>& messages, bool a, bool b) override {}
    void OnEmojiDetail(const std::string& msgID, const std::string& code, bool a, bool b,
                       const std::vector<ChatEmojiComment>& cmts) override {}
    void OnSendMessageFailed() override {}
};

// ─── IClosedCaptionHelperSink ─────────────────────────────────────────────────
class ZrcClosedCaptionHelperSink : public IClosedCaptionHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcClosedCaptionHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnUpdateClosedCaptionNotification(const ClosedCaptionInfo& info) override {}
    void OnClosedCaptionResponse(int32_t result, ClosedCaptionFontSize fontSize) override {}
    void OnNewLTTLanguageNotification(const NewLTTCaptionInfo& info) override {}
    void OnNewLTTCaptionNotification(NewLTTCaptionNotificationType type) override {}
    void OnUpdateInterpretLanguageNotification(const InterpretLanguageInfoList& info) override {}
    void OnMessageAdd(const LTTCaptionMessage& msg) override;
    void OnMessageUpdate(const LTTCaptionMessage& msg) override {}
    void OnMessageNotSupported(const LTTCaptionMessage& msg) override {}
    void OnMessageLoad(const std::vector<LTTCaptionMessage>& msgs, bool hasMore) override {}
    void OnMessageInstanceOnlySpeakerTagUpdate(const LTTCaptionMessage& msg) override {}
};

// ─── ICameraControlHelperSink ─────────────────────────────────────────────────
class ZrcCameraControlHelperSink : public ICameraControlHelperSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcCameraControlHelperSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnUpdateSpeakerSwitchingSettingsNotification(const SpeakerSwitchingSettingsInfo& info) override {}
    void OnEnableSpeakerSwitchingResult(int32_t result, bool enable) override {}
    void OnUpdateCameraIntelligentZoomNotification(const CameraIntelligentZoomInfo& info) override {}
    void OnUpdateMultiCameraEnabled(bool enable) override {}
    void OnUpdateMaxMultiCameraCount(int32_t count) override {}
    void OnUpdateMainCameraControlInfo(const CameraControlInfo& info) override {}
    void OnUpdateMultiCameraControlInfos(const std::vector<CameraControlInfo>& infos) override {}
    void OnCameraPanTiltResult(const CameraPanTiltResult& result) override {}
    void OnUpdateMeetingUserCameraControlStatus(int32_t userID, const CameraControlStatus& status) override {}
    void OnFarEndCameraControlNotification(const FarEndCameraControlInfo& info) override;
    void OnFarEndCameraCameraControlResult(int32_t userID, CameraControlType type, bool success) override {}
    void OnTreatedCameraControlRequestNotification(bool accept, int32_t userID) override {}
    void OnMirrorVideoResult(const MirrorSelfVideoRes& res) override {}
    void OnUpdateCameraPresetInfo(const CameraPresetInfo& info) override;
};

// ─── IPhoneCallServiceSink ────────────────────────────────────────────────────
class ZrcPhoneCallServiceSink : public IPhoneCallServiceSink
{
public:
    ZrcSdkInstance* owner;
    explicit ZrcPhoneCallServiceSink(ZrcSdkInstance* inst) : owner(inst) {}
    void OnAnswerSIPCallResult(bool success, const SIPCallInfo& call, bool isHold) override {}
    void OnAcceptSIPCallToMeetingResult(bool success, const SIPCallInfo& call) override {}
    void OnUpdateSIPCallPeerResult(bool success, const SIPCallInfo& call) override {}
    void OnUpdateSIPCallAudioResult(bool success) override {}
    void OnTransferSIPCallResult(bool success, const SIPCallInfo& call, const SIPCallTransferInfo& info) override {}
    void OnUpgradeSIPCallToMeetingResult(bool success, const SIPCallInfo& call) override {}
    void OnReceiveIncomingSIPCallNotification(const SIPCallInfo& call) override;
    void OnTreatSIPCallNotification(bool success, const SIPCallInfo& call) override {}
    void OnInviteSIPCallToJoinMeetingNotification(const SIPCallInfo& call) override {}
    void OnTerminateSIPCallNotification(SIPCallTerminateReason reason, const SIPCallInfo& call) override;
    void OnUpdateSIPCallAudioStatusNotification(bool muted) override {}
    void OnUpdateSIPCallStatusNotification(const SIPCallInfo& call) override;
    void OnTransferSIPCallNotification(bool success, const SIPCallInfo& call) override {}
    void OnUpgradeSIPCallToMeetingNotification(bool success, const SIPCallInfo& call) override {}
    void OnUpdateSIPServiceStatusNotification(const SIPService& service) override;
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
    ISettingService*            pSettingService;
    IMeetingService*            pMeetingService;
    ZrcMeetingServiceSink*      pMeetingServiceSink;
    ZrcMeetingAudioHelperSink*      pAudioHelperSink;
    ZrcMeetingVideoHelperSink*      pVideoHelperSink;
    ZrcMeetingShareHelperSink*      pShareHelperSink;
    ZrcMeetingViewLayoutHelperSink* pViewLayoutHelperSink;
    ZrcMeetingControlHelperSink*    pMeetingControlHelperSink;
    ZrcReactionEmojiHelperSink*     pReactionEmojiHelperSink;
    ZrcWaitingRoomHelperSink*       pWaitingRoomHelperSink;
    ZrcBreakoutRoomHelperSink*      pBreakoutRoomHelperSink;
    ZrcMeetingChatHelperSink*       pChatHelperSink;
    ZrcClosedCaptionHelperSink*     pClosedCaptionHelperSink;
    ZrcCameraControlHelperSink*     pCameraControlHelperSink;
    ZrcParticipantHelperSink*   pParticipantHelperSink;
    ZrcRecordingHelperSink*     pRecordingHelperSink;
    ZrcControlSystemHelperSink* pControlSystemHelperSink;
    IPhoneCallService*          pPhoneCallService;
    ZrcPhoneCallServiceSink*    pPhoneCallServiceSink;
    IContactHelper*             pContactHelper;
    ZrcContactHelperSink*       pContactHelperSink;
    IMeetingListHelper*         pMeetingListHelper;
    ZrcMeetingListHelperSink*   pMeetingListHelperSink;
    // Active SIP calls keyed by callID. The SDK's hangup/hold/DTMF take a SIPCallInfo (not a callID),
    // so we cache the info from the status notifications and look it up by callID for command calls.
    std::map<std::string, SIPCallInfo> sipCalls;
    std::mutex                  sipCallsMutex;
    // Last received meeting invite. AnswerMeetingInvite takes the full MeetingInvitationInfo (not a
    // meeting number), so we cache it from OnReceiveMeetingInviteNotification and answer with it.
    MeetingInvitationInfo       lastMeetingInvite;
    bool                        hasMeetingInvite;
    std::mutex                  meetingInviteMutex;

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
    ZrcMeetingInviteCallback meetingInviteCallback; void* meetingInviteUserData;
    SdkEventCallback audioStatusCallback;           void* audioStatusUserData;
    SdkEventCallback muteOnEntryCallback;           void* muteOnEntryUserData;
    SdkEventCallback participantCountCallback;      void* participantCountUserData;
    SdkEventCallback hostChangedCallback;           void* hostChangedUserData;
    SdkEventCallback recordingStatusCallback;       void* recordingStatusUserData;
    ZrcMeetingRecordingInfoCallback meetingRecordingInfoCallback; void* meetingRecordingInfoUserData;
    ZrcCameraPresetInfoCallback cameraPresetInfoCallback; void* cameraPresetInfoUserData;
    SdkEventCallback controlSystemEnabledCallback;  void* controlSystemEnabledUserData;
    ZrcParticipantListCallback participantListCallback; void* participantListUserData;
    // Contacts / directory
    ZrcContactListCallback contactListCallback;     void* contactListUserData;
    // Bookings / schedule
    ZrcMeetingListCallback meetingListCallback;     void* meetingListUserData;
    // Audio extended
    SdkEventCallback allowAttendeesUnmuteCallback;  void* allowAttendeesUnmuteUserData;
    SdkEventCallback askUnmuteByHostCallback;        void* askUnmuteByHostUserData;
    SdkEventCallback feacRequestCallback;            void* feacRequestUserData;
    SdkEventCallback feacApprovedCallback;           void* feacApprovedUserData;
    SdkEventCallback feacDeclinedCallback;           void* feacDeclinedUserData;
    // Video extended
    SdkEventCallback allowAttendeesVideoCallback;   void* allowAttendeesVideoUserData;
    // Layout
    ZrcVideoPageStatusCallback videoPageStatusCallback; void* videoPageStatusUserData;
    ZrcScreenLayoutStatusCallback screenLayoutStatusCallback; void* screenLayoutStatusUserData;
    // Share
    ZrcSharingStatusCallback sharingStatusCallback; void* sharingStatusUserData;
    ZrcAirPlayStatusCallback airPlayStatusCallback; void* airPlayStatusUserData;
    // Breakout room
    SdkEventCallback boStatusChangedCallback;       void* boStatusChangedUserData;
    ZrcBORoomListCallback boRoomListCallback;        void* boRoomListUserData;
    // Waiting room
    SdkEventCallback inSilentModeCallback;          void* inSilentModeUserData;
    // Reactions
    SdkEventCallback reactionStatusCallback;        void* reactionStatusUserData;
    // Chat
    SdkEventCallback chatPrivilegeCallback;         void* chatPrivilegeUserData;
    ZrcChatMessageCallback chatMessageCallback;     void* chatMessageUserData;
    // Caption
    ZrcCaptionCallback captionCallback;             void* captionUserData;
    // Meeting control extended
    SdkEventCallback meetingLockStatusCallback;     void* meetingLockStatusUserData;
    SdkEventCallback qaEnabledCallback;             void* qaEnabledUserData;
    // Recording extended
    SdkEventCallback recordingRequestCallback;      void* recordingRequestUserData;
    // Camera control
    SdkEventCallback farEndCameraControlCallback;   void* farEndCameraControlUserData;
    // Phone / SIP
    ZrcSIPCallCallback sipCallStatusCallback;       void* sipCallStatusUserData;
    SdkEventCallback sipServiceStatusCallback;      void* sipServiceStatusUserData;
    // ZRCS extended
    ZrcZRCSDeviceListCallback zrcsDeviceListCallback; void* zrcsDeviceListUserData;
    ZrcZRCSSceneListCallback  zrcsSceneListCallback;  void* zrcsSceneListUserData;

    ZrcSdkInstance()
        : pNativeSDK(nullptr), bInitialized(false)
        , pSdkSink(nullptr), pZoomRoomsService(nullptr), pZoomRoomsServiceSink(nullptr)
        , pPreMeetingService(nullptr), pPreMeetingServiceSink(nullptr)
        , pSettingService(nullptr)
        , pMeetingService(nullptr), pMeetingServiceSink(nullptr)
        , pAudioHelperSink(nullptr), pVideoHelperSink(nullptr), pShareHelperSink(nullptr)
        , pViewLayoutHelperSink(nullptr), pMeetingControlHelperSink(nullptr)
        , pReactionEmojiHelperSink(nullptr), pWaitingRoomHelperSink(nullptr)
        , pBreakoutRoomHelperSink(nullptr), pChatHelperSink(nullptr)
        , pClosedCaptionHelperSink(nullptr), pCameraControlHelperSink(nullptr)
        , pParticipantHelperSink(nullptr)
        , pRecordingHelperSink(nullptr), pControlSystemHelperSink(nullptr)
        , pPhoneCallService(nullptr), pPhoneCallServiceSink(nullptr)
        , pContactHelper(nullptr), pContactHelperSink(nullptr)
        , pMeetingListHelper(nullptr), pMeetingListHelperSink(nullptr)
        , hasMeetingInvite(false)
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
        , meetingRecordingInfoCallback(nullptr), meetingRecordingInfoUserData(nullptr)
        , cameraPresetInfoCallback(nullptr), cameraPresetInfoUserData(nullptr)
        , controlSystemEnabledCallback(nullptr), controlSystemEnabledUserData(nullptr)
        , participantListCallback(nullptr), participantListUserData(nullptr)
        , contactListCallback(nullptr), contactListUserData(nullptr)
        , meetingListCallback(nullptr), meetingListUserData(nullptr)
        , allowAttendeesUnmuteCallback(nullptr), allowAttendeesUnmuteUserData(nullptr)
        , askUnmuteByHostCallback(nullptr), askUnmuteByHostUserData(nullptr)
        , feacRequestCallback(nullptr), feacRequestUserData(nullptr)
        , feacApprovedCallback(nullptr), feacApprovedUserData(nullptr)
        , feacDeclinedCallback(nullptr), feacDeclinedUserData(nullptr)
        , allowAttendeesVideoCallback(nullptr), allowAttendeesVideoUserData(nullptr)
        , videoPageStatusCallback(nullptr), videoPageStatusUserData(nullptr)
        , sharingStatusCallback(nullptr), sharingStatusUserData(nullptr)
        , airPlayStatusCallback(nullptr), airPlayStatusUserData(nullptr)
        , boStatusChangedCallback(nullptr), boStatusChangedUserData(nullptr)
        , boRoomListCallback(nullptr), boRoomListUserData(nullptr)
        , inSilentModeCallback(nullptr), inSilentModeUserData(nullptr)
        , reactionStatusCallback(nullptr), reactionStatusUserData(nullptr)
        , chatPrivilegeCallback(nullptr), chatPrivilegeUserData(nullptr)
        , chatMessageCallback(nullptr), chatMessageUserData(nullptr)
        , captionCallback(nullptr), captionUserData(nullptr)
        , meetingLockStatusCallback(nullptr), meetingLockStatusUserData(nullptr)
        , qaEnabledCallback(nullptr), qaEnabledUserData(nullptr)
        , recordingRequestCallback(nullptr), recordingRequestUserData(nullptr)
        , farEndCameraControlCallback(nullptr), farEndCameraControlUserData(nullptr)
        , sipCallStatusCallback(nullptr), sipCallStatusUserData(nullptr)
        , sipServiceStatusCallback(nullptr), sipServiceStatusUserData(nullptr)
        , zrcsDeviceListCallback(nullptr), zrcsDeviceListUserData(nullptr)
        , zrcsSceneListCallback(nullptr), zrcsSceneListUserData(nullptr)
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
    void RaiseMeetingInviteEvent(const MeetingInvitationInfo& invite);  // defined out-of-line (needs strncpy_safe)
    void RaiseAudioStatusEvent(int isMuted)                       { Raise(audioStatusCallback, audioStatusUserData, "", isMuted); }
    void RaiseMuteOnEntryEvent(int enabled)                       { Raise(muteOnEntryCallback, muteOnEntryUserData, "", enabled); }
    void RaiseParticipantCountEvent(int count)                    { Raise(participantCountCallback, participantCountUserData, "", count); }
    void RaiseHostChangedEvent(int amIHost)                       { Raise(hostChangedCallback, hostChangedUserData, "", amIHost); }
    void RaiseRecordingStatusEvent(int isRecording)               { Raise(recordingStatusCallback, recordingStatusUserData, "", isRecording); }
    void RaiseMeetingRecordingInfoEvent(const ZrcMeetingRecordingInfo* s) { if (meetingRecordingInfoCallback) meetingRecordingInfoCallback(s, meetingRecordingInfoUserData); }
    void RaiseCameraPresetInfoEvent(const ZrcCameraPresetInfo* s) { if (cameraPresetInfoCallback) cameraPresetInfoCallback(s, cameraPresetInfoUserData); }
    void RaiseControlSystemEnabledEvent(int enabled)              { Raise(controlSystemEnabledCallback, controlSystemEnabledUserData, "", enabled); }
    // Extended raise helpers
    void RaiseAllowAttendeesUnmuteEvent(int allow)          { Raise(allowAttendeesUnmuteCallback, allowAttendeesUnmuteUserData, "", allow); }
    void RaiseAskUnmuteByHostEvent(int type)                { Raise(askUnmuteByHostCallback, askUnmuteByHostUserData, "", type); }
    void RaiseFEACRequestEvent(int32_t userID, const char* name) { Raise(feacRequestCallback, feacRequestUserData, name, (int)userID); }
    void RaiseFEACApprovedEvent(int32_t userID, const char* name){ Raise(feacApprovedCallback, feacApprovedUserData, name, (int)userID); }
    void RaiseFEACDeclinedEvent(int32_t userID, const char* name){ Raise(feacDeclinedCallback, feacDeclinedUserData, name, (int)userID); }
    void RaiseAllowAttendeesVideoEvent(int allow)           { Raise(allowAttendeesVideoCallback, allowAttendeesVideoUserData, "", allow); }
    void RaiseSharingStatusEvent(const ZrcSharingStatus* s) { if (sharingStatusCallback) sharingStatusCallback(s, sharingStatusUserData); }
    void RaiseAirPlayStatusEvent(const ZrcAirPlayStatus* s) { if (airPlayStatusCallback) airPlayStatusCallback(s, airPlayStatusUserData); }
    void RaiseVideoPageStatusEvent(const ZrcVideoPageStatus* s) { if (videoPageStatusCallback) videoPageStatusCallback(s, videoPageStatusUserData); }
    void RaiseScreenLayoutStatusEvent(const ZrcScreenLayoutStatus* s) { if (screenLayoutStatusCallback) screenLayoutStatusCallback(s, screenLayoutStatusUserData); }
    void RaiseBOStatusChangedEvent(int status)              { Raise(boStatusChangedCallback, boStatusChangedUserData, "", status); }
    void RaiseInSilentModeEvent(int inSilent)               { Raise(inSilentModeCallback, inSilentModeUserData, "", inSilent); }
    void RaiseReactionStatusEvent(int feedback)             { Raise(reactionStatusCallback, reactionStatusUserData, "", feedback); }
    void RaiseChatPrivilegeEvent(int privilegeType)         { Raise(chatPrivilegeCallback, chatPrivilegeUserData, "", privilegeType); }
    void RaiseChatMessageEvent(const ZrcChatMessage* msg)   { if (chatMessageCallback) chatMessageCallback(msg, chatMessageUserData); }
    void RaiseCaptionEvent(const ZrcCaption* cap)           { if (captionCallback) captionCallback(cap, captionUserData); }
    void RaiseMeetingLockStatusEvent(int locked)            { Raise(meetingLockStatusCallback, meetingLockStatusUserData, "", locked); }
    void RaiseQAEnabledEvent(int enabled)                   { Raise(qaEnabledCallback, qaEnabledUserData, "", enabled); }
    void RaiseRecordingRequestEvent(int userID)             { Raise(recordingRequestCallback, recordingRequestUserData, "", userID); }
    void RaiseFarEndCameraControlEvent(int userID)          { Raise(farEndCameraControlCallback, farEndCameraControlUserData, "", userID); }
    void RaiseSIPCallEvent(const ZrcSIPCall* call)          { if (sipCallStatusCallback) sipCallStatusCallback(call, sipCallStatusUserData); }
    void RaiseSIPServiceStatusEvent(const char* name, int status) { Raise(sipServiceStatusCallback, sipServiceStatusUserData, name, status); }
    void RaiseBORoomListEvent(const std::vector<BreakoutRoomInfo>& rooms);
    void RaiseZRCSDeviceListEvent(ControlSystemUpdateDeviceType type, const ControlSystemDeviceList& list);
    void RaiseZRCSSceneListEvent(const std::vector<ControlSystemSceneInfo>& scenes);

    // eventType: 0=join, 1=initialize (full replace), 2=leave, 3=update
    void RaiseParticipantListEvent(const std::vector<MeetingParticipant>& participants,
                                   int total, int eventType, ConfSessionType session);
    void RaiseContactListEvent(const std::vector<Contact>& contacts);
    void RaiseMeetingListEvent(int result, const std::vector<MeetingItem>& meetings);
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
                                               int total, int eventType, ConfSessionType session)
{
    if (!participantListCallback) return;
    std::vector<ZrcParticipant> flat(participants.size());
    for (size_t i = 0; i < participants.size(); ++i)
        FlattenParticipant(participants[i], flat[i]);
    participantListCallback(flat.empty() ? nullptr : flat.data(),
                            (int)flat.size(), eventType,
                            (int)session, participantListUserData);
}

// Helper: flatten a C++ Contact to the C ZrcContact struct.
static void FlattenContact(const Contact& src, ZrcContact& dst)
{
    memset(&dst, 0, sizeof(ZrcContact));
    strncpy_safe(dst.contactID,      src.contactID,      sizeof(dst.contactID));
    strncpy_safe(dst.screenName,     src.screenName,     sizeof(dst.screenName));
    strncpy_safe(dst.firstName,      src.firstName,      sizeof(dst.firstName));
    strncpy_safe(dst.lastName,       src.lastName,       sizeof(dst.lastName));
    strncpy_safe(dst.email,          src.email,          sizeof(dst.email));
    strncpy_safe(dst.phoneNumber,    src.phoneNumber,    sizeof(dst.phoneNumber));
    strncpy_safe(dst.sipPhoneNumber, src.sipPhoneNumber, sizeof(dst.sipPhoneNumber));
    dst.presenceStatus = (int32_t)src.presenceStatus;
    dst.buddyType      = (int32_t)src.buddyType;
}

void ZrcSdkInstance::RaiseContactListEvent(const std::vector<Contact>& contacts)
{
    if (!contactListCallback) return;
    std::vector<ZrcContact> flat(contacts.size());
    for (size_t i = 0; i < contacts.size(); ++i)
        FlattenContact(contacts[i], flat[i]);
    contactListCallback(flat.empty() ? nullptr : flat.data(),
                        (int)flat.size(), contactListUserData);
}

void ZrcContactHelperSink::OnImUpdateContactNotification(const std::vector<Contact>& contacts)
{
    if (owner) owner->RaiseContactListEvent(contacts);
}

void ZrcContactHelperSink::OnDynamicContactListNotification(const DynamicContactListInfo& info)
{
    if (owner) owner->RaiseContactListEvent(info.contacts);
}

// Helper: flatten a C++ MeetingItem to the C ZrcMeetingItem struct.
static void FlattenMeetingItem(const MeetingItem& src, ZrcMeetingItem& dst)
{
    memset(&dst, 0, sizeof(ZrcMeetingItem));
    strncpy_safe(dst.meetingNumber, src.meetingNumber, sizeof(dst.meetingNumber));
    strncpy_safe(dst.meetingName,   src.meetingName,   sizeof(dst.meetingName));
    strncpy_safe(dst.hostName,      src.hostName,      sizeof(dst.hostName));
    strncpy_safe(dst.startTime,     src.startTime,     sizeof(dst.startTime));
    strncpy_safe(dst.endTime,       src.endTime,       sizeof(dst.endTime));
    strncpy_safe(dst.meetingDomain, src.meetingDomain, sizeof(dst.meetingDomain));
    dst.scheduledFrom    = (int32_t)src.scheduledFrom;
    dst.isPrivate        = src.isPrivate ? 1 : 0;
    dst.isAllDayEvent    = src.isAllDayEvent ? 1 : 0;
    dst.isCheckedIn      = src.isCheckedIn ? 1 : 0;
    dst.isInstantMeeting = src.isInstantMeeting ? 1 : 0;
}

void ZrcSdkInstance::RaiseMeetingListEvent(int result, const std::vector<MeetingItem>& meetings)
{
    if (!meetingListCallback) return;
    std::vector<ZrcMeetingItem> flat(meetings.size());
    for (size_t i = 0; i < meetings.size(); ++i)
        FlattenMeetingItem(meetings[i], flat[i]);
    meetingListCallback(result, flat.empty() ? nullptr : flat.data(),
                        (int)flat.size(), meetingListUserData);
}

void ZrcMeetingListHelperSink::OnUpdateMeetingList(ListMeetingResult result, const std::vector<MeetingItem>& meetingList)
{
    if (owner) owner->RaiseMeetingListEvent((int)result, meetingList);
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

// Flatten the SDK MeetingInvitationInfo into the C ZrcMeetingInvite and raise it.
void ZrcSdkInstance::RaiseMeetingInviteEvent(const MeetingInvitationInfo& invite)
{
    if (!meetingInviteCallback) return;
    ZrcMeetingInvite flat;
    memset(&flat, 0, sizeof(flat));
    strncpy_safe(flat.callerName,      invite.callerName,      sizeof(flat.callerName));
    strncpy_safe(flat.callerContactID, invite.callerContactID, sizeof(flat.callerContactID));
    strncpy_safe(flat.meetingID,       invite.meetingID,       sizeof(flat.meetingID));
    strncpy_safe(flat.meetingNumber,   std::to_string(invite.meetingNumber), sizeof(flat.meetingNumber));
    meetingInviteCallback(&flat, meetingInviteUserData);
}

void ZrcMeetingServiceSink::OnReceiveMeetingInviteNotification(const MeetingInvitationInfo& invitation)
{
    if (!owner) return;
    // Cache the full invite so AnswerMeetingInvite (accept/decline) can answer it by struct.
    {
        std::lock_guard<std::mutex> lk(owner->meetingInviteMutex);
        owner->lastMeetingInvite = invitation;
        owner->hasMeetingInvite = true;
    }
    owner->RaiseMeetingInviteEvent(invitation);
}

void ZrcMeetingServiceSink::OnTreatedMeetingInviteNotification(const MeetingInvitationInfo& invitation, bool accepted)
{
    if (!owner) return;
    // The invite was answered (here or elsewhere) — clear the cache to avoid acting on a stale invite.
    // Match the meeting number before clearing so a newer invite that arrived between this callback
    // and acquiring the mutex is not discarded (mirrors the guard in ZrcSdk_AnswerMeetingInvite).
    std::lock_guard<std::mutex> lk(owner->meetingInviteMutex);
    if (owner->hasMeetingInvite && owner->lastMeetingInvite.meetingNumber == invitation.meetingNumber)
        owner->hasMeetingInvite = false;
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

void ZrcMeetingAudioHelperSink::OnAskUnmuteAudioByHostNotification(bool show, AskUnmuteAudioByHostType type)
{
    if (owner && show) owner->RaiseAskUnmuteByHostEvent((int)type);
}

void ZrcMeetingAudioHelperSink::OnAllowAttendeesUnmuteThemselvesNotification(bool canAttendeesUnmuteThemselves)
{
    if (owner) owner->RaiseAllowAttendeesUnmuteEvent(canAttendeesUnmuteThemselves ? 1 : 0);
}

void ZrcMeetingAudioHelperSink::OnFEACApproveNotification(int32_t farEndUserID, const std::string& farEndUserName)
{
    if (owner) owner->RaiseFEACApprovedEvent(farEndUserID, farEndUserName.c_str());
}

void ZrcMeetingAudioHelperSink::OnFEACDeclineNotification(int32_t farEndUserID, const std::string& farEndUserName)
{
    if (owner) owner->RaiseFEACDeclinedEvent(farEndUserID, farEndUserName.c_str());
}

void ZrcMeetingAudioHelperSink::OnFEACRequestNotification(int32_t requesterUserID, const std::string& requesterUserName)
{
    if (owner) owner->RaiseFEACRequestEvent(requesterUserID, requesterUserName.c_str());
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
    owner->RaiseParticipantListEvent(participants, totalParticipantsCount, needCleanUpUserList ? 1 : 0, session);
}

void ZrcParticipantHelperSink::OnUserJoin(const std::vector<MeetingParticipant>& participants, ConfSessionType session)
{
    if (!owner) return;
    if (session == CurrentSession)
    {
        owner->participantCount += (int)participants.size();
        owner->RaiseParticipantCountEvent(owner->participantCount);
    }
    owner->RaiseParticipantListEvent(participants, owner->participantCount, 0, session); // eventType 0 = join
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
    owner->RaiseParticipantListEvent(participants, owner->participantCount, 2, session); // eventType 2 = leave
}

void ZrcParticipantHelperSink::OnUserUpdate(const std::vector<MeetingParticipant>& participants, ConfSessionType session)
{
    if (owner) owner->RaiseParticipantListEvent(participants, owner->participantCount, 3, session); // eventType 3 = update
}

void ZrcParticipantHelperSink::OnHostChangedNotification(int32_t /*hostUserID*/, bool amIHost, ConfSessionType session)
{
    if (owner && session == CurrentSession) owner->RaiseHostChangedEvent(amIHost ? 1 : 0);
}

void ZrcRecordingHelperSink::OnUpdateMeetingRecordingInfo(const MeetingRecordingInfo& recordingInfo)
{
    if (!owner) return;
    owner->RaiseRecordingStatusEvent(recordingInfo.isMeetingBeingRecorded ? 1 : 0);
    ZrcMeetingRecordingInfo info;
    info.isMeetingBeingRecorded = recordingInfo.isMeetingBeingRecorded ? 1 : 0;
    info.canIRecord             = recordingInfo.canIRecord ? 1 : 0;
    info.amIRecording           = recordingInfo.amIRecording ? 1 : 0;
    owner->RaiseMeetingRecordingInfoEvent(&info);
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

static void FlattenSIPCall(const SIPCallInfo& src, ZrcSIPCall& dst)
{
    memset(&dst, 0, sizeof(ZrcSIPCall));
    strncpy_safe(dst.callID, src.callID, sizeof(dst.callID));
    dst.status = (int32_t)src.status;
    strncpy_safe(dst.peerDisplayName, src.peerDisplayName, sizeof(dst.peerDisplayName));
    strncpy_safe(dst.peerNumber, src.peerNumber, sizeof(dst.peerNumber));
    dst.isIncomingCall = src.isIncomingCall ? 1 : 0;
    dst.elapsedCallTime = src.elapsedCallTime;
}

void ZrcControlSystemHelperSink::OnUpdateZRCSDeviceList(ControlSystemUpdateDeviceType type, const ControlSystemDeviceList& list)
{
    if (owner) owner->RaiseZRCSDeviceListEvent(type, list);
}

void ZrcControlSystemHelperSink::OnUpdateZRCSSceneList(const std::vector<ControlSystemSceneInfo>& scenes)
{
    if (owner) owner->RaiseZRCSSceneListEvent(scenes);
}

void ZrcSdkInstance::RaiseZRCSDeviceListEvent(ControlSystemUpdateDeviceType type, const ControlSystemDeviceList& list)
{
    if (!zrcsDeviceListCallback) return;
    std::vector<ZrcZRCSDevice> flat(list.devices.size());
    for (size_t i = 0; i < list.devices.size(); ++i)
    {
        memset(&flat[i], 0, sizeof(ZrcZRCSDevice));
        strncpy_safe(flat[i].deviceID, list.devices[i].deviceID, sizeof(flat[i].deviceID));
        strncpy_safe(flat[i].deviceName, list.devices[i].name, sizeof(flat[i].deviceName));
        flat[i].deviceStatus = (int32_t)list.devices[i].status;
        flat[i].updateType = (int32_t)type;
    }
    zrcsDeviceListCallback(flat.empty() ? nullptr : flat.data(), (int)flat.size(), (int)type, zrcsDeviceListUserData);
}

void ZrcSdkInstance::RaiseZRCSSceneListEvent(const std::vector<ControlSystemSceneInfo>& scenes)
{
    if (!zrcsSceneListCallback) return;
    std::vector<ZrcZRCSScene> flat(scenes.size());
    for (size_t i = 0; i < scenes.size(); ++i)
    {
        memset(&flat[i], 0, sizeof(ZrcZRCSScene));
        strncpy_safe(flat[i].sceneID, scenes[i].sceneID, sizeof(flat[i].sceneID));
        strncpy_safe(flat[i].sceneName, scenes[i].name, sizeof(flat[i].sceneName));
        strncpy_safe(flat[i].sceneIcon, scenes[i].icon, sizeof(flat[i].sceneIcon));
    }
    zrcsSceneListCallback(flat.empty() ? nullptr : flat.data(), (int)flat.size(), zrcsSceneListUserData);
}

void ZrcSdkInstance::RaiseBORoomListEvent(const std::vector<BreakoutRoomInfo>& rooms)
{
    if (!boRoomListCallback) return;
    std::vector<ZrcBORoom> flat(rooms.size());
    for (size_t i = 0; i < rooms.size(); ++i)
    {
        memset(&flat[i], 0, sizeof(ZrcBORoom));
        strncpy_safe(flat[i].sessionBID, rooms[i].sessionBID, sizeof(flat[i].sessionBID));
        strncpy_safe(flat[i].sessionName, rooms[i].sessionName, sizeof(flat[i].sessionName));
    }
    boRoomListCallback(flat.empty() ? nullptr : flat.data(), (int)flat.size(), boRoomListUserData);
}

void ZrcMeetingVideoHelperSink::OnUpdateAllowAttendeesStartVideo(bool allow)
{
    if (owner) owner->RaiseAllowAttendeesVideoEvent(allow ? 1 : 0);
}

void ZrcMeetingShareHelperSink::OnSharingStatusNotification(const SharingStatus& status)
{
    if (!owner) return;
    ZrcSharingStatus s;
    s.sharingState   = (int32_t)status.sharingState;
    s.canShareToBO   = status.canShareToBO ? 1 : 0;
    s.isSharingToBO  = status.isSharingToBO ? 1 : 0;
    owner->RaiseSharingStatusEvent(&s);
}

void ZrcMeetingShareHelperSink::OnUpdateAirPlayBlackMagicStatus(const AirplayBlackMagicStatus& status)
{
    if (!owner) return;
    ZrcAirPlayStatus s;
    memset(&s, 0, sizeof(s));
    s.instructionDisplayState       = (int32_t)status.instructionDisplayState;
    strncpy_safe(s.wifiName,                       status.wifiName.c_str(),                       sizeof(s.wifiName));
    strncpy_safe(s.serverName,                     status.serverName.c_str(),                     sizeof(s.serverName));
    strncpy_safe(s.password,                       status.password.c_str(),                       sizeof(s.password));
    strncpy_safe(s.directPresentationPairingCode,  status.directPresentationPairingCode.c_str(),  sizeof(s.directPresentationPairingCode));
    strncpy_safe(s.directPresentationSharingKey,   status.directPresentationSharingKey.c_str(),   sizeof(s.directPresentationSharingKey));
    s.isAirHostClientConnected      = status.isAirHostClientConnected ? 1 : 0;
    s.isBlackMagicConnected         = status.isBlackMagicConnected ? 1 : 0;
    s.isBlackMagicDataAvailable     = status.isBlackMagicDataAvailable ? 1 : 0;
    s.isSharingBlackMagic           = status.isSharingBlackMagic ? 1 : 0;
    s.isDirectPresentationConnected = status.isDirectPresentationConnected ? 1 : 0;
    owner->RaiseAirPlayStatusEvent(&s);
}


void ZrcMeetingViewLayoutHelperSink::OnUpdateVideoPageStatusNotification(const VideoPageStatus& status)
{
    if (!owner) return;
    ZrcVideoPageStatus s;
    s.isInFirstPage           = status.isInFirstPage ? 1 : 0;
    s.isInLastPage            = status.isInLastPage ? 1 : 0;
    s.pageVideoType           = (int32_t)status.pageVideoType;
    s.videoCountInCurrentPage = status.videoCountInCurrentPage;
    owner->RaiseVideoPageStatusEvent(&s);
}

void ZrcMeetingViewLayoutHelperSink::OnUpdateScreenLayoutStatus(const ScreenLayoutStatus& status)
{
    if (!owner) return;
    ZrcScreenLayoutStatus s = {};
    s.canShowContentOnly                            = status.canShowContentOnly ? 1 : 0;
    s.isInContentOnly                               = status.isInContentOnly ? 1 : 0;
    s.canAdjustFloatingVideo                        = status.canAdjustFloatingVideo ? 1 : 0;
    s.canSwitchFloatingShareContent                 = status.canSwitchFloatingShareContent ? 1 : 0;
    s.isInFloatingShareContent                      = status.isInFloatingShareContent ? 1 : 0;
    s.canAdjustMyAutoGeneratedVideoStreamsVisibility = status.canAdjustMyAutoGeneratedVideoStreamsVisibility ? 1 : 0;
    s.isShowMyAutoGeneratedVideoStreams              = status.isShowMyAutoGeneratedVideoStreams ? 1 : 0;

    int screenCount = (int)status.layoutInfos.size();
    if (screenCount > ZRC_MAX_SCREEN_LAYOUT_SCREENS) screenCount = ZRC_MAX_SCREEN_LAYOUT_SCREENS;
    s.screenCount = screenCount;

    for (int i = 0; i < screenCount; ++i)
    {
        const auto& src = status.layoutInfos[i];
        ZrcScreenLayoutInfo& dst = s.screens[i];
        dst.screen = (int32_t)src.screen;
        dst.layout = (int32_t)src.layout;

        int ctrlCount = (int)src.layoutCtrlInfos.size();
        if (ctrlCount > ZRC_MAX_SCREEN_LAYOUT_CTRLS) ctrlCount = ZRC_MAX_SCREEN_LAYOUT_CTRLS;
        dst.ctrlInfoCount = ctrlCount;

        for (int j = 0; j < ctrlCount; ++j)
        {
            const auto& csrc = src.layoutCtrlInfos[j];
            dst.ctrlInfos[j].layout  = (int32_t)csrc.layout;
            dst.ctrlInfos[j].enable  = csrc.enable ? 1 : 0;
            dst.ctrlInfos[j].visible = csrc.visible ? 1 : 0;
        }
    }

    owner->RaiseScreenLayoutStatusEvent(&s);
}

void ZrcMeetingControlHelperSink::OnUpdateMeetingLockStatus(bool locked)
{
    if (owner) owner->RaiseMeetingLockStatusEvent(locked ? 1 : 0);
}

void ZrcReactionEmojiHelperSink::OnUpdateReactionStatus(const ReactionStatus& status)
{
    if (owner) owner->RaiseReactionStatusEvent((int)status.selectedFeedback);
}

void ZrcWaitingRoomHelperSink::OnInSilentModeNotification(const InSilentModeInfo& info)
{
    if (owner) owner->RaiseInSilentModeEvent(info.isInSilentMode ? 1 : 0);
}

void ZrcBreakoutRoomHelperSink::OnBOStatusChanged(BO_STATUS boStatus)
{
    if (owner) owner->RaiseBOStatusChangedEvent((int)boStatus);
}

static void FlattenChatMessage(const MeetingChatMessage& src, ZrcChatMessage& dst)
{
    memset(&dst, 0, sizeof(ZrcChatMessage));
    strncpy_safe(dst.messageID, src.messageID, sizeof(dst.messageID));
    dst.senderMeetingUserID   = src.senderID;
    strncpy_safe(dst.senderName, src.senderDisplayName, sizeof(dst.senderName));
    dst.receiverMeetingUserID = src.receiverID;
    dst.sendToType = (int32_t)src.msgSendToType;
    strncpy_safe(dst.content, src.content, sizeof(dst.content));
    dst.isDeleted = src.isDeleted ? 1 : 0;
}

static void FlattenNewChatMessage(const NewMeetingChatMessage& src, ZrcChatMessage& dst)
{
    memset(&dst, 0, sizeof(ZrcChatMessage));
    strncpy_safe(dst.messageID, src.messageID, sizeof(dst.messageID));
    dst.senderMeetingUserID   = src.senderMeetingUserID;
    strncpy_safe(dst.senderName, src.senderName, sizeof(dst.senderName));
    dst.sendToType = (int32_t)src.chatSendToType;
    dst.messageTime = src.messageServerTime;
    strncpy_safe(dst.content, src.body, sizeof(dst.content));
    dst.isDeleted = src.isDeleted ? 1 : 0;
}

void ZrcMeetingChatHelperSink::OnMeetingChatPrivilegeNotification(const MeetingChatPrivilege& priv)
{
    if (owner) owner->RaiseChatPrivilegeEvent((int)priv.type);
}

void ZrcMeetingChatHelperSink::OnMeetingChatMessagesNotification(const std::vector<MeetingChatMessage>& messages)
{
    if (!owner) return;
    for (const auto& m : messages)
    {
        ZrcChatMessage flat;
        FlattenChatMessage(m, flat);
        owner->RaiseChatMessageEvent(&flat);
    }
}

void ZrcMeetingChatHelperSink::OnThreadReceived(const NewMeetingChatMessage& thread)
{
    if (!owner) return;
    ZrcChatMessage flat;
    FlattenNewChatMessage(thread, flat);
    owner->RaiseChatMessageEvent(&flat);
}

void ZrcClosedCaptionHelperSink::OnMessageAdd(const LTTCaptionMessage& msg)
{
    if (!owner) return;
    ZrcCaption cap;
    memset(&cap, 0, sizeof(ZrcCaption));
    strncpy_safe(cap.messageID, msg.messageID, sizeof(cap.messageID));
    cap.userID = msg.userNodeID;
    strncpy_safe(cap.userName, msg.userName, sizeof(cap.userName));
    cap.messageTime = msg.messageTime;
    strncpy_safe(cap.content, msg.messageContent, sizeof(cap.content));
    cap.isFinal = 1;
    owner->RaiseCaptionEvent(&cap);
}

void ZrcCameraControlHelperSink::OnFarEndCameraControlNotification(const FarEndCameraControlInfo& info)
{
    if (owner) owner->RaiseFarEndCameraControlEvent((int)info.userID);
}

void ZrcCameraControlHelperSink::OnUpdateCameraPresetInfo(const CameraPresetInfo& info)
{
    if (!owner) return;
    ZrcCameraPresetInfo out;
    memset(&out, 0, sizeof(out));
    out.defaultIndex = info.defaultIndex;
    out.supportedPresetCount = (int32_t)info.supportedPresetCount;
    int n = 0;
    for (const auto& kv : info.namedPresets)
    {
        if (n >= 3) break;
        out.presets[n].index = (int32_t)kv.first;
        strncpy(out.presets[n].name, kv.second.c_str(), sizeof(out.presets[n].name) - 1);
        out.presets[n].name[sizeof(out.presets[n].name) - 1] = '\0';
        ++n;
    }
    out.presetCount = n;
    owner->RaiseCameraPresetInfoEvent(&out);
}

void ZrcPhoneCallServiceSink::OnReceiveIncomingSIPCallNotification(const SIPCallInfo& call)
{
    if (!owner) return;
    { std::lock_guard<std::mutex> lk(owner->sipCallsMutex); owner->sipCalls[call.callID] = call; }
    ZrcSIPCall flat;
    FlattenSIPCall(call, flat);
    owner->RaiseSIPCallEvent(&flat);
}

void ZrcPhoneCallServiceSink::OnTerminateSIPCallNotification(SIPCallTerminateReason /*reason*/, const SIPCallInfo& call)
{
    if (!owner) return;
    { std::lock_guard<std::mutex> lk(owner->sipCallsMutex); owner->sipCalls.erase(call.callID); }
    ZrcSIPCall flat;
    FlattenSIPCall(call, flat);
    owner->RaiseSIPCallEvent(&flat);
}

void ZrcPhoneCallServiceSink::OnUpdateSIPCallStatusNotification(const SIPCallInfo& call)
{
    if (!owner) return;
    { std::lock_guard<std::mutex> lk(owner->sipCallsMutex); owner->sipCalls[call.callID] = call; }
    ZrcSIPCall flat;
    FlattenSIPCall(call, flat);
    owner->RaiseSIPCallEvent(&flat);
}

void ZrcPhoneCallServiceSink::OnUpdateSIPServiceStatusNotification(const SIPService& service)
{
    if (owner) owner->RaiseSIPServiceStatusEvent(service.displayName.c_str(), (int)service.status);
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
    // Sinks were deregistered from their helpers in ZrcSdk_Uninitialize (above) before the SDK
    // singleton was destroyed, so these deletes cannot race a live SDK callback. Null after delete
    // to stay safe against any re-entrant teardown.
    delete inst->pContactHelperSink;     inst->pContactHelperSink = nullptr;
    delete inst->pMeetingListHelperSink; inst->pMeetingListHelperSink = nullptr;
    delete inst->pPhoneCallServiceSink;
    delete inst->pCameraControlHelperSink;
    delete inst->pClosedCaptionHelperSink;
    delete inst->pChatHelperSink;
    delete inst->pBreakoutRoomHelperSink;
    delete inst->pWaitingRoomHelperSink;
    delete inst->pReactionEmojiHelperSink;
    delete inst->pMeetingControlHelperSink;
    delete inst->pViewLayoutHelperSink;
    delete inst->pShareHelperSink;
    delete inst->pVideoHelperSink;
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

        // Setting service: speaker/output volume get/set and camera device list/selection. No sink
        // registered: ISettingServiceSink has ~40 callbacks, so volume and camera-selection feedback
        // are driven by the host (read on connect / after set), not by SDK push notifications.
        inst->pSettingService = inst->pZoomRoomsService->GetSettingService();

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

            IContactHelper* pContacts = inst->pPreMeetingService->GetContactHelper();
            if (pContacts)
            {
                inst->pContactHelper = pContacts;
                inst->pContactHelperSink = new ZrcContactHelperSink(inst);
                pContacts->RegisterSink(inst->pContactHelperSink);
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

            IMeetingVideoHelper* pVideo = inst->pMeetingService->GetMeetingVideoHelper();
            if (pVideo)
            {
                inst->pVideoHelperSink = new ZrcMeetingVideoHelperSink(inst);
                pVideo->RegisterSink(inst->pVideoHelperSink);
            }

            IMeetingShareHelper* pShare = inst->pMeetingService->GetMeetingShareHelper();
            if (pShare)
            {
                inst->pShareHelperSink = new ZrcMeetingShareHelperSink(inst);
                pShare->RegisterSink(inst->pShareHelperSink);
            }

            IMeetingViewLayoutHelper* pLayout = inst->pMeetingService->GetMeetingViewLayoutHelper();
            if (pLayout)
            {
                inst->pViewLayoutHelperSink = new ZrcMeetingViewLayoutHelperSink(inst);
                pLayout->RegisterSink(inst->pViewLayoutHelperSink);
            }

            IMeetingControlHelper* pControl = inst->pMeetingService->GetMeetingControlHelper();
            if (pControl)
            {
                inst->pMeetingControlHelperSink = new ZrcMeetingControlHelperSink(inst);
                pControl->RegisterSink(inst->pMeetingControlHelperSink);
            }

            IMeetingReactionEmojiHelper* pReaction = inst->pMeetingService->GetMeetingReactionEmojiHelper();
            if (pReaction)
            {
                inst->pReactionEmojiHelperSink = new ZrcReactionEmojiHelperSink(inst);
                pReaction->RegisterSink(inst->pReactionEmojiHelperSink);
            }

            IWaitingRoomHelper* pWR = inst->pMeetingService->GetWaitingRoomHelper();
            if (pWR)
            {
                inst->pWaitingRoomHelperSink = new ZrcWaitingRoomHelperSink(inst);
                pWR->RegisterSink(inst->pWaitingRoomHelperSink);
            }

            IBreakoutRoomHelper* pBO = inst->pMeetingService->GetBreakoutRoomHelper();
            if (pBO)
            {
                inst->pBreakoutRoomHelperSink = new ZrcBreakoutRoomHelperSink(inst);
                pBO->RegisterSink(inst->pBreakoutRoomHelperSink);
            }

            IMeetingChatHelper* pChat = inst->pMeetingService->GetMeetingChatHelper();
            if (pChat)
            {
                inst->pChatHelperSink = new ZrcMeetingChatHelperSink(inst);
                pChat->RegisterSink(inst->pChatHelperSink);
            }

            IClosedCaptionHelper* pCC = inst->pMeetingService->GetClosedCaptionHelper();
            if (pCC)
            {
                inst->pClosedCaptionHelperSink = new ZrcClosedCaptionHelperSink(inst);
                pCC->RegisterSink(inst->pClosedCaptionHelperSink);
            }

            ICameraControlHelper* pCam = inst->pMeetingService->GetCameraControlHelper();
            if (pCam)
            {
                inst->pCameraControlHelperSink = new ZrcCameraControlHelperSink(inst);
                pCam->RegisterSink(inst->pCameraControlHelperSink);
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

            IMeetingListHelper* pMeetingList = inst->pMeetingService->GetMeetingListHelper();
            if (pMeetingList)
            {
                inst->pMeetingListHelper = pMeetingList;
                inst->pMeetingListHelperSink = new ZrcMeetingListHelperSink(inst);
                pMeetingList->RegisterSink(inst->pMeetingListHelperSink);
            }
        }

        // Phone call service
        IPhoneCallService* pPhone = inst->pZoomRoomsService->GetPhoneCallService();
        if (pPhone)
        {
            inst->pPhoneCallService = pPhone;
            inst->pPhoneCallServiceSink = new ZrcPhoneCallServiceSink(inst);
            pPhone->RegisterSink(inst->pPhoneCallServiceSink);
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

            // Deregister our sinks while the SDK singleton (and the owning helpers) are still alive,
            // so no queued callback can fire on a sink we are about to delete in ZrcSdk_Destroy. The
            // SDK holds the sink pointer directly, so nulling our copy alone would not prevent a
            // dangling call — the sink must be deregistered from the helper first.
            if (inst->pContactHelper && inst->pContactHelperSink)
                inst->pContactHelper->DeregisterSink(inst->pContactHelperSink);
            if (inst->pMeetingListHelper && inst->pMeetingListHelperSink)
                inst->pMeetingListHelper->DeregisterSink(inst->pMeetingListHelperSink);

            inst->pNativeSDK->ForceFlushLog();  // flush credential cache and logs to disk
            IZRCSDK::DestroyInstance();          // cleanly shut down SDK singleton
            inst->pNativeSDK = nullptr;

            // Helper/service pointers are owned by the now-destroyed SDK; clear them so nothing
            // reuses dangling memory (entrypoints also gate on bInitialized, but don't rely on it).
            inst->pContactHelper = nullptr;
            inst->pMeetingListHelper = nullptr;
            inst->pSettingService = nullptr;
            inst->pMeetingService = nullptr;
            inst->pPreMeetingService = nullptr;
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

// ─── Audio Extensions ─────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_MuteUserAudio(ZrcSdkHandle handle, int32_t userID, int mute)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingAudioHelper* pA = pMS->GetMeetingAudioHelper();
    if (!pA) { inst->RaiseErrorEvent("Audio Helper not available", -1); return -1; }
    return (int)pA->MuteUserAudio(userID, mute != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_MuteAllAudio(ZrcSdkHandle handle, int mute)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingAudioHelper* pA = pMS->GetMeetingAudioHelper();
    if (!pA) { inst->RaiseErrorEvent("Audio Helper not available", -1); return -1; }
    return (int)pA->MuteAllAudios(mute != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetMuteOnEntry(ZrcSdkHandle handle, int mute)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingAudioHelper* pA = pMS->GetMeetingAudioHelper();
    if (!pA) { inst->RaiseErrorEvent("Audio Helper not available", -1); return -1; }
    return (int)pA->MuteOnEntry(mute != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AnswerUnmuteRequest(ZrcSdkHandle handle, int accepted)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingAudioHelper* pA = pMS->GetMeetingAudioHelper();
    if (!pA) { inst->RaiseErrorEvent("Audio Helper not available", -1); return -1; }
    return (int)pA->AnswerUnmuteAudioByHostRequest(accepted != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AllowAttendeesUnmute(ZrcSdkHandle handle, int allow)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingAudioHelper* pA = pMS->GetMeetingAudioHelper();
    if (!pA) { inst->RaiseErrorEvent("Audio Helper not available", -1); return -1; }
    return (int)pA->AllowAttendeesUnmuteThemselves(allow != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RequestFarEndAudioControl(ZrcSdkHandle handle, int32_t farEndUserID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingAudioHelper* pA = pMS->GetMeetingAudioHelper();
    if (!pA) { inst->RaiseErrorEvent("Audio Helper not available", -1); return -1; }
    return (int)pA->RequestFarEndAudioControl(farEndUserID);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GiveUpFarEndAudioControl(ZrcSdkHandle handle, int32_t farEndUserID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingAudioHelper* pA = pMS->GetMeetingAudioHelper();
    if (!pA) { inst->RaiseErrorEvent("Audio Helper not available", -1); return -1; }
    return (int)pA->GiveUpFarEndAudioControl(farEndUserID);
}

// ─── Video Extensions ─────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_MuteUserVideo(ZrcSdkHandle handle, int32_t userID, int mute)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingVideoHelper* pV = pMS->GetMeetingVideoHelper();
    if (!pV) { inst->RaiseErrorEvent("Video Helper not available", -1); return -1; }
    return (int)pV->MuteUserVideo(userID, mute != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AllowAttendeesStartVideo(ZrcSdkHandle handle, int allow)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingVideoHelper* pV = pMS->GetMeetingVideoHelper();
    if (!pV) { inst->RaiseErrorEvent("Video Helper not available", -1); return -1; }
    return (int)pV->AllowAttendeesStartVideo(allow != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PinUserOnScreen(ZrcSdkHandle handle, int32_t userID, int32_t screenIndex)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingVideoHelper* pV = pMS->GetMeetingVideoHelper();
    if (!pV) { inst->RaiseErrorEvent("Video Helper not available", -1); return -1; }
    return (int)pV->PinUserOnScreen(userID, screenIndex);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UnpinUserFromScreen(ZrcSdkHandle handle, int32_t userID, int32_t screenIndex)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingVideoHelper* pV = pMS->GetMeetingVideoHelper();
    if (!pV) { inst->RaiseErrorEvent("Video Helper not available", -1); return -1; }
    return (int)pV->UnpinUserFromScreen(userID, screenIndex);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SpotlightUser(ZrcSdkHandle handle, int32_t userID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingVideoHelper* pV = pMS->GetMeetingVideoHelper();
    if (!pV) { inst->RaiseErrorEvent("Video Helper not available", -1); return -1; }
    return (int)pV->SpotlightUser(userID);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CancelSpotlightUser(ZrcSdkHandle handle, int32_t userID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingVideoHelper* pV = pMS->GetMeetingVideoHelper();
    if (!pV) { inst->RaiseErrorEvent("Video Helper not available", -1); return -1; }
    return (int)pV->CancelSpotlightUser(userID);
}

// ─── Share Extensions ─────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartShare(ZrcSdkHandle handle, const char* shareSourceID)
{
    if (!handle || !shareSourceID) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingShareHelper* pSh = pMS->GetMeetingShareHelper();
    if (!pSh) { inst->RaiseErrorEvent("Share Helper not available", -1); return -1; }
    (void)shareSourceID;
    // No generic StartShare in this SDK version; use ShareCamera for camera source
    return (int)pSh->ShareCamera(true, std::string(shareSourceID));
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StopShare(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingShareHelper* pSh = pMS->GetMeetingShareHelper();
    if (!pSh) { inst->RaiseErrorEvent("Share Helper not available", -1); return -1; }
    return (int)pSh->StopSharing();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PinShare(ZrcSdkHandle handle, int32_t shareSourceID, int32_t screenIndex)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingShareHelper* pSh = pMS->GetMeetingShareHelper();
    if (!pSh) { inst->RaiseErrorEvent("Share Helper not available", -1); return -1; }
    (void)shareSourceID; (void)screenIndex;
    return -2; // PinShare requires ShareSource struct — not directly supported
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UnpinShare(ZrcSdkHandle handle, int32_t shareSourceID, int32_t screenIndex)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingShareHelper* pSh = pMS->GetMeetingShareHelper();
    if (!pSh) { inst->RaiseErrorEvent("Share Helper not available", -1); return -1; }
    (void)shareSourceID; (void)screenIndex;
    return -2; // UnpinShare requires ShareSource struct — not directly supported
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LaunchSharingMeeting(ZrcSdkHandle handle, int32_t isInLocalShare, int32_t displayState)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingShareHelper* pSh = pMS->GetMeetingShareHelper();
    if (!pSh) { inst->RaiseErrorEvent("Share Helper not available", -1); return -1; }
    return (int)pSh->LaunchSharingMeeting(isInLocalShare != 0, (SharingInstructionDisplayState)displayState);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SwitchFromLocalPresentationToNormalMeeting(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingShareHelper* pSh = pMS->GetMeetingShareHelper();
    if (!pSh) { inst->RaiseErrorEvent("Share Helper not available", -1); return -1; }
    return (int)pSh->SwitchFromLocalPresentationToNormalMeeting();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ShowSharingInstruction(ZrcSdkHandle handle, int32_t show, int32_t instructionState)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingShareHelper* pSh = pMS->GetMeetingShareHelper();
    if (!pSh) { inst->RaiseErrorEvent("Share Helper not available", -1); return -1; }
    return (int)pSh->ShowSharingInstruction(show != 0, (SharingInstructionDisplayState)instructionState);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ShareBlackMagic(ZrcSdkHandle handle, int32_t isStart, int32_t isViewLocally)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingShareHelper* pSh = pMS->GetMeetingShareHelper();
    if (!pSh) { inst->RaiseErrorEvent("Share Helper not available", -1); return -1; }
    return (int)pSh->ShareBlackMagic(isStart != 0, isViewLocally != 0);
}

// ─── Setting (output/speaker volume) Extensions ───────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetSpeakerVolume(ZrcSdkHandle handle, float* outVolume)
{
    if (!handle || !outVolume) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pSettingService) { inst->RaiseErrorEvent("Setting Service not available", -1); return -1; }
    float v = 0.0f;
    int rc = (int)inst->pSettingService->GetSpeakerVolume(v);
    *outVolume = v;
    return rc;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetSpeakerVolume(ZrcSdkHandle handle, float volume)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pSettingService) { inst->RaiseErrorEvent("Setting Service not available", -1); return -1; }
    return (int)inst->pSettingService->SetSpeakerVolume(volume);
}

// ─── Layout Extensions ────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetScreenLayout(ZrcSdkHandle handle, int32_t screen, int32_t layoutSourceType)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingViewLayoutHelper* pL = pMS->GetMeetingViewLayoutHelper();
    if (!pL) { inst->RaiseErrorEvent("ViewLayout Helper not available", -1); return -1; }
    return (int)pL->SetScreenLayout((MeetingScreen)screen, (ScreenLayoutSourceType)layoutSourceType);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoOrder(ZrcSdkHandle handle, int32_t videoOrderType)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingViewLayoutHelper* pL = pMS->GetMeetingViewLayoutHelper();
    if (!pL) { inst->RaiseErrorEvent("ViewLayout Helper not available", -1); return -1; }
    return (int)pL->SelectVideoOrder((VideoOrderType)videoOrderType);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UpdateVideoLayoutStyle(ZrcSdkHandle handle, int32_t style)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingViewLayoutHelper* pL = pMS->GetMeetingViewLayoutHelper();
    if (!pL) { inst->RaiseErrorEvent("ViewLayout Helper not available", -1); return -1; }
    return (int)pL->UpdateVideoLayoutStyle((VideoLayoutStyle)style);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetFollowingHostOrder(ZrcSdkHandle handle, int follow)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingViewLayoutHelper* pL = pMS->GetMeetingViewLayoutHelper();
    if (!pL) { inst->RaiseErrorEvent("ViewLayout Helper not available", -1); return -1; }
    (void)follow;
    return -2; // SetFollowingHostOrder not available in this SDK version
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ControlVideoPosition(ZrcSdkHandle handle, int32_t position, int32_t size)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingViewLayoutHelper* pL = pMS->GetMeetingViewLayoutHelper();
    if (!pL) { inst->RaiseErrorEvent("ViewLayout Helper not available", -1); return -1; }
    return (int)pL->ControlVideoPosition((VideoThumbPosition)position, (VideoThumbSize)size);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_TurnVideoPage(ZrcSdkHandle handle, int32_t forward, int32_t pageVideoType)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingViewLayoutHelper* pL = pMS->GetMeetingViewLayoutHelper();
    if (!pL) { inst->RaiseErrorEvent("ViewLayout Helper not available", -1); return -1; }
    return (int)pL->TurnVideoPage(forward != 0, (PageVideoType)pageVideoType);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ChangeThumbnailsPosition(ZrcSdkHandle handle, int32_t type)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingViewLayoutHelper* pL = pMS->GetMeetingViewLayoutHelper();
    if (!pL) { inst->RaiseErrorEvent("ViewLayout Helper not available", -1); return -1; }
    return (int)pL->ChangeThumbnailsPosition((ThumbnailsPositionType)type);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SwitchToFloatingShareForSingleScreen(ZrcSdkHandle handle, int32_t floatingShare)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingViewLayoutHelper* pL = pMS->GetMeetingViewLayoutHelper();
    if (!pL) { inst->RaiseErrorEvent("ViewLayout Helper not available", -1); return -1; }
    return (int)pL->SwitchToFloatingShareForSingleScreen(floatingShare != 0);
}

// ─── Recording Extensions ────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AllowUserRecording(ZrcSdkHandle handle, int32_t userID, int allow)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS2);
    IRecordingHelper* pRec = pMS2->GetRecordingHelper();
    if (!pRec) { inst->RaiseErrorEvent("Recording Helper not available", -1); return -1; }
    return (int)pRec->AllowUserRecording(userID, allow != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ResponseToRecordingRequest(ZrcSdkHandle handle, int accept, int acceptAlways)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS2);
    IRecordingHelper* pRec = pMS2->GetRecordingHelper();
    if (!pRec) { inst->RaiseErrorEvent("Recording Helper not available", -1); return -1; }
    return (int)pRec->ResponseToRecordingRequest(accept != 0, acceptAlways != 0);
}

// ─── Reaction / Hand ─────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RaiseHand(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingReactionEmojiHelper* pR = pMS->GetMeetingReactionEmojiHelper();
    if (!pR) { inst->RaiseErrorEvent("Reaction Helper not available", -1); return -1; }
    return (int)pR->SendReactionFeedback(ReactionFeedbackHand);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LowerHand(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingReactionEmojiHelper* pR = pMS->GetMeetingReactionEmojiHelper();
    if (!pR) { inst->RaiseErrorEvent("Reaction Helper not available", -1); return -1; }
    return (int)pR->SendReactionFeedback(ReactionFeedbackNone);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LowerHandForUser(ZrcSdkHandle handle, int32_t userID)
{
    // The SDK lowers hand via participant helper — use SendReactionFeedback on behalf of host
    // The SDK's reaction emoji helper does not have LowerHandForUser, so return not-implemented
    (void)handle; (void)userID;
    return -2; // not implemented
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ExpelUser(ZrcSdkHandle handle, int32_t userID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IParticipantHelper* pPart = pMS->GetParticipantHelper();
    if (!pPart) { inst->RaiseErrorEvent("Participant Helper not available", -1); return -1; }
    return (int)pPart->ExpelUser(userID);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AssignHost(ZrcSdkHandle handle, int32_t userID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IParticipantHelper* pPart = pMS->GetParticipantHelper();
    if (!pPart) { inst->RaiseErrorEvent("Participant Helper not available", -1); return -1; }
    return (int)pPart->AssignHost(userID);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendReactionEmoji(ZrcSdkHandle handle, const char* emoji)
{
    if (!handle || !emoji) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingReactionEmojiHelper* pR = pMS->GetMeetingReactionEmojiHelper();
    if (!pR) { inst->RaiseErrorEvent("Reaction Helper not available", -1); return -1; }
    return (int)pR->SendEmoji(std::string(emoji));
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendReactionFeedback(ZrcSdkHandle handle, int32_t feedbackType)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingReactionEmojiHelper* pR = pMS->GetMeetingReactionEmojiHelper();
    if (!pR) { inst->RaiseErrorEvent("Reaction Helper not available", -1); return -1; }
    return (int)pR->SendReactionFeedback((ReactionFeedback)feedbackType);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ClearAllFeedback(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingReactionEmojiHelper* pR = pMS->GetMeetingReactionEmojiHelper();
    if (!pR) { inst->RaiseErrorEvent("Reaction Helper not available", -1); return -1; }
    return (int)pR->ClearAllFeedback();
}

// ─── Waiting Room ─────────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AdmitUserFromWaitingRoom(ZrcSdkHandle handle, int32_t userID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IWaitingRoomHelper* pWR = pMS->GetWaitingRoomHelper();
    if (!pWR) { inst->RaiseErrorEvent("WaitingRoom Helper not available", -1); return -1; }
    return (int)pWR->PutUsersIntoMeeting(std::vector<int32_t>{userID});
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AdmitAllFromWaitingRoom(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IWaitingRoomHelper* pWR = pMS->GetWaitingRoomHelper();
    if (!pWR) { inst->RaiseErrorEvent("WaitingRoom Helper not available", -1); return -1; }
    return (int)pWR->PutAllUsersIntoMeeting();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PutUserInWaitingRoom(ZrcSdkHandle handle, int32_t userID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IWaitingRoomHelper* pWR = pMS->GetWaitingRoomHelper();
    if (!pWR) { inst->RaiseErrorEvent("WaitingRoom Helper not available", -1); return -1; }
    return (int)pWR->PutUsersIntoWaitingRoom(std::vector<int32_t>{userID});
}

// ─── Breakout Rooms ───────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartBreakoutRooms(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IBreakoutRoomHelper* pBO = pMS->GetBreakoutRoomHelper();
    if (!pBO) { inst->RaiseErrorEvent("BreakoutRoom Helper not available", -1); return -1; }
    IBOAdminHelper* pAdmin = pBO->GetBOAdminHelper();
    if (!pAdmin) return -1;
    return (int)pAdmin->StartBreakoutRooms();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StopBreakoutRooms(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IBreakoutRoomHelper* pBO = pMS->GetBreakoutRoomHelper();
    if (!pBO) { inst->RaiseErrorEvent("BreakoutRoom Helper not available", -1); return -1; }
    IBOAdminHelper* pAdmin = pBO->GetBOAdminHelper();
    if (!pAdmin) return -1;
    return (int)pAdmin->StopBreakoutRooms();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_JoinBreakoutRoom(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IBreakoutRoomHelper* pBO = pMS->GetBreakoutRoomHelper();
    if (!pBO) { inst->RaiseErrorEvent("BreakoutRoom Helper not available", -1); return -1; }
    IBOAttendeeHelper* pAtt = pBO->GetBOAttendeeHelper();
    if (!pAtt) return -1;
    return (int)pAtt->JoinBreakoutRoom();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LeaveBreakoutRoom(ZrcSdkHandle handle, const char* boRoomBID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IBreakoutRoomHelper* pBO = pMS->GetBreakoutRoomHelper();
    if (!pBO) { inst->RaiseErrorEvent("BreakoutRoom Helper not available", -1); return -1; }
    IBOAssistantHelper* pAsst = pBO->GetBOAssistantHelper();
    if (!pAsst) return -1;
    return (int)pAsst->LeaveBreakoutRoom(boRoomBID ? std::string(boRoomBID) : std::string());
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_BroadcastMessageToBreakoutRooms(ZrcSdkHandle handle, const char* message)
{
    if (!handle || !message) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IBreakoutRoomHelper* pBO = pMS->GetBreakoutRoomHelper();
    if (!pBO) { inst->RaiseErrorEvent("BreakoutRoom Helper not available", -1); return -1; }
    IBOAdminHelper* pAdmin = pBO->GetBOAdminHelper();
    if (!pAdmin) return -1;
    return (int)pAdmin->BroadcastMessageToBreakoutRoom(std::string(message));
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AskForHelpInBreakoutRoom(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IBreakoutRoomHelper* pBO = pMS->GetBreakoutRoomHelper();
    if (!pBO) { inst->RaiseErrorEvent("BreakoutRoom Helper not available", -1); return -1; }
    IBOAttendeeHelper* pAtt = pBO->GetBOAttendeeHelper();
    if (!pAtt) return -1;
    return (int)pAtt->AskHostForHelpInBO();
}

// ─── Chat ─────────────────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendChatMessage(ZrcSdkHandle handle, const char* message, int32_t receiverUserID, int32_t sendToType)
{
    if (!handle || !message) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingChatHelper* pChat = pMS->GetMeetingChatHelper();
    if (!pChat) { inst->RaiseErrorEvent("Chat Helper not available", -1); return -1; }
    return (int)pChat->SendNewChatMessage("", receiverUserID, (MeetingChatMSGSendToType)sendToType, std::string(message));
}

// ─── Closed Caption ───────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ShowCaption(ZrcSdkHandle handle, int show)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IClosedCaptionHelper* pCC = pMS->GetClosedCaptionHelper();
    if (!pCC) { inst->RaiseErrorEvent("CC Helper not available", -1); return -1; }
    return (int)pCC->ShowCaption(show != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_EnableLiveTranscription(ZrcSdkHandle handle, int enable)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IClosedCaptionHelper* pCC = pMS->GetClosedCaptionHelper();
    if (!pCC) { inst->RaiseErrorEvent("CC Helper not available", -1); return -1; }
    return (int)pCC->EnableNewLTTManualCaptioner(enable != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AssignClosedCaption(ZrcSdkHandle handle, int32_t userID, int assign)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IClosedCaptionHelper* pCC = pMS->GetClosedCaptionHelper();
    if (!pCC) { inst->RaiseErrorEvent("CC Helper not available", -1); return -1; }
    return (int)pCC->AssignClosedCaption(userID, assign != 0);
}

// ─── Camera Control ───────────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ControlCamera(ZrcSdkHandle handle, const char* deviceID, int32_t action, int32_t type, const char* panTilt)
{
    if (!handle || !deviceID) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ICameraControlHelper* pCam = pMS->GetCameraControlHelper();
    if (!pCam) { inst->RaiseErrorEvent("Camera Helper not available", -1); return -1; }
    std::string panTiltStr = panTilt ? std::string(panTilt) : std::string();
    return (int)pCam->ControlLocalCamera(std::string(deviceID), (CameraControlAction)action, (CameraControlType)type, panTiltStr);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ControlUserCamera(ZrcSdkHandle handle, int32_t userID, int32_t action, int32_t type)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ICameraControlHelper* pCam = pMS->GetCameraControlHelper();
    if (!pCam) { inst->RaiseErrorEvent("Camera Helper not available", -1); return -1; }
    return (int)pCam->ControlUserCamera(userID, (CameraControlAction)action, (CameraControlType)type);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RespondRemoteCameraControl(ZrcSdkHandle handle, int32_t userID, int accept)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ICameraControlHelper* pCam = pMS->GetCameraControlHelper();
    if (!pCam) { inst->RaiseErrorEvent("Camera Helper not available", -1); return -1; }
    return (int)pCam->RespondRemoteCameraControlFromUser(userID, accept != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ChangeSmartCameraMode(ZrcSdkHandle handle, int32_t mask, const char* deviceID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ICameraControlHelper* pCam = pMS->GetCameraControlHelper();
    if (!pCam) { inst->RaiseErrorEvent("Camera Helper not available", -1); return -1; }
    // Empty deviceID targets the main (near-end) camera.
    return (int)pCam->ChangeSmartCameraMode((SmartCameraMask)mask, deviceID ? std::string(deviceID) : std::string());
}

// Camera presets. index range [0,1,2]; empty deviceID = main (near-end) camera.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetCameraPreset(ZrcSdkHandle handle, uint32_t index, const char* deviceID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ICameraControlHelper* pCam = pMS->GetCameraControlHelper();
    if (!pCam) { inst->RaiseErrorEvent("Camera Helper not available", -1); return -1; }
    return (int)pCam->SetCameraPreset(index, deviceID ? std::string(deviceID) : std::string());
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GoToCameraPreset(ZrcSdkHandle handle, uint32_t index, const char* deviceID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ICameraControlHelper* pCam = pMS->GetCameraControlHelper();
    if (!pCam) { inst->RaiseErrorEvent("Camera Helper not available", -1); return -1; }
    return (int)pCam->GoToCameraPreset(index, deviceID ? std::string(deviceID) : std::string());
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_NameCameraPreset(ZrcSdkHandle handle, uint32_t index, const char* name, const char* deviceID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    ICameraControlHelper* pCam = pMS->GetCameraControlHelper();
    if (!pCam) { inst->RaiseErrorEvent("Camera Helper not available", -1); return -1; }
    return (int)pCam->NameCameraPreset(index, name ? std::string(name) : std::string(), deviceID ? std::string(deviceID) : std::string());
}

// Helper: flatten an SDK Device to the C ZrcDevice struct.
static void FlattenDevice(const Device& src, ZrcDevice& dst)
{
    memset(&dst, 0, sizeof(ZrcDevice));
    strncpy_safe(dst.id,          src.id,          sizeof(dst.id));
    strncpy_safe(dst.name,        src.name,        sizeof(dst.name));
    strncpy_safe(dst.displayName, src.displayName, sizeof(dst.displayName));
    dst.isSelected = src.isSelected ? 1 : 0;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetCameraList(ZrcSdkHandle handle, ZrcDevice* outDevices, int maxCount)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pSettingService) { inst->RaiseErrorEvent("Setting Service not available", -1); return -1; }
    std::vector<Device> cameras;
    int rc = (int)inst->pSettingService->GetCameraList(cameras);
    if (rc != 0) return rc < 0 ? rc : -rc;  // surface SDK error as negative
    if (outDevices && maxCount > 0)
    {
        int n = (int)cameras.size();
        if (n > maxCount) n = maxCount;
        for (int i = 0; i < n; ++i) FlattenDevice(cameras[i], outDevices[i]);
    }
    return (int)cameras.size();
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetCurrentCamera(ZrcSdkHandle handle, ZrcDevice* outDevice)
{
    if (!handle || !outDevice) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pSettingService) { inst->RaiseErrorEvent("Setting Service not available", -1); return -1; }
    Device cam;
    int rc = (int)inst->pSettingService->GetCurrentCamera(cam);
    if (rc == 0) FlattenDevice(cam, *outDevice);
    return rc;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetCurrentCamera(ZrcSdkHandle handle, const char* deviceID)
{
    if (!handle || !deviceID) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pSettingService) { inst->RaiseErrorEvent("Setting Service not available", -1); return -1; }
    // SetCurrentCamera takes a full Device; resolve the matching entry from the camera list by ID.
    std::vector<Device> cameras;
    int listRc = (int)inst->pSettingService->GetCameraList(cameras);
    if (listRc != 0) { inst->RaiseErrorEvent("SetCurrentCamera: GetCameraList failed", listRc); return listRc < 0 ? listRc : -listRc; }
    std::string wanted(deviceID);
    for (const auto& cam : cameras)
    {
        if (cam.id == wanted)
            return (int)inst->pSettingService->SetCurrentCamera(cam);
    }
    inst->RaiseErrorEvent("SetCurrentCamera: device ID not found", -2);
    return -2;
}

// Accept (accept != 0) or decline an incoming meeting invite using the cached MeetingInvitationInfo
// from the last OnReceiveMeetingInviteNotification. Returns -2 if there is no pending invite.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AnswerMeetingInvite(ZrcSdkHandle handle, int accept)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    MeetingInvitationInfo invite;
    {
        std::lock_guard<std::mutex> lk(inst->meetingInviteMutex);
        if (!inst->hasMeetingInvite) { inst->RaiseErrorEvent("No pending meeting invite", -2); return -2; }
        invite = inst->lastMeetingInvite;
    }
    int rc = (int)pMS->AnswerMeetingInvite(invite, accept != 0);
    // Consume the invite only on success, so a failed answer can be retried. Guard against a new
    // invite arriving in the meantime by matching the meeting number before clearing.
    if (rc == 0)
    {
        std::lock_guard<std::mutex> lk(inst->meetingInviteMutex);
        if (inst->hasMeetingInvite && inst->lastMeetingInvite.meetingNumber == invite.meetingNumber)
            inst->hasMeetingInvite = false;
    }
    return rc;
}

// ─── Meeting Control Extensions ───────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LockMeeting(ZrcSdkHandle handle, int lock)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    IMeetingControlHelper* pCtrl = pMS->GetMeetingControlHelper();
    if (!pCtrl) { inst->RaiseErrorEvent("MeetingControl Helper not available", -1); return -1; }
    return (int)pCtrl->LockMeeting(lock != 0);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_EnableMeetingQA(ZrcSdkHandle handle, int enable)
{
    // Q&A enable is done via IMeetingQAHelper
    (void)handle; (void)enable;
    return -2; // not exposed directly
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetMeetingStatus(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    GET_MEETING_SERVICE(inst, pMS);
    MeetingStatus status;
    ZRCSDKError err = pMS->GetMeetingStatus(status);
    if (err != ZRCSDKERR_SUCCESS)
    {
        inst->RaiseErrorEvent("Failed to get meeting status", (int)err);
        return -1;
    }
    return (int)status;
}

// ─── Phone / SIP ─────────────────────────────────────────────────────────────

// Look up a cached SIPCallInfo by callID. If callID is null/empty and exactly one call is active,
// returns that one (the EPI tracks a single active call). Returns false if not found.
static bool FindSIPCall(ZrcSdkInstance* inst, const char* callID, SIPCallInfo& out)
{
    if (!inst) return false;
    std::lock_guard<std::mutex> lk(inst->sipCallsMutex);
    std::string id = callID ? callID : "";
    if (!id.empty())
    {
        auto it = inst->sipCalls.find(id);
        if (it == inst->sipCalls.end()) return false;
        out = it->second;
        return true;
    }
    if (inst->sipCalls.size() == 1) { out = inst->sipCalls.begin()->second; return true; }
    return false;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CallSIP(ZrcSdkHandle handle, const char* uri)
{
    if (!handle || !uri) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pPhoneCallService) { inst->RaiseErrorEvent("Phone Call Service not available", -1); return -1; }
    return (int)inst->pPhoneCallService->CallSIP(std::string(uri));
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_DeclineSIPCall(ZrcSdkHandle handle, const char* callID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pPhoneCallService) { inst->RaiseErrorEvent("Phone Call Service not available", -1); return -1; }
    SIPCallInfo info;
    if (!FindSIPCall(inst, callID, info)) return -2; // no matching active call
    return (int)inst->pPhoneCallService->DeclineIncomingSIPCall(info);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_TerminateSIPCall(ZrcSdkHandle handle, const char* callID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pPhoneCallService) { inst->RaiseErrorEvent("Phone Call Service not available", -1); return -1; }
    SIPCallInfo info;
    if (!FindSIPCall(inst, callID, info)) return -2;
    return (int)inst->pPhoneCallService->HangupSIPCall(info);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_HoldSIPCall(ZrcSdkHandle handle, const char* callID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pPhoneCallService) { inst->RaiseErrorEvent("Phone Call Service not available", -1); return -1; }
    SIPCallInfo info;
    if (!FindSIPCall(inst, callID, info)) return -2;
    return (int)inst->pPhoneCallService->HoldSIPCall(info);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UnholdSIPCall(ZrcSdkHandle handle, const char* callID)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pPhoneCallService) { inst->RaiseErrorEvent("Phone Call Service not available", -1); return -1; }
    SIPCallInfo info;
    if (!FindSIPCall(inst, callID, info)) return -2;
    return (int)inst->pPhoneCallService->UnholdSIPCall(info);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendDTMFToSIPCall(ZrcSdkHandle handle, const char* dtmf, const char* callID)
{
    if (!handle || !dtmf) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pPhoneCallService) { inst->RaiseErrorEvent("Phone Call Service not available", -1); return -1; }
    SIPCallInfo info;
    if (!FindSIPCall(inst, callID, info)) return -2;
    return (int)inst->pPhoneCallService->SendDTMFToSIPCall(std::string(dtmf), info);
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CallOutPSTNUser(ZrcSdkHandle handle, const char* phoneNumber, int cancelCall, int hasVoicePrompt)
{
    if (!handle || !phoneNumber) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pMeetingService) { inst->RaiseErrorEvent("Meeting Service not available", -1); return -1; }
    IThirdPartyMeetingHelper* pTPM = inst->pMeetingService->GetThirdPartyMeetingHelper();
    if (!pTPM) { inst->RaiseErrorEvent("Third Party Meeting Helper not available", -1); return -1; }
    return (int)pTPM->CallOutPSTNUser(std::string(phoneNumber), cancelCall != 0, hasVoicePrompt != 0);
}

// ─── Contacts / Directory ─────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SubscribeContacts(ZrcSdkHandle handle, int startIndex, int count, int isSearchSip)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pContactHelper) { inst->RaiseErrorEvent("Contact Helper not available", -1); return -1; }
    if (startIndex < 0 || count < 0) { inst->RaiseErrorEvent("SubscribeContacts: startIndex/count must be non-negative", -1); return -1; }
    return (int)inst->pContactHelper->Subscribe((uint32_t)startIndex, (uint32_t)count, isSearchSip != 0);
}

// ─── Invite by contact ID ─────────────────────────────────────────────────────

// Marshal a C string array into a std::vector<std::string>; null/empty entries are skipped.
static std::vector<std::string> ToStringVector(const char** items, int count)
{
    std::vector<std::string> vec;
    if (items && count > 0)
    {
        vec.reserve((size_t)count);
        // Skip null/empty contact IDs — the SDK expects valid strings, and empties would
        // produce invalid invite targets.
        for (int i = 0; i < count; ++i)
            if (items[i] && items[i][0] != '\0') vec.emplace_back(items[i]);
    }
    return vec;
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_InviteAttendees(ZrcSdkHandle handle, const char** contactIDs, int count)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pMeetingService) { inst->RaiseErrorEvent("Meeting Service not available", -1); return -1; }
    return (int)inst->pMeetingService->InviteAttendees(ToStringVector(contactIDs, count));
}

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_MeetWithIMUsers(ZrcSdkHandle handle, const char** contactIDs, int count)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pMeetingService) { inst->RaiseErrorEvent("Meeting Service not available", -1); return -1; }
    return (int)inst->pMeetingService->MeetWithIMUsers(ToStringVector(contactIDs, count));
}

// ─── Bookings / Schedule ──────────────────────────────────────────────────────

ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ListMeeting(ZrcSdkHandle handle)
{
    if (!handle) return -1;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    if (!inst->bInitialized) { inst->RaiseErrorEvent("SDK not initialized", -1); return -1; }
    if (!inst->pMeetingListHelper) { inst->RaiseErrorEvent("Meeting List Helper not available", -1); return -1; }
    return (int)inst->pMeetingListHelper->ListMeeting();
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
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingInviteCallback(ZrcSdkHandle handle, ZrcMeetingInviteCallback callback, void* userData)
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
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingRecordingInfoCallback(ZrcSdkHandle handle, ZrcMeetingRecordingInfoCallback callback, void* userData)
    { SET_CB(meetingRecordingInfo, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetCameraPresetInfoCallback(ZrcSdkHandle handle, ZrcCameraPresetInfoCallback callback, void* userData)
    { SET_CB(cameraPresetInfo, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetControlSystemEnabledCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(controlSystemEnabled, callback, userData); }

ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetParticipantListCallback(ZrcSdkHandle handle, ZrcParticipantListCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->participantListCallback = callback;
    inst->participantListUserData = userData;
}

ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetContactListCallback(ZrcSdkHandle handle, ZrcContactListCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->contactListCallback = callback;
    inst->contactListUserData = userData;
}

ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingListCallback(ZrcSdkHandle handle, ZrcMeetingListCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->meetingListCallback = callback;
    inst->meetingListUserData = userData;
}

// ─── Extended Event Callback Setters ─────────────────────────────────────────

ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAllowAttendeesUnmuteCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(allowAttendeesUnmute, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAskUnmuteByHostCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(askUnmuteByHost, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetFEACRequestCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(feacRequest, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetFEACApprovedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(feacApproved, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetFEACDeclinedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(feacDeclined, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAllowAttendeesVideoCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(allowAttendeesVideo, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetSharingStatusCallback(ZrcSdkHandle handle, ZrcSharingStatusCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->sharingStatusCallback = callback;
    inst->sharingStatusUserData = userData;
}
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAirPlayStatusCallback(ZrcSdkHandle handle, ZrcAirPlayStatusCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->airPlayStatusCallback = callback;
    inst->airPlayStatusUserData = userData;
}
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoPageStatusCallback(ZrcSdkHandle handle, ZrcVideoPageStatusCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->videoPageStatusCallback = callback;
    inst->videoPageStatusUserData = userData;
}
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetScreenLayoutStatusCallback(ZrcSdkHandle handle, ZrcScreenLayoutStatusCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->screenLayoutStatusCallback = callback;
    inst->screenLayoutStatusUserData = userData;
}
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetBOStatusChangedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(boStatusChanged, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetBORoomListCallback(ZrcSdkHandle handle, ZrcBORoomListCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->boRoomListCallback = callback;
    inst->boRoomListUserData = userData;
}
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetInSilentModeCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(inSilentMode, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetReactionStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(reactionStatus, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetChatPrivilegeCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(chatPrivilege, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetChatMessageCallback(ZrcSdkHandle handle, ZrcChatMessageCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->chatMessageCallback = callback;
    inst->chatMessageUserData = userData;
}
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetCaptionCallback(ZrcSdkHandle handle, ZrcCaptionCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->captionCallback = callback;
    inst->captionUserData = userData;
}
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingLockStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(meetingLockStatus, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetQAEnabledCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(qaEnabled, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetRecordingRequestCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(recordingRequest, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetFarEndCameraControlRequestCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(farEndCameraControl, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetSIPCallStatusCallback(ZrcSdkHandle handle, ZrcSIPCallCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->sipCallStatusCallback = callback;
    inst->sipCallStatusUserData = userData;
}
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetSIPServiceStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
    { SET_CB(sipServiceStatus, callback, userData); }
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetZRCSDeviceListCallback(ZrcSdkHandle handle, ZrcZRCSDeviceListCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->zrcsDeviceListCallback = callback;
    inst->zrcsDeviceListUserData = userData;
}
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetZRCSSceneListCallback(ZrcSdkHandle handle, ZrcZRCSSceneListCallback callback, void* userData)
{
    if (!handle) return;
    ZrcSdkInstance* inst = (ZrcSdkInstance*)handle;
    inst->zrcsSceneListCallback = callback;
    inst->zrcsSceneListUserData = userData;
}

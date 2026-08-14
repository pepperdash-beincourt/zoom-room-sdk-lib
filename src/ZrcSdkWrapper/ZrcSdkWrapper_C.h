#pragma once

#include <stdint.h>

#ifdef _WIN32
    #ifdef ZRCSDKWRAPPER_EXPORTS
        #define ZRCSDKWRAPPER_API __declspec(dllexport)
    #else
        #define ZRCSDKWRAPPER_API __declspec(dllimport)
    #endif
    #define ZRCSDKWRAPPER_CALL __cdecl
#else
    #define ZRCSDKWRAPPER_API __attribute__((visibility("default")))
    #define ZRCSDKWRAPPER_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Opaque handle type for SDK instance
typedef void* ZrcSdkHandle;

// Callback type: message + errorCode + caller-supplied userData
typedef void (ZRCSDKWRAPPER_CALL *SdkEventCallback)(const char* message, int errorCode, void* userData);

// ── Participant flat struct ────────────────────────────────────────────────────
// Flattened representation of MeetingParticipant + nested status structs.
// All strings are null-terminated; all booleans are int32_t (0=false, 1=true).
typedef struct ZrcParticipant {
    int32_t userID;
    int32_t parentUserID;
    char    userGUID[256];
    int32_t userType;               // UserType enum
    char    userName[256];
    char    pronouns[128];
    int32_t isMySelf;
    int32_t isHost;
    int32_t isOriginalOrAlternativeHost;
    int32_t isCohost;
    int32_t isGuest;
    int32_t isViewOnlyUser;
    int32_t isViewOnlyUserCanTalk;
    int32_t canRecord;
    int32_t isRecording;
    int32_t recordingDisabled;
    int32_t isInSilentMode;
    // AudioStatus
    int32_t audioType;              // AudioType enum
    int32_t audioMuted;
    // VideoStatus
    int32_t videoHasSource;
    int32_t videoReceiving;
    int32_t videoSending;
    int32_t videoCanControl;
    // CameraControlStatus
    int32_t cameraCanRequestControl;
    int32_t cameraAmIControlling;
    int32_t cameraCanSwitch;
    int32_t cameraCanMove;
    int32_t cameraCanZoom;
    // HandStatus
    int32_t handRaised;
    int32_t handSkinTone;           // HandSkinTone enum
    char    reactionEmoji[32];
    int32_t reactionFeedback;       // ReactionFeedback enum
    // Interpretation
    int32_t isInterpreter;
    int32_t interpreterLanguage;    // InterpretLanguage enum
    char    interpreterLanguageID[64];
    char    interpreterLanguageName[64];
    // Miscellaneous
    int32_t isRemoteControlAdmin;
    int32_t isVirtualAssistant;
    int32_t isCompanionModeUser;
    int32_t isCompanionZRUser;
    // BreakoutRoomStatus
    char    boSessionBID[128];
    int32_t boUserStatus;           // BO_USER_STATUS enum
    int32_t boSupportForceJoinLeave;
    int32_t boSupportSelfChoose;
    int32_t boSupportCohostStartStop;
    // Streaming / webinar flags
    int32_t canPinMultiVideo;
    int32_t isSupportGreenRoom;
    int32_t isInGreenRoom;
    // AudioControlStatus
    int32_t audioCanRequestControl;
    int32_t audioCanBeRequested;
    int32_t audioAmIControlling;
    // RTMP / special user flags
    int32_t isRTMPUser;
    int32_t isActiveRTMPUser;
    int32_t isSimuliveUser;
    // Timezone
    int32_t timeZoneOffsetMinutes;
    int32_t isSupportDisplayLocalTime;
    char    attendeeJid[256];
} ZrcParticipant;

// Callback type for participant list updates.
// participants: array of count ZrcParticipant values (caller-owned, valid only during callback)
// eventType:    0 = join, 1 = initialize (full replace), 2 = leave, 3 = update
// sessionType:  ConfSessionType enum value
typedef void (ZRCSDKWRAPPER_CALL *ZrcParticipantListCallback)(
    const ZrcParticipant* participants,
    int count,
    int eventType,
    int sessionType,
    void* userData);

// ── Contact flat struct ────────────────────────────────────────────────────────
// Flattened representation of the SDK Contact struct (directory/phonebook entry).
// All strings are null-terminated; presenceStatus/buddyType are the raw SDK enum ints.
typedef struct ZrcContact {
    char    contactID[256];
    char    screenName[256];
    char    firstName[128];
    char    lastName[128];
    char    email[256];
    char    phoneNumber[128];
    char    sipPhoneNumber[128];
    int32_t presenceStatus;         // IMPresenceStatus enum
    int32_t buddyType;              // IMBuddyType enum
} ZrcContact;

// Callback type for contact list updates.
// contacts:   array of count ZrcContact values (caller-owned, valid only during callback)
// sourceType: 0 = ambient IM/presence update (native OnImUpdateContactNotification) — a contact's
//                 status changed on its own, NOT a response to ZrcSdk_SubscribeContacts.
//             1 = dynamic contact list notification (native OnDynamicContactListNotification) —
//                 the actual paged response to ZrcSdk_SubscribeContacts. Only batches with this
//                 sourceType should be used to drive phonebook paging/completion logic.
typedef void (ZRCSDKWRAPPER_CALL *ZrcContactListCallback)(
    const ZrcContact* contacts,
    int count,
    int sourceType,
    void* userData);

// ── Incoming meeting-invite flat struct ────────────────────────────────────────
// Flattened MeetingInvitationInfo for an inbound invite. meetingNumber is stringified (int64).
typedef struct ZrcMeetingInvite {
    char callerName[256];
    char callerContactID[256];
    char meetingID[128];
    char meetingNumber[32];
} ZrcMeetingInvite;

typedef void (ZRCSDKWRAPPER_CALL *ZrcMeetingInviteCallback)(const ZrcMeetingInvite* invite, void* userData);

// Fired when a pending invite is resolved (answered here, answered/expired/cancelled elsewhere).
// accepted: 1 = accepted, 0 = declined/expired/cancelled.
typedef void (ZRCSDKWRAPPER_CALL *ZrcMeetingInviteTreatedCallback)(const ZrcMeetingInvite* invite, int32_t accepted, void* userData);

// ── Meeting (booking/schedule) flat struct ─────────────────────────────────────
// Flattened representation of the SDK MeetingItem struct (calendar/schedule entry).
// All strings are null-terminated; startTime/endTime are ISO-8601 (e.g. "2017-03-15T11:30:00-07:00").
typedef struct ZrcMeetingItem {
    char    meetingNumber[128];
    char    meetingName[256];
    char    hostName[256];
    char    startTime[64];
    char    endTime[64];
    char    meetingDomain[256];
    int32_t scheduledFrom;          // MeetingScheduleFrom enum (-1 if N/A)
    int32_t isPrivate;
    int32_t isAllDayEvent;
    int32_t isCheckedIn;
    int32_t isInstantMeeting;
} ZrcMeetingItem;

// Callback type for meeting (schedule) list updates.
// result:   ListMeetingResult enum value (0 = success)
// meetings: array of count ZrcMeetingItem values (caller-owned, valid only during callback)
typedef void (ZRCSDKWRAPPER_CALL *ZrcMeetingListCallback)(
    int result,
    const ZrcMeetingItem* meetings,
    int count,
    void* userData);

// ── Sharing status flat struct ────────────────────────────────────────────────
typedef struct ZrcSharingStatus {
    int32_t sharingState;           // SharingState enum
    int32_t canShareToBO;
    int32_t isSharingToBO;
} ZrcSharingStatus;

typedef void (ZRCSDKWRAPPER_CALL *ZrcSharingStatusCallback)(const ZrcSharingStatus* status, void* userData);

// ── AirPlay / Black Magic status flat struct ──────────────────────────────────
typedef struct ZrcAirPlayStatus {
    int32_t instructionDisplayState;               // SharingInstructionDisplayState enum
    char    wifiName[128];                         // Airplay server WiFi name (SSID)
    char    serverName[256];                       // Airplay server name
    char    password[64];                          // Airplay server password / code
    char    directPresentationPairingCode[64];     // Direct presentation pairing code
    char    directPresentationSharingKey[128];     // Laptop share key
    int32_t isAirHostClientConnected;              // 1 = AirHost client connected
    int32_t isBlackMagicConnected;                 // 1 = HDMI cable connected
    int32_t isBlackMagicDataAvailable;             // 1 = HDMI data ready to share
    int32_t isSharingBlackMagic;                   // 1 = HDMI currently sharing
    int32_t isDirectPresentationConnected;         // 1 = direct presentation connected
} ZrcAirPlayStatus;

typedef void (ZRCSDKWRAPPER_CALL *ZrcAirPlayStatusCallback)(const ZrcAirPlayStatus* status, void* userData);

// ── Video page status flat struct ─────────────────────────────────────────────
typedef struct ZrcVideoPageStatus {
    int32_t isInFirstPage;            // 1 = on the first page
    int32_t isInLastPage;             // 1 = on the last page
    int32_t pageVideoType;            // PageVideoType enum
    int32_t videoCountInCurrentPage;
} ZrcVideoPageStatus;

typedef void (ZRCSDKWRAPPER_CALL *ZrcVideoPageStatusCallback)(const ZrcVideoPageStatus* status, void* userData);

// ── Video thumb (self-view PiP) info flat struct ───────────────────────────────────────
typedef struct ZrcVideoThumbInfo {
    int32_t isSupported;               // 1 = self-view thumb is supported in the current context
    int32_t position;                  // VideoThumbPosition enum
    int32_t size;                      // VideoThumbSize enum
    int32_t isInFirstPage;             // nested VideoPageStatus fields
    int32_t isInLastPage;
    int32_t pageVideoType;
    int32_t videoCountInCurrentPage;
    int32_t isThumbnailOnTop;
} ZrcVideoThumbInfo;

typedef void (ZRCSDKWRAPPER_CALL *ZrcVideoThumbInfoCallback)(const ZrcVideoThumbInfo* info, void* userData);

// ── Screen layout status flat structs ─────────────────────────────────────────
#define ZRC_MAX_SCREEN_LAYOUT_SCREENS 4
#define ZRC_MAX_SCREEN_LAYOUT_CTRLS   16

typedef struct ZrcScreenLayoutCtrlInfo {
    int32_t layout;   // ScreenLayoutSourceType enum
    int32_t enable;   // 1 = enabled
    int32_t visible;  // 1 = visible
} ZrcScreenLayoutCtrlInfo;

typedef struct ZrcScreenLayoutInfo {
    int32_t screen;         // MeetingScreen enum
    int32_t layout;         // ScreenLayoutSourceType enum (current)
    int32_t ctrlInfoCount;  // number of valid entries in ctrlInfos
    ZrcScreenLayoutCtrlInfo ctrlInfos[ZRC_MAX_SCREEN_LAYOUT_CTRLS];
} ZrcScreenLayoutInfo;

typedef struct ZrcScreenLayoutStatus {
    int32_t canShowContentOnly;
    int32_t isInContentOnly;
    int32_t canAdjustFloatingVideo;
    int32_t canSwitchFloatingShareContent;
    int32_t isInFloatingShareContent;
    int32_t canAdjustMyAutoGeneratedVideoStreamsVisibility;
    int32_t isShowMyAutoGeneratedVideoStreams;
    int32_t screenCount;    // number of valid entries in screens
    ZrcScreenLayoutInfo screens[ZRC_MAX_SCREEN_LAYOUT_SCREENS];
} ZrcScreenLayoutStatus;

typedef void (ZRCSDKWRAPPER_CALL *ZrcScreenLayoutStatusCallback)(const ZrcScreenLayoutStatus* status, void* userData);

// ── Meeting recording info flat struct ────────────────────────────────────────
typedef struct ZrcMeetingRecordingInfo {
    int32_t isMeetingBeingRecorded;   // 1 = meeting is being recorded
    int32_t canIRecord;               // 1 = this room can start recording
    int32_t amIRecording;             // 1 = this room is recording
} ZrcMeetingRecordingInfo;

typedef void (ZRCSDKWRAPPER_CALL *ZrcMeetingRecordingInfoCallback)(const ZrcMeetingRecordingInfo* info, void* userData);

// ── Camera preset flat struct (max 3 slots; ZRC SDK preset index range is [0,1,2]) ────
typedef struct ZrcCameraPreset {
    int32_t index;        // preset slot index (0..2)
    char    name[128];    // preset name (empty if unnamed)
} ZrcCameraPreset;

typedef struct ZrcCameraPresetInfo {
    int32_t         defaultIndex;          // default preset index (-1 if none)
    int32_t         supportedPresetCount;  // slots the current camera supports (<=3)
    int32_t         presetCount;           // number of valid entries in presets[]
    ZrcCameraPreset presets[3];            // saved presets
} ZrcCameraPresetInfo;

typedef void (ZRCSDKWRAPPER_CALL *ZrcCameraPresetInfoCallback)(const ZrcCameraPresetInfo* info, void* userData);

// ── Chat message flat struct ──────────────────────────────────────────────────
typedef struct ZrcChatMessage {
    char    messageID[128];
    int32_t senderMeetingUserID;
    char    senderName[256];
    int32_t receiverMeetingUserID;  // 0 = all
    int32_t sendToType;             // MeetingChatMSGSendToType enum
    char    content[2048];
    int64_t messageTime;            // milliseconds
    int32_t isDeleted;
} ZrcChatMessage;

typedef void (ZRCSDKWRAPPER_CALL *ZrcChatMessageCallback)(const ZrcChatMessage* msg, void* userData);

// ── Caption flat struct ───────────────────────────────────────────────────────
typedef struct ZrcCaption {
    char    messageID[128];
    int32_t userID;
    char    userName[256];
    int64_t messageTime;
    char    content[2048];
    int32_t isFinal;                // 1 = not expected to change further
} ZrcCaption;

typedef void (ZRCSDKWRAPPER_CALL *ZrcCaptionCallback)(const ZrcCaption* caption, void* userData);

// ── SIP call flat struct ──────────────────────────────────────────────────────
typedef struct ZrcSIPCall {
    char    callID[128];
    int32_t status;                 // SIPCallStatus enum
    char    peerDisplayName[256];
    char    peerNumber[128];
    int32_t isIncomingCall;
    int64_t elapsedCallTime;
} ZrcSIPCall;

typedef void (ZRCSDKWRAPPER_CALL *ZrcSIPCallCallback)(const ZrcSIPCall* call, void* userData);

// ── BreakoutRoom flat struct ──────────────────────────────────────────────────
typedef struct ZrcBORoom {
    char sessionBID[128];
    char sessionName[256];
} ZrcBORoom;

typedef void (ZRCSDKWRAPPER_CALL *ZrcBORoomListCallback)(const ZrcBORoom* rooms, int count, void* userData);

// ── ZRCS device/scene flat structs ───────────────────────────────────────────
typedef struct ZrcZRCSDevice {
    char    deviceID[256];
    char    deviceName[256];
    int32_t deviceStatus;           // ControlSystemDeviceStatus enum
    int32_t updateType;             // ControlSystemUpdateDeviceType enum
} ZrcZRCSDevice;

typedef struct ZrcZRCSScene {
    char sceneID[256];
    char sceneName[256];
    char sceneIcon[256];
} ZrcZRCSScene;

typedef void (ZRCSDKWRAPPER_CALL *ZrcZRCSDeviceListCallback)(const ZrcZRCSDevice* devices, int count, int updateType, void* userData);
typedef void (ZRCSDKWRAPPER_CALL *ZrcZRCSSceneListCallback)(const ZrcZRCSScene* scenes, int count, void* userData);

// ── SDK Lifecycle ─────────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API ZrcSdkHandle ZRCSDKWRAPPER_CALL ZrcSdk_Create();
ZRCSDKWRAPPER_API void         ZRCSDKWRAPPER_CALL ZrcSdk_Destroy(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int          ZRCSDKWRAPPER_CALL ZrcSdk_Initialize(ZrcSdkHandle handle, const char* configPath);
ZRCSDKWRAPPER_API void         ZRCSDKWRAPPER_CALL ZrcSdk_Uninitialize(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API void         ZRCSDKWRAPPER_CALL ZrcSdk_HeartBeat(ZrcSdkHandle handle);

// ── SDK Information ───────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetSDKVersion(ZrcSdkHandle handle, char* buffer, int bufferSize);

// ── Room Pairing ──────────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PairRoomWithActivationCode(ZrcSdkHandle handle, const char* activationCode);
// Returns 1 if stored credentials exist and RetryToPairRoom can be called, 0 otherwise
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CanRetryToPairLastRoom(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RetryToPairRoom(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UnpairRoom(ZrcSdkHandle handle);

// ── Pre-Meeting / Connection ──────────────────────────────────────────────────
// Returns current ConnectionState enum value (0=Established, 1=Connected, 2=Disconnected, -1=None)
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetConnectionState(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_WakeZoomRoomUp(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LogoutZoomRoomDevice(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RestartZoomRoomOS(ZrcSdkHandle handle);

// ── Meeting Control ───────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartMeeting(ZrcSdkHandle handle, const char* meetingNumber);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartInstantMeeting(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartMeetingWithHostKey(ZrcSdkHandle handle, const char* hostKey);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_JoinMeeting(ZrcSdkHandle handle, const char* meetingNumber);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_JoinMeetingWithURL(ZrcSdkHandle handle, const char* url);
// Leave meeting (self only); for host "end for all" use ZrcSdk_EndMeeting
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LeaveMeeting(ZrcSdkHandle handle);
// End meeting for all participants (host only)
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_EndMeeting(ZrcSdkHandle handle);
// Accept (accept!=0) or decline an incoming meeting invite using the last cached invite. Returns -2 if none pending.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AnswerMeetingInvite(ZrcSdkHandle handle, int accept);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendMeetingPassword(ZrcSdkHandle handle, const char* password);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CancelEnteringMeetingPassword(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CancelWaitingForHost(ZrcSdkHandle handle);

// ── Audio / Video ─────────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetAudioMute(ZrcSdkHandle handle, int mute);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoState(ZrcSdkHandle handle, int start);// Hide/show the ZR's own self video locally (does not stop video to the far end).
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetMyVideoHidden(ZrcSdkHandle handle, int hidden);
// ── Cloud Recording ───────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartRecording(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StopRecording(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PauseRecording(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ResumeRecording(ZrcSdkHandle handle);

// ── Participants ──────────────────────────────────────────────────────────────
// Returns last-known participant count (updated by callbacks)
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetParticipantCount(ZrcSdkHandle handle);

// ── Event Callback Setters ────────────────────────────────────────────────────
// Participant list (Init / Join / Leave / Update)
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetParticipantListCallback(ZrcSdkHandle handle, ZrcParticipantListCallback callback, void* userData);
// Contact list (directory / phonebook subscription results)
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetContactListCallback(ZrcSdkHandle handle, ZrcContactListCallback callback, void* userData);
// Meeting (booking/schedule) list
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingListCallback(ZrcSdkHandle handle, ZrcMeetingListCallback callback, void* userData);

// ── Audio extensions ──────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_MuteUserAudio(ZrcSdkHandle handle, int32_t userID, int mute);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_MuteAllAudio(ZrcSdkHandle handle, int mute);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetMuteOnEntry(ZrcSdkHandle handle, int mute);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AnswerUnmuteRequest(ZrcSdkHandle handle, int accepted);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AllowAttendeesUnmute(ZrcSdkHandle handle, int allow);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RequestFarEndAudioControl(ZrcSdkHandle handle, int32_t farEndUserID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GiveUpFarEndAudioControl(ZrcSdkHandle handle, int32_t farEndUserID);

// ── Video extensions ──────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_MuteUserVideo(ZrcSdkHandle handle, int32_t userID, int mute);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AllowAttendeesStartVideo(ZrcSdkHandle handle, int allow);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PinUserOnScreen(ZrcSdkHandle handle, int32_t userID, int32_t screenIndex);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UnpinUserFromScreen(ZrcSdkHandle handle, int32_t userID, int32_t screenIndex);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SpotlightUser(ZrcSdkHandle handle, int32_t userID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CancelSpotlightUser(ZrcSdkHandle handle, int32_t userID);

// ── Share extensions ──────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartShare(ZrcSdkHandle handle, const char* shareSourceID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StopShare(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PinShare(ZrcSdkHandle handle, int32_t shareSourceID, int32_t screenIndex);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UnpinShare(ZrcSdkHandle handle, int32_t shareSourceID, int32_t screenIndex);
// Sharing-only ("local presentation") meeting. isInLocalShare: 1 = local presentation, 0 = sharing meeting. displayState: SharingInstructionDisplayState.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LaunchSharingMeeting(ZrcSdkHandle handle, int32_t isInLocalShare, int32_t displayState);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SwitchFromLocalPresentationToNormalMeeting(ZrcSdkHandle handle);
// show: 1 = show instruction, 0 = hide. instructionState: SharingInstructionDisplayState (Desktop/IOS/WhiteboardCamera).
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ShowSharingInstruction(ZrcSdkHandle handle, int32_t show, int32_t instructionState);
// HDMI ("black magic") cable share. isStart: 1 = start, 0 = stop. isViewLocally: 1 = also show locally.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ShareBlackMagic(ZrcSdkHandle handle, int32_t isStart, int32_t isViewLocally);

// ── Setting (output/speaker volume) extensions ────────────────────────────────
// Room speaker (audio output) volume. Float value as defined by the SDK (host maps to its own scale).
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetSpeakerVolume(ZrcSdkHandle handle, float* outVolume);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetSpeakerVolume(ZrcSdkHandle handle, float volume);

// ── Layout extensions ─────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetScreenLayout(ZrcSdkHandle handle, int32_t screen, int32_t layoutSourceType);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoOrder(ZrcSdkHandle handle, int32_t videoOrderType);
// style: VideoLayoutStyle enum (Gallery=1, Speaker=2, Thumbnail=3, ContentOnly=4, DynamicLayout=6)
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UpdateVideoLayoutStyle(ZrcSdkHandle handle, int32_t style);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetFollowingHostOrder(ZrcSdkHandle handle, int follow);
// Self-view PiP. position: VideoThumbPosition; size: VideoThumbSize (Off=0 hides the PiP / 1x/2x/3x/Stripe).
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ControlVideoPosition(ZrcSdkHandle handle, int32_t position, int32_t size);
// forward: 1 = next page, 0 = previous. pageVideoType: PageVideoType (GalleryView=0/ThumbnailView=1/DynamicLayoutView=2).
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_TurnVideoPage(ZrcSdkHandle handle, int32_t forward, int32_t pageVideoType);
// type: ThumbnailsPositionType.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ChangeThumbnailsPosition(ZrcSdkHandle handle, int32_t type);
// Single-screen "swap content with thumbnail": floatingShare 1 = float the share (show video full), 0 = full share.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SwitchToFloatingShareForSingleScreen(ZrcSdkHandle handle, int32_t floatingShare);

// ── Recording extensions ──────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AllowUserRecording(ZrcSdkHandle handle, int32_t userID, int allow);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ResponseToRecordingRequest(ZrcSdkHandle handle, int accept, int acceptAlways);

// ── Reaction / Hand ───────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RaiseHand(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LowerHand(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LowerHandForUser(ZrcSdkHandle handle, int32_t userID);
// Host actions on a participant (IParticipantHelper).
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ExpelUser(ZrcSdkHandle handle, int32_t userID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AssignHost(ZrcSdkHandle handle, int32_t userID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendReactionEmoji(ZrcSdkHandle handle, const char* emoji);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendReactionFeedback(ZrcSdkHandle handle, int32_t feedbackType);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ClearAllFeedback(ZrcSdkHandle handle);

// ── Waiting Room ──────────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AdmitUserFromWaitingRoom(ZrcSdkHandle handle, int32_t userID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AdmitAllFromWaitingRoom(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PutUserInWaitingRoom(ZrcSdkHandle handle, int32_t userID);

// ── Breakout Rooms ────────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartBreakoutRooms(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StopBreakoutRooms(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_JoinBreakoutRoom(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LeaveBreakoutRoom(ZrcSdkHandle handle, const char* boRoomBID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_BroadcastMessageToBreakoutRooms(ZrcSdkHandle handle, const char* message);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AskForHelpInBreakoutRoom(ZrcSdkHandle handle);

// ── Chat ──────────────────────────────────────────────────────────────────────
// Send chat message to all (receiverUserID=0) or to individual
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendChatMessage(ZrcSdkHandle handle, const char* message, int32_t receiverUserID, int32_t sendToType);

// ── Closed Caption / LTT ──────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ShowCaption(ZrcSdkHandle handle, int show);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_EnableLiveTranscription(ZrcSdkHandle handle, int enable);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_AssignClosedCaption(ZrcSdkHandle handle, int32_t userID, int assign);

// ── Device flat struct ──────────────────────────────────────────────────────────
// Flattened representation of the SDK Device struct (camera/microphone/speaker entry).
// Strings are null-terminated; isSelected is 1 when this is the active device.
typedef struct ZrcDevice {
    char    id[256];                // Device ID
    char    name[256];              // Device name
    char    displayName[256];       // Display name for UI
    int32_t isSelected;             // 1 = currently selected/active
} ZrcDevice;

// ── Camera control ────────────────────────────────────────────────────────────
// action: CameraControlAction enum, type: CameraControlType enum
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ControlCamera(ZrcSdkHandle handle, const char* deviceID, int32_t action, int32_t type, const char* panTilt);
// Far-end (participant) camera control. userID: target participant; action: CameraControlAction; type: CameraControlType (Start/Continue/Stop)
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ControlUserCamera(ZrcSdkHandle handle, int32_t userID, int32_t action, int32_t type);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_RespondRemoteCameraControl(ZrcSdkHandle handle, int32_t userID, int accept);
// Smart/auto camera framing mode. mask: SmartCameraMask (Manual=1, SpeakerFocus=2, GroupFocus=4, MultiFocus=8, SmartGallery=16, Director=32, PresenterFocus=64). deviceID empty = main camera.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ChangeSmartCameraMode(ZrcSdkHandle handle, int32_t mask, const char* deviceID);
// Camera presets. index range [0,1,2]. deviceID empty = main (near-end) camera. All return <0 on error.
// Save current position to slot:
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetCameraPreset(ZrcSdkHandle handle, uint32_t index, const char* deviceID);
// Recall slot:
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GoToCameraPreset(ZrcSdkHandle handle, uint32_t index, const char* deviceID);
// Name a slot:
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_NameCameraPreset(ZrcSdkHandle handle, uint32_t index, const char* name, const char* deviceID);
// Camera device list / selection (ISettingService). All return <0 on error.
// GetCameraList fills up to maxCount entries and returns the total camera count (may exceed maxCount).
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetCameraList(ZrcSdkHandle handle, ZrcDevice* outDevices, int maxCount);
// GetCurrentCamera fills outDevice with the active camera; returns 0 on success.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetCurrentCamera(ZrcSdkHandle handle, ZrcDevice* outDevice);
// SetCurrentCamera selects the active camera by device ID; returns 0 on success.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetCurrentCamera(ZrcSdkHandle handle, const char* deviceID);

// ── Phone Call ────────────────────────────────────────────────────────────────
// SIP call wrapping. Command methods that take a callID look up the cached SIPCallInfo; pass an
// empty/null callID to target the single active call. Return -2 if no matching call is cached.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CallSIP(ZrcSdkHandle handle, const char* uri);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_DeclineSIPCall(ZrcSdkHandle handle, const char* callID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_TerminateSIPCall(ZrcSdkHandle handle, const char* callID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_HoldSIPCall(ZrcSdkHandle handle, const char* callID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_UnholdSIPCall(ZrcSdkHandle handle, const char* callID);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendDTMFToSIPCall(ZrcSdkHandle handle, const char* dtmf, const char* callID);

// PSTN dial-out (adds a PSTN number to the current meeting via the third-party meeting helper).
// cancelCall != 0 cancels an in-progress call-out; hasVoicePrompt != 0 rings on the Zoom Room.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CallOutPSTNUser(ZrcSdkHandle handle, const char* phoneNumber, int cancelCall, int hasVoicePrompt);

// ── Contacts / Directory ──────────────────────────────────────────────────────
// Subscribe to a range of directory contacts; results arrive via the contact-list callback.
// startIndex/count page the directory (count ~50 suggested); isSearchSip != 0 searches SIP contacts.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SubscribeContacts(ZrcSdkHandle handle, int startIndex, int count, int isSearchSip);

// ── Invite by contact ID ──────────────────────────────────────────────────────
// InviteAttendees adds the given IM contacts to the CURRENT meeting; MeetWithIMUsers starts a NEW
// meeting with them. contactIDs is an array of count null-terminated UTF-8 contact-ID strings.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_InviteAttendees(ZrcSdkHandle handle, const char** contactIDs, int count);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_MeetWithIMUsers(ZrcSdkHandle handle, const char** contactIDs, int count);

// ── Bookings / Schedule ───────────────────────────────────────────────────────
// Requests the current list of scheduled meetings (calendar bookings); results arrive via the
// meeting-list callback. Returns 0 on accepted request.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ListMeeting(ZrcSdkHandle handle);

// ── Meeting Control extensions ────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_LockMeeting(ZrcSdkHandle handle, int lock);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_EnableMeetingQA(ZrcSdkHandle handle, int enable);
// Synchronously queries the current meeting status (does not require a status change to have
// occurred). Returns the MeetingStatus enum value (0=NotInMeeting,1=Connecting,2=InMeeting,
// 3=LoggedOut) cast to int, or -1 on error (e.g. meeting service not available, or the query
// itself failed). Use this once connected to pick up a meeting that was already in progress
// before the SDK callbacks were registered, since ZrcSdk_SetMeetingStatusCallback only fires on
// a subsequent *change*.
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetMeetingStatus(ZrcSdkHandle handle);

// ── Control System (ZRCS) ─────────────────────────────────────────────────────
// Returns 1 if enabled, 0 if disabled, <0 on error
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_IsZRCSEnabled(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_OpenRoomControls(ZrcSdkHandle handle, int open);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ControlZRCSDevice(ZrcSdkHandle handle, const char* deviceID, const char* methodID, const char* paramID, const char* value);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_ExecuteZRCSScene(ZrcSdkHandle handle, const char* sceneID);

// ── Event Callback Setters ────────────────────────────────────────────────────
// Connection / pairing
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetInitializedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetPairRoomResultCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// message=stateStr, errorCode=ConnectionState enum (0=Established,1=Connected,2=Disconnected,-1=None)
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetConnectionStateChangedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// errorCode=MeetingStatus enum (0=NotInMeeting,1=Connecting,2=InMeeting,3=LoggedOut)
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// message=meetingNumber, errorCode=result
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetStartPmiResultCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// errorCode=ExitMeetingReason enum
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetExitMeetingCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// errorCode=1 if wrong password and retry, 0 otherwise
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingNeedsPasswordCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// message=callerName, errorCode=0
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingInviteCallback(ZrcSdkHandle handle, ZrcMeetingInviteCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingInviteTreatedCallback(ZrcSdkHandle handle, ZrcMeetingInviteTreatedCallback callback, void* userData);
// errorCode=1 if muted
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAudioStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// errorCode=1 if mute on entry enabled
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMuteOnEntryCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// errorCode=participant count
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetParticipantCountCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// errorCode=1 if self is host
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetHostChangedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// errorCode=1 if meeting is being recorded
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetRecordingStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingRecordingInfoCallback(ZrcSdkHandle handle, ZrcMeetingRecordingInfoCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetCameraPresetInfoCallback(ZrcSdkHandle handle, ZrcCameraPresetInfoCallback callback, void* userData);
// errorCode=1 if ZRCS enabled
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetControlSystemEnabledCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// Generic error / instant meeting started
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetErrorCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetInstantMeetingStartedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);

// ── Event Callback Setters (continued) ───────────────────────────────────────
// Audio extended
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAllowAttendeesUnmuteCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAskUnmuteByHostCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetFEACRequestCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetFEACApprovedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetFEACDeclinedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// Video extended
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAllowAttendeesVideoCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// Share
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetSharingStatusCallback(ZrcSdkHandle handle, ZrcSharingStatusCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetAirPlayStatusCallback(ZrcSdkHandle handle, ZrcAirPlayStatusCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoPageStatusCallback(ZrcSdkHandle handle, ZrcVideoPageStatusCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetScreenLayoutStatusCallback(ZrcSdkHandle handle, ZrcScreenLayoutStatusCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoThumbInfoCallback(ZrcSdkHandle handle, ZrcVideoThumbInfoCallback callback, void* userData);
// Breakout Room
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetBOStatusChangedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);  // errorCode=BO_STATUS
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetBORoomListCallback(ZrcSdkHandle handle, ZrcBORoomListCallback callback, void* userData);
// Waiting room
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetInSilentModeCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);  // errorCode=isInSilentMode
// Reactions
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetReactionStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData); // errorCode=selectedFeedback
// Chat
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetChatPrivilegeCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetChatMessageCallback(ZrcSdkHandle handle, ZrcChatMessageCallback callback, void* userData);
// Caption
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetCaptionCallback(ZrcSdkHandle handle, ZrcCaptionCallback callback, void* userData);
// Meeting control
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingLockStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetQAEnabledCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// Recording extended
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetRecordingRequestCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);  // errorCode=userID
// Camera control
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetFarEndCameraControlRequestCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);  // errorCode=userID
// Phone / SIP
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetSIPCallStatusCallback(ZrcSdkHandle handle, ZrcSIPCallCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetSIPServiceStatusCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData); // message=displayName, errorCode=status
// ZRCS extended
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetZRCSDeviceListCallback(ZrcSdkHandle handle, ZrcZRCSDeviceListCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetZRCSSceneListCallback(ZrcSdkHandle handle, ZrcZRCSSceneListCallback callback, void* userData);

#ifdef __cplusplus
}
#endif

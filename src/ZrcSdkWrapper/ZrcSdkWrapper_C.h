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
// needCleanUp:  1 = caller should clear its local list before applying updates
// sessionType:  ConfSessionType enum value
typedef void (ZRCSDKWRAPPER_CALL *ZrcParticipantListCallback)(
    const ZrcParticipant* participants,
    int count,
    int needCleanUp,
    int sessionType,
    void* userData);

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
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SendMeetingPassword(ZrcSdkHandle handle, const char* password);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CancelEnteringMeetingPassword(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_CancelWaitingForHost(ZrcSdkHandle handle);

// ── Audio / Video ─────────────────────────────────────────────────────────────
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetAudioMute(ZrcSdkHandle handle, int mute);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoState(ZrcSdkHandle handle, int start);

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
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingInviteCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
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
// errorCode=1 if ZRCS enabled
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetControlSystemEnabledCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
// Generic error / instant meeting started
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetErrorCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetInstantMeetingStartedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);

#ifdef __cplusplus
}
#endif

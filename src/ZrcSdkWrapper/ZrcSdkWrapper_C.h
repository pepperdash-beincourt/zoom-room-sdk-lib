#pragma once

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

// Callback types
typedef void (ZRCSDKWRAPPER_CALL *SdkEventCallback)(const char* message, int errorCode, void* userData);

// SDK Lifecycle
ZRCSDKWRAPPER_API ZrcSdkHandle ZRCSDKWRAPPER_CALL ZrcSdk_Create();
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_Destroy(ZrcSdkHandle handle);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_Initialize(ZrcSdkHandle handle, const char* configPath);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_Uninitialize(ZrcSdkHandle handle);

// SDK Information
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetSDKVersion(ZrcSdkHandle handle, char* buffer, int bufferSize);

// Room Pairing
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PairRoomWithActivationCode(ZrcSdkHandle handle, const char* activationCode);

// Meeting Control
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartMeeting(ZrcSdkHandle handle, const char* meetingNumber);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_EndMeeting(ZrcSdkHandle handle);

// Audio/Video Control
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetAudioMute(ZrcSdkHandle handle, int mute);
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoState(ZrcSdkHandle handle, int start);

// Event Callbacks
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetInitializedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingStateChangedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetErrorCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData);

#ifdef __cplusplus
}
#endif

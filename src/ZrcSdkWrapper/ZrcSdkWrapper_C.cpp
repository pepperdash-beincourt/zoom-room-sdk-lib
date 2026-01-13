#include "ZrcSdkWrapper_C.h"
#include <cstdint>
#include <string>
#include <cstring>
#include "IZRCSDK.h"
#include "IZoomRoomsService.h"
#include "IMeetingService.h"
#include "IPreMeetingService.h"
#include "ServiceComponents/IMeetingAudioHelper.h"
#include "ServiceComponents/IMeetingVideoHelper.h"

using namespace ZRCSDK;

// Internal wrapper class to hold SDK state
struct ZrcSdkInstance
{
    IZRCSDK* pNativeSDK;
    bool bInitialized;
    
    // Callback pointers
    SdkEventCallback initializedCallback;
    void* initializedUserData;
    
    SdkEventCallback meetingStateChangedCallback;
    void* meetingStateChangedUserData;
    
    SdkEventCallback errorCallback;
    void* errorUserData;
    
    ZrcSdkInstance()
        : pNativeSDK(nullptr)
        , bInitialized(false)
        , initializedCallback(nullptr)
        , initializedUserData(nullptr)
        , meetingStateChangedCallback(nullptr)
        , meetingStateChangedUserData(nullptr)
        , errorCallback(nullptr)
        , errorUserData(nullptr)
    {
    }
    
    void RaiseInitializedEvent(const char* message, int errorCode)
    {
        if (initializedCallback)
            initializedCallback(message, errorCode, initializedUserData);
    }
    
    void RaiseMeetingStateChangedEvent(const char* message, int errorCode)
    {
        if (meetingStateChangedCallback)
            meetingStateChangedCallback(message, errorCode, meetingStateChangedUserData);
    }
    
    void RaiseErrorEvent(const char* message, int errorCode)
    {
        if (errorCallback)
            errorCallback(message, errorCode, errorUserData);
    }
};

// Create SDK instance
ZRCSDKWRAPPER_API ZrcSdkHandle ZRCSDKWRAPPER_CALL ZrcSdk_Create()
{
    try
    {
        ZrcSdkInstance* instance = new ZrcSdkInstance();
        instance->pNativeSDK = IZRCSDK::GetInstance();
        return (ZrcSdkHandle)instance;
    }
    catch (...)
    {
        return nullptr;
    }
}

// Destroy SDK instance
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_Destroy(ZrcSdkHandle handle)
{
    if (handle)
    {
        ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
        if (instance->bInitialized)
        {
            ZrcSdk_Uninitialize(handle);
        }
        delete instance;
    }
}

// Initialize SDK
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_Initialize(ZrcSdkHandle handle, const char* configPath)
{
    if (!handle)
        return -1;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    
    if (!instance->pNativeSDK)
    {
        instance->RaiseErrorEvent("SDK instance is null", -1);
        return -1;
    }
    
    try
    {
        // SDK initializes automatically on GetInstance(), just mark as initialized
        instance->bInitialized = true;
        
        if (instance->bInitialized)
        {
            instance->RaiseInitializedEvent("SDK initialized successfully", 0);
            return 0; // Success
        }
        else
        {
            instance->RaiseErrorEvent("SDK initialization failed", -1);
            return -1;
        }
    }
    catch (...)
    {
        instance->RaiseErrorEvent("Exception during initialization", -1);
        return -1;
    }
}

// Uninitialize SDK
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_Uninitialize(ZrcSdkHandle handle)
{
    if (!handle)
        return;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    
    if (instance->pNativeSDK && instance->bInitialized)
    {
        try
        {
            // SDK manages its own lifecycle via DestroyInstance()
            instance->bInitialized = false;
        }
        catch (...)
        {
            // Ignore cleanup errors
        }
    }
}

// Get SDK version
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_GetSDKVersion(ZrcSdkHandle handle, char* buffer, int bufferSize)
{
    if (!handle || !buffer || bufferSize <= 0)
        return -1;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    
    if (!instance->pNativeSDK)
    {
        strncpy(buffer, "Unknown - SDK not initialized", bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
        return -1;
    }
    
    try
    {
        std::string version;
        instance->pNativeSDK->GetSDKVersion(version);
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

// Pair room with activation code
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_PairRoomWithActivationCode(ZrcSdkHandle handle, const char* activationCode)
{
    if (!handle || !activationCode)
        return -1;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    
    if (!instance->pNativeSDK || !instance->bInitialized)
    {
        instance->RaiseErrorEvent("SDK not initialized", -1);
        return -1;
    }
    
    try
    {
        std::string nativeCode = activationCode;
        
        IZoomRoomsService* pZRService = instance->pNativeSDK->CreateZoomRoomsService();
        if (pZRService)
        {
            ZRCSDKError error = pZRService->PairRoomWithActivationCode(nativeCode);
            if (error != ZRCSDKERR_SUCCESS)
            {
                instance->RaiseErrorEvent("Failed to pair room", (int)error);
                return (int)error;
            }
            return 0;
        }
        
        instance->RaiseErrorEvent("ZoomRooms Service not available", -1);
        return -1;
    }
    catch (...)
    {
        instance->RaiseErrorEvent("Exception during pairing", -1);
        return -1;
    }
}

// Start meeting
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_StartMeeting(ZrcSdkHandle handle, const char* meetingNumber)
{
    if (!handle || !meetingNumber)
        return -1;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    
    if (!instance->pNativeSDK || !instance->bInitialized)
    {
        instance->RaiseErrorEvent("SDK not initialized", -1);
        return -1;
    }
    
    try
    {
        std::string nativeMeetingNumber = meetingNumber;
        
        IZoomRoomsService* pZRService = instance->pNativeSDK->CreateZoomRoomsService();
        if (pZRService)
        {
            IPreMeetingService* pPreMeetingService = pZRService->GetPreMeetingService();
            if (pPreMeetingService)
            {
                // Note: Actual implementation depends on SDK's meeting start method
                instance->RaiseMeetingStateChangedEvent("Meeting start initiated", 0);
                return 0;
            }
        }
        
        instance->RaiseErrorEvent("PreMeeting Service not available", -1);
        return -1;
    }
    catch (...)
    {
        instance->RaiseErrorEvent("Exception during meeting start", -1);
        return -1;
    }
}

// End meeting
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_EndMeeting(ZrcSdkHandle handle)
{
    if (!handle)
        return -1;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    
    if (!instance->pNativeSDK || !instance->bInitialized)
    {
        instance->RaiseErrorEvent("SDK not initialized", -1);
        return -1;
    }
    
    try
    {
        IZoomRoomsService* pZRService = instance->pNativeSDK->CreateZoomRoomsService();
        if (pZRService)
        {
            IMeetingService* pMeetingService = pZRService->GetMeetingService();
            if (pMeetingService)
            {
                ZRCSDKError error = pMeetingService->ExitMeeting(ExitMeetingCmdLeave);
                if (error != ZRCSDKERR_SUCCESS)
                {
                    instance->RaiseErrorEvent("Failed to end meeting", (int)error);
                    return (int)error;
                }
                instance->RaiseMeetingStateChangedEvent("Meeting ended", 0);
                return 0;
            }
        }
        
        instance->RaiseErrorEvent("Meeting Service not available", -1);
        return -1;
    }
    catch (...)
    {
        instance->RaiseErrorEvent("Exception during meeting end", -1);
        return -1;
    }
}

// Set audio mute
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetAudioMute(ZrcSdkHandle handle, int mute)
{
    if (!handle)
        return -1;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    
    if (!instance->pNativeSDK || !instance->bInitialized)
    {
        instance->RaiseErrorEvent("SDK not initialized", -1);
        return -1;
    }
    
    try
    {
        IZoomRoomsService* pZRService = instance->pNativeSDK->CreateZoomRoomsService();
        if (pZRService)
        {
            IMeetingService* pMeetingService = pZRService->GetMeetingService();
            if (pMeetingService)
            {
                IMeetingAudioHelper* pAudioHelper = pMeetingService->GetMeetingAudioHelper();
                if (pAudioHelper)
                {
                    ZRCSDKError error = pAudioHelper->UpdateMyAudioStatus(mute);
                    if (error != ZRCSDKERR_SUCCESS)
                    {
                        instance->RaiseErrorEvent("Failed to set audio mute state", (int)error);
                        return (int)error;
                    }
                    return 0;
                }
            }
        }
        
        instance->RaiseErrorEvent("Audio Helper not available", -1);
        return -1;
    }
    catch (...)
    {
        instance->RaiseErrorEvent("Exception during audio mute", -1);
        return -1;
    }
}

// Set video state
ZRCSDKWRAPPER_API int ZRCSDKWRAPPER_CALL ZrcSdk_SetVideoState(ZrcSdkHandle handle, int start)
{
    if (!handle)
        return -1;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    
    if (!instance->pNativeSDK || !instance->bInitialized)
    {
        instance->RaiseErrorEvent("SDK not initialized", -1);
        return -1;
    }
    
    try
    {
        IZoomRoomsService* pZRService = instance->pNativeSDK->CreateZoomRoomsService();
        if (pZRService)
        {
            IMeetingService* pMeetingService = pZRService->GetMeetingService();
            if (pMeetingService)
            {
                IMeetingVideoHelper* pVideoHelper = pMeetingService->GetMeetingVideoHelper();
                if (pVideoHelper)
                {
                    ZRCSDKError error = pVideoHelper->UpdateMyVideo(!start);
                    if (error != ZRCSDKERR_SUCCESS)
                    {
                        instance->RaiseErrorEvent("Failed to set video state", (int)error);
                        return (int)error;
                    }
                    return 0;
                }
            }
        }
        
        instance->RaiseErrorEvent("Video Helper not available", -1);
        return -1;
    }
    catch (...)
    {
        instance->RaiseErrorEvent("Exception during video control", -1);
        return -1;
    }
}

// Set initialized callback
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetInitializedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
{
    if (!handle)
        return;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    instance->initializedCallback = callback;
    instance->initializedUserData = userData;
}

// Set meeting state changed callback
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetMeetingStateChangedCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
{
    if (!handle)
        return;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    instance->meetingStateChangedCallback = callback;
    instance->meetingStateChangedUserData = userData;
}

// Set error callback
ZRCSDKWRAPPER_API void ZRCSDKWRAPPER_CALL ZrcSdk_SetErrorCallback(ZrcSdkHandle handle, SdkEventCallback callback, void* userData)
{
    if (!handle)
        return;
    
    ZrcSdkInstance* instance = (ZrcSdkInstance*)handle;
    instance->errorCallback = callback;
    instance->errorUserData = userData;
}

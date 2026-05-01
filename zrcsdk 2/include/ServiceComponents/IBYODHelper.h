/*!
 * \file IBYODHelper.h
 * \since Added in ZRC SDK 6.0.0.
 * \brief BYOD interfaces.
 * 
 *     BYOD workflows:
 * 
 *     BYODModeStatusIDLE    ->  Connect personal device  ->  BYODModeStatusPlugin
 * 
 *     BYODModeStatusPlugin  ->  Call DeactivateBYOD API  ->  BYODModeStatusInactive
 *     BYODModeStatusPlugin  ->  Call ActiveHDMIShare API ->  BYODModeStatusInactive
 *     BYODModeStatusPlugin  ->  Call ActiveBYODMode API  ->  BYODModeStatusActive    ->  Control byod camera, speaker, microphone  ->  BYODModeStatusActive
 * 
 *     BYODModeStatusActive:
 *     if the BYODModeInfo.supportManuallyOperate is true:
 *         Call DeactivateBYOD API  ->  BYODModeStatusInactive
 *     else 
 *         Unplug personal device to exit HDMI share  ->  BYODModeStatusIDLE
 * 
 *     BYODModeStatusInactive:
 *     if the BYODModeInfo.supportManuallyOperate is true:
 *         Call ActiveBYODMode API  ->  BYODModeStatusActive
 * 
 */
#ifndef ZRC_IBYODHELPER_H
#define ZRC_IBYODHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum BYODCapability
    \brief BYOD capability
    Here are more detailed structural descriptions.
*/
enum BYODCapability
{
    BYODCapabilityBYOD = 0, ///<Only support BYOD
    BYODCapabilityHDMI      ///<Support both BYOD and HDMI
};

/*! \enum BYODModeStatus
    \brief BYOD mode status.
    Here are more detailed structural descriptions.
*/
enum BYODModeStatus
{
    BYODModeStatusIDLE = 0,    ///<BYOD mode status is idle.
    BYODModeStatusPlugin,      ///<BYOD mode status is plugin.
    BYODModeStatusInactive,    ///<BYOD mode status is inactive.
    BYODModeStatusActive       ///<BYOD mode status is active.
};

/*! \enum BYODAutoFramingStatus
    \brief BYOD auto framing status.
    Here are more detailed structural descriptions.
*/
enum BYODAutoFramingStatus
{
    BYODAutoFramingStatusNone = 0,    ///<BYOD auto framing status is none.
    BYODAutoFramingStatusOn,          ///<BYOD auto framing status is on.
    BYODAutoFramingStatusOff          ///<BYOD auto framing status is off.
};

/*! \enum BYODMicrophoneStatus
    \brief BYOD microphone status.
    Here are more detailed structural descriptions.
*/
enum BYODMicrophoneStatus
{
    BYODMicrophoneStatusNone = 0,    ///<BYOD microphone status is none.
    BYODMicrophoneStatusMuted,       ///<BYOD microphone status is muted.
    BYODMicrophoneStatusUnmuted      ///<BYOD microphone status is unmuted.
};

/*! \enum ActiveBYODAction
    \brief Active BYOD action.
    Here are more detailed structural descriptions.
*/
enum ActiveBYODAction
{
    ActiveBYODActionNone = 0,    ///<Active BYOD action is none.
    ActiveBYODActionActive,      ///<Active BYOD action is active.
    ActiveBYODActionInactive     ///<Active BYOD action is inactive.
};

/*! \enum ActiveHDMIAction
    \since The enum is supported on Zoom Rooms clients with version 6.0.0 or later.
    \brief Active HDMI action.
    Here are more detailed structural descriptions.
*/
enum ActiveHDMIAction
{
    ActiveHDMIActionNone = 0,    ///<Active HDMI action is none.
    ActiveHDMIActionActive,      ///<Active HDMI action is active.
    ActiveHDMIActionInactive     ///<Active HDMI action is inactive.
};

/*! \enum BYODManualOperateAction
    \brief BYOD manual operate action.
    Here are more detailed structural descriptions.
*/
enum BYODManualOperateAction
{
    BYODManualOperateActionNone = 0,    ///<BYOD manual operate action is none.
    BYODManualOperateActionEnter,       ///<BYOD manual operate action is enter.
    BYODManualOperateActionExit         ///<BYOD manual operate action is exit.
};

/// \brief BYOD mode info.
///
struct BYODModeInfo
{
    BYODModeStatus        status = BYODModeStatusIDLE;                      ///<BYOD mode status.
    Device                cameraInfo;                                       ///<Camera info in BYOD mode.
    BYODAutoFramingStatus autoFramingStatus = BYODAutoFramingStatusNone;    ///<Auto framing status in BYOD mode, BYODAutoFramingStatusNone indicates not support control auto framing in BYOD mode.
    BYODMicrophoneStatus  microphoneStatus = BYODMicrophoneStatusNone;      ///<Microphone status in BYOD mode, BYODMicrophoneStatusNone indicates not support control microphone in BYOD mode.
    int32_t               speakerVolume = -1;                               ///<Speaker volume in BYOD mode, -1 indicates not support control speaker volume in BYOD mode.
    bool                  supportManuallyOperate = false;                   ///<TRUE indicates support manually operate.
    int32_t               capability = BYODCapabilityBYOD;                  ///<BYOD's capability, see \link BYODCapability \endlink enum.
    bool                  isHDMIReady = false;                              ///<TRUE indicates HDMI is ready.
    bool                  supportEmergencyCall = false;                     ///<TRUE indicates supports emergency call in BYOD mode
};

/// \brief Operate BYOD result.
///
struct BYODModeResult
{
    int32_t                 result = 0;                                    ///<[0] indicates success else fail.
    ActiveBYODAction        byodAction = ActiveBYODActionNone;             ///<Operation for BYOD mode.
    ActiveHDMIAction        hdmiAction = ActiveHDMIActionNone;             ///<Operation for HDMI share.
    BYODManualOperateAction manualAction = BYODManualOperateActionNone;    ///<Manual Operation for BYOD mode.
};

/// \brief BYOD helper event callback
///
class IBYODHelperSink
{
public:
    virtual ~IBYODHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief BYOD mode info notification.
     *
     * @param info BYOD mode info, details \link BYODModeInfo \endlink struct.
     */
    virtual void OnBYODModeInfoNotification(const BYODModeInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Operate BYOD result.
     *
     * @param result Operate BYOD result, details \link BYODModeResult \endlink struct.
     */
    virtual void OnBYODModeResult(const BYODModeResult& result) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief BYOD to emergency call notification. After using this API, zoom room will exit byod mode, then you can make emergency call through calling \link IPhoneCallService::CallSIP \endlink API.
     *
     * @param succeed TRUE indicates make emergency call in BYOD mode successfully.
     */
    virtual void OnMakeEmergencyCallInBYODModeNotification(bool succeed) = 0;
};

/// \brief BYOD helper interface
///
class IBYODHelper
{
public:
    virtual ~IBYODHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register BYOD helper callback sink.
     *
     * @param pSink A pointer to the IBYODHelperSink that receives the BYOD helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IBYODHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister BYOD helper callback sink.
     *
     * @param pSink A pointer to the IBYODHelperSink that receives the BYOD helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IBYODHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Active BYOD mode, you can use ZR's camera, speaker, microphone and screen.
     *
     * If supportManuallyOperate field in \link BYODModeInfo \endlink struct is true, you can exit BYOD mode by calling ExitBYODMode; otherwise you can only exit BYOD mode by unplugging the personal device.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ActiveBYODMode() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Active HDMI share, you can only share content on ZR's screen. The API can be called to enter HDMI share when BYOD mode status is plugin.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
     virtual ZRCSDKError ActiveHDMIShare() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deactivate BYOD.
     *
     * You can call this API when the BYOD mode status is plugin and you do not want to enter BYOD mode or HDMI share, or when you want to exit BYOD mode and the supportManuallyOperate field in \link BYODModeInfo \endlink struct is true.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeactivateBYOD() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control BYOD camera.
     *
     * @param controlAction Camera control action, see \link CameraControlAction \endlink enum.
     * @param controlType Camera control type, see \link CameraControlType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetBYODCameraControl(CameraControlAction controlAction, CameraControlType controlType) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control BYOD auto framing.
     *
     * @param autoFramingOn TRUE indicates set BYOD auto framing on.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetBYODAutoFramingOn(bool autoFramingOn) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control BYOD microphone.
     *
     * @param muted TRUE indicates set BYOD microphone muted.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetBYODMicrophoneMuted(bool muted) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control BYOD speaker volume.
     *
     * @param volume Speaker volume [0 - 100].
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetBYODSpeakerVolume(int32_t volume) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Exit BYOD mode for making emergency call in BYOD mode. Call this API will only exit BYOD mode but not make emergency call.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MakeEmergencyCallInBYODMode() = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_IBYODHELPER_H

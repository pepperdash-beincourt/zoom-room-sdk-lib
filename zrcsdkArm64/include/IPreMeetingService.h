/*!
 * \file IPreMeetingService.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Entrance of all the PreMeeting Service.
 */
#ifndef ZRC_IPREMEETINGSERVICE_H
#define ZRC_IPREMEETINGSERVICE_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum LogType
    \brief Send problem report log type.
    Here are more detailed structural descriptions.
 */
enum LogType
{
    LogTypeBasic = 0,           ///<Send basic logs.
    LogTypeAudio = 1,           ///<Send basic and audio logs.
    LogTypeContentSharing = 2,  ///<Send basic and content sharing logs.
    LogTypeCrashDump = 3        ///<Send basic and crash dump logs.
};

/// \brief PreMeeting service event callback.
///
class IPreMeetingServiceSink
{
public:
    virtual ~IPreMeetingServiceSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Connect to Zoom Room state changed notification.
     *
     * When connect to a Zoom Room, ConnectionState will first become @ref ConnectionStateEstablished.
     * If the connection is verified by Zoom Rooms, ConnectionState will become @ref ConnectionStateConnected.
     * After @ref ConnectionStateConnected, you can call other APIs to Zoom Rooms safely.
     *
     * @param connectionState Current connection state, see \link ConnectionState \endlink enum.
     */
    virtual void OnZRConnectionStateChanged(ConnectionState connectionState) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Shut down OS notification.
     *
     * @param restartOS TRUE indicates to restart OS.
     */
    virtual void OnShutdownOSNot(bool restartOS) = 0;
};

class IContactHelper;
class IBYODHelper;
class IControlSystemHelper;

/// \brief PreMeeting service interface
///
class IPreMeetingService
{
public:
    virtual ~IPreMeetingService() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register premeeting service callback sink.
     *
     * @param pSink A pointer to the IPreMeetingServiceSink that receives the premeeting service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IPreMeetingServiceSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister premeeting service callback sink.
     *
     * @param pSink A pointer to the IPreMeetingServiceSink that receives the premeeting service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IPreMeetingServiceSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query connection state with Zoom Room.
     *
     * @param[out] state Connection state, see \link ConnectionState \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetConnectionState(ConnectionState& state) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Notify ZR to send problem report to Zoom.
     *
     * @param subject Problem report email subject.
     * @param body Problem report email body.
     * @param logType Send log type, see \link LogType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError NotifyZoomRoomsSendProblemReport(const std::string& subject, const std::string& body, LogType logType) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if ZR support restart OS.
     *
     * @param[out] support TRUE indicates ZR supports restart os.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsZoomRoomSupportRestartOS(bool& support) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Restart current room displays, scheduling displays and controllers.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RestartZoomRoomOS() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Logout room and other paired devices.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LogoutZoomRoomDevice() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Wake Zoom Room up.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError WakeZoomRoomUp() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the contact helper.
     *
     * @return If the function succeeds, the return value is a pointer to IContactHelper. Otherwise the function returns NULL.
     */
    virtual IContactHelper* GetContactHelper() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the BYOD helper.
     *
     * @return If the function succeeds, the return value is a pointer to IBYODHelper. Otherwise the function returns NULL.
     */
    virtual IBYODHelper* GetBYODHelper() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the control system helper.
     *
     * @return If the function succeeds, the return value is a pointer to IControlSystemHelper. Otherwise the function returns NULL.
     */
    virtual IControlSystemHelper* GetControlSystemHelper() = 0;
};

NS_ZRCSDK_END
#endif    //ZRC_IPREMEETINGSERVICE_H

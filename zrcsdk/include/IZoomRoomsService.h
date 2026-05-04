/*!
 * \file IZoomRoomsService.h
 * \since Added in ZRC SDK 6.0.0
 * \brief ZoomRooms Service Interface
 */
#ifndef ZRC_IZOOMROOMSSERVICE_H
#define ZRC_IZOOMROOMSSERVICE_H

#include "ZRCSDKDefines.h"
#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

class ISettingService;
class IPreMeetingService;
class IMeetingService;
class IPhoneCallService;
class IProAVService;

/// \brief ZoomRooms Service event callback.
///
class IZoomRoomsServiceSink
{
public:
    virtual ~IZoomRoomsServiceSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Pair room with activation code result notification.
     *
     * When result is success, sdk internal will do the connect to the Zoom Rooms.
     * After IPreMeetingService::OnZRConnectionStateChanged ConnectionState becomes ConnectionStateConnected,
     * sdk is completely connected to the Zoom Rooms.
     *
     * @param result [0] success. [30055016] Invalid activation code.
     *
     * @changelog
     * - 6.5.0: Add error codes: 
     *  [100] Fail to connect to room. 
     *  [101] Room cannot verify connection. 
     *  [102] Timeout waiting room's verify response.
     *
     */
    virtual void OnPairRoomResult(int32_t result) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Room been unpaired with reason notification.
     *
     * @param reason Room unpaired reason, see \link RoomUnpairedReason \endlink enum.
     */
    virtual void OnRoomUnpairedReason(RoomUnpairedReason reason) = 0;
};

/// \brief ZoomRooms Service Interface.
///
class IZoomRoomsService
{
public:
    virtual ~IZoomRoomsService() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register ZoomRooms service callback sink.
     *
     * @param pSink A pointer to the IZoomRoomsServiceSink that receives the ZoomRooms service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IZoomRoomsServiceSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister ZoomRooms service callback sink.
     *
     * @param pSink A pointer to the IZoomRoomsServiceSink that receives the ZoomRooms service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IZoomRoomsServiceSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Pair Zoom Room with activation code.
     *
     * @param activationCode Specify the Zoom Room's activation code.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PairRoomWithActivationCode(const std::string& activationCode) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Unpair SDK with current paired Zoom Room.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnpairRoom() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Retry to pair room without activation code.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RetryToPairRoom() = 0;

    /**
     * @since 6.3.0
     * @zrminver 6.0.0
     *
     * @brief Query if can retry to pair last room.
     *
     * @param[out] canRetryToPair TRUE indicates can retry to pair last room. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CanRetryToPairLastRoom(bool& canRetryToPair) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.0.0
     *
     * @brief Get last paired Zoom Room's information.
     *
     * @param[out] roomInfo Zoom Room's information.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetLastZoomRoomInfo(ZoomRoomInfo& roomInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the setting service.
     *
     * @return If the function succeeds, the return value is a pointer to ISettingService. Otherwise the function returns NULL.
     */
    virtual ISettingService* GetSettingService() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the premeeting service.
     *
     * @return If the function succeeds, the return value is a pointer to IPreMeetingService. Otherwise the function returns NULL.
     */
    virtual IPreMeetingService* GetPreMeetingService() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the meeting service.
     *
     * @return If the function succeeds, the return value is a pointer to IMeetingService. Otherwise the function returns NULL.
     */
    virtual IMeetingService* GetMeetingService() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the phone call service.
     *
     * @return If the function succeeds, the return value is a pointer to IPhoneCallService. Otherwise the function returns NULL.
     */
    virtual IPhoneCallService* GetPhoneCallService() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get the Pro/AV service.
     *
     * @return If the function succeeds, the return value is a pointer to IProAVService. Otherwise the function returns NULL.
     */
    virtual IProAVService* GetProAVService() = 0;
};

NS_ZRCSDK_END

#endif /* ZRC_IZOOMROOMSSERVICE_H */

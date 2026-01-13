/*!
 * \file IWaitingRoomHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Waiting Room interfaces.
 */
#ifndef ZRC_IWAITINGROOMHELPER_H
#define ZRC_IWAITINGROOMHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/// \brief In silent mode information.
///Silent mode includes waiting room and put on hold.
struct InSilentModeInfo
{
    bool isInSilentMode = false;         ///<TRUE indicates current is in silent mode.
    bool silentModeForNoHost = false;    ///<TRUE indicates no host is in meeting, FALSE indicates waiting for host to admit into meeting.
    bool isPutInByManual = false;        ///<TRUE indicates be manually put into silent mode.
};

/// \brief Waiting Room helper event callback.
///
class IWaitingRoomHelperSink
{
public:
    virtual ~IWaitingRoomHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief In silent mode update notification.
     *
     * @param info In silent mode information.
     */
    virtual void OnInSilentModeNotification(const InSilentModeInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable waiting room on entry notification.
     *
     * @param isEnable TRUE indicates all users will enter waiting room when joined meeting.
     */
    virtual void OnEnableWaitingRoomOnEntryNotification(bool isEnable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Can admit guest from waiting room into meeting update notification.
     *
     * @param isEnabled TRUE indicates can admit guest from waiting room into meeting.
     */
    virtual void OnUpdateAdmitGuestEnableNotification(bool isEnabled) = 0;
};

/// \brief Waiting Room helper interface.
///
class IWaitingRoomHelper
{
public:
    virtual ~IWaitingRoomHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register waiting room helper callback sink.
     *
     * @param pSink A pointer to the IWaitingRoomHelperSink that receives the waiting room helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IWaitingRoomHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister waiting room helper callback sink.
     *
     * @param pSink A pointer to the IWaitingRoomHelperSink that receives the waiting room helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IWaitingRoomHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Put users into meeting.
     *
     * @param userIDs Assigned user list to be put into meeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PutUsersIntoMeeting(std::vector<int32_t> userIDs) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Put users into waiting room.
     *
     * @param userIDs Assigned user list to be put into waiting room.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PutUsersIntoWaitingRoom(std::vector<int32_t> userIDs) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Put all the waiting room users into meeting.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PutAllUsersIntoMeeting() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if enable waiting room setting is locked.
     *
     * @param[out] locked TRUE indicates enable waiting room setting is locked.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsWaitingRoomLocked(bool& locked) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable waiting room on entry.
     *
     * @param enable TRUE indicates users will enter in waiting room when joined the meeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableWaitingRoomOnEntry(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if is waiting room on entry.
     *
     * @param[out] onEntry TRUE indicates users will enter in waiting room when joined the meeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsWaitingRoomOnEntry(bool& onEntry) = 0;
};

NS_ZRCSDK_END
#endif    //ZRC_IWAITINGROOMHELPER_H

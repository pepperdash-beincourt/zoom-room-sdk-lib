/*!
 * \file IWebinarBreakoutRoomHelper.h
 * \since Added in ZRC SDK 6.3.5.
 * \brief Webinar Breakout Room interfaces.
 *
 *      1. WBO are short for Webinar Breakout Room.
 *      2. Managing WBO (e.g., creating, renaming, or deleting) is not supported yet.
 *      3. BO common work flow
 *          1) Enter meeting    >>>  BO_STATUS_EDIT
 *          2) Creates BO       >>>  BO_STATUS_EDIT
 *          3) Starts  BO       >>>  BO_STATUS_STARTED
 *          4) Stops   BO       >>>  BO_STATUS_STOPPING
 *          5) BO is ended      >>>  BO_STATUS_EDIT
 *      4. Lifecycle
 *          BO_STATUS_EDIT >>> BO_STATUS_STARTED >>> BO_STATUS_STOPPING >>> BO_STATUS_EDIT
 *      5. Use Master session and webinarBOStatus.uniqueJoinIndex when comparing user between breakout rooms.
 *      6. Helpers
 *            1) host in master conference      : creator + admin + assistant + dataHelper
 *            2) host in BO conference          : admin + assistant + dataHelper
 *            3) CoHost in master conference    : [attendee] or [creator + admin + assistant + dataHelper]
 *            4) CoHost in BO conference        : [attendee] or [admin + assistant + dataHelper]
 *            5) attendee in master conference  : attendee + [assistant + dataHelper]
 *            6) attendee in BO conference      : attendee + [assistant + dataHelper]
 *
 */

#ifndef IWebinarBreakoutRoomHelper_h
#define IWebinarBreakoutRoomHelper_h

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

////////////////////////////////////////// IWebinarBOCreatorHelper //////////////////////////////////////////

/// \brief Webinar breakout room options
///
struct WebinarBOOptions
{
    bool     isPanelistCanChooseRoom = false;                    ///<True indicates panelist can choose room.
    bool     isAttendeeCanChooseRoom = false;                    ///<True indicates attendee can choose room.
    bool     isParticipantCanReturnToMainSessionAtAnyTime = true;///<True indicates participants can return to main session at any time.
    bool     isAutoMoveAllAssignedParticipantsEnabled = false;   ///<True indicates automatically move all assigned participants into breakout rooms.
    int      maxParticipantsCountPerRoom = 500;                  ///<Max participants count of each room.

    bool     isBOTimerEnabled = false;                              ///<True indicates it's a timer BO.
    uint64_t boTimerDuration = 0;                                   ///<Seconds of BO timer duration, if BO timer is enabled, boTimerDuration is 30*60 seconds by default.

    bool     isNotifyMeWhenTimeIsUp = false;                        ///<True indicates will receive a notification when duration time is up. Otherwise BO is auto stopped.

    BO_STOP_COUNTDOWN countdownSeconds = BO_STOP_NOT_COUNTDOWN;     ///<BO stop countdown seconds, see \link BO_STOP_COUNTDOWN \endlink enum.
};

/// \brief Breakout room users
///
struct BreakoutRoomUsersInfo
{
    std::string sessionBID;                 ///<Session BID.
    int32_t     userCount = 0;              ///<user count including assigned and joined
    std::vector<int32_t> userJoinIndexes;   ///<user's uniqueJoinIndex in MasterSession, attendee can't access other attendee's information
};

/// \brief WebinarBO creator callback handler
///
class IWebinarBOCreatorHelperSink
{
public:
    virtual ~IWebinarBOCreatorHelperSink() {}

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Breakout room options notification.
     * 
     * @param boOptions The webinar breakout room options.
     */
    virtual void OnBOOptionsNotification(const WebinarBOOptions& boOptions) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Breakout room info list changed callback. If bo room count or name changed, you will receive this callback.
     * 
     * @note room can be added/removed in webinar meeting dynamatically.
     * @param boRoomInfoList New bo room info list.
     */
    virtual void OnBORoomInfoListChanged(const std::vector<BreakoutRoomInfo>& boRoomInfoList) = 0;
};

/// \brief WebinarBO creator interface
///
class IWebinarBOCreatorHelper
{
public:
    virtual void RegisterSink(IWebinarBOCreatorHelperSink* pSink) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Get current WebinarBO options.
     * 
     * @param[out] boOptions Current BO options.
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetBOOptions(WebinarBOOptions& boOptions) = 0;
};

////////////////////////////////////////// IWebinarBOAdminHelper //////////////////////////////////////////

/// \brief WebinarBO admin callback handler
///
class IWebinarBOAdminHelperSink
{
public:
    virtual ~IWebinarBOAdminHelperSink() {}

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief BO end timer update notification, if BO enable timer, you will receive this notification when bo starts.
     *
     * @param remainingSecond The bo remaining time in seconds.
     */
    virtual void OnBOEndTimerUpdate(uint64_t remainingSecond) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief BO time is up notification, you can decide to stop BO immediately or later.
     */
    virtual void OnBOTimeIsUpNotification() = 0;
};

/// \brief WebinarBO admin interface
///
class IWebinarBOAdminHelper
{
public:
    virtual void RegisterSink(IWebinarBOAdminHelperSink* pSink) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Stop all breakout rooms.
     * 
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopBreakoutRooms() = 0;
};

////////////////////////////////////////// IWebinarBOAssistantHelper //////////////////////////////////////////

/// \brief WebinarBO assistant interface.
///
class IWebinarBOAssistantHelper
{
public:
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Join breakout room.
     * 
     * @param sessionBID Specify the BID of the room to join.
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError JoinBreakoutRoom(const std::string& sessionBID) = 0;
        
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Response host invite to main session when receiving OnHostInviteReturnToMainSession.
     *
     * @param accept TRUE indicates to return to main session. Otherwise ignore.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ResponseHostInviteToMainSession(bool accept) = 0;
};

////////////////////////////////////////// IWebinarBOAttendeeHelper //////////////////////////////////////////

/// \brief WebinarBO attendee interface
///
class IWebinarBOAttendeeHelper
{
public:
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Join the assigned breakout room.
     * 
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError JoinBreakoutRoom() = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Leave joined breakout room.
     * 
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LeaveBreakoutRoom() = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Determine if participant can return to main session.
     *
     * @param[out] canReturn TRUE indicates can return to main session.
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsCanReturnMainSession(bool& canReturn) = 0;
};

////////////////////////////////////////// IWebinarBODataHelper //////////////////////////////////////////

/// \brief WebinarBO data callback handler.
///
class IWebinarBODataHelperSink
{
public:
    virtual ~IWebinarBODataHelperSink() {}
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief BO users info notification.
     *
     * @param roomUsers users info of specific BO.
     */
    virtual void OnGetBreakoutRoomUserList(const BreakoutRoomUsersInfo& roomUsers) = 0;
};

/// \brief WebinarBO data interface
///
class IWebinarBODataHelper
{
public:
    virtual void RegisterSink(IWebinarBODataHelperSink* pSink) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Query self breakout room user status.
     *
     * @param[out] boUserStatus Self bo user status, see \link BO_USER_STATUS \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetBOUserStatus(BO_USER_STATUS& boUserStatus) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Get all breakout room info list.
     * 
     * @param[out] boRoomList All bo room info list.
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetBreakoutRoomList(std::vector<BreakoutRoomInfo>& boRoomList) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Get users info of breakout room.
     *
     * @param[in] sessionBID  The BO's sessionBID .
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetBreakoutRoomUserList(const std::string& sessionBID) = 0;
};

////////////////////////////////////////// IBreakoutRoomHelper //////////////////////////////////////////

/// \brief Breakout Room helper event callback
///
class IWebinarBreakoutRoomHelperSink
{
public:
    virtual ~IWebinarBreakoutRoomHelperSink() {}

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Breakout room status changed notification.
     *
     * @param boStatus New BO status, see \link BO_STATUS \endlink enum.
     */
    virtual void OnBOStatusChanged(BO_STATUS boStatus) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Receive switch to breakout room request.
     * 
     * @param fromUserName The request from user's name, empty string indicates from host.
     * @param newBORoomInfo The new BO room info.
     * Whenever the host switches you to another BO while you are assigned but haven't joined the BO, you will receive this event.
     */
    virtual void OnBOSwitchRequestReceived(const std::string& fromUserName, const BreakoutRoomInfo& newBORoomInfo) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Host invite you to return to main session notification.
     */
    virtual void OnHostInviteReturnToMainSession() = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief BO stop countdown, if BO enable countdown, you will receive this notification when bo is stopping, see \link BO_STOP_COUNTDOWN \endlink enum.
     *
     * @param remainingSeconds The bo stop countdown remaining time in seconds.
     */
    virtual void OnBOStopCountDown(uint64_t remainingSeconds) = 0;

    /**
    * @since 6.3.5
    * @zrminver 6.2.0
    *
    * @brief Start to join BO notification.
    */
    virtual void OnStartJoinBONotification() = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @param meetingInfo The updated meeting info.
     */
    virtual void OnUpdateMeetingInfoNotification(const MeetingInfo& meetingInfo) = 0;
};

/// \brief Breakout Room helper interface
///
class IWebinarBreakoutRoomHelper
    : public IWebinarBOCreatorHelper
    , public IWebinarBOAdminHelper
    , public IWebinarBOAssistantHelper
    , public IWebinarBOAttendeeHelper
    , public IWebinarBODataHelper
{
public:
    virtual ~IWebinarBreakoutRoomHelper() {}

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Register webinar breakout room helper callback sink.
     * 
     * @param pSink A pointer to the IWebinarBreakoutRoomHelperSink that receives the breakout room help callback.
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IWebinarBreakoutRoomHelperSink* pSink) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Deregister webinar breakout room helper callback sink.
     * 
     * @param pSink A pointer to the IWebinarBreakoutRoomHelperSink that receives the breakout room help callback.
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IWebinarBreakoutRoomHelperSink* pSink) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Get the pointer of WebinarBO creator object.
     * 
     * @return If the function succeeds, the return value is a pointer to IWebinarBOCreatorHelper object. For more details,
     * see \link IWebinarBOCreatorHelper \endlink. Otherwise failed, the return value is NULL.
     */
    virtual IWebinarBOCreatorHelper*   GetBOCreatorHelper() = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Get the pointer of WebinarBO administrator object.
     * 
     * @return If the function succeeds, the return value is a pointer to IWebinarBOAdminHelper object. For more details,
     * see \link IWebinarBOAdminHelper \endlink. Otherwise failed, the return value is NULL.
     */
    virtual IWebinarBOAdminHelper*     GetBOAdminHelper() = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Get the pointer of WebinarBO assistant object.
     * 
     * @return If the function succeeds, the return value is a pointer to IWebinarBOAssistantHelper object. For more details,
     * see \link IWebinarBOAssistantHelper \endlink. Otherwise failed, the return value is NULL.
     */
    virtual IWebinarBOAssistantHelper* GetBOAssistantHelper() = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Get the pointer of WebinarBO attendee object.
     * 
     * @return If the function succeeds, the return value is a pointer to IWebinarBOAttendeeHelper object. For more details,
     * see \link IWebinarBOAttendeeHelper \endlink. Otherwise failed, the return value is NULL.
     */
    virtual IWebinarBOAttendeeHelper*  GetBOAttendeeHelper() = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Get the pointer of WebinarBO data object.
     * 
     * @return If the function succeeds, the return value is a pointer to IWebinarBODataHelper object. For more details,
     * see \link IWebinarBODataHelper \endlink. Otherwise failed, the return value is NULL.
     */
    virtual IWebinarBODataHelper*      GetBODataHelper() = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Determine if the WebinarBO feature is enabled in current meeting.
     * 
     * @param[out] enabled TRUE indicates breakout room is enabled in this meeting.
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsBOEnabled(bool& enabled) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Determine if the WebinarBO is started or not.
     * 
     * @param[out] started TRUE indicates breakout room is started or stopping, see \link BO_STATUS \endlink enum.
     * @return If the function succeeds, the return value is ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsBOStarted(bool& started) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Query if you are in main session.
     *
     * @param[out] isInMainSession TRUE indicates you are in main session.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsInMainSession(bool& isInMainSession) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Query current breakout room status.
     *
     * @param[out] boStatus The bo status, see \link BO_STATUS \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetBOStatus(BO_STATUS& boStatus) = 0;
};

NS_ZRCSDK_END

#endif /* IWebinarBreakoutRoomHelper_h */

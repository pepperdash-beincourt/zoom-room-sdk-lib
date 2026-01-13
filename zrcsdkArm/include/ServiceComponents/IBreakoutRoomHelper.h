/*!
 * \file IBreakoutRoomHelper.h
 * \since Added in ZRC SDK 6.0.0.
 * \brief Breakout Room interfaces.
 * 
 *      1. BO or BoR are short for Breakout Room.
 *      2. BO common work flow
 *          1) Enter meeting    >>>  BO_STATUS_EDIT
 *          2) Creates BO       >>>  BO_STATUS_EDIT
 *          3) Starts  BO       >>>  BO_STATUS_STARTED
 *          4) Stops   BO       >>>  BO_STATUS_STOPPING
 *          5) BO is ended      >>>  BO_STATUS_ENDED
 *      3. Lifecycle
 *          BO_STATUS_EDIT >>> BO_STATUS_STARTED >>> BO_STATUS_STOPPING >>> BO_STATUS_ENDED
 *      4. BOParticipantList includes all the breakout rooms and master conference participants.
 *      5. Use BOParticipantList and userGUID when comparing user between breakout rooms.
 *      6. Helpers
 *	        1) host in master conference      : creator + admin + assistant + dataHelper
 *	        2) host in BO conference          : admin + assistant + dataHelper
 *	        3) CoHost in master conference    : [attendee] or [creator + admin + assistant + dataHelper]
 *	        4) CoHost in BO conference        : [attendee] or [admin + assistant + dataHelper]
 *	        5) attendee in master conference  : attendee + [assistant + dataHelper]
 *          6) attendee in BO conference      : attendee + [assistant + dataHelper]
 *
 */
#ifndef ZRC_IBREAKOUTROOMHELPER_H
#define ZRC_IBREAKOUTROOMHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

////////////////////////////////////////// IBOCreatorHelper //////////////////////////////////////////

/*! \enum BO_ASSIGN_PARTICIPANTS_TYPE
    \brief After creating breakout rooms, assign participants type.
    Here are more detailed structural descriptions.
 */
enum BO_ASSIGN_PARTICIPANTS_TYPE
{
    ASSIGN_PARTICIPANTS_AUTOMATICALLY = 0,    ///<Assign participants to breakout rooms automatically.
    ASSIGN_PARTICIPANTS_MANUALLY = 1,         ///<Assign participants to breakout rooms manually.
    LET_PARTICIPANTS_CHOOSE_ROOM = 2          ///<Let participants choose breakout rooms.
};

/// \brief Breakout room options
///
struct BOOptions
{
    /// options support configure
    bool     isParticipantCanChooseRoom = false;                    ///<True indicates participants can choose room.
    bool     isParticipantCanReturnToMainSessionAtAnyTime = true;   ///<True indicates participants can return to main session at any time.
    bool     isAutoMoveAllAssignedParticipantsEnabled = false;      ///<True indicates automatically move all assigned participants into breakout rooms.
    bool     isBOTimerEnabled = false;                              ///<True indicates it's a timer BO.
    uint64_t boTimerDuration = 0;                                   ///<Seconds of BO timer duration, if BO timer is enabled, boTimerDuration is 30*60 seconds by default.
    bool     isNotifyMeWhenTimeIsUp = false;                        ///<True indicates will receive a notification when duration time is up. Otherwise BO is auto stopped.
    BO_STOP_COUNTDOWN countdownSeconds = BO_STOP_NOT_COUNTDOWN;     ///<BO stop countdown seconds, see \link BO_STOP_COUNTDOWN \endlink enum.

    /// options not support configure
    BO_STOP_COUNTDOWN defaultCountDown = BO_STOP_COUNTDOWN_SECONDS_60;  ///<BO default stop countdown seconds, see \link BO_STOP_COUNTDOWN \endlink enum.
    uint64_t          defaultBOTimerDuration = 30 * 60;                 ///<Default BO timer duration in seconds.
    bool              isPreAssignEnabled = false;                       ///<True indicates web enabled pre-assigned BO options when schedule the meeting.
    int               maxRoomCount = 100;                               ///<Max room count when creating BO.
};

/// \brief BO creator callback handler
///
class IBOCreatorHelperSink
{
public:
    virtual ~IBOCreatorHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Breakout room options notification. Options are only used for configure BO settings before BO starts.
     *
     * @param boOptions The breakout room options.
     */
    virtual void OnBOOptionsNotification(const BOOptions& boOptions) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Breakout room info list changed callback. If bo room count or name changed, you will receive this callback before BO starts.
     *
     * @param boRoomInfoList New bo room info list.
     */
    virtual void OnBORoomInfoListChanged(const std::vector<BreakoutRoomInfo>& boRoomInfoList) = 0;
};

/// \brief BO creator interface
///
class IBOCreatorHelper
{
public:
    virtual void RegisterSink(IBOCreatorHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Create breakout rooms.
     *
     * @param count Breakout rooms count.
     * @param type Assign participants type, see \link BO_ASSIGN_PARTICIPANTS_TYPE \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CreateBreakoutRooms(int count, BO_ASSIGN_PARTICIPANTS_TYPE type) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set the BO options.
     *
     * @param[in] boOptions The configured BO options.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetBOOptions(const BOOptions& boOptions) = 0;
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get current BO options.
     *
     * @param[out] boOptions Current BO options.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetBOOptions(BOOptions& boOptions) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Assign users to a BO.
     *
     * @param userGUIDList The user GUID list, can be one if assign single user to bo.
     * @param sessionBID The BO ID, empty string indicates to assign user back to main session.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AssignUsersToBO(const std::vector<std::string>& userGUIDList, const std::string& sessionBID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Rename the breakout room.
     *
     * @param sessionBID Specify the BID of the room to rename.
     * @param newBOName Specify the new BO room name.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RenameBreakoutRoom(const std::string& sessionBID, const std::string& newBOName) = 0;
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Add a breakout room.
     *
     * This will result in a default breakout room to be added.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AddBreakoutRoom() = 0;
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Delete breakout room.
     *
     * @param sessionBID Specify the BID of the room to delete.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeleteBreakoutRoom(const std::string& sessionBID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if web pre-assign BO options enabled.
     *
     * @param[out] enabled TRUE indicates web pre-assign BO options enabled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsWebPreAssignBOEnabled(bool& enabled) = 0;
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Recover breakout rooms to web pre-assigned options.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RecoverToWebPreAssignBO() = 0;
};

////////////////////////////////////////// IBOAdminHelper //////////////////////////////////////////

/// \brief BO admin callback handler
///
class IBOAdminHelperSink
{
public:
    virtual ~IBOAdminHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Receive help request from user in bo.
     *
     * @param userGUID The user guid.
     * @param boRoomInfo The bo room info.
     */
    virtual void OnBOHelpRequestReceived(const std::string& userGUID, const BreakoutRoomInfo& boRoomInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief BO end timer update notification, if BO enable timer, you will receive this notification when bo starts.
     *
     * @param remainingSecond The bo remaining time in seconds.
     */
    virtual void OnBOEndTimerUpdate(uint64_t remainingSecond) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief BO time is up notification, you can decide to stop BO immediately or later.
     */
    virtual void OnBOTimeIsUpNotification() = 0;
};

/// \brief BO admin interface
///
class IBOAdminHelper
{
public:
    virtual void RegisterSink(IBOAdminHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start all breakout rooms.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartBreakoutRooms() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Stop all breakout rooms.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopBreakoutRooms() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Assign a unassigned user to a BO, when BO is started.
     *
     * @param userGUID The user GUID.
     * @param sessionBID The BO ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MoveUserToRunningBO(const std::string& userGUID, const std::string& sessionBID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Host invite user return to main session, When BO is started and user is in BO.
     *
     * @param userGUID The user GUID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError InviteBOUserReturnToMainSession(const std::string& userGUID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Join breakout room by user help request.
     *
     * @param userGUID The user guid.
     * @param boRoomInfo The BO room info.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError JoinBOByUserRequest(const std::string& userGUID, const BreakoutRoomInfo& boRoomInfo) = 0;
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Ignore user help request.
     *
     * @param userGUID The user guid.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IgnoreUserHelpRequest(const std::string& userGUID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Broadcast message to all breakout rooms.
     *
     * @param message The message.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BroadcastMessageToBreakoutRoom(const std::string& message) = 0;
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if you can broadcast message to BO.
     *
     * @param[out] canBroadcast TRUE indicates you can broadcast message to BO.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CanBroadcastMessageToBO(bool& canBroadcast) = 0;
};

////////////////////////////////////////// IBOAssistantHelper //////////////////////////////////////////

/// \brief BO assistant interface.
///
class IBOAssistantHelper
{
public:

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Join breakout room.
     *
     * @param sessionBID Specify the BID of the room to join.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError JoinBreakoutRoom(const std::string& sessionBID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Leave breakout room.
     *
     * @param sessionBID Specify the BID of the room to leave.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LeaveBreakoutRoom(const std::string& sessionBID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Response host invite to main session when receiving OnHostInviteReturnToMainSession.
     *
     * @param accept TRUE indicates to return to main session. Otherwise ignore.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ResponseHostInviteToMainSession(bool accept) = 0;
};

////////////////////////////////////////// IBOAttendeeHelper //////////////////////////////////////////

/*! \enum BO_HELP_ATTENDEE_RESULT
    \since The enum is supported on Zoom Rooms clients with version 6.0.0 or later.
    \brief BO help attendee result.
    Here are more detailed structural descriptions.
 */
enum BO_HELP_ATTENDEE_RESULT
{
    BO_HELP_ATTENDEE_RESULT_UNKNOWN = -1,             ///<Invalid
    BO_HELP_ATTENDEE_RESULT_ACCEPT = 0,               ///<Host accept attendee's help request.
    BO_HELP_ATTENDEE_RESULT_BUSY = 1,                 ///<Host is handling other's request with the request dialog.
    BO_HELP_ATTENDEE_RESULT_IGNORE = 2,               ///<Host click "later" button or close the request dialog directly.
    BO_HELP_ATTENDEE_RESULT_HOST_ALREADY_IN_BO = 3    ///<Host already in your BO meeting
};

/// \brief BO attendee callback handler.
///
class IBOAttendeeHelperSink
{
public:
    virtual ~IBOAttendeeHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Help attendee result.
     *
     * @param result The help attendee result, see \link BO_HELP_ATTENDEE_RESULT \endlink enum.
     */
    virtual void OnBOHelpAttendeeResult(BO_HELP_ATTENDEE_RESULT result) = 0;
};

/// \brief BO attendee interface
///
class IBOAttendeeHelper
{
public:
    virtual void RegisterSink(IBOAttendeeHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Join the assigned breakout room.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError JoinBreakoutRoom() = 0;
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Leave breakout room.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LeaveBreakoutRoom() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Ask host for help when you are in need in BO.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AskHostForHelpInBO() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Determine if host is in the BO which attendee is assigned to.
     *
     * @param[out] in TRUE indicates host is in current BO.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsHostInThisBO(bool& in) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Determine if participant can return to main session.
     *
     * @param[out] canReturn TRUE indicates can return to main session.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsCanReturnMainSession(bool& canReturn) = 0;
};

////////////////////////////////////////// IBODataHelper //////////////////////////////////////////

/// \brief BO data callback handler.
///
class IBODataHelperSink
{
public:
    virtual ~IBODataHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Breakout room user status changed notification.
     *
     * @param boUserStatus New BO user status, see \link BO_USER_STATUS \endlink enum.
     * @param boRoomInfo New BO room info.
     */
    virtual void OnBOUserStatusChanged(BO_USER_STATUS boUserStatus, const BreakoutRoomInfo& boRoomInfo) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief BO participants update notification.
     *
     * @param participants The bo participants.
     * @param totalParticipantsCount Total count of participants.
     * @param needCleanUpUserList TRUE indicates this is a initial notification, you should clean up former user list before processing here participants.
     * If there are many participants in meeting, the initial meeting participants will be updated in bulks.
     *
     * @deprecated Please use IParticipantHelperSink::OnInitMeetingParticipants(const std::vector<MeetingParticipant>&, int32\_t , bool, ConfSessionType)
     */
    virtual void OnInitBOParticipants(const std::vector<MeetingParticipant>& participants, int32_t totalParticipantsCount, bool needCleanUpUserList) = 0;

     /**
      * @since 6.0.0
      * @zrminver 6.0.0
      *
      * @brief BO participants join meeting notification.
      *
      * @param participants List of join participants.
      *
      * @deprecated Please use IParticipantHelperSink::OnUserJoin(const std::vector<MeetingParticipant>& , ConfSessionType)
      */
    virtual void OnBOUserJoin(const std::vector<MeetingParticipant>& participants) = 0;

     /**
      * @since 6.0.0
      * @zrminver 6.0.0
      *
      * @brief BO participants leave meeting notification,
      *
      * @param participants List of left participants.
      *
      * @deprecated Please use IParticipantHelperSink::OnUserLeave(const std::vector<MeetingParticipant>& , ConfSessionType)
      */
    virtual void OnBOUserLeave(const std::vector<MeetingParticipant>& participants) = 0;

     /**
      * @since 6.0.0
      * @zrminver 6.0.0
      *
      * @brief BO participants update notification.
      *
      * @param participants List of update participants.
      *
      * @deprecated Please use IParticipantHelperSink::OnUserUpdate(const std::vector<MeetingParticipant>& , ConfSessionType)
      */
    virtual void OnBOUserUpdate(const std::vector<MeetingParticipant>& participants) = 0;
};

/// \brief BO data interface
///
class IBODataHelper
{
public:
    virtual void RegisterSink(IBODataHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query self breakout room user status.
     *
     * @param[out] boUserStatus Self bo user status, see \link BO_USER_STATUS \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetBOUserStatus(BO_USER_STATUS& boUserStatus) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get all breakout room info list.
     *
     * @param[out] boRoomList All bo room info list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetBreakoutRoomList(std::vector<BreakoutRoomInfo>& boRoomList) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get all breakout rooms' user list.
     *
     * The BO user list contains all users of breakout rooms, including main session. Check user breakout room status with MeetingParticipant.BreakoutRoomStatus
     *
     * @param[out] userList All breakout rooms' user list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetBreakoutRoomUserList(std::vector<MeetingParticipant>& userList) = 0;
};

////////////////////////////////////////// IBreakoutRoomHelper //////////////////////////////////////////

/// \brief Breakout Room helper event callback
///
class IBreakoutRoomHelperSink
{
public:
    virtual ~IBreakoutRoomHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Breakout room status changed notification.
     *
     * @param boStatus New BO status, see \link BO_STATUS \endlink enum.
     */
    virtual void OnBOStatusChanged(BO_STATUS boStatus) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Receive switch to breakout room request.
     *
     * Whenever the host switches you to another BO while you are assigned but haven't joined the BO, you will receive this event.
     *
     * @param fromUserName The request from user's name, empty string indicates from host.
     * @param newBORoomInfo The new BO room info.
     */
    virtual void OnBOSwitchRequestReceived(const std::string& fromUserName, const BreakoutRoomInfo& newBORoomInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Host invite you to return to main session notification.
     *
     * @param fromUserName The request from user's name, empty string indicates from host.
     */
    virtual void OnHostInviteReturnToMainSession(const std::string& fromUserName) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief BO stop countdown, if BO enable countdown, you will receive this notification when bo is stopping, see \link BO_STOP_COUNTDOWN \endlink enum.
     *
     * @param remainingSeconds The bo stop countdown remaining time in seconds.
     */
    virtual void OnBOStopCountDown(uint64_t remainingSeconds) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start to join BO notification.
     */
    virtual void OnStartJoinBONotification() = 0;
};

/// \brief Breakout Room helper interface
///
class IBreakoutRoomHelper
    : public IBOCreatorHelper
    , public IBOAdminHelper
    , public IBOAssistantHelper
    , public IBOAttendeeHelper
    , public IBODataHelper
{
public:
    virtual ~IBreakoutRoomHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register breakout room help callback sink.
     *
     * @param pSink A pointer to the IBreakoutRoomHelperSink that receives the breakout room help callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IBreakoutRoomHelperSink* pSink) = 0;
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister breakout room help callback sink.
     *
     * @param pSink A pointer to the IBreakoutRoomHelperSink that receives the breakout room help callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IBreakoutRoomHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the pointer of BO creator object.
     *
     * @return If the function succeeds, the return value is a pointer to @ref IBOCreatorHelper object. For more details, see \link IBOCreatorHelper \endlink.
     * Otherwise failed, the return value is NULL.
     */
    virtual IBOCreatorHelper* GetBOCreatorHelper() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the pointer of BO administrator object.
     *
     * @return If the function succeeds, the return value is a pointer to @ref IBOAdminHelper object. For more details, see \link IBOAdminHelper \endlink.
     * Otherwise failed, the return value is NULL.
     */
    virtual IBOAdminHelper* GetBOAdminHelper() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the pointer of BO assistant object.
     *
     * @return If the function succeeds, the return value is a pointer to @ref IBOAssistantHelper object. For more details, see \link IBOAssistantHelper \endlink.
     * Otherwise failed, the return value is NULL.
     */
    virtual IBOAssistantHelper* GetBOAssistantHelper() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the pointer of BO attendee object.
     *
     * @return If the function succeeds, the return value is a pointer to @ref IBOAttendeeHelper object. For more details, see \link IBOAttendeeHelper \endlink.
     * Otherwise failed, the return value is NULL.
     */
    virtual IBOAttendeeHelper* GetBOAttendeeHelper() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the pointer of BO data object.
     *
     * @return If the function succeeds, the return value is a pointer to @ref IBODataHelper object. For more details, see \link IBODataHelper \endlink.
     * Otherwise failed, the return value is NULL.
     */
    virtual IBODataHelper* GetBODataHelper() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Determine if the BO feature is enabled in current meeting.
     *
     * @param[out] enabled TRUE indicates breakout room is enabled in this meeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsBOEnabled(bool& enabled) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Determine if the BO is started or not.
     *
     * @param[out] started TRUE indicates breakout room is started or stopping, see \link BO_STATUS \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsBOStarted(bool& started) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if you are in main session.
     *
     * @param[out] isInMainSession TRUE indicates you are in main session.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsInMainSession(bool& isInMainSession) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
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

#endif    //ZRC_IBREAKOUTROOMHELPER_H

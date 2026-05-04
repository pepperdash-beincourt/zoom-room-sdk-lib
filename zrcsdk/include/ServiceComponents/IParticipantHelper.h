/*!
 * \file IParticipantHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Meeting Participant interfaces.
 */
#ifndef ZRC_IPARTICIPANTHELPER_H
#define ZRC_IPARTICIPANTHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum ZRWUserChangeType
    \brief Zoom Room Whiteboard as a user in meeting, change type.
    Here are more detailed structural descriptions.
 */
enum ZRWUserChangeType
{
    ZRW_JOIN = 0,     ///<ZRW join meeting.
    ZRW_LEAVE = 1,    ///<ZRW leave meeting.
};

/*! \enum ClaimHostResult
    \brief Claim host result
    Here are more detailed structural descriptions.
 */
enum ClaimHostResult
{
    ClaimHostResultSuccess = 0,           ///<Success
    ClaimHostResultInvalidHostKey = 1,    ///<Fail, invalid host key.
    ClaimHostResultUnknownError = 2       ///<Fail, unclassified error.
};

/*! \enum ReportIssueType
    \brief Report issue type.
    Here are more detailed structural descriptions.
 */
enum ReportIssueType
{
    ISSUE_TYPE_OFFENSIVE_ILLEGAL_ABUSIVE = 0x40,
    ISSUE_TYPE_SUICIDE_SELF_HARM = 0x80,
    ISSUE_TYPE_PRIVATE_INFORMATION = 0x100,
    ISSUE_TYPE_SPAM = 0x200,
    ISSUE_TYPE_COPYRIGHT_TRADEMARK_INFRINGEMENT = 0x400,
    ISSUE_TYPE_IMPERSONATION = 0x800,
    ISSUE_TYPE_ILL_TELL_YOU_LATER = 0x1
};

/*! \enum ConfSessionType
    \brief Conf session type
 ZR supports various scenarios, such as breakout rooms in normal meetings and backstage in webinars (all depending on web option).
 In these cases, ZR can connect to multiple conference instances simultaneously (e.g., session1, session2, etc.).As a result, datas of
 a participant may appear in more than one session at the same time.

 To streamline participant management, two logical session types are defined: Master and Current.

    Current: The Current session refers to the specific session in which you are actively participating.
    Master:  The Master session represents the overarching meeting that encompasses all sub-sessions.

 If you have the appropriate permissions and API support, the SDK can manage the current session and all sessions through the data
 from 'Current' and 'Master' respectively (note: If there is only one conference instance, the Master is the same as the Current ).
*/
enum ConfSessionType
{
    CurrentSession = 1,    ///<The specific session in which you are actively participating.
    MasterSession = 2,     ///<The overarching meeting that encompasses all sub-sessions.
};

/// \brief Meeting Participant helper event callback.
///
class IParticipantHelperSink
{
public:
    virtual ~IParticipantHelperSink() {}

    /**
     * @since 6.5.0
     * @zrminver 6.0.0
     *
     * @brief Meeting participants update notification.
     *
     * @param participants All the meeting participants.
     * @param totalParticipantsCount Total count of participants.
     * @param needCleanUpUserList TRUE indicates this is a initial notification, you should clean up former user list before processing here participants.
     * @param session session that need init.
     * 
     * If there are many participants in meeting, the initial meeting participants will be updated in bulks.
     */
    virtual void OnInitMeetingParticipants(const std::vector<MeetingParticipant>& participants, int32_t totalParticipantsCount,
                                           bool needCleanUpUserList, ConfSessionType session) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.0.0
     *
     * @brief Participants join meeting notification.
     *
     * @param participants List of join participants.
     * @param session session that changed.
     */
    virtual void OnUserJoin(const std::vector<MeetingParticipant>& participants, ConfSessionType session) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.0.0
     *
     * @brief Participants leave meeting notification,
     * @param participants List of left participants.
     * @param session session that changed.
     */
    virtual void OnUserLeave(const std::vector<MeetingParticipant>& participants, ConfSessionType session) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.0.0
     *
     * @brief Participants update notification.
     *
     * @param participants List of info updated participants.
     * @param session session that changed.
     */
    virtual void OnUserUpdate(const std::vector<MeetingParticipant>& participants, ConfSessionType session) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Host changed notification.
     *
     * @param hostUserID Current host user ID.
     * @param amIHost TRUE indicates self is host. Otherwise not.
     * @param session session that changed.
     *
     * @changelog
     *  - 6.5.0: Added a parameter ConfSessionType to get specific type of participant list.
     */
    virtual void OnHostChangedNotification(int32_t hostUserID, bool amIHost, ConfSessionType session) = 0;

    /**
     * @since 6.3.0
     * @zrminver 6.0.0
     *
     * @brief Participants changed notification, using GetParticipantsInMeeting with session to get latest list .
     *
     * @param session The session that changed.
     */
    virtual void OnMeetingParticipantsChanged(ConfSessionType session) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Hide profile picture update notification.
     *
     * @param isHideProfilePictures TRUE indicates hide. Otherwise not.
     */
    virtual void OnUpdateHideProfilePictures(bool isHideProfilePictures) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Hide full room view notification
     *
     * @param userIDs Full room view hidden user ID list.
     */
    virtual void OnHideFullRoomViewNotification(const std::vector<int32_t>& userIDs) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Claim host notification.
     *
     * @param result See \link ClaimHostResult \endlink enum.
     */
    virtual void OnClaimHostNotification(ClaimHostResult result) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow user annotate on shared content notification.
     *
     * @param support TRUE indicates support control sharing annotation enable or not.
     * @param enable TRUE indicates allow user to annotate on shared content.
     */
    virtual void OnUpdateSharingAnnotationInfo(bool support, bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow attendees rename themselves notification.
     *
     * @param allow TRUE indicate allow attendees rename themselves. Otherwise not.
     */
    virtual void OnAllowAttendeesRenameThemselvesNotification(bool allow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow attendees share whiteboard notification.
     *
     * @param isSupported TRUE indicate control allow attendees share whiteboards is supported. Otherwise not.
     * @param isAllowed TRUE indicate attendees are allowed to share whiteboard. Otherwise not.
     */
    virtual void OnAllowAttendeesShareWhiteboardsNotification(bool isSupported, bool isAllowed) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow raise hande for attendee notification.
     *
     * @param canRaiseHandForAttendee TRUE indicate allow attendees rename themselves. Otherwise not.
     */
     virtual void OnAllowRaiseHandForAttendeeNotification(bool canRaiseHandForAttendee) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Zoom Room Whiteboard user change notification.
     *
     * @param type Change type, see \link ZRWUserChangeType \endlink enum.
     * @param zrwUserID The zrw user ID if @ref ZRW_JOIN.
     */
    virtual void OnUpdateOnZRWUserChangeNotification(ZRWUserChangeType type, int32_t zrwUserID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Remote control admin exist update notification.
     *
     * @param isAdminExist TRUE indicate remote control admin exist.
     */
    virtual void OnUpdateHasRemoteControlAdmin(bool isAdminExist) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Remote control assistant exist update notification.
     *
     * @param isAssistantExist TRUE indicate remote control assistant exist.
     */
    virtual void OnUpdateHasRemoteControlAssistant(bool isAssistantExist) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Download result.
     *
     * @param localFilePath The specified local file path when calling download API.
     * @param result [0] success. Otherwise failed.
     */
    virtual void OnDownloadingFinished(const std::string& localFilePath, uint32_t result) = 0;

    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Show participant local time notification. Triggered when the display state of participant local time changes.
     *
     * @param isShowing TRUE indicates participant local time is being displayed. Otherwise not.
     */
    virtual void OnShowParticipantLocalTimeNotification(bool isShowing) = 0;
};

/// \brief Meeting Participant helper interface.
///
class IParticipantHelper
{
public:
    virtual ~IParticipantHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting user helper callback sink.
     *
     * @param pSink A pointer to the IParticipantHelperSink that receives the meeting user helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IParticipantHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting user helper callback sink.
     *
     * @param pSink A pointer to the IParticipantHelperSink that receives the meeting user helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IParticipantHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get participants in meeting.
     *
     * @param[out] participants In meeting participant list.
     * @param[in] session The session that needed.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.3.0: Added a parameter ConfSessionType to get specific type of participant list.
     */
    virtual ZRCSDKError GetParticipantsInMeeting(std::vector<MeetingParticipant>& participants, ConfSessionType session) = 0;

    /**
     * @since 6.3.0
     * @zrminver 6.0.0
     *
     * @brief Get virtual participants in meeting (e.g., multi stream, multi camera, etc.).
     *
     * @param[out] participants In meeting virtual participant list.
     * @param[in] session The session that needed.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetVirtualParticipantsInMeeting(std::vector<MeetingParticipant>& participants, ConfSessionType session) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get participants in master session who are in silent mode.
     *
     * @param[out] participants In silent mode participant list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.3.0: Only works for getting participants in silent mode of master session.
     */
    virtual ZRCSDKError GetParticipantsInSilentMode(std::vector<MeetingParticipant>& participants) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get participants in master session who have left meeting.
     *
     * @param[out] participants Left meeting participant list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetParticipantsLeftMeeting(std::vector<MeetingParticipant>& participants) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Change host to the assigned user.
     *
     * @param userID Specify the user ID to change host to.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AssignHost(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Make user become co-host.
     *
     * @param userID Specify the user ID to make co-host.
     * @param assign TRUE indicates make user become co-host. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AssignCohost(int32_t userID, bool assign) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Claim the role of the host.
     *
     * @param key The key to get the role of host.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ClaimHost(const std::string& key) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable attendees annotate on shared content.
     *
     * @param enable TRUE indicates to enable attendees annotate on shared content.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableAttendeesAnnotateOnShare(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Rename user.
     *
     * @param userID Specify the user ID to rename.
     * @param name Specify the name rename to.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RenameUser(int32_t userID, const std::string& name) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow attendees rename themselves.
     *
     * @param allow TRUE indicate allow attendees rename themselves. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowAttendeesRenameThemselves(bool allow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query is attendees rename themselves enabled.
     *
     * @param[out] enable TRUE indicates attendees can rename themselves.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAttendeesRenameThemselvesEnabled(bool& enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query is attendees rename themselves locked.
     *
     * @param[out] locked TRUE indicates attendees rename themselves is locked.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAttendeesRenameThemselvesLocked(bool& locked) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query is attendees rename themselves allowed.
     *
     * @param[out] allow TRUE indicates attendees rename themselves is allowed.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAttendeesRenameThemselvesAllowed(bool& allow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow webinar attendees raise hand.
     *
     * @param allow TRUE indicate allow webinar attendees raise hand. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowWebinarAttendeeRaiseHand(bool allow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Raise hands in the meeting.
     *
     * @param raise TRUE indicates to raise self hand. FALSE indicates to lower self hand.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RaiseHand(bool raise) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Lower user hand.
     *
     * @param userID Specify the user ID to put down the hands.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LowerUserHand(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Cancel all hands raised.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LowerAllHands() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Lower all attendees' hands.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LowerAllAttendeesHands() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Expel user.
     *
     * @param userID Specify the user ID to expel user.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ExpelUser(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Expel users.
     *
     * @param userIDs Specify the user ID list to expel users.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ExpelUsers(const std::vector<int32_t>& userIDs) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Hide profile pictures.
     *
     * @param hidden TRUE indicates to hide. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError HideProfilePictures(bool hidden) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if user supports hide full room view
     *
     * @param[out] isAvailable TRUE indicates user support hide/show full room view.
     * @param[in] userID The userID
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsFullRoomViewAvailableForUser(bool& isAvailable, int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Hide full room view
     *
     * @param isHide TRUE indicates to hide full room view. Otherwise not.
     * @param userID The Room's userID
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError HideFullRoomView(bool isHide, int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Download user avatar.
     *
     * @param avatarUrl The avatar url.
     * @param localFilePath Specify the local file path of this avatar. For example, ..../MyAvatarFolder/this_avatar.jpg
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DownloadUserAvatar(std::string avatarUrl, std::string localFilePath) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow attendees share whiteboards.
     *
     * @param allow TRUE indicate allow attendees share whiteboards. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowAttendeesShareWhiteboards(bool allow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Suspend participants activities.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SuspendParticipantsActivities() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Report meeting participants issues.
     *
     * @param userIDs Participants' ID list
     * @param type Choose the report issue type, bitset see \link ReportIssueType \endlink enum.
     * @param email Receive response notification email.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     * - 6.7.0: Support report in waiting room, in this case userIDs can be empty
     */
    virtual ZRCSDKError ReportIssue(const std::vector<int32_t>& userIDs, int32_t type, const std::string& email) = 0;
        
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Set myself as active speaker.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetMySelfAsActiveSpeaker() = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Set my child (who's parentUserID equal to my userID) as active speaker.
     *
     * @param[in] userID The child's userID
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetMyChildAsActiveSpeaker(int32_t userID) = 0;

    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Show or hide participant local time display. Requires web master switch "Enable show participants local time" to be enabled.
     *
     * @param show TRUE indicates to show participant local time. FALSE indicates to hide.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowParticipantLocalTime(bool show) = 0;
};

NS_ZRCSDK_END
#endif    //ZRC_IPARTICIPANTHELPER_H

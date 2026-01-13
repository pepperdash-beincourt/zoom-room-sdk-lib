/*!
 * \file IMeetingWebinarHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Meeting Webinar interfaces.
 */
#ifndef ZRC_IMEETINGWEBINARHELPER_H
#define ZRC_IMEETINGWEBINARHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum WebinarRoleChangedState
    \brief Webinar role changed state.
    Here are more detailed structural descriptions.
 */
enum WebinarRoleChangedState
{
    WebinarRoleChangedNone = 0,    ///<Invalid
    WebinarRoleChangedPromote,     ///<You are promoted to panelist.
    WebinarRoleChangedDemote       ///<You are demoted to attendee.
};

/*! \enum DynamicWebinarAttendeeListType
    \brief Dynamic list webinar attendee type.
    Here are more detailed structural descriptions.
 */
enum DynamicWebinarAttendeeListType
{
    DynamicWebinarAttendeeListType_Unknown = 0,                  ///<Invalid
    DynamicWebinarAttendeeListType_ATTENDEE_LIST,                ///<Dynamic list attendee list.
    DynamicWebinarAttendeeListType_UNENCRYPTED_ATTENDEE_LIST,    ///<Dynamic list unencrypted attendee list.
};

/// \brief Webinar info.
///
struct WebinarInfo
{
    bool isViewOnlyCanTalk = false;    ///<TRUE indicates webinar attendee can talk.
};

/// \brief Webinar attendee base info
///
struct WebinarAttendeeBaseInfo
{
    int32_t attendeeCount = 0;    ///<Attendees count.
    int32_t raisedCount = 0;      ///<Attendees raised hand count.
};

/// \brief Dynamic webinar attendee list response.
/// For unencrypted attendee list and index is 0, should clean up local list before add participants.
struct DynamicWebinarAttendeeListRes
{
    DynamicWebinarAttendeeListType attendeeListType = DynamicWebinarAttendeeListType_Unknown;    ///<List type, see \link DynamicWebinarAttendeeListType \endlink enum.

    int32_t                         result = 0;              ///<Result, [0] for success.
    std::string                     keywords;                ///<Search filter keywords. If it is an empty string indicates that the attendees are at most the first 100 attendees. Otherwise, attendees are the search result.
    int32_t                         totalOfAttendees = 0;    ///<Total count of attendees.
    std::vector<MeetingParticipant> attendees;               ///<Response attendee list.
    int32_t                         index = 0;               ///<Current attendee list start index.
};

/// \brief Webinar participants count info.
///
struct WebinarParticipantCountInfo
{
    int32_t panelistCount = 0;    ///<Panelists count.
    int32_t attendeeCount = 0;    ///<Attendees count.
};

/// \brief Webinar unencrypted info.
///
struct WebinarUnencryptedInfo
{
    int32_t unencryptedAttendeeCount = 0;     ///<Unencrypted attendees count.
    int32_t unencryptedPhoneUserCount = 0;    ///<Unencrypted phone users count.
};

/// \brief Backstage error.
///
enum BackstageError
{
    BackstageErrorNone = 0,                     ///<None
    BackstageErrorFailToJoinBackstage,          ///<Fail to join backstage
    BackstageErrorFailToJoinWebinar,            ///<Fail to join webinar
    BackstageErrorFailToMovePanelistToWebinar,  ///<Fail to move panelist to webinar
    BackstageErrorFailToMovePanelistToBackstage ///<Fail to move panelist to backstage
};

/// \brief Backstage status.
///
enum BackstageStatus
{
    BackstageStatusUnknown = -1,            ///<Unknown
    BackstageStatusInBackstage = 0,         ///<In backstage
    BackstageStatusInWebinar,               ///<In webinar
    BackstageStatusInDebriefSession,        ///<In debrief session
    BackstageStatusJoiningBackstage,        ///<Joining backstage
    BackstageStatusJoiningWebinar,          ///<Joining webinar
    BackstageStatusJoiningDebriefSession    ///<Joining debrief session
};

/// \brief Backstage status.
///
struct BackstageInfo
{
    BackstageStatus status = BackstageStatusUnknown;                        ///<Backstage status, see \link BackstageStatus \endlink enum,
    float           webinarVolume = 0;                                      ///<Webinar volume
    bool            isShowingWebinarLiveFeed = false;                       ///<TRUE indicates is showing webinar live feed.
    bool            canSwitchBetweenBackstageContentAndLiveFeed = false;    ///<TRUE indicates it is allowed to switch between backstage content and live feed.
    bool            canPanelistMoveBetweenWebinarAndBackstage = false;      ///<TRUE indicates panelists can move between webinar and backstage.
    bool            showVideoPreview = false;                               ///<TRUE indicates to show video preview.
    NewJoinFlowInfo videoPreviewInfo;                                       ///<Video preview info.
};

/// \brief Backstage notification.
///
struct BackstageNotification
{
    BackstageInfo      backstageInfo;                   ///<The backstage info.
    BackstageError     errorType = BackstageErrorNone;  ///<Error type, see \link BackstageError \endlink enum.
    int32_t            errorCode = 0;                   ///<Error code
};

/// \brief Simulive webinar info.
/// This struct is valid only if the meeting is simulive webinar which supports go live notification.
struct SimuliveWebinarInfo
{
    bool   isInSimuliveStatus = false;           ///<TRUE indicates it is currently in simulive status. Otherwise not.
    time_t recordingEndTimeStampInSecond = 0;    ///<The recording end time stamp, it is valid when the isInSimuliveStatus is true.
};

/// \brief Meeting Webinar helper event callback
///
class IMeetingWebinarHelperSink
{
public:
    virtual ~IMeetingWebinarHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Webinar practice session notification
     *
     * @param inWebinarPracticeSession TRUE indicates you are in webinar practice session.
     */
    virtual void OnWebinarPracticeSessionNotification(bool inWebinarPracticeSession) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Webinar info update notification.
     *
     * @param info Webinar info.
     */
    virtual void OnUpdateWebinarInfo(const WebinarInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Webinar role changed notification.
     *
     * @param roleChangedState Role changed state, see \link WebinarRoleChangedState \endlink enum.
     */
    virtual void OnWebinarRoleChangedNotification(WebinarRoleChangedState roleChangedState) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Panelist reach maximum capacity notification.
     *
     * @param maximumCapacity The maximum capacity.
     */
    virtual void OnPanelistReachMaximumCapacityNotification(int32_t maximumCapacity) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Webinar attendee base info notification
     *
     * @param baseInfo The webinar attendee base info.
     */
    virtual void OnWebinarAttendeeBaseInfoNotification(const WebinarAttendeeBaseInfo& baseInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Dynamic webinar attendee list response.
     *
     * @param response The dynamic webinar attendee list response.
     */
    virtual void OnDynamicWebinarAttendeeListResponse(const DynamicWebinarAttendeeListRes& response) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Need promote attendee to allow talking notification.
     *
     * This happens when the user is using an older version of Zoom.
     *
     * @param userID The attendee userID
     * @param userName The attendee userName.
     */
    virtual void OnNeedPromoteAttendeeToAllowTalkingNotification(int32_t userID, const std::string& userName) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow attendee view participant count notification.
     *
     * @param allow TRUE indicates attendees are now allowed to view participant count.
     */
    virtual void OnAllowAttendeeViewParticipantCountNotification(bool allow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Webinar attendee count info notification
     *
     * @param countInfo The webinar attendee count info.
     */
    virtual void OnWebinarParticipantCountNotification(const WebinarParticipantCountInfo& countInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Webinar unencrypted info notification in end-to-end meeting.
     *
     * @param unencryptedInfo The webinar unencrypted info.
     */
    virtual void OnWebinarUnencryptedInfoNotification(const WebinarUnencryptedInfo& unencryptedInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Backstage  notification
     *
     * @param noti The notification.
     */
    virtual void OnBackstageNotification(const BackstageNotification& noti) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Backstage info update notification
     *
     * @param info The backstage info.
     */
    virtual void OnBackstageInfoNotification(const BackstageInfo& info) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Production studio notification
     *
     * @param isProducerPublishing TRUE indicates is producer publishing, can change confidence monitor layout to "Production Studio".
     */
    virtual void OnProductionStudioNotification(bool isProducerPublishing) = 0;

    /**
     * @since 6.7.0
     * @zrminver 6.6.0
     *
     * @brief Simulive webinar info notification. This sink will be triggered only when the simulive webinar supports go live.
     *
     * @param info Simulive webinar info.
     */
    virtual void OnSimuliveWebinarInfoNotification(const SimuliveWebinarInfo& info) = 0;
};

/// \brief Meeting Webinar helper interface
///
class IMeetingWebinarHelper
{
public:
    virtual ~IMeetingWebinarHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting webinar helper callback sink.
     *
     * @param pSink A pointer to the IMeetingWebinarHelperSink that receives the meeting webinar helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingWebinarHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting webinar helper callback sink.
     *
     * @param pSink A pointer to the IMeetingWebinarHelperSink that receives the meeting webinar helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingWebinarHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start webinar from practice session.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartWebinarFromPracticeSession() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if you are in webinar practice session.
     *
     * @param[out] inPracticeSession TRUE indicates you are in webinar practice session.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsInWebinarPracticeSession(bool& inPracticeSession) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Promote attendee to panelist.
     *
     * @param userID Specify the user ID to promote.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PromoteAttendeeToPanelist(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Demote panelist to attendee.
     *
     * @param userID Specify the user ID to demote.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DemotePanelistToAttendee(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow webinar attendee talk
     *
     * @param userID Specify the user ID to allow talking.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowWebinarAttendeeTalk(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Disallow webinar attendee talk
     *
     * @param userID Specify the user ID to disallow talking.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DisallowWebinarAttendeeTalk(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow webinar attendee view participants count
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowWebinarAttendeeViewParticipantCount() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Disallow webinar attendee view participants count
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DisallowWebinarAttendeeViewParticipantCount() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief List unencrypted webinar attendees
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ListUnencryptedWebinarAttendees() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief List webinar attendees.
     *
     * If keywords are empty, this api will request the first 100 attendees. Otherwise, this api will request attendees filtered by the keywords.
     *
     * @param keywords The search keywords.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ListWebinarAttendees(const std::string& keywords) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get current attendee list.
     *
     * The API only contains part of all the attendees. It may the first 100 attendees or the keywords filtered attendees you last request by ListWebinarAttendees.
     *
     * @param[out] attendeeList The attendee list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetCurrentAttendeeList(std::vector<MeetingParticipant>& attendeeList) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Move all users from webinar into backstage
     *
     * @param includeSelf TRUE indicates include ZR self.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MoveAllToBackstage(bool includeSelf) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Move all users from backstage into webinar
     *
     * @param includeSelf TRUE indicates include ZR self.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MoveAllToWebinar(bool includeSelf) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Move selected users from webinar into backstage
     *
     * @param userIDs User IDs. Using ID in IParticipantHelper.GetParticipantsInMeeting to move participant.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MoveToBackstage(const std::vector<int32_t>& userIDs) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Move selected users from backstage into webinar
     *
     * @param userIDs User IDs. Using ID in IParticipantHelper.GetParticipantsInMeeting to move participant.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MoveToWebinar(const std::vector<int32_t>& userIDs) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if can go to webinar
     *
     * @param[out] canGo TRUE indicates can go.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CanGoToWebinar(bool &canGo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if can go to backstage
     *
     * @param[out] canGo TRUE indicates can go.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CanGoToBackstage(bool &canGo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Go to backstage.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GoToBackstage() = 0;
  
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Go to webinar.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GoToWebinar() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief End webinar for attendees and send all panelists to a private debriefing session.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EndWebinarForAttendees() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set webinar volume.
     *
     * @param webinarVolume The volume.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetWebinarVolume(float webinarVolume) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow panelist move between webinar and backstage
     *
     * @param allowMove TRUE indicates to allow move.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowPanelistMoveBetweenWebinarAndBackstage(bool allowMove) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if needs to show video preview before going to webinar.
     *
     * @param[out] needShow TRUE indicates needs to show video preview before going to webinar.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError NeedShowVideoPreviewBeforeGoToWebinar(bool& needShow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show video preview
     *
     * @param showVideoPreview TRUE indicates to show video preview. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowVideoPreview(bool showVideoPreview) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update media status in video preview before you enter into webinar.
     *
     * @param audioOn TRUE indicates to turn on audio. Otherwise not.
     * @param videoOn TRUE indicates to turn on video. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UpdateMediaStatusInVideoPreview(bool audioOn, bool videoOn) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if can switch between webinar live feed and backstage content.
     *
     * @param[out] canSwitch TRUE indicates can switch between contents.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CanSwitchBetweenBackstageContentAndLiveFeed(bool& canSwitch) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Switch between webinar live feed and backstage content.
     *
     * @param showWebinarLiveFeed TRUE indicates to switch to webinar live feed. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SwitchWebinarLiveFeed(bool showWebinarLiveFeed) = 0;

    /**
     * @since 6.7.0
     * @zrminver 6.6.0
     *
     * @brief Transition simulive webinar to live session.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SimuliveWebinarGoLive() = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_IMEETINGWEBINARHELPER_H

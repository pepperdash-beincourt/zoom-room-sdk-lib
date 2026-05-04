/*!
 * \file IMeetingListHelper.h
 * \since Added in 6.0.0
 * \brief Meeting List interfaces.
 */
#ifndef ZRC_IMEETINGLISTHELPER_H
#define ZRC_IMEETINGLISTHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/// \brief Schedule calendar event param
///
struct ScheduleCalendarEventParam
{
    std::string topic;        ///<Event topic
    std::string password;     ///<Event password
    std::string startTime;    ///<Event start time, data format see \link ZRCSDK_MEETING_DATE_FORMAT \endlink enum.
    std::string endTime;      ///<Event end time, data format see \link ZRCSDK_MEETING_DATE_FORMAT \endlink enum.

    std::vector<std::string> attendees;    ///<Invited attendees, using email.

    bool enableWaitingRoom = false;    ///<TRUE indicates meeting enable waiting room.
};

/// \brief Meeting List helper event callback
///
class IMeetingListHelperSink
{
public:
    virtual ~IMeetingListHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Meeting list changed callback.
     *
     * @param result List meeting result, see \link ListMeetingResult \endlink enum.
     * @param meetingList The updated meeting list.
     */
    virtual void OnUpdateMeetingList(ListMeetingResult result, const std::vector<MeetingItem>& meetingList) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Schedule calendar event result notification.
     *
     * @param scheduleResult Schedule calendar event result, see \link ScheduleCalendarEventResult \endlink enum.
     */
    virtual void OnUpdatedScheduleCalendarEventNotification(ScheduleCalendarEventResult scheduleResult) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Delete calendar event result notification.
     *
     * @param deleteResult Delete calendar event result, see \link DeleteCalendarEventResult \endlink enum.
     */
    virtual void OnUpdatedDeleteCalendarEventNotification(DeleteCalendarEventResult deleteResult) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show upcoming meeting alert result.
     *
     * @param result [0] Success
     * @param meetingItem Meeting item
     */
    virtual void OnShowUpcomingMeetingAlertResult(int32_t result, const MeetingItem& meetingItem) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Close upcoming meeting alert result.
     *
     * @param result [0] Success
     */
    virtual void OnCloseUpcomingMeetingAlertResult(int32_t result) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.1.5
     *
     * @brief Meeting will auto release notification
     *
     * @param meetingItem Meeting item
     */
    virtual void OnMeetingWillReleaseAutomatically(const MeetingItem& meetingItem) = 0;
};

/// \brief Meeting List helper interface
///
class IMeetingListHelper
{
public:
    virtual ~IMeetingListHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting list helper callback sink.
     *
     * @param pSink A pointer to the IMeetingListHelperSink that receives the meeting list helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingListHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting list helper callback sink.
     *
     * @param pSink A pointer to the IMeetingListHelperSink that receives the meeting list helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingListHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief List all the meetings from current Zoom Room configured calendar.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ListMeeting() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Schedule a calendar event into current Zoom Room configured calendar.
     *
     * @param scheduleParam Schedule calendar event param, specify the event information.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ScheduleCalendarEvent(ScheduleCalendarEventParam scheduleParam) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Delete the calendar event from current Zoom Room configured calendar.
     *
     * @param meetingItem The assigned meeting item to be deleted.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeleteCalendarEvent(const MeetingItem& meetingItem) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Check in the calendar event.
     *
     * @param meetingItem The assigned meeting item to be checked in.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CheckInCalendarEvent(const MeetingItem& meetingItem) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Check out the calendar event.
     *
     * @param meetingItem The assigned meeting item to be checked out.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CheckOutCalendarEvent(const MeetingItem& meetingItem) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show current upcoming meeting alert.
     *
     * @param meetingItem Specify the meeting item
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowUpcomingMeetingAlert(const MeetingItem& meetingItem) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Close current upcoming meeting alert.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CloseUpcomingMeetingAlert() = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.1.5
     *
     * @brief Close auto release meeting alert.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CloseAutoReleaseMeetingAlert() = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_IMEETINGLISTHELPER_H

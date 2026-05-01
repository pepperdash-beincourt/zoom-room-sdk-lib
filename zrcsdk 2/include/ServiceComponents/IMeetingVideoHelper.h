/*!
 * \file IMeetingVideoHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Meeting Video interfaces.
 */
#ifndef ZRC_IMEETINGVIDEOHELPER_H
#define ZRC_IMEETINGVIDEOHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum PinShareWarningType
    \brief Screen pin share warning type.
    Here are more detailed structural descriptions.
 */
enum PinShareWarningType
{
    PinShareWarningTypeNone = 0,               ///<None
    PinShareWarningTypeNoAnnotationForSelf,    ///<Warning that the annotation will be lost if pin share on the screen.
    PinShareWarningTypeStopSelfShare,          ///<Warning that the user's share will be stopped if pin share on the screen.
    PinShareWarningTypeStopCameraShare,        ///<Warning that the camera share will be stopped if pin share on the screen.
    PinShareWarningTypeStopWhiteboard          ///<Warning that the classic whiteboard share will be stopped if pin share on the screen.
};

/*! \enum CanNotPinShareReason
    \brief Screen cannot pin share reason.
    Here are more detailed structural descriptions.
 */
enum CanNotPinShareReason
{
    CanNotPinShareReasonUnknown = 1,    ///<Unclassified reason
    CanNotPinShareReasonContentOnly     ///<Content only not enabled, current ScreenLayoutSourceType is not ScreenLayoutSourceTypeSharedContent.
};

/*! \enum PreviewVideoType
    \brief Preview video type.
    Here are more detailed structural descriptions.
 */
enum PreviewVideoType
{
    PreviewVideoTypeCameraSettings = 0,    ///<Preview video type camera settings.
    PreviewVideoTypeVirtualBackground,     ///<Preview video type virtual background.
    PreviewVideoTypeMeetingAlert,          ///<Preview video type meeting alert.
};

/// \brief Screen status for pin video or pin share.
///
struct ScreenStatusForPin
{
    int32_t                      screenIndex = 0;                                           ///<Screen index, see \link MeetingScreen \endlink enum.
    bool                         canPinVideo = false;                                       ///<TRUE indicates screen can be pinned video.
    std::vector<int32_t>         pinnedUserIDs;                                             ///<Pinned user id list.
    ScreenLayoutSourceType       screenLayout = ScreenLayoutSourceTypeActiveVideo;          ///<Screen layout source type, see \link ScreenLayoutSourceType \endlink enum.
    int32_t                      pinnedShareSourceID = 0;                                   ///<Screen pinned share source ID.
    ShareSourceType              pinnedShareSourceType = ShareSourceTypeUnknown;            ///<Share source type, see \link ShareSourceType \endlink enum.
    std::vector<ShareSourceType> pinnableShareTypes;                                        ///< Screen pinnable share types.
    bool                         canPinShare = false;                                       ///<TRUE indicates screen can be pinned share content.
    CanNotPinShareReason         canNotPinShareReason = CanNotPinShareReasonUnknown;        ///<Can not pin share reason, see \link CanNotPinShareReason \endlink enum.
    bool                         isZRWScreen = false;                                       ///<TRUE indicates screen is a Zoom Room Whiteboard screen.
    bool                         isThumbnailScreen = false;                                 ///<TRUE indicates screen is a Thumbnail screen.
    int32_t                      pinnedShareUserID = 0;                                     ///<Screen pinned share user ID.
};

/// \brief Spotlight status
///
struct SpotlightStatus
{
    bool                 present = false;    ///<TRUE indicates current meeting presents spotlighted user.
    std::vector<int32_t> userIDs;            ///<List of spotlighted users' ID.
};

/// \brief My video touch up settings.
///
struct MyVideoTouchUpSettings
{
    bool    isFaceBeautyEnabled = false;    ///<TRUE indicates touch up function is enabled. Otherwise not.
    int32_t faceBeautyStrength = 0;         ///<The strength of the touch up function. Range is [0, 100].
};

/// \brief My video low light settings.
///
struct MyVideoLowLightSettings
{
    bool    isAdjustLowLightEnabled = false;    ///<TRUE indicates low light function is enabled. Otherwise not.
    bool    isAutoAdjustLowLight = false;       ///<TRUE indicates automatically adjust low light value. Otherwise manually adjust.
    int32_t adjustLowLightValue = 0;            ///<The value of the low light function. Range is [0, 100].
};

/// \brief My video settings.
///
struct MyVideoSettings
{
    MyVideoTouchUpSettings  touchUpSettings;     ///<My video touch up settings.
    MyVideoLowLightSettings lowLightSettings;    ///<My video low light settings.

    bool  allowUserEnhanceAppearance = false;     ///<TRUE indicates allow user enhance appearance. Otherwise not.
    bool  canPresetSettingsForMeeting = false;    ///<TRUE indicates can preset settings for specific meeting. Otherwise not.
    bool  isLocked = false;                       ///<TRUE the setting is locked on web, and you can't change the settings, but you can preset settings for specific meeting and change the setting in meeting.
};

/// \brief My meeting video settings of the specific meeting, which includes my video settings and the specific meeting.
///
struct MyMeetingVideoSettings
{
    MyVideoSettings setting;    ///<My video settings. The fields allowUserEnhanceAppearance, canPresetSettingsForMeeting, and isLocked in the MyVideoSettings struct are invalid in this context.
    MeetingItem     meeting;    ///<The specific meeting.
};

/// \brief Meeting Video helper event callback
///
class IMeetingVideoHelperSink
{
public:
    virtual ~IMeetingVideoHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Zoom Room's video status changed callback.
     *
     * @param videoStatus Updated video status.
     */
    virtual void OnUpdateMyVideoNotification(const VideoStatus& videoStatus) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief After mute user video, user video status changed callback.
     *
     * @param userID The specific user ID.
     * @param videoStatus Assigned user's video status.
     */
    virtual void OnMuteUserVideoNotification(int32_t userID, const VideoStatus& videoStatus) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Notification from host ask to start video.
     *
     * @param userID The user ID when requested.
     */
    virtual void OnAskStartVideoByHostNotification(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Meeting screen status for pin update notification.
     *
     * @param pinStatusList Zoom Room screen status for pin.
     * @param warningType Pin share warning type, see \link PinShareWarningType \endlink enum.
     */
    virtual void OnUpdateScreenStatusForPinNotification(const std::vector<ScreenStatusForPin>& pinStatusList, PinShareWarningType warningType) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Meeting spotlight status changed notification.
     *
     * @param spotlightStatus Updated spotlight status.
     */
    virtual void OnSpotlightStatusNotification(const SpotlightStatus& spotlightStatus) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow attendees start video update notification.
     *
     * @param allow TRUE indicates attendees are allowed to start video.
     */
    virtual void OnUpdateAllowAttendeesStartVideo(bool allow) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief My video settings notification.
     *
     * @param settings My video settings. If you use the APIs in IMeetingListHelper to set the settings for the specific meeting, the specific meeting will use the settings you set.
     */
    virtual void OnMyVideoSettingsNotification(const MyVideoSettings& settings) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief My video settings of the specific meeting notification.
     *
     * @param settings My meeting video settings.
     */
    virtual void OnMyMeetingVideoSettingsNotification(const MyMeetingVideoSettings& settings) = 0;
};

/// \brief Meeting Video helper interface
///
class IMeetingVideoHelper
{
public:
    virtual ~IMeetingVideoHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting video helper callback sink.
     *
     * @param pSink A pointer to the IMeetingVideoHelperSink that receives the meeting video helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingVideoHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting video helper callback sink.
     *
     * @param pSink A pointer to the IMeetingVideoHelperSink that receives the meeting video helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingVideoHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute or unmute self video.
     *
     * @param stop TRUE indicates mute self video. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UpdateMyVideo(bool stop) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute or unmute the assigned user video.
     *
     * @param userID Specify the user ID to unmute.
     * @param mute TRUE indicates to mute user video. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteUserVideo(int32_t userID, bool mute) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Answer incoming unmute video request
     *
     * @param accepted TRUE indicates accept unmute request. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AnswerHostRequestUnmuteVideo(bool accepted) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow attendees start video.
     *
     * @param allow TRUE indicate allow attendees start video. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowAttendeesStartVideo(bool allow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show the screen index of each display for pin video.
     *
     * @param show TRUE indicates to show the instruction on ZR screen. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowPinUserInstruction(bool show) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow user multi pin.
     *
     * @param userID Specify the user ID to allow multi pin.
     * @param allow TRUE indicates allow to multi pin. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowUserMultiPin(int32_t userID, bool allow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Pin user video on Zoom Room's screen.
     *
     * @param userID Specify the user ID to pin video.
     * @param screenIndex Index of the screen to pin video, see \link MeetingScreen \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PinUserOnScreen(int32_t userID, int32_t screenIndex) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Add pin user on Zoom Room's screen.
     *
     * @param userID Specify the user ID to add pin.
     * @param screenIndex Specify the screen index to add pin, see \link MeetingScreen \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AddPinUserOnScreen(int32_t userID, int32_t screenIndex) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Unpin user video from screen.
     *
     * @param userID Specify the user ID to unpin.
     * @param screenIndex Index of the screen to unpin, see \link MeetingScreen \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnpinUserFromScreen(int32_t userID, int32_t screenIndex) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Unpin use video from all screens.
     *
     * @param userID Specify the user ID to unpin from all screens.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnpinUserFromAllScreens(int32_t userID) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Pin smart name tag video stream on screen.
     *
     * @param streamUserID Video stream user ID. Call ISmartTagHelper::GetNamedNameTagList to retrieve the video stream userID.
     * @param screenIndex Index of the screen to pin video, see \link MeetingScreen \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PinSmartNameTagStreamOnScreen(int32_t streamUserID, int32_t screenIndex) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Add pin smart name tag video stream on Zoom Room's screen.
     *
     * @param streamUserID Video stream user ID. Call ISmartTagHelper::GetNamedNameTagList to retrieve the video stream userID.
     * @param screenIndex Index of the screen to pin video, see \link MeetingScreen \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AddPinSmartNameTagStreamOnScreen(int32_t streamUserID, int32_t screenIndex) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Unpin smart name tag video stream from screen.
     *
     * @param streamUserID Video stream user ID.
     * @param screenIndex Index of the screen to pin video, see \link MeetingScreen \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnpinSmartNameTagStreamFromScreen(int32_t streamUserID, int32_t screenIndex) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Unpin smart name tag video stream from all screens.
     *
     * @param streamUserID Video stream user ID.
     * @param screenIndex Index of the screen to pin video, see \link MeetingScreen \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnpinSmartNameTagStreamFromAllScreens(int32_t streamUserID) = 0;
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Remove all users' pinned video.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RemoveAllPinUsers() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Spotlight user
     *
     * @param userID Specify the user ID to spotlight.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SpotlightUser(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Add spotlight user.
     *
     * @param userID Specify the user ID to add spotlight.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AddSpotlightUser(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Cancel spotlight user
     *
     * @param userID Specify the user ID to spotlight.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CancelSpotlightUser(int32_t userID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Remove all spotlight users.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RemoveAllSpotlightUsers() = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.0.0
     *
     * @brief Query if support set my video hidden.
     *
     * @param[out] support TRUE indicates support set my video hidden.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsSupportSetMyVideoHidden(bool& support) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set my video hidden.
     *
     * @param hidden TRUE indicates to hidden my video. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetMyVideoHidden(bool hidden) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Set my video touch up settings. The meeting will use the specified settings, if you preset the settings for the specific meeting. And if you want to adjust touch up settings in meeting, you also can use this API.
     *
     * @param settings My video touch up settings.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetMyVideoTouchUp(const MyVideoTouchUpSettings& settings) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Set my video low light settings. The meeting will use the specified settings, if you preset the settings for the specific meeting. And if you want to adjust low light settings in meeting, you also can use this API.
     *
     * @param settings My video low light settings.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetMyVideoLowLight(const MyVideoLowLightSettings& settings) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Fetch my video settings of the specific meeting. If you want to see the effect of the settings, you can call the \link ShowVideoPreview \endlink API in IMeetingVideoHelper and set PreviewVideoTypeMeetingAlert as the value of type.
     *
     * @param meetingItem The specific meeting item.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError FetchMyMeetingVideoSettings(const MeetingItem& meetingItem) = 0;
    /**
    * @since 6.5.0
    * @zrminver 6.5.0
    *
    * @brief Set my video touch up settings for the specific meeting.
    *
    * @param meetingItem The specific meeting item.
    * @param settings My video touch up settings.
    * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
    * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
    */
    virtual ZRCSDKError SetMyMeetingVideoTouchUp(const MeetingItem& meetingItem, const MyVideoTouchUpSettings& settings) = 0;
    /**
    * @since 6.5.0
    * @zrminver 6.5.0
    *
    * @brief Set my video low light settings for the specific meeting.
    *
    * @param meetingItem The specific meeting item.
    * @param settings My video low light settings.
    * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
    * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
    */
    virtual ZRCSDKError SetMyMeetingVideoLowLight(const MeetingItem& meetingItem, const MyVideoLowLightSettings& settings) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Show video preview.
     *
     * @param show TRUE indicates to show video preview. Otherwise not.
     * @param type The preview video type.
     * @param meeting The specific meeting. This field is necessary only when type == PreviewVideoTypeMeetingAlert.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowVideoPreview(bool show, PreviewVideoType type, const MeetingItem& meeting = MeetingItem()) = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_IMEETINGVIDEOHELPER_H

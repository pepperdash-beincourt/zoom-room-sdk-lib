/*!
 * \file IMeetingViewLayoutHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Meeting View Layout interfaces.
 */
#ifndef ZRC_IVIDEOLAYOUTHELPER_H
#define ZRC_IVIDEOLAYOUTHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum VideoLayoutStyle
    \brief Zoom Room video layout style.
    Here are more detailed structural descriptions.
 */
enum VideoLayoutStyle
{
    VideoLayoutStyleUnknown = 0,              ///<Unknown
    VideoLayoutStyleGallery = 1,              ///<Gallery View layout
    VideoLayoutStyleSpeaker = 2,              ///<Speaker View layout
    VideoLayoutStyleThumbnail = 3,            ///<Thumbnail View layout
    VideoLayoutStyleContentOnly = 4,          ///<Content only layout, all screens show shared content
    VideoLayoutStyleCancelContentOnly = 5,    ///<Cancel content only layout.
    VideoLayoutStyleDynamicLayout = 6,        ///<Dynamic View layout
};

/*! \enum VideoThumbPosition
    \brief Video thumb position.
    Here are more detailed structural descriptions.
 */
enum VideoThumbPosition
{
    VideoThumbPositionCenter = 0,       ///<Video thumb position center.
    VideoThumbPositionUp = 1,           ///<Video thumb position up.
    VideoThumbPositionRight = 2,        ///<Video thumb position right.
    VideoThumbPositionUpRight = 3,      ///<Video thumb position up-right.
    VideoThumbPositionDown = 4,         ///<Video thumb position down.
    VideoThumbPositionDownRight = 5,    ///<Video thumb position down-right.
    VideoThumbPositionLeft = 6,         ///<Video thumb position left.
    VideoThumbPositionUpLeft = 7,       ///<Video thumb position up-left.
    VideoThumbPositionDownLeft = 8      ///<Video thumb position down-left.
};

/*! \enum VideoThumbSize
    \brief Video thumb size.
    Here are more detailed structural descriptions.
 */
enum VideoThumbSize
{
    VideoThumbSizeOff = 0,            ///<Video thumb size off
    VideoThumbSize1x = 1,             ///<Video thumb size normal
    VideoThumbSize2x = 2,             ///<Video thumb size 2x
    VideoThumbSize3x = 3,             ///<Video thumb size 3x
    VideoThumbSizeVideoStripe = 4,    ///<Video thumb in video stripe
};

/*! \enum PageVideoType
    \brief Page video type.
    Here are more detailed structural descriptions.
 */
enum PageVideoType
{
    PageVideoTypeUnknown = -1,            ///<Page video type unknown.
    PageVideoTypeGalleryView = 0,         ///<Page video type Gallery View.
    PageVideoTypeThumbnailView = 1,       ///<Page video type Thumbnail View.
    PageVideoTypeDynamicLayoutView = 2    ///<Page video type Dynamic View.
};

/*! \enum VideoOrderType
    \brief Video order type.
    Here are more detailed structural descriptions.
 */
enum VideoOrderType
{
    VideoOrderTypeUnknown = -1,           ///<Invalid
    VideoOrderTypeDefault = 0,            ///<Default
    VideoOrderTypeAlphabetical,           ///<Videos are ordered by alphabetical.
    VideoOrderTypeReverseAlphabetical,    ///<Videos are ordered by reverse alphabetical.
    VideoOrderTypeSavedOrder              ///<Videos are ordered by saved order.
};

/*! \enum DynamicLayoutType
    \brief Dynamic layout type
    Here are more detailed structural descriptions.
 */
enum DynamicLayoutType
{
    DynamicLayoutTypeSpeakersOnUnknown = -1,
    DynamicLayoutTypeSpeakersOnBottom = 0,
    DynamicLayoutTypeSpeakersOnMiddle,
    DynamicLayoutTypeSpeakersOnTop
};

/*! \enum ConfidenceMonitorLayoutType
    \brief Confidence monitor layout type
    Here are more detailed structural descriptions.
 */
enum ConfidenceMonitorLayoutType
{
    ConfidenceMonitorLayoutTypeUnknown = -1,        ///<None
    ConfidenceMonitorLayoutTypeSelf = 0,            ///<Self view
    ConfidenceMonitorLayoutTypeActive = 1,          ///<Active speaker
    ConfidenceMonitorLayoutTypeShareContent = 2,    ///<Share content
};

/*! \enum AttendeeViewLayoutType
    \brief Attendee view layout type when viewing live feed.
    Here are more detailed structural descriptions.
 */
enum AttendeeViewLayoutType
{
    AttendeeViewLayoutTypeNone = 0,         ///<None
    AttendeeViewLayoutTypeStandard,         ///<Side-by-side if exist share source.
    AttendeeViewLayoutTypeSpeaker,          ///<Speaker
    AttendeeViewLayoutTypeGallery,          ///<Gallery
    AttendeeViewLayoutTypeFollow,           ///<Follow host's view
    AttendeeViewLayoutTypeShareContentOnly  ///<Share content only.
};

/*! \enum ThumbnailsPositionType
    \brief thumbnails position type.
    Here are more detailed structural descriptions.
*/
enum ThumbnailsPositionType
{
    ThumbnailsPositionTypeNone = -1,        ///<Thumbnails position is none.
    ThumbnailsPositionTypeBottom = 0,       ///<Thumbnails position is bottom.
    ThumbnailsPositionTypeTop,              ///<Thumbnails position is top.
    ThumbnailsPositionTypeUnknown = 255,    ///<Thumbnails position is unknown.
};

/// \brief Video page status.
///
struct VideoPageStatus
{
    bool          isInFirstPage = false;                       ///<TRUE indicates video page is in first page.
    bool          isInLastPage = false;                        ///<TRUE indicates video page is in last first page.
    PageVideoType pageVideoType = PageVideoTypeGalleryView;    ///<Page video type, see \link PageVideoType \endlink enum.
    int           videoCountInCurrentPage = 0;                 ///<Video count in current page.
};

/// \brief Video thumb info.
///
struct VideoThumbInfo
{
    bool               isSupported = false;                    ///<TRUE indicates video thumb is supported.
    VideoThumbPosition position = VideoThumbPositionCenter;    ///<Video thumb position, see \link VideoThumbPosition \endlink enum.
    VideoThumbSize     size = VideoThumbSizeOff;               ///<Video thumb size, see \link VideoThumbSize \endlink enum.
    VideoPageStatus    videoPageStatus;                        ///<Video page status
    bool               isThumbnailOnTop = false;               ///<TRUE indicates video thumbnail is at top of screen.
};

/// \brief Video layout status.
///Default is Speaker View layout.
struct VideoLayoutStatus
{
    bool canSwitchSpeakerView = false;      ///<TRUE indicates can switch to Speaker View layout.
    bool canSwitchThumbnailView = false;    ///<TRUE indicates can switch to Thumbnail View layout.
    bool canSwitchGalleryView = false;      ///<TRUE indicates can switch to Gallery View layout.
    bool canSwitchContentOnly = false;      ///<TRUE indicates can switch to Content Only layout.
    bool canSwitchDynamicLayout = false;    ///<TRUE indicates can switch to Dynamic View layout.

    bool isInThumbnail = false;        ///<TRUE indicates Current is in Thumbnail View layout.
    bool isInGalleryView = false;      ///<TRUE indicates current is in Gallery View layout.
    bool isInContentOnly = false;      ///<TRUE indicates current is in Content Only layout.
    bool isInImmersive = false;        ///<TRUE indicates current is in Immersive View layout.
    bool isInDynamicLayout = false;    ///<TRUE indicates current is in Dynamic View layout.

    bool canAdjustFloatingVideo = false;           ///<TRUE indicates can adjust floating video.
    bool canSwitchFloatingShareContent = false;    ///<TRUE indicates can switch between floating video and floating shared content.
    bool isInFloatingShareContent = false;         ///<TRUE indicates current is in floating shared content.
};

/// \brief Wall view style status
///Gallery view
struct WallViewStyleStatus
{
    VideoLayoutStatus videoLayoutStatus;    ///<Gallery View video layout status
    VideoPageStatus   videoPageStatus;      ///<Gallery View video page status
    VideoThumbInfo    videoThumbInfo;       ///<Gallery View video thumb info
};

/// \brief Video order info
///
struct VideoOrderInfo
{
    VideoOrderType type = VideoOrderTypeUnknown;    ///<Video order type.
    bool           hasSavedOrder = false;           ///<TRUE indicates has video order.
    bool           isFollowHostOrder = false;       ///<TRUE indicates current is following host's video order
    bool           isSavedOrderEnabled = false;     ///<TRUE indicates saved order is enabled.
};

/// \brief Confidence monitor info
///
struct ConfidenceMonitorInfo
{
    ConfidenceMonitorLayoutType layout = ConfidenceMonitorLayoutTypeUnknown;    ///<Confidence monitor layout type, see \link ConfidenceMonitorLayoutType \endlink enum.
    bool                        isSharedContentAvailable = false;
};

/// \brief Meeting screen layout control info
///
struct ScreenLayoutCtrlInfo
{
    ScreenLayoutSourceType layout = ScreenLayoutSourceTypeNone;     ///<Screen layout type, see \link ScreenLayoutSourceType \endlink enum.
    bool enable = false;                                            ///<TRUE indicates current layout is enabled.
    bool visible = false;                                           ///<TRUE indicates current layout is visible
};

/// \brief Meeting screen layout info
///
struct ScreenLayoutInfo
{
    MeetingScreen screen = MeetingScreenUnknown;                    ///<Screen index
    ScreenLayoutSourceType layout = ScreenLayoutSourceTypeNone;     ///<Screen layout type, see \link ScreenLayoutSourceType \endlink enum.
    std::vector<ScreenLayoutCtrlInfo> layoutCtrlInfos;              ///<Screen layout infos, indicates which layout current screen can switch to.
};

/// \brief Meeting screen layout status
///
struct ScreenLayoutStatus
{
    bool canShowContentOnly = false;                             ///<TRUE indicates can switch to Content Only layout.
    bool isInContentOnly = false;                                ///<TRUE indicates current is in Content Only layout.
    bool canAdjustFloatingVideo = false;                         ///<TRUE indicates can adjust floating video.
    bool canSwitchFloatingShareContent = false;                  ///<TRUE indicates can switch between floating video and floating shared content.
    bool isInFloatingShareContent = false;                       ///<TRUE indicates current is in floating shared content.
    bool canAdjustMyAutoGeneratedVideoStreamsVisibility = false; ///<TRUE indicates can adjust my auto generated video streams visibility.
    bool isShowMyAutoGeneratedVideoStreams = true;               ///<TRUE  indicates show my auto generated video streams, otherwise not.
    std::vector<ScreenLayoutInfo> layoutInfos;                   ///<All screen layout infos.
};

/// \brief Meeting View Layout helper event callback
///
class IMeetingViewLayoutHelperSink
{
public:
    virtual ~IMeetingViewLayoutHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Wall view style status update callback.
     *
     * @param status Wall view style status.
     */
    virtual void OnUpdateWallviewStyleNotification(const WallViewStyleStatus& status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Video thumb info update callback.
     *
     * @param info Video thumb info.
     */
    virtual void OnUpdateVideoThumbInfo(const VideoThumbInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Video page status update callback.
     *
     * @param noti Video page status.
     */
    virtual void OnUpdateVideoPageStatusNotification(const VideoPageStatus& noti) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show non-video participant changed callback.
     *
     * @param isShowNonVideoParticipants TRUE indicates audio participant is shown.
     */
    virtual void OnUpdateIsNonVideoParticipantsShowedNotification(bool isShowNonVideoParticipants) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show up to 49 user's video per page in gallery changed callback.
     *
     * @param enabled TRUE indicates enabled show up to 49 user's video per page in gallery.
     */
    virtual void OnUpdateShowUpTo49PerPageInGallery(bool enabled) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Auto switch speaker view changed callback.
     *
     * @param support TRUE indicates support auto switch to speaker view.
     * @param enable TRUE indicates auto switch to speaker view is enabled.
     */
    virtual void OnAutoSwitchSpeakerNotification(bool support, bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Video order changed notification.
     *
     * @param videoOrderInfo The new video order info.
     */
    virtual void OnVideoOrderNotification(const VideoOrderInfo& videoOrderInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Dynamic layout option notification.
     *
     * @param layout The new dynamic layout type.
     */
    virtual void OnDynamicLayoutOptionNotification(DynamicLayoutType layout) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Confidence monitor info notification.
     *
     * @param info The confidence monitor info
     */
    virtual void OnConfidenceMonitorNotification(const ConfidenceMonitorInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Change attendee view layout notification.
     *
     * @param layout The attendee view layout type.
     */
    virtual void OnChangeAttendeeViewNotification(AttendeeViewLayoutType layout) = 0;
   
    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Enable change attendee view layout to shared content only update notification.
     *
     * @param isSupport TRUE indicates current ZR supports change attendee view layout to shared content only.
     * @param isEnable TRUE indicates feature is disabled in meeting. This usually dues to the host is an old client not supports this feature.
     */
    virtual void OnAttendeeViewLayoutEnableShareContentOnlyNotification(bool isSupport, bool isEnable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update gallery grid selection notification
     *
     * @param isEnabled TRUE indicates is enabled
     * @param row The row
     * @param column The column
     */
    virtual void OnUpdateGalleryGridSelectionNotification(bool isEnabled, uint32_t row, uint32_t column) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Screen layout status changed notification.
     *
     * @param status The new screen layout status.
     */
    virtual void OnUpdateScreenLayoutStatus(const ScreenLayoutStatus& status) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Thumbnails position notification.
     *
     * @param type The current thumbnails position.
     */
    virtual void OnThumbnailsPositionNotification(ThumbnailsPositionType type) = 0;
};

/// \brief Meeting View Layout helper interface
///
class IMeetingViewLayoutHelper
{
public:
    virtual ~IMeetingViewLayoutHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting view layout helper callback sink.
     *
     * @param pSink A pointer to the IMeetingViewLayoutHelperSink that receives the meeting view layout helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingViewLayoutHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting view layout helper callback sink.
     *
     * @param pSink A pointer to the IMeetingViewLayoutHelperSink that receives the meeting view layout helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingViewLayoutHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update Zoom Room meeting video layout style.
     *
     * @param style Specify the video layout style to update.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @deprecated Please use SetScreenLayout(MeetingScreen screenIndex, ScreenLayoutSourceType layout)
     */
    virtual ZRCSDKError UpdateVideoLayoutStyle(VideoLayoutStyle style) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control video thumb position and size.
     *
     * @param position Specify the position to move video thumb to.
     * @param size Specify the size to change video thumb to.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ControlVideoPosition(VideoThumbPosition position, VideoThumbSize size) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Turn video page.
     *
     * @param forward TRUE indicates to page forward video.
     * @param pageVideoType Specify the current page video type, see \link PageVideoType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError TurnVideoPage(bool forward, PageVideoType pageVideoType) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Switch to floating share content when in single screen.
     *
     * @param floatingShare TRUE indicates to switch to floating share content. Otherwise switch to floating video.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SwitchToFloatingShareForSingleScreen(bool floatingShare) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.0.0
     *
     * @brief Query if support show non-video participants.
     *
     * @param[out] support TRUE indicates support show non-video participants
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsSupportShowNonVideoParticipants(bool& support) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show non-video participants.
     *
     * @param show TRUE indicates to show audio participants. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowNonVideoParticipants(bool show) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief  Enable to show up tp 49 user video per page in gallery.
     *
     * @param enable TRUE indicates to show up tp 49 per page in gallery. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableShowUpTo49PerPageInGallery(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable auto switch speaker
     *
     * @param enable TRUE indicates to enable auto switch speaker. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableAutoSwitchSpeaker(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Select video order
     *
     * @param type The video order type, see \link VideoOrderType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectVideoOrder(VideoOrderType type) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set dynamic layout option
     *
     * @param layout The dynamic layout type, see \link DynamicLayoutType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetDynamicLayoutOption(DynamicLayoutType layout) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set confidence monitor layout
     *
     * @param layout The confidence monitor layout type, see \link ConfidenceMonitorLayoutType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetConfidenceMonitorLayout(ConfidenceMonitorLayoutType layout) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Change attendee view layout
     *
     * @param layout The attendee view layout type, see \link AttendeeViewLayoutType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeAttendeeView(AttendeeViewLayoutType layout) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Change attendee view layout, row and column range [2,7].
     *
     * Current we only support row == column. If your row is not equal to column, ZR will adjust row x column to 5 x 5.
     *
     * @param row The gallery gird row.
     * @param column  The gallery gird column.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectGalleryGrid(uint32_t row, uint32_t column) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Expand conf self video.
     *
     * @param expand  TRUE indicates to expand. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ExpandConfSelfVideo(bool expand) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Set screen layout
     *
     * @param screenIndex The screen index which you want to set, see \link MeetingScreen \endlink enum.
     * @param layout The new layout type, see \link ScreenLayoutSourceType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetScreenLayout(MeetingScreen screenIndex, ScreenLayoutSourceType layout) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Set share content only
     *
     * @param enable TRUE indicates enable share content only
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetShareContentOnly(bool enable) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Show screen index on Zoom Rooms client
     *
     * @param show TRUE indicates show screen index
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowScreenIndex(bool show) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Get current thumbnails position. When type is ThumbnailsPositionTypeNone, you can't call ChangeThumbnailsPosition API.
     *
     * @param[out] type The current thumbnails position.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetThumbnailsPosition(ThumbnailsPositionType& type) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Change thumbnails position in multi-speaker and thumbnails layout.
     *
     * @param type The position of thumbnails position.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeThumbnailsPosition(ThumbnailsPositionType type) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Show my auto generated video streams.
     *
     * @param show TRUE indicates show my auto generated video streams, otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowMyAutoGeneratedVideoStreams(bool show) = 0;
};

NS_ZRCSDK_END
#endif    //ZRC_IVIDEOLAYOUTHELPER_H

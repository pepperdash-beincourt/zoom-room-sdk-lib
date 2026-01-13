/*!
 * \file INDIHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief NDI interfaces.
 */
#ifndef ZRC_INDIHELPER_H
#define ZRC_INDIHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum NDIResolution
    \brief NDI output's resolution.
    Here are more detailed structural descriptions.
*/
enum NDIResolution
{
    NDIResolutionUnknown = -1, ///<NDI output's resolution is unknown.
    NDIResolution360p = 0,     ///<NDI output's resolution is 360p.
    NDIResolution720p,         ///<NDI output's resolution is 720p.
    NDIResolution1080p,        ///<NDI output's resolution is 1080p.
};

/*! \enum NDIFrameRate
    \brief NDI output's frame rate.
    Here are more detailed structural descriptions.
*/
enum NDIFrameRate
{
    NDIFrameRateUnknown = -1, ///<NDI output's frame rate is unknown.
    NDIFrameRate25fps = 0,  ///<NDI output's frame rate is 25fps.
    NDIFrameRate29_97fps,   ///<NDI output's frame rate is 29_97fps.
    NDIFrameRate30fps,      ///<NDI output's frame rate is 30fps.
    NDIFrameRate50fps,      ///<NDI output's frame rate is 50fps.
    NDIFrameRate59_94fps,   ///<NDI output's frame rate is 50_94fps.
    NDIFrameRate60fps,      ///<NDI output's frame rate is 60fps.
};

/*! \enum NDISourceType
    \brief The NDI source type supports both non-persistent and persistent assignments.
    Here are more detailed structural descriptions.
*/
enum NDISourceType
{
    NDISourceTypeNone = 0,          ///<NDI source type is none.
    NDISourceTypeActiveSpeaker,     ///<NDI source type is active speaker.
    NDISourceTypeUser,              ///<NDI source type is user.
    NDISourceTypeShare,             ///<NDI source type is share.
    NDISourceTypePinGroup,          ///<NDI source type is pin group.
    NDISourceTypeSpotlightGroup,    ///<NDI source type is spotlight group.
    NDISourceTypeGallery,           ///<NDI source type is gallery.
};

/// \brief NDI usage settings
///
struct NDIUsageSettings
{
    bool          isPersistentNDIEnabled = false;       ///<TRUE indicates the NDI is enabled. Otherwise not.
    bool          isPersistentNDILocked = false;        ///<TRUE indicates the NDI is locked. Otherwise not.
    bool          isNDIEnabledForPreMeeting = false;    ///<TRUE indicates the NDI is enabled in premeeting. Otherwise not.
    NDIResolution resolution = NDIResolutionUnknown;    ///<NDI output's resolution.
    NDIFrameRate  frameRate = NDIFrameRateUnknown;      ///<NDI output's frame rate.
    std::vector<NDIResolution> supportedResolutionList; ///<Supported NDI resolution list.
    std::vector<NDIFrameRate> supportedFrameRateList;   ///<Supported NDI frame rate list.
    int32_t       outputCount = 12;                     ///<NDI output count.
    int32_t       maxOutputCount = 12;                  ///<Max NDI output count.
};

/// \brief NDI source
/// The NDISource supports both non-persistent and persistent assignments.
struct NDISource
{
    NDISourceType    type = NDISourceTypeNone;              ///<NDI source type, such as \link NDISourceType \endlink enum.
    int32_t          sourceID = 0;                          ///<NDI source ID. Valid for type is NDISourceTypeShare, the sourceID is the userID that the NDI source belongs to.
    ConfInstType     fromType = ConfInstTypeCurrentConf;    ///<Where does the NDI source come from, such as \link ConfInstType \endlink enum.
    int32_t          sourceTypeIndex = 0;                   ///<Valid for type is Share(only persistently pinned), PinGroup or Gallery.
    int32_t          shareSourceID = 0;                     ///<Valid for type is NDISourceTypeShare.
    GalleryGridSize  gridSize;                              ///<Valid for type is NDISourceTypeGallery.
};

/// \brief NDI pinned source
/// The NDIPinnedSource refers to the non-persistent or persistent pinned source.
struct NDIPinnedSource
{
    NDISource        source;    ///<NDI source.
    int32_t          index = 0; ///<The index where the source is pinned, index values start from 1.
};

/// \brief NDI usage info
/// 
struct NDIUsageInfo
{
    bool                           ndiEnabled = false;          ///<True indicates the NDI is enabled and self is host or cohost. Otherwise not.
    int32_t                        supportedCount = 0;          ///<The max number of pinned sources at the same time.
    std::vector<NDIPinnedSource>   sources;                     ///<The non-persistent pinned sources in meeting.
    int32_t                        galleryPageCount = -1;       ///<The number of gallery pages.
};

/// \brief NDI helper event callback.
///
class INDIHelperSink
{
public:
    virtual ~INDIHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief NDI usage settings notification.
     *
     * @param settings NDI usage settings, details \link NDIUsageSettings \endlink struct.
     */
    virtual void OnNDIUsageSettingsNotification(const NDIUsageSettings& settings) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief NDI usage info notification.
     *
     * @param ndiUsageInfo NDI usage info, details \link NDIUsageInfo \endlink struct.
     */
    virtual void OnNDIUsageNotification(const NDIUsageInfo& ndiUsageInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief NDI available source notification.
     *
     * @param sources NDI available sources to pin.
     */
    virtual void OnNDIAvailableSourcesNotification(const std::vector<NDISource>& sources) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief NDI devices notification.
     *
     * @param devices NDI available devices.
     */
    virtual void OnNDIDeviceListNotification(const std::vector<Device>& devices) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Persistent NDI source notification.
     *
     * @param sources persistent NDI source list, details \link NDIPinnedSource \endlink struct.
     *
     * @changelog
     *  - 6.4.0: Replaced the parameter PersistentNDISource with NDIPinnedSource.
     */
    virtual void OnPersistentNDISourcesNotification(const std::vector<NDIPinnedSource>& sources) = 0;
};

/// \brief NDI helper interface
///
class INDIHelper
{
public:
    virtual ~INDIHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register NDI helper callback sink.
     *
     * @param pSink A pointer to the INDIHelperSink that receives the NDI helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(INDIHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting NDI callback sink.
     *
     * @param pSink A pointer to the INDIHelperSink that receives the NDI helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(INDIHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set NDI output resolution.
     *
     * @param resolution The NDI output resolution, such as \link NDIResolution \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetNDIResolution(NDIResolution resolution) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set NDI output frame rate.
     *
     * @param frameRate The NDI output frame rate, such as \link NDIFrameRate \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetNDIFrameRate(NDIFrameRate frameRate) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set NDI enable in premeeting.
     *
     * @param enable TRUE indicates enable NDI in premeeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetNDIEnableInPreMeeting(bool enable) = 0;
    
    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Set NDI output count.
     *
     * @param outputCount NDI output count.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetNDIOutputCount(int32_t outputCount) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get available NDI sources.
     *
     * @param[out] sources The available NDI sources.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetAvailableNDISources(std::vector<NDISource>& sources) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get NDI usage sources.
     *
     * @param[out] sources The NDI usage sources.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetNDIPinnedSources(std::vector<NDIPinnedSource>& sources) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Non-persistently pin NDI source in meeting.
     *
     * @param source The NDI source to non-persistently pinned, you can get NDI sources from \link GetAvailableNDISources \endlink function.
     * @param index The index where NDI source to be pinned, can't exceed the supportedCount in \link NDIUsageInfo \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.4.0: Added support for configuring the gallery view grid size when pinning a gallery.
     */
    virtual ZRCSDKError PinNDI(const NDISource& source, int32_t index) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Non-persistently unpin NDI usage source in meeting.
     *
     * @param source The NDI usage source to be unpinned, you can get NDI usage sources from \link INDIHelperSink::OnNDIUsageNotification \endlink sink or \link INDIHelper::GetNDIPinnedSources \endlink function.
     * @param index The index where NDI source to be unpinned.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnpinNDI(const NDISource& source, int32_t index) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get NDI device list.
     *
     * @param[out] devices The NDI devices.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetNDIDeviceList(std::vector<Device>& devices) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Add persistent NDI source.
     *
     * @param source The NDI source to persistently pinned.
     * @param index The index where NDI source to be persistently pinned.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     * - 6.4.0: Replaced the input parameters with NDISource and index.
     * - 6.4.0: Added support for configuring the gallery view grid size when pinning a gallery.
     */
    virtual ZRCSDKError AddPersistentNDISource(const NDISource& source, int32_t index) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Remove persistent NDI source.
     *
     * @param index The index where persistent NDI source to be removed.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RemovePersistentNDISource(int32_t index) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief List persist NDI sources.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ListPersistentNDISources() = 0;
};

NS_ZRCSDK_END

#endif /* ZRC_INDIHELPER_H */

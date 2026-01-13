/*!
 * \file IProAVService.h
 * \since Added in ZRC SDK 6.1.0
 * \brief Entrance of all the Pro AV Service.
 */
#ifndef ZRC_IPROAVSERVICE_H
#define ZRC_IPROAVSERVICE_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum ProAVVideoNameStrapPosition
    \brief Pro AV video overlay name strap position.
    Here are more detailed structural descriptions.
 */
enum ProAVVideoNameStrapPosition
{
    ProAVVideoNameStrapPositionUnknown = -1,    ///<Unknown.
    ProAVVideoNameStrapPositionLeft = 0,        ///<Left.
    ProAVVideoNameStrapPositionCenter = 1,      ///<Center.
    ProAVVideoNameStrapPositionRight = 2,       ///<Right.
};


/*! \enum ProAVUnassignedBehaviorType
    \brief Pro AV unassigned behavior type.
    Here are more detailed structural descriptions.
 */
enum ProAVUnassignedBehaviorType
{
    ProAVUnassignedBehaviorTypeOff = 0,         ///<Off
    ProAVUnassignedBehaviorTypeWallpaper = 1,   ///<Wallpaper
};

/*! \enum ProAVWallpaperRoomType
    \brief Pro AV wallpaper room type.
    Here are more detailed structural descriptions.
 */
enum ProAVWallpaperRoomType
{
    ProAVWallpaperRoomTypeNone = -1,    ///<None.
    ProAVWallpaperRoomTypeMain = 0,     ///<Main Zoom Rooms wallpaper.
    ProAVWallpaperRoomTypeCZR = 1,      ///<Companion Zoom Rooms wallpaper.
    ProAVWallpaperRoomTypeCWB = 2,      ///<Companion Whiteboard wallpaper.
};

/*! \enum ProAVGalleryDistributionMode
    \brief Pro AV gallery distribution mode
    Here are more detailed structural descriptions.
 */
enum ProAVGalleryDistributionMode
{
    ProAVGalleryDistributionModeWaterfall = 0,      ///<Waterfall: Each page fills up before the next one shows users. (Default)
    ProAVGalleryDistributionModeRoundRobin = 1,     ///<Round-robin: Users are evenly distributed across all pages, up to a max page count.
};

/// \brief Pro AV video overlay settings.
///
struct ProAVVideoOverlaySettings
{
    bool isActiveSpeakerGreenOutlineEnabled = false;                            ///<TRUE indicates active speaker green outline is enabled.
    bool isReactionIconsEnabled = false;                                        ///<TRUE indicates reaction icons is enabled.
    bool isRaiseHandIconEnabled = false;                                        ///<TRUE indicates raise hand icon is enabled.
    bool isNameStrapEnabled = false;                                            ///<TRUE indicates name strap is enabled.
    ProAVVideoNameStrapPosition position= ProAVVideoNameStrapPositionUnknown;   ///<Name strap position, see \link ProAVVideoNameStrapPosition \endlink enum.
    bool isMuteIconEnabled = false;                                             ///<TRUE indicates mute icon is enabled.
    bool isPollOverlayEnabled = false;                                          ///<TRUE indicates poll overlay is enabled.
    ProAVGalleryDistributionMode galleryDistributionMode = ProAVGalleryDistributionModeWaterfall; ///<gallery distribution mode, see \link ProAVGalleryDistributionMode \endlink enum.
    uint32_t    maxGalleryPageCount = 0;                                        ///<Max gallery page count, only used for round-robin mode.
    float elementScale = 1.0f;                                                  ///<Scaling factor for the video element; valid range: 0.5 to 3.0.
};

/// \brief Pro AV unassigned behavior.
///
struct ProAVUnassignedBehavior
{
    ProAVUnassignedBehaviorType unassignedType = ProAVUnassignedBehaviorTypeOff; ///Unassigned type, see \link ProAVUnassignedBehaviorType \endlink enum.
    ProAVWallpaperRoomType roomType = ProAVWallpaperRoomTypeNone;                ///Wallpaper room type, see \link ProAVWallpaperRoomType \endlink enum.
    uint32_t wallpaperIndex = 0;                                                 ///Wallpaper index.
};

/*! \enum ProAVVideoLossBehaviorType
    \brief Pro AV video loss behavior type.
    Here are more detailed structural descriptions.
 */
enum ProAVVideoLossBehaviorType
{
    ProAVVideoLossBehaviorTypeDefault = 0,       ///<Default behavior
    ProAVVideoLossBehaviorTypeBlackFrame = 1,    ///<Show black frame
    ProAVVideoLossBehaviorTypeFreezeFrame = 2,   ///<Freeze last frame
    ProAVVideoLossBehaviorTypeWallpaper = 3,     ///<Show wallpaper
};

/// \brief Pro AV video loss behavior.
///
struct ProAVVideoLossBehavior
{
    ProAVVideoLossBehaviorType behaviorType = ProAVVideoLossBehaviorTypeDefault; ///<Video loss behavior type, see \link ProAVVideoLossBehaviorType \endlink enum.
    ProAVWallpaperRoomType wallpaperRoomType = ProAVWallpaperRoomTypeNone;       ///<Wallpaper room type for Wallpaper behavior, see \link ProAVWallpaperRoomType \endlink enum.
    uint32_t wallpaperIndex = 0;                                                 ///<Wallpaper index for Wallpaper behavior.
};

/// \brief Pro AV Service event callback.
///
class IProAVServiceSink
{
public:
    virtual ~IProAVServiceSink() {}

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Pro AV video overlay setting  notification callback.
     *
     * @param settings pro AV video overlay settings, see \link ProAVVideoOverlaySettings \endlink struct.
     */
    virtual void OnProAVVideoOverlaySettingsNotification(const ProAVVideoOverlaySettings& settings) = 0;
    
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Pro AV unassigned behavior  notification callback.
     *
     * @param behavior pro AV unassigned behavior settings, see \link ProAVVideoOverlaySettings \endlink struct.
     */
    virtual void OnProAVUnassignedBehaviorNotification(const ProAVUnassignedBehavior& behavior) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Pro AV video loss behavior notification callback.
     *
     * @param behavior pro AV video loss behavior settings, see \link ProAVVideoLossBehavior \endlink struct.
     */
    virtual void OnProAVVideoLossBehaviorNotification(const ProAVVideoLossBehavior& behavior) = 0;
};

class IDanteOutputHelper;
class IHWIOHelper;

/// \brief Pro AV service interface.
///
class IProAVService
{
public:
    virtual ~IProAVService() {}

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Register pro AV service callback sink.
     *
     * @param pSink A pointer to the IProAVServiceSink that receives the pro AV service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IProAVServiceSink* pSink) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Deregister pro AV service callback sink.
     *
     * @param pSink A pointer to the IProAVServiceSink that receives the pro AV service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IProAVServiceSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get the dante output helper.
     *
     * @return If the function succeeds, the return value is a pointer to IDanteOutputHelper. Otherwise the function returns NULL.
     */
    virtual IDanteOutputHelper* GetDanteOutputHelper() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get the HWIO helper.
     *
     * @return If the function succeeds, the return value is a pointer to IHWIOHelper. Otherwise the function returns NULL.
     */
    virtual IHWIOHelper* GetHWIOHelper() = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Query pro AV video overlay settings.
     *
     * @param[out] settings pro AV video overlay settings, see \link ProAVVideoOverlaySettings \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetProAVVideoOverlaySettings(ProAVVideoOverlaySettings& settings) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Enable pro AV video active speaker green outline.
     *
     * @param enable TRUE indicates enable active speaker green outline. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableProAVVideoActiveSpeakerGreenOutline(bool enable) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Enable pro AV video reaction icons.
     *
     * @param enable TRUE indicates enable video reaction icons. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableProAVVideoReactionIcons(bool enable) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Enable pro AV video raise hand icon.
     *
     * @param enable TRUE indicates enable raise hand icon. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableProAVVideoRaiseHandIcon(bool enable) = 0;
    
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Enable pro AV video mute icon.
     *
     * @param enable TRUE indicates enable video mute icon. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableProAVVideoMuteIcon(bool enable) = 0;
        
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Enable pro AV video poll overlay.
     *
     * @param enable TRUE indicates enable video poll overlay. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableProAVVideoPollOverlay(bool enable) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Enable pro AV video name strap.
     *
     * @param enable TRUE indicates enable video name strap. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableProAVVideoNameStrap(bool enable) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Set pro AV video name strap position.
     *
     * @param position pro AV video name strap position, see \link ProAVVideoNameStrapPosition \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetProAVVideoNameStrapPosition(ProAVVideoNameStrapPosition position) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Set pro AV gallery distribution mode.
     *
     * @param mode gallery distribution mode, see \link ProAVGalleryDistributionMode \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetProAVGalleryDistributionMode(ProAVGalleryDistributionMode mode) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Set pro AV max gallery page count.
     *
     * @param count max gallery page count.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetProAVMaxGalleryPageCount(uint32_t count) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Set scaling factor for the video element.
     *
     * @param scale The scaling factor to apply. Recommended range: 0.5 to 3.0.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetProAVVideoElementScale(float scale) = 0;
    
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Set pro AV video unassigned behavior.
     *
     * @param behavior pro AV video unassigned behavior, see \link ProAVUnassignedBehavior \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetProAVUnassignedBehavior(const ProAVUnassignedBehavior& behavior) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Query pro AV video unassigned behavior.
     *
     * @param behavior pro AV video unassigned behavior, see \link ProAVUnassignedBehavior \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetProAVUnassignedBehavior(ProAVUnassignedBehavior& behavior) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Set pro AV video loss behavior.
     *
     * @param behavior pro AV video loss behavior, see \link ProAVVideoLossBehavior \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetProAVVideoLossBehavior(const ProAVVideoLossBehavior& behavior) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Query pro AV video loss behavior.
     *
     * @param behavior pro AV video loss behavior, see \link ProAVVideoLossBehavior \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetProAVVideoLossBehavior(ProAVVideoLossBehavior& behavior) = 0;
};


NS_ZRCSDK_END

#endif    // ZRC_IPROAVSERVICE_H

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
    ProAVGalleryDistributionModeAssigned = 2,       ///<Assigned: Users are either placed in reserved seats, or are given the first available unreserved seat.
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
    bool isActiveSpeakerSelfViewEnabled = true;                                 ///<TRUE indicates self view is enabled in active speaker output.
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

/// \brief Pro AV assigned seat info.
/// Represents a seat configuration in the assigned gallery mode.
///
struct ProAVAssignedSeatInfo
{
    uint32_t index = 0;         ///< The seat index position in the gallery.
    bool isReserved = false;    ///< TRUE indicates the seat is reserved for a specific user.
    std::string email;          ///< The email address of the user assigned to this seat (if reserved).
    std::string name;           ///< The display name of the user assigned to this seat (if reserved).
};

/// \brief Pro AV assigned gallery info.
/// Contains the configuration for assigned gallery mode, including seat assignments.
///
struct ProAVAssignedGalleryInfo
{
    std::vector<ProAVAssignedSeatInfo> seats;   ///< List of seat configurations in the assigned gallery.
    int64_t meetingId = 0;                      ///< The meeting ID this configuration applies to. 0 indicates an invalid or unspecified meeting ID.
};

/// \brief Pro AV assigned seat status.
/// Represents the current status of a seat, including which user is occupying it.
///
struct ProAVAssignedSeatStatus
{
    ProAVAssignedSeatInfo seat;     ///< The seat configuration information.
    std::string userGuid;           ///< The GUID of the user currently occupying this seat. Empty if the seat is unoccupied.
};

/// \brief Pro AV assigned gallery status.
/// Represents the real-time status of the assigned gallery, including whether the configuration
/// is successfully applied and the current seat occupancy details.
///
struct ProAVAssignedGalleryStatus
{
    bool fullUpdate = false;                        ///< TRUE indicates this is a full status update, FALSE indicates partial update.
    bool configApplied = false;                     ///< TRUE indicates the assigned gallery configuration has been successfully applied.
    std::vector<ProAVAssignedSeatStatus> seats;     ///< List of seat statuses showing current user occupancy.
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
    
    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Pro AV non-persistent assigned gallery update notification callback.
     * This notification is triggered when the non-persistent assigned gallery configuration is updated during a meeting.
     *
     * @param info pro AV assigned gallery info, see \link ProAVAssignedGalleryInfo \endlink struct.
     */
    virtual void OnProAVNonPersistentAssignedGalleryUpdate(const ProAVAssignedGalleryInfo& info) = 0;

    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Pro AV persistent assigned galleries update notification callback.
     * This notification is triggered when the persistent assigned gallery configurations are updated.
     *
     * @param infos pro AV assigned gallery infos, see \link ProAVAssignedGalleryInfo \endlink struct.
     */
    virtual void OnProAVPersistentAssignedGalleryUpdate(const std::vector<ProAVAssignedGalleryInfo>& infos) = 0;
    
    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Pro AV assigned gallery status update callback.
     * This callback is triggered when:
     * - Receiving a successful response to RequestProAVAssignedGalleryStatus()
     * - Receiving a successful response to DeleteProAVAssignedGallerySeat()
     * - ZR pushes updated status (e.g., when status changes)
     *
     * @param status The updated assigned gallery status, see \link ProAVAssignedGalleryStatus \endlink struct.
     * @param deleteIndices The indices of seats that were successfully deleted. Empty if this is not a delete operation response.
     */
    virtual void OnProAVAssignedGalleryStatusUpdate(const ProAVAssignedGalleryStatus& status, const std::vector<uint32_t>& deleteIndices) = 0;
    
    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Pro AV assigned gallery seat deletion failure callback.
     * This callback is triggered when DeleteProAVAssignedGallerySeat() fails to delete one or more seats.
     *
     * @param deleteIndices The indices of seats that failed to be deleted.
     */
    virtual void OnProAVDeleteAssignedGallerySeatFailed(const std::vector<uint32_t>& deleteIndices) = 0;
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
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Enable show self view in Pro AV Active Speaker outputs.
     *
     * @param enable TRUE indicates enable self view in active speaker outputs. Otherwise disable (hide self view).
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableProAVActiveSpeakerSelfView(bool enable) = 0;

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

    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Set pro AV assigned gallery configuration.
     * For non-persistent configurations, this method can only be called during a meeting.
     * Persistent configurations can be set at any time and apply to all future meetings.
     *
     * @param assignedGallery pro AV assigned gallery info, see \link ProAVAssignedGalleryInfo \endlink struct.
     * @param isPersistent TRUE indicates setting persistent configuration that applies to all meetings.
     *                     FALSE indicates setting non-persistent configuration that only applies to the current meeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetProAVAssignedGallery(const ProAVAssignedGalleryInfo& assignedGallery, bool isPersistent) = 0;

    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Query pro AV persistent assigned gallery infos.
     *
     * @param[out] infos pro AV persistent assigned gallery infos, see \link ProAVAssignedGalleryInfo \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetProAVPersistentAssignedGalleryInfos(std::vector<ProAVAssignedGalleryInfo>& infos) = 0;
    
    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Request pro AV assigned gallery status from ZR during a meeting.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RequestProAVAssignedGalleryStatus() = 0;
    
    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Delete pro AV assigned gallery seats during a meeting.
     *
     * @param deleteIndices The indices of seats to delete.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeleteProAVAssignedGallerySeat(const std::vector<uint32_t>& deleteIndices) = 0;
};


NS_ZRCSDK_END

#endif    // ZRC_IPROAVSERVICE_H

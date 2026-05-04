/*!
 * \file IVirtualBackgroundHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Virtual Background interfaces.
 */
#ifndef ZRC_IVIRTUALBACKGROUNDHELPER_H
#define ZRC_IVIRTUALBACKGROUNDHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum VBResourceType
    \brief Virtual background resource type.
    Here are more detailed structural descriptions.
 */
enum VBResourceType
{
    RESOURCE_TYPE_IMAGE = 0,    ///<Indicates a image virtual background
    RESOURCE_TYPE_VIDEO = 1     ///<Indicates a video virtual background
};

/// \brief Virtual background item.
///
struct VirtualBackgroundItem
{
    std::string     id;                 ///<ID. Special cases: 1. Empty string "" for NONE; 2."zr_virtual_background_blur_id" for Blur. // Share ZR current not support blur.
    std::string     name;               ///<Name
    std::string     bigImageURL;        ///<Big image url
    std::string     smallImageURL;      ///<Small image url
    bool            isDownloadedOnZR;   ///<TRUE indicates this item is downloaded on ZR and can be selected.
    VBResourceType  resourceType;       ///<See enum \link VBResourceType \endlink. 
    std::string     downloadFileId;     ///<If downloadFileId not empty, use downloadFileId to download the virtual background resource.
};

/// \brief Virtual background options.
///
struct VirtualBackgroundOptions
{
    bool isVirtualBackgroundFeatureOn = false;              ///<TRUE indicates virtual background feature is available.
    bool isSupportSmartVirtualBackground = false;           ///<TRUE indicates ZR supports smart vb algorithm.
    bool isSupportGreenScreenVirtualBackground = false;     ///<TRUE indicates ZR supports green screen algorithm
    bool isSupportBlurVirtualBackground = false;            ///<TRUE indicates ZR supports blur algorithm. You can switch to blur by SelectVirtualBackground with ID "zr_virtual_background_blur_id".
    bool isSettingLockedOnWeb = false;                      ///<TRUE indicates virtual background setting is locked by web option. Local config is disabled.
    bool isForceEnabledOnWeb = false;                       ///<TRUE indicates virtual background is force enabled by web option. SelectVirtualBackground with ID "" is disabled.(switch to NONE)
};

/// \brief Virtual background helper event callback.
///
class IVirtualBackgroundHelperSink
{
public:
    virtual ~IVirtualBackgroundHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Virtual background options update notification.
     *
     * @param options Virtual background options.
     */
    virtual void OnUpdateVirtualBackgroundOptions(const VirtualBackgroundOptions& options) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Virtual background list update notification.
     *
     * @param vbList The virtual background item list.
     */
    virtual void OnUpdateVirtualBackgroundList(const std::vector<VirtualBackgroundItem>& vbList) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Selected virtual background update notification.
     *
     * @param selectedVBID Selected virtual background image ID.
     */
    virtual void OnUpdateSelectedVirtualBackground(const std::string& selectedVBID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Virtual background enable green screen algorithm update notification.
     *
     * @param isEnableGreenScreen TRUE indicates green screen algorithm is enabled.
     */
    virtual void OnUpdateEnableGreenScreen(bool isEnableGreenScreen) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief In immersive cutout mode notification.
     *
     * @param isImmersiveCutOut TRUE indicates is in immersive cutout.
     */
    virtual void OnUpdateIsImmersiveCutOut(bool isImmersiveCutOut) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Detecting virtual background notification.
     *
     * @param isDetecting TRUE indicates is detecting.
     */
    virtual void OnUpdateIsDetectingVirtualBackground(bool isDetecting) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Detecting virtual background result notification.
     *
     * @param result [0] Success. Otherwise failed.
     */
    virtual void OnUpdateDetectVirtualBackgroundResult(int32_t result) = 0;

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
};

/// \brief Virtual background helper interface.
///
class IVirtualBackgroundHelper
{
public:
    virtual ~IVirtualBackgroundHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register virtual background helper callback sink.
     *
     * @param pSink A pointer to the IVirtualBackgroundHelperSink that receives the virtual background helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IVirtualBackgroundHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister virtual background callback sink.
     *
     * @param pSink A pointer to the IVirtualBackgroundHelperSink that receives the virtual background helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IVirtualBackgroundHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start preview virtual background in pre-meeting.
     *
     * @param start TRUE indicates to start preview virtual background.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PreviewVirtualBackground(bool start) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Select virtual background.
     *
     * @param vbID Select the virtual background ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectVirtualBackground(const std::string& vbID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Redetect virtual background.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RedetectVirtualBackground() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Download virtual background image
     *
     * @param imgUrl The virtual background image url.
     * @param localFilePath Specify the local file path of this image. For example, ..../MyVBFolder/this_vb.jpg
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DownloadVirtualBackgroundImage(std::string imgUrl, std::string localFilePath) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Notify ZR that controller opens virtual background view to trigger list update when in meeting.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError NotifyZRRefreshVirtualBackgroundListInMeeting() = 0;
};

NS_ZRCSDK_END

#endif //ZRC_IVIRTUALBACKGROUNDHELPER_H

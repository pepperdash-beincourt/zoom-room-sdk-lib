/*!
 * \file IHWIOHelper.h
 * \since Added in ZRC SDK 6.1.0
 * \brief HWIO interfaces.
 */
#ifndef IHWIOHelper_h
#define IHWIOHelper_h

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum HWIOChannelModality
    \brief HWIO channel modality.
    Here are more detailed structural descriptions.
 */
enum HWIOChannelModality
{
    HWIOChannelModalityNone = 0,        ///<HWIO channel modality is none.
    HWIOChannelModalityInput,           ///<HWIO channel modality is input.
    HWIOChannelModalityOutput,          ///<HWIO channel modality is output.
    HWIOChannelModalityInputAndOutput   ///<HWIO channel modality is input and output.
};

/*! \enum HWIOChannelConnectorType
    \brief HWIO channel connector type.
    Here are more detailed structural descriptions.
 */
enum HWIOChannelConnectorType
{
    HWIOChannelConnectorTypeUnspecified = 0,    ///<HWIO channel connector type is unspecified.
    HWIOChannelConnectorTypeSDI,                ///<HWIO channel connector type is SDI.
    HWIOChannelConnectorTypeHDMI,               ///<HWIO channel connector type is HDMI.
    HWIOChannelConnectorTypeEthernet            ///<HWIO channel connector type is ethernet.
};

/*! \enum HWIOChannelActivityStatus
    \brief HWIO channel activity status.
    Here are more detailed structural descriptions.
 */
enum HWIOChannelActivityStatus
{
    HWIOChannelActivityStatusOff = 0,       ///<HWIO channel is not running.
    HWIOChannelActivityStatusOutput = 1,    ///<HWIO channel is running for playback/output.
    HWIOChannelActivityStatusInput = 2,     ///<HWIO channel is running for capture/input.
};

/*! \enum HWIOVideoResolution
    \brief HWIO video resolution.
    Here are more detailed structural descriptions.
 */
enum HWIOVideoResolution
{
    HWIOVideoResolutionUnspecified = 0,  ///<HWIO video resolution is unspecified.
    HWIOVideoResolution720p,             ///<HWIO video resolution is 720p.
    HWIOVideoResolution1080p,            ///<HWIO video resolution is 1080p.
    HWIOVideoResolution2160p             ///<HWIO video resolution is 2160p.
};

/*! \enum HWIOVideoFrameRate
    \brief HWIO video frame rate.
    Here are more detailed structural descriptions.
 */
enum HWIOVideoFrameRate
{
    HWIOVideoFrameRateUnspecified = 0,  ///<HWIO video frame rate is unspecified.
    HWIOVideoFrameRate25fps,            ///<HWIO video frame rate is 25fps.
    HWIOVideoFrameRate29_97fps,         ///<HWIO video frame rate is 29_97fps.
    HWIOVideoFrameRate30fps,            ///<HWIO video frame rate is 30fps.
    HWIOVideoFrameRate60fps,            ///<HWIO video frame rate is 60fps.
    HWIOVideoFrameRate50fps,            ///<HWIO video frame rate is 50fps.
    HWIOVideoFrameRate59_94fps,         ///<HWIO video frame rate is 59_94fps.
};

/*! \enum HWIOVideoFormat
    \brief HWIO video format.
    Here are more detailed structural descriptions.
 */
enum HWIOVideoFormat
{
    HWIOVideoFormatUnspecified = 0,     ///<HWIO video format is unspecified.
    HWIOVideoFormatYUV420Limited,       ///<HWIO video format is YUV420 limited.
    HWIOVideoFormatYUV422UYVYLimited    ///<HWIO video format is 422UYVY limited.
};

/*! \enum HWIOAudioSampleRate
    \brief HWIO audio sample rate.
    Here are more detailed structural descriptions.
 */
enum HWIOAudioSampleRate
{
    HWIOAudioSampleRateUnspecified = 0,     ///<HWIO audio sample rate is unspecified.
    HWIOAudioSampleRate48000       = 48000  ///<HWIO audio sample rate is 48000.
};

/*! \enum HWIOAudioChannels
    \brief HWIO audio channels.
    Here are more detailed structural descriptions.
 */
enum HWIOAudioChannels
{
    HWIOAudioChannelsUnspecified = 0,   ///<HWIO audio channels is unspecified.
    HWIOAudioChannelsMONO        = 1,   ///<HWIO audio channels is MONO.
    HWIOAudioChannelsStereo      = 2,   ///<HWIO audio channels is stereo.
    HWIOAudioChannelsFour        = 4,   ///<HWIO audio channels is four.
    HWIOAudioChannelsEight       = 8,   ///<HWIO audio channels is eight.
    HWIOAudioChannelsSixteen     = 16,  ///<HWIO audio channels is sixteen.
    HWIOAudioChannelsThirtyTwo   = 32   ///<HWIO audio channels is thirty two.
};

/*! \enum HWIOAudioFormat
    \brief HWIO audio format.
    Here are more detailed structural descriptions.
 */
enum HWIOAudioFormat
{
    HWIOAudioFormatUnspecified = 0,             ///<HWIO audio format is unspecified.
    HWIOAudioFormatLinearPCM16BitInterleaved    ///<HWIO audio format is linear PCM 16 bit Interleaved.
};

/*! \enum HWIOVideoAssignmentType
    \brief HWIO video assignment type.
    Here are more detailed structural descriptions.
 */
enum HWIOVideoAssignmentType
{
    HWIOVideoAssignmentTypeUnassigned = 0,          ///<HWIO video assignment type  is unspecified.
    HWIOVideoAssignmentTypeOutputFromUser,          ///<HWIO video assignment type  is output from user.
    HWIOVideoAssignmentTypeInputToVirtualInput,     ///<HWIO video assignment type  is input to virtual input.
    HWIOVideoAssignmentTypeOutputTestSignal,        ///<HWIO video assignment type  is output test signal.
    HWIOVideoAssignmentTypeOutputScreenShare,       ///<HWIO video assignment type  is output screen share.
    HWIOVideoAssignmentTypeOutputActiveSpeaker,     ///<HWIO video assignment type  is output active speaker.
    HWIOVideoAssignmentTypeOutputPinGroup,          ///<HWIO video assignment type  is output pin group.
    HWIOVideoAssignmentTypeOutputSpotlightGroup,    ///<HWIO video assignment type  is output spotlight group.
    HWIOVideoAssignmentTypeOutputGalleryView        ///<HWIO video assignment type  is output gallery view.
};

/*! \enum HWIOAudioAssignmentType
    \brief HWIO audio assignment type.
    Here are more detailed structural descriptions.
 */
enum HWIOAudioAssignmentType
{
    HWIOAudioAssignmentTypeUnassigned = 0,          ///<HWIO audio assignment type is unspecified.
    HWIOAudioAssignmentTypeOutputFromUser,          ///<HWIO audio assignment type is output from user.
    HWIOAudioAssignmentTypeInputToVirtualInput,     ///<HWIO audio assignment type is input to virtual input.
    HWIOAudioAssignmentTypeOutputMixedMinus         ///<HWIO audio assignment type is output mixed minus.
};

/*! \enum HWIODeviceManufacturer
    \brief HWIO device manufacturer.
    Here are more detailed structural descriptions.
 */
enum HWIODeviceManufacturer
{
    HWIODeviceManufacturerUnspecified = 0,  ///<HWIO device manufacturer is unspecified.
    HWIODeviceManufacturerBlackMagic,       ///<HWIO device manufacturer is black magic.
    HWIODeviceManufacturerAJA,              ///<HWIO device manufacturer is AJA.
    HWIODeviceManufacturerMagewell,         ///<HWIO device manufacturer is magewell.
    HWIODeviceManufacturerDeltacast         ///<HWIO device manufacturer is deltacast.
};

/*! \enum HWIODeviceConnectionType
    \brief HWIO device connection type.
    Here are more detailed structural descriptions.
 */
enum HWIODeviceConnectionType
{
    HWIODeviceConnectionTypeUnspecified = 0,    ///<HWIO device connection type is unspecified.
    HWIODeviceConnectionTypeThunderBolt,        ///<HWIO device connection type is thunder bolt.
    HWIODeviceConnectionTypePcie,               ///<HWIO device connection type is PCIe.
    HWIODeviceConnectionTypeUSB                 ///<HWIO device connection type is USB.
};

/*! \enum HWIODeviceUpdateEventType
    \brief HWIO device update event type.
    Here are more detailed structural descriptions.
 */
enum HWIODeviceUpdateEventType
{
    HWIODeviceUpdateEventTypeDeviceAdded = 0,               ///<Device added.
    HWIODeviceUpdateEventTypeDeviceRemoved = 1,             ///<Device removed.
    HWIODeviceUpdateEventTypeDeviceChanged = 2,             ///<Device changed.
    HWIODeviceUpdateEventTypeDeviceActivityStatus = 3,      ///<Device channel activity status updated.
    HWIODeviceUpdateEventTypeDeviceDetectedInputSignal = 4, ///<Device channel input signal detected.
};

/*! \enum HWIODeviceResult
    \brief HWIO device result
    Here are more detailed structural descriptions.
 */
enum HWIODeviceResult
{
    HWIODeviceResultSuccess = 0,                    ///<Success.
    HWIODeviceResultInvalidDeviceID = 1,            ///<Invalid device ID.
    HWIODeviceResultInvalidChannelID = 2,           ///<Invalid channel ID.
    HWIODeviceResultInvalidConfiguration = 3,       ///<Invalid configuration.
    HWIODeviceResultInvalidAssignment = 4,          ///<Invalid assignment.
    HWIODeviceResultDeviceInaccessible = 5,         ///<Device inaccessible.
    HWIODeviceResultServiceUnavailable = 6,         ///<Service unavailable.
    HWIODeviceResultOperationNotAllowed = 7,        ///<Operation not allowed.
    
    HWIODeviceResultUnknownError = 255              ///<Unknown error.
};

/*! \enum HWIOVideoConvertPreferenceType
    \brief HWIO video convert preference type
    Here are more detailed structural descriptions.
 */
enum HWIOVideoConvertPreferenceType
{
    HWIOVideoConvertPreferenceTypeUnknown = -1,     ///< Unknown
    HWIOVideoConvertPreferenceTypeZoom = 0,         ///< Prefer Zoom software for video conversion.
    HWIOVideoConvertPreferenceTypeDevice = 1        ///< Prefer device hardware for video conversion.
};


/// \brief HWIO video settings
///
struct HWIOVideoSettings
{
    HWIOVideoResolution resolution = HWIOVideoResolutionUnspecified;    ///<HWIO video resolution, see \link HWIOVideoResolution \endlink enum.
    HWIOVideoFrameRate  frameRate = HWIOVideoFrameRateUnspecified;      ///<HWIO video frame rate, see \link HWIOVideoFrameRate \endlink enum.
    HWIOVideoFormat     format = HWIOVideoFormatUnspecified;            ///<HWIO video frame format, see \link HWIOVideoFormat \endlink enum.
};

/// \brief HWIO audio settings
///
struct HWIOAudioSettings
{
    HWIOAudioSampleRate sampleRate = HWIOAudioSampleRateUnspecified;    ///<HWIO audio sample rate, see \link HWIOAudioSampleRate \endlink enum.
    HWIOAudioChannels   channels = HWIOAudioChannelsUnspecified;        ///<HWIO audio channels, see \link HWIOAudioChannels \endlink enum.
    HWIOAudioFormat     format = HWIOAudioFormatUnspecified;            ///<HWIO audio format, see \link HWIOAudioFormat \endlink enum.
};

/// \brief HWIO configuration
///
struct HWIOConfiguration
{
    HWIOVideoSettings videoSettings;    ///<HWIO video settings, see \link HWIOVideoSettings \endlink struct.
    HWIOAudioSettings audioSettings;    ///<HWIO audio settings, see \link HWIOAudioSettings \endlink struct.
};

/// \brief HWIO device configuration
///
struct HWIODeviceConfiguration
{
    HWIOChannelModality modality = HWIOChannelModalityNone; ///<HWIO channel modality, see \link HWIOChannelModality \endlink enum.
    int32_t             deviceID = 0;                       ///<Configuration device ID.
    int32_t             channelID = -1;                     ///<Configuration channel ID. 0 means apply to all channels, positive value means a specific channel.
    HWIOConfiguration   configuration;                      ///<HWIO configuration, see \link HWIOConfiguration \endlink struct.
};

/// \brief HWIO video assignment
///
struct HWIOVideoAssignment
{
    HWIOVideoAssignmentType type = HWIOVideoAssignmentTypeUnassigned;   ///<HWIO video assignment type, see \link HWIOVideoAssignmentType \endlink enum.
    std::string             userGuid;                  ///<User GUID, valid for type is HWIOVideoAssignmentTypeOutputFromUser.
    uint32_t                virtualInputID = 0;        ///<Virtual input ID, 1-indexed, valid for type is HWIOVideoAssignmentTypeInputToVirtualInput.
    uint32_t                testSignalFrameRGB = 0;    ///<Test signal from RGB, valid for type is HWIOVideoAssignmentTypeOutputTestSignal.
    uint32_t                screenShare = 0;           ///<Share screen index, 1-indexed, valid for type is HWIOVideoAssignmentTypeOutputScreenShare.
    uint32_t                pinGroupScreen = 0;        ///<Pin group screen index, 1-indexed, valid for type is HWIOVideoAssignmentTypeOutputPinGroup.
    uint32_t                galleryPage = 0;           ///<Gallery page, 1-indexed, valid for type is HWIOVideoAssignmentTypeOutputGalleryView.
    GalleryGridSize         gridSize;                  ///<Gallery view grid size, see \link GalleryGridSize \endlink struct, valid for type is HWIOVideoAssignmentTypeOutputGalleryView.
};

/// \brief HWIO audio assignment
///
struct HWIOAudioAssignment
{
    HWIOAudioAssignmentType type = HWIOAudioAssignmentTypeUnassigned;   ///<HWIO audio assignment type, see \link HWIOAudioAssignmentType \endlink enum.
    std::string             userGuid;              ///<User GUID, valid for type is HWIOAudioAssignmentTypeOutputFromUser.
    uint32_t                virtualInputID = 0;    ///<Virtual input ID, 1-indexed, valid for type is HWIOAudioAssignmentTypeInputToVirtualInput.
};

/// \brief HWIO assignment status
///
struct HWIOAssignmentStatus
{
    HWIOVideoAssignment videoAssignment;    ///<HWIO video assignment, see \link HWIOVideoAssignment \endlink struct.
    HWIOAudioAssignment audioAssignment;    ///<HWIO audio assignment, see \link HWIOAudioAssignment \endlink struct.
};

/// \brief HWIO assign deviceInfo
///
struct HWIOAssignDeviceInfo
{
    int32_t                deviceID = 0;        ///<Assign device ID.
    int32_t                channelID = -1;      ///<Assign channel ID. 0 means apply to all channels, positive value means a specific channel.
    HWIOAssignmentStatus   assignmentStatus;    ///<Assignment status, see \link HWIOAssignmentStatus \endlink struct.
};

/// \brief HWIO channel detected input signal
///
struct HWIOChannelDetectedInputSignal
{
    std::string videoFormat;                    ///<Video format
    bool        videoFormatSupported = false;   ///<Video format supported.
};

/// \brief HWIOChannelInfo
///
struct HWIOChannelInfo
{
    uint32_t                       channelID = 0;                       ///<Channel ID. 0 means invalid/unused, positive value means a specific channel.
    HWIOChannelModality            modality = HWIOChannelModalityNone;  ///<Channel modality, see \link HWIOChannelModality \endlink enum.
    HWIOChannelConnectorType       connectorType = HWIOChannelConnectorTypeUnspecified; ///<Channel connector type, see \link HWIOChannelConnectorType \endlink enum.
    HWIOAssignmentStatus           assignmentStatus;                    ///<Assignment status, see \link HWIOAssignmentStatus \endlink struct.
    HWIOConfiguration              inputConfiguration;                  ///<Input configuration, see \link HWIOConfiguration \endlink struct.
    HWIOConfiguration              outputConfiguration;                 ///<Output configuration, see \link HWIOConfiguration \endlink struct.
    std::vector<HWIOVideoSettings> supportedInputVideoSettings;         ///<Supported input video setting list, see \link HWIOVideoSettings \endlink struct.
    std::vector<HWIOVideoSettings> supportedOutputVideoSettings;        ///<Supported output video setting list, see \link HWIOVideoSettings \endlink struct.
    std::vector<HWIOAudioSettings> supportedInputAudioSettings;         ///<Supported input audio setting list, see \link HWIOAudioSettings \endlink struct.
    std::vector<HWIOAudioSettings> supportedOutputAudioSettings;        ///<Supported output audio setting list, see \link HWIOAudioSettings \endlink struct.
    std::string                    name;                                ///<Channel name. If empty, use the parent device's name.
    HWIOVideoConvertPreferenceType videoConvertPreferenceType = HWIOVideoConvertPreferenceTypeUnknown; ///<Video convert preference, see \link HWIOVideoConvertPreferenceType \endlink enum.
    HWIOChannelActivityStatus      activityStatus = HWIOChannelActivityStatusOff;   ///<Channel activity status.
    HWIOChannelDetectedInputSignal detectedInputSignal;                  ///<Channel detected input signal. Valid only if input signal detection is enabled for the device.
};

/// \brief HWIO device information
///
struct HWIODeviceInfo
{
    uint32_t                     deviceID = 0;          ///<Device ID.
    HWIODeviceManufacturer       manufacturer = HWIODeviceManufacturerUnspecified;      ///<Device manufacturer, see \link HWIODeviceManufacturer \endlink enum.
    HWIODeviceConnectionType     connectionType = HWIODeviceConnectionTypeUnspecified;  ///<Device connection type, see \link HWIODeviceConnectionType \endlink enum.
    std::string                  name;                  ///<Name, such as "DeckLink Duo 2".
    std::string                  internalIdentifier;    ///<Internal identifier.
    std::string                  driverVersion;         ///<Driver version.
    std::vector<HWIOChannelInfo> channelInfos;          ///<Channel info list, see \link HWIOChannelInfo \endlink struct.
    bool                         inputSignalDetectionSupported = false;  ///<Input signal detection supported.
    bool                         inputSignalDetectionEnabled = false;    ///<Input signal detection enabled.
};

/// \brief HWIO information
///
struct HWIOInfo
{
    std::vector<uint32_t>         virtualVideoInputIDs;         ///<Virtual video input ID list.
    std::vector<uint32_t>         virtualAudioInputIDs;         ///<Virtual audio input ID list.
    std::vector<HWIODeviceInfo>   deviceInfos;                  ///<Device info list, see \link HWIODeviceInfo \endlink struct.
    bool                          isServiceAvailable = false;   ///<TRUE indicates the HWIO service is enabled. Otherwise not.
    bool                          isFeatureAllowed = false;     ///<TRUE indicates the HWIO feature is allowed. Otherwise not.
};

/// \brief HWIO device update
///
struct HWIODeviceUpdate
{
    HWIODeviceUpdateEventType eventType = HWIODeviceUpdateEventTypeDeviceAdded; ///<HWIO device update event type, see \link HWIODeviceUpdateEventType \endlink enum.
    HWIODeviceInfo            deviceInfo;                                       ///<HWIO device info, used for device added, removed or changed events.
    int32_t                   deviceID = 0;                                     ///<Device ID for the updated device.
    int32_t                   channelID = -1;                                   ///<Channel ID for the updated channel. 0 means invalid/unused, positive value means a specific channel.
    HWIOChannelActivityStatus activityStatus = HWIOChannelActivityStatusOff;    ///<HWIO channel activity status, used for activity status update events.
    HWIOChannelDetectedInputSignal detectedInputSignal;                         ///<HWIO channel detected input signal, used for input signal detection events.
};

/// \brief HWIO video convert preference
///
struct HWIOVideoConvertPreference
{
    int32_t                        deviceID = 0;      ///<Device ID.
    int32_t                        channelID = -1;    ///<Channel ID. 0 means apply to all channels, positive value means a specific channel.
    HWIOVideoConvertPreferenceType type = HWIOVideoConvertPreferenceTypeUnknown; ///<Video convert preference type, see \link HWIOVideoConvertPreferenceType \endlink enum.
};

/// \brief HWIO input signal detection
///
struct HWIOInputSignalDetection
{
    int32_t deviceID = 0;           ///<Device ID.
    bool    enable = false;         ///<TRUE indicates the input signal detection is enabled. Otherwise not.
};

/// \brief HWIO helper event callback.
///
class IHWIOHelperSink
{
public:
    virtual ~IHWIOHelperSink() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief HWIO list device notification.
     *
     * @param result List devices result, see \link HWIODeviceResult \endlink enum.
     * @param info HWIO info, details \link HWIOInfo \endlink struct.
     * @param companionZRID CompanionZR's ID.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual void OnHWIOListDevicesResult(int32_t result, const HWIOInfo& info, const std::string& companionZRID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief HWIO configure device notification.
     *
     * @param result Configure device result, see \link HWIODeviceResult \endlink enum.
     * @param configuration HWIO device configuration, details \link HWIODeviceConfiguration \endlink struct.
     * @param companionZRID CompanionZR's ID.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual void OnHWIOConfigureDeviceResult(int32_t result, const HWIODeviceConfiguration& configuration, const std::string& companionZRID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief HWIO assign device notification.
     *
     * @param result Assign device result, see \link HWIODeviceResult \endlink enum.
     * @param assignDeviceInfo HWIO assign device info, details \link HWIOAssignDeviceInfo \endlink struct.
     * @param companionZRID CompanionZR's ID.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual void OnHWIOAssignDeviceResult(int32_t result, const HWIOAssignDeviceInfo& assignDeviceInfo, const std::string& companionZRID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief HWIO device update notification.
     *
     * @param deviceUpdate HWIO device update info, details \link HWIODeviceUpdate \endlink struct.
     * @param companionZRID CompanionZR's ID.
     *
     * @changelog
     *  - 6.6.0: Added deviceID and channelID fields to update specific channel information.
     *  - 6.6.0: Added activityStatus field to notify channel activity status changes.
     *  - 6.6.0: Added detectedInputSignal field to notify channel detected input signal changes.
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual void OnHWIODeviceUpdated(const HWIODeviceUpdate& deviceUpdate, const std::string& companionZRID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief HWIO service status update notification.
     *
     * @param isServiceAvailable TRUE indicates the Zoom Rooms HWIO service is available. Otherwise not.
     * @param isFeatureAllowed TRUE indicates "Enable Custom AV input and output devices" in Zoom Web Portal is enabled. Otherwise not.
     * @param companionZRID CompanionZR's ID.
     *
     * @changelog
     *  - 6.6.0: Add isFeatureAllowed field to indicate the Web Portal feature status.
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual void OnHWIOServiceStatusUpdated(bool isServiceAvailable, bool isFeatureAllowed, const std::string& companionZRID) = 0;

    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief HWIO set video convert preference notification.
     *
     * @param result Set video convert preference result, see \link HWIODeviceResult \endlink enum.
     * @param preference video convert preference, see \link HWIOVideoConvertPreference \endlink struct.
     * @param companionZRID CompanionZR's ID.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual void OnHWIOSetVideoConvertPreferenceResult(int32_t result, const HWIOVideoConvertPreference& preference,
                                                       const std::string& companionZRID) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief HWIO set input signal detection result notification.
     *
     * @param result Set input signal detection result, see \link HWIODeviceResult \endlink enum.
     * @param signalDetection Input signal detection, see \link HWIOInputSignalDetection \endlink struct.
     * @param companionZRID CompanionZR's ID.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual void OnHWIOSetInputSignalDetectionResult(int32_t result, const HWIOInputSignalDetection& signalDetection,
                                                     const std::string& companionZRID) = 0;
};

/// \brief HWIO helper interface
///
class IHWIOHelper
{
public:
    
    virtual ~IHWIOHelper() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Register HWIO helper callback sink.
     *
     * @param pSink A pointer to the IHWIOHelperSink that receives the HWIO helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IHWIOHelperSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Deregister meeting HWIO callback sink.
     *
     * @param pSink A pointer to the IHWIOHelperSink that receives the HWIO helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IHWIOHelperSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief List HWIO devices.
     *
     * @param companionZRID CompanionZR's ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual ZRCSDKError ListHWIODevices(const std::string& companionZRID = "") = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Configure HWIO device.
     *
     * @param configuration device configure, see \link HWIODeviceConfiguration \endlink struct.
     * @param companionZRID CompanionZR's ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual ZRCSDKError ConfigureHWIODevice(const HWIODeviceConfiguration& configuration, const std::string& companionZRID = "") = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Assign HWIO device.
     *
     * @param assignDevice assign device, see \link HWIOAssignDeviceInfo \endlink struct.
     * @param companionZRID CompanionZR's ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual ZRCSDKError AssignHWIODevice(const HWIOAssignDeviceInfo& assignDevice, const std::string& companionZRID = "") = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query HWIO service available.
     *
     * @param[out] available TRUE indicates the Zoom Rooms HWIO service is available. Otherwise not.
     * @param companionZRID CompanionZR's ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual ZRCSDKError IsHWIOServiceAvailable(bool& available, const std::string& companionZRID = "") = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.1.0
     *
     * @brief Query HWIO feature status.
     *
     * @param[out] allow TRUE indicates "Enable Custom AV input and output devices" in Zoom Web Portal is enabled. Otherwise not.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsHWIOFeatureAllowed(bool& allow) = 0;
    
    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Set HWIO video convert preference.
     *
     * @param preference Video convert preference, see \link HWIOVideoConvertPreference \endlink struct.
     * @param companionZRID CompanionZR's ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual ZRCSDKError SetHWIOVideoConvertPreference(const HWIOVideoConvertPreference& preference, const std::string& companionZRID = "") = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Set HWIO input signal detection.
     *
     * @param signalDetection Input signal detection, see \link HWIOInputSignalDetection \endlink struct.
     * @param companionZRID CompanionZR's ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.7.0: support HWIO for Mac&Win CZR.
     */
    virtual ZRCSDKError SetHWIOInputSignalDetection(const HWIOInputSignalDetection& signalDetection, const std::string& companionZRID = "") = 0;
};

NS_ZRCSDK_END

#endif /* ZRC_IHWIOHELPER_H */

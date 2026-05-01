/*!
 * \file ISettingService.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Entrance of all the Setting Service.
 * 
 * Setting describes media device information.
 */
#ifndef ZRC_ISETTINGSERVICE_H
#define ZRC_ISETTINGSERVICE_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

class ICalibrationHelper;

/*! \enum AudioCheckupCommand
    \brief Audio checkup command
    Here are more detailed structural descriptions.
 */
enum AudioCheckupCommand
{
    AudioCheckupCommandStart = 0,       ///<Start audio checkup.
    AudioCheckupCommandCancel           ///<Cancel audio checkup.
};

/*! \enum AudioCheckupStatus
    \brief Audio checkup status
    Here are more detailed structural descriptions.
 */
enum AudioCheckupStatus
{
    AudioCheckupStatusIdle = 0,         ///<Idle.
    AudioCheckupStatusScheduled,        ///<Scheduled.
    AudioCheckupStatusStarting,         ///<Starting.
    AudioCheckupStatusChecking,         ///<Checking.
    AudioCheckupStatusSucceeded,        ///<Succeeded.
    AudioCheckupStatusFailed,           ///<Failed.
    AudioCheckupStatusFailedLikely      ///<Failed likely.
};

/*! \enum AdvancedNoiseSuppressionMode
    \brief Advanced noise suppression mode.
    Here are more detailed structural descriptions.
 */
enum AdvancedNoiseSuppressionMode
{
    AdvancedNoiseSuppressionModeNone = -1,      ///<None, no advanced noise suppression mode.
    AdvancedNoiseSuppressionModeAuto = 0,       ///<Auto, automatically adjusts noise suppression.
    AdvancedNoiseSuppressionModeHigh,           ///<High, recommended for noisy environments.
    AdvancedNoiseSuppressionModeOff             ///<Off
};

/*! \enum MicRecordTestStatus
    \brief Microphone record test status.
    Here are more detailed structural descriptions.
 */
enum MicRecordTestStatus
{
    MicRecordTestStatusNone = 1,            ///<None
    MicRecordTestStatusRecording = 2,       ///<Microphone test is recording record.
    MicRecordTestStatusPlaying = 3,         ///<Microphone test is playing record.
};

/*! \enum ScreenResolutionStatus
    \brief Screen resolution status
    Here are more detailed structural descriptions.
 */
enum ScreenResolutionStatus
{
    ScreenResolutionStatusOptimizable = 0,      ///<Screen resolution status is optimizable.
    ScreenResolutionStatusOptimized = 1         ///<Screen resolution status is optimized.
};

/*! \enum ScreenSequenceCalibrationAction
    \brief Screen sequence calibration action type.
    Here are more detailed structural descriptions.
 */
enum ScreenSequenceCalibrationAction
{
    ScreenSequenceCalibrationNone = -1,         ///<None
    ScreenSequenceCalibrationStart = 0,         ///<Start calibration.
    ScreenSequenceCalibrationIdentify,          ///<Identify screen sequence.
    ScreenSequenceCalibrationConfirm,           ///<Confirm screen sequence calibration.
    ScreenSequenceCalibrationCancel,            ///<Cancel screen sequence calibration.
    ScreenSequenceCalibrationConfidenceStart,   ///<Start confidence monitor calibration.
    ScreenSequenceCalibrationConfidenceIdentify ///<Cancel confidence monitor calibration.
};

/*! \enum ChannelSignalType
    \brief Channel signal type
    Here are more detailed structural descriptions.
 */
enum ChannelSignalType
{
    ChannelSignalTypeUnknown = -1,              ///<Unknown.
    ChannelSignalTypeMono = 0,                  ///<Mono.
    ChannelSignalTypeStereoLeft,                ///<StereoLeft.
    ChannelSignalTypeStereoRight,               ///<StereoRight.
};

/*! \enum NetworkType
    \brief Network type
    Here are more detailed structural descriptions.
 */
enum NetworkType
{
    NetworkTypeUnknown = -1,                ///<Unknown.
    NetworkTypeWired = 0,                   ///<Wired.
    NetworkTypeWifi = 1,                    ///<Wi-Fi.
    NetworkTypePPP = 2,                     ///<Point-to-Point Protocol.
    NetworkType3G = 3                       ///<3G.
};

/*! \enum NetworkConnectionType
    \brief Network connection type
    Here are more detailed structural descriptions.
 */
enum NetworkConnectionType
{
    NetworkConnectionTypeUnknown = -1,          ///<Unknown
    NetworkConnectionTypeDirect = 0,            ///<Direct
    NetworkConnectionTypeCloud = 1,             ///<Cloud
};

/*! \enum NetworkAudioDeviceListAction
    \brief Network audio device list action type.
    Here are more detailed structural descriptions.
 */
enum NetworkAudioDeviceListAction
{
    NetworkAudioDeviceListActionUnknown = -1,         ///<Unknown.
    NetworkAudioDeviceListActionRefreshList,          ///<Refresh list.
    NetworkAudioDeviceListActionRemoveDevice,         ///<Remove device.
    NetworkAudioDeviceListActionAddDevice,            ///<Add device.
    NetworkAudioDeviceListActionUpdateDevice,         ///<Update device.
    NetworkAudioDeviceListActionUseDanteController    ///<Action use Dante controller.
};

/*! \enum NetworkAudioDeviceState
    \brief Network audio device state
    Here are more detailed structural descriptions.
 */
enum NetworkAudioDeviceState
{
    NetworkAudioDeviceStateNone = 0,        ///<None
    NetworkAudioDeviceStateAvailable,       ///<Available
    NetworkAudioDeviceStateConnecting,      ///<Connecting
    NetworkAudioDeviceStateConnected,       ///<Connected
    NetworkAudioDeviceStateDisconnected,    ///<Disconnected
    NetworkAudioDeviceStateError,           ///<Error
    NetworkAudioDeviceStateOccupied         ///<Occupied
};

/*! \enum NetworkAdapterUpdateType
    \brief Network adapter update type
    Here are more detailed structural descriptions.
 */
enum NetworkAdapterUpdateType
{
    NetworkAdapterUpdateTypeNone = -1,  ///<None
    NetworkAdapterUpdateTypeDante = 0,  ///<Network audio devices (Dante)
    NetworkAdapterUpdateTypeNRC = 1,    ///<Native Room Controls (NRC)
};

/*! \enum SystemAudioEnhancementsMode
    \brief System audio enhancements mode.
    Here are more detailed structural descriptions.
 */
enum SystemAudioEnhancementsMode
{
    SystemAudioEnhancementsModeNone = -1,       ///<System audio enhancements mode is None, indicates the system audio enhancements feature is not available.
    SystemAudioEnhancementsModeAuto = 0,        ///<System audio enhancements mode is Auto.
    SystemAudioEnhancementsModeOn,              ///<System audio enhancements mode is On.
    SystemAudioEnhancementsModeOff              ///<System audio enhancements mode is Off.
};

/*! \enum VoiceCommandProvider
    \brief Voice command provider.
 */
enum VoiceCommandProvider
{
    VoiceCommandProviderNone = 0,   ///<Voice command is disabled.
    VoiceCommandProviderAlexa,      ///<Alexa voice command.
    VoiceCommandProviderSensory,    ///<Sensory voice command.
    VoiceCommandProviderAIC,        ///<AIC voice command.
};

/// \brief Audio quality statistical
///
struct QualityStatisticalAudio
{
    uint32_t              sampleRate = 0xffffffff;  ///<Send sample rate.
    std::vector<uint32_t> recSampleRates;           ///<Receive sample rates if has multiple sources.
};

/// \brief Video quality statistical
///
struct QualityStatisticalVideo
{
    uint32_t fpsOfRecvMaxVideo = 0xffffffff;        ///<FPS of receive video .
    uint32_t widthOfMaxRecvVideo = 0xffffffff;      ///<Width of receive video.
    uint32_t heightOfMaxRecvVideo = 0xffffffff;     ///<Height of receive video.
    uint32_t resolutionOfSend = 0xffffffff;         ///<Resolution of send video.  width: resolutionOfSend & 0xFFFF, height: (resolutionOfSend >> 16) & 0xFFFF
    uint64_t fpsOfSend = 0xffffffff;                ///<FPS of send video.
};

/// \brief Share content quality statistical
///
struct QualityStatisticalShare
{
    uint32_t fpsOfRecvShare = 0xffffffff;           ///<FPS of receive share content.
    uint32_t widthOfRecvShare = 0xffffffff;         ///<Width of receive share content.
    uint32_t heightOfRecvShare = 0xffffffff;        ///<Height of receive share content.
    uint32_t resolutionOfSend = 0xffffffff;         ///<Resolution of send share content. width: resolutionOfSend & 0xFFFF, height: (resolutionOfSend >> 16) & 0xFFFF
    uint64_t fpsOfSend = 0xffffffff;                ///<FPS of send share content.
};

/// \brief Quality statistical info
///
struct QualityStatisticalInfo
{
    QualityStatisticalAudio audioStatisticalInfo;   ///<Audio quality statistical info.
    QualityStatisticalVideo videoStatisticalInfo;   ///<Video quality statistical info.
    QualityStatisticalShare shareStatisticalInfo;   ///<Share quality statistical info.
};

/// \brief Statistical network status info
///
struct StatisticalNetWorkStatusInfo
{
    uint32_t avgLossRatio = 0xffffffff;             ///<Average packet loss ratio.
    uint32_t maxLossRatio = 0xffffffff;             ///<Max packet loss ratio.
    uint32_t rtt = 0xffffffff;                      ///<Latency.
    uint32_t jitter = 0xffffffff;                   ///<Jitter.
    uint32_t rate = 0xffffffff;                     ///<Bandwidth rate.
};

/// \brief Statistical media info
///
struct StatisticalMediaInfo
{
    StatisticalNetWorkStatusInfo networkSendingStatus;      ///<Network sending statistical info.
    StatisticalNetWorkStatusInfo networkReceivingStatus;    ///<Network receiving statistical info.
    QualityStatisticalInfo       qualityStatisticalInfo;    ///<Quality statistical info.
};

/// \brief Statistical overall info
///
struct StatisticalOverallInfo
{
    int32_t               cpuCoreNumber = 0;                ///<CPU core number.
    float                 cpuFrequency = 0.0f;              ///<CPU frequency.
    int32_t               cpuZRUsage = 0;                   ///<CPU ZR usage.
    int32_t               cpuOverallUsage = 0;              ///<CPU overall usage.
    int32_t               memorySize = 0;                   ///<Memory size.
    int32_t               memoryZRUsage = 0;                ///<Memory ZR usage.
    int32_t               memoryOverallUsage = 0;           ///<Memory overall usage.
    NetworkType           networkType = NetworkTypeUnknown; ///<Network type, see \link NetworkType \endlink enum.
    std::string           proxy;                            ///<Proxy
    NetworkConnectionType netWorkConnectionType = NetworkConnectionTypeUnknown; ///<Network connection type.
    std::string           dataCenterRegionMessage;          ///<Data center region message.
    std::string           encryption;                       ///<Encryption type.
};

/// \brief Statistical phone network info
///
struct StatisticalPhoneNetworkInfo
{
    uint32_t      packetsNumber = 0;        ///<Packets number.
    uint32_t      frequency = 0;            ///<Frequency.
    float         packetLoss = 0.0f;        ///<Packet loss.
    float         packetLossMax = 0.0f;     ///<Packet loss max.
    uint32_t      jitter = 0;               ///<Jitter.
    float         bandwidth = 0.0f;         ///<Bandwidth.
    std::string   codec;                    ///<Codec.
};

/// \brief Statistical phone peer info
///
struct StatisticalPhonePeerInfo
{
    std::string                 peerNumber;             ///<Peer number.
    std::string                 localIp;                ///<Local IP
    uint32_t                    localPort = 0;          ///<Local port
    std::string                 remoteIp;               ///<Remote IP
    uint32_t                    remotePort = 0;         ///<Remote port
    uint32_t                    networkDelay = 0;       ///<Network delay
    StatisticalPhoneNetworkInfo networkSendingStatus;   ///<Network sending status
    StatisticalPhoneNetworkInfo networkReceivingStatus; ///<network receiving status
};

/// \brief Statistical phone info
///
struct StatisticalPhoneInfo
{
    std::string                           registerId;               ///<Register ID
    std::string                           registerServerIp;         ///<Register server IP
    uint32_t                              registerServerPort = 0;   ///<Register server port
    std::string                           networkSwitch;            ///<Network switch
    std::string                           localNetworkInterface;    ///<Local network interface.
    std::vector<StatisticalPhonePeerInfo> phonePeers;               ///<Phone peer info.
};

/// \brief Statistical diagnostic msg
///
struct DiagnosticMsg
{
    std::string field;    ///<field, For example: "Camera Capture Resolution"
    std::string
        description;    ///<description, For example: "3840×2160", may be empty since it might be binary or something else that is hard to represent
};

/// \brief Statistical diagnostic group
///
struct DiagnosticMsgGroup
{
    std::string                name;    ///<name
    std::vector<DiagnosticMsg> msgs;    ///<msgs
};

/// \brief Statistical diagnostic info
///
struct DiagnosticInfo
{
    std::vector<DiagnosticMsgGroup> groups;    ///<groups
};

/// \brief Statistical info
///
struct StatisticalInfo
{
    StatisticalOverallInfo overallInfo;     ///<Overall statistical info.
    StatisticalMediaInfo   audioInfo;       ///<Audio statistical info.
    StatisticalMediaInfo   videoInfo;       ///<Video statistical info.
    StatisticalMediaInfo   shareInfo;       ///<Share statistical info.
    StatisticalPhoneInfo   phoneInfo;       ///<Phone statistical info.
};

/// \brief Audio checkup info
///
struct AudioCheckupInfo
{
    AudioCheckupStatus status = AudioCheckupStatusIdle;     ///<Audio checkup status, see \link AudioCheckupStatus \endlink enum.
    int32_t            intervalAfterScheduled = 0;          ///<Interval after scheduled.
    int32_t            percentageOfCheckup = 0;             ///<Percentage of checkup progress
    bool               canRestartZoomRoomsSystem = false;   ///<TRUE indicates can restart ZR system
    int32_t            intervalAfterFinished = 0;           ///<Interval after finished
    int32_t            aecLevel = -1;                       ///<AEC level
    std::string        testTime;                            ///<Test time
};

/// \brief Room profile item.
///
struct RoomProfile
{
    std::string             ID;                 ///<ID
    std::string             name;               ///<Name
    bool                    isSelected = false; ///<TRUE indicates current selected.
    std::vector<int32_t>    issueDevices;       ///<Issue devices. Device type see \link DeviceType \endlink enum.
};

/// \brief Room profile list.
///
struct RoomProfileList
{
    std::vector<RoomProfile> roomProfileList;   ///<Room profile list.
};

/// \brief Room capability
///
struct RoomCapability
{
    bool supportAutoLoginOS = false;            ///<TRUE indicates room supports auto login os.
    bool supportRestartOS = false;              ///<TRUE indicates room supports restart os.
    bool notSupportDigitalSignage = false;      ///<TRUE indicates room not supports digital signage
    bool notSupportMicAdvancedOption = false;   ///<TRUE indicates room not supports microphone advance option.
};

/// \brief Adjust screens response
///
struct AdjustScreensRes
{
    int32_t                         result = 0;                              ///<Result, [0] success, otherwise failed.
    int32_t                         currentScreen = -1;                      ///<Current screen index.
    int32_t                         quantityOfScreens= -1;                   ///<Quantity of screens.
    ScreenSequenceCalibrationAction action = ScreenSequenceCalibrationNone;  ///<Screen sequence calibration action, see \link ScreenSequenceCalibrationAction \endlink enum.
};

/// \brief Screen information.
///
struct ScreenInfos
{
    int32_t quantityOfScreens = 1;                      ///<Quantity of screens.
    int32_t quantityOfCecAdapterAttachedScreens = 0;    ///<Quantity of CEC adapter attached screen.
    bool    hasConfidenceMonitor = false;               ///<TRUE indicates has confidence monitor.
    int32_t mainDisplayPosition = 0;                    ///<Main display position.
};

/*! \enum CalibrationAction
    \brief Calibration action type.
    Here are more detailed structural descriptions.
 */
enum CalibrationAction
{
    CalibrationActionPageAdjustCamera,              ///<Page adjust camera
    CalibrationActionPageCameraControl,             ///<Page camera layout
    CalibrationActionPageReadyToMove,               ///<Page ready to move
    CalibrationActionEventStartToCalibrate,         ///<Event, success and save the result, layout, preset, start to calibrate.
    CalibrationActionEventFinishToCheckResult,      ///<Event, finish to check result.
    CalibrationActionEventAcceptResult,             ///<Event, accept result.
    CalibrationActionEventStop,                     ///<Event, stop.

    // boundary
    CalibrationActionPageAskNeedSetCameraBoundary,  ///<Page ask need set camera boundary
    CalibrationActionEventSwitchBoundaryCamera,     ///<Event, switch boundary camera
    CalibrationActionEventAdjustBoundary,           ///<Event, adjust boundary
    CalibrationActionPageVerifyBoundary,            ///<Page verify boundary
    CalibrationActionPagePreAcceptBoundary,         ///<Page pre-accept boundary
    CalibrationActionEventAcceptBoundaryResult,     ///<Event, accept boundary result
    CalibrationActionPagePreviewIDBoundary,         ///<Page preview ID boundary
};

/// \brief Network audio channel info
///
struct NetworkAudioChannelInfo
{
    NetworkAudioDeviceState state = NetworkAudioDeviceStateNone;      ///<Network audio device state, see \link NetworkAudioDeviceState \endlink enum.
    ChannelSignalType       signalType = ChannelSignalTypeUnknown;    ///<Channel signal type, see \link ChannelSignalType \endlink enum.
    std::string             deviceId;                                 ///<Device ID
    std::string             channelName;                              ///<Channel Name
};

/// \brief Network audio device
///
struct NetworkAudioDevice
{
    NetworkAudioDeviceState state = NetworkAudioDeviceStateNone;    ///<Network audio device state, see \link NetworkAudioDeviceState \endlink enum.
    std::vector<NetworkAudioChannelInfo> channels;                  ///<Network audio channel info.
    std::string                          ID;                        ///<Device ID
    std::string                          name;                      ///<Device name
    bool                                 identifiable = false;      ///<TRUE indicates this device is identifiable.
};

/// \brief Binding information for audio channel and camera device.
///
struct AudioChannelAndCameraBindInfo
{
    std::string cameraDeviceID;         ///<Camera device ID.
    std::string networkDeviceName;      ///<Network audio device name.
    std::string networkChannelName;     ///<Network audio device channel name.
    int32_t rxChannelID = 0;            ///<Room input audio channel ID (range: 1–8) for Dante or similar systems.
};

/// \brief Intelligent director info.
///
struct IntelligentDirectorInfo
{
    bool    supportsDirectorMode = false;                   ///<TRUE indicates support director mode.
    bool    isCalibrationConfigured = false;                ///<TRUE indicates calibration is configured.
    bool    allowDirectorAndMultiCameraParallel = false;    ///<TRUE indicates allow director and multiCamera parallel.
    bool    isRegionLimited = false;                        ///<TRUE indicates the current region is limited.
    int32_t supportedCameraNumber = 3;                      ///<The number of support camera.
    int32_t multiCameraParallelNumInDirector = 0;           ///<The parallel number of multi camera in director.
    bool    supportSavePresetImage = false;                 ///<TRUE indicates support save preset image.
};

/// \brief Camera boundary configuration info
///
struct CameraBoundaryConfigurationInfo
{
    bool     supportsBoundary = false;          ///<TRUE indicates support Camera Boundary. Otherwise not.
    bool     isBoundaryConfigured = false;      ///<TRUE indicates the Camera Boundary has been configured. Otherwise not.
    uint32_t cameraBoundaryCapability = 0;      ///<Camera boundary capability.
    int32_t  cameraBoundaryEnableStatus = -1;   ///<Camera boundary enable status.
};

/// \brief Network adapter info
///
struct NetworkAdapterInfo
{
    NetworkAdapterUpdateType updateType = NetworkAdapterUpdateTypeNone; ///<Network adapter update type, see \link NetworkAdapterUpdateType \endlink enum.
    std::string              adapter;                                   ///<Adapter name.
    std::string              ip;                                        ///<IP address.
};

/// \brief AIC voice command runtime info.
///
struct AICVoiceCommandInfo
{
    bool isSessionStarted = false;  ///<TRUE indicates AIC voice session started.
    bool isPlaybackMuted = false;   ///<TRUE indicates AIC voice playback muted.
};

/// \brief Voice command status.
///
struct VoiceCommandStatus
{
    VoiceCommandProvider provider = VoiceCommandProviderNone;   ///<Current voice command provider.
    bool                 enable = false;                        ///<TRUE indicates voice command enabled.
    AICVoiceCommandInfo  aicVoiceCommandInfo;                   ///<AIC voice command runtime info.
};

/// \brief Setting service event callback.
///
class ISettingServiceSink
{
public:
    virtual ~ISettingServiceSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Microphone list changed callback.
     *
     * @param microphones Microphone list.
     */
    virtual void OnMicrophoneListChanged(const std::vector<Device>& microphones) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Speaker list changed callback.
     *
     * @param speakers Speaker list.
     */
    virtual void OnSpeakerListChanged(const std::vector<Device>& speakers) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Camera list changed callback.
     *
     * @param cameras Camera list.
     */
    virtual void OnCameraListChanged(const std::vector<Device>& cameras) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief COM list changed callback.
     *
     * @param comList COM list.
     */
    virtual void OnUpdateCOMList(const std::vector<Device>& comList) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Companion ZR device update notification.
     *
     * @param noti CZR update notification.
     */
    virtual void OnCompanionZRDeviceUpdateNotification(const CompanionZRDeviceUpdateNot& noti) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Current microphone changed callback.
     *
     * @param exist TRUE indicates exist current microphone.
     * @param microphone Microphone device.
     */
    virtual void OnCurrentMicrophoneChanged(bool exist, const Device& microphone) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Current speaker changed callback.
     *
     * @param exist TRUE indicates exist current speaker.
     * @param speaker Speaker device.
     */
    virtual void OnCurrentSpeakerChanged(bool exist, const Device& speaker) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Current camera changed callback.
     *
     * @param exist TRUE indicates exist current camera.
     * @param camera Camera device.
     */
    virtual void OnCurrentCameraChanged(bool exist, const Device& camera) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Current microphone volume changed callback.
     *
     * @param volume Microphone volume.
     */
    virtual void OnCurrentMicrophoneVolumeChanged(float volume) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Current speaker volume changed callback.
     *
     * @param volume Speaker volume
     */
    virtual void OnCurrentSpeakerVolumeChanged(float volume) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Hardware status changed callback.
     *
     * Some TV and ZoomRooms Applicance have privacy mute buttons, sync disable status.
     *
     * @param status Hardware status
     */
    virtual void OnUpdateHardwareStatus(const HardwareStatus& status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Generic settings changed callback.
     *
     * @param genericSettings Zoom Room generic settings.
     */
    virtual void OnUpdatedGenericSettings(const GenericSettings& genericSettings) = 0;

    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Voice command status changed callback.
     *
     * @param status Voice command status.
     */
    virtual void OnUpdateVoiceCommandStatus(const VoiceCommandStatus& status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update room profile list.
     *
     * @param list Room profile list.
     */
    virtual void OnUpdateRoomProfileList(const RoomProfileList& list) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update room capability
     *
     * @param roomCapability Room capability
     */
    virtual void OnUpdateZoomRoomCapability(const RoomCapability& roomCapability) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Current selected microphone muted notification.
     *
     * @param muted TRUE indicates microphone is mutes.
     */
    virtual void OnCurrentSelectedMicrophoneMuted(bool muted) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Microphone testing notification.
     *
     * @param volume Microphone testing real time volume.
     */
    virtual void OnMicrophoneTestingNotification(int32_t volume) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Microphone testing recording notification.
     *
     * @param status Microphone testing recording status, see \link MicRecordTestStatus \endlink enum.
     */
    virtual void OnMicrophoneRecordingNotification(MicRecordTestStatus status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Speaker testing notification.
     *
     * @param volume Speaker testing real time volume.
     * @param isEnabled TRUE indicates speaker testing is enabled.
     */
    virtual void OnSpeakerTestingNotification(int32_t volume, bool isEnabled) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Speaker testing result.
     *
     * @param result [0] Success.
     * @param duration Speaker testing duration.
     * @param isStopped TRUE indicates speaker testing is stopped.
     */
    virtual void OnSpeakerTestingResult(int32_t result, float duration, bool isStopped) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Statistical info notification.
     *
     * @param info The statistical info.
     */
    virtual void OnStatisticalInfoNotification(const StatisticalInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Audio checkup info notification.
     *
     * @param info The audio checkup info.
     */
    virtual void OnAudioCheckupNotification(const AudioCheckupInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Audio system failure notification.
     *
     * @param isDismiss TRUE indicates to dismiss the audio system failure alert. Otherwise to show the alert.
     */
    virtual void OnAudioSystemFailureNotification(bool isDismiss) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Screen infos notification.
     *
     * @param screenInfos The screen infos.
     */
    virtual void OnScreenInfosNotification(const ScreenInfos& screenInfos) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Adjust screen response.
     *
     * @param response Response of adjust screen.
     */
    virtual void OnAdjustScreensResponse(const AdjustScreensRes& response) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Room screen saver running notification.
     *
     * @param running TRUE indicates screen saver is running.
     */
    virtual void OnZoomPresenceScreenSaverNotification(bool running) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Operation time status notification.
     *
     * @param shouldDimScreen TRUE indicates to dim device screen.
     */
    virtual void OnUpdatedOperationTimeStatusNotification(bool shouldDimScreen) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Director calibration notification.
     *
     * @param noti The notification info.
     */
    virtual void OnDirectorCalibrationNotification(const DirectorCalibrationNot& noti) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Intelligent director info notification.
     *
     * @param info The notification info.
     */
    virtual void OnIntelligentDirectorInfoNotification(const IntelligentDirectorInfo& info) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Camera Boundary configuration info notification.
     *
     * @param info The notification info.
     */
    virtual void OnCameraBoundaryConfigurationInfoNotification(const CameraBoundaryConfigurationInfo& info) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Diagnostic info notification.
     *
     * @param info The diagnostic info.
     */
    virtual void OnUpdateDiagnosticInfo(const DiagnosticInfo& info) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Change windows password notification.
     *
     * @param result [0] Success. Otherwise failed.
     */
    virtual void OnChangeWindowsPasswordNotification(int32_t result) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Network audio device list update notification.
     *
     * @param virtualDeviceID Virtual device ID
     * @param action List action type, see \link NetworkAudioDeviceListAction \endlink enum.
     * @param networkAudioDeviceList The network audio device list.
     * @param isUsedDanteController TRUE indicates is used Dante controller.
     */
    virtual void OnUpdateNetworkAudioDeviceList(const std::string& virtualDeviceID, NetworkAudioDeviceListAction action,
                                                const std::vector<NetworkAudioDevice>& networkAudioDeviceList, bool isUsedDanteController) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief List audio channel and camera bind information notification.
     *
     * @param bindInfoList List of current audio channel and camera bindings, see \link AudioChannelAndCameraBindInfo \endlink enum.
     */
    virtual void OnListAudioChannelAndCameraBindInfoNotification(const std::vector<AudioChannelAndCameraBindInfo>& bindInfoList) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Bind audio channel and camera notification.
     *
     * @param bindInfoList Updated list of audio channel and camera bindings, see \link AudioChannelAndCameraBindInfo \endlink enum.
     */
    virtual void OnBindAudioChannelAndCameraNotification(const std::vector<AudioChannelAndCameraBindInfo>& bindInfoList) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Unbind audio channel and camera notification.
     *
     * @param bindInfoList Updated list of audio channel and camera bindings, see \link AudioChannelAndCameraBindInfo \endlink enum.
     */
    virtual void OnUnbindAudioChannelAndCameraNotification(const std::vector<AudioChannelAndCameraBindInfo>& bindInfoList) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Unbind all audio channel and camera connections notification.
     *
     * @param bindInfoList Updated list of audio channel and camera bindings, see \link AudioChannelAndCameraBindInfo \endlink enum.
     */
    virtual void OnUnbindAllAudioChannelAndCameraConnectionsNotification(const std::vector<AudioChannelAndCameraBindInfo>& bindInfoList) = 0;
 
    /**
     * @since 6.3.0
     * @zrminver 6.3.0
     *
     * @brief Network adapter info update notification.
     *
     * @param networkAdapterInfos Network adapter info list.
     */
    virtual void OnNetworkAdapterUpdateInfo(const std::vector<NetworkAdapterInfo>& networkAdapterInfos) = 0;

    /**
    * @since 6.6.0
    * @zrminver 6.6.0
    *
    * @brief Update microphone stethoscope mode enabled status notification.
    *
    * @param isMicStethoscopeModeEnabled TRUE indicates microphone stethoscope mode is enabled, otherwise not.
    */
    virtual void OnUpdateMicStethoscopeModeEnabledStatus(bool isMicStethoscopeModeEnabled) = 0;

    /**
     * @since 6.7.0
     * @zrminver 6.7.0
     *
     * @brief Update system audio enhancements mode notification.
     *
     * @param mode Current system audio enhancements mode. See \link SystemAudioEnhancementsMode \endlink enum.
     */
    virtual void OnUpdateSystemAudioEnhancementsMode(SystemAudioEnhancementsMode mode) = 0;
};

/// \brief Setting service interface.
///
class ISettingService
{
public:
    virtual ~ISettingService() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register setting service callback sink.
     *
     * @param pSink A pointer to the ISettingServiceSink that receives the setting service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(ISettingServiceSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister setting service callback sink.
     *
     * @param pSink A pointer to the ISettingServiceSink that receives the setting service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(ISettingServiceSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get microphone list.
     *
     * @param[out] microphones Microphone list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetMicrophoneList(std::vector<Device>& microphones) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get speaker list.
     *
     * @param[out] speakers Speaker list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSpeakerList(std::vector<Device>& speakers) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get camera list.
     *
     * @param[out] cameras Camera list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetCameraList(std::vector<Device>& cameras) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Get Companion ZR list.
     *
     * @param[out] CZRs Companion ZR list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetCompanionZRList(std::vector<CompanionZRDeviceInfo>& CZRs) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get network audio device list.
     *
     * You need to ListVirtualAudioDevices first before query the list.
     *
     * @param[in] virtualDeviceID Specify the virtual device ID.
     * @param[out] networkAudioDeviceList Camera list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetNetworkAudioDeviceList(const std::string& virtualDeviceID, std::vector<NetworkAudioDevice>& networkAudioDeviceList) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get current microphone.
     *
     * @param[out] microphone Current using microphone.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetCurrentMicrophone(Device& microphone) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get current speaker.
     *
     * @param[out] speaker Current using speaker.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetCurrentSpeaker(Device& speaker) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get current camera.
     *
     * @param [out] camera Current using camera.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetCurrentCamera(Device& camera) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set current microphone.
     *
     * @param microphone Specify the microphone to use.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetCurrentMicrophone(const Device& microphone) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set current speaker.
     *
     * @param speaker Specify the speaker to use.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetCurrentSpeaker(const Device& speaker) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set current camera.
     *
     * @param camera Specify the camera to use.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     * - 6.4.0: Added a restriction preventing an NDI camera that is currently being shared from being selected as the main camera.
     */
    virtual ZRCSDKError SetCurrentCamera(const Device& camera) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get current microphone volume.
     *
     * @param[out] volume Microphone volume.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetMicrophoneVolume(float& volume) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get current speaker volume.
     *
     * @param[out] volume Speaker volume.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
     virtual ZRCSDKError GetSpeakerVolume(float& volume) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set current microphone volume.
     *
     * @param volume Specify the volume to set.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetMicrophoneVolume(float volume) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set current speaker volume.
     *
     * @param volume Specify the volume to set.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetSpeakerVolume(float volume) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set current speaker volume temperately in current meeting.
     *
     * When meeting ends, speaker volume will fall back to the original volume.
     *
     * @param volume Specify the volume to set.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     *  - 6.6.0: Support adjust the volume of video (if customized in web setting) in waiting room
     */
    virtual ZRCSDKError SetSpeakerTempVolumeInMeeting(float volume) = 0;

    // microphone

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Test microphone record.
     *
     * @param on TRUE indicates to start testing microphone record.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError TestMicrophone(bool on) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start testing microphone volume.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartTestingMicrophoneVolume() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Stop testing microphone volume.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopTestingMicrophoneVolume() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Confirm detected microphone number.
     *
     * @param number The detected microphones' number.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ConfirmNumberOfCombinedMicrophone(int32_t number) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if ZR support acoustic echo cancellation
     *
     * @param[out] support TRUE indicates support. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsSupportAcousticEchoCancellation(bool& support) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable acoustic echo cancellation.
     *
     * @param enable TRUE indicates to enable. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     * - 6.6.0: Added a restriction can't enable acoustic echo cancellation when microphone stethoscope mode is enabled. Whether the microphone stethoscope mode is enabled, please refer to ISettingServiceSink::OnUpdateMicStethoscopeModeEnabledStatus.
     */
    virtual ZRCSDKError EnableAcousticEchoCancellation(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if ZR support advanced noise suppression
     *
     * @param [out] support TRUE indicates support. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsSupportAdvancedNoiseSuppression(bool& support) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get current advanced noise suppression mode.
     *
     * @param[out] mode Current advanced noise suppression mode.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetCurrentAdvancedNoiseSuppressionMode(AdvancedNoiseSuppressionMode& mode) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Select advanced noise suppression mode.
     *
     * @param mode See \link AdvancedNoiseSuppressionMode \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     * 
     * @changelog
     * - 6.3.5: May be unavailable while enable HIFI mode in some vendor device since zr 6.3.0
     * - 6.6.0: Added a restriction can't select advanced noise suppression mode when microphone stethoscope mode is enabled. Whether the microphone stethoscope mode is enabled, please refer to ISettingServiceSink::OnUpdateMicStethoscopeModeEnabledStatus.
     */
    virtual ZRCSDKError SelectAdvancedNoiseSuppressionMode(AdvancedNoiseSuppressionMode mode) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable hardware troubleshooting for the selected microphone.
     *
     * @param enable TRUE indicates to enable. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableMicrophoneHardwareTroubleshooting(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Audio echo test.
     *
     * @param command See \link AudioCheckupCommand \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AudioCheckup(AudioCheckupCommand command) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if ZR support audio framing.
     *
     * @param[out] available TRUE indicates ZR supports audio framing. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAudioFramingAvailable(bool& available) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable audio framing
     *
     * Suppress audio outside a 60° arc in front of the device. Participants must be within this arc to be heard. Use this feature in noisy open plan offices and avoid enabling it in regular Zoom Rooms.
     *
     * @param enable TRUE indicates to enable. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableAudioFraming(bool enable) = 0;

    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Enable or disable voice command with current provider.
     *
     * @param enable TRUE indicates to enable. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableVoiceCommand(bool enable) = 0;

    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Close current AIC voice command session.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CloseAICVoiceCommandSession() = 0;

    /**
     * @since 7.0.0
     * @zrminver 7.0.0
     *
     * @brief Mute or unmute AIC voice command playback.
     *
     * @param mute TRUE indicates to mute. Otherwise unmute.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteAICVoiceCommandPlayback(bool mute) = 0;

    // speaker

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start testing speaker
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartTestingSpeaker() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Stop testing speaker
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopTestingSpeaker() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if ZR support spatial audio.
     *
     * @param[out] available TRUE indicates ZR supports spatial audio. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsSpatialAudioAvailable(bool& available) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable spatial audio. Requires stereo loudspeaker hardware.
     *
     * @param enable TRUE indicates to enable. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableSpatialAudio(bool enable) = 0;
    
    // camera

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Select multiple camera
     *
     * @param deviceID Camera device ID.
     * @param isSelected TRUE indicates to select the camera as multiple camera.
     * @param companionZRID Specify the CZR ID if you want to control CZR's camera.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     * - 6.4.0: Added a restriction preventing an NDI camera that is currently being shared from being selected as the multiple camera.
     */
    virtual ZRCSDKError SelectMultipleCamera(const std::string& deviceID, bool isSelected, const std::string& companionZRID = "") = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Select director camera
     *
     * @param deviceID Camera device ID.
     * @param isSelected TRUE indicates to select the camera as director camera.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     * - 6.4.0: Added a restriction preventing an NDI camera that is currently being shared from being selected as the intelligent director camera.
     */
    virtual ZRCSDKError SelectIntelligentDirectorCamera(const std::string& deviceID, bool isSelected) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Director calibrate.
     *
     *     Director calibrate work flow:
     *
     *     Page change:
     *         CalibrationActionPageAdjustCamera   --> CalibrationActionPageReadyToMove
     *         CalibrationActionPageAdjustCamera  <--> CalibrationActionPageCameraControl
     *
     *     Start the director calibration process:         actionType = CalibrationActionPageAdjustCamera.
     *     In CalibrationActionPageAdjustCamera  page:     You can select the camera which you want to adjust(actionType = CalibrationActionPageCameraControl && deviceId = cameraID), and page will change to CalibrationActionPageCameraControl. If the cameras adjustment completed or the cameras don't need to adjust, you can change the page to CalibrationActionPageReadyToMove(actionType = CalibrationActionPageReadyToMove).
     *     In CalibrationActionPageCameraControl page:     You can adjust the camera position using the \link ControlLocalCamera \endlink API. After adjusting the camera position, you need to return to CalibrationActionPageAdjustCamera page(actionType = CalibrationActionPageReadyToMove).
     *     In CalibrationActionPageReadyToMove   page:     You can start the calibration(actionType = CalibrationActionEventStartToCalibrate), then you need to move and make sure the face appears in both cameras at the same time. After moving, you can finish moving(actionType = CalibrationActionEventFinishToCheckResult).
     *     If director calibration successes, you can accept the result(actionType = CalibrationActionEventAcceptResult), otherwise you can adjust the cameras and start calibrate again.
     *     And When you don't want to continue the calibration, you can stop the calibration(actionType = CalibrationActionEventStop).
     *
     * @param actionType Action type, see \link CalibrationAction \endlink enum.
     * @param deviceID Camera device ID. Only need when actionType is CalibrationActionPageCameraControl.
     * @param boundaryAdjustField The field which you want to adjust, such as \link CameraBoundaryAdjustField \endlink.
     * @param boundaryAdjustValue The value of the field. The range is [0, 100] when the filed is Left or Right. The range is [50, 150] when the field is Depth.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CalibrateIntelligentDirectorMode(CalibrationAction actionType, const std::string& deviceID = "", CameraBoundaryAdjustField boundaryAdjustField = CameraBoundaryAdjustFieldUnknown, int32_t boundaryAdjustValue = 0) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set camera com ID
     *
     * @param deviceID Camera device ID.
     * @param comID com ID. You can get com id from OnUpdateCOMList sink. If you want to restore after you have already set COM ID, you can set com id to -1. When set camera com id in companion zr, you should find com id in comDeviceList in \link CompanionZRDeviceInfo \endlink struct.
     * @param companionZRID Specify the CZR ID if you want to control CZR's camera.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetCameraCOMId(const std::string& deviceID, int32_t comID, const std::string& companionZRID = "") = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set camera display name.
     *
     * @param deviceID Camera device ID.
     * @param displayName Specify the camera display name.
     * @param companionZRID Specify the CZR ID if you want to control CZR's camera.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetCameraDisplayName(const std::string& deviceID, const std::string& displayName, const std::string& companionZRID = "") = 0;

    // General room setting action

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Select room profile.
     *
     * @param profileID The room profile ID
     * @param profileName The room profile name
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectRoomProfile(const std::string& profileID, const std::string& profileName) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable statistical info
     *
     * @param enable TRUE indicates to enable. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableStatisticalInfo(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start to adjust zoom room screens.
     *
     * If ZR has confidence monitor, always first adjust confidence monitor, then adjust ZR screens.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartAdjustZRScreens() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start over adjust zoom room screens.
     *
     * If ZR has confidence monitor, always first adjust confidence monitor, then adjust ZR screens.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartOverAdjustZRScreens() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Identify ZR confidence monitor, select a screen position to use as ZR confidence monitor.
     *
     * @param positionOfConfidenceMonitor Position, cannot set main display position. Range [2, quantityOfScreens]
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IdentifyZRConfidenceMonitor(int32_t positionOfConfidenceMonitor) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Identify zoom room screens.
     *
     * @param currentScreen Current in adjust screen.
     * @param positionOfCurrentScreen The position you want to set as current screen. Range [1, quantityOfScreens]
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IdentifyZRScreens(int32_t currentScreen, int32_t positionOfCurrentScreen) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Confirm adjust zoom room screens.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ConfirmAdjustZRScreens() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Cancel adjust zoom room screens.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CancelAdjustZRScreens() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Turn CEC screen on.
     *
     * @param on TRUE indicates to turn on. Otherwise to turn off.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError TurnCECScreensOn(bool on) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Manual refresh diagnostic info
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RefreshDiagnosticInfo() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get Windows IoT account name.
     *
     * @param[out] osAccountName IoT account name.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetWindowsIoTAccountName(std::string& osAccountName) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Change windows password
     *
     * @param oldPassword Windows old password.
     * @param newPassword Windows new password.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeWindowsPassword(const std::string& oldPassword, const std::string& newPassword) = 0;

    // virtual audio device

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief List virtual audio device.
     *
     * @param virtualDeviceID The virtual device ID
     * @param deviceType The virtual audio device type.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ListVirtualAudioDevices(const std::string& virtualDeviceID, VirtualAudioDeviceType deviceType) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Select virtual audio device.
     *
     * @param virtualDeviceID The virtual device ID
     * @param deviceType The virtual audio device type.
     * @param networkDeviceID The network device device ID.
     * @param channel Specify the channel info to select. If channel is default struct, this API will select one of the virtual audio device's channel.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectVirtualAudioDevice(const std::string& virtualDeviceID, VirtualAudioDeviceType deviceType,
                                                 const std::string& networkDeviceID, const NetworkAudioChannelInfo& channel) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Unselect virtual audio device.
     *
     * @param virtualDeviceID The virtual device ID
     * @param deviceType The virtual audio device type.
     * @param networkDeviceID The network device device ID.
     * @param channel Specify the channel info to unselect. If channel is default, this API will unselect all the virtual audio device's channels.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnselectVirtualAudioDevice(const std::string& virtualDeviceID, VirtualAudioDeviceType deviceType,
                                                   const std::string& networkDeviceID, const NetworkAudioChannelInfo& channel) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Identify virtual audio device.
     *
     * @param virtualDeviceID The virtual device ID
     * @param deviceType The virtual audio device type.
     * @param networkDeviceID The network device device ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IdentifyVirtualAudioDevice(const std::string& virtualDeviceID, VirtualAudioDeviceType deviceType,
                                                   const std::string& networkDeviceID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Use Dante controller
     *
     * Toggle this option is only available for VirtualAudioDeviceTypeMicrophone, for VirtualAudioDeviceTypeSpeaker isUsedDanteController always should be TRUE.
     *
     * @param virtualDeviceID The virtual device ID
     * @param deviceType The virtual audio device type.
     * @param isUsedDanteController TRUE indicates to use Dante controller.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UseDanteController(const std::string& virtualDeviceID, VirtualAudioDeviceType deviceType, bool isUsedDanteController) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if virtual audio device use Dante controller
     *
     * @param[in] virtualDeviceID Specify the virtual device ID
     * @param[out] isUseDanteController TRUE indicates virtual audio device uses Dante controller
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsUseDanteController(const std::string& virtualDeviceID, bool& isUseDanteController) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief List audio channel and camera binding information.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ListAudioChannelAndCameraBindInfo() = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Binds a camera to a specific input audio channel.
     *
     * @param cameraDeviceID The camera device ID.
     * @param rxChannelID The room input audio channel ID (range: 1–8) for Dante or similar systems.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BindCameraToAudioChannel(const std::string& cameraDeviceID, int32_t rxChannelID) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Unbind cameral from its associated audio channels.
     *
     * @param cameraDeviceID The camera device ID.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnbindCameraFromAudioChannel(const std::string& cameraDeviceID) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Unbind audio channel from its associated camera.
     *
     * @param rxChannelID The room input audio channel ID (range: 1–8) for Dante or similar systems.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnbindAudioChannelFromCamera(int32_t rxChannelID) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Unbind all audio channel and camera connections.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnbindAllAudioChannelAndCameraConnections() = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Rename CompanionZR
     *
     * @param czrID CompanionZR ID
     * @param displayName Specify the CompanionZR name.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RenameCompanionZR(const std::string& czrID, const std::string& displayName) = 0;

    /**
     * @since 6.3.0
     * @zrminver 6.3.0
     *
     * @brief Get network adapter info.
     *
     * @param[out] networkAdapterInfos network adapter info list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetNetWorkAdapterInfo(std::vector<NetworkAdapterInfo>& networkAdapterInfos) = 0;

    /**
     * @since 6.7.0
     * @zrminver 6.7.0
     *
     * @brief Get current system audio enhancements mode.
     *
     * @param[out] mode Current system audio enhancements mode.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetCurrentSystemAudioEnhancementsMode(SystemAudioEnhancementsMode& mode) = 0;

    /**
     * @since 6.7.0
     * @zrminver 6.7.0
     *
     * @brief Select system audio enhancements mode.
     *
     * @param mode See \link SystemAudioEnhancementsMode \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectSystemAudioEnhancementsMode(SystemAudioEnhancementsMode mode) = 0;
    
    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get the calibration helper.
     *
     * @return If the function succeeds, the return value is a pointer to ICalibrationHelper. Otherwise the function returns NULL.
     */
    virtual ICalibrationHelper* GetCalibrationHelper() = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Make ZR work in multi camera only mode
     *
     * @return If the function succeeds, the return value is a pointer to ICalibrationHelper. Otherwise the function returns NULL.
     */
    virtual ZRCSDKError EnableMultiCameraOnlyMode(bool enable) = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_ISETTINGSERVICE_H

/*!
 * \file IMeetingShareHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Meeting Share interfaces.
 */
#ifndef ZRC_IMEETINGSHAREHELPER_H
#define ZRC_IMEETINGSHAREHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum SharingState
    \brief Sharing state
    Here are more detailed structural descriptions.
 */
enum SharingState
{
    SharingStateNone,                  ///<None
    SharingStateConnecting,            ///<ZR is connecting share.
    SharingStateSending,               ///<ZR is sending share to meeting, means ZR is sharing.
    SharingStateReceiving,             ///<ZR is receiving share from meeting, means ZR is viewing other's share.
    SharingStateSendingAndReceiving    ///<ZR is both sending and receiving share in meeting.
};

/*! \enum ZRSharePrivilegeType
    \brief Zoom Room's share privilege type.
    Here are more detailed structural descriptions.
 */
enum ZRSharePrivilegeType
{
    ZRSharePrivilegeTypeEnabled = 0,                         ///<Zoom Room is enabled to share to meeting.
    ZRSharePrivilegeTypeDisabled = 1,                        ///<Zoom Room is disabled from sharing to meeting.
    ZRSharePrivilegeTypeDisabledParticipant = 2,             ///<Zoom Room is disabled from sharing to meeting because participants cannot share.
    ZRSharePrivilegeTypeDisabledWhileOthersSharing = 3,      ///<Zoom Room is disabled from sharing to meeting because others' sharing.
    ZRSharePrivilegeTypeDisabledWhileGuestsInMeeting = 4,    ///<Zoom Room is disabled from sharing to meeting because guests in meeting.
    ZRSharePrivilegeTypeDisabledWhileCloudWhiteboard = 5,    ///<Zoom Room is disabled from sharing to meeting because cloud whiteboard sharing.
    ZRSharePrivilegeTypeDisabledInBOWhileMainSessionSharing = 6,    ///<Zoom Room is disabled from sharing to meeting because in breakout room while main session sharing.
    ZRSharePrivilegeTypeDisabledStartShareForSimulive = 7,   ///<Zoom Room is disabled from sharing to meeting because ZR is in simulive webinars.
    ZRSharePrivilegeTypeDisabledStartShareForDSOnly = 8      ///<Zoom Room is disabled from sharing to meeting because ZR is Digital Signage-only.
};

/*! \enum MeetingSharePrivilegeType
    \brief Meeting share privilege type.
    Here are more detailed structural descriptions.
 */
enum MeetingSharePrivilegeType
{
    MeetingSharePrivilegeTypeUnknown = -1,    ///<Meeting share privilege type unknown.
    MeetingSharePrivilegeTypeHostGrab = 0,    ///<Anyone can share, but one sharing only at one moment, and only host can grab other's sharing
    MeetingSharePrivilegeTypeLockShare,       ///<Only host can share, the same as "lock share"
    MeetingSharePrivilegeTypeAnyoneGrab,      ///<Anyone can share, but one sharing only at one moment, and anyone can grab other's sharing
    MeetingSharePrivilegeTypeMultiShare       ///<Anyone can share, Multi-share can exist at the same time
};

/*! \enum MeetingShareViewPrivilege
    \brief Meeting share view privilege.
    Here are more detailed structural descriptions.
 */
enum MeetingShareViewPrivilege
{
    MeetingShareViewPrivilege_FocusModeOff = 0,           ///<Meeting share view privilege focus mode off.
    MeetingShareViewPrivilege_FocusModeHostOnly,          ///<Meeting share view privilege focus mode host only.
    MeetingShareViewPrivilege_FocusModeAllParticipants    ///<Meeting share view privilege focus mode all participants.
};

/*! \enum HDMI60FPSShareDisableReason
    \brief HDMI 60 FPS share disable reason.
    Here are more detailed structural descriptions.
 */
enum HDMI60FPSShareDisableReason
{
    HDMI60FPSShareDisableReasonUnknown = -1,                      ///<Unknown
    HDMI60FPSShareDisableReasonNotDisable = 0,                    ///<Not disabled.
    HDMI60FPSShareDisableReasonCaptureCardNotSupport = 1,         ///<Capture card not support.
    HDMI60FPSShareDisableReasonZRNotSupport = 2,                  ///<ZR not support
    HDMI60FPSShareDisableReasonCaptureCardAndZRNotSupport = 3,    ///<Capture and ZR not support.
    HDMI60FPSShareDisableReasonOptimizeVideoShareIsOff = 4,       ///<Optimize video share is off
    HDMI60FPSShareDisableReasonMultiShareIsOn = 5                 ///<Multi share is on.
};

/*! \enum CurrentShareType
    \brief Current pinned share type.
    Here are more detailed structural descriptions.
 */
enum CurrentShareType
{
    CurrentShareTypeUnknown = 0,          ///<Unknown.
    CurrentShareTypeNormal,               ///<Airplay, meeting HDMI or direct share.
    CurrentShareTypeCamera,               ///<Camera.
    CurrentShareTypeAnnotated,            ///<Annotated content, Airplay, meeting HDMI or direct share.
    CurrentShareTypeZoomApp,              ///<Zoom App.
    CurrentShareTypeWhiteboard,           ///<Whiteboard.
    CurrentShareTypeLocalHDMI,            ///<Normal local HDMI share.
    CurrentShareTypeAnnotatedLocalHDMI    ///<Local HDMI share converted to annotated mode when connected to Windows Zoom Rooms for touch support.
};

/*! \enum SlideOperationType
    \brief Slide control operation type.
    Here are more detailed structural descriptions.
 */
enum SlideOperationType
{
    SlideOperationTypeLeft = 0,     ///<Slide left
    SlideOperationTypeRight         ///<Slide right
};

/*! \enum DocsSharePrivilegeType
    \brief Docs share privilege type.
    Here are more detailed structural descriptions.
 */
enum DocsSharePrivilegeType
{
    DocsSharePrivilegeTypeUnknown = 0,     ///<Invalid
    DocsSharePrivilegeTypeHostGrab = 1,    ///<Only host can grab share when someone else is sharing.
    DocsSharePrivilegeTypeAnyoneGrab,      ///<Anyone can grab share when someone else is sharing.
};

/*! \enum DocsInitiatePrivilegeType
    \brief Docs initiate privilege type.
    Here are more detailed structural descriptions.
 */
enum DocsInitiatePrivilegeType
{
    DocsInitiatePrivilegeTypeUnknown = 0,        ///<Invalid
    DocsInitiatePrivilegeTypeHostOnly = 1,       ///<Only host can initiate new docs in the meeting.
    DocsInitiatePrivilegeTypeInternalUsers,      ///<Internal users can initiate new docs in the meeting.
    DocsInitiatePrivilegeTypeAllParticipants,    ///<All participants can initiate new docs in the meeting.
};

/// \brief Local presentation info
///
struct LocalPresentationInfo
{
    bool        success = false;    ///<TRUE indicates start local presentation success
    std::string meetingNumber;      ///<Local present meeting number.
    std::string meetingPassword;    ///<Local present meeting password.
};

/// \brief Sharing status
///
struct SharingStatus
{
    SharingState sharingState = SharingStateNone;    ///<Current sharing state.
    bool         canShareToBO = false;               ///<TRUE indicates can share to breakout room.
    bool         isSharingToBO = false;              ///<TRUE indicates ZR is sharing to breakout room.
};

/// \brief ZRW Sharing status
///
struct ZRWSharingStatus
{
    bool isSharing = false;        ///<TRUE indicates ZRW is sharing.
    bool canShareToBO = false;     ///<TRUE indicates can share ZRW to breakout room.
    bool isSharingToBO = false;    ///<TRUE indicates ZRW is sharing to breakout room.
};

/// \brief Share setting
///
struct ShareSetting
{
    bool                      isMultiShareOn = false;                                ///<TRUE indicates multi share is turned on. Otherwise not.
    bool                      isMultiShareDisabled = false;                          ///<TRUE indicates multi share is disabled. Otherwise not.
    ZRSharePrivilegeType      zrSharePrivilegeType = ZRSharePrivilegeTypeEnabled;    ///<Zoom Room's share privilege type.
    MeetingSharePrivilegeType meetingSharePrivilegeType = MeetingSharePrivilegeTypeUnknown;    ///<Meeting's share privilege type.
    bool                      isSharePrivilegeSettingLocked = false;    ///<TRUE indicates share privilege setting is locked. Otherwise not.
};

/// \brief Share source item
/// Every share in the meeting will show up as a share source.
struct ShareSource
{
    int32_t         userID = 0;                                  ///<User ID that the share source belongs to.
    int32_t         shareSourceID = 0;                           ///<Share source ID.
    ShareSourceType shareSourceType = ShareSourceTypeUnknown;    ///<Share source type.
    bool            isSharingAudio = false;                      ///<TRUE indicates share source with audio. Otherwise not.
    bool            isAudioMuted = false;                        ///<TRUE indicates share source audio is muted. Otherwise not.
    ConfInstType    fromType = ConfInstTypeUnknown;              ///<Share source from conf instance type.
};

/// \brief Airplay and black magic status
/// Airplay is Apple's Screen Mirroring, black magic is HDMI cable sharing.
struct AirplayBlackMagicStatus
{
    SharingInstructionDisplayState instructionDisplayState = SharingInstructionDisplayStateNone;    ///<Sharing instruction display state.

    std::string wifiName;                         ///<Airplay server WiFi name.
    std::string serverName;                       ///<Airplay server name.
    std::string password;                         ///<Airplay server password.
    std::string directPresentationPairingCode;    ///<Direct presentation pairing code.
    std::string directPresentationSharingKey;     ///<Direct presentation sharing key.

    bool isAirHostClientConnected = false;               ///<TRUE indicates AirHost client connected. Otherwise not.
    bool isBlackMagicConnected = false;                  ///<TRUE indicates HDMI cable connected. Otherwise not.
    bool isBlackMagicDataAvailable = false;              ///<TRUE indicates HDMI data is available and ready to be shared. Otherwise not.
    bool isSharingBlackMagic = false;                    ///<TRUE indicates HDMI is sharing. Otherwise not.
    bool isDirectPresentationConnected = false;          ///<TRUE indicates direct presentation connected. Otherwise not.
    bool isBlackMagicSharingLocallyAvailable = false;    ///<TRUE indicates share HDMI locally available. Otherwise not.
    bool isBlackMagicSharingLocally = false;             ///<TRUE indicates HDMI is sharing locally. Otherwise not.
};

/// \brief Camera sharing status.
///
struct CameraSharingStatus
{
    std::string deviceID;                      ///<Camera device ID.
    bool        isSharing = false;             ///<TRUE indicates camera is sharing. Otherwise not.
    bool        isMirrored = false;            ///<TRUE indicates camera is mirrored. Otherwise not.
    bool        canBeControlled = false;       ///<TRUE indicates camera can be controlled. Otherwise not.
    int32_t     panTiltSpeedPercentage = 0;    ///<Movement speed of camera when sharing
};

/// \brief Slide control info
///
struct SlideControlInfo
{
    int32_t     userID = 0;           ///<The user's ID who the share slide belongs to.
    std::string userName;             ///<The user's name who the share slide belongs to.
    int32_t     shareSourceID = 0;    ///<The share slide source ID.
};

/// \brief Docs share settings info
///
struct DocsShareSettingsInfo
{
    bool                      isSupported = false;                                     ///<True indicates the current meeting supports changing docs share settings. Otherwise not.
    bool                      isAllowParticipantsToShare = false;                      ///<True indicates allow participants to share docs. Otherwise not.
    DocsSharePrivilegeType    sharePrivilege = DocsSharePrivilegeTypeUnknown;          ///<Who can start share when someone else is sharing.
    DocsInitiatePrivilegeType initiatePrivilege = DocsInitiatePrivilegeTypeUnknown;    ///<Who can initiate new docs in the meeting.
    bool                      isLocked = false;                                        ///<TRUE indicates settings are locked. Otherwise not.
};

/// \brief Classic whiteboard share status
///
struct ClassicWhiteboardShareStatus
{
    bool    isSharing = false;      ///<TRUE indicates classic whiteboard is being shared.
    int32_t shareSourceID = -1;     ///<Share source ID. -1 if not available. The share that the shareSourceID refers to can be pinned on confidence monitor screen.
};

/// \brief Incoming share notification.
///
struct IncomingMeetingShareNot
{
    ShareSource      incomingSource;                                ///<Incoming share source.
    std::string      shareUserName;                                 ///<Incoming share user name.
    CurrentShareType currentShareType = CurrentShareTypeUnknown;    ///<Current pinned share type.
};

/// \brief Meeting Share helper event callback.
///
class IMeetingShareHelperSink
{
public:
    virtual ~IMeetingShareHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start local present meeting notification.
     *
     * @param info Local presentation info.
     */
    virtual void OnStartLocalPresentNotification(const LocalPresentationInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start local present meeting response.
     *
     * @param isSharingMeeting Start local present meeting request sharing meeting.
     * @param displayState Start local present meeting requested display state, see \link SharingInstructionDisplayState \endlink enum.
     */
    virtual void OnStartLocalPresentResult(bool isSharingMeeting, SharingInstructionDisplayState displayState) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Switch local presentation to normal meeting result.
     *
     * @param result [0] success.
     */
    virtual void OnSwitchToNormalMeetingResult(int result) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show or dismiss the sharing instruction on Zoom Room's screen result.
     *
     * @param result [0] success.
     * @param show TRUE indicates to show the instruction on ZR screen, FALSE indicates to dismiss the instruction on ZR screen.
     * @param instructionState The sharing instruction display state to show, see \link SharingInstructionDisplayState \endlink enum.
     */
    virtual void OnShowSharingInstructionResult(int result, bool show, SharingInstructionDisplayState instructionState) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Share setting changed callback.
     *
     * @param setting The updated share setting.
     */
    virtual void OnShareSettingNotification(const ShareSetting& setting) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Sharing status changed callback.
     *
     * @param status The updated sharing status.
     */
    virtual void OnSharingStatusNotification(const SharingStatus& status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Airplay black magic status changed callback.
     *
     * @param status The updated Airplay black magic status.
     */
    virtual void OnUpdateAirPlayBlackMagicStatus(const AirplayBlackMagicStatus& status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Camera sharing status changed callback.
     *
     * @param status The updated camera sharing status.
     */
    virtual void OnUpdateCameraSharingStatus(const CameraSharingStatus& status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Sharing source changed callback.
     *
     * @param zrShareSources All of meeting's sharing source list.
     * @param zrwShareSources Zoom Room Companion Whiteboard received share source list.
     */
    virtual void OnSharingSourceNotification(const std::vector<ShareSource>& zrShareSources, const std::vector<ShareSource>& zrwShareSources) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief HDMI 60 FPS share info notification.
     *
     * @param isAllow TRUE indicates hdmi 60 fps share is allowed.
     * @param isOn TRUE indicates hdmi 60 fps share is on.
     * @param disableReason Disable reason, see \link HDMI60FPSShareDisableReason \endlink enum.
     */
    virtual void OnHDMI60FPSShareInfoNotification(bool isAllow, bool isOn, HDMI60FPSShareDisableReason disableReason) = 0;
    
    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Local HDMI share audio playback notification.
     *
     * @param isEnabled TRUE indicates local HDMI share audio playback is enabled.
     */
    virtual void OnLocalHDMIShareAudioPlaybackNotification(bool isEnabled) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Classic whiteboard share status notification.
     *
     * @param status Classic whiteboard share status, see \link ClassicWhiteboardShareStatus \endlink.
     *
     * @changelog 
     * - 6.7.0: Renamed from OnUpdateWhiteboardShareStatusNotification to OnUpdateClassicWhiteboardShareStatusNotification.
     * - 6.7.0: Changed parameter from bool isSharing to ClassicWhiteboardShareStatus structure because added shareSourceID field.
     */
    virtual void OnUpdateClassicWhiteboardShareStatusNotification(const ClassicWhiteboardShareStatus& status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief ZRW Sharing status changed callback.
     *
     * @param status The updated ZRW sharing status.
     */
    virtual void OnZRWSharingStatusNotification(const ZRWSharingStatus& status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update local view status notification.
     *
     * @param isOn TRUE indicates local view for triple screens is configured and enabled. Otherwise not.
     */
    virtual void OnUpdateLocalViewStatus(bool isOn) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Incoming meeting share notification.
     *
     * @param noti The notification info.
     *
     * @changelog
     * - 6.4.0: In addition to the existing local HDMI share type, other CurrentShareType options have been added.
     */
    virtual void OnIncomingMeetingShareNotification(const IncomingMeetingShareNot& noti) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Slide control notification.
     *
     * @param slideControlInfos Available slide control infos.
     *
     * @changelog
     *  - 6.3.0: Changed from std::vector<MeetingParticipant>& slideControlUsers to std::vector<SlideControlInfo>& slideControlInfos for multi-session-share.
     */
    virtual void OnSlideControlNotification(const std::vector<SlideControlInfo>& slideControlInfos) = 0;

    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Docs share settings notification.
     *
     * @param info The docs share settings info.
     */
    virtual void OnDocsShareSettingsNotification(const DocsShareSettingsInfo& info) = 0;
};

/// \brief Meeting Share helper interface.
///
class IMeetingShareHelper
{
public:
    virtual ~IMeetingShareHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting share helper callback sink.
     *
     * @param pSink A pointer to the IMeetingShareHelperSink that receives the meeting share helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingShareHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting share helper callback sink.
     *
     * @param pSink A pointer to the IMeetingShareHelperSink that receives the meeting share helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingShareHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Launch a sharing meeting.
     *
     * @param isInLocalShare TRUE indicate to start a local presentation, FALSE indicates to start a sharing meeting.
     * @param displayState Specify the init display state, see \link SharingInstructionDisplayState \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LaunchSharingMeeting(bool isInLocalShare, SharingInstructionDisplayState displayState) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Switch local presentation to normal meeting.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SwitchFromLocalPresentationToNormalMeeting() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show or dismiss the sharing instruction on Zoom Room's screen.
     *
     * @param show TRUE indicates to show the instruction on ZR screen, FALSE indicates to dismiss the instruction on ZR screen.
     * @param instructionState The sharing instruction display state to show, see \link SharingInstructionDisplayState \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowSharingInstruction(bool show, SharingInstructionDisplayState instructionState) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Share HDMI.
     *
     * @param isStart TRUE indicates to start to share HDMI, FALSE indicates to stop to share HDMI.
     * @param isViewLocally TRUE indicates to share HDMI locally, FALSE indicates to share HDMI to the meeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShareBlackMagic(bool isStart, bool isViewLocally) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Share camera.
     *
     * @param isStart TRUE indicates to show the instruction on ZR screen, FALSE indicates to dismiss the instruction on ZR screen.
     * @param deviceId Device ID of the camera.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     * - 6.4.0: Added a restriction preventing an NDI camera that is currently selected as the main camera, multiple camera or intelligent director camera from being shared.
     */
    virtual ZRCSDKError ShareCamera(bool isStart, const std::string& deviceId) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Share the current source to all breakout rooms.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShareToBreakoutRooms() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Stop share to breakout rooms.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopShareToBreakoutRooms() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Stop Zoom Room's sharing.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopSharing() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Stop Zoom Room Companion Whiteboard's sharing.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopZRWSharing() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Turn on or off multi share function for current meeting.
     *
     * This API can only be called by host or co-host.
     *
     * @param enabled TRUE indicates to turn on multi share. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableMultiShare(bool enabled) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show the screen index of each display.
     *
     * @param show TRUE indicates to show the instruction on ZR screen. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowPinShareInstruction(bool show) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Pin share source on Zoom Room's screen.
     *
     * When PinShareWarningType from OnUpdateScreenStatusForPinNotification is PinShareWarningTypeNoAnnotationForSelf or PinShareWarningTypeStopCameraShare,
     * Need to set confirmed=TRUE to double confirm replace pin share on ZR screen.
     *
     * @param shareSource Share source to pin. Changed in version 6.3.0 for multi-session-share.
     * @param screenIndex Index of the screen to pin share, see \link MeetingScreen \endlink enum.
     * @param confirmed When screen already pinned other share source, replace to pin new share source on the screen needs set confirmed to TRUE.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PinShareOnZRScreen(const ShareSource& shareSource, int32_t screenIndex, bool confirmed) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Pin share source on Zoom Room Companion Whiteboard's screen.
     *
     * @param shareSource The Share source to pin. Changed in version 6.3.0 for multi-session-share.
     * @param screenIndex Index of ZRW's screen to pin share, see \link MeetingScreen \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError PinShareOnZRWScreen(const ShareSource& shareSource, int32_t screenIndex) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Pin incoming meeting share.
     *
     * @param incomingShare The source of the incoming share, see \link MeetingScreen \endlink enum.
     * @param currentShare Current pinned share type, see \link CurrentShareType \endlink enum.
     * @param pin TRUE indicates to pin incoming share source. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     *
     * @changelog
     * -6.4.0: In addition to the existing local HDMI share type, other CurrentShareType options have been extended.
     */
    virtual ZRCSDKError PinIncomingMeetingShare(const ShareSource& incomingShare, CurrentShareType currentShare, bool pin) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control slide
     *
     * @param slideInfo The slide info that you want to control. Changed in version 6.3.0 for multi-session-share.
     * @param type Operation type, see \link SlideOperationType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ControlSlide(const SlideControlInfo& slideInfo, SlideOperationType type) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute or unmute the sharing audio.
     *
     * @param source The Share source to control. Changed in version 6.3.0 for multi-session-share.
     * @param mute TRUE indicates to mute the sharing audio, FALSE indicates to unmute the sharing audio.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteShareAudio(const ShareSource& source, bool mute) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable HDMI share in 60fps
     *
     * @param enable TRUE indicates to enable HDMI share in 60fps. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableHDMI60FPSShare(bool enable) = 0;
    
    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Query local HDMI share audio playback status.
     *
     * @param isSupport TRUE indicates that enabling or disabling audio playback is supported. Otherwise not.
     * @param isEnabled TRUE indicates that HDMI share audio playback is enabled. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetLocalHDMIShareAudioPlaybackStatus(bool& isSupport, bool& isEnabled) = 0;
    
    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Enable local HDMI share audio playback.
     *
     * @param enable TRUE indicates to enable HDMI share audio playback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableLocalHDMIShareAudioPlayback(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set meeting share privilege type.
     *
     * @param type Meeting share privilege type, see \link MeetingSharePrivilegeType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetMeetingShareSetting(MeetingSharePrivilegeType type) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set meeting share view privilege.
     *
     * @param type Meeting share view privilege, see \link MeetingShareViewPrivilege \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetMeetingShareViewPrivilege(MeetingShareViewPrivilege type) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Optimize video sharing
     *
     * @param optimize TRUE indicates to enable optimize video sharing. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError OptimizeVideoSharing(bool optimize) = 0;

    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Allow participants share docs.
     *
     * @param allow TRUE indicates to allow participant to share docs. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowParticipantsShareDocs(bool allow) = 0;

    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Change docs share privilege type.
     *
     * @param type Specify the docs share privilege type.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeDocsSharePrivilege(DocsSharePrivilegeType type) = 0;

    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Change docs initiate privilege type.
     *
     * @param type Specify the docs initiate privilege type.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeDocsInitiatePrivilege(DocsInitiatePrivilegeType type) = 0;

    /**
     * @since 6.4.0
     * @zrminver 6.4.0
     *
     * @brief Get docs share settings info.
     *
     * @param[out] info The share docs setting info.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetDocsShareSettingsInfo(DocsShareSettingsInfo& info) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Enable annotation over HDMI (disabled by default in Android ZR due to performence)
     *
     * @param enable TRUE indicates to enable annotation over HDMI
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableAnnotationOverHDMI(bool enable) = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_IMEETINGSHAREHELPER_H

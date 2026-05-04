/*!
 * \file IThirdPartyMeetingHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Third Party Meeting interfaces.
 * 
 * 1. PSTN callout
 * 2. Room system callout
 */
#ifndef ZRC_ITHIRDPARTYMEETINGHELPER_H
#define ZRC_ITHIRDPARTYMEETINGHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum RoomSystemCallingStatus
    \brief Room system calling status
    Here are more detailed structural descriptions.
 */
enum RoomSystemCallingStatus
{
    RoomSystemCallingStatusAccepted = 0,                      ///<Call accepted
    RoomSystemCallingStatusRinging = 1,                       ///<Ringing
    RoomSystemCallingStatusTimeOut = 2,                       ///<Call time-out
    RoomSystemCallingStatusFailed = 3,                        ///<Call failed
    RoomSystemCallingStatusFailedNotSupportEncryption = 4,    ///<Call failed, not support encryption,
    RoomSystemCallingStatusExceedFreePorts = 5,               ///<Exceed free ports.
};

/*! \enum IntegrationMeetingState
    \brief Current integration meeting status.
    Here are more detailed structural descriptions.
 */
enum IntegrationMeetingState
{
    IntegrationMeetingStateNone = 0,        ///<Integration meeting state is none.
    IntegrationMeetingStateRejoining,       ///<Integration meeting state is rejoining.
    IntegrationMeetingStateJoining,         ///<Integration meeting state is joining.
    IntegrationMeetingStateWaitingRoom,     ///<Integration meeting state is in waiting room.
    IntegrationMeetingStateConnected,       ///<Integration meeting state is connected.
    IntegrationMeetingStateDisconnecting,   ///<Integration meeting state is disconnecting.
    IntegrationMeetingStateDisconnected,    ///<Integration meeting state is disconnected.
    IntegrationMeetingStateNeedPassword,    ///<Integration meeting needs the password.
};

/*! \enum IntegrationContentShareState
    \brief Current integration meeting content share status.
    Here are more detailed structural descriptions.
 */
enum IntegrationContentShareState
{
    IntegrationContentShareStateInactive = 0,    ///<Integration meeting content share state is inactive.
    IntegrationContentShareStateActive,          ///<Integration meeting content share state is active.
};

/*! \enum IntegrationMeetingLayoutType
    \brief Integration meeting layout type.
    Here are more detailed structural descriptions.
 */
enum IntegrationMeetingLayoutType
{
    INTEGRATION_MEETING_FULL_SCREEN = 1 << 0,
    INTEGRATION_MEETING_SIDEBAR_LEFT = 1 << 1,
    INTEGRATION_MEETING_SIDEBAR_RIGHT = 1 << 2,
    INTEGRATION_MEETING_GRID = 1 << 3,
    INTEGRATION_MEETING_TOP_BAR = 1 << 4,
    INTEGRATION_MEETING_BOTTOM_BAR = 1 << 5
};

/*! \enum IntegrationMeetingJoinMethod
    \brief Integration meeting join method.
    Here are more detailed structural descriptions.
 */
enum IntegrationMeetingJoinMethod
{
    IntegrationMeetingJoinMethodWebClient = 0,  ///<Integration meeting join method is web client.
    IntegrationMeetingJoinMethodSIP,            ///<Integration meeting join method is SIP.
};

/// \brief Integration meeting info
///
struct IntegrationMeetingInfo
{
    ThirdPartyMeetingServiceProvider provider = ThirdPartyMeetingServiceProviderInvalid;  ///<Integration meeting service provider, such as \link ThirdPartyMeetingServiceProvider \endlink enum.
    IntegrationMeetingState meetingState = IntegrationMeetingStateNone;                   ///<Integration meeting's state.
    std::string meetingTitle;                                                             ///<Integration meeting's title.
    std::string meetingID;                                                                ///<Integration meeting's ID.
    bool isAudioMuted = false;                                                            ///<TRUE indicates audio is muted. Otherwise not.
    bool isVideoMuted = false;                                                            ///<TRUE indicates video is stopped. Otherwise not.
    MeetingItem meetingListItem;                                                          ///<The meeting item in meeting list represents the current integration meeting.
    bool isSupportCameraControl = false;                                                  ///<TRUE indicates the current integration meeting supports camera control. Otherwise not.
};

/// \brief Inter operability info
///
struct InterOperabilityInfo
{
    ThirdPartyMeetingServiceProvider meetingType = ThirdPartyMeetingServiceProviderInvalid; ///<Integration meeting service type, such as \link ThirdPartyMeetingServiceProvider \endlink
    bool supportJoinMeeting = false;                                                        ///<TRUE indicates support join current integration meeting. Otherwise not.
    bool supportJoinWebClient = false;                                                      ///<TRUE indicates support join integration meeting via web client. Otherwise not.
    bool supportSipJoin = false;                                                            ///<TRUE indicates support join integration meeting via SIP. Otherwise not.
    bool supportPhoneJoin = false;                                                          ///<TRUE indicates support join integration meeting via phone. Otherwise not.
    std::vector<IntegrationMeetingJoinMethod> preferredJoinMethod;                          ///<Preferred join method when supportJoinWebClient and supportSipJoin both enabled. such as \link IntegrationMeetingJoinMethod \endlink.
    bool isPexipEnabled = false;                                                            ///<TRUE indicates Pexip is enabled. Otherwise not.
};

/// \brief Integration meeting error info
///
struct IntegrationMeetingErrorInfo
{
    int32_t errorCode = 0;      ///<Integration meeting's error code.
    std::string errorMessage;   ///<Integration meeting's error message.
};

/// \brief Integration meeting problem report info
///
struct IntegrationMeetingProblemReportInfo
{
    std::string correlationID;  ///<This is needed when report problem in current integration meeting.
};

/// \brief Integration meeting content share info
///
struct IntegrationMeetingContentShareInfo
{
    bool isHDMIContentShareAvailable = false;                                                   ///<TRUE indicates HDMI content share is available.
    IntegrationContentShareState contentShareState = IntegrationContentShareStateInactive;      ///<Content share state in current integration meeting.
};

/// \brief Integration meeting layout info
///
struct IntegrationMeetingLayoutInfo
{
    uint32_t availableLayoutType = 0;   ///<Supported layout type. Bit combination, see \link IntegrationMeetingLayoutType \endlink enum.
    uint32_t selectedLayoutType = 0;    ///<Selected layout type, see \link IntegrationMeetingLayoutType \endlink enum.
};

/// \brief Third Party Meeting helper event callback.
///
class IThirdPartyMeetingHelperSink
{
public:
    virtual ~IThirdPartyMeetingHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Zoom Room notify to join third party meeting.
     *
     * Join third party meeting method:
     * 1. DialSIP: StartThirdPartyMeetingByRoomSystemCall
     * 2. DialPSTN: StartThirdPartyMeetingByPSTNCall
     *
     * @param item The third party meeting item.
     */
    virtual void OnAskToJoinThirdPartyMeeting(const MeetingItem& item) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief PSTN callout status notification.
     *
     * @param callStatus The current PSTN callout status.
     */
    virtual void OnPSTNCallOutNotification(PSTNCallOutStatus callStatus) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief PSTN callout response.
     *
     * @param isCancel The current PSTN callout status.
     * @param isSwitchToNormalMeeting The current PSTN callout status.
     * @param phoneNumber The current PSTN callout status.
     * @param userName The current PSTN callout status.
     */
    virtual void OnPSTNCalloutResult(bool isCancel, bool isSwitchToNormalMeeting, const std::string& phoneNumber, const std::string& userName) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Room system joined notification.
     *
     * @param userID Room system as a participant userID.
     * @param userName Room system as a participant userName.
     */
    virtual void OnRoomSystemJoinedNotification(int32_t userID, const std::string& userName) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Room system calling status notification.
     *
     * @param status The updated calling status.
     */
    virtual void OnRoomSystemCallingStatusNotification(RoomSystemCallingStatus status) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start integration meeting result
     *
     * @param result [0] Success]
     * @param serviceProvider The service provider, see \link ThirdPartyMeetingServiceProvider \endlink enum.
     */
    virtual void OnStartIntegrationMeetingResult(int32_t result, int32_t serviceProvider) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief The integration meeting info update notification.
     *
     * @param info Meeting info about integration meeting, details \link IntegrationMeetingInfo \endlink struct.
     */
    virtual void OnUpdateIntegrationMeetingInfo(const IntegrationMeetingInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief The integration meeting error info update notification.
     *
     * @param info Integration meeting error info, details \link IntegrationMeetingErrorInfo \endlink struct.
     */
    virtual void OnUpdateIntegrationMeetingErrorInfo(const IntegrationMeetingErrorInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief The integration meeting content share info update notification.
     *
     * @param info Integration meeting content share info, details \link IntegrationMeetingContentShareInfo \endlink struct.
     */
    virtual void OnUpdateIntegrationMeetingContentShareInfo(const IntegrationMeetingContentShareInfo& info) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The integration meeting layout info update notification.
     *
     * @param info Integration meeting layout info, details \link IntegrationMeetingLayoutInfo \endlink struct.
     */
    virtual void OnUpdateIntegrationMeetingLayoutInfo(const IntegrationMeetingLayoutInfo& info) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Support integration teams meeting update notification.
     *
     * @param isSupport TRUE indicates supports integration teams meeting. Otherwise not.
     */
    virtual void OnUpdateSupportIntegrationTeamsMeeting(bool isSupport) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Support integration teams meeting for adhoc update notification.
     *
     * @param isSupport TRUE indicates supports integration teams meeting for adhoc. Otherwise not.
     */
    virtual void OnUpdateSupportIntegrationTeamsMeetingForAdhoc(bool isSupport) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Support integration Google meet update notification.
     *
     * @param isSupport TRUE indicates supports integration Google meet. Otherwise not.
     */
    virtual void OnUpdateSupportIntegrationGoogleMeet(bool isSupport) = 0;
};

/// \brief Third Party Meeting helper interface.
///
class IThirdPartyMeetingHelper
{
public:
    virtual ~IThirdPartyMeetingHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register third party meeting helper callback sink.
     *
     * @param pSink A pointer to the IThirdPartyMeetingHelperSink that receives the third party meeting helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IThirdPartyMeetingHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister third party meeting helper callback sink.
     *
     * @param pSink A pointer to the IThirdPartyMeetingHelperSink that receives the third party meeting helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IThirdPartyMeetingHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Dial out a PSTN user.
     *
     * @param phoneNumber Specify the phone number to be called.
     * @param cancelCall TRUE indicates to cancel the call.
     * @param hasVoicePrompt TRUE indicates to ring on Zoom Room.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CallOutPSTNUser(const std::string& phoneNumber, bool cancelCall, bool hasVoicePrompt) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start the third party meeting by PSTN call.
     *
     * @param meeting The third party meeting item.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartThirdPartyMeetingByPSTNCall(const MeetingItem& meeting) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Switch a PSTN call to normal Zoom Meeting.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SwitchPstnCallToMeeting() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start the third party meeting by room system call.
     *
     * @param meeting The third party meeting item.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartThirdPartyMeetingByRoomSystemCall(const MeetingItem& meeting) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Join integration meeting in meeting list.
     *
     * @param meeting The integration meeting item.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartIntegrationMeeting(const MeetingItem& meeting) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Join integration meeting via meeting id.
     *
     * @param meetingID The meeting id of the meeting.
     * @param password The password of the meeting.
     * @param  provider The service provider of the integration meeting, such as \link ThirdPartyMeetingServiceProvider \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError JoinIntegrationMeeting(const std::string& meetingID, const std::string& password, ThirdPartyMeetingServiceProvider provider) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Rejoin the integration meeting in the integration meeting
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RejoinIntegrationMeeting() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Leave the current integration meeting.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LeaveIntegrationMeeting() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute or unmute self audio.
     *
     * @param mute TRUE indicates mute self audio. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteIntegrationAudio(bool mute) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Stop or start self video.
     *
     * @param stop TRUE indicates stop self video. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopIntegrationVideo(bool stop) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Stop integration meeting content share.
     *
     * @param stop TRUE indicates stop. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopIntegrationContentShare(bool stop) = 0;
    
    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Change integration meeting layout.
     *
     * @param selectedLayoutType Specify the layout type.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeIntegrationLayout(uint32_t selectedLayoutType) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Get inter operability info by integration meeting type.
     *
     * @param[in] meetingType Integration meeting type, see \link ThirdPartyMeetingServiceProvider \endlink enum.
     * @param[out] info Inter operability info, see \link InterOperabilityInfo \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetInterOperabilityInfoByMeetingType(ThirdPartyMeetingServiceProvider meetingType, InterOperabilityInfo& info) = 0;
};

NS_ZRCSDK_END


#endif    //ZRC_ITHIRDPARTYMEETINGHELPER_H

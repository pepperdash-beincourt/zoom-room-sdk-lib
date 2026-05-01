/*!
 * \file IPhoneCallService.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Entrance of all the Phone Call Service.
 */
#ifndef ZRC_IPHONECALLSERVICE_H
#define ZRC_IPHONECALLSERVICE_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum SIPCallStatus
    \brief SIP call status.
    Here are more detailed structural descriptions.
*/
enum SIPCallStatus
{
    SIPCallStatusInit = 0,              ///<SIP call's status is init.
    SIPCallStatusCallOutFailed,         ///<SIP call's status is call out failed.
    SIPCallStatusIncoming,              ///<SIP call's status is incoming.
    SIPCallStatusRinging,               ///<SIP call's status is ringing.
    SIPCallStatusNotFound,              ///<SIP call's status is not found.
    SIPCallStatusBusy,                  ///<SIP call's status is busy.
    SIPCallStatusDeclined,              ///<SIP call's status is declined.
    SIPCallStatusNotAvailable,          ///<SIP call's status is not available.
    SIPCallStatusTimeout,               ///<SIP call's status is timeout.
    SIPCallStatusAccepted,              ///<SIP call's status is accepted.
    SIPCallStatusHold,                  ///<SIP call's status is hold.
    SIPCallStatusInCall,                ///<SIP call's status is in call.
    SIPCallStatusTerminated,            ///<SIP call's status is terminated.
    SIPCallStatusRemoteHold,            ///<SIP call's status is remote hold.
    SIPCallStatusBothHold,              ///<SIP call's status is both hold.
    SIPCallStatusSessionInProgress,     ///<SIP call's status is session in progress.
    SIPCallStatusStayOnPhone            ///<SIP call's status is stay on phone.
};

/*! \enum SIPCallConferenceRole
    \brief SIP call conference role in merged calls.
    Here are more detailed structural descriptions.
*/
enum SIPCallConferenceRole
{
    SIPCallConferenceRoleUnknown = 0,   ///<SIP call conference role is unknown.
    SIPCallConferenceRoleHost,          ///<SIP call conference role is host in merged calls.
    SIPCallConferenceRoleParticipant,   ///<SIP call conference role is participant in merged calls.
};

/*! \enum SIPCallSpamType
    \brief SIP call spam type.
    Here are more detailed structural descriptions.
*/
enum SIPCallSpamType
{
    SIPCallSpamTypeNone = 0,        ///<SIP call spam type is none.
    SIPCallSpamTypeNotSpam = 1,     ///<SIP call spam type is not spam.
    SIPCallSpamTypeSpam = 2,        ///<SIP call spam type is spam.
    SIPCallSpamTypeMaybeSpam = 3    ///<SIP call spam type is maybe spam.
};

/*! \enum SIPCallAttestLevel
    \brief SIP call attest level.
    Here are more detailed structural descriptions.
*/
enum SIPCallAttestLevel
{
    SIPCallAttestLevelUndefined = -1,   ///<SIP call attest level is undefined.
    SIPCallAttestLevelA = 0,            ///<SIP call attest level is A, represents high attest level.
    SIPCallAttestLevelB = 1,            ///<SIP call attest level is B, represents medium attest level.
    SIPCallAttestLevelC = 2             ///<SIP call attest level is C, represents low attest level.
};

/*! \enum SIPCallThirdPartyType
    \brief SIP call thirdparty type.
    Here are more detailed structural descriptions.
*/
enum SIPCallThirdPartyType
{
    SIPCallThirdPartyTypeDefault = 0,   ///<SIP call thirdparty type is default.
    SIPCallThirdPartyTypeTransfer = 1,  ///<SIP call thirdparty type is transfer.
};

/*! \enum EmergencyAddressType
    \brief emergency address type.
    Here are more detailed structural descriptions.
*/
enum EmergencyAddressType
{
    EmergencyAddressTypeUnknown = 0,    ///<Emergency address type is unknown.
    EmergencyAddressTypeStatic = 1,     ///<Emergency address type is static.
    EmergencyAddressTypeDetect = 2,     ///<Emergency address type is detect.
};

/*! \enum SIPCallTransferInfoType
    \brief SIP call transfer type.
    Here are more detailed structural descriptions.
*/
enum SIPCallTransferInfoType
{
    SIPCallTransferInfoTypeUnknown = -1,    ///<SIP call transfer type is unknown.
    SIPCallTransferInfoTypeBlind,           ///<SIP call transfer type is blind transfer.
    SIPCallTransferInfoTypeWarm,            ///<SIP call transfer type is warm transfer.
    SIPCallTransferInfoTypeWarmComplete,    ///<SIP call transfer type is warm complete.
    SIPCallTransferInfoTypeVoicemail        ///<SIP call transfer type is voicemail transfer.
};

/*! \enum SIPServiceStatus
    \brief SIP service status.
    Here are more detailed structural descriptions.
*/
enum SIPServiceStatus
{
    SIPServiceStatusIdle = 0,       ///<SIP service status is idle.
    SIPServiceStatusRegistering,    ///<SIP service status is registering.
    SIPServiceStatusRegFailed,      ///<SIP service status is registration failed.
    SIPServiceStatusRegistered,     ///<SIP service status is registered.
    SIPServiceStatusRinging,        ///<SIP service status is ringing.
    SIPServiceStatusCallingOut,     ///<SIP service status is calling out.
    SIPServiceStatusInCall          ///<SIP service status is in call.
};

/*! \enum SIPCallTerminateReason
    \brief SIP call terminate reason.
    Here are more detailed structural descriptions.
*/
enum SIPCallTerminateReason
{
    SIPCallTerminateReasonUnknown = 0,              ///<SIP call terminate reason is unknown.
    SIPCallTerminateReasonByLocal,                  ///<SIP call terminate reason is local termination.
    SIPCallTerminateReasonByRemote,                 ///<SIP call terminate reason is remote termination.
    SIPCallTerminateReasonByNetworkBreak,           ///<SIP call terminate reason is network break.
    SIPCallTerminateReasonByInitAudioDeviceFailed,  ///<SIP call terminate reason is init audio device failed.
    SIPCallTerminateReasonBySipServiceStopped       ///<SIP call terminate reason is sip service stopped.
};

/// \brief SIP call member info
/// The SIPCallMemberInfo refers to member information in the SIP call.
struct SIPCallMemberInfo
{
    std::string            name;                                        ///<Member's name in SIP call.
    std::string            number;                                      ///<Member's number in SIP call.
    SIPCallAttestLevel     attestLevel = SIPCallAttestLevelUndefined;   ///<Member's attest level in SIP call.
};

/// \brief SIP call conference info
/// The SIPCallConferenceInfo refers to call's info in merged calls.
struct SIPCallConferenceInfo
{
    SIPCallConferenceRole role = SIPCallConferenceRoleUnknown;  ///<SIP call' role in merged calls, such as \link SIPCallConferenceRole \endlink.
    std::string                  hostCallID;                    ///<SIP call's host call ID in merged calls.
};

/// \brief SIP call redirect info
/// The SIPCallRedirectInfo refers to call's redirect info. For instance, the redirect info refers to the peer info which a SIP call is transferred to when using the transfer function.
struct SIPCallRedirectInfo
{
    SIPCallThirdPartyType endType = SIPCallThirdPartyTypeDefault;   ///<SIP call redirect type, such as \link SIPCallThirdPartyType \endlink.
    std::string           endName;                                  ///<The person's name who the call is redirected to.
    std::string           endNumber;                                ///<The person's number who the call is redirected to.
};

/// \brief emergency call's address
/// 
struct EmergencyCallAddress
{
    EmergencyAddressType addressType = EmergencyAddressTypeUnknown; ///<Emergency call address's type.
    std::string          address;                                   ///<Emergency call address's address.
};

/// \brief Emergency call info
/// The info about emergency call.
struct EmergencyCall
{
    EmergencyCallAddress        emergencyCallAddress;               ///<Emergency call's address.
    bool                        locationPermissionEnabled = false;  ///<TRUE indicates the location permission is enable. Otherwise not.
    std::vector<std::string>    customEmergencyNumbers;             ///<The custom emergency numbers.
};

/// \brief SIP call info
///
struct SIPCallInfo
{
    SIPCallStatus                   status = SIPCallStatusInit;                     ///<The SIP call's status.
    std::string                     callID;                                         ///<The SIP call's callID.
    std::string                     peerDisplayName;                                ///<The SIP call's peer display name.
    std::string                     peerNumber;                                     ///<The SIP call's peer number.
    std::string                     peerURI;                                        ///<The SIP call's peer URI.
    bool                            isIncomingCall = false;                         ///<The SIP call is incoming call.
    SIPCallMemberInfo               selfInfo;                                       ///<The self info in the SIP call.
    SIPCallConferenceInfo           conferenceInfo;                                 ///<The SIP call's conference info.
    std::vector<SIPCallMemberInfo>  remoteMembers;                                  ///<The SIP call's members info.
    int64_t                         elapsedCallTime = -1;                           ///<The SIP call's elapsed time, elapsedCallTime's unit is second.
    std::string                     relatedCallID;                                  ///<The call's ID, which the current call is warm transferred to.
    std::string                     blindDisplayName;                               ///<The call's display name, which the current call is blind transferred from.
    std::string                     originalPeerURI;                                ///<The SIP call's original peer URI.
    SIPCallSpamType                 peerSpamType = SIPCallSpamTypeNone;             ///<The SIP call's spam type, such as \link SIPCallSpamType \endlink.
    SIPCallAttestLevel              peerAttestLevel = SIPCallAttestLevelUndefined;  ///<The SIP call's attest level, such as \link SIPCallAttestLevel \endlink.
    SIPCallRedirectInfo             redirectInfo;                                   ///<The SIP call's redirect info.
    bool                            isEmergencyCall = false;                        ///<TRUE indicates the call is emergency call. Otherwise not.
    EmergencyCallAddress            emergencyCallAddress;                           ///<The SIP call's emergency address.
};

/// \brief SIP call transfer info
/// 
struct SIPCallTransferInfo
{
    SIPCallTransferInfoType type = SIPCallTransferInfoTypeUnknown;  ///<SIP call transfer's type, such as \link SIPCallTransferInfoType \endlink.
    std::string             peerURI;                                ///<The peer URI which the current call is transferred to.
};

/// \brief SIP caller ID
/// 
struct SIPCallerID
{
    std::string name;           ///<SIP caller's name.
    std::string number;         ///<SIP caller's number.
    std::string extensionID;    ///<SIP caller's extension ID.
};

/// \brief Zoom Phone info
/// 
struct CloudPBXServiceInfo
{
    std::string              extension;                                 ///<Zoom Phone's extension.
    std::string              companyNumber;                             ///<Zoom Phone's companyNumber.
    std::vector<std::string> directNumbers;                             ///<Zoom Phone's directNumbers.
    std::string              countryCode;                               ///<Zoom Phone's countryCode.
    std::string              countryName;                               ///<Zoom Phone's countryName.
    std::string              areaCode;                                  ///<Zoom Phone's areaCode.
    std::vector<SIPCallerID> callerIDs;                                 ///<Zoom Phone's callerIDs.
    std::string              formattedCompanyNumber;                    ///<Zoom Phone's formattedCompanyNumber.
    std::vector<std::string> formattedDirectNumbers;                    ///<Zoom Phone's formattedDirectNumbers.
    bool                     isEnabledMakeOutBoundPSTNCall = false;     ///<TRUE indicates you can make PSTN calls. Otherwise not.
    bool                     isEnabledHaveADID = false;                 ///<TRUE indicates enabled have a DID. Otherwise not.
    bool                     isEnable911Call =false;                    ///<TRUE indicates you can make emergency calls. Otherwise not.
    EmergencyCall            emergencyCall;                             ///<Zoom Phone's emergency info.
};

/// \brief SIP service info
///
struct SIPService
{
    SIPServiceStatus    status = SIPServiceStatusIdle;  ///<SIP service's status, such as \link SIPServiceStatus \endlink.
    std::string         userName;                       ///<SIP service's user name.
    std::string         displayName;                    ///<SIP service's display name.
    int32_t             responseCode = 0;               ///<SIP service's response code, 200 means the sip service is available. 
    std::string         responseDescription;            ///<SIP service's response code description.
    bool                isZoomPhoneAvailable = false;   ///<TRUE indicates Zoom Phone is available. Otherwise not.
    CloudPBXServiceInfo cloudPBXServiceInfo;            ///<Zoom Phone's info.
};

/// \brief Phone call service event callback.
///
class IPhoneCallServiceSink
{
public:
    virtual ~IPhoneCallServiceSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Answer incoming SIP call response callback.
     *
     * @param succeeded TRUE indicates answer the incoming SIP call successfully. Otherwise not.
     * @param sipCallInfo The SIP call which is answered.
     * @param accepted TRUE indicates accept the incoming SIP call. Otherwise not.
     */
    virtual void OnAnswerSIPCallResult(bool succeeded, const SIPCallInfo& sipCallInfo, bool accepted) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Accept the upgrade SIP call to meeting result.
     *
     * @param succeeded TRUE indicates accept the upgrade SIP call to meeting successfully. Otherwise not.
     * @param sipCallInfo The SIP call which the peer wants to upgrade to meeting.
     */
    virtual void OnAcceptSIPCallToMeetingResult(bool succeeded, const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Make a call to peer result.
     *
     * @param succeeded TRUE indicates call peer successfully. Otherwise not.
     * @param sipCallInfo The call with peer.
     */
    virtual void OnUpdateSIPCallPeerResult(bool succeeded, const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update SIP call audio status result.
     *
     * @param succeeded TRUE indicates update SIP call audio successfully.Otherwise not.
     */
    virtual void OnUpdateSIPCallAudioResult(bool succeeded) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Transfer SIP call result.
     *
     * @param succeeded TRUE indicates transfer the SIP call successfully. Otherwise not.
     * @param sipCallInfo The SIP call which is transferred.
     * @param transferInfo The struct includes transfer's type and peer URI who the call is transferred to, details \link SIPCallTransferInfo \endlink struct.
     */
    virtual void OnTransferSIPCallResult(bool succeeded, const SIPCallInfo& sipCallInfo, const SIPCallTransferInfo& transferInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Upgrade SIP call to meeting result.
     *
     * @param succeeded TRUE indicates upgrade the SIP call to meeting successfully. Otherwise not.
     * @param sipCallInfo The SIP call which is upgraded to meeting.
     */
    virtual void OnUpgradeSIPCallToMeetingResult(bool succeeded, const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Receive incoming SIP call notification.
     *
     * @param sipCallInfo The SIP call which is received.
     */
    virtual void OnReceiveIncomingSIPCallNotification(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Treat SIP call notification.
     *
     * @param accepted TRUE indicates the SIP call is accepted. Otherwise not.
     * @param sipCallInfo The SIP call which is treated.
     */
    virtual void OnTreatSIPCallNotification(bool accepted, const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief The invitation to upgrade the SIP call to meeting.
     *
     * @param sipCallInfo The SIP call which the peer wants to upgrade to meeting.
     */
    virtual void OnInviteSIPCallToJoinMeetingNotification(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Terminate SIP call notification.
     *
     * @param reason The reason why the SIP call is terminated.
     * @param sipCallInfo The SIP call which is terminated.
     */
    virtual void OnTerminateSIPCallNotification(SIPCallTerminateReason reason, const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update SIP call audio status notification.
     *
     * @param muted The status of SIP call audio.
     */
    virtual void OnUpdateSIPCallAudioStatusNotification(bool muted) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update SIP call notification.
     *
     * @param sipCallInfo The new SIP call info.
     */
    virtual void OnUpdateSIPCallStatusNotification(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Transfer SIP call notification.
     *
     * @param succeeded TRUE indicates the SIP call is transferred successfully. Otherwise not.
     * @param sipCallInfo The SIP call which is transferred.
     */
    virtual void OnTransferSIPCallNotification(bool succeeded, const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Upgrade the SIP call to meeting notification.
     *
     * @param succeeded TRUE indicates upgrade the SIP call to meeting successfully. Otherwise not.
     * @param sipCallInfo The SIP call which is upgraded.
     */
    virtual void OnUpgradeSIPCallToMeetingNotification(bool succeeded, const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update SIP service info notification.
     *
     * @param sipService The new SIP service info.
     */
    virtual void OnUpdateSIPServiceStatusNotification(const SIPService& sipService) = 0;
};

/// \brief Phone call service interface.
///
class IPhoneCallService
{
public:
    virtual ~IPhoneCallService() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register phone call service callback sink.
     *
     * @param pSink A pointer to the IPhoneCallServiceSink that receives the phone call service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IPhoneCallServiceSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister phone call service callback sink.
     *
     * @param pSink A pointer to the IPhoneCallServiceSink that receives the phone call service callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IPhoneCallServiceSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Accept incoming SIP call.
     *
     * This interface can be used when there are no other ongoing calls. If there are incoming meeting calls or invitations to upgrade the SIP call to meeting, use the API will auto these.
     *
     * @param sipCallInfo The SIP call which is accepted.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AcceptIncomingSIPCall(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Accept incoming SIP call and hold the foreground call.
     *
     * This interface can be used when there is already an ongoing call. If there are incoming meeting calls or invitations to upgrade the SIP call to meeting, use the API will auto these.
     *
     * @param sipCallInfo The SIP call which is accepted.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError HoldAndAcceptIncomingSIPCall(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Accept incoming SIP call and end the foreground call.
     *
     * This interface can be used when there is already an ongoing call. If there are incoming meeting calls or invitations to upgrade the SIP call to meeting, use the API will auto these.
     *
     * @param sipCallInfo The SIP call which is accepted.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EndAndAcceptIncomingSIPCall(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Decline incoming SIP call.
     *
     * @param sipCallInfo The SIP call which is declined.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeclineIncomingSIPCall(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Accept the invitation to upgrade the SIP call to meeting.
     *
     * If there are incoming meeting calls, incoming SIP calls or other invitations to upgrade the SIP call to meeting, use the API will auto these.
     *
     * @param sipCallInfo The SIP call which is accepted.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AcceptSIPCallToMeeting(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Decline the invitation to upgrade the SIP call to meeting.
     *
     * @param sipCallInfo The SIP call which is declined.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeclineSIPCallToMeeting(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Make a call.
     *
     * @param uri The peer uri, it can be the extension number or direct number.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CallSIP(const std::string& uri) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Hang up an active SIP call.
     *
     * @param sipCallInfo The SIP call which is hung up, or which the SIP call which is upgraded to meeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError HangupSIPCall(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute the SIP call audio.
     *
     * @param mute TRUE indicates mute SIP call audio. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteSIPCallAudio(bool mute) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Send DTMF to the SIP call.
     *
     * @param dtmf The character which is sent to the SIP call. The characters include 0~9, *, +, #.
     * @param sipCallInfo The SIP call which the dtmf is sent to.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SendDTMFToSIPCall(const std::string& dtmf, const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Hold the SIP call.
     *
     * @param sipCallInfo The SIP call which is held.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError HoldSIPCall(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Unhold the SIP call.
     *
     * @param sipCallInfo The SIP call which is unheld.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnholdSIPCall(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Merge the participant SIP Call to host SIP call.
     *
     * @param hostSIPCallInfo The SIP call which the participant SIP call info is merged to.
     * @param participantSIPCallInfo The SIP call which is merged.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MergeSIPCall(const SIPCallInfo& hostSIPCallInfo, const SIPCallInfo& participantSIPCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Transfer the SIP call to other people.
     *
     * @param sipCallInfo The SIP call which is transferred.
     * @param transferInfo The info includes transfer's type and peer URI who the call is transferred to, details \link SIPCallTransferInfo \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError TransferSIPCall(const SIPCallInfo& sipCallInfo, const SIPCallTransferInfo& transferInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Complete the warm transfer.
     *
     * @param sipCallInfo The SIP call which is transferred.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CompleteWarmTransfer(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Cancel the warm transfer.
     *
     * @param sipCallInfo The SIP call which is transferred.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CancelWarmTransfer(const SIPCallInfo& sipCallInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Upgrade the SIP call to meeting.
     *
     * @param sipCallInfo The SIP call which is upgraded.
     * @param isEndCurrentMeeting TRUE indicates end current meeting. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UpgradeSIPCallToMeeting(const SIPCallInfo& sipCallInfo, bool isEndCurrentMeeting) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set location permission enable.
     *
     * @param enable TRUE indicates the enable location permission. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetLocationPermissionEnable(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get location permission enable.
     *
     * @param[out] enable TRUE indicates enable location permission.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetLocationPermissionEnable(bool& enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the SIP calls.
     *
     * @param[out] sipCalls The SIP calls.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSIPCallList(std::vector<SIPCallInfo>& sipCalls) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the unhold call which is ongoing call.
     *
     * @param[out] unholdCall The unhold call.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetUnholdSIPCall(SIPCallInfo& unholdCall) = 0;
};

NS_ZRCSDK_END
#endif    //ZRC_IPHONECALLSERVICE_H

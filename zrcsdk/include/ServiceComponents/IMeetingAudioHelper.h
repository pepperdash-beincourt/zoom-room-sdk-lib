/*!
 * \file IMeetingAudioHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Meeting Audio interfaces.
 *
 * Far End Audio Control(FEAC)
 */
#ifndef ZRC_IMEETINGAUDIOHELPER_H
#define ZRC_IMEETINGAUDIOHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum AskUnmuteAudioByHostType
    \brief Host or co-host ask to unmute audio type
    Here are more detailed structural descriptions.
 */
enum AskUnmuteAudioByHostType
{
    AskUnmuteAudioByHostTypeUnmuteAudio = 0,    ///<Host or co-host request Zoom Room to unmute audio.
    AskUnmuteAudioByHostTypeSpotlight,          ///<Zoom Room is being spotlighted, being spotlighted will be asked to unmute audio.
    AskUnmuteAudioByHostTypeViewOnlyTalk,       ///<In webinar, host allow attendee to talk.
};

/// \brief Audio troubleshooting status
///
struct AudioTroubleShootingStatus
{
    bool    isRecording = false;
    int32_t totalDuration = 0;
    int32_t timeLeft = 0;
};

/// \brief Far end audio device info.
///
struct FarEndAudioDeviceInfo
{
    std::string deviceID;                   ///<Device ID.
    std::string deviceName;                 ///<Device name.
    int32_t     volume = 0;                 ///<The volume of the device. When isSelected == false, this field is invalid.
    bool        volumeCanAdjust = false;    ///<True indicate the volume can be adjusted, otherwise can't. When isSelected == false, this field is invalid.
    bool        isSelected = false;         ///<True indicate the device is currently in use.
};

/// \brief Far end audio controlling user device info. This device info includes all control info of the controlled user.
struct FarEndAudioControllingUserDeviceInfo
{
    int32_t                            userID = 0;            ///<User ID of the controlled user.
    bool                               muteState = false;     ///<TRUE indicate microphone is muted. Otherwise not.
    FarEndAudioDeviceInfo              selectedMicrophone;    ///<The microphone that is currently in use.
    FarEndAudioDeviceInfo              selectedSpeaker;       ///<The speaker that is currently in use.
    std::vector<FarEndAudioDeviceInfo> microphones;           ///<The all microphones of the controlled user, include the microphone that is currently in use.
    std::vector<FarEndAudioDeviceInfo> speakers;              ///<The all speakers of the controlled user, include the speaker that is currently in use.
};

/// \brief Meeting Audio helper event callback
///
class IMeetingAudioHelperSink
{
public:
    virtual ~IMeetingAudioHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Zoom Room's audio status changed callback.
     *
     * @param audioStatus Updated audio status.
     */
    virtual void OnUpdateMyAudioStatus(const AudioStatus& audioStatus) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief After mute user audio, user audio status changed callback
     *
     * @param userID The assigned user ID
     * @param audioStatus The user's audio status
     */
    virtual void OnMuteUserAudioNotification(int32_t userID, const AudioStatus& audioStatus) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute on entry status notification
     *
     * @param isMuteOnEntry TRUE indicates to mute the user after joining the meeting.
     */
    virtual void OnMuteOnEntryNotification(bool isMuteOnEntry) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Notification from host ask to unmute audio
     *
     * @param show TRUE indicates to show the request. Otherwise not.
     * @param type Ask to unmute audio by host type, see \link AskUnmuteAudioByHostType \endlink enum.
     */
    virtual void OnAskUnmuteAudioByHostNotification(bool show, AskUnmuteAudioByHostType type) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow attendees unmute themselves notification.
     *
     * @param canAttendeesUnmuteThemselves TRUE indicates the user can unmute himself. Otherwise not.
     */
    virtual void OnAllowAttendeesUnmuteThemselvesNotification(bool canAttendeesUnmuteThemselves) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable play join or leave chime status notification
     *
     * @param enable TRUE indicates to play chime when the user joins or leaves the meeting.
     */
    virtual void OnEnablePlayJoinOrLeaveChimeNotification(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Audio troubleshooting status update notification
     *
     * @param status The audio troubleshooting status. If never receiving this notification, it indicates the feature not available currently.
     */
    virtual void OnUpdateAudioTroubleShootingStatus(const AudioTroubleShootingStatus& status) = 0;

    // ==========================================================================
    // FEAC Requester Sinks
    // ==========================================================================
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC approve your request notification.
     *
     * @param farEndUserID The controlled user ID.
     * @param farEndUserName The controlled user name.
     */
    virtual void OnFEACApproveNotification(int32_t farEndUserID, const std::string& farEndUserName) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC decline your request notification.
     *
     * @param farEndUserID The controlled user ID.
     * @param farEndUserName The controlled user name.
     */
    virtual void OnFEACDeclineNotification(int32_t farEndUserID, const std::string& farEndUserName) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC take over notification.
     *
     * @param farEndUserID The user ID who is taken over.
     * @param farEndUserName The user name who is taken over.
     * @param controllingUserID The user ID who took over.
     * @param controllingUserName The user name who took over.
     */
    virtual void OnFEACTakeOverNotification(int32_t farEndUserID, const std::string& farEndUserName, int32_t controllingUserID, const std::string& controllingUserName) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC microphone list changed notification.
     *
     * @param farEndUserID The controlled user ID.
     * @param micList All controlled user's microphones.
     */
    virtual void OnFEACMicListChangedNotification(int32_t farEndUserID, const std::vector<FarEndAudioDeviceInfo>& micList) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC speaker list changed notification.
     *
     * @param farEndUserID The controlled user ID.
     * @param speakerList All controlled user's speakers.
     */
    virtual void OnFEACSpeakerListChangedNotification(int32_t farEndUserID, const std::vector<FarEndAudioDeviceInfo>& speakerList) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC mute state changed notification.
     *
     * @param farEndUserID The controlled user ID.
     * @param muteState The controlled user's mute state.
     */
    virtual void OnFEACMuteStateChangedNotification(int32_t farEndUserID, bool muteState) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC unmute disabled by host notification. You will receive this notification when you unmute the controlled user and the unmute is disabled by host.
     *
     * @param farEndUserID The controlled user ID.
     */
    virtual void OnFEACUnmuteDisabledByHostNotification(int32_t farEndUserID) = 0;

    // ==========================================================================
    // FEAC Be Requested Sinks
    // ==========================================================================
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC request your audio control notification.
     *
     * @param requesterUserID The requester user ID.
     * @param requesterUserName The requester user name.
     */
    virtual void OnFEACRequestNotification(int32_t requesterUserID, const std::string& requesterUserName) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC give up your audio control notification.
     *
     * @param requesterUserID The requester user ID.
     * @param requesterUserName The requester user name.
     */
    virtual void OnFEACGiveUpNotification(int32_t requesterUserID, const std::string& requesterUserName) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC approve audio control request notification. You will receive this notification when other ZRC approve the requester audio control.
     *
     * @param requesterUserID The requester user ID.
     */
    virtual void OnFEACApproveControlRequestNotification(int32_t requesterUserID) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief FEAC decline audio control request notification. You will receive this notification when other ZRC decline the requester audio control.
     *
     * @param requesterUserID The requester user ID.
     */
    virtual void OnFEACDeclineControlRequestNotification(int32_t requesterUserID) = 0;
};

/// \brief Meeting Audio helper interface
///
class IMeetingAudioHelper
{
public:
    virtual ~IMeetingAudioHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting audio helper callback sink.
     *
     * @param pSink A pointer to the IMeetingAudioHelperSink that receives the meeting audio helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingAudioHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting audio helper callback sink.
     *
     * @param pSink A pointer to the IMeetingAudioHelperSink that receives the meeting audio helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingAudioHelperSink* pSink) = 0;

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
    virtual ZRCSDKError UpdateMyAudioStatus(bool mute) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute or unmute the assigned user audio.
     *
     * @param userID Specify the user ID to unmute.
     * @param mute TRUE indicates to mute user audio. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteUserAudio(int32_t userID, bool mute) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute or unmute all meeting users' audio.
     *
     * @param mute TRUE indicates mute all users' audio. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteAllAudios(bool mute) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute or unmute the user after joining the meeting.
     *
     * @param mute TRUE indicates user will be muted on entry.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteOnEntry(bool mute) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Answer incoming unmute audio request
     *
     * @param accepted TRUE indicates accept unmute request. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AnswerUnmuteAudioByHostRequest(bool accepted) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Allow attendee unmute themselves.
     *
     * @param allow TRUE indicates allow user to unmute themselves. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowAttendeesUnmuteThemselves(bool allow) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable play chime when join or leave meeting.
     *
     * @param enable TRUE indicates to play chime when the user joins or leaves the meeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnablePlayJoinOrLeaveChime(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if meeting audio troubleshooting available
     *
     * @param[out] isAvailable TRUE indicates  meeting audio troubleshooting is available. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsMeetingAudioTroubleShootingAvailable(bool& isAvailable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable meeting audio troubleshooting.
     *
     * @param enable TRUE indicates to enable meeting audio troubleshooting. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableMeetingAudioTroubleShooting(bool enable) = 0;

    // ==========================================================================
    // FEAC Requester Commands
    // ==========================================================================
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Request far end audio control.
     *
     * @param farEndUserID The user ID whose audio control you want to control.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RequestFarEndAudioControl(int32_t farEndUserID) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Give up far end audio control.
     *
     * @param farEndUserID The user ID whose audio control you want to give up.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GiveUpFarEndAudioControl(int32_t farEndUserID) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Begin control far end audio. Only when you begin control the controlled user's audio can you receive relevant control information. Otherwise not.
     *
     * @param farEndUserID The user ID whose audio control you want to begin control.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BeginControlFarEndAudio(int32_t farEndUserID) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief End control far end audio. You will not receive relevant control information after end control the controlled user's audio.
     *
     * @param farEndUserID The user ID whose audio control you want to end control.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EndControlFarEndAudio(int32_t farEndUserID) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Select the controlled user's microphone.
     *
     * @param farEndUserID The user ID whose audio you controlled.
     * @param deviceID The microphone ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectFarEndAudioMicrophone(int32_t farEndUserID, const std::string& deviceID) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Select the controlled user's speaker.
     *
     * @param farEndUserID The user ID whose audio you controlled.
     * @param deviceID The speaker ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectFarEndAudioSpeaker(int32_t farEndUserID, const std::string& deviceID) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Adjust the controlled user's microphone volume.
     *
     * @param farEndUserID The user ID whose audio you controlled.
     * @param volume The microphone volume. The range is [0, 255].
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AdjustFarEndAudioSelectedMicrophoneVolume(int32_t farEndUserID, int32_t volume) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Adjust the controlled user's speaker volume.
     *
     * @param farEndUserID The user ID whose audio you controlled.
     * @param volume The speaker volume. The range is [0, 255].
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AdjustFarEndAudioSelectedSpeakerVolume(int32_t farEndUserID, int32_t volume) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Mute the controlled user's microphone.
     *
     * @param farEndUserID The user ID whose audio you controlled.
     * @param mute True indicates mute the controlled user's audio. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteFarEndAudioMicrophone(int32_t farEndUserID, bool mute) = 0;

    // ==========================================================================
    // FEAC Be Requested Sinks
    // ==========================================================================
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Approve far end audio control request.
     *
     * @param requesterUserID The requester user ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ApproveFarEndAudioControlRequest(int32_t requesterUserID) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Decline far end audio control request.
     *
     * @param requesterUserID The requester user ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeclineFarEndAudioControlRequest(int32_t requesterUserID) = 0;

    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Get the controlling user's device info by user ID.
     *
     * @param[out] deviceInfo The controlling device info.
     * @param userID The controlled user ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetFarEndAudioControllingUserDeviceInfo(FarEndAudioControllingUserDeviceInfo& deviceInfo, int32_t userID) = 0;
    /**
     * @since 6.5.0
     * @zrminver 6.5.0
     *
     * @brief Get all controlling user's device infos.
     *
     * @param[out] deviceInfoList The all controlling device infos.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetAllFarEndAudioControllingUserDeviceInfo(std::vector<FarEndAudioControllingUserDeviceInfo>& deviceInfoList) = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_IMEETINGAUDIOHELPER_H

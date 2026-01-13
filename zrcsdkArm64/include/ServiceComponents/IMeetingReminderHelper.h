/*!
 * \file IMeetingReminderHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Meeting Reminder interfaces.
 */
#ifndef ZRC_IMEETINGREMINDERHELPER_H
#define ZRC_IMEETINGREMINDERHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum MeetingReminderType
    \brief Meeting reminder type.
    Here are more detailed structural descriptions.
 */
enum MeetingReminderType
{
    REMINDER_TYPE_NONE = -1,                   ///<Invalid
    REMINDER_TYPE_START_OR_JOIN_MEETING,       ///<Reminder of start of join meeting
    REMINDER_TYPE_JOIN_EXTERNAL_MEETING,       ///<Reminder of join external meeting
    REMINDER_TYPE_RECORDING_REMINDER,          ///<Reminder of recording reminder
    REMINDER_TYPE_RECORDING_DISCLAIMER,        ///<Reminder of recording disclaimer
    REMINDER_TYPE_ARCHIVING_FAIL,              ///<Reminder of archiving fail
    REMINDER_TYPE_JOIN_WEBINAR_AS_PANELIST,    ///<Reminder of join webinar as panelist
};

/*! \enum CustomizedMeetingReminderType
    \brief Customized meeting reminder type, reminder content is set by ZR.
    Here are more detailed structural descriptions.
 */
enum CustomizedMeetingReminderType
{
    CUSTOMIZED_REMINDER_TYPE_NONE = 0,                      ///<Invalid
    CUSTOMIZED_REMINDER_TYPE_ONZOOM_JOIN_AS_PANELIST = 1    ///<Customized reminder type of join ONZOOM as panelist.
};

/*! \enum ConsentType
    \brief Privacy consent type.
    Here are more detailed structural descriptions.
 */
enum ConsentType
{
    CONSENT_TYPE_NONE = -1,               ///<Consent type none
    CONSENT_TYPE_LIVE_STREAMING = 0,      ///<Consent type of live streaming
    CONSENT_TYPE_PROMOTED_TO_PANELIST,    ///<Consent type of being promoted to panelist
    CONSENT_TYPE_ARCHIVING,               ///<Consent type of archiving
    CONSENT_TYPE_NDI,                     ///<Consent type of ndi
    CONSENT_TYPE_FOCUS_MODE_START,        ///<Consent type of focus mode start
    CONSENT_TYPE_FOCUS_MODE_ENDING,       ///<Consent type of focus mode ending
    CONSENT_TYPE_ADMIN_PAY_REMIND,        ///<Consent type of admin pay remind
    CONSENT_TYPE_PAC,                     ///<Consent type of personal audio conference
    CONSENT_TYPE_ZOOM_PHONE_ACR,          ///<Consent type of Zoom phone ACR
    CONSENT_TYPE_HDMI_CONNECTED,          ///<Consent type of hdmi connected
    CONSENT_TYPE_MEETING_SUMMARY,         ///<Consent type of meeting summary
    CONSENT_TYPE_MEETING_QUERY,           ///<Consent type of meeting query
    CONSENT_TYPE_CUSTOM_AI_COMPANION,     ///<Consent type of AICompanion
    CONSENT_TYPE_COMMON,                  ///<Consent type of Common
    CONSENT_TYPE_SIMULIVE_WEBINAR,        ///<Consent type of simulive webinar
    CONSENT_TYPE_CUSTOM_RECORDING,        ///<Consent type of  custom recoding
};

/*! \enum PrivacyAlertType
    \brief Privacy alert type.
    Here are more detailed structural descriptions.
 */
enum PrivacyAlertType
{
    PRIVACY_ALERT_TYPE_LIVE_TRANSCRIPTION = 0,    ///<Privacy alert type live transcription.
    PRIVACY_ALERT_TYPE_NEW_LTT_CAPTION = 1,       ///<Privacy alert type new LTT caption.
};

/*! \enum PrivacyAlertAction
    \brief Privacy alert action.
    Here are more detailed structural descriptions.
 */
enum PrivacyAlertAction
{
    PRIVACY_ALERT_ACTION_NONE = 0,               ///<None.
    PRIVACY_ALERT_ACTION_SHOW = 1,               ///<Show privacy alert.
    PRIVACY_ALERT_ACTION_CLOSE = 2,              ///<Close privacy alert.
    PRIVACY_ALERT_ACTION_SHOW_DISCLAIMER = 3,    ///<Show privacy alert disclaimer.
    PRIVACY_ALERT_ACTION_CLOSE_DISCLAIMER = 4    ///<Close privacy alert disclaimer.
};

/*! \enum MessageEvent
    \brief Meeting message event
    Here are more detailed structural descriptions.
 */
enum MessageEvent
{
    MESSAGE_EVENT_UNKNOWN = 0,                                              ///<Unknown
    MESSAGE_EVENT_OpenVideoFailForHostStop = 1,                             ///<Open video failed because host stopped your video.
    MESSAGE_EVENT_OpenVideoFailForForceVBEnabledButUserOptionDisabled = 2,  ///<Open video failed because virtual background is force enabled but user option is disabled.
    MESSAGE_EVENT_OpenVideoFailForForceVBEnabledButUserNoGreenScreen = 3,   ///<Open video failed because virtual background is force enabled but user has no green screen.
    MESSAGE_EVENT_OpenVideoFailForForceVBEnabledButDeviceNotSupport = 4     ///<Open video failed because virtual background is force enabled but device not support.
};

/// \brief Privacy message
///Privacy message contains hyperlink to privacy policy.
struct PrivacyMessage
{
    std::string privacyMessage;    ///<Privacy message body
    std::string hyperlinkKey;      ///<Hyperlink text key of the privacy message
    std::string hyperlinkURL;      ///<Hyperlink text url of the privacy message
};

/// \brief Disclaimer privacy.
///Disclaimer privacy information, UI structure follows title -> privacyMessage(or privacySection from ZR) -> message -> link -> action
struct DisclaimerPrivacy
{
    std::string title;                ///<Disclaimer title
    PrivacyMessage privacyMessage;    ///<Privacy message, supported type: REMINDER_TYPE_RECORDING_REMINDER, CONSENT_TYPE_ARCHIVING, CONSENT_TYPE_NDI, CONSENT_TYPE_ZOOM_PHONE_ACR, CONSENT_TYPE_MEETING_SUMMARY, CONSENT_TYPE_CUSTOM_AI_COMPANION.
    std::string message;              ///<Disclaimer message
    std::string linkUrl;              ///<Disclaimer privacy link url. If linkUrl is not empty, link is valid.
    std::string linkText;             ///<Disclaimer privacy link text. If linkText is not empty, indicates this is a suggested clickable text to navigate to a linkUrl or do PRIVACY_ALERT_ACTION_SHOW_DISCLAIMER.
    std::string positiveActionText;   ///<Positive button text. If positiveActionText is not empty, action is valid.
    std::string negativeActionText;   ///<Negative button text. If negativeActionText is not empty, action is valid.
    std::string privacySection;       ///<Privacy section from ZR, this section contains URLSpan. REMINDER_TYPE_RECORDING_REMINDER , CONSENT_TYPE_MEETING_QUERY, see Zoom -> Account Setting ->Recording -> Customize Disclaimer
};

/// \brief Meeting reminder content.
///
struct MeetingReminderContent
{
    MeetingReminderType reminderType = REMINDER_TYPE_NONE;    ///<Reminder type, see \link MeetingReminderType \endlink enum.
    DisclaimerPrivacy   disclaimerPrivacy;                    ///<Disclaimer privacy.
    bool                isShowing = false;                    ///<TRUE indicates Zoom Room is showing the reminder.
};

/// \brief Customized meeting reminder content.
///
struct CustomizedMeetingReminderContent
{
    int32_t customizedDisclaimerType = 0;   ///<Customized disclaimer type, for compatibility, use plain int here. See \link CustomizedMeetingReminderType \endlink enum for more information.
    DisclaimerPrivacy disclaimerPrivacy;    ///<Disclaimer privacy.
    bool              isShowing = false;    ///<TRUE indicates Zoom Room is showing the reminder.
};

/// \brief Combined consent
///
struct CombinedConsent
{
    bool isShowing = false;                 ///<TRUE indicates Zoom Room is showing the combined consent.
    int64_t type = 0;                       ///<Combined consent type.
    DisclaimerPrivacy disclaimerPrivacy;    ///<Disclaimer privacy.
};

/// \brief Consent notification info.
///
struct ConsentInfo
{
    ConsentType       type = CONSENT_TYPE_NONE;    ///<Consent type
    DisclaimerPrivacy disclaimer;                  ///<Disclaimer privacy.
    bool              isShowing = false;           ///<TRUE indicates Zoom Room is showing the consent.
    std::string       consentID;                   ///<Unique id for CONSENT_TYPE_COMMON,
};

/// \brief Meeting Reminder helper event callback.
///
class IMeetingReminderHelperSink
{
public:
    ~IMeetingReminderHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Meeting reminder notification
     *
     * @param reminderContent Reminder content.
     */
    virtual void OnMeetingReminderNotification(const MeetingReminderContent& reminderContent) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Customized meeting reminder notification
     *
     * @param customizedContent Customized reminder content.
     */
    virtual void OnCustomizedReminderNotification(const CustomizedMeetingReminderContent& customizedContent) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Combined consent notification.
     *
     * @param combinedConsent Combined consent.
     */
    virtual void OnCombinedConsentNotification(const CombinedConsent& combinedConsent) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Consent notification
     *
     * @param info The consent info.
     * @changelog
     * - 6.6.0: For common consent (type == CONSENT_TYPE_COMMON), use consentID to identify different consent
     */
    virtual void OnConsentNotification(const ConsentInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Privacy alert notification.
     *
     * @param action The privacy alert action.
     * @param type The privacy alert type.
     * @param message The privacy alert message. If message contains linkText, you can do @ref PRIVACY_ALERT_ACTION_SHOW_DISCLAIMER to see detail disclaimer.
     */
    virtual void OnPrivacyAlertNotification(PrivacyAlertAction action, PrivacyAlertType type, const DisclaimerPrivacy& message) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Message event notification.
     *
     * @param messageEvent The event, see \link MessageEvent \endlink enum.
     */
    virtual void OnMessageEventNotification(MessageEvent messageEvent) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Inactive detection notification.
     *
     * This notification will be triggered when no participants are detected in the room for 30 minutes.
     *
     * @param isShowPrompt  TRUE show inactive detection prompt.
     * @param autoEndTime  The UTC timestamp (in seconds) at which the meeting will automatically end due to inactivity.
     */
    virtual void OnInactiveDetectionNotification(bool isShowPrompt, time_t autoEndTime) = 0;
};

/// \brief Meeting Reminder helper interface.
///
class IMeetingReminderHelper
{
public:
    virtual ~IMeetingReminderHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting reminder helper callback sink.
     *
     * @param pSink A pointer to the IMeetingReminderHelperSink that receives the meeting reminder helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingReminderHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting reminder helper callback sink.
     *
     * @param pSink A pointer to the IMeetingReminderHelperSink that receives the meeting reminder helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingReminderHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Confirm meeting reminder
     *
     * @param isAgree TRUE indicates to agree the meeting reminder. Otherwise not.
     * @param reminderType Meeting reminder type, see \link MeetingReminderType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ConfirmMeetingReminder(bool isAgree, MeetingReminderType reminderType) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Confirm customized meeting reminder
     *
     * @param isAgree TRUE indicates to agree the customized meeting reminder. Otherwise not.
     * @param customizedDisclaimerType Customized disclaimer type.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ConfirmCustomizedMeetingReminder(bool isAgree, int32_t customizedDisclaimerType) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Confirm consent.
     *
     * @param isAgree TRUE indicates to agree the consent. Otherwise not.
     * @param type Consent type, see \link ConsentType \endlink enum.
     *
     * @changelog
     * - 6.6.0: For common consent (type == CONSENT_TYPE_COMMON),  consentID is required
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ConfirmConsent(bool isAgree, ConsentType type, const std::string& consentID = {}) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Confirm combined consent.
     *
     * @param agree TRUE indicates to agree the consent. Otherwise not.
     * @param consentType Combined consent type.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ConfirmCombinedConsent(bool agree, int64_t consentType) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Handle privacy alert
     *
     * @param action Privacy alert action, see \link PrivacyAlertAction \endlink enum.
     * @param type Privacy alert type, see \link PrivacyAlertType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError HandlePrivacyAlert(PrivacyAlertAction action, PrivacyAlertType type) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Continue meeting after inactivity detection notification.
     *
     * This method should be called when the meeting has been marked as inactive (via OnInactiveDetectionNotification).
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ContinueMeetingOnInactivity() = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_IMEETINGREMINDERHELPER_H

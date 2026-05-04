/*!
 * \file IClosedCaptionHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Closed Caption interfaces.
 * 
 * Closed Caption function has 2 phases:
 * 1. Closed Caption
 * 2. New LTT Caption（Next Generation Live Transcription Translation）
 *
 * NOTE: If NewLTTCaptionInfo.isNewLttCaptionFeatureOn = true, use New LTT APIs, else use Closed Caption.
 */
#ifndef ZRC_ICLOSEDCAPTIONHELPER_H
#define ZRC_ICLOSEDCAPTIONHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum ClosedCaptionFontSize
    \brief Closed caption font size
    Here are more detailed structural descriptions.
 */
enum ClosedCaptionFontSize
{
    ClosedCaptionFontSizeSmall,     ///<Closed caption font size small.
    ClosedCaptionFontSizeMedium,    ///<Closed caption font size medium.
    ClosedCaptionFontSizeLarge      ///<Closed caption font size large.
};

/*! \enum NewLTTCaptionNotificationType
    \brief NewLTT caption notification type
    Here are more detailed structural descriptions.
 */
enum NewLTTCaptionNotificationType
{
    NewLTTCaptionNotificationTypeCaptionStart,
    NewLTTCaptionNotificationTypeEnableCaptionRequestReceived,
    NewLTTCaptionNotificationTypeEnableCaptionRequestDeclined,
    NewLTTCaptionNotificationTypeSpeakerLanguageMismatch
};

/*! \enum LTTCaptionWritingDirection
    \brief LTT caption writing direction.
    Here are more detailed structural descriptions.
 */
enum LTTCaptionWritingDirection
{
    LTTCaptionWritingDirectionLeftToRight = 0,    ///<Direction Left to Right.
    LTTCaptionWritingDirectionRightToLeft = 1     ///<Direction Right to Left.
};

/*! \enum LTTCaptionMessageResultType
    \brief LTT caption message result type.
    Here are more detailed structural descriptions.
 */
enum LTTCaptionMessageResultType
{
    LTTCaptionMessageResultTypeSuccess = 0,                 ///<Success
    LTTCaptionMessageResultTypeTranslationNotSupport = 1    ///<Language translation not support.
};

/// \brief Closed Caption info
///
struct ClosedCaptionInfo
{
    bool                  available = false;                        ///<TRUE indicates closed caption is available. Otherwise not.
    bool                  visible = false;                          ///<TRUE indicates closed caption is visible. Otherwise not.
    ClosedCaptionFontSize fontSize = ClosedCaptionFontSizeSmall;    ///<Closed caption font size.
};

/// \brief New LTT caption language
///If languageID is -1, indicates that current language is invalid.
struct NewLTTCaptionLanguage
{
    int32_t     languageID = -1;    ///<Language ID defined and used between Controller and Zoom Room.
    std::string displayName;        ///<Language display name.
    std::string abbreviatedName;    ///<Language abbreviated name.
};

/// \brief New LTT caption speaking language info
///
struct NewLTTCaptionSpeakerLanguageInfo
{
    NewLTTCaptionLanguage              currentLanguage;       ///<Current selected language.
    std::vector<NewLTTCaptionLanguage> availableLanguages;    ///<Available languages.
};

/// \brief New LTT caption translation info
///
struct NewLTTCaptionTranslationInfo
{
    NewLTTCaptionLanguage              currentLanguage;          ///<Current selected language.
    std::vector<NewLTTCaptionLanguage> availableLanguages;       ///<Available languages.
    std::vector<NewLTTCaptionLanguage> recentlyUsedLanguages;    ///<Recently used languages.
};

/// \brief New LTT caption info
///
struct NewLTTCaptionInfo
{
    bool                  isNewLttCaptionFeatureOn = false;         ///<TRUE indicates New LTT is enabled. Otherwise only Closed Caption is enabled.
    bool                  isAutomatedCaptionFeatureOn = false;      ///<TRUE indicates automated caption is enabled. Otherwise not.
    bool                  isTranslatedCaptionFeatureOn = false;     ///<TRUE indicates translated caption is enabled. Otherwise not.
    bool                  isShowCaptionOn = false;                  ///<TRUE indicates show caption is on. Otherwise not.
    ClosedCaptionFontSize fontSize = ClosedCaptionFontSizeSmall;    ///<Caption font size, see \link ClosedCaptionFontSize \endlink enum.
    bool                  isManualCaptionerEnabled = false;         ///<TRUE indicates manual captioner is enabled. Otherwise not.
    bool                  isShowOriginalAndTranslated = false;      ///<TRUE indicates showing original and translated. Otherwise not.
    NewLTTCaptionSpeakerLanguageInfo speakingLanguageInfo;          ///<New LTT caption speaker language info.
    NewLTTCaptionTranslationInfo     translationInfo;               ///<New LTT caption translation info.
    bool                  isMmrSupportDisableLttCaption = false;    ///<TRUE indicates current meeting server supports disable LTT caption.
    bool                  isCaptionDisabled = false;                ///<TRUE indicates caption is disabled.
    bool                  isAllowShowCaption = false;               ///<TRUE indicates allow show caption.
    bool                  isAllowRequestCaption = false;            ///<TRUE indicates allow request caption
    bool                  isShowTranscriptPanelOnZR = false;        ///<TRUE indicates is showing transcript panel on ZR
    bool                  isAllowViewFullTranscript = false;        ///<TRUE indicates allow view full transcript.
};

/// \brief Interpret language info list.
///
struct InterpretLanguageInfoList
{
    bool                               isAvailable = false;             ///<TRUE indicates interpret language is available.
    bool                               isOriginalAudioMuted = false;    ///<TRUE indicates interpret language original audio is muted.
    std::vector<InterpretLanguageInfo> languages;                       ///<Supported interpret language list.
    InterpretLanguageInfo              activeLanguage;                  ///<Active interpret language.
};

/// \brief LTT caption message
///
struct LTTCaptionMessage
{
    LTTCaptionMessageResultType result = LTTCaptionMessageResultTypeSuccess;          ///<Message result type.
    std::string                 messageID;                                            ///<Message ID
    int32_t                     userNodeID = 0;                                       ///<User node ID, compare it with MeetingParticipant userID.
    std::string                 userName;                                             ///<User name
    int64_t                     messageTime = 0;                                      ///<Message time.
    std::string                 messageContent;                                       ///<Message content
    LTTCaptionWritingDirection  direction = LTTCaptionWritingDirectionLeftToRight;    ///<Message writing direction.
    int32_t                     speakerTagID = 0;                                     ///<Smart speaker tag speaker ID.
    std::string                 speakerTagName;                                       ///<Smart speaker tag speaker name.
    SmartTagUser                instanceOnlySpeakerTag;                               ///<Instance only smart speaker tag only for individual message.
    std::string                 attendeeJid;                                          ///<An attendee's attendeeJid remains unchanged when switching between main session and sub-session.
};

/// \brief Closed Caption helper event callback
///
class IClosedCaptionHelperSink
{
public:
    virtual ~IClosedCaptionHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Closed caption info changed callback.
     *
     * @param closedCaptionInfo Updated closed caption info.
     */
    virtual void OnUpdateClosedCaptionNotification(const ClosedCaptionInfo& closedCaptionInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Closed caption control response callback.
     *
     * @param result [0] success, otherwise error.
     * @param fontSize Specify the font size which requested to control.
     */
    virtual void OnClosedCaptionResponse(int32_t result, ClosedCaptionFontSize fontSize) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief New LTT caption info changed callback.
     *
     * @param newLttCaptionInfo Updated New LTT caption info.
     */
    virtual void OnNewLTTLanguageNotification(const NewLTTCaptionInfo& newLttCaptionInfo) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief New LTT caption notification
     *
     * @param type The notification type, see \link NewLTTCaptionNotificationType \endlink enum.
     */
     virtual void OnNewLTTCaptionNotification(NewLTTCaptionNotificationType type) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Interpret language info update notification.
     *
     * @param infoList Interpret language info
     */
    virtual void OnUpdateInterpretLanguageNotification(const InterpretLanguageInfoList& infoList) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief LTT caption message add notification.
     *
     * @param message LTT caption message
     */
    virtual void OnMessageAdd(const LTTCaptionMessage& message) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief LTT caption message update notification.
     *
     * @param message LTT caption message
     */
    virtual void OnMessageUpdate(const LTTCaptionMessage& message) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief LTT caption message not supported notification.
     *
     * @param message LTT caption message
     */
    virtual void OnMessageNotSupported(const LTTCaptionMessage& message) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief LTT caption messages loaded notification.
     *
     * @param messages The loaded LTT caption messages.
     * @param hasMoreHistory TRUE indicates has more caption message history.
     */
    virtual void OnMessageLoad(const std::vector<LTTCaptionMessage>& messages, bool hasMoreHistory) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief LTT caption message instance only smart speaker tag update notification.
     *
     * @param message LTT caption message.
     */
    virtual void OnMessageInstanceOnlySpeakerTagUpdate(const LTTCaptionMessage& message) = 0;
};

/// \brief Closed Caption helper interface
///
class IClosedCaptionHelper
{
public:
    virtual ~IClosedCaptionHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register closed caption helper callback sink.
     *
     * @param pSink A pointer to the IClosedCaptionHelperSink that receives the closed caption helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IClosedCaptionHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister closed caption helper callback sink.
     *
     * @param pSink A pointer to the IClosedCaptionHelperSink that receives the closed caption helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IClosedCaptionHelperSink* pSink) = 0;

    // CLOSED CAPTION

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control closed caption visibility and font size.
     *
     * @param show TRUE indicates show the closed caption. Otherwise not.
     * @param fontSize Choose the closed caption font size, see \link ClosedCaptionFontSize \endlink enum
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ControlClosedCaptionVisible(bool show, ClosedCaptionFontSize fontSize) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Assign user to type closed caption.
     *
     * @param userID Specify the userID.
     * @param assign TRUE indicates assign user to type CC. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AssignClosedCaption(int32_t userID, bool assign) = 0;

    // Next Generation Language Transcription Translation

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control New LTT caption visibility.
     *
     * @param show TRUE indicates show the caption. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowCaption(bool show) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control New LTT caption font size.
     *
     * @param fontSize Choose the caption font size, see \link ClosedCaptionFontSize \endlink enum
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetNewLTTCaptionFontSize(ClosedCaptionFontSize fontSize) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable New LTT manual captioner
     *
     * @param enable TRUE indicates enable NEW LTT manual captioner. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableNewLTTManualCaptioner(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set New LTT caption speaker language
     *
     * @param languageID Speaker language ID
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetNewLTTSpeakerLanguage(int32_t languageID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Set New LTT caption translation language
     *
     * @param languageID Translation language ID
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetNewLTTTranslationLanguage(int32_t languageID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show New LTT caption both original and translated caption
     *
     * @param show TRUE indicates show both, FALSE indicates show translated caption only.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowNewLTTOriginalAndTranslated(bool show) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Send enable caption request to host.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SendEnableCaptionRequest() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Approve enable caption request
     *
     * @param approve TRUE indicates approve the request. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ApproveEnableCaptionRequest(bool approve) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable caption
     *
     * @param enable TRUE indicates enable caption. Otherwise disable.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableCaption(bool enable) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Query recommend speaker language for closed caption.
     *
     * This method should be called after receiving an OnNewLTTCaptionNotification with type 'NewLTTCaptionNotificationTypeSpeakerLanguageMismatch'.
     *
     * @param[out] recommendLanguage The recommended language information for the current speaker.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetRecommendSpeakerLanguageInfo(NewLTTCaptionLanguage& recommendLanguage) = 0;

    // Interpret Language

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Mute original audio.
     *
     * @param mute TRUE indicates mute the original audio. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError MuteOriginalAudio(bool mute) = 0;
    
    /**
     * @since 6.6.0
     * @zrminver 6.0.0
     *
     * @brief Query the list of supported interpretation languages.
     *
     * @param[out] infoList Supported interpretation languages.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetInterpretLanguageInfoList(InterpretLanguageInfoList& infoList) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Select interpret language.
     *
     * @param languageInfo Specify the interpret language.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectInterpretLanguage(const InterpretLanguageInfo& languageInfo) = 0;

    //LTT Caption message

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Show transcript panel on ZR display.
     *
     * @param show TRUE indicates to receive message update. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowTranscriptPanelOnZR(bool show) = 0;
 
    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Show transcript panel on controller.
     *
     * @param show TRUE indicates to receive message update. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowTranscriptPanelOnZRC(bool show) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Load more LTT caption history messages step by 20.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LoadLTTCaptionMessage() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if is allow view full transcript.
     *
     * @param[out] isAllow Specify the interpret language.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAllowViewFullTranscript(bool& isAllow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get current loaded LTT caption message list.
     *
     * @param[out] messages Specify the interpret language.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetLTTCaptionMessageList(std::vector<LTTCaptionMessage>& messages) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if has more history on ZR.
     *
     * @param[out] hasMore TRUE indicates has more history on ZR.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError HasMoreMessageHistoryOnZR(bool& hasMore) = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_ICLOSEDCAPTIONHELPER_H

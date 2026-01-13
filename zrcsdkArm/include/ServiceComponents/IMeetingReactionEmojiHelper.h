/*!
 * \file IMeetingReactionEmojiHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Meeting Reaction Emoji interfaces.
 */
#ifndef ZRC_IMEETINGREACTIONEMOJIHELPER_H
#define ZRC_IMEETINGREACTIONEMOJIHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum ReactionInfoType
    \brief Reaction info type.
    Here are more detailed structural descriptions.
 */
enum ReactionInfoType
{
    ReactionInfoTypeUnknown,     ///<Invalid
    ReactionInfoTypeEmoji,       ///<Emoji
    ReactionInfoTypeFeedback,    ///<Feedback
};

/*! \enum WebinarEmojiReactionType
    \brief Webinar reaction emoji type.
    Here are more detailed structural descriptions.
 */
enum WebinarEmojiReactionType
{
    WebinarEmojiReactionType_None = 0,      ///<Invalid
    WebinarEmojiReactionType_Clap,          ///<👏
    WebinarEmojiReactionType_Thumbsup,      ///<👍
    WebinarEmojiReactionType_Heart,         ///<❤️
    WebinarEmojiReactionType_Joy,           ///<😂
    WebinarEmojiReactionType_Openmouth,     ///<😮
    WebinarEmojiReactionType_Tada,          ///<🎉
};

/// \brief Reaction status
///
struct ReactionStatus
{
    bool              isReactionEnabled = false;                  ///<TRUE indicates reaction is enabled.
    ReactionFeedback  selectedFeedback = ReactionFeedbackNone;    ///<Selected reaction feedback, see \link ReactionFeedback \endlink enum.
    HandSkinTone      skinTone = HandSkinToneUnknown;             ///<Hand skin tone, see \link HandSkinTone \endlink enum.
    ReactionIntensity intensity = ReactionIntensityHidden;        ///<Reaction intensity, see \link ReactionIntensity \endlink enum.
    bool              isSupportEnableAnimateEmoji = false;        ///<TRUE indicates support enable/disable animate emoji, otherwise not.
    bool              isAnimateEmojiEnabled = false;              ///<TRUE indicates animate emoji is enabled, otherwise not.
};

/// \brief Reaction info
///
struct ReactionInfo
{
    ReactionInfoType type = ReactionInfoTypeUnknown;     ///<Reaction info type, see \link ReactionInfoType \endlink enum.
    std::string      emoji;                              ///<Emoji, in unicode.
    ReactionFeedback feedback = ReactionFeedbackNone;    ///<Reaction feedback, see \link ReactionFeedback \endlink enum.
};

/// \brief Reaction statistics info item.
///
struct ReactionStatisticsInfoItem
{
    ReactionInfo reactionInfo;    ///<Reaction info
    int          count = 0;       ///<Count of the reaction info.
};

/// \brief Reaction statistics info.
///
struct ReactionStatisticsInfo
{
    std::vector<ReactionStatisticsInfoItem> infos;    ///<List of each reaction statistics info item list.
};

/// \brief Meeting Reaction Emoji helper event callback
///
class IMeetingReactionEmojiHelperSink
{
public:
    virtual ~IMeetingReactionEmojiHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Reaction status update notification.
     *
     * @param reactionStatus The reaction status.
     */
    virtual void OnUpdateReactionStatus(const ReactionStatus& reactionStatus) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Reaction statistics info notification.
     *
     * @param reactionStatisticsInfo The reaction statistics info.
     */
    virtual void OnUpdateReactionStatisticsInfoNotification(const ReactionStatisticsInfo& reactionStatisticsInfo) = 0;
};

/// \brief Meeting Reaction Emoji helper interface
///
class IMeetingReactionEmojiHelper
{
public:
    virtual ~IMeetingReactionEmojiHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting reaction emoji helper callback sink.
     *
     * @param pSink A pointer to the IMeetingReactionEmojiHelperSink that receives the meeting reaction emoji helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingReactionEmojiHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting reaction emoji helper callback sink.
     *
     * @param pSink A pointer to the IMeetingReactionEmojiHelperSink that receives the meeting reaction emoji helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingReactionEmojiHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if reaction emoji enabled.
     *
     * @param[out] enabled TRUE indicates reaction emoji is enabled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsReactionEmojiEnabled(bool& enabled) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if reaction feedback enabled.
     *
     * @param[out] enabled TRUE indicates reaction feedback is enabled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsReactionFeedbackEnabled(bool& enabled) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Query if animated reaction enabled.
     *
     * @param[out] enabled TRUE indicates animated reaction is enabled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAnimatedReactionEnabled(bool& enabled) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Send emoji.
     *
     * @param emoji The emoji in unicode. Get the supported emoji list with GetSupportedEmojisForMeeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SendEmoji(const std::string& emoji) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Send reaction feedback.
     *
     * @param feedback The reaction feedback, see \link ReactionFeedback \endlink.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SendReactionFeedback(ReactionFeedback feedback) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable reaction in webinar meeting.
     *
     * @param enable TRUE indicates enable. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableReactionInWebinar(bool enable) = 0;
    
    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Send emoji reaction in webinar meeting.
     *
     * @param type Webinar reaction emoji type, see \link WebinarEmojiReactionType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SendWebinarEmojiReaction(WebinarEmojiReactionType type) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Change skin tone
     *
     * @param skinTone The skin tone, see different HandSkinTone example emojis with GetSupportedEmojiSkinsForMeeting.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeSkinTone(HandSkinTone skinTone) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Change reaction intensity in webinar meeting.
     *
     * @param intensity The reaction intensity, this changes how you see the reactions on screen.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeReactionIntensity(ReactionIntensity intensity) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Clear all feedback.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ClearAllFeedback() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get supported emoji list.
     *
     * @param[out] emojis The emoji list, non-skinTone, default emoji, stored as a json style list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSupportedEmojisForMeeting(std::string& emojis) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get supported emoji skins example.
     *
     * Default emoji takes one single character unicode while emoji with skin tones takes two, first is the base emoji and the second is the skin tone unicode.
     *
     * @param[out] emojiSkins The supported emoji skin tone list, stored as a json style list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSupportedEmojiSkinsForMeeting(std::string& emojiSkins) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get supported emojis by skin tone.
     *
     * @param[out] emojis  The emoji list, non-skinTone, default emoji, stored as a json style list.
     * @param[in] skinTone The skin tone, see \link HandSkinTone \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSupportedEmojisBySkinTone(std::string& emojis, HandSkinTone skinTone) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Check if support enable animate emoji.
     *
     * @param[out] support TRUE indicates support enable animate emoji, otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsSupportEnableAnimateEmoji(bool& support) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Enable animate emoji.
     *
     * @param enable TRUE indicates enable animate emoji, otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableAnimateEmoji(bool enable) = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_IMEETINGREACTIONEMOJIHELPER_H

/*!
 * \file IMeetingChatHelper.h
 * \since Added in ZRC SDK 6.0.0.
 * \brief Meeting Chat interfaces.
 *
 * **
 * ZRC SDK supports sending meeting chat feature since 6.1.0
 * There are two versions of sending meeting chat, check with API IsNewChatExperience.
 * 1. Old meeting chat: For webinar meeting chat or new meeting chat is OFF on Zoom web, which supports send and view messages.
 * 2. New meeting chat: Available when new meeting chat is ON on Zoom web, which supports send and view messages and emojis, reply to messages, CMC(Continuous Meeting Chat) feature, etc.
 *
 * **
 */
#ifndef ZRC_IMEETINGCHATHELPER_H
#define ZRC_IMEETINGCHATHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum MeetingChatPrivilegeType
    \brief Meeting chat privilege type for participants in normal meeting and attendees in webinar.
    Here are more detailed structural descriptions.
*/
enum MeetingChatPrivilegeType
{
    MeetingChatPrivilegeTypeUnknown = -1,           ///<Meeting chat privilege type is unknown.
    MeetingChatPrivilegeTypeAll = 1,                ///<Meeting chat privilege type is all.
    MeetingChatPrivilegeTypeAllPanelist,            ///<Meeting chat privilege type is panelist.
    MeetingChatPrivilegeTypeHost,                   ///<Meeting chat privilege type is host.
    MeetingChatPrivilegeTypeDisableAttendeeChat,    ///<Meeting chat privilege type is none.
    MeetingChatPrivilegeTypeHostPublic              ///<Meeting chat privilege type is host public.
};

/*! \enum MeetingChatPanelistPrivilegeType
    \brief Meeting chat privilege type for panelist in webinar.
    Here are more detailed structural descriptions.
*/
enum MeetingChatPanelistPrivilegeType
{
    MeetingChatPanelistPrivilegeTypeUnknown = -1,       ///<Meeting chat panelist privilege type is unknown.
    MeetingChatPanelistPrivilegeTypeAllPanelist = 1,    ///<Meeting chat panelist privilege type is all panelist.
    MeetingChatPanelistPrivilegeTypeAll                 ///<Meeting chat panelist privilege type is all.
};

/*! \enum MeetingChatMSGSendToType
    \brief Meeting chat message send to type.
    Here are more detailed structural descriptions.
*/
enum MeetingChatMSGSendToType
{
    MeetingChatMSGSendToTypeUnknown = -1,                  ///<Unknown
    MeetingChatMSGSendToTypeToAll = 0,                     ///<Send chat to every one.
    MeetingChatMSGSendToTypeToPanelist = 1,                ///<Send chat to host and panelists
    MeetingChatMSGSendToTypeToIndividualCcPanelist = 2,    ///<Send chat to individual attendee and cc host and panelists.
    MeetingChatMSGSendToTypeToIndividual = 3,              ///<Send chat to individual
    MeetingChatMSGSendToTypeToGreenRoom = 7,               ///<Send chat to backstage
};

/*! \enum MeetingChatMessageType
    \brief Meeting chat message type.
    Here are more detailed structural descriptions.
*/
enum MeetingChatMessageType
{
    MeetingChatMessageType_UNKNOWN = 0,        ///<Unknown.
    MeetingChatMessageType_TEXT = 1,           ///<Message is Text
    MeetingChatMessageType_FILE = 2,           ///<Message is File
    MeetingChatMessageType_IMAGE = 3,          ///<Message is Image, JPG/PNG.
    MeetingChatMessageType_CMC_MEETING = 8,    ///<System message, example "Meeting started/ended ${data & time}"
    MeetingChatMessageType_CMC_GROUP_CHAT =
        9,    ///<System message, example "Messages addressed to "Meeting Group Chat" will also appear in the meeting group chat in Team Chat"
    MeetingChatMessageType_CMC_GUEST_JOIN_OR_LEFT = 10,    ///<System message, example "Darrel Marquez joined as a guest / Darrel Marquez left".
    MeetingChatMessageType_REVOKE_MESSAGE = 11,            ///<System message, example "You deleted a message"
};

/*! \enum MeetingChatMessageSubType
    \brief Meeting chat message sub-type.
    Here are more detailed structural descriptions.
*/
enum MeetingChatMessageSubType
{
    MeetingChatMessageSubType_NOT_SET = 0,    ///<Not set.
    MeetingChatMessageSubType_THREAD = 1,     ///<Thread. A thread is a root message and can contain other comments.
    MeetingChatMessageSubType_COMMENT = 2,    ///<Comment. Message under thread message.
};

/*! \enum MeetingChatMessageState
    \brief Meeting chat message state.
    Here are more detailed structural descriptions.
*/
enum MeetingChatMessageState
{
    MeetingChatMessageState_UNKNOWN = 0,     ///<Unknown
    MeetingChatMessageState_SENDING = 1,     ///<Message sending.
    MeetingChatMessageState_SENT = 2,        ///<Message sent.
    MeetingChatMessageState_RECEIVED = 3,    ///<Message received
};

/*! \enum MeetingChatSyncType
    \brief Meeting chat sync type.
    Here are more detailed structural descriptions.
*/
enum MeetingChatSyncType
{
    MeetingChatSyncType_UNKNOWN = 0,              ///<Unknown
    MeetingChatSyncType_INIT = 1,                 ///<Init action
    MeetingChatSyncType_LOAD_MORE_HISTORY = 2,    ///<Load more history
};

/// \brief Meeting chat privilege
///
struct MeetingChatPrivilege
{
    MeetingChatPrivilegeType         type = MeetingChatPrivilegeTypeUnknown;                             ///<Meeting chat privilege type.
    bool                             isChatOff = false;                                                  ///<TRUE indicates the chat is on.
    bool                             isAttendeeChatAllowed = false;                                      ///<TRUE indicates attendee chat is allowed.
    MeetingChatPanelistPrivilegeType panelistPrivilegeType = MeetingChatPanelistPrivilegeTypeUnknown;    ///<Meeting chat panelist privilege type.
};

/// \brief Share file in meeting chat params.
///
struct ShareFileInMeetingChatParam
{
    std::string previewURL;      ///<File preview URL
    std::string downloadURL;     ///<File download URL
    std::string fileName;        ///<File name
    std::string thumbnailURL;    ///<File thumbnail URL
    std::string fileSize;        ///<File size
    std::string fileID;          ///<File ID
    std::string type;            ///<Type
};

/// \brief Meeting chat message
/// old chat message struct
struct MeetingChatMessage
{
    int32_t                     index = 0;              ///<Index.
    std::string                 messageID;              ///<Message ID.
    std::string                 content;                ///<Content.
    bool                        isSelfSend = false;     ///<TRUE indicates message is sent by self.
    MeetingChatMSGSendToType    msgSendToType = MeetingChatMSGSendToTypeUnknown;    ///<Message send to type, see \link MeetingChatMSGSendToType \endlink enum.
    bool                        isFile = false;         ///<TRUE indicates message is a file.
    std::string                 fileURL;                ///<File URL.
    std::string                 fileName;               ///<File name.
    int32_t                     fileSize = 0;           ///<File size.
    int32_t                     contentType = 0;        ///<Content type. see \link MeetingChatMessageType \endlink enum.
    int32_t                     senderID = 0;           ///<Sender user id.
    int32_t                     receiverID = 0;         ///<Receiver user id.
    std::string                 senderDisplayName;      ///<Sender display name.
    std::string                 receiverDisplayName;    ///<Receiver display name.
    std::string                 timestamp;              ///<Time stamp.
    bool                        isShareFileMessage = false; ///<TRUE indicates message is a share file message.
    ShareFileInMeetingChatParam shareFileInfo;          ///<Share file info.
    bool                        isDeleted = false;      ///<TRUE indicates message is deleted.
    bool                        canBeDeleted = false;   ///<TRUE indicates message can be deleted.
    bool                        isEdited = false;       ///<TRUE indicates message is edited.
};

/// \brief Chat emoji count info.
/// New chat supports emoji reaction.
struct ChatEmojiCountInfo
{
    std::string emoji;                  ///<Emoji.
    int64_t     count = 0;              ///<Count.
    int64_t     firstEmojiTime = 0;     ///<First sent emoji time.
    bool        containMine = false;    ///<TRUE indicates self also sent this emoji.
};

/// \brief Chat emoji comment
/// New chat supports emoji reaction.
struct ChatEmojiComment
{
    std::string  guid;              ///<Guid.
    int32_t      meetingUserID = 0; ///<Meeting user id.
    std::string  displayName;       ///<Display name.
    int64_t      time = 0;          ///<Time.
};

/// \brief New meeting chat message
/// New meeting chat message struct. Time all timestamp.
struct NewMeetingChatMessage
{
    std::string                     messageID;                ///<Message ID
    std::string                     threadID;                 ///<Thread ID.
    MeetingChatMessageType          type = MeetingChatMessageType_UNKNOWN;              ///<Message type, see \link MeetingChatMessageType \endlink enum.
    MeetingChatMessageSubType       subType = MeetingChatMessageSubType_NOT_SET;        ///<Message sub-type, see \link MeetingChatMessageSubType \endlink enum.
    MeetingChatMSGSendToType        chatSendToType = MeetingChatMSGSendToTypeUnknown;   ///<Message send to type, see \link MeetingChatMSGSendToType \endlink enum.
    std::string                     body;                     ///<Message body.
    bool                            isDeleted = false;        ///<TRUE indicates message is deleted.
    bool                            isNotExist = false;       ///<TRUE indicates message not exist.
    bool                            isSupported = false;      ///<TRUE indicates message type is supported.
    MeetingChatMessageState         state = MeetingChatMessageState_UNKNOWN;            ///<Message state, see \link MeetingChatMessageState \endlink enum.
    int64_t                         threadServerTime = 0;     ///<Thread server time in millisecond.
    int64_t                         messageServerTime = 0;    ///<Message server time in millisecond.
    int64_t                         messageEditTime = 0;      ///<Message edit time in millisecond.
    std::string                     senderConfUserID;         ///<Sender conf user ID.
    int32_t                         senderMeetingUserID = 0;  ///<Sender meeting user ID.
    std::string                     senderUserGUID;           ///<Sender user GUID.
    std::string                     senderName;               ///<Sender name.
    std::string                     receiverConfUserID;       ///<Receiver conf user ID.
    int32_t                         receiverMeetingUserID = 0;///<Receiver meeting user ID.
    std::string                     receiverUserGUID;         ///<Receiver user GUID.
    std::string                     receiverName;             ///<Receiver name.
    std::vector<ChatEmojiCountInfo> emojiList;                ///<Emoji list.
    int64_t                         commentCount = 0;         ///<Comment count.
    int64_t                         meetingStartedRealTime = -1;///<[-1] Invalid, meeting started real time.
    int64_t                         meetingEndedRealTime = -1;  ///<[-1] Invalid, meeting ended real time.
    int64_t                         meetingRealDuration = -1;   ///<[-1] Invalid, meeting real duration
    std::string                     fileName;                 ///<File name.
    std::string                     fileID;                   ///<File ID.
    bool                            fileCmcDownAttachPlain = false; ///<TRUE indicates file CMC down attach plain.
    EncryptionAlgorithm             fileEncryptionAlgorithm = EncryptionAlgorithm_Unknown;  ///<File encryption algorithm, see \link EncryptionAlgorithm \endlink enum.
    std::string                     fileE2EKey;               ///<File E2E key.
    std::string                     fileE2EIv;                ///<File E2E iv.
    int64_t                         visibleTime = 0;          ///<Visible time. This indicates the message being updated time including thread's comments. The most recent updated message has the biggest visible time, so it will locate at the bottom of the message list.
    bool                            isSelfSend = false;       ///<TRUE indicates message is self send.
    bool                            canBeDeleted = false;     ///<TRUE indicates message can be deleted.
    bool                            canBeReplied = false;     ///<TRUE indicates message can be replied.
    bool                            isCurrentMeetingMessage = false; ///<TRUE indicates message is current meeting's message.
};

/// \brief New meeting chat thread item.
/// A thread can contain several comments
struct NewMeetingChatThreadItem
{
    NewMeetingChatMessage              thread;      ///<Thread message.
    std::vector<NewMeetingChatMessage> comments;    ///<Comment messages.
};

/// \brief Chat message sync filter
///
struct ChatMessageSyncFilter
{
    MeetingChatSyncType       syncType = MeetingChatSyncType_UNKNOWN;               ///<Sync type, see \link MeetingChatSyncType \endlink enum.
    MeetingChatMessageSubType messageSubType = MeetingChatMessageSubType_NOT_SET;   ///<Message sub-type, see \link MeetingChatMessageSubType \endlink enum.
    uint32_t                  maxSize = 0;          ///<Max size.
    std::string               threadID;             ///<Thread ID.
    int64_t                   startVisibleTime = 0; ///<Start visible time.
};

/// \brief Meeting Chat helper event callback
///
class IMeetingChatHelperSink
{
public:
    virtual ~IMeetingChatHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Chat display settings notification.
     *
     * @param isShowChatNotificationOnZR TRUE indicates show chat notification on ZR. Otherwise not.
     * @param isShowChatListOnZR TRUE indicates show chat list on ZR. Otherwise not.
     */
    virtual void OnChatDisplaySettingsNotification(bool isShowChatNotificationOnZR, bool isShowChatListOnZR) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Meeting chat privilege notification.
     *
     * @param chatPrivilege Meeting chat privilege info.
     */
    virtual void OnMeetingChatPrivilegeNotification(const MeetingChatPrivilege& chatPrivilege) = 0;

    // old meeting chat

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Chat message response.
     *
     * @param result [0] success. Otherwise failed.
     * @param action [0] Fetch, [1] Delete, [2] Send.
     */
    virtual void OnChatMessageResponse(int32_t result, int32_t action) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Meeting chat messages notification.
     *
     * @param messages Meeting chat messages.
     */
    virtual void OnMeetingChatMessagesNotification(const std::vector<MeetingChatMessage>& messages) = 0;

    // new meeting chat

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief CMC info update notification.
     *
     * @param isCMCForBackendEnabled TRUE indicates CMC for backend enabled.
     * @param isTeamChatEnabled TRUE indicates team chat enabled.
     */
    virtual void OnUpdateCMCInfo(bool isCMCForBackendEnabled, bool isTeamChatEnabled) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief CMC tip info update notification.
     *
     * PRIVACY NOTE: Every first time in a meeting, you should always check the CMC tip before send chat.
     * You cannot send chat if CMC tip is required but not confirmed.
     *
     * @param needConfirm TRUE indicates need to confirm CMC tip.
     * @param title Suggest title for the CMC tip.
     * @param message Suggest message for the CMC tip.
     */
    virtual void OnUpdateCMCTip(bool needConfirm, const std::string& title, const std::string& message) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Ready to load message changes notification.
     */
    virtual void OnIsReadyForLoadMessageChanged() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Thread received notification.
     *
     * @param thread Thread message.
     */
    virtual void OnThreadReceived(const NewMeetingChatMessage& thread) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Comment received notification
     *
     * @param comment Comment message.
     * @param updatedThread Comment update triggers its thread update.
     */
     virtual void OnCommentReceived(const NewMeetingChatMessage& comment, const NewMeetingChatMessage& updatedThread) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Thread sent notification
     *
     * @param thread Sent thread message.
     */
    virtual void OnThreadSent(const NewMeetingChatMessage& thread) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Comment sent notification
     *
     * @param comment Sent comment message
     * @param updatedThread Comment update triggers its thread update.
     */
    virtual void OnCommentSent(const NewMeetingChatMessage& comment, const NewMeetingChatMessage& updatedThread) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Thread deleted notification
     *
     * @param thread Deleted thread message.
     */
    virtual void OnThreadDeleted(const NewMeetingChatMessage& thread) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Comment deleted notification
     *
     * @param comment Deleted comment message.
     * @param updatedThread Comment update triggers its thread update.
     */
    virtual void OnCommentDeleted(const NewMeetingChatMessage& comment, const NewMeetingChatMessage& updatedThread) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Messages updated notification.
     *
     * @param messages Messages list.
     */
    virtual void OnMessagesUpdated(const std::vector<NewMeetingChatMessage>& messages) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Emoji updated notification
     *
     * @param messages Messages list.
     */
    virtual void OnEmojiUpdated(const std::vector<NewMeetingChatMessage>& messages) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Message loaded notification
     *
     * @param filter Message sync filter.
     * @param result TRUE indicates success.
     * @param messages Loaded messages.
     * @param hasMoreHistory TRUE indicates has more history messages.
     * @param hasMoreRecent TRUE indicates has more recent messages.
     */
    virtual void OnMessageLoaded(const ChatMessageSyncFilter& filter, bool result, const std::vector<NewMeetingChatMessage>& messages,
                                 bool hasMoreHistory, bool hasMoreRecent) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Emoji updated notification
     *
     * @param messageID Messages ID.
     * @param emojiCode Emoji code.
     * @param isFirstPage TRUE indicates is the first page.
     * @param isLastPage TRUE indicates is the last page.
     * @param emojiSenders Emoji senders' info.
     */
    virtual void OnEmojiDetail(const std::string& messageID, const std::string& emojiCode, bool isFirstPage, bool isLastPage,
                               const std::vector<ChatEmojiComment>& emojiSenders) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Send message failed notification
     */
    virtual void OnSendMessageFailed() = 0;
};

/// \brief Meeting Chat helper interface
///
class IMeetingChatHelper
{
public:
    virtual ~IMeetingChatHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register meeting chat helper callback sink.
     *
     * @param pSink A pointer to the IMeetingChatHelperSink that receives the meeting chat helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingChatHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting chat helper callback sink.
     *
     * @param pSink A pointer to the IMeetingChatHelperSink that receives the meeting chat helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingChatHelperSink* pSink) = 0;

    // Check if new chat experience to choose the API set.

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if new chat experience.
     *
     * @param[out] isNewChat TRUE indicates is new chat. Otherwise old chat.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsNewChatExperience(bool& isNewChat) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Inform ZR that ZRC enters chat page.
     *
     * @param open TRUE indicates enter chat page.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError OpenChatPageOnZRC(bool open) = 0;

    // ==========================================================================
    // New Meeting Chat
    // ==========================================================================

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Confirm CMC tip.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ConfirmCMCTip() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if required to confirm CMC tip.
     *
     * If you are required to confirm cmc tip but haven't ConfirmCMCTip, calls to New chat APIs will fail.
     *
     * @param[out] required TRUE indicates is required to confirm.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsRequiredConfirmCMCTip(bool& required) = 0;

    // ==========================================================================
    // New meeting chat messages load from most recent to history.
    // ==========================================================================

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Load more history thread messages. Start from the most history message, max size 20 one call.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LoadThreadMessages() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Load more history thread's comment messages. Start from the most history thread's comment message, max size 20 one call.
     *
     * @param threadID Specify the thread ID
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError LoadCommentMessages(const std::string& threadID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Send new chat message
     *
     * @param threadID Specify the thread ID if this is a comment. A comment's chatMSGType should be the same as thread's chatMSGType.
     * @param receiverMeetingUserID Specify the receiver meeting user ID if chatMSGType is @ref MeetingChatMSGSendToTypeToIndividual.
     * @param chatMSGType Send to type, see \link MeetingChatMSGSendToType \endlink enum.
     * @param text Send message content.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SendNewChatMessage(const std::string& threadID, int32_t receiverMeetingUserID, MeetingChatMSGSendToType chatMSGType,
                                           const std::string& text) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Delete new chat message
     *
     * @param messageID Message ID
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeleteNewChatMessage(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Edit new chat message
     *
     * @param messageID Message ID
     * @param text The new message content.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EditNewChatMessage(const std::string& messageID, const std::string& text) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Fetch new chat emoji detail
     *
     * @param messageID Message ID
     * @param emojiCode The emoji code.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError FetchNewChatEmojiDetail(const std::string& messageID, const std::string& emojiCode) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Query if meeting chat only allows to react with selected emojis.
     *
     * @param[out] isOnlySelectedEmojisEnabled TRUE indicates only allow participants to react to chat messages with selected emojis. Otherwise all emojis are allowed.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsOnlySelectedEmojisEnabled(bool& isOnlySelectedEmojisEnabled) = 0;

    /**
     * @since 6.2.0
     * @zrminver 6.2.0
     *
     * @brief Query allowed emojis to react to chat messages. Only needed when meeting chat only allows to react with selected emojis.
     *
     * @param[out] selectedEmojis The allowed emojis to react to chat messages, stored as a json style list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSelectedEmojisForReaction(std::string& selectedEmojis) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief React new chat emoji
     *
     * @param messageID Message ID
     * @param add TRUE indicates add emoji. Otherwise revoke emoji.
     * @param emojiCode The emoji code.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ReactNewChatEmoji(const std::string& messageID, bool add, const std::string& emojiCode) = 0;

    // ==========================================================================
    // Help methods
    // ==========================================================================

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if reply chat disabled.
     *
     * @param[out] isReplyChatDisabled TRUE indicates reply chat is disabled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsReplyChatDisabled(bool& isReplyChatDisabled) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if edit message enabled
     *
     * @param[out] isEditMessageEnabled TRUE indicates edit message is enabled
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsEditMessageEnabled(bool& isEditMessageEnabled) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if react emoji enabled
     *
     * @param[out] isReactEmojiEnabled TRUE indicates react emoji is enabled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsReactEmojiEnabled(bool& isReactEmojiEnabled) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if allow hyper links
     *
     * @param[out] isAllowHyperlinks TRUE indicates allow hyper links
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAllowHyperlinks(bool& isAllowHyperlinks) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if ready for load message
     *
     * @param[out] isReadyForLoadMessage TRUE indicates is ready for load message.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsReadyForLoadMessage(bool& isReadyForLoadMessage) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query loaded new meeting chat message list.
     *
     * The messages are ordered by the thread's visibleTime which means the most recent changed message at bottom.
     *
     * @param[out] messages Current loaded new meeting chat messages.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetNewMeetingChatMessageList(std::vector<NewMeetingChatThreadItem>& messages) = 0;

    // ==========================================================================
    // Old meeting chat
    // ==========================================================================

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Fetch chat message.
     *
     * @param start Fetch message start index.
     * @param count Fetch message count. Suggest count 20.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError FetchChatMessages(int32_t start, int32_t count) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Delete chat message.
     *
     * @param messageID Message ID
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeleteChatMessage(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Send chat message.
     *
     * @param receiverID Receiver ID, used when type is @ref MeetingChatMSGSendToTypeToIndividualCcPanelist or @ref MeetingChatMSGSendToTypeToIndividual.
     * @param type Send message to type, see \link MeetingChatMSGSendToType \endlink enum.
     * @param content Send message content.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SendChatMessage(int32_t receiverID, MeetingChatMSGSendToType type, const std::string& content) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query loaded old meeting chat message list.
     *
     * Only include FetchChatMessages returned messages.
     *
     * @param[out] messages Current loaded old meeting chat messages, ordered by index.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetOldMeetingChatMessages(std::vector<MeetingChatMessage>& messages) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query webinar chatted attendees, not include self.
     *
     * Only include FetchChatMessages returned messages' involved attendees.
     *
     * @param[out] chattedAttendees Attendees that chatted in webinar. NOTE: use only userID and userName.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetWebinarChattedAttendees(std::vector<MeetingParticipant>& chattedAttendees) = 0;

    // ==========================================================================
    // Option control
    // ==========================================================================

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show chat notification on ZR.
     *
     * @param show TRUE indicates show chat notification on ZR
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowChatNotificationOnZR(bool show) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Show chat list on ZR.
     *
     * @param show TRUE indicates show chat list on ZR.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowChatListOnZR(bool show) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update chat privilege for participants in normal meeting and attendees in webinar.
     *
     * @param type The meeting chat privilege type, such as \link MeetingChatPrivilegeType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetChatPrivilege(MeetingChatPrivilegeType type) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update Chat Panelist Privilege in webinar.
     *
     * @param type The meeting chat panelist privilege type, such as \link MeetingChatPanelistPrivilegeType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetChatPanelistPrivilege(MeetingChatPanelistPrivilegeType type) = 0;
};

NS_ZRCSDK_END

#endif /* ZRC_IMEETINGCHATHELPER_H */

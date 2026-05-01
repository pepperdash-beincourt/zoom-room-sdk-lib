/*!
 * \file IMeetingQAHelper.h
 * \since Added in ZRC SDK 6.1.0
 * \brief Meeting Q&A interfaces.
 */
#ifndef ZRC_IMEETINGQAHELPER_H
#define ZRC_IMEETINGQAHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum MeetingQAMessageType
    \brief Meeting Q&A message type
    Here are more detailed structural descriptions.
 */
enum MeetingQAMessageType
{
    MeetingQAMessageType_Unknown = 0,   ///<Meeting Q&A message type unknown.
    MeetingQAMessageType_Question,      ///<Meeting Q&A message type question.
    MeetingQAMessageType_Answer,        ///<Meeting Q&A message type answer.
    MeetingQAMessageType_Comment        ///<Meeting Q&A message type comment.
};

/// \brief Meeting Q&A message.
///
struct MeetingQAMessage
{
    MeetingQAMessageType messageType = MeetingQAMessageType_Unknown; ///<Meeting Q&A message type, , such as \link MeetingQAMessageType \endlink enum.
    std::string          messageID;                                  ///<Meeting Q&A message ID.
    uint64_t             timestamp = 0;                              ///<Meeting Q&A message timestamp.
    std::string          content;                                    ///<Meeting Q&A message content.
    std::string          senderName;                                 ///<Meeting Q&A message sender name.
    std::string          senderAvatar;                               ///<Meeting Q&A message sender avatar url.
    bool                 isMyselfMessage = false;                    ///<TRUE indicates this message is sent by myself.
    bool                 isGuest = false;                            ///<TRUE indicates this message is sent by guest.
    bool                 isMarkedAsDeleted = false;                  ///<TRUE indicates this message is deleted.

    //Answer
    std::string questionID;                                          ///<if this message is an answer or a comment, the value indicates belongs to which question.
    bool isPrivate = false;                                          ///<TRUE indicates this answer is private.
    bool isLiveAnswer = false;                                       ///<TRUE indicates this answer is live answer.

    //Question
    bool hasTextAnswers = false;                                     ///<TRUE indicates this question has text answers.
    bool typeAnswering = false;                                      ///<TRUE indicates someone is typing answer.
    std::vector<std::string> typingUserNames;                        ///<the user's names who is typing answer.

    bool hasLiveAnswers = false;                                     ///<TRUE indicates this question has live answers.
    bool liveAnswering = false;                                      ///<TRUE indicates someone is living answer.
    std::vector<std::string> livingUserNames;                        ///<the user's names who is living answer.

    bool amILiveAnswering = false;                                   ///<TRUE indicates I am living answer.
    bool isAnonymous = false;                                        ///<TRUE indicates this question is anonymous.
    bool isMarkedAsAnswered = false;                                 ///<TRUE indicates this question is answered.
    bool isMarkedAsDismissed = false;                                ///<TRUE indicates this question is dismissed.

    //Upvote
    int32_t upvoteNumber = 0;                                        ///<the upvote count of this question.
    bool  isMyselfUpvoted = false;                                   ///<TRUE indicates I upvoted this question.

    std::vector<MeetingQAMessage> answers;                           ///<answers of this question.
};

/// \brief Meeting Q&A helper callback.
///
class IMeetingQAHelperSink
{
public:
    virtual ~IMeetingQAHelperSink() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify if this controller become main ZRC.
     *
     * @param isMainZRC indicates become main ZRC.
     */
    virtual void OnBecomeMainZRC(bool isMainZRC) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify if the meeting Q&A feature is enabled or not by host  / co-host.
     *
     * @param enable indicates the meeting Q&A feature enabled.
     */
    virtual void OnIsMeetingQAEnabled(bool enable) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify if the attendee is allowed to submit questions.
     *
     * @param allow indicates allow the attendee submit questions.
     */
    virtual void OnIsAllowSubmitQuestionsChanged(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify if the attendee is allowed to submit questions anonymously.
     *
     * @param allow indicates allow the attendee submit questions anonymously.
     */
    virtual void OnIsAllowAnonymousQuestionsChanged(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify if the attendee is allowed to view all questions.
     *
     * @param allow indicates allow the attendee view all questions.
     */
    virtual void OnIsAllowViewAllQuestionsChanged(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify if the participant is allowed to upvote.
     *
     * @param allow indicates allow the participant upvote.
     */
    virtual void OnIsAllowUpvoteChanged(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify if the attendee is allowed to comment.
     *
     * @param allow indicates allow the attendee comment.
     */
    virtual void OnIsAllowCommentChanged(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify if the meeting Q&A UI shown on ZR.
     *
     * @param show indicates the meeting Q&A UI shown on ZR.
     */
    virtual void OnIsShowOnZRChanged(bool show) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify the meeting Q&A messages load status.
     *
     * @param isLoadBegin indicates the meeting Q&A messages load begin.
     * @param isLoadEnd indicates the meeting Q&A messages load end.
     */
    virtual void OnLoadMessages(bool isLoadBegin, bool isLoadEnd) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify receive messages when someone creates a question / answer or comment.
     *
     * @param messages contain questions and answers.
     */
    virtual void OnReceiveMessages(const std::vector<MeetingQAMessage>& messages) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Notify update messages when someone operate a question or create answer / comment.
     *
     * @param message contain questions and answers.
     */
    virtual void OnUpdateMessage(const MeetingQAMessage& message) = 0;
};

/// \brief Waiting Room helper interface.
///
class IMeetingQAHelper
{
public:
    virtual ~IMeetingQAHelper() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Register waiting room helper callback sink.
     *
     * @param pSink A pointer to the IMeetingQAHelperSink that receives the waiting room helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingQAHelperSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Deregister waiting room helper callback sink.
     *
     * @param pSink A pointer to the IMeetingQAHelperSink that receives the waiting room helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingQAHelperSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Request to become main ZRC, only main ZRC can operate message. If quit Q&A UI, you should release main ZRC.
     *
     * @param mainZRC TRUE indicates become main ZRC.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BecomeMainZRC(bool mainZRC) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The attendee can call this API to create question.
     *
     * @param context the question content.
     * @param isAnonymous TRUE indicates the question is anonymous.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CreateQuestion(const std::string& context, bool isAnonymous) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host / co-host or panelist can call this API to create answer.
     *
     * @param context the answer content.
     * @param questionID indicates answer which question.
     * @param isPrivate TRUE indicates the answer is private.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CreateAnswer(const std::string& context, const std::string& questionID, bool isPrivate) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The attendee can call this API to create comment.
     *
     * @param context the comment content.
     * @param questionID indicates comment which question.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CreateComment(const std::string& context, const std::string& questionID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host or co-host can call this API to delete message.
     *
     * @param messageID the message ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeleteMessage(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host or co-host can call this API to dismiss question.
     *
     * @param messageID the message ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DismissQuestion(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host or co-host can call this API to reopen question.
     *
     * @param messageID the message ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ReopenQuestion(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host / co-host or panelist can call this API to start composing answer.
     *
     * @param messageID the message ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartComposing(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host / co-host or panelist can call this API to stop composing answer.
     *
     * @param messageID the message ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopComposing(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host / co-host or panelist can call this API to start living answer.
     *
     * @param messageID the message ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartLiving(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host / co-host or panelist can call this API to stop living answer.
     *
     * @param messageID the message ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopLiving(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The participant can call this API to upvote question.
     *
     * @param messageID the message ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UpvoteQuestion(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The participant can call this API to revoke upvote question.
     *
     * @param messageID the message ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RevokeUpvoteQuestion(const std::string& messageID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host or co-host can call this API to enable Q&A feature.
     *
     * @param enable TRUE indicates enable Q&A feature.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableMeetingQA(bool enable) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host or co-host can call this API to allow submit questions
     *
     * @param allow TRUE indicates allow submit questions.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowSubmitQuestions(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host or co-host can call this API to allow submit questions anonymously.
     *
     * @param allow TRUE indicates allow submit questions anonymously.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowAnonymousQuestions(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host or co-host can call this API to allow view all questions.
     *
     * @param allow TRUE indicates allow view all questions.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowViewAllQuestions(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host or co-host can call this API to allow upvote.
     *
     * @param allow TRUE indicates allow upvote.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowUpvote(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The host or co-host can call this API to allow comment.
     *
     * @param allow TRUE indicates allow comment.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError AllowComment(bool allow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief The main ZRC can call this API to show Q&A UI on ZR.
     *
     * @param show TRUE indicates show Q&A UI on ZR.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowOnZR(bool show) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if is main ZRC.
     *
     * @param[out] isMainZRC TRUE indicates is main ZRC.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsMainZRC(bool& isMainZRC) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get all messages.
     *
     * @param[out] messages contains questions and answers.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetAllMessages(std::vector<MeetingQAMessage>& messages) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if meeting Q&A feature enabled.
     *
     * @param[out] isEnabled TRUE indicates meeting Q&A feature enabled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsMeetingQAEnabled(bool& isEnabled) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if allow submit questions.
     *
     * @param[out] isAllow TRUE indicates allow submit questions.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAllowSubmitQuestions(bool& isAllow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if allow anonymous questions.
     *
     * @param[out] isAllow TRUE indicates allow anonymous questions.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAllowAnonymousQuestions(bool& isAllow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if allow view all questions.
     *
     * @param[out] isAllow TRUE indicates allow view all questions.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAllowViewAllQuestions(bool& isAllow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if allow upvote.
     *
     * @param[out] isAllow TRUE indicates allow upvote.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAllowUpvote(bool& isAllow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if allow comment.
     *
     * @param[out] isAllow TRUE indicates allow comment.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsAllowComment(bool& isAllow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if Q&A UI shown on ZR.
     *
     * @param[out] isShow TRUE indicates Q&A UI shown on ZR.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsShowOnZR(bool& isShow) = 0;
};

NS_ZRCSDK_END
#endif    //ZRC_IMEETINGQAHELPER_H

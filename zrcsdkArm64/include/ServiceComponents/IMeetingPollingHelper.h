/*!
 * \file IMeetingPollingHelper.h
 * \since Added in ZRC SDK 6.1.0.
 * \brief Meeting Polling interfaces.
 */
#ifndef ZRC_IMEETINGPOLLINGHELPER_H
#define ZRC_IMEETINGPOLLINGHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum PollingStatus
    \brief Polling status
    Here are more detailed structural descriptions.
 */
enum PollingStatus
{
    PollingStatusNotStart = 0,    ///<Polling not started.
    PollingStatusOpen,            ///<Polling opened.
    PollingStatusClosed,          ///<Polling closed.
    PollingStatusShareResult,     ///<Polling is sharing result.
};

/*! \enum PollingRoleType
    \brief Polling role type. Polling session role type is different from meeting's host/cohost or webinar's panelist or attendee.
    Here are more detailed structural descriptions.
 */
enum PollingRoleType
{
    PollingRoleTypeUnknown = 0,    ///<Invalid
    PollingRoleTypeHost,           ///<Host, can manage polling.
    PollingRoleTypePanelist,       ///<Panelist, view only for polling. If poll host allow PollingOptionTypeAllowPanelistVote, panelist role will turn to attendee role.
    PollingRoleTypeAttendee,       ///<Attendee, can answer questions or participate poll.
};

/*! \enum PollingType
    \brief Polling type.
    Here are more detailed structural descriptions.
 */
enum PollingType
{
    PollingTypeUnknown = 0,    ///<Invalid
    PollingTypeBasic,          ///<Basic polling
    PollingTypeAdvanced,       ///<Advanced polling
    PollingTypeQuiz,           ///<Quiz
};

/*! \enum PollingQuestionType
    \brief Polling question type
    Here are more detailed structural descriptions.
 */
enum PollingQuestionType
{
    PollingQuestionTypeUnknown = 0,    ///<Invalid.
    PollingQuestionTypeSingle,         ///<Single choice.
    PollingQuestionTypeMulti,          ///<Multi choice.
    PollingQuestionTypeMatching,       ///<Matching.
    PollingQuestionTypeRankOrder,      ///<Rank order.
    PollingQuestionTypeShortAnswer,    ///<Short answer.
    PollingQuestionTypeLongAnswer,     ///<Long answer.
    PollingQuestionTypeFillBlank,      ///<Fill blank.
    PollingQuestionTypeRatingScale,    ///<Rating scale.
    PollingQuestionTypeDropDown,       ///<Drop down choice.
};

/*! \enum PollingActionType
    \brief Polling action type
    Here are more detailed structural descriptions.
 */
enum PollingActionType
{
    PollingActionTypeFetch = 0,    ///<Fetch polling detail. No used, use GetPollingDetail instead.
    PollingActionTypeStart,        ///<Start polling
    PollingActionTypeClose,        ///<Close polling
    PollingActionTypeSubmit,       ///<Submit polling
    PollingActionTypeShare,        ///<Share polling result
    PollingActionTypeStopShare,    ///<Stop share polling result.
    PollingActionTypeError,        ///<No used.
    PollingActionTypeReopen        ///<Reopen polling.
};

/*! \enum PollingDisplayType
    \brief Polling display type
    Here are more detailed structural descriptions.
 */
enum PollingDisplayType
{
    PollingDisplayTypeInvalid = -1,    ///<Invalid
    PollingDisplayTypeMainView = 0,    ///<Display question information.
    PollingDisplayTypeImage,           ///<Display question image.
    PollingDisplayTypeAnswers          ///<Display question answer.
};

/*! \enum PollingOptionType
    \brief Polling option type
    Here are more detailed structural descriptions.
 */
enum PollingOptionType
{
    PollingOptionTypeAllowRandomOrder = 0,    ///<Option type: AllowRandomOrder, applicable to PollingTypeQuiz
    PollingOptionTypeShowQuestionOneByOne,    ///<Option type: ShowQuestionOneByOne, applicable to PollingTypeQuiz
    PollingOptionTypeShowAnswerToAll,         ///<Option type: ShowAnswerToAll, applicable to PollingTypeQuiz
    PollingOptionTypeAllowPanelistVote        ///<Option type: AllowPanelistVote, applicable in webinar
};

/// \brief Polling option
///
struct PollingOption
{
    PollingOptionType type = PollingOptionTypeAllowRandomOrder;    ///<The option type
    bool              isAllow = false;                             ///<The option value.
};

/// \brief Polling answer
///
struct PollingAnswer
{
    std::string answerID;         ///<Answer ID. [Required] if you are SyncAnswer with ZR.
    std::string answerContent;    ///<Answer content

    std::string textAnswer;            ///<User input text answer. [Required] if you are SyncAnswer with ZR.
    bool        isSelected = false;    ///<TRUE indicates current answer is selected. [Required] if you are SyncAnswer with ZR.
    int32_t     selectedCount = 0;     ///<Answer being selected count, used for display scale
};

/// \brief Polling question info.
///
struct PollingQuestionInfo
{
    std::string         questionID;                                   ///<Question ID. [Required] if you are SyncAnswer with ZR.
    PollingQuestionType questionType = PollingQuestionTypeUnknown;    ///<Question type, [Required] if you are SyncAnswer with ZR. see \link PollingQuestionType \endlink enum.
    bool                isRequired = false;                           ///<TRUE indicates the question must be answered before submit polling.
    std::string         questionContent;                              ///<Question content
    int32_t             questionReplyCount = 0;                       ///<Question being replied count.
    std::string         imageURL;                                     ///<Question image URL.
    std::string         imageContent;                                 ///<Question image content.
    int32_t             questionIndex = -1;                           ///<Question's index in polling. [Required] if you are SyncAnswer with ZR.
    std::string         questionSequence;           ///<Sub-questions serial number. For main question, this is same as its questionIndex.
    int32_t             minCharacters = 0;          ///<Short/Long answer question type minimum input characters.
    int32_t             maxCharacters = 0;          ///<Short/Long answer question type maximum input characters.
    bool                isCaseSensitive = false;    ///<TRUE indicates FillBlankType question is case sensitive.

    std::vector<PollingQuestionInfo> subQuestions;    ///<Sub-question list. If sub-questions exist, [Required] if you are SyncAnswer with ZR.
    std::vector<PollingAnswer>       answers;         ///<The answer list. [Required] if you are SyncAnswer with ZR.
    std::vector<PollingAnswer>       rightAnswers;    ///<The answer list. The right answer only contains answerId.
};

/// \brief Polling info.
///
struct PollingInfo
{
    std::string                      pollingId;                                  ///<Polling ID
    std::string                      pollingName;                                ///<Polling Name
    PollingType                      pollingType = PollingTypeUnknown;           ///<Polling type, see \link PollingType \endlink enum.
    PollingStatus                    pollingStatus = PollingStatusNotStart;      ///<Polling actual status, see \link PollingType \endlink enum.
    PollingStatus                    myPollingStatus = PollingStatusNotStart;    ///<Self polling status, see \link PollingType \endlink enum.
    bool                             isAnonymous = false;                        ///<TRUE indicates polling is anonymous.
    int32_t                          totalVotedCount = 0;                        ///<Polling total voted count.
    int32_t                          votableUserCount = 0;                       ///<Votable user count.
    uint64_t                         elapsedTime = 0;                            ///<Polling starting elapsed time.
    int32_t                          questionCount = 0;                          ///<Polling question count.
    std::vector<PollingQuestionInfo> questions;                                  ///<Polling questions.
};

/// \brief Polling user action info.
///
struct PollingUserAction
{
    PollingActionType actionType = PollingActionTypeFetch;        ///<Polling action type, see \link PollingActionType \endlink enum.
    int32_t           result = 0;                                 ///<Action result
    std::string       pollingID;                                  ///<Polling ID
    PollingStatus     pollingStatus = PollingStatusNotStart;      ///<Polling actual status, see \link PollingType \endlink enum.
    PollingStatus     myPollingStatus = PollingStatusNotStart;    ///<My polling status, see \link PollingType \endlink enum.
    std::string       errorMessage;                               ///<Error message
    int32_t           unanswerQuestionIndex = -1;                 ///<Unanswered question index.
};

/// \brief Polling display on ZR screen info.
/// Used to sync status between multi ZRC and ZR display.
struct PollingDisplayOnZRInfo
{
    bool               isShowOnZROption = false;            ///<TRUE indicates polling is showing on ZR.
    bool               isShowOnInChargeZRC = false;         ///<TRUE indicates polling is showing on in charge ZRC.
    PollingDisplayType type = PollingDisplayTypeInvalid;    ///<Display on ZR screen type, see \link PollingDisplayType \endlink enum.
    std::string        pollingID;                           ///<Polling ID
    std::string        questionID;                          ///<Question ID
    int32_t            questionIndex = -1;                  ///<Question index.
};

/// \brief Polling basic info
///
struct PollingBasicInfo
{
    PollingRoleType          roleType = PollingRoleTypeUnknown;    ///<My role type in this polling, see \link PollingRoleType \endlink enum.
    bool                     isPollingButtonVisible = false;       ///<PollingRoleTypeHost check this show polling button.
    bool                     isPollSessionInactive = false;        ///<TRUE indicates polling session inactive.
    bool                     isInChargeDevice = false;    ///<TRUE indicates current is the incharge device of polling.
    PollingInfo              detailPolling;               ///<Current detail polling info.
    std::vector<PollingInfo> pollings;                    ///<Pollings' basic info.
    bool                     isShowOnZrOption = false;    ///<TRUE indicates polling is showing on ZR.
    bool                     isInRandomOrder = false;     ///<TRUE indicates show polling questions in random order.
    bool                     isShowCorrectAnswerToAll = false;    ///<TRUE indicates show polling correct answer to all.
    bool                     isAllowPanelistVote = false;         ///<TRUE indicates allow panelist to vote.
    bool                     isShowQuestionOneByOne = false;      ///<TRUE indicates show polling questions one by one.
    bool                     isShowPollOverlay = false;           ///<TRUE indicates show polling overlay.
};

/// \brief Meeting Polling helper event callback
///
class IMeetingPollingHelperSink
{
public:
    virtual ~IMeetingPollingHelperSink() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Update polling basic info notification.
     *
     * @param basicInfo The polling basic info.
     */
    virtual void OnUpdatePollingBasicInfo(const PollingBasicInfo& basicInfo) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Polling detail info notification.
     *
     * @param detailPolling The polling detail info.
     */
    virtual void OnUpdateDetailPollingInfo(const PollingInfo& detailPolling) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Polling status change notification.
     *
     * @param polling Polling info.
     * @param roleType Polling role type.
     * @param deviceID Polling incharge device ID.
     */
    virtual void OnPollingStatusChange(const PollingInfo& polling, PollingRoleType roleType, const std::string& deviceID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Receive polling doc notification.
     *
     * @param detailPolling The polling detail info.
     * @param pollings All the pollings basic info.
     * @param isPollingButtonVisible TRUE indicates polling button is visible for host role.
     */
    virtual void OnPollingDocReceived(const PollingInfo& detailPolling, const std::vector<PollingInfo>& pollings, bool isPollingButtonVisible) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Polling result update notification.
     *
     * @param polling The polling info.
     */
    virtual void OnPollingResultUpdated(const PollingInfo& polling) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Polling action notification.
     *
     * @param userAction The polling user action data.
     */
    virtual void OnPollingActionNotification(const PollingUserAction& userAction) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Take in charge device update notification.
     *
     * @param isSelfIncharge TRUE indicates current is the incharge device of current polling.
     */
    virtual void OnTakeInChargeNotification(bool isSelfIncharge) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Polling display on ZR notification.
     *
     * @param enable TRUE indicates polling display on ZR screen.
     */
    virtual void OnPollingDisplayOnZRNotification(bool enable) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Indicates whether show overlay on ZR.
     *
     * @param isShow Show overlay status.
     */
    virtual void OnPollingIsShowPollOverlay(bool isShow) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Polling role type changed notification.
     *
     * @param roleType Polling role type, see \link PollingRoleType \endlink enum.
     * @param isPollingButtonVisible TRUE indicates polling button is visible for host role.
     */
    virtual void OnPollingRoleChanged(PollingRoleType roleType, bool isPollingButtonVisible) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Polling session inactive notification.
     *
     * @param isInactive TRUE indicates polling session is inactive.
     */
    virtual void OnPollingSessionInactiveNotification(bool isInactive) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Polling options update notification.
     *
     * @param pollingOptions The polling options.
     */
    virtual void OnPollingOptionsUpdated(const std::vector<PollingOption>& pollingOptions) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Polling question's answer update notification.
     *
     * @param question The polling question info.
     */
    virtual void OnPollingAnswerUpdated(const PollingQuestionInfo& question) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Download result.
     *
     * @param localFilePath The specified local file path when calling download API.
     * @param result [0] success. Otherwise failed.
     */
    virtual void OnDownloadingFinished(const std::string& localFilePath, uint32_t result) = 0;
};

/// \brief Meeting Polling helper interface
///
class IMeetingPollingHelper
{
public:
    virtual ~IMeetingPollingHelper() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Register meeting polling helper callback sink.
     *
     * @param pSink A pointer to the IMeetingPollingHelperSink that receives the meeting polling helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IMeetingPollingHelperSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Deregister meeting polling helper callback sink.
     *
     * @param pSink A pointer to the IMeetingPollingHelperSink that receives the meeting polling helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IMeetingPollingHelperSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get polling detail information.
     *
     * @param pollingID Specific polling ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetPollingDetail(const std::string& pollingID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Operate polling.
     *
     * @param actionType Specify action type, see \link PollingActionType \endlink enum.
     * @param pollingID Specific polling ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError OperatePolling(PollingActionType actionType, const std::string& pollingID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Sync answer with ZR.
     *
     * Check the [Required] fields in @ref PollingQuestionInfo and @ref PollingAnswer when sync answer with ZR.
     *
     * @param question The polling question info.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SyncAnswer(const PollingQuestionInfo& question) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Update polling option.
     *
     * Polling options can only be configured before polling is started except @ref PollingOptionTypeShowAnswerToAll.
     *
     * @param option The polling option.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UpdatePollingOption(const PollingOption& option) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Take in charge of polling.
     *
     * Only incharge device can answer questions.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError TakeInChargePolling() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query if you are in charge of polling.
     *
     * @param[out] inCharge TRUE indicates you are in charge of polling.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsSelfInChargePolling(bool& inCharge) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Query your polling role type.
     *
     * @param[out] myRole The polling role type, see \link PollingRoleType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetMyPollingRoleType(PollingRoleType& myRole) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Update display on ZR info.
     *
     * This is only able to be used after polling started.
     *
     * @param option The display on ZR option.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UpdateDisplayOnZRInfo(const PollingDisplayOnZRInfo& option) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Enable or disable show poll overlay.
     *
     * This is only able to be used after polling started.
     *
     * @param enable Enable or disable.
     * @param pollingID Polling ID.
     * @param questionID Question ID.
     * @param questionIndex Question index.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableShowPollOverlay(bool enable, const std::string& pollingID, const std::string& questionID, int32_t questionIndex) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Show or hide poll overlay.
     *
     * This is only able to be used after polling started.
     *
     * @param show Show or hide.
     * @param pollingID Polling ID.
     * @param questionID Question ID.
     * @param questionIndex Question index.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ShowPollOverlay(bool show, const std::string& pollingID, const std::string& questionID, int32_t questionIndex) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Download question image
     *
     * This will not auto create the folder if not exist.
     *
     * @param imageUrl The image url.
     * @param localFilePath Specify the local file path of this image. For example, ..../MyImageFolder/this_question.jpg
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DownloadQuestionImage(const std::string& imageUrl, const std::string& localFilePath) = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_IMEETINGPOLLINGHELPER_H

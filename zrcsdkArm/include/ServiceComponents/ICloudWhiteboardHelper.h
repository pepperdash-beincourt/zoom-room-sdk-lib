/*!
 * \file ICloudWhiteboardHelper.h
 * \since Added in ZRC SDK 6.1.0
 * \brief Cloud whiteboard interfaces.
 */
#ifndef ZRC_ICLOUDWHITEBOARDHELPER_H
#define ZRC_ICLOUDWHITEBOARDHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum CloudWhiteboardAction
    \brief Cloud whiteboard action
    Here are more detailed structural descriptions.
 */
enum CloudWhiteboardAction
{
    CloudWhiteboardActionNone = 0,    ///<Invalid.
    CloudWhiteboardActionClose        ///<Close cloud whiteboard.
};

/*! \enum CloudWhiteboardUserChoiceType
    \brief Respond to ZR cloud whiteboard warning.
    Here are more detailed structural descriptions.
 */
enum CloudWhiteboardUserChoiceType
{
    CloudWhiteboardUserChoiceTypeCancel = 0,    ///<Cancel
    CloudWhiteboardUserChoiceTypeConfirm = 1    ///<Confirm
};

/*! \enum CloudWhiteboardWebWbState
    \brief Cloud whiteboard webview state
    Here are more detailed structural descriptions.
 */
enum CloudWhiteboardWebWbState
{
    CloudWhiteboardWebWbStateNone = 0,            ///<None
    CloudWhiteboardWebWbStateLoading = 1,         ///<Cloud whiteboard is loading.
    CloudWhiteboardWebWbStateWebviewError = 2,    ///<Cloud whiteboard load error.
    CloudWhiteboardWebWbStateShareError = 3,      ///<Cloud whiteboard share error.
    CloudWhiteboardWebWbStateLoaded = 4           ///<Cloud whiteboard loaded successfully.
};

/*! \enum WhiteboardSharePrivilegeType
    \brief Whiteboard share privilege type, apply to both classic and cloud whiteboard.
    Here are more detailed structural descriptions.
 */
enum WhiteboardSharePrivilegeType
{
    WhiteboardSharePrivilegeTypeUnknown = 0,     ///<Invalid
    WhiteboardSharePrivilegeTypeHostGrab = 1,    ///<Only host can grab share when someone else is sharing.
    WhiteboardSharePrivilegeTypeAnyoneGrab,      ///<Anyone can grab share when someone else is sharing.
};

/*! \enum WhiteboardInitiatePrivilegeType
    \brief Whiteboard initiate privilege type, apply to both classic and cloud whiteboard.
    Here are more detailed structural descriptions.
 */
enum WhiteboardInitiatePrivilegeType
{
    WhiteboardInitiatePrivilegeTypeUnknown = 0,        ///<Invalid
    WhiteboardInitiatePrivilegeTypeHostOnly = 1,       ///<Only host can initiate new whiteboard in the meeting.
    WhiteboardInitiatePrivilegeTypeInternalUsers,      ///<Internal users can initiate new whiteboard in the meeting.
    WhiteboardInitiatePrivilegeTypeAllParticipants,    ///<All participants can initiate new whiteboard in the meeting.
};

/// \brief Cloud whiteboard status
///
struct CloudWhiteboardStatus
{
    bool isWhiteboardOpen = false;    ///<TRUE indicates cloud whiteboard is open.
    bool shouldPromptSave = false;    ///<TRUE indicates should prompt save dialog when leave meeting..
};

/// \brief Whiteboard settings info
///
struct WhiteboardSettingsInfo
{
    WhiteboardSharePrivilegeType    sharePrivilege = WhiteboardSharePrivilegeTypeUnknown;    ///<Who can start share when someone else is sharing.
    WhiteboardInitiatePrivilegeType initiatePrivilege = WhiteboardInitiatePrivilegeTypeUnknown;    ///<Who can initiate new whiteboard in the meeting.
    bool                            isLocked = false;                                              ///<TRUE indicates settings are locked.
};

/// \brief Cloud whiteboard helper event callback.
///
class ICloudWhiteboardHelperSink
{
public:
    virtual ~ICloudWhiteboardHelperSink() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Cloud whiteboard status notification.
     *
     * @param status The cloud whiteboard status.
     */
    virtual void OnCloudWhiteboardStatusNotification(const CloudWhiteboardStatus& status) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Whiteboard settings notification.
     *
     * @param info The cloud whiteboard settings info.
     */
    virtual void OnWhiteboardSettingsNotification(const WhiteboardSettingsInfo& info) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Cloud whiteboard webview state notification.
     *
     * @param webWbState The cloud whiteboard webview state.
     * @param errorCode The error code.
     */
    virtual void OnCloudWhiteboardWebWbStateNotification(CloudWhiteboardWebWbState webWbState, int32_t errorCode) = 0;
};

/// \brief Cloud whiteboard helper interface.
///
class ICloudWhiteboardHelper
{
public:
    virtual ~ICloudWhiteboardHelper() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Register cloud whiteboard helper callback sink.
     *
     * @param pSink A pointer to the ICloudWhiteboardHelperSink that receives the cloud whiteboard helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(ICloudWhiteboardHelperSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Deregister cloud whiteboard callback sink.
     *
     * @param pSink A pointer to the ICloudWhiteboardHelperSink that receives the cloud whiteboard helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(ICloudWhiteboardHelperSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Request cloud whiteboard action.
     *
     * @param action Specify the action, see \link CloudWhiteboardAction \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RequestCloudWhiteboardAction(CloudWhiteboardAction action) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Confirm cloud whiteboard user choice, respond to ZR cloud whiteboard warning.
     *
     * @param choice Specify the confirm choice, see \link CloudWhiteboardUserChoiceType \endlink enum.
     * @param webWbState Specify the current webWbState, see \link CloudWhiteboardWebWbState \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ConfirmCloudWhiteboardUserChoice(CloudWhiteboardUserChoiceType choice, CloudWhiteboardWebWbState webWbState) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Change whiteboard share privilege type.
     *
     * @param type Specify the whiteboard share privilege type.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeWhiteboardSharePrivilege(WhiteboardSharePrivilegeType type) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Change whiteboard initiate privilege type.
     *
     * @param type Specify the whiteboard initiate privilege type.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ChangeWhiteboardInitiatePrivilege(WhiteboardInitiatePrivilegeType type) = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_ICLOUDWHITEBOARDHELPER_H

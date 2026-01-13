/*!
 * \file IControlSystemHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Room control system interfaces.
 */
#ifndef ZRC_ICONTROLSYSTEMHELPER_H
#define ZRC_ICONTROLSYSTEMHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum ControlSystemDeviceStatus
    \brief Control system device status
    Here are more detailed structural descriptions.
 */
enum ControlSystemDeviceStatus
{
    ControlSystemDeviceStatusDisconnected = 0,  ///<Disconnected
    ControlSystemDeviceStatusConnecting,        ///<Connecting
    ControlSystemDeviceStatusConnected          ///<Connected
};

/*! \enum ControlSystemDeviceMethodType
    \brief Control system device method type
    Here are more detailed structural descriptions.
 */
enum ControlSystemDeviceMethodType
{
    ControlSystemDeviceMethodTypeNone = 0,      ///<None
    ControlSystemDeviceMethodTypeInit,          ///<Init
    ControlSystemDeviceMethodTypeAction,        ///<Action
    ControlSystemDeviceMethodTypeActions,       ///<Actions
    ControlSystemDeviceMethodTypeRange          ///<Range
};

/*! \enum ControlSystemErrorCode
    \brief Control system error code
    Here are more detailed structural descriptions.
 */
enum ControlSystemErrorCode
{
    ControlSystemErrorCodeUnknown = -1,         ///<Unknown
    ControlSystemErrorCodeSuccess,              ///<Success
    ControlSystemErrorCodeNoConfig,             ///<No Config error
    ControlSystemErrorCodeJsonSyntax,           ///<Json syntax error
    ControlSystemErrorCodeJsonConfig,           ///<Json config error
    ControlSystemErrorCodeIP,                   ///<IP error
    ControlSystemErrorCodeIPIsPublic,           ///<IP is public
    ControlSystemErrorCodeDeviceID,             ///<Device ID error
    ControlSystemErrorCodeMethodID,             ///<Method ID error
    ControlSystemErrorCodeParamID,              ///<Param ID error
    ControlSystemErrorCodeIP2SLSettings,        ///<IP2SL settings error
    ControlSystemErrorCodeEmptyDevice           ///<Empty device
};

/*! \enum ControlSystemUpdateDeviceType
    \brief Control system update device type
    Here are more detailed structural descriptions.
 */
enum ControlSystemUpdateDeviceType
{
    ControlSystemUpdateDeviceTypeInit = 0,      ///<Init
    ControlSystemUpdateDeviceTypeAdd = 1,       ///<Add
    ControlSystemUpdateDeviceTypeUpdate = 2     ///<Update
};

/// \brief Control system device param
///
struct ControlSystemDeviceParam
{
    std::string paramID;        ///<Param ID
    std::string name;           ///<Name
    std::string icon;           ///<Icon
    std::string value;          ///<Value
};

/// \brief Control system device method
///
struct ControlSystemDeviceMethod
{
    std::string                           methodID;         ///<Method ID
    std::string                           name;             ///<Name
    std::string                           icon;             ///<Icon
    std::string                           command;          ///<Command
    ControlSystemDeviceMethodType         type = ControlSystemDeviceMethodTypeNone; ///<Method type, see \link ControlSystemDeviceMethodType \endlink enum.
    std::vector<ControlSystemDeviceParam> params;           ///<Param list
    bool                                  isMain = false;   ///<TRUE indicates this is a main method.
    bool                                  isHidden = false; ///<TRUE indicates this is hidden.
};

/// \brief Control system device info
///
struct ControlSystemDeviceInfo
{
    std::string                               deviceID;     ///<Device ID
    std::string                               name;         ///<Name
    std::string                               icon;         ///<Icon
    ControlSystemDeviceStatus                 status = ControlSystemDeviceStatusDisconnected;   ///<Device status, see \link  ControlSystemDeviceStatus \endlink enum.
    std::vector<ControlSystemDeviceMethod>    methods;      ///<Supports methods
};

/// \brief Control system device list
///
struct ControlSystemDeviceList
{
    ControlSystemErrorCode               errorCode = ControlSystemErrorCodeUnknown; ///<Error code, see \link ControlSystemErrorCode \endlink enum.
    std::vector<ControlSystemDeviceInfo> devices;           ///<Devices
};

/// \brief Control system scene info
///
struct ControlSystemSceneInfo
{
    std::string sceneID;    ///<Scene ID
    std::string name;       ///<Name
    std::string icon;       ///<Icon
};

/// \brief Control System helper event callback
///
class IControlSystemHelperSink
{
public:
    virtual ~IControlSystemHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Enable Zoom Room Control System notification.
     *
     * @param enable TRUE indicates ZRCS is enabled. Otherwise not.
     */
    virtual void OnEnableZRCSNotification(bool enable) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Zoom Room Control System device list update notification.
     *
     * @param type Update type.
     * @param list Incoming control system device list. Combine usage with type. Init, Add or Update.
     */
    virtual void OnUpdateZRCSDeviceList(ControlSystemUpdateDeviceType type, const ControlSystemDeviceList& list) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Zoom Room Control System scene list update notification.
     *
     * @param scenes Control system scene infos.
     */
    virtual void OnUpdateZRCSSceneList(const std::vector<ControlSystemSceneInfo>& scenes) = 0;
};

/// \brief Control System  helper interface
///
class IControlSystemHelper
{
public:
    virtual ~IControlSystemHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register ControlSystem helper callback sink.
     *
     * @param pSink A pointer to the IControlSystemHelperSink that receives the ControlSystem helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IControlSystemHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister meeting ControlSystem callback sink.
     *
     * @param pSink A pointer to the IControlSystemHelperSink that receives the ControlSystem helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IControlSystemHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query if control system is enabled currently.
     *
     * @param[out] enabled TRUE indicates control system is enabled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IsZRCSEnabled(bool& enabled) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Notify ZR controller enter room control system state.
     *
     * If Room control config some events when controller opens room control system, it will be triggered.
     *
     * @param open TRUE indicates controller has entered room control system state.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError OpenRoomControls(bool open) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query current control system device list info.
     *
     * @param[out] deviceList The device list info.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetZRCSDeviceList(ControlSystemDeviceList& deviceList) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Connect to ZRCS device.
     *
     * @param deviceID Device ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ConnectZRCSDevice(const std::string& deviceID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Control ZRCS device, execute the action with specified params.
     *
     * @param deviceID Device ID
     * @param methodID Method ID
     * @param paramID Param ID
     * @param value  Param value
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ControlZRCSDevice(const std::string& deviceID, const std::string& methodID, const std::string& paramID, const std::string& value) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query current control system scene list.
     *
     * @param[out] scenes The config scenes.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetZRCSSceneList(std::vector<ControlSystemSceneInfo>& scenes) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Execute control system scene.
     *
     * @param sceneID Specify the scene ID to execute.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError ExecuteZRCSScene(const std::string& sceneID) = 0;
};

NS_ZRCSDK_END

#endif //ZRC_ICONTROLSYSTEMHELPER_H

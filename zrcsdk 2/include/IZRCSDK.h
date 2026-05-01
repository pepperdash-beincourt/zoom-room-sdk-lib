/*!
 * \file IZRCSDK.h
 * \version 7.0.0
 * \since Added in ZRC SDK 6.0.0
 * \brief Entrance of ZRC SDK.
 */
#ifndef ZRC_IZRCSDK_H
#define ZRC_IZRCSDK_H

#include "ZRCSDKDefines.h"
#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum ProxyType
    \brief Proxy type
    Here are more detailed structural descriptions.
 */
enum ProxyType
{
    ProxyTypeNoProxy,   ///<None
    ProxyTypePAC,       ///<Proxy Auto-configuration
    ProxyTypeManual,    ///<Manual set proxy
};

/// \brief Proxy information
///
struct ProxyInfo
{
    ProxyType proxyType = ProxyTypeNoProxy;   ///<Proxy type
    std::string pacPath;                      ///<PAC(Proxy Auto-configuration) file path
    std::string httpProxyHost;                ///<HTTP proxy host
    std::string httpProxyPort;                ///<HTTP proxy port
    std::string httpsProxyHost;               ///<HTTPS proxy host
    std::string httpsProxyPort;               ///<HTTPS proxy port
};

class IZoomRoomsService;

/// \brief ZRC SDK event callback.
///
class IZRCSDKSink
{
public:
    virtual ~IZRCSDKSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request device manufacturer.
     *
     * @return Device manufacturer string.
     */
    virtual std::string OnGetDeviceManufacturer() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request device model.
     *
     * @return Device model string.
     */
    virtual std::string OnGetDeviceModel() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request device serial number.
     *
     * @return Device serial number string.
     */
    virtual std::string OnGetDeviceSerialNumber() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request device mac address.
     *
     * @return Device mac address string.
     */
    virtual std::string OnGetDeviceMacAddress() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request device IP address.
     *
     * @return Device IP address string.
     */
    virtual std::string OnGetDeviceIP() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request device firmware version.
     *
     * @return Device firmware version string.
     */
    virtual std::string OnGetFirmwareVersion() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request app name.
     *
     * @return App name string.
     */
    virtual std::string OnGetAppName() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request app version.
     *
     * @return App version string.
     */
    virtual std::string OnGetAppVersion() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request app developer.
     *
     * @return App developer string.
     */
    virtual std::string OnGetAppDeveloper() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request app contact.
     *
     * @return App contact string.
     */
    virtual std::string OnGetAppContact() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief SDK request app content dir path to save log file and config file. In the path, app can read/write files.
     *
     * @return App content dir full path string.
     */
    virtual std::string OnGetAppContentDirPath() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Proxy authentication prompt.
     *
     * @return Result of inputting username and password for proxy.
     */
    virtual bool OnPromptToInputUserNamePasswordForProxyServer(const std::string& proxyHost, uint32_t port, const std::string& description) = 0;
};

/// \brief ZRC SDK API interface.
///
class ZRCSDK_API IZRCSDK
{
public:
    virtual ~IZRCSDK() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Create Instance and register ZRC SDK callback sink.
     *
     * @param pSdkSink A pointer to the IZRCSDKSink that receives the ZRC SDK event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    static IZRCSDK* CreateInstance(IZRCSDKSink* pSdkSink);

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the ZRC SDK instance.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    static IZRCSDK* GetInstance();

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Destroy the ZRC SDK instance.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    static ZRCSDKError DestroyInstance();

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the build version of current ZRC SDK.
     *
     * @param[out] version The build version.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSDKVersion(std::string& version) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Get the display version of current ZRC SDK.
     *
     * @param[out] displayVersion The display version.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSDKDisplayVersion(std::string& displayVersion) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.0.0
     *
     * @brief Get the expire date of an evaluation build ZRC SDK.
     *
     * @param[out] expireDate The expire date. Empty string indicates not an evaluation build.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSDKExpireDate(std::string& expireDate) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief For linux platform only, you must call this API in main thread to trigger SDK event loop, time interval suggest 150ms.
     */
    virtual void HeartBeat() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.0.0
     *
     * @brief The log system has a cache mechanism, before you catch the log files, please call this API.
     */
    virtual void ForceFlushLog() = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Init Zoom web domain. If not manually set, the default web domain is https://zoom.us
     *
     * @param domain Zoom web domain.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError InitWebDomain(const std::string& domain) = 0;

    /**
     * @since 6.3.0
     * @zrminver 6.0.0
     *
     * @brief Set Proxy Info.
     *
     * @param proxyInfo Proxy Info.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SetProxyInfo(const ProxyInfo& proxyInfo) = 0;

    /**
     * @since 6.3.0
     * @zrminver 6.0.0
     *
     * @brief Provide username and password for proxy.
     *
     * @param host Proxy host.
     * @param port Proxy port.
     * @param userName Proxy username.
     * @param password Proxy password.
     * @param cancel Cancel to input username and password or not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UserInputUsernamePasswordForProxy(const std::string& host, const uint32_t& port, const std::string& userName, const std::string& password, bool cancel) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Create a ZoomRoomsService.
     *
     * @param roomID The IZoomRoomsService's ID (optional). If specified, this ID will be used as Serial Number filed in Zoom Web portal.
     * @return If the function succeeds, the return value is a pointer to IZoomRoomsService. Otherwise the function returns NULL.
     *
     * @changelog
     * - 6.3.0: Added an optional parameter roomID to support control multiple Zoom Rooms simultaneously, defaults to @ref ZRCSDK_DEFAULT_ROOM_ID .
     * Specify the same roomID will return the same IZoomRoomsService each IZoomRoomsService can be used to control a separate Zoom Rooms client.
     */
    virtual IZoomRoomsService* CreateZoomRoomsService(const std::string& roomID = ZRCSDK_DEFAULT_ROOM_ID) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Destroy the ZoomRoomsService.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DestroyZoomRoomsService(IZoomRoomsService* pZoomRoomsService) = 0;

    /**
     * @since 6.3.0
     * @zrminver 6.0.0
     *
     * @brief Query all the existing created IZoomRoomsServices, including previous successfully paired IZoomRoomsServices.
     *
     * Each time app launches, ZRC SDK will recover your previous successfully paired IZoomRoomsServices. You can continue to use them to control Zoom Rooms.
     *
     * @param[out] result The existing IZoomRoomsService list.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError QueryAllZoomRoomsServices(std::vector<ZoomRoomInfo>& result) = 0;
};

NS_ZRCSDK_END
#endif    //ZRC_IZRCSDK_H

#pragma once

#include "include/IZRCSDK.h"

USING_NS_ZRCSDK

class CZRCSDkSink : public IZRCSDKSink
{
public:
    virtual ~CZRCSDkSink() {}

    std::string OnGetDeviceManufacturer() { return "Zoom Demo"; }

    virtual std::string OnGetDeviceModel() { return "Linux"; }

    virtual std::string OnGetDeviceSerialNumber() { return "FP38052191H"; }

    virtual std::string OnGetDeviceMacAddress() { return "08:00:27:AB:CD:EF"; }

    virtual std::string OnGetDeviceIP() { return "192.168.1.156"; }

    virtual std::string OnGetFirmwareVersion() { return "1.0.0"; }

    virtual std::string OnGetAppName() { return "My ZRC SDK"; }

    virtual std::string OnGetAppVersion() { return "1.0.1"; }

    virtual std::string OnGetAppDeveloper() { return "My App Developer"; }

    virtual std::string OnGetAppContact() { return "my.email@zoom.us"; }

    virtual std::string OnGetAppContentDirPath()
    {
        // specify a path that ZRC SDK can read & write.
        return "";
    }

    virtual bool OnPromptToInputUserNamePasswordForProxyServer(const std::string& proxyHost, uint32_t port, const std::string& description)
    {
        return true;
    }
};

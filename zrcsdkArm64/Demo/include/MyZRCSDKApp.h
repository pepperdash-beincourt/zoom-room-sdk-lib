#ifndef MyZRCSDKApp_h
#define MyZRCSDKApp_h

#include <string>
#include "IZRCSDK.h"

const char* getInputLine();

class MyZRCSDKApp
{
public:
    void AppInit();
    void HeartBeat();
    void ReceiveCommand(std::string command);

private:
    void InitServices();

private:
    NS_ZRCSDK::IZoomRoomsService* m_roomService = nullptr;
};

#endif    // !MyZRCSDKApp_h

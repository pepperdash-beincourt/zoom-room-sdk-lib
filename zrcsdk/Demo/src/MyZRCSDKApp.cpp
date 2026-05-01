#include "MyZRCSDKApp.h"
#include "ZRCSDKSink.h"
#include "sinks.h"

USING_NS_ZRCSDK

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

#define BUFFER_MAX 1024

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <windows.h>
#include <process.h>

char   g_nbstdinBuffer[2][BUFFER_MAX];
HANDLE g_input[2];
HANDLE g_process[2];

DWORD WINAPI consoleInput(LPVOID lpPara)
{
    while (true)
    {
        for (int i = 0; i < 2; i++)
        {
            fgets(g_nbstdinBuffer[i], BUFFER_MAX, stdin);
            SetEvent(g_input[i]);
            WaitForSingleObject(g_process[i], INFINITE);
        }
    }
    return 0;
}

void createNBStdin()
{
    DWORD tid;
    CreateThread(nullptr, 1024, &consoleInput, 0, 0, &tid);
    for (int i = 0; i < 2; i++)
    {
        g_input[i] = CreateEvent(nullptr, false, false, nullptr);
        g_process[i] = CreateEvent(nullptr, false, false, nullptr);
        g_nbstdinBuffer[i][0] = '\0';
    }
}

const char* getInputLine()
{
    DWORD n = WaitForMultipleObjects(2, g_input, false, 0);
    if (n == WAIT_OBJECT_0 || n == WAIT_OBJECT_0 + 1)
    {
        n = n - WAIT_OBJECT_0;
        SetEvent(g_process[n]);
        return g_nbstdinBuffer[n];
    }
    else
    {
        return nullptr;
    }
}

#endif


#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__MACOS__)

char            g_nbstdinBuffer[BUFFER_MAX];
char            g_input[BUFFER_MAX];
char            g_output[BUFFER_MAX];
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  empty = PTHREAD_COND_INITIALIZER;
bool            available = true;

void* consoleInput(void* param)
{
    while (true)
    {
        fgets(g_input, BUFFER_MAX, stdin);
        pthread_mutex_lock(&m);
        while (available == false) pthread_cond_wait(&empty, &m);
        memcpy(g_nbstdinBuffer, g_input, BUFFER_MAX); //checked safe
        memset(g_input, 0, BUFFER_MAX); //checked safe
        available = false;
        pthread_mutex_unlock(&m);
    }
    return 0;
}

void createNBStdin()
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, consoleInput, NULL) != 0)
    {
        printf("Unable to create consoleInput thread");
        exit(1);
    }
}

const char* getInputLine()
{
    pthread_mutex_lock(&m);
    memcpy(g_output, g_nbstdinBuffer, BUFFER_MAX); //checked safe
    memset(g_nbstdinBuffer, 0, BUFFER_MAX); //checked safe
    available = true;
    pthread_mutex_unlock(&m);
    pthread_cond_signal(&empty);
    return g_output;
}

#endif

void MyZRCSDKApp::InitServices()
{
    IZRCSDKSink* sdkSink = new CZRCSDkSink();
    IZRCSDK*     sdk = IZRCSDK::CreateInstance(sdkSink);

    m_roomService = sdk->CreateZoomRoomsService();
    IZoomRoomsServiceSink* pRoomSink = new AutoIZoomRoomsServiceSink();
    m_roomService->RegisterSink(pRoomSink);

    IMeetingService*     meetingService = m_roomService->GetMeetingService();
    IMeetingServiceSink* meetingServiceSink = new AutoIMeetingServiceSink();
    meetingService->RegisterSink(meetingServiceSink);

    IMeetingAudioHelper*     meetingAudioHelper = meetingService->GetMeetingAudioHelper();
    IMeetingAudioHelperSink* meetingAudioHelperSink = new AutoIMeetingAudioHelperSink();
    meetingAudioHelper->RegisterSink(meetingAudioHelperSink);

    createNBStdin();
}

void MyZRCSDKApp::AppInit()
{
    InitServices();
}

void MyZRCSDKApp::HeartBeat()
{
    IZRCSDK* sdk = IZRCSDK::GetInstance();
    sdk->HeartBeat();
}

// assume my command format as below
#define PAIR                  "pair"
#define UNPAIR                "unpair"
#define START_INSTANT_MEETING "startmeeting"
#define LEAVE_MEETING         "leavemeeting"
#define END_MEETING           "endmeeting"
#define UPDATE_MY_AUDIO       "mute"

void MyZRCSDKApp::ReceiveCommand(std::string commond)
{
    // Ignore empty commands
    if (commond.empty() || commond.find_first_not_of(" \t\n\r") == std::string::npos)
    {
        return;
    }
    
    std::cout << " ReceiveCommand:" << commond << std::endl;
    std::istringstream iss(commond);
    std::string        api;
    iss >> api;

    std::cout << " api is " << api << std::endl;

    if (api == PAIR)
    {
        std::string activationCode;
        iss >> activationCode;
        std::cout << " activationCode is " << activationCode << std::endl;

        if (!m_roomService)
            return;
        ZRCSDKError result = m_roomService->PairRoomWithActivationCode(activationCode);
        if (result == ZRCSDKERR_SUCCESS)
        {
            std::cout << "Send request: PairRoomWithActivationCode success!" << std::endl;
        }
    }
    else if (api == UNPAIR)
    {
        if (!m_roomService)
            return;
        ZRCSDKError result = m_roomService->UnpairRoom();
        if (result == ZRCSDKERR_SUCCESS)
        {
            std::cout << "Send request: UnpairRoom success!" << std::endl;
        }
    }
    else if (api == START_INSTANT_MEETING)
    {
        if (!m_roomService || !m_roomService->GetMeetingService())
            return;
        ZRCSDKError result = m_roomService->GetMeetingService()->StartInstantMeeting();
        if (result == ZRCSDKERR_SUCCESS)
        {
            std::cout << "Send request: StartInstantMeeting success!" << std::endl;
        }
    }
    else if (api == UPDATE_MY_AUDIO)
    {
        // mute audio
        if (!m_roomService || !m_roomService->GetMeetingService() || !m_roomService->GetMeetingService()->GetMeetingAudioHelper())
            return;
        ZRCSDKError result = m_roomService->GetMeetingService()->GetMeetingAudioHelper()->UpdateMyAudioStatus(true);
        if (result == ZRCSDKERR_SUCCESS)
        {
            std::cout << "Send request: Mute audio success!" << std::endl;
        }
    }
    else if (api == LEAVE_MEETING)
    {
        if (!m_roomService || !m_roomService->GetMeetingService())
            return;
        ZRCSDKError result = m_roomService->GetMeetingService()->ExitMeeting(ExitMeetingCmdLeave);
        if (result == ZRCSDKERR_SUCCESS)
        {
            std::cout << "Send request: leave meeting success!" << std::endl;
        }
    }
    else if (api == END_MEETING)
    {
        if (!m_roomService || !m_roomService->GetMeetingService())
            return;
        ZRCSDKError result = m_roomService->GetMeetingService()->ExitMeeting(ExitMeetingCmdEnd);
        if (result == ZRCSDKERR_SUCCESS)
        {
            std::cout << "Send request: end meeting success!" << std::endl;
        }
    }
}

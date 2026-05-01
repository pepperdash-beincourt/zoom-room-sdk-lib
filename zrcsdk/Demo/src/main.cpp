#include "MyZRCSDKApp.h"
#include <stdio.h>
#include <iostream>

#if defined(__linux) || defined(__linux__) || defined(linux)
#include "uv.h"
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <windows.h>
#endif

MyZRCSDKApp app;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
void inputCallback(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
#elif defined(__linux) || defined(__linux__) || defined(linux)
void inputCallback(uv_timer_t *handle)
#endif
{
    const char* req = getInputLine();
    if (req) 
    {
        app.ReceiveCommand(req);
    }
}

#if defined(__linux) || defined(__linux__) || defined(linux)
void heartBeat(uv_timer_t *handle)
{
    app.HeartBeat();
}
#endif

int main(int argc, char *argv[])
{
    std::cout << " app start..." << std::endl;
    app.AppInit();

#if defined(__linux) || defined(__linux__) || defined(linux)
    uv_timer_t clientReqTimer;
    uv_timer_init(uv_default_loop(), &clientReqTimer);
    uv_timer_start(&clientReqTimer, inputCallback, 1000, 1000);

    uv_timer_t heartBeatTimer;
    uv_timer_init(uv_default_loop(), &heartBeatTimer);
    uv_timer_start(&heartBeatTimer, heartBeat, 150, 150);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    uv_loop_close(uv_default_loop());

#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

    SetTimer(NULL, 0, 1000 * 0.5, (TIMERPROC)&inputCallback);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
#endif

    return 0;
}

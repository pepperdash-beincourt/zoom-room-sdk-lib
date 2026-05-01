#include "MyZRCSDKApp.h"
#include <stdio.h>
#include <iostream>
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <dispatch/dispatch.h>

MyZRCSDKApp app;

// C++ logic
void inputCallback() {
    std::string req = getInputLine();
    if (!req.empty())
    {
        app.ReceiveCommand(req);
    }
}

int main(int argc, char *argv[]) {
    @autoreleasepool {
        std::cout << "App start..." << std::endl;

        // Create a GCD timer on a global queue
        dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);

        if (timer) {
            dispatch_source_set_timer(timer,
                dispatch_time(DISPATCH_TIME_NOW, 0),
                1000 * NSEC_PER_MSEC,   // Interval: 1000 ms
                100 * NSEC_PER_MSEC);   // Leeway

            dispatch_source_set_event_handler(timer, ^{
                inputCallback();
            });

            dispatch_resume(timer);
        }

        app.AppInit();
        // Keep the command-line app alive
        [[NSRunLoop currentRunLoop] run];
    }

    return 0;
}

//
//  XMainRunLoop_IOS.m
//  TestGCDCPP
//
//  Created by ximiao on 16/3/3.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "XRunLoop.hpp"

class CFRunLoopSourceContextContener {
public:
    CFRunLoopSourceContext mContext;
};

CFRunLoopSourceContextContener *gContext;
CFRunLoopSourceRef gRunLoopSource;
CFRunLoopRef gMainRunloop;
std::shared_ptr<IXRunLoop> gXMainRunloop;

void RunLoopSourceScheduleRoutine (void *info, CFRunLoopRef rl, CFStringRef mode)
{
    
}
void RunLoopSourcePerformRoutine (void *info)
{
    getMainRunLoop()->_do();
}
void RunLoopSourceCancelRoutine (void *info, CFRunLoopRef rl, CFStringRef mode)
{
    CFRelease(gRunLoopSource);
    delete gContext;
}

void init()
{
    gMainRunloop = CFRunLoopGetMain();
    gContext = new CFRunLoopSourceContextContener();
    gContext->mContext =  {0, NULL, NULL, NULL, NULL, NULL, NULL,
        &RunLoopSourceScheduleRoutine,
        &RunLoopSourceCancelRoutine,
        &RunLoopSourcePerformRoutine};
    gRunLoopSource = CFRunLoopSourceCreate(NULL, 0, &(gContext->mContext));
    CFRunLoopRef runLoop = CFRunLoopGetCurrent();
    CFRunLoopAddSource(runLoop, gRunLoopSource, kCFRunLoopDefaultMode);
    
    gXMainRunloop = std::make_shared<XMainRunloop>();
}

void startMainRunLoop() {
    init();
}

std::shared_ptr<IXRunLoop> getMainRunLoop() {
    return gXMainRunloop;
}

void XMainRunloop::run() {
}
bool XMainRunloop::weakUp(XRunLoopSource *source) {
    IXRunLoop::weakUp(source);
    CFRunLoopSourceSignal(gRunLoopSource);
    CFRunLoopWakeUp(gMainRunloop);
    return true;
}
void XMainRunloop::wait() {
}
bool XMainRunloop::waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) {
    CFRunLoopRunInMode(kCFRunLoopDefaultMode , 1000, false);
    return true;
}




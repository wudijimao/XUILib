//
// Created by ximiao on 16/7/12.
//

#include "XMainRunloop_android.hpp"

static std::shared_ptr<IXRunLoop> gXMainRunloop = std::make_shared<XMainRunloop>();

static ALooper *androidMainLooper = nullptr;


        std::shared_ptr<IXRunLoop> getMainRunLoop() {
    return gXMainRunloop;
}


void XMainRunloop::run() {
}

bool XMainRunloop::weakUp(XRunLoopSource *source) {
    IXRunLoop::weakUp(source);
    ALooper_wake(androidMainLooper);
    return true;
}

void XMainRunloop::wait() {
}

bool XMainRunloop::waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) {
    //CFRunLoopRunInMode(kCFRunLoopDefaultMode , 1000, false);
    return true;
}

int _ALooper_callbackFunc(int fd, int events, void *data) {
    gXMainRunloop->_do();
}

void initMainRunloop() {
    androidMainLooper = ALooper_forThread();
    ALooper_addFd(androidMainLooper, 0, 0, ALOOPER_EVENT_INPUT, &_ALooper_callbackFunc, nullptr);
}
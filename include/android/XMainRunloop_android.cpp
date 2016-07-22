//
// Created by ximiao on 16/7/12.
//

#include "XMainRunloop_android.hpp"

std::shared_ptr<IXRunLoop> gXMainRunloop;



std::shared_ptr<IXRunLoop> getMainRunLoop() {
    return gXMainRunloop;
}


void XMainRunloop::run() {
}
bool XMainRunloop::weakUp(XRunLoopSource *source) {
    IXRunLoop::weakUp(source);
    //CFRunLoopSourceSignal(gRunLoopSource);
    //CFRunLoopWakeUp(gMainRunloop);
    return true;
}
void XMainRunloop::wait() {
}
bool XMainRunloop::waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) {
    //CFRunLoopRunInMode(kCFRunLoopDefaultMode , 1000, false);
    return true;
}

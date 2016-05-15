//
//  XRunLoop.cpp
//  TestGCDCPP
//
//  Created by ximiao on 16/3/3.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XRunLoop.hpp"


std::shared_ptr<IXRunLoop> getCurrentRunLoop() {
	//TODO::make it right
	std::thread::id threadId = std::this_thread::get_id();
	return getMainRunLoop();
}

void XRunLoop::run() {
    while (_do()){}
}

bool XRunLoop::weakUp(XRunLoopSource *source) {
    IXRunLoop::weakUp(source);
    cv.notify_one();
    return true;
}
void XRunLoop::wait() {
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait(lk);
}
bool XRunLoop::waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) {
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait_until(lk, in_time);
    return true;
}

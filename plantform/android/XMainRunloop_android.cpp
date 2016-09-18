//
// Created by ximiao on 16/7/12.
//

#include "XMainRunloop_android.hpp"
#include <unistd.h>
//#include <utils/Looper.h>

static std::shared_ptr<IXRunLoop> gXMainRunloop = std::make_shared<XMainRunloop>();

static ALooper *androidMainLooper = nullptr;


        std::shared_ptr<IXRunLoop> getMainRunLoop() {
    return gXMainRunloop;
}

//static inline Looper* ALooper_to_Looper(ALooper* alooper) {
//    return reinterpret_cast<Looper*>(alooper);
//}

void XMainRunloop::run() {
}

//int _ALooper_callbackFunc(int fd, int events, void *data) {
//    gXMainRunloop->_do();
//    return 1;
//}

void doMainRunloop() {
    gXMainRunloop->_do();
}

struct android_app *gAndroidApp;


static void android_app_write_cmd(struct android_app* android_app, int8_t cmd) {
    if (write(android_app->msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
    }
}

bool XMainRunloop::weakUp(XRunLoopSource *source) {
    IXRunLoop::weakUp(source);
    android_app_write_cmd(gAndroidApp, APP_CMD_CUSTOM);
    return true;
}

void XMainRunloop::wait() {
}

bool XMainRunloop::waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) {
    //CFRunLoopRunInMode(kCFRunLoopDefaultMode , 1000, false);
    return true;
}



void initMainRunloop(struct android_app *state) {
    gAndroidApp = state;
    androidMainLooper = ALooper_forThread();
    //ALooper_addFd(androidMainLooper, ALOOPER_POLL_CALLBACK, 0, ALOOPER_EVENT_INPUT, &_ALooper_callbackFunc, nullptr);
}
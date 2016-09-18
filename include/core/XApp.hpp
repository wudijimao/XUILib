//
//  XApp.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
//#include "WindowsManager.h"
#include "IXWindow.hpp"
#include "stdafx.hpp"
#ifdef TARGET_OS_ANDROID
#include <android_native_app_glue.h>
#endif

namespace XDUILib
{
    enum class XAppStatus {
        Foreground,
        Background,
        Killed,
    };
    
    class SIMPLEDIRECTUI_API XApp {
    public:
        static XApp& thisApp();
    public:
#ifdef TARGET_OS_IPHONE
		int run(int argc, char * argv[]);
#endif // 
#ifdef TARGET_OS_MSWINDOWS
		int run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);
#endif
#ifdef TARGET_OS_ANDROID
        int run(struct android_app* state);
#endif
    //overWrite
        virtual bool init();
        virtual void onStatusChanged(XAppStatus status, XAppStatus lastStatus);
        virtual ~XApp();
        void showSoftKeyBoard(bool bShow);
    public:
        //std::shared_ptr<WindowsManager> windowsManager;
        void setMainWindow(std::shared_ptr<IXWindow> window);
        inline std::shared_ptr<IXWindow> mainWindow() {
                return _mainWindow;
        }
        void internalInit();
    private:
        std::shared_ptr<IXWindow> _mainWindow = nullptr;
        static XApp *_thisApp;

    };
}
//
//  XApp.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
//#include "WindowsManager.h"
#include "IBaseWindow.hpp"
#include "stdafx.hpp"


namespace XDUILib
{
    enum class XAppStatus {
        Foreground,
        Background,
        Killed,
    };
    
    class XApp {
    public:
        static XApp& thisApp();
    public:
        int run(int argc, char * argv[]);
    //overWrite
        virtual bool init();
        virtual void onStatusChanged(XAppStatus status, XAppStatus lastStatus);
    public:
        //std::shared_ptr<WindowsManager> windowsManager;
        void setMainWindow(std::shared_ptr<IXWindow> window) {
            _mainWindow = window;
        }
        inline std::shared_ptr<IXWindow> mainWindow() {
            return _mainWindow;
        }
    private:
        std::shared_ptr<IXWindow> _mainWindow;
        static XApp *_thisApp;
    };
}
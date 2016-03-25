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
    class enum XAppStatus {
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
        bool init();
        void onStatusChanged(XAppStatus status, XAppStatus lastStatus);
    public:
        //std::shared_ptr<WindowsManager> windowsManager;
        std::shared_ptr<IXWindow> mainWindow;
    };
}
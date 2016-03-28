//
//  XDUILib_ios.m
//  XDUILib
//
//  Created by ximiao on 16/3/17.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "../XDUILib.hpp"
#import <UIKit/UIKit.h>
#import "XDUIAppDelegate.h"

#import "../core/XWindow.hpp"


namespace XDUILib
{
    XApp& XApp::thisApp(){
        static XApp app;
        return app;
    }
    bool XApp::init() {
        this->mainWindow.reset(new XWindow());
        return true;
    }
    int XApp::run(int argc, char * argv[]) {
        @autoreleasepool {
            return UIApplicationMain(argc, argv, nil, NSStringFromClass([XDUIAppDelegate class]));
        }
    }
}
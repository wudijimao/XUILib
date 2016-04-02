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
    XApp *gThisAppTemp;//临时解决方案，先暂存在这里
    XApp& XApp::thisApp(){
        return *gThisAppTemp;
    }
    int XApp::run(int argc, char * argv[]) {
        gThisAppTemp = this;
        @autoreleasepool {
            return UIApplicationMain(argc, argv, nil, NSStringFromClass([XDUIAppDelegate class]));
        }
    }
}
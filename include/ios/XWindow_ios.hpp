//
//  XWindow_ios.h
//  XDUILib
//
//  Created by ximiao on 16/3/29.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/stdafx.hpp"
#include "../core/XWindow.hpp"
#import "XDUIViewController.h"

class XWindow_ios : public XWindow {
public:
    ~XWindow_ios();
    XWindow_ios();
    virtual void showInFront();
    UIWindow *_window;
    bool init(CAEAGLLayer* drawable);
};

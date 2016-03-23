//
//  XWindow.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/IBaseWindow.hpp"
#import <UIKit/UIKit.h>

class XWindow : public IXWindow {
public:
    XWindow();
    ~XWindow();
    UIWindow *window;
    void showInFront();
    std::shared_ptr<IXCanvas> _canvas;
private:
};
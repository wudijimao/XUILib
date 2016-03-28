//
//  XWindow.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/IBaseWindow.hpp"
#include "../core/UIView.hpp"
#import <UIKit/UIKit.h>

class XWindow : public IXWindow {
public:
    XWindow();
    ~XWindow();
    UIWindow *window;
    virtual void showInFront();
    std::shared_ptr<IXCanvas> _canvas;
    std::shared_ptr<IXRender> _render;
    virtual void setRootViewController(std::shared_ptr<XUI::UIViewController> rootViewController);
private:
    std::shared_ptr<XUI::UIViewController> _rootController;
    friend class XUI::UIView;
    XResource::XRect _rect;
};
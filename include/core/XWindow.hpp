//
//  XWindow.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "stdafx.hpp"
#include "../core/IXWindow.hpp"


class XWindow : public IXWindow {
public:
    ~XWindow();
    std::shared_ptr<IXCanvas> _canvas;
    std::shared_ptr<IXRender> _render;
    virtual void setRootViewController(std::shared_ptr<XUI::UIViewController> rootViewController);
    //do not call it by libray user
    void update();
    void input(XTouch *touchs, unsigned int count);
    void dispatchInput();
protected:
    std::vector<std::shared_ptr<XTouch>> _touchList;
    XWindow();
    std::shared_ptr<XUI::UIViewController> _rootController;
    friend class XUI::UIView;
    XResource::XRect _rect;
};
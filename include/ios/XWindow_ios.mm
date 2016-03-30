//
//  XWindow.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XWindow_ios.hpp"
#include "../core/GLRender.hpp"
#include "GLCanvas_ios.hpp"

std::shared_ptr<IXWindow> IXWindow::createWindow() {
    return std::make_shared<XWindow_ios>();
}

XWindow_ios::~XWindow_ios() {
    [(XDUIViewController*)this->_window.rootViewController removeBelongWindow];
}

XWindow_ios::XWindow_ios() {
    CGRect rect = [UIScreen mainScreen].bounds;
    this->_window = [[UIWindow alloc] initWithFrame:rect];
    _rect.X(0.0);
    _rect.Y(0.0);
    _rect.Width(rect.size.width);
    _rect.Height(rect.size.height);
    
    auto controller = [[XDUIViewController alloc] init];
    this->_window.rootViewController = controller;
    auto canvas = std::make_shared<GLCanvas_ios>();
    canvas->init((CAEAGLLayer*)controller.view.layer);
    ((GLKView*)controller.view).context = canvas->_context;
    _canvas = canvas;
    _render = std::make_shared<GLRender>();
    _render->Init(_canvas.get());
    [controller setBelongWindow:this];
}


void XWindow_ios::showInFront() {
    [this->_window makeKeyAndVisible];
}

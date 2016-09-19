//
//  XWindow.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XWindow_ios.hpp"
#include "../../include/core/GLRender.hpp"
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
    setSize(XResource::XDisplaySize(rect.size.width, rect.size.height));
    
    auto controller = [[XDUIViewController alloc] init];
    [controller setBelongWindow:this];
    this->_window.rootViewController = controller;
}

bool XWindow_ios::init(CAEAGLLayer* drawable) {
    auto canvas = std::make_shared<GLCanvas_ios>();
    canvas->init(drawable);
    XResource::gHighResolutionPixelScale = drawable.contentsScale;
    _canvas = canvas;
    initFinished();
    return true;
}


void XWindow_ios::showInFront() {
    [this->_window makeKeyAndVisible];
    //this->update();  可能引起黑屏渲染不出东西？
}

//
//  XWindow.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XWindow_ios.hpp"
#import "XDUIViewController.h"
#import "../core/GLRender.hpp"
#import "../core/UIView.hpp"

XUI::UIViewController *rootController;

XWindow::XWindow() {
    CGRect rect = [UIScreen mainScreen].bounds;
    this->window = [[UIWindow alloc] initWithFrame:rect];
    
    auto controller = [[XDUIViewController alloc] init];
    this->window.rootViewController = controller;
    
    _canvas.reset([controller initOpenGLES]);
    _render.reset(new GLRender());
    _render->Init(_canvas.get());
    rootController = new XUI::UIViewController();
}

XWindow::~XWindow() {
}

void XWindow::showInFront() {
    [this->window makeKeyAndVisible];
    rootController->getView().layoutSubViews();
    rootController->getView().draw(*(_render.get()));
    _canvas->Present();
}
//
//  XWindow.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "../core/XWindow.hpp"
#import "XDUIViewController.h"
#import "../core/GLRender.hpp"
#import "../core/UIView.hpp"


XWindow::XWindow() {
    CGRect rect = [UIScreen mainScreen].bounds;
    this->window = [[UIWindow alloc] initWithFrame:rect];
    _rect.X(0.0);
    _rect.Y(0.0);
    _rect.Width(rect.size.width);
    _rect.Height(rect.size.height);
    
    auto controller = [[XDUIViewController alloc] init];
    this->window.rootViewController = controller;
    _canvas = [controller initOpenGLES];
    _render.reset(new GLRender());
    _render->Init(_canvas.get());
}

XWindow::~XWindow() {
}

void XWindow::showInFront() {
    [this->window makeKeyAndVisible];
    XResource::XRect rect = _rect;
    rect.X(0.0);
    rect.Y(0.0);
    auto view = _rootController->getView();
    view->layout(rect);
    view->draw(*(_render.get()));
    _canvas->Present();
}

void XWindow::setRootViewController(std::shared_ptr<XUI::UIViewController> rootViewController) {
    _rootController = rootViewController;
}
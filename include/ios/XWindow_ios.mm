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

XUI::UIViewController *rootController;

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
    rootController = new XUI::UIViewController();
    auto testSubView = std::make_shared<XUI::UIView>();
    testSubView->setBkgColor(XResource::XUIColor::redColor());
    testSubView->setRect(XResource::XRectPro(20,20, 150,150));
    rootController->getView()->addSubView(testSubView);
}

XWindow::~XWindow() {
}

void XWindow::showInFront() {
    [this->window makeKeyAndVisible];
    XResource::XRect rect = _rect;
    rect.X(0.0);
    rect.Y(0.0);
    auto view = rootController->getView();
    view->layout(rect);
    view->draw(*(_render.get()));
    _canvas->Present();
}
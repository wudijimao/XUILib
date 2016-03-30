//
//  XWindow.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/29.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XWindow.hpp"
#include "GLRender.hpp"

XWindow::XWindow() {
    _rect.X(0.0);
    _rect.Y(0.0);
    _rect.Width(200.0);
    _rect.Height(200.0);
}

XWindow::~XWindow() {
}

void XWindow::update() {
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
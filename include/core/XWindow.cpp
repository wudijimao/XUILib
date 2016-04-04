//
//  XWindow.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/29.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XWindow.hpp"
#include "GLRender.hpp"
#include "../core/UIView.hpp"

XWindow::XWindow() {
    _rect.X(0.0);
    _rect.Y(0.0);
    _rect.Width(375.0);
    _rect.Height(625.0);
}

XWindow::~XWindow() {
}

void XWindow::update() {
    _canvas->clear();
    XResource::XRect rect = _rect;
    rect.X(0.0);
    rect.Y(0.0);
    auto view = _rootController->getView();
    view->layout(rect);
    view->draw(*(_render.get()));
    _canvas->Present();
}

void XWindow::input(XTouch *touchs, unsigned int count) {
    for (int i = 0; i < count; ++i, ++touchs) {
        _touchList.push_back(std::shared_ptr<XTouch>(touchs));
    }
}
void XWindow::dispatchInput() {
    std::map<XUI::UIView *, std::vector<std::shared_ptr<XTouch>>> touchsMap;
    std::map<XUI::UIView *, std::shared_ptr<XUI::UIView>> viewPtrMap;
    for (auto touch : _touchList) {
        auto view = _rootController->view()->getResponseSubView(touch);
        if (view != nullptr) {
            touch->_belongView = view.get();
            touchsMap[view.get()];
            auto iter = touchsMap.find(view.get());
            iter->second.push_back(touch);
            viewPtrMap[view.get()] = view;
        }
    }
    for (auto iter : touchsMap) {
        viewPtrMap[iter.first]->onTouch(iter.second);
        _rootController->onTouch(iter.second);
    }
}

void XWindow::setSize(const XResource::XSize &size) {
	_rect.Width(size.Width());
	_rect.Height(size.Height());
}
void XWindow::setPositon(const XResource::XPoint &pos) {
	_rect.X(pos.X());
	_rect.Y(pos.Y());
}

void XWindow::setRootViewController(std::shared_ptr<XUI::UIViewController> rootViewController) {
    _rootController = rootViewController;
}







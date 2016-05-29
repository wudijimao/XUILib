//
//  XWindow.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/29.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XWindow.hpp"
#include "GLRender.hpp"
#include "../core/UIViewController.hpp"
#include <chrono>

XWindow::XWindow() {
	setPositon(XResource::XDisplayPoint(0., 0.));
	setSize(XResource::XDisplaySize(375.0, 625.0));
}

XWindow::~XWindow() {
}

void XWindow::update() {
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	/*std::stringstream st;
	st << "time:" << now_ms.time_since_epoch().count() - tempLast << std::endl;
	std::string str;
	st >> str;*/
	//OutputDebugStringA(str.c_str());
	//tempLast = now_ms.time_since_epoch().count();
	_rootController->update(now_ms.time_since_epoch().count() - mLastTimeMs);
	mLastTimeMs = now_ms.time_since_epoch().count();
    if(mNeedReDraw) {
        mNeedReDraw = false;
        _canvas->clear();
        _canvas->makeCurrent();
		_rootController->draw();
        _canvas->Present();
        _canvas->popCurrent();
    }
}

void XWindow::input(std::shared_ptr<XTouch> touch) {
    _touchList.push_back(touch);
}
void XWindow::input(std::shared_ptr<XMouse> mouseEvent) {
    _mouseEventList.push_back(mouseEvent);
}
void XWindow::dispatchInput() {
	dispatchTouchs();
	dispatchMouseEvents();
}

void XWindow::dispatchTouchs() {
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
	_touchList.clear();
}
void XWindow::dispatchMouseEvents() {
	std::map<XUI::UIView *, std::vector<std::shared_ptr<XMouse>>> touchsMap;
	std::map<XUI::UIView *, std::shared_ptr<XUI::UIView>> viewPtrMap;
	for (auto touch : _mouseEventList) {
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
		viewPtrMap[iter.first]->onMouseEvent(iter.second);
		_rootController->onMouseEvent(iter.second);
	}
	_mouseEventList.clear();
}

void XWindow::initFinished() {
    mIsFulllyInited = true;
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	mLastTimeMs = now_ms.time_since_epoch().count();
    if (_rootController != nullptr) {
        _rootController->view();
    }
}

const XResource::XDisplaySize& XWindow::size() {
    return _rect.size();
}

const XResource::XDisplayPoint& XWindow::position() {
    return _rect.point();
}


void XWindow::setSize(const XResource::XDisplaySize &size) {
	_rect.Width(size.Width());
	_rect.Height(size.Height());
    mLocalRect.setSize(_rect.size());
	if (_rootController)
	{
		_rootController->onWindowSizeChange(size);
	}
}
void XWindow::setPositon(const XResource::XDisplayPoint &pos) {
	_rect.X(pos.X());
	_rect.Y(pos.Y());
}

void XWindow::setRootViewController(std::shared_ptr<XUI::UIViewController> rootViewController) {
    rootViewController->mBelongWindow = this;
    _rootController = rootViewController;
    if (mIsFulllyInited) {
		_rootController->onWindowSizeChange(mLocalRect.size());
        _rootController->view();
    }
}







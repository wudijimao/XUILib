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

void XWindow::input(const std::shared_ptr<XTouch> &touch) {
    switch (touch->phase) {
        case TouchPhase::Began: {
            auto view = _rootController->view()->getResponseSubView(touch);
            if (view != nullptr) {
                touch->_belongView = view.get();
                _touchsMap[view.get()];
                auto iter = _touchsMap.find(view.get());
                iter->second.push_back(touch);
            }
            _lastTouchList.push_back(touch);
        }
            break;
        case TouchPhase::Moved: {
            std::vector<std::shared_ptr<XTouch>>::iterator iter;
            if (findFitTouch(touch, iter)) {
                touch->_belongView = (*iter)->_belongView;
                *iter = touch;
                if (touch->_belongView != nullptr) {
                    _touchsMap[touch->_belongView];
                    auto iter = _touchsMap.find(touch->_belongView);
                    iter->second.push_back(touch);
                } else {
                    //std::string("吭爹啊");
                }
                
            }
        }
            
            break;
    }
    _touchList.push_back(touch);
}

bool XWindow::findFitTouch(const std::shared_ptr<XTouch> &touch, std::vector<std::shared_ptr<XTouch>>::iterator& out_iter) {
    auto iter = _lastTouchList.begin();
    auto end = _lastTouchList.end();
    bool ret = false;
    double distance = 4000000.0;
    while (iter != end) {
        double temp =(*iter)->mPosition.quickDistance(touch->mPosition);
        if (temp < distance) {
            distance = temp;
            out_iter = iter;
            ret = true;
        }
        ++iter;
    }
    return ret;
}

void XWindow::input(const std::shared_ptr<XMouse> &mouseEvent) {
    _mouseEventList.push_back(mouseEvent);
}
void XWindow::dispatchInput() {
	dispatchTouchs();
	dispatchMouseEvents();
}

void XWindow::dispatchTouchs() {
//	std::map<XUI::UIView *, std::vector<std::shared_ptr<XTouch>>> touchsMap;
//	std::map<XUI::UIView *, std::shared_ptr<XUI::UIView>> viewPtrMap;
//	for (auto touch : _touchList) {
//		auto view = _rootController->view()->getResponseSubView(touch);
//		if (view != nullptr) {
//			touch->_belongView = view.get();
//			touchsMap[view.get()];
//			auto iter = touchsMap.find(view.get());
//			iter->second.push_back(touch);
//			viewPtrMap[view.get()] = view;
//		}
//	}
//	for (auto iter : touchsMap) {
//		viewPtrMap[iter.first]->onTouch(iter.second);
//		_rootController->onTouch(iter.second);
//	}
    for (auto touchs : _touchsMap) {
        if(touchs.second.size() > 0) {
            touchs.first->onTouch(touchs.second);
        }
    }
    _touchsMap.clear();
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
    _rootController->onWindowSizeChange(mLocalRect.size());
    if (mIsFulllyInited) {
        _rootController->view();
    }
}







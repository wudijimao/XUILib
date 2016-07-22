//
//  XApp.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XApp.hpp"
#include "UIViewController.hpp"
#include "MutiThread/XDispatch.h"

namespace XDUILib
{
	XApp *XApp::_thisApp = nullptr;

	XApp& XApp::thisApp() {
		return *_thisApp;
	}

	XApp::~XApp() {

	}

	void XApp::setMainWindow(std::shared_ptr<IXWindow> window) {
		_mainWindow = window;
	}

	bool XApp::init() {
		setMainWindow(IXWindow::createWindow());
		auto rootController = std::make_shared<XUI::UIViewController>();
		mainWindow()->setRootViewController(rootController);
		return true;
	}
	void XApp::onStatusChanged(XAppStatus status, XAppStatus lastStatus) {

	}
    void XApp::internalInit() {
        XDispatch::XThreadPool::initGlobelPool();
    }

}
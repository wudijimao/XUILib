//
//  XWindow.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "stdafx.hpp"
#include "../core/IXWindow.hpp"


class SIMPLEDIRECTUI_API_DEBUG XWindow : public IXWindow {
public:
    ~XWindow();
    virtual void setRootViewController(std::shared_ptr<XUI::UIViewController> rootViewController);
	virtual void setSize(const XResource::XDisplaySize &size) override;
	virtual void setPositon(const XResource::XDisplayPoint &pos) override;
public:
	//do not call it by libray user
	void update();
    void input(std::shared_ptr<XTouch> touch);
	void input(std::shared_ptr<XMouse> mouseEvents);
	void dispatchInput();
	void dispatchTouchs();
	void dispatchMouseEvents();
protected:
    XWindow();
    std::shared_ptr<XUI::UIViewController> _rootController;
    friend class XUI::UIView;
private:
    XResource::XRect _rect;
    XResource::XRect mLocalRect;//x=0,y=0
	std::vector<std::shared_ptr<XTouch>> _touchList;
	std::vector<std::shared_ptr<XMouse>> _mouseEventList;
};
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
    std::shared_ptr<IXCanvas> _canvas;
    std::shared_ptr<IXRender> _render;
    virtual void setRootViewController(std::shared_ptr<XUI::UIViewController> rootViewController);
	virtual void setSize(const XResource::XSize &size) override;
	virtual void setPositon(const XResource::XPoint &pos) override;
protected:
	//do not call it by libray user
	void update();
	void input(XTouch *touchs, unsigned int count);
	void input(XMouse *mouseEvents, unsigned int count);
	void dispatchInput();
	void dispatchTouchs();
	void dispatchMouseEvents();

    XWindow();
    std::shared_ptr<XUI::UIViewController> _rootController;
    friend class XUI::UIView;
    XResource::XRect _rect;
private:
	std::vector<std::shared_ptr<XTouch>> _touchList;
	std::vector<std::shared_ptr<XMouse>> _mouseEventList;
};
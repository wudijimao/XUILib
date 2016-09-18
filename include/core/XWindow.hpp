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
    virtual const XResource::XDisplaySize &size() override;
    virtual const XResource::XDisplayPoint &position() override;
	virtual void setSize(const XResource::XDisplaySize &size) override;
	virtual void setPositon(const XResource::XDisplayPoint &pos) override;
	const XResource::XRect& rect() {
		return _rect;
	}
public:
	//do not call it by libray user
	void update();
    void input(const std::shared_ptr<XTouch> &touch);
	void input(const std::shared_ptr<XMouse> &mouseEvents);
	void dispatchInput();
	void dispatchTouchs();
	void dispatchMouseEvents();
	void insertText(const char *text);
protected:
    bool mIsFulllyInited = false;
    void initFinished();
    XWindow();
    std::shared_ptr<XUI::UIViewController> _rootController;
    friend class XUI::UIView;
private:
    bool findFitTouch(const std::shared_ptr<XTouch> &touch, std::vector<std::shared_ptr<XTouch>>::iterator &out_iter);
    
	long long mLastTimeMs;
    XResource::XRect _rect;
    XResource::XRect mLocalRect;//x=0,y=0
	std::vector<std::shared_ptr<XTouch>> _touchList;
    std::map<XUI::UIView *, std::vector<std::shared_ptr<XTouch>>> _touchsMap;
    
    std::vector<std::shared_ptr<XTouch>> _lastTouchList;
	std::vector<std::shared_ptr<XMouse>> _mouseEventList;
};
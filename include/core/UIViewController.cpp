//
//  UIViewController.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/29.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "UIViewController.hpp"

namespace XUI
{

    const std::shared_ptr<UIView> UIViewController::view() {
        if (!_isLoaded) {
            LoadView();
        }
        return _view;
    }
    //override
    void UIViewController::viewDidLoad() {
        
    }
    void UIViewController::onWindowSizeChange(const XResource::XDisplaySize &size) {
        mFixRect.setSize(size);
        //XResource::XRectPro rect = _view->getRect();
        //rect.Width(size.Width());
        //rect.Height(size.Height());
        //_view->setRect(rect);
    }
    
    void UIViewController::LoadView() {
        _isLoaded = true;
        _view = std::make_shared<XUI::UIView>();
        _view->mBelongingViewController = this;
        XResource::XRectPro rect;
        rect.setSize(mBelongWindow->size());
        rect.HAlign(XResource::XRectPro::HAlign_Stretch);
        rect.VAlign(XResource::XRectPro::VAlign_Stretch);
        _view->setRect(rect);
        
        viewDidLoad();
    }
    
    void UIViewController::presentViewControler(std::shared_ptr<UIViewController> controller, PresentAnimation ani) {
        this->mBelongWindow->setRootViewController(controller);
    }
    
    void UIViewController::update(unsigned long passedMS) {
        for (auto ani : mAnimations)
        {
            if(ani->state() == AnimatingStates::Playing)
                ani->process(passedMS);
        }
        if (mIsNeedLayout)
        {
            view()->layout(mFixRect);
        }
    }
    void UIViewController::draw() {
        view()->draw();
    }
    
    Animation& UIViewController::addAnimation(const std::shared_ptr<Animation> &ani) {
        mAnimations.push_back(ani);
        return *ani;
    }
    bool UIViewController::removeAnimation(const Animation *ani) {
        return false;
    }
    void UIViewController::setNeedRedraw() {
        if (mBelongWindow != nullptr) {
            mBelongWindow->setNeedReDraw();
        }
    }
    void UIViewController::setNeedLayout() {
        mIsNeedLayout = true;
    }

}




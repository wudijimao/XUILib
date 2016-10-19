//
//  UIViewController.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/29.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "UIViewController.hpp"
#include "Animation/Animation.h"



namespace XUI
{
    std::shared_ptr<XView> UIViewController::view() {
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
        _view = std::make_shared<XUI::XView>();
        _view->mBelongingViewController = this;
        XResource::XRectPro rect;
        rect.setSize(mBelongWindow->size());
        rect.HAlign(XResource::XRectPro::HAlign_Stretch);
        rect.VAlign(XResource::XRectPro::VAlign_Stretch);
        _view->setRect(rect);
        
        viewDidLoad();
    }
    
    void UIViewController::presentViewController(std::shared_ptr<UIViewController> controller,
                                                 PresentAnimation ani) {
        switch (ani) {
            case PresentAnimation::Present: {
                mPresentingViewController = controller;
                this->mBelongWindow->setPresentingViewController(controller);
                auto ani = ValueAnimation<float>::createAni(0.0f, 800.0f, [this](float val) {
                    XResource::XRectPro rect = mPresentingViewController->view()->getRect();
                    rect.Y(800 - val);
                    mPresentingViewController->view()->setRect(rect);
                });
                ani->setDurationMS(500);
                controller->addAnimation(ani);
                ani->play();
            }
                break;
            case PresentAnimation::None:
            default:
                this->mBelongWindow->setRootViewController(controller);
                break;
        }
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




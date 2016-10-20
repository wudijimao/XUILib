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
    UIViewController::~UIViewController() {
        for (auto ani : mAnimations) {
            ani->stop();
        }
        mAnimations.clear();
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

    void UIViewController::onKeyEvent(const std::vector<std::shared_ptr<XKeyInput>> &keyEvent) {
        auto event = *keyEvent.begin();
        if (event->eventType == KeyEventType::Down) {
            if (event->eventButton == KeyEventButton::BackForward) {
                dismiss(DismissAnimation::Dismiss);
            }
        }
    }
    
    void UIViewController::presentViewController(const std::shared_ptr<UIViewController> &controller,
                                                 PresentAnimation ani) {
        mChildViewController = controller.get();
        controller->mParentViewController = this;
        switch (ani) {
            case PresentAnimation::Present: {
                mPresentingViewController = controller;
                mBelongWindow->perparToShow(controller.get());
                //this->mBelongWindow->setPresentingViewController(controller);
                auto ani = ValueAnimation<float>::createAni(0.0f, 625.0f, [this](float val) {
                    XResource::XRectPro rect = mPresentingViewController->view()->getRect();
                    rect.Y(625 - val);
                    mPresentingViewController->view()->setRect(rect);
                });
                ani->setDurationMS(700);
                auto interpolator = std::make_shared<TestInterpolator>();
                ani->setAnimationInterpolator(interpolator);
                controller->addAnimation(ani);
                ani->onFinish = [this](){
                    getNavigationManager()->push(mPresentingViewController);
                    mPresentingViewController->becomFirstResponder(true);//temp  FirstResponder save in VC  All showing VC will recive input firstly
                    mPresentingViewController.reset();
                };
                ani->play();
            }
                break;
            case PresentAnimation::None:
            default:
                getNavigationManager()->push(controller);
                break;
        }
    }

    void UIViewController::dismiss(DismissAnimation ani) {
        if (mIsDismising || !mParentViewController || !mBelongWindow) {
            return;
        }
        switch (ani) {
            case DismissAnimation::Dismiss: {
                mIsDismising = true;
                auto ani = ValueAnimation<float>::createAni(0.0f, 625.0f, [this](float val) {
                    XResource::XRectPro rect = this->view()->getRect();
                    rect.Y(val);
                    this->view()->setRect(rect);
                });
                ani->setDurationMS(700);
                auto interpolator = std::make_shared<TestInterpolator>();
                ani->setAnimationInterpolator(interpolator);
                addAnimation(ani);
                ani->onFinish = [this](){
                    dismiss();
                };
                ani->play();
            }
                break;
            case DismissAnimation::None:
            default:
                if (!mChildViewController) {
                    getNavigationManager()->pop();
                } else {
                    getNavigationManager()->pop(this);
                }
                mParentViewController->mChildViewController = nullptr;
                mParentViewController->becomFirstResponder(true);
                mBelongWindow->setNeedReDraw();
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
            mIsNeedLayout = false;
            view()->layout(mFixRect);
        }
        if (mIsDismising) {
            mParentViewController->update(passedMS);
        } else if (mPresentingViewController) {
            mPresentingViewController->update(passedMS);
        }
    }
    void UIViewController::draw() {
        if (mIsDismising) {
            mParentViewController->draw();
        }
        view()->draw();
        if (mPresentingViewController) {
            mPresentingViewController->draw();
        }
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


    const UIViewController* UIViewController::getChildViewController() {
        return mChildViewController;
    }
    const UIViewController* UIViewController::getParentViewController() {
        return mParentViewController;
    }
    const std::shared_ptr<XNavigationManager>& UIViewController::getNavigationManager() {
        return mBelongWindow->getNavigationManager();
    }

    void UIViewController::setChildViewController(UIViewController *vc) {
        mChildViewController = vc;
    }
    void UIViewController::setParentViewController(UIViewController *vc) {
        mParentViewController = vc;
    }

}




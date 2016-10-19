//
//  UIViewController.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/29.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "UIView.hpp"

namespace XUI {
    
    enum class PresentAnimation {
        None,
        Present,
        Custom,
    };
    
    class SIMPLEDIRECTUI_API UIViewController : public UIResponder {
    public:
        friend class ::XWindow;
        //override
        virtual void viewDidLoad();
        //do not override
        std::shared_ptr<XView> view();
        void setNeedRedraw();
        void setNeedLayout();
        //void setCustomPresentAnimation(PresentAnimation ani = PresentAnimation::Custom);
        void presentViewControler(std::shared_ptr<UIViewController> controller, PresentAnimation ani = PresentAnimation::None);
        Animation& addAnimation(const std::shared_ptr<Animation> &ani);
        bool removeAnimation(const Animation *ani);
    protected:
        std::vector<std::shared_ptr<Animation>> mAnimations;
        void update(unsigned long passedMS);
        void draw();
        void onWindowSizeChange(const XResource::XDisplaySize &size);
        IXWindow *mBelongWindow = nullptr;
        XResource::XRect mFixRect;
    private:
        bool mIsNeedLayout = true;
        void LoadView();
        bool _isLoaded = false;
        std::shared_ptr<XView> _view;
    private:
        std::shared_ptr<UIViewController> mPresentingViewController = nullptr;
    };
}



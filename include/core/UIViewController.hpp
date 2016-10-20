//
//  UIViewController.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/29.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "UIView.hpp"
#include "XNavigationManager.hpp"

namespace XUI {
    
    enum class PresentAnimation {
        None,
        Present,
        Push,
        Custom,
    };
    enum class DismissAnimation {
        None,
        Dismiss,
        Pop,
        Custom,
    };
    
    class SIMPLEDIRECTUI_API UIViewController : public UIResponder {
    public:
        friend class ::XWindow;
        friend class XUI::XNavigationManager;
        virtual ~UIViewController();
        //override
        virtual void viewDidLoad();
        //do not override
        std::shared_ptr<XView> view();
        void setNeedRedraw();
        void setNeedLayout();
        //void setCustomPresentAnimation(PresentAnimation ani = PresentAnimation::Custom);
        void presentViewController(const std::shared_ptr<UIViewController> &controller,
                                   PresentAnimation ani = PresentAnimation::None);
        void dismiss(DismissAnimation ani = DismissAnimation::None);
        Animation& addAnimation(const std::shared_ptr<Animation> &ani);
        bool removeAnimation(const Animation *ani);
        const UIViewController* getChildViewController();
        const UIViewController* getParentViewController();
        const std::shared_ptr<XNavigationManager>& getNavigationManager();
        virtual void onKeyEvent(const std::vector<std::shared_ptr<XKeyInput>> &keyEvent) override ;
    protected:
        //don't directly call it in ViewController
        void setChildViewController(UIViewController*);
        void setParentViewController(UIViewController*);
        std::vector<std::shared_ptr<Animation>> mAnimations;
        void update(unsigned long passedMS);
        void draw();
        void onWindowSizeChange(const XResource::XDisplaySize &size);
        IXWindow *mBelongWindow = nullptr;
        XResource::XRect mFixRect;
    private:
        UIViewController* mChildViewController;
        UIViewController* mParentViewController;
        bool mIsNeedLayout = true;
        void LoadView();
        bool _isLoaded = false;
        std::shared_ptr<XView> _view;
    private:
        std::shared_ptr<UIViewController> mPresentingViewController = nullptr;
        bool mIsDismising;
    };
}



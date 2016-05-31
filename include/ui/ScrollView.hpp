//
//  ScrollView.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/UIView.hpp"

namespace XUI
{
    class ScrollView : public UIView {
    public:
        ScrollView();
        void setContentView(const std::shared_ptr<UIView> &contentView) {
            if(mContentView.get() != contentView.get()) {
                if(mContentView) {
                    removeSubView(mContentView.get());
                }
                mContentView = contentView;
                addSubView(mContentView);
            }
            
        }
        std::shared_ptr<UIView> &contentView() {
            return mContentView;
        }
        
        virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) override;
    private:
        void movePos(double x, double y);
        
        std::shared_ptr<UIView> mContentView;
        XResource::XDisplayPoint mTouchPos;
    };
    
}
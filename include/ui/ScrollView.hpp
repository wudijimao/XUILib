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
    class SIMPLEDIRECTUI_API ScrollView : public XView {
    public:
        ScrollView();
        void setContentView(const std::shared_ptr<XView> &contentView) {
            if(mContentView.get() != contentView.get()) {
                if(mContentView) {
                    removeSubView(mContentView.get());
                }
                mContentView = contentView;
                addSubView(mContentView);
            }
            
        }
        std::shared_ptr<XView> &contentView() {
            return mContentView;
        }
        
        virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) override;
    private:
        void moveTo(double x, double y);
        void moveTo(double x, double y, bool ani);
        void decelerateMove();
        double mVelocityY = 0;
        std::shared_ptr<XView> mContentView;
        XResource::XDisplayPoint mTouchPos;
    };
    
}
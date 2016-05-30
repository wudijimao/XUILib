//
//  XScrollView.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/UIView.hpp"

namespace XUI
{
    class XScrollView : public UIView {
    public:
        void setContentView(const std::shared_ptr<UIView> &contentView) {
            if(mContentView.get() != contentView.get()) {
                if(mContentView) {
                    
                }
                mContentView = contentView;
            }
            
        }
        std::shared_ptr<UIView> &contentView() {
            return mContentView;
        }
    private:
        std::shared_ptr<UIView> mContentView;
    };
    
}
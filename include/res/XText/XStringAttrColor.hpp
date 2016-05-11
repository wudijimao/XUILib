//
//  XStringAttrColor.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/11.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "XStringAttr.hpp"
#include "../XColor.hpp"

namespace XResource {
    class SIMPLEDIRECTUI_API XStringAttrColor : public XStringAttr {
    public:
        static std::shared_ptr<XStringAttrColor> colorWithColor(const std::shared_ptr<XUIColor> color) {
            auto c = std::make_shared<XStringAttrColor>();
            c->mColor = color;
            return c;
        }
        static std::shared_ptr<XStringAttrColor> colorWithColor(const std::shared_ptr<XUIColor> color, const std::shared_ptr<XUIColor> backColor) {
            auto c = std::make_shared<XStringAttrColor>();
            c->mColor = color;
            c->mBackgroundColor = backColor;
            return c;
        }
        
        static std::shared_ptr<XStringAttrColor> colorWithBackgroundColor(const std::shared_ptr<XUIColor> color) {
            auto c = std::make_shared<XStringAttrColor>();
            c->mBackgroundColor = color;
            return c;
        }
        
        std::shared_ptr<XUIColor> mColor;
        std::shared_ptr<XUIColor> mBackgroundColor;
        virtual XAttrStrKeyEnum getKey() override {
            return XAttrStrKey_Color;
        }
    };
}


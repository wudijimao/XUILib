//
//  XTextView.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/9.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XTextView.hpp"

namespace  XUI {
    
    UITextView::UITextView() {
        mFont = XResource::XFont::systemFont(16.0);
        mTextColor = XResource::XStringAttrColor::colorWithColor(XResource::XUIColor::black());
    }
    
    void UITextView::setFont(const std::shared_ptr<XResource::XFont> &font) {
        if (font) {
            mFont = font;
            if (mIsSetBySetText) {
                mText->clearAttrs(XResource::XAttrStrKey_Font);
                mText->addAttr(mFont);
                setNeedReDraw();
            }
        }
    }
    
    const std::shared_ptr<XResource::XFont> &UITextView::font() {
        return mFont;
    }
    
    void UITextView::setTextColor(const std::shared_ptr<XResource::XUIColor> &color) {
        if(color) {
            mTextColor->mColor = color;
            setNeedReDraw();
        }
    }
    
    const std::shared_ptr<XResource::XUIColor> &UITextView::textColor() {
        return mTextColor->mColor;
    }
    
    void UITextView::setText(const XResource::XString &str) {
        mIsSetBySetText = true;
        auto attrStr = XResource::XAttributedString::attrStr(str);
        attrStr->addAttr(mTextColor);
        attrStr->addAttr(mFont);
        mText = attrStr;
        setNeedReDraw();
    }
    
    void UITextView::setText(const XResource::XAttributedStringPtr &attrStr) {
        mIsSetBySetText = false;
        if (attrStr) {
            mText = attrStr;
            setNeedReDraw();
        }
    }
    
    void UITextView::drawRect(IXRender &render) {
        if (mText) {
            XResource::XRect rect = getFixRect();
            render.DrawString(*mText.get(), rect);
        }
    }
}

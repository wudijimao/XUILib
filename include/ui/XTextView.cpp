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
    
//    void UITextView::sizeToFit() {
//        auto rect = getRect();
//        auto frame = mText->createFrame(rect.size());
//        rect.setSize(frame->mSize);
//        setRect(rect);
//    }
    
    void UITextView::judgeRect(XResource::XRect &in_out_rect, const XResource::XDisplaySize &size) {
        switch (mAlignmentH) {
            case UITextAlignmentH::Center:
                in_out_rect.moveX((in_out_rect.Width() - size.Width()) / 2.0);
                break;
            case UITextAlignmentH::Right:
                in_out_rect.moveX(in_out_rect.Width() - size.Width());
                break;
            default:
                break;
        }
        switch (mAlignmentV) {
            case UITextAlignmentV::Center:
                in_out_rect.moveY((in_out_rect.Height() - size.Height()) / 2.0);
                break;
            case UITextAlignmentV::Bottom:
                in_out_rect.moveY(in_out_rect.Height() - size.Height());
                break;
            default:
                break;
        }
        in_out_rect.setSize(size);//for test
    }
    
    XResource::XDisplaySize UITextView::sizeThatFit(const XResource::XDisplaySize &size) {
        auto frame = mText->createFrame(size);
        return frame->mSize;
    }
    
    void UITextView::drawRect(IXRender &render) {
        if (mText) {
            XResource::XRect rect = getFixRect();
            auto frame = mText->createFrame(rect.size());
            judgeRect(rect, frame->mSize);
            
            render.DrawString(*mText.get(), rect);
        }
    }
    
    void UITextView::setAlignmentH(UITextAlignmentH align) {
        if (mAlignmentH != align) {
            mAlignmentH = align;
            setNeedReDraw();
        }
    }
    void UITextView::setAlignmentV(UITextAlignmentV align) {
        if (mAlignmentV != align) {
            mAlignmentV = align;
            setNeedReDraw();
        }
    }
    UITextAlignmentH UITextView::alignmentH() {
        return mAlignmentH;
    }
    UITextAlignmentV UITextView::alignmentV() {
        return mAlignmentV;
    }
}

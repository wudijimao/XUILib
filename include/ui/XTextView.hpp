//
//  XTextView.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/9.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "../core/UIView.hpp"
#include "../res/XResource.hpp"

namespace XUI
{
    enum class UITextAlignmentH {
        Left,
        Center,    // Visually centered
        Right ,    // Visually right aligned
        Justified,    // Fully-justified. The last line in a paragraph is natural-aligned.
        Natural,
    };
    
    enum class UITextAlignmentV {
        Top,
        Center,
        Bottom,
    };
    
    class SIMPLEDIRECTUI_API UITextView : public XView {
    public:
        UITextView();
        void setFont(const std::shared_ptr<XResource::XFont> &font);
        const std::shared_ptr<XResource::XFont> &font();
        void setTextColor(const std::shared_ptr<XResource::XUIColor> &color);
        const std::shared_ptr<XResource::XUIColor> &textColor();
        void setText(const XResource::XString &str);
        void setText(const XResource::XAttributedStringPtr &attrStr);
        const XResource::XAttributedStringPtr &text() const;
        void setAlignmentH(UITextAlignmentH align);
        void setAlignmentV(UITextAlignmentV align);
        UITextAlignmentH alignmentH();
        UITextAlignmentV alignmentV();
        //void sizeToFit();
        XResource::XDisplaySize sizeThatFit(const XResource::XDisplaySize &size);
    protected:
        virtual void drawRect(IXRender &render) override;
        void judgeRect(XResource::XRect &in_out_rect, const XResource::XDisplaySize &size);
        XResource::XAttributedStringPtr mText = nullptr;
    private:
        UITextAlignmentH mAlignmentH = UITextAlignmentH::Left;
        UITextAlignmentV mAlignmentV = UITextAlignmentV::Top;
        std::shared_ptr<XResource::XFont> mFont;
        std::shared_ptr<XResource::XStringAttrColor> mTextColor;
        bool mIsSetBySetText = false;
        void updateTextAttr();
    };
}

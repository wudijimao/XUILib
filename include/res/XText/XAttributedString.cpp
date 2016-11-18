//
//  XAttributedString.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/11.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XAttributedString.hpp"
#include "XFreeType.hpp"

namespace XResource {
    
    void XAttributedString::clearFrameCache() {
        mFrameCache.reset();
    }

    void XAttributedString::fillTextColor(XCoreTextFrame &frame) const {
        unsigned long location = 0;
        XResource::XRange effactRange;
        effactRange.location = 0;
        unsigned long size = mUnicodeCacheStr.length();
        std::shared_ptr<XResource::XStringAttrColor> textColor;
        for (auto line : frame.mLines) {
            for (auto group : line->mGroups) {
                for (auto c : group->mChars) {
                    if (location >= effactRange.rightPosition()) {
                        textColor = std::dynamic_pointer_cast<XResource::XStringAttrColor>(getAttr(location, XResource::XAttrStrKey_Color, effactRange));
                        if (!textColor) {
                            textColor = XStringAttrColor::colorWithColor(XResource::XUIColor::blackColor());
                        }
                    }
                    c->mFrontColor = textColor->mColor;
                    ++location;
                }
            }
        }
    }
    
    std::shared_ptr<XCoreTextFrame> XAttributedString::createFrame(const XResource::XDisplaySize &mSize) const {
        //clear size when Size is too small
        if (mFrameCache) {
            return mFrameCache;
        }
        std::shared_ptr<XCoreTextFrame> frame = std::make_shared<XCoreTextFrame>();
        auto line = new XCoreTextLine();
        frame->mLines.push_back(line);
        auto group = new XCoreTextGroup();
        line->mGroups.push_back(group);
        int x = 0;
        int y = 0;
        int right = mSize.Width();
        unsigned long size = mUnicodeCacheStr.length();
        
        unsigned long location = 0;
        XResource::XRange effactRange;
        auto defaultFont = XFont::systemFont(20);
        double lineMaxAssender = 0.0;
        double lineMaxVertAdvance = 0.0;
        double lineHeight = 3;
        double frameMaxWith = 0;
        while (location < size) {
            auto font = std::dynamic_pointer_cast<XFont>(getAttr(location, XResource::XAttrStrKey_Font, effactRange));
            if (effactRange.length <= 0) {
                break;
            }
            if (!font) {
                font = defaultFont;
            }
            XGlyphPtr *glyphs = XFreeType::sharedInstance()->getXGlyphs(font.get(), mUnicodeCacheStr.c_str() + location, effactRange.length);
            lineMaxVertAdvance = (std::max)(lineMaxVertAdvance, glyphs[0]->mFontMetrics->max_advance);
            for (int i = 0; i < effactRange.length; ++i) {
                auto textChar = new XCoreTextChar();
                auto temp = glyphs[i];
                textChar->mGlyph = glyphs[i];
                textChar->mRect.setSize(textChar->mGlyph->mImage->size());
                //lineMaxVertAdvance = (std::max)(lineMaxVertAdvance, textChar->mRect.Height());
                //lineMaxAssender = (std::max)(lineMaxAssender, textChar->mGlyph->mMetrics.horiBearingY);
                lineMaxAssender = (std::max)(lineMaxAssender,textChar->mGlyph->mFontMetrics->ascender);
                if (x + textChar->mGlyph->mImageLeft + textChar->mRect.Width() - right > 1.0) {
                    for (auto g : line->mGroups) {
                        for (auto c : g->mChars) {
                            c->mRect.moveY(lineMaxAssender);
                        }
                    }
                    line->mRect.Width(x);
                    line->mRect.Height(lineMaxVertAdvance);
                    frameMaxWith = (std::max)(frameMaxWith, line->mRect.Width());
                    x = 0;
                    y += lineMaxVertAdvance;
                    y += lineHeight;
                    lineMaxVertAdvance = textChar->mGlyph->mFontMetrics->max_advance;
                    //lineMaxAssender = textChar->mGlyph->mMetrics.horiBearingY;
                    lineMaxAssender = textChar->mGlyph->mFontMetrics->ascender;
                    line = new XCoreTextLine();
                    line->mRect.X(x);
                    line->mRect.Y(y);
                    frame->mLines.push_back(line);
                    group = new XCoreTextGroup();
                    line->mGroups.push_back(group);
                }
                textChar->mRect.X(x + textChar->mGlyph->mMetrics.horiBearingX);
                textChar->mRect.Y(y - textChar->mGlyph->mMetrics.horiBearingY);
                
                group->mChars.push_back(textChar);
                x += textChar->mGlyph->mMetrics.horiAdvance;
            }
            location += effactRange.length;
        }
        for (auto g : line->mGroups) {
            for (auto c : g->mChars) {
                c->mRect.moveY(lineMaxAssender);
            }
        }
        y += lineMaxVertAdvance;
        line->mRect.Width(x);
        line->mRect.Height(lineMaxVertAdvance);
        frameMaxWith = (std::max)(frameMaxWith, line->mRect.Width());
        frame->mSize.Width(frameMaxWith);
        frame->mSize.Height(y);
        fillTextColor(*frame);
        (std::shared_ptr<XCoreTextFrame>)mFrameCache = frame;
        return frame;
    }
    void XAttributedString::addAttr(const XStrAttrPtr &attr) {
        addAttr(attr, XRange(0, mUnicodeCacheStr.length()));
    }
    void XAttributedString::addAttr(const XStrAttrPtr &attr, const XRange &range) {
        this->mTypedAttrs[attr->getKey()].push_back(XStrAttrContainer(attr, range));
        clearFrameCache();
    }
    void XAttributedString::addAttrs(const XStrAttrVec &attr, unsigned long location, unsigned long length) {
        clearFrameCache();
    }
    XStrAttrVec XAttributedString::getAttrs(unsigned long in_loc, XRange &out_effactRange) const {
        XStrAttrVec vec;
        out_effactRange.length = 0;
        return vec;
    }
    const XStrAttrPtr& XAttributedString::getAttr(unsigned long in_loc, XAttrStrKeyEnum type, XRange &out_effactRange) const {
        auto mapIter = mTypedAttrs.find(type);
        if (mapIter != mTypedAttrs.end()) {
            auto rIter = mapIter->second.rbegin();
            auto rBegin = rIter;
            auto rEnd = mapIter->second.rend();
            int i = 0;
            while (rIter != rEnd) {
                if (rIter->range.location <= in_loc && rIter->range.rightPosition() > in_loc) {
                    out_effactRange.location = rIter->range.location;
                    out_effactRange.length = rIter->range.length;
                    //for nest color attr: e.g.:red (1,1) + blue(0,3) => 0:blue/1:red/2:blue (maybe have bugs
                    //TODO::recode attr when addAttr()
                    auto tempIter = rIter;
                    while (i > 0) {
                        --tempIter;
                        if (tempIter->range.location >= out_effactRange.location && tempIter->range.rightPosition() <= out_effactRange.rightPosition()) {
                            if (in_loc < tempIter->range.location) {
                                out_effactRange.length = tempIter->range.location - out_effactRange.location;
                            } else {
                                unsigned long loc = tempIter->range.rightPosition();
                                out_effactRange.length -= (loc - out_effactRange.location);
                                out_effactRange.location = loc;
                            }
                        }
                        --i;
                    }
                    return rIter->attr;
                }
                ++i;
                ++rIter;
            }

            out_effactRange.location = in_loc;
            out_effactRange.length = mUnicodeCacheStr.length();
            auto tempIter = rIter;
            while (i > 0) {
                --tempIter;
                if (tempIter->range.location >= out_effactRange.location && tempIter->range.rightPosition() <= out_effactRange.rightPosition()) {
                    if (in_loc < tempIter->range.location) {
                        out_effactRange.length = tempIter->range.location - out_effactRange.location;
                    } else {
                        unsigned long loc = tempIter->range.rightPosition();
                        out_effactRange.length -= (loc - out_effactRange.location);
                        out_effactRange.location = loc;
                    }
                }
                --i;
            }
            return mEmptyAttr;
        }
        
        out_effactRange.location = in_loc;
        out_effactRange.length = mUnicodeCacheStr.length();
        return mEmptyAttr;
    }
    
    void XAttributedString::clearAttrs() {
        for (auto pair : mTypedAttrs) {
            pair.second.clear();
        }
        clearFrameCache();
    }
    
    void XAttributedString::clearAttrs(XAttrStrKeyEnum type) {
        mTypedAttrs[type].clear();
        clearFrameCache();
    }
}
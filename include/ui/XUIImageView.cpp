//
//  XUIImageView.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/12.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XUIImageView.hpp"

namespace XUI
{
    XUIImageView::XUIImageView() {
        setClipsToBounds(true);
    }
    
    void XUIImageView::setScaleType(XUIImageScaleType type) {
        if (mScaleType != type) {
            mScaleType = type;
            if (mDisplayingImage) {
                setNeedReDraw();
            }
        }
    }
    
    XUIImageScaleType XUIImageView::scaleType() {
        return mScaleType;
    }
    
    void XUIImageView::setHighlighted(bool highlighted) {
        if (mIsHighlighted != highlighted) {
            mIsHighlighted = highlighted;
            _selectDisplayImage();
        }
    }
    
    bool XUIImageView::isHighLighted() {
        return mIsHighlighted;
    }
    
    void XUIImageView::setImage(const std::shared_ptr<XResource::IXImage> &img) {
        mImage = img;
        _selectDisplayImage();
    }
    
    void XUIImageView::setHighlightedImage(const std::shared_ptr<XResource::IXImage> &img) {
        mHighlightedImage = img;
        _selectDisplayImage();
    }
    
    const std::shared_ptr<XResource::IXImage>& XUIImageView::image() {
        return mImage;
    }
    
    const std::shared_ptr<XResource::IXImage>& XUIImageView::highlightedImage() {
        return mDisplayingImage;
    }
    
    const std::shared_ptr<XResource::IXImage>& XUIImageView::selectDisplayImage() {
        if (mIsHighlighted) {
            return mHighlightedImage;
        } else {
            return mImage;
        }
    }
    
    void XUIImageView::drawRect(IXRender &render) {
        if (mDisplayingImage) {
            auto rect = getFixRect();
            auto size = mDisplayingImage->size();
            switch (mScaleType) {
                case XUIImageScaleType::Original:
                    rect.moveX((rect.Width() - size.Width()) / 2.0);
                    rect.moveY((rect.Height() - size.Height()) / 2.0);
                    rect.setSize(size);
                    break;
                case XUIImageScaleType::AspectFit: {
                    double scaleX = rect.Width() / size.Width();
                    double scaleY = rect.Height() / size.Height();
                    if (scaleX < scaleY) {
                        double height = scaleX * size.Height();
                        rect.moveY((rect.Height() - height) / 2.0);
                        rect.Height(height);
                    } else {
                        double width = scaleY * size.Width();
                        rect.moveX((rect.Width() - width) / 2.0);
                        rect.Width(width);
                    }
                }
                    break;
                case XUIImageScaleType::AspectFill: {
                    double scaleX = rect.Width() / size.Width();
                    double scaleY = rect.Height() / size.Height();
                    if (scaleX > scaleY) {
                        double height = scaleX * size.Height();
                        rect.moveY((rect.Height() - height) / 2.0);
                        rect.Height(height);
                    } else {
                        double width = scaleY * size.Width();
                        rect.moveX((rect.Width() - width) / 2.0);
                        rect.Width(width);
                    }
                }
                    break;
                case XUIImageScaleType::Fill:
                default:
                    break;
            }
            render.DrawImage(mDisplayingImage, rect);
        }
    }
    
    //protected
    void XUIImageView::setDisplayImage(const std::shared_ptr<XResource::IXImage> &img) {
        if (mDisplayingImage.get() !=  img.get()) {
            mDisplayingImage = img;
            setNeedReDraw();
        }
    }
    
    const std::shared_ptr<XResource::IXImage>& XUIImageView::displayingImage() {
        return mDisplayingImage;
    }
    
    //private
    void XUIImageView::_selectDisplayImage() {
        setDisplayImage(selectDisplayImage());
    }
    
}






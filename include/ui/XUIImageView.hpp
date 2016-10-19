//
//  XUIImageView.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/12.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "../core/UIView.hpp"

namespace XUI
{
    
    enum class XUIImageScaleType {
        Original, //not scale
        Fill,
        AspectFill, // same x,y scale to fill
        AspectFit,  // same x,y scale to fit
    };
    
    class SIMPLEDIRECTUI_API XUIImageView : public XView
    {
    public:
        XUIImageView();
        void setScaleType(XUIImageScaleType type);
        XUIImageScaleType scaleType();
        void setHighlighted(bool highlighted);
        bool isHighLighted();
        void setImage(const std::shared_ptr<XResource::IXImage> &img);
        void setHighlightedImage(const std::shared_ptr<XResource::IXImage> &img);
        const std::shared_ptr<XResource::IXImage>& image();
        const std::shared_ptr<XResource::IXImage>& highlightedImage();
        virtual void drawRect(IXRender &render);
    protected:
        const std::shared_ptr<XResource::IXImage>& selectDisplayImage();
        void setDisplayImage(const std::shared_ptr<XResource::IXImage> &img);
        const std::shared_ptr<XResource::IXImage>& displayingImage();
    private:
        void _selectDisplayImage();
        
        std::shared_ptr<XResource::IXImage> mImage;
        std::shared_ptr<XResource::IXImage> mHighlightedImage;
        
        std::shared_ptr<XResource::IXImage> mDisplayingImage;
        
        XUIImageScaleType mScaleType = XUIImageScaleType::AspectFit;
        bool mIsHighlighted = false;
    };
    
}
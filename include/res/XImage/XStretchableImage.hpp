//
//  XStretchableImage.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/16.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once
#include "XImage.hpp"

namespace XResource {
    class SIMPLEDIRECTUI_API XStretchableImage {
    public:
        friend XImage;
        std::shared_ptr<XImage> &image() {
            return mImage;
        }
        XEdge& stretchEdge() {
            return mStretchEdge;
        }
        XStretchableImage(const std::shared_ptr<XImage> &img, const XEdge& stretchEdge);
        XStretchableImage(const std::shared_ptr<XImage> &img, double left, double top);
    private:
        std::shared_ptr<XImage> mImage;
        XEdge mStretchEdge;
    };
}
//
//  XStretchableImage.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/16.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XStretchableImage.hpp"


namespace XResource {
    
    XStretchableImage::XStretchableImage(const std::shared_ptr<XImage> &img, const XEdge& stretchEdge) {
        mStretchEdge = stretchEdge;
        mImage = img;
    }
    
    XStretchableImage::XStretchableImage(const std::shared_ptr<XImage> &img, double left, double top) {
        mImage = img;
        mStretchEdge.left(left);
        mStretchEdge.top(top);
        mStretchEdge.right(left);
        mStretchEdge.bottom(top);
    }
    
}
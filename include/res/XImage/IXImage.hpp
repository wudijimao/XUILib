//
//  IXImage.hpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#define NULL_ABLE
#define NONE_NULL
#include "../../core/stdafx.hpp"
#include "../XData.hpp"
namespace XResource {
    
    class SIMPLEDIRECTUI_API IXImage {
    public:
        virtual int width() = 0;
        virtual int height() = 0;
        virtual int count() = 0;
        virtual bool getImage(void *outBuf, int index) = 0;
    };
    
}
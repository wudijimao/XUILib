//
//  XImageDataDecoder.hpp
//  XDUILib
//
//  Created by ximiao on 16/4/29.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "XImageDecoder.hpp"
#include <png.h>

namespace XResource
{
    
    class XImageDataDecoder : public XImageDecoder {
        friend XImageDecoder;
    private:
    protected:
        virtual ~XImageDataDecoder();
        static NULL_ABLE XImageDecoder *getInstance();
        virtual NULL_ABLE XImageDecoder *fork();
        bool isThisFormart(const std::shared_ptr<XData> &data) {
            return false;
        }
        bool initWithData(const std::shared_ptr<XData> &data) {
            return false;
        }
        int width();
        int height();
        int count();
        bool getImage(void *outBuf, int index);
    };
    
}
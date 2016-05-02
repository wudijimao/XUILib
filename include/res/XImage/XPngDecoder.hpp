//
//  XPngDecoder.hpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include "XImageDecoder.hpp"
#include <png.h>

namespace XResource
{

class XPngDecoder : public XImageDecoder {
    friend XImageDecoder;
private:
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
protected:
    virtual ~XPngDecoder();
    static NULL_ABLE XImageDecoder *getInstance();
    virtual NULL_ABLE XImageDecoder *fork();
    bool isThisFormart(const std::shared_ptr<XData> &data);
    bool initWithData(const std::shared_ptr<XData> &data);
    int width();
    int height();
    int count();
    bool getImage(void *outBuf, int index);
};


}
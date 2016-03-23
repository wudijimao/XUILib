//
//  XGifDecoder.hpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include "gif_lib.h"
#include "XImageDecoder.hpp"

namespace XResource {
    class XGifDecoder : public XImageDecoder {
        friend XImageDecoder;
    protected:
        //if .lib not exsist return null
        static NULL_ABLE XImageDecoder *getInstance();
        virtual NULL_ABLE XImageDecoder *fork();
        bool isThisFormart(XData *data);
        bool initWithData(XData *data);
        int width();
        int height();
        int count();
        bool getImage(void *outBuf, int index);
    private:
        GifFileType *mGifFile_cPtr;
    };
}


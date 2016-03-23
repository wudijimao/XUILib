//
//  XImage.cpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XImage.hpp"
#include "XJPGDecoder.hpp"

namespace XResource {
    
    
    std::vector<XImageDecoder*>* XImage::decoders = 0;
    
    XImage::XImage (const char *filePath) {
        if (decoders == 0) {
            decoders = new std::vector<XImageDecoder*>();
            XImageDecoder::getDecoders(*decoders);
        }
        XData *data = new XData();
        data->open(filePath);
        for(auto decoder : *decoders) {
            if (decoder->isThisFormart(data)) {
                mDecoder = decoder->fork();
                mDecoder->initWithData(data);
                break;
            }
        }
    }
    XImage::~XImage() {
        if (mDecoder != nullptr) {
            delete mDecoder;
            mDecoder = nullptr;
        }
    }
    int XImage::width() {
        if (mDecoder) {
            return mDecoder->width();
        }
        return 0;
    }
    int XImage::height() {
        if (mDecoder) {
            return mDecoder->height();
        }
        return 0;
    }
    int XImage::count() {
        if (mDecoder) {
            return mDecoder->count();
        }
        return 0;
    }
    bool XImage::getImage(void *outBuf, int index) {
        if (mDecoder) {
            return mDecoder->getImage(outBuf, index);
        }
        return false;
    }
    
    
}



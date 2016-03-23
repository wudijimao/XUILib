//
//  XDecoder.cpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XImageDecoder.hpp"
#include "XGifDecoder.hpp"
#include "XPngDecoder.hpp"
#include "XJPGDecoder.hpp"

namespace XResource {
    
    bool XImageDecoder::initWithData(XData *data) {
        mData = data;
        return true;
    }
    XImageDecoder::~XImageDecoder() {
        if (mData != nullptr) {
            delete mData;
            mData = nullptr;
        }
    }
    
    size_t XImageDecoder::getDecoders(std::vector<XImageDecoder*> &decoders) {
        XImageDecoder *decoder;
        decoder = XGifDecoder::getInstance();
        if (decoder) {
            decoders.push_back(decoder);
        }
        decoder = XPngDecoder::getInstance();
        if (decoder) {
            decoders.push_back(decoder);
        }
        decoder = XJPGDecoder::getInstance();
        if (decoder) {
            decoders.push_back(decoder);
        }
        return decoders.size();
    }
    
    
}
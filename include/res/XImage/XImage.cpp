//
//  XImage.cpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XImage.hpp"
#include "../XData.hpp"
#include "XJPGDecoder.hpp"
#include "../../core/XResManager.hpp"

namespace XResource {
    
    
    std::vector<XImageDecoder*>* XImage::decoders = 0;
    
    XImage::XImage (const char *filePath) {
        auto data = XData::dataForContentOfFile(filePath);
		initWithData(data);
    }
    XImage::XImage(std::shared_ptr<XData> &data) {
		initWithData(data);
	}
	bool XImage::initWithData(std::shared_ptr<XData> &data) {
        if (decoders == nullptr) {
            decoders = new std::vector<XImageDecoder*>();
            XImageDecoder::getDecoders(*decoders);
        }
        bool ret = false;
		for (auto decoder : *decoders) {
			if (decoder->isThisFormart(data)) {
				mDecoder = decoder->fork();
				mDecoder->initWithData(data);
                ret = true;
				break;
			}
		}
        return ret;
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
	std::shared_ptr<XImage> XImage::imageNamed(const char *name) {
		auto path = XBundle::mainBundle()->pathForResource(name);
		return std::make_shared<XImage>(path.UTF8CStr());
	}
    
}



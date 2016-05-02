//
//  XDecoder.hpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include <vector>
#include "IXImage.hpp"
#include "../XData.hpp"

namespace XResource {
    
    class XImage;
    
    class XImageDecoder : protected IXImage {
        friend XImage;
    protected:
        virtual NULL_ABLE XImageDecoder *fork() = 0;
        virtual bool isThisFormart(const std::shared_ptr<XData> &data) = 0;
        virtual bool initWithData(const std::shared_ptr<XData> &data);
        virtual ~XImageDecoder();
        
        static size_t getDecoders(std::vector<XImageDecoder*> &decoders);
		virtual XImagePixelFormat pixelFormat() override {
			return mPixelFormart;
		}
	protected:
		XImagePixelFormat mPixelFormart = XImagePixelFormat::RGBA32;
    protected:
        std::shared_ptr<XData> mData;
    };
    
}
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
#include "../XRect.hpp"

namespace XResource {
    
    enum class XImagePixelFormat {
		UnKnown,
        RGBA32,
		RGB24,
        Gray,
    };
    
    class SIMPLEDIRECTUI_API IXImage {
    public:
        virtual ~IXImage() {}
        const XResource::XDisplaySize& size() {
            if(mDisplaySize.Width() < 0.000001) {
                mDisplaySize.Width(width() / XResource::gHighResolutionPixelScale);
                mDisplaySize.Height(height() / XResource::gHighResolutionPixelScale);
            }
            return mDisplaySize;
        }
        virtual int width() = 0;
        virtual int height() = 0;
        virtual int count() = 0;
        virtual bool getImage(void *outBuf, int index) = 0;
		virtual bool saveToFile(const char *fileName);
		virtual std::shared_ptr<XData> getPixelsData(int index) {
			if (mPixelsData.get() == nullptr) {
				unsigned long size;
				switch (pixelFormat())
				{
				case XResource::XImagePixelFormat::RGBA32:
					size = width() * height() * 4;
					break;
				case XResource::XImagePixelFormat::RGB24:
					size = width() * height() * 3;
					break;
				case XResource::XImagePixelFormat::Gray:
					size = width() * height();
					break;
				default:
					return nullptr;
				}
				mPixelsData = XData::data();
				mPixelsData->resize(size);
				void *buf = mPixelsData->detachBuf();
				getImage(buf, index);
				mPixelsData->attachBuf(buf, size);
			}
			return mPixelsData;
		}
		virtual XImagePixelFormat pixelFormat() = 0;
	protected:
	private:
        XResource::XDisplaySize mDisplaySize;
		std::shared_ptr<XData> mPixelsData;
    };
    
}
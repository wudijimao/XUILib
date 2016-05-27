//
//  XImage.hpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include <stdio.h>
#include "../../core/stdafx.hpp"
#include <vector>
#include "IXImage.hpp"
#include "XImageDecoder.hpp"

namespace XResource {
    class XStretchableImage;
    
    class SIMPLEDIRECTUI_API XImage : public IXImage {
    public:
		static std::shared_ptr<XImage> imageNamed(const char *name);
        static std::shared_ptr<XImage> imageFromData(std::shared_ptr<XData> &data);
        static std::shared_ptr<XImage> imageFromFile(const char *filePath);
        XImageDecoder *mDecoder = nullptr;
        ~XImage();
        bool initWithData(const std::shared_ptr<XData> &data);
        virtual int width() override;
        virtual int height() override;
        virtual int count() override;
        virtual bool getImage(void *outBuf, int index) override;
		virtual XImagePixelFormat pixelFormat() override {
			if (mDecoder != nullptr) {
				return mDecoder->pixelFormat();
			}
			return XImagePixelFormat::UnKnown;
		}
        std::shared_ptr<XStretchableImage> createStretchableImage(const XEdge& stretchEdge);
        std::shared_ptr<XStretchableImage> createStretchableImage(double left, double top);
    protected:
        XImage () {
        }
    private:
        std::weak_ptr<XImage> mSelf;
		friend IXImage;
        static std::vector<XImageDecoder*> *decoders;
    };

	class SIMPLEDIRECTUI_API XPixelImage : public IXImage {
	public:
		virtual int width() override {
			return mWidth;
		}
		virtual int height() override {
			return mHeight;
		}
		virtual int count() override {
			return 1;
		}
		virtual bool getImage(void *outBuf, int index) override {
			if(mData) {
				memcpy(outBuf, mData->getBuf(), mData->size());
				return true;
			}
			return false;
		}
		virtual std::shared_ptr<XData> getPixelsData(int index) override {
			return mData;
		}
		XPixelImage(const std::shared_ptr<XData> &pixelsData, int width, int height, XImagePixelFormat format) {
			mPixelFormart = format;
			mWidth = width;
			mHeight = height;
			mData = pixelsData;
		}
		virtual XImagePixelFormat pixelFormat() override {
			return mPixelFormart;
		}
	private:
		XImagePixelFormat mPixelFormart = XImagePixelFormat::RGBA32;
		int mWidth;
		int mHeight;
		std::shared_ptr<XData> mData;
	};
}


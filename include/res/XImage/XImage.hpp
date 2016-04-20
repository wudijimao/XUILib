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
    class SIMPLEDIRECTUI_API XImage : public IXImage {
    public:
		static std::shared_ptr<XImage> imageNamed(const char *name);
        XImageDecoder *mDecoder = nullptr;
        XImage () {
        }
        XImage (const char *filePath);
		XImage(std::shared_ptr<XData> &data);
        ~XImage();
        bool initWithData(std::shared_ptr<XData> &data);
        virtual int width() override;
        virtual int height() override;
        virtual int count() override;
        virtual bool getImage(void *outBuf, int index) override;
    private:
        static std::vector<XImageDecoder*> *decoders;
    };
}


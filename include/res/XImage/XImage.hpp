//
//  XImage.hpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include <stdio.h>
#include "../XData.hpp"
#include <vector>
#include "IXImage.hpp"
#include "XImageDecoder.hpp"

namespace XResource {
    class XImage : public IXImage {
    public:
        XImageDecoder *mDecoder = nullptr;
        XImage () {
        }
        XImage (const char *filePath);
        ~XImage();
        virtual int width() override;
        virtual int height() override;
        virtual int count() override;
        virtual bool getImage(void *outBuf, int index) override;
    private:
        static std::vector<XImageDecoder*> *decoders;
    };
}


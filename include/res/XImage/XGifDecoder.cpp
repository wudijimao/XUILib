//
//  XGifDecoder.cpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XGifDecoder.hpp"
#include "gif_lib.h"

namespace XResource {
    
    struct RGBACOLOR {
        char r,g,b,a;
    };
    
    static int memReadFuncGif(GifFileType* GifFile, GifByteType* buf, int count)
    {
        char* ptr = (char*)(GifFile->UserData);
        memcpy(buf, ptr, count);
        GifFile->UserData = ptr + count;
        return count;
    }
    
    
    //if .lib not exsist return null
    NULL_ABLE XImageDecoder * XGifDecoder::getInstance() {
        XImageDecoder *decoder = new XGifDecoder();
        return decoder;
    }
    NULL_ABLE XImageDecoder * XGifDecoder::fork() {
        return new XGifDecoder(*this);
    }
    bool XGifDecoder::isThisFormart(XData *data) {
        char *buf = data->getBuf(0, 6);
        static std::string key1 = "GIF89a";
        static std::string key2 = "GIF87a";
        if (memcmp(buf, key1.c_str(), key1.length()) == 0 ||
            memcmp(buf, key2.c_str(), key2.length()) == 0) {
            return true;
        }
        return false;
    }
    bool XGifDecoder::initWithData(XData *data) {
        this->XImageDecoder::initWithData(data);
        int err;
        char *buf = data->getBuf(0, data->size());
        mGifFile_cPtr = DGifOpen(buf, memReadFuncGif, &err);
        if (mGifFile_cPtr == NULL) {
            return false;
        }
        return true;
    }
    int XGifDecoder::width() {
        if (mGifFile_cPtr) {
            return mGifFile_cPtr->SWidth;
        }
        return 0;
    }
    int XGifDecoder::height() {
        if (mGifFile_cPtr) {
            return mGifFile_cPtr->SHeight;
        }
        return 0;
    }
    int XGifDecoder::count() {
        if (mGifFile_cPtr) {
            return mGifFile_cPtr->ImageCount;
        }
        return 0;
    }
    
    bool XGifDecoder::getImage(void *outBuf, int index) {
        RGBACOLOR *temp = (RGBACOLOR*)outBuf;
        const GifColorType *bgColor = &(mGifFile_cPtr->SColorMap->Colors[mGifFile_cPtr->SBackGroundColor]);
        RGBACOLOR rgbaBGColor;
        rgbaBGColor.r = bgColor->Red;
        rgbaBGColor.g = bgColor->Green;
        rgbaBGColor.b = bgColor->Blue;
        rgbaBGColor.a = 255;
        for (int i = 0; i < mGifFile_cPtr->SWidth * mGifFile_cPtr->SHeight; ++i) {
            memcpy(temp++, &rgbaBGColor, sizeof(GifColorType));
            
        }
        
        DGifSlurp(mGifFile_cPtr);
        SavedImage *image = &(mGifFile_cPtr->SavedImages[index]);
        unsigned char *src = image->RasterBits;
        char *dst = (char*)outBuf;
        GifColorType *ColorMapEntry;
        ColorMapObject *ColorMap;
        //SavedImage *saveImg;
        ColorMap = (image->ImageDesc.ColorMap ? image->ImageDesc.ColorMap : mGifFile_cPtr->SColorMap);
        dst += image->ImageDesc.Top * mGifFile_cPtr->SWidth * 4;
        for (int i = image->ImageDesc.Top; i < (image->ImageDesc.Top + image->ImageDesc.Height); i++)
        {
            dst += image->ImageDesc.Left * 4;
            for (int j = image->ImageDesc.Left; j < (image->ImageDesc.Left + image->ImageDesc.Width); j++)
            {
                ColorMapEntry = &(ColorMap->Colors[*(src++)]);
                *dst++ = ColorMapEntry->Red;
                *dst++ = ColorMapEntry->Green;
                *dst++ = ColorMapEntry->Blue;
                *dst++ = 255;
            }
            dst += (mGifFile_cPtr->SWidth - image->ImageDesc.Left - image->ImageDesc.Width) * 4;
        }
        return true;
    }
    
}

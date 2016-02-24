//
//  XImage.hpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#ifndef XImage_hpp
#define XImage_hpp

#include <stdio.h>
#include "XData.hpp"
#include "gif_lib.h"
#include "../png/png.h"
#include <vector>

#define NULL_ABLE
#define NONE_NULL

struct RGBACOLOR {
    char r,g,b,a;
};

class IXImage {
public:
    virtual int width() = 0;
    virtual int height() = 0;
    virtual int count() = 0;
    virtual bool getImage(void *outBuf, int index) = 0;
};

class XImageDecoder : public IXImage {
public:
    virtual NULL_ABLE XImageDecoder *fork() = 0;
    virtual bool isThisFormart(XData *data) = 0;
    virtual bool initWithData(XData *data) {
        mData = data;
        return true;
    }
    static size_t getDecoders(std::vector<XImageDecoder*> &decoders);
    virtual ~XImageDecoder() {
        if (mData != nullptr) {
            delete mData;
            mData = nullptr;
        }
    }
protected:
    XData *mData = nullptr;
};

class XImage : public IXImage {
public:
    XImageDecoder *mDecoder;
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


static int memReadFuncGif(GifFileType* GifFile, GifByteType* buf, int count)
{
    char* ptr = (char*)(GifFile->UserData);
    memcpy(buf, ptr, count);
    GifFile->UserData = ptr + count;
    return count;
}


class XGifDecoder : public XImageDecoder {
public:
    //if .lib not exsist return null
    static NULL_ABLE XImageDecoder *getInstance() {
        XImageDecoder *decoder = new XGifDecoder();
        return decoder;
    }
    virtual NULL_ABLE XImageDecoder *fork() {
        return new XGifDecoder(*this);
    }
    bool isThisFormart(XData *data) {
        char *buf = data->getBuf(0, 6);
        static std::string key1 = "GIF89a";
        static std::string key2 = "GIF87a";
        if (memcmp(buf, key1.c_str(), key1.length()) == 0 ||
            memcmp(buf, key2.c_str(), key2.length()) == 0) {
            return true;
        }
        return false;
    }
    bool initWithData(XData *data) {
        this->XImageDecoder::initWithData(data);
        int err;
        char *buf = data->getBuf(0, data->size());
        mGifFile_cPtr = DGifOpen(buf, memReadFuncGif, &err);
        if (mGifFile_cPtr == NULL) {
            return false;
        }
        return true;
    }
    int width() {
        if (mGifFile_cPtr) {
            return mGifFile_cPtr->SWidth;
        }
        return 0;
    }
    int height() {
        if (mGifFile_cPtr) {
            return mGifFile_cPtr->SHeight;
        }
        return 0;
    }
    int count() {
        if (mGifFile_cPtr) {
            return mGifFile_cPtr->ImageCount;
        }
        return 0;
    }
    bool getImage(void *outBuf, int index) {
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
private:
    GifFileType *mGifFile_cPtr;
};


static void pngReadCallback(png_structp png_ptr, png_bytep buf, png_size_t length)
{
    XData* data = (XData*)png_get_io_ptr(png_ptr);
    char *ptr = data->getBuf(data->mSeekLocation, length);
    memcpy(buf, ptr, length);
}

class XPngDecoder : public XImageDecoder {
private:
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
public:
    virtual ~XPngDecoder() {
        if (png_ptr)
        {
            png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
        }
        XImageDecoder::~XImageDecoder();
    }
    static NULL_ABLE XImageDecoder *getInstance() {
        XImageDecoder *decoder = new XPngDecoder();
        return decoder;
    }
    virtual NULL_ABLE XImageDecoder *fork() {
        return new XPngDecoder(*this);
    }
    bool isThisFormart(XData *data) {
        png_const_bytep buf = (png_const_bytep)data->getBuf(0, 8);
        bool is_png = !png_sig_cmp(buf, 0, 8);
        return is_png;
    }
    bool initWithData(XData *data) {
        this->XImageDecoder::initWithData(data);
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr)
            return false;
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            png_destroy_read_struct(&png_ptr, (png_infopp)nullptr, (png_infopp)nullptr);
            return false;
        }
        data->mSeekLocation = 0;
        png_set_read_fn(png_ptr, data, pngReadCallback);
        png_read_info(png_ptr, info_ptr);
        
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);
        
        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png_ptr);
        }
        // low-bit-depth grayscale images are to be expanded to 8 bits
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        {
            bit_depth = 8;
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        // expand any tRNS chunk data into a full alpha channel
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png_ptr);
        }
        // reduce images with 16-bit samples to 8 bits
        if (bit_depth == 16)
        {
            png_set_strip_16(png_ptr);
        }
        
        // Expanded earlier for grayscale, now take care of palette and rgb
        if (bit_depth < 8)
        {
            png_set_packing(png_ptr);
        }
        // update info
        png_read_update_info(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        
        switch (color_type)
        {
            case PNG_COLOR_TYPE_GRAY:
                //_renderFormat = Texture2D::PixelFormat::I8;
                break;
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                //_renderFormat = Texture2D::PixelFormat::AI88;
                break;
            case PNG_COLOR_TYPE_RGB:
                //_renderFormat = Texture2D::PixelFormat::RGB888;
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                //_renderFormat = Texture2D::PixelFormat::RGBA8888;
                break;
            default:
                break;
        }
        
        // read png data
//        png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);
//        png_read_rows(png_ptr, row_pointers, nullptr, this->height());
//        png_read_image(png_ptr, row_pointers);
//        png_read_end(png_ptr, nullptr);
        return true;
    }
    int width() {
        return png_get_image_width(png_ptr, info_ptr);
    }
    int height() {
        return png_get_image_height(png_ptr, info_ptr);
    }
    int count() {
        return 0;
    }
    bool getImage(void *outBuf, int index) {
        png_size_t rowbytes;
        png_bytep* row_pointers = new png_bytep[this->height()];
        
        rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
        //unsigned long dataSize = rowbytes * this->height() * sizeof(char);
        unsigned char *data;
        //data = static_cast<char*>(malloc(dataSize));
        data = (unsigned char *)outBuf;
        if (!data)
        {
            if (row_pointers != nullptr)
            {
                delete [] row_pointers;
            }
            return false;
        }
        
        for (unsigned short i = 0; i < this->height(); ++i)
        {
            row_pointers[i] = data + i * rowbytes;
        }
        png_read_image(png_ptr, row_pointers);
        png_read_end(png_ptr, nullptr);
        return false;
    }
};


#endif /* XImage_hpp */

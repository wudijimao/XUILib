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
#include <png.h>


namespace XResource {
    
	bool IXImage::saveToFile(const char *fileName) {
        auto path = XSandBox::sharedSandBox().documentDict()->fileInfoFor(fileName)->getPath();
        
		auto data = getPixelsData(0);
		const char *pixels = (const char*)data->getBuf();

		png_byte color_type;

		png_structp png_ptr;
		png_infop info_ptr;
		/* create file */
		FILE *fp = fopen(path.UTF8CStr(), "wb");
		if (!fp)
		{
			return false;
		}


		/* initialize stuff */
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		if (!png_ptr)
		{
			return false;
		}
		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			printf("[write_png_file] png_create_info_struct failed");
			return false;
		}
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			printf("[write_png_file] Error during init_io");
			return false;
		}
		png_init_io(png_ptr, fp);
		/* write header */
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			printf("[write_png_file] Error during writing header");
			return false;
		}
		int width = this->width();
		int height = this->height();
		int rowByteSize;
		int perPixelSize;
		/* 判断要写入至文件的图片数据是否有透明度，来选择色彩类型 */
		switch (pixelFormat())
		{
		case XImagePixelFormat::RGBA32:
			color_type = PNG_COLOR_TYPE_RGB_ALPHA;
			rowByteSize = width * 4;
			perPixelSize = 4;
			break;
		case XImagePixelFormat::RGB24:
			color_type = PNG_COLOR_TYPE_RGB;
			rowByteSize = width * 3;
			perPixelSize = 3;
			break;
		case XImagePixelFormat::Gray:
			color_type = PNG_COLOR_TYPE_GRAY;
			rowByteSize = width;
			perPixelSize = 1;
			break;
		default:
			return false;
		}

		png_set_IHDR(png_ptr, info_ptr, width, height,
			8, color_type, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		png_write_info(png_ptr, info_ptr);


		/* write bytes */
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			printf("[write_png_file] Error during writing bytes");
			return false;
		}
		int pos = 0;
		png_bytep *row_pointers = new png_bytep[height];
		for (int i = 0; i < height; i++)
		{
			//row_pointers[i] = (png_bytep)pixels[i*rowByteSize];
			row_pointers[i] = new png_byte[rowByteSize];
			memcpy(row_pointers[i], &pixels[i*rowByteSize], rowByteSize);
		}

		png_write_image(png_ptr, row_pointers);

		/* end write */
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			printf("[write_png_file] Error during end of write");
			return false;
		}
		png_write_end(png_ptr, NULL);
		for (int i = 0; i < height; i++)
		{
			delete[] row_pointers[i];
		}

		delete[] row_pointers;
		fclose(fp);
		return true;
	}
  
    std::vector<XImageDecoder*>* XImage::decoders = 0;
    
	bool XImage::initWithData(const std::shared_ptr<XData> &data) {
        if (decoders == nullptr) {
            decoders = new std::vector<XImageDecoder*>();
            XImageDecoder::getDecoders(*decoders);
        }
        bool ret = false;
        if(data) {
            for (auto decoder : *decoders) {
                if (decoder->isThisFormart(data)) {
                    mDecoder = decoder->fork();
                    mDecoder->initWithData(data);
                    ret = true;
                    break;
                }
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
    //static
    std::shared_ptr<XImage> XImage::imageFromData(std::shared_ptr<XData> &data) {
        auto img = std::make_shared<XImage>();
        if(img->initWithData(data)) {
            return img;
        } else {
            return nullptr;
        }
    }
    std::shared_ptr<XImage> XImage::imageFromFile(const char *filePath) {
        auto data = XData::dataForContentOfFile(filePath);
        return imageFromData(data);
    }
	std::shared_ptr<XImage> XImage::imageNamed(const char *name) {
		auto path = XBundle::mainBundle()->pathForResource(name);
        return imageFromFile(path.UTF8CStr());
	}
    
}



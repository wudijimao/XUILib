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
		auto data = getPixelsData(0);
		char *pixels = new char[data->size()];
		memcpy(pixels, data->getBuf(), data->size());
			//(const char*)data->getBuf();

		int j, i, pos;
		png_byte color_type;

		png_structp png_ptr;
		png_infop info_ptr;
		png_bytep * row_pointers;
		/* create file */
		FILE *fp = fopen(fileName, "wb");
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
		/* 判断要写入至文件的图片数据是否有透明度，来选择色彩类型 */
		switch (pixelFormat())
		{
		case XImagePixelFormat::RGBA32:
			color_type = PNG_COLOR_TYPE_RGB_ALPHA;
			rowByteSize = width * 4;
			break;
		case XImagePixelFormat::RGB24:
			color_type = PNG_COLOR_TYPE_RGB;
			rowByteSize = width * 3;
			break;
		case XImagePixelFormat::Gray:
			color_type = PNG_COLOR_TYPE_GRAY;
			rowByteSize = width;
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
		//if (graph->flag == HAVE_ALPHA) temp = (4 * graph->width);
		
		row_pointers = new png_bytep[height];
		for (i = 0; i < height; i++)
		{
			row_pointers[i] = (png_bytep)pixels[i*rowByteSize];
		}
		png_write_image(png_ptr, row_pointers);

		/* end write */
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			printf("[write_png_file] Error during end of write");
			return false;
		}
		png_write_end(png_ptr, NULL);
		delete[] row_pointers;
		fclose(fp);
		return true;
	}
  
    std::vector<XImageDecoder*>* XImage::decoders = 0;
    
    XImage::XImage (const char *filePath) {
        auto data = XData::dataForContentOfFile(filePath);
		initWithData(data);
    }
    XImage::XImage(std::shared_ptr<XData> &data) {
		initWithData(data);
	}
	bool XImage::initWithData(const std::shared_ptr<XData> &data) {
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



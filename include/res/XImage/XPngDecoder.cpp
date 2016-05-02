//
//  XPngDecoder.cpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XPngDecoder.hpp"
namespace XResource
{
    
    static void pngReadCallback(png_structp png_ptr, png_bytep buf, png_size_t length)
    {
        XData* data = (XData*)png_get_io_ptr(png_ptr);
        const void *ptr = data->getBuf(data->seekLocation(), length);
        memcpy(buf, ptr, length);
    }
    
    
    XPngDecoder::~XPngDecoder() {
        if (png_ptr)
        {
            png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
        }
    }
    NULL_ABLE XImageDecoder * XPngDecoder::getInstance() {
        XImageDecoder *decoder = new XPngDecoder();
        return decoder;
    }
    NULL_ABLE XImageDecoder * XPngDecoder::fork() {
        return new XPngDecoder(*this);
    }
    bool XPngDecoder::isThisFormart(const std::shared_ptr<XData> &data) {
        png_const_bytep buf = (png_const_bytep)data->getBuf(0, 8);
		if (buf != nullptr)
		{
			bool is_png = !png_sig_cmp(buf, 0, 8);
			return is_png;
		}
        return false;
    }
    bool XPngDecoder::initWithData(const std::shared_ptr<XData> &data) {
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
        data->seek(0);
        png_set_read_fn(png_ptr, data.get(), pngReadCallback);
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
				mPixelFormart = XResource::XImagePixelFormat::Gray;
                break;
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                //_renderFormat = Texture2D::PixelFormat::AI88;
                break;
            case PNG_COLOR_TYPE_RGB:
				mPixelFormart = XResource::XImagePixelFormat::RGB24;
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
				mPixelFormart = XResource::XImagePixelFormat::RGBA32;
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
    int XPngDecoder::width() {
        return png_get_image_width(png_ptr, info_ptr);
    }
    int XPngDecoder::height() {
        return png_get_image_height(png_ptr, info_ptr);
    }
    int XPngDecoder::count() {
        return 1;
    }
    bool XPngDecoder::getImage(void *outBuf, int index) {
        png_size_t rowbytes;
        png_bytep* row_pointers = new png_bytep[this->height()];
        
		

        rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
        //unsigned long dataSize = rowbytes * this->height() * sizeof(char);

		unsigned char *data;// = (unsigned char*)malloc(rowbytes * height());;
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

		/*unsigned char *outData = (unsigned char *)outBuf;
		for (unsigned short i = 0; i < this->height(); ++i)
		{
			for (size_t i = 0; i < this->width(); i++, outData+=4, data+=3)
			{
				outData[0] = data[0];
				outData[1] = data[1];
				outData[2] = data[2];
				outData[3] = 255;
			}
		}*/
        return true;
    }

    
}

//
//  XJPGDecoder.cpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XJPGDecoder.hpp"

namespace XResource {
    XJPGDecoder::~XJPGDecoder() {
        //TODO::这块逻辑没对上号~可能引起释放时崩溃
		//jpeg_finish_decompress(&_cinfo);
		//jpeg_destroy_decompress(&_cinfo);
    }
    NULL_ABLE XImageDecoder* XJPGDecoder::getInstance() {
        XImageDecoder *decoder = new XJPGDecoder();
        return decoder;
    }
    NULL_ABLE XImageDecoder* XJPGDecoder::fork() {
        return new XJPGDecoder(*this);
    }
    bool XJPGDecoder::isThisFormart(const std::shared_ptr<XData> &data) {
        char *buf = (char *)data->getBuf(0, 2);
        if (buf) {
            static const unsigned char JPG_SOI[] = {0xFF, 0xD8};
            return memcmp(buf, JPG_SOI, 2) == 0;
        }
        return false;
    }
    bool XJPGDecoder::initWithData(const std::shared_ptr<XData> &data) {
        this->XImageDecoder::initWithData(data);
        struct jpeg_error_mgr jerr;
        
        _cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&_cinfo);
        jpeg_mem_src(&_cinfo, (unsigned char *)data->getBuf(0, data->size()), (unsigned long)data->size());
        jpeg_read_header(&_cinfo, TRUE);
        jpeg_start_decompress(&_cinfo);
        return true;
    }
    int XJPGDecoder::width() {
        return _cinfo.output_width;
    }
    int XJPGDecoder::height() {
        return _cinfo.output_height;
    }
    int XJPGDecoder::count() {
        return 0;
    }
    bool XJPGDecoder::getImage(void *outBuf, int index) {
        unsigned char *data;
        data = (unsigned char *)outBuf;
        if (data) {
            unsigned int row_stride = _cinfo.output_width * _cinfo.output_components;
            JSAMPARRAY buffer = (*_cinfo.mem->alloc_sarray)((j_common_ptr) &_cinfo, JPOOL_IMAGE, row_stride, 1);
            while (_cinfo.output_scanline < _cinfo.output_height) {
                jpeg_read_scanlines(&_cinfo, buffer, 1);
                for (int i = 0; i < _cinfo.output_width; ++i, data += 4) {
                    memcpy(data, (*buffer) + i * 3, 3);
                    data[3] = 255;
                }
            }
            jpeg_abort_decompress(&_cinfo);
            return true;
        }
        return false;
    }
}

//
//  XData.cpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include <stdio.h>
#include <string>
#include "XData.hpp"

namespace XResource {
    
    std::shared_ptr<XData> XData::dataForContentOfFile(const char *fileName) {
        auto data = std::make_shared<XFileData>();
		if (data->open(fileName)) {
			return data;
		}
		return nullptr;
    }
    
    std::shared_ptr<XData> XData::dataForBuf(char *buf, unsigned long size) {
        auto data = std::make_shared<XData>();
        data->attachBuf(buf, size);
        return data;
    }
    

    unsigned long XData::size() {
        return mSize;
    }
    const void * XData::getBuf(unsigned long location, unsigned long size) {
        mSeekLocation = location + size;
        if(mSeekLocation <= mSize) {
            return mBuf + location;
        }
        return nullptr;
    }
    const void * XData::getBufFrom(unsigned long location){
        return mBuf + location;
    }
    const void * XData::getBuf(unsigned long size){
        if (size <= mSize) {
            return mBuf;
        }
        return nullptr;
    }
	const void * XData::getBuf() {
		return mBuf;
	}
    const char * XData::c_str() {
        resize(size() + 1);
        setDataAt(size(), '\0');
        return mBuf;
    }
    void * XData::detachBuf(){
        char *buf = mBuf;
        mBuf = nullptr;
        return buf;
    }
    void XData::clear() {
        delete [] mBuf;
        mBuf = nullptr;
        mSize = 0;
    }
    
    bool XFileData::open(const char *fileName) {
        clear();
        bool ret = false;
        mFileName = fileName;
        FILE *fp = fopen(fileName, "rb");
        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            long fileSize = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            if (fileSize > 0) {
                mFileSize = fileSize;
                resize(fileSize + 1);
                setDataAt(fileSize, '\0');
                ret = true;
            } else {
                clear();
            }
            fclose(fp);
        }
        return ret;
    }
    unsigned long XFileData::size() {
        return mFileSize;
    }
    const void* XFileData::getBuf(unsigned long location, unsigned long size) {
        const void *ret = XData::getBuf(location, size);
		if (ret == nullptr) {
			FILE *fp = fopen(mFileName.c_str(), "rb");
			if (fp != nullptr) {
				unsigned long bufferedSize = XData::size();
				char *buf = (char*)getBufIntenal() + bufferedSize;
				fseek(fp, bufferedSize, SEEK_SET);
				fread(buf, 1, seekLocation() - bufferedSize, fp);
				setSizeIntenal(seekLocation());
				fclose(fp);
				ret = (char*)getBufIntenal() + location;
			}
        }
        return ret;
    }
    const void* XFileData::getBufFrom(unsigned long location) {
        return getBuf(location, size());
    }
    const void* XFileData::getBuf(unsigned long size) {
        return getBuf(0, size);
    }
	const void * XFileData::getBuf() {
		return getBuf(size());
	}
    void* XFileData::detachBuf() {
        getBuf(0, size());
        return XData::detachBuf();
    }

    void XFileData::clear() {
        XData::clear();
        mFileSize = 0;
        mFileName = "";
    }
    
}



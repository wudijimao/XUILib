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
    
    XData::XData() {
        
    }
    unsigned long XData::size() {
        return mSize;
    }
    
    bool XData::open(const char *fileName) {
        clear();
        bool ret = false;
        mFileName = fileName;
        FILE *fp = fopen(fileName, "rb");
        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            long fileSize = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            if (fileSize > 0) {
                mSize = fileSize;
                mBuf = new char[mSize];
                ret = true;
            } else {
                clear();
            }
            fclose(fp);
        }
        return ret;
    }
    char* XData::getBuf(unsigned long location, unsigned long size) {
        if (mBuf) {
            unsigned long needBufferedSize = mSeekLocation = location + size;
            if (needBufferedSize > mBufferedSize && needBufferedSize <= mSize) {
                FILE *fp = fopen(mFileName.c_str(), "rb");
                fseek(fp, mBufferedSize, SEEK_SET);
                fread(mBuf + mBufferedSize, 1, needBufferedSize - mBufferedSize, fp);
                mBufferedSize = needBufferedSize;
                fclose(fp);
            }
            return mBuf + location;
        }
        return nullptr;
    }
    char* XData::getBufFrom(unsigned long location) {
        return getBuf(location, size());
    }
    char* XData::getBuf(unsigned long size) {
        return getBuf(0, size);
    }
    char* XData::detachBuf() {
        char *buf = getBuf(0, size());
        mBuf = nullptr;
        return buf;
    }
#pragma -mark private function
    
    void XData::clear() {
        delete [] mBuf;
        mBufferedSize = 0;
        mBuf = nullptr;
        mFileName = "";
        mSize = 0;
    }
    
}



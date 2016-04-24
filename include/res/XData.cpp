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
        data->open(fileName);
        return data;
    }
    
    std::shared_ptr<XData> XData::dataForBuf(char *buf, unsigned long size) {
        return std::make_shared<XFixedBufData>(buf, size);
    }
    
    XFixedBufData::XFixedBufData() {
        
    }
    unsigned long XData::size() {
        return mSize;
    }
    char *XFixedBufData::getBuf(unsigned long location, unsigned long size) {
        if(location + size <= mSize) {
            return mBuf + location;
        }
        return nullptr;
    }
    char *XFixedBufData::getBufFrom(unsigned long location){
        return mBuf + location;
    }
    char *XFixedBufData::getBuf(unsigned long size){
        if (size <= mSize) {
            return mBuf;
        }
        return nullptr;
    }
    char *XFixedBufData::detachBuf(){
        char *buf = mBuf;
        mBuf = nullptr;
        return buf;
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
                mSize = fileSize;
                mBuf = new char[mSize + 1];
                mBuf[mSize] = '\0';
                ret = true;
            } else {
                clear();
            }
            fclose(fp);
        }
        return ret;
    }
    char* XFileData::getBuf(unsigned long location, unsigned long size) {
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
    char* XFileData::getBufFrom(unsigned long location) {
        return getBuf(location, size());
    }
    char* XFileData::getBuf(unsigned long size) {
        return getBuf(0, size);
    }
    char* XFileData::detachBuf() {
        char *buf = getBuf(0, size());
        mBuf = nullptr;
        return buf;
    }
#pragma -mark private function
    
    void XFixedBufData::clear() {
        delete [] mBuf;
        mBuf = nullptr;
        mSize = 0;
    }
    void XFileData::clear() {
        XFixedBufData::clear();
        mBufferedSize = 0;
        mFileName = "";
    }
    
}



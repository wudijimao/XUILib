//
//  XData.cpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include <stdio.h>
#include <string>

class XData {
public:
    XData() {
        
    }
    unsigned long mSeekLocation = 0;
    unsigned long size() {
        return mSize;
    }
    std::string mFileName;
    bool open(const char *fileName) {
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
    char *getBuf(unsigned long location, unsigned long size) {
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
private:
    unsigned long mBufferedSize;
    unsigned long mSize = 0;
    char *mBuf = nullptr;
    void clear() {
        delete [] mBuf;
        mBufferedSize = 0;
        mBuf = nullptr;
        mFileName = "";
        mSize = 0;
    }
};
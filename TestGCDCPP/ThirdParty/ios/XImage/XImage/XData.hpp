//
//  XData.h
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include <string>

class XData {
public:
    XData();
    unsigned long mSeekLocation = 0;
    unsigned long size();
    std::string mFileName;
    bool open(const char *fileName);
    char *getBuf(unsigned long location, unsigned long size);
private:
    unsigned long mBufferedSize;
    unsigned long mSize = 0;
    char *mBuf = nullptr;
    void clear();
};


//
//  XData.h
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once
#include "../core/stdafx.hpp"

namespace XResource
{
    
    class SIMPLEDIRECTUI_API XData {
    public:
        XData();
        unsigned long mSeekLocation = 0;
        unsigned long size();
        std::string mFileName;
        bool open(const char *fileName);
        char *getBuf(unsigned long location, unsigned long size);
        char *getBufFrom(unsigned long location);
        char *getBuf(unsigned long size);
        char *detachBuf();
    private:
        unsigned long mBufferedSize;
        unsigned long mSize = 0;
        char *mBuf = nullptr;
        void clear();
    };
    
}
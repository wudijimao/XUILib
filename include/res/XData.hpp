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
        virtual ~XData(){}
        XData();
        XData(char *buf, unsigned long size) {
            mBuf = buf;
            mBufferedSize = size;
            mSize = size;
        }
        unsigned long size();
        virtual char *getBuf(unsigned long location, unsigned long size);
        virtual char *getBufFrom(unsigned long location);
        virtual char *getBuf(unsigned long size);
        virtual char *detachBuf();
        static std::shared_ptr<XData> dataForContentOfFile(const char *fileName);
    protected:
        //virtual bool readBuf(unsigned long size);
        unsigned long mBufferedSize;
        unsigned long mSize = 0;
        char *mBuf = nullptr;
        virtual void clear();
    public:
        unsigned long mSeekLocation = 0;
    };
    
    class XFileData : public XData {
    public:
        bool open(const char *fileName);
        virtual char *getBuf(unsigned long location, unsigned long size) override;
        virtual char *getBufFrom(unsigned long location) override;
        virtual char *getBuf(unsigned long size) override;
        virtual char *detachBuf() override;
    private:
        virtual void clear() override;
        std::string mFileName;
    };
    
}


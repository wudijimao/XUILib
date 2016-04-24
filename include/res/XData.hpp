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
        unsigned long size();
        virtual char *getBuf(unsigned long location, unsigned long size) = 0;
        virtual char *getBufFrom(unsigned long location) = 0;
        virtual char *getBuf(unsigned long size) = 0;
        virtual char *detachBuf() = 0;
        //for small file
        static std::shared_ptr<XData> dataForContentOfFile(const char *fileName);
        static std::shared_ptr<XData> dataForBuf(char *buf, unsigned long size);
    public:
        unsigned long mSeekLocation = 0;
    protected:
        unsigned long mSize = 0;
    };
    
    class SIMPLEDIRECTUI_API XMutableData : XData {
    public:
        virtual void addData(char *, unsigned long size) = 0;
    };
    
    class XFixedBufData : public XData {
    public:
        XFixedBufData();
        XFixedBufData(char *buf, unsigned long size) {
            mBuf = buf;
            mSize = size;
        }
        virtual char *getBuf(unsigned long location, unsigned long size);
        virtual char *getBufFrom(unsigned long location);
        virtual char *getBuf(unsigned long size);
        virtual char *detachBuf();
    protected:
        //virtual bool readBuf(unsigned long size);
        char *mBuf = nullptr;
        virtual void clear();
    };
    
    class XFileData : public XFixedBufData {
    public:
        bool open(const char *fileName);
        virtual char *getBuf(unsigned long location, unsigned long size) override;
        virtual char *getBufFrom(unsigned long location) override;
        virtual char *getBuf(unsigned long size) override;
        virtual char *detachBuf() override;
    private:
        unsigned long mBufferedSize = 0;
        virtual void clear() override;
        std::string mFileName;
    };
    
}


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
		static std::shared_ptr<XData> data() {
			return std::make_shared<XData>();
		}

        typedef std::allocator<char> _Allocator;
        _Allocator mAllocator;
        virtual ~XData(){
            mAllocator.deallocate(mBuf, mBufSize);
        }
        virtual unsigned long size();
        virtual const void *getBuf(unsigned long location, unsigned long size);
        virtual const void *getBufFrom(unsigned long location);
        virtual const void *getBuf(unsigned long size);
		virtual const void *getBuf();
        virtual void *detachBuf();
        virtual const void *attachBuf(void *buf, unsigned long size) {
            mBuf = (char*)buf;
            mSize = size;
            buf = nullptr;
            return mBuf;//这样传进来的buf  用 allocator的dealloc 不知道是否有错误
        }
        void resize(unsigned long size) {
            if(mBufSize < size) {
                resizeInternal(size);
            }
        }
        //appendSizeToUsedBuf
        inline void appendSize(unsigned long size) {
            unsigned long needSize = mSize + size;
            if(needSize > mBufSize) {
                needSize += ((size >= mBufSize)? size : mBufSize);
                resizeInternal(needSize);
            }
        }
        void appendData(const void *buf, unsigned long size) {
            appendSize(size);
            memcpy(mBufTail, buf, size);
            mSize += size;
            mBufTail += size;
        }
        inline void resizeInternal(unsigned long size) {
            char *temp = mAllocator.allocate(size);
            memcpy(temp, mBuf, mSize);
            mAllocator.deallocate(mBuf, mBufSize);
            mBufSize = size;
            mBuf = temp;
            mSeekBuf = mBuf + mSeekLocation;
            mBufTail = mBuf + mSize;
        }
        //for small file
        static std::shared_ptr<XData> dataForContentOfFile(const char *fileName);
        static std::shared_ptr<XData> dataForBuf(char *buf, unsigned long size);
        virtual void clear();
        unsigned long seekLocation() {
            return mSeekLocation;
        }
        void seek(unsigned long location) {
            mSeekLocation = location;
        }
    protected:
        inline void setDataAt(unsigned long location, const char data) {
            mBuf[location] = data;
        }
        inline void *getBufIntenal() {
            return mBuf;
        }
        inline void setSizeIntenal(unsigned long size) {
            mSize = size;
        }
    private:
        unsigned long mSeekLocation = 0;
        char *mSeekBuf = nullptr;
        char *mBufTail = nullptr;
        unsigned long mSize = 0; //used size
        char *mBuf = nullptr;
        unsigned long mBufSize = 0; //buffer length
    };
    
    class XFileData : public XData {
    public:
        bool open(const char *fileName);
        virtual unsigned long size() override;
        virtual const void *getBuf(unsigned long location, unsigned long size) override;
        virtual const void *getBufFrom(unsigned long location) override;
        virtual const void *getBuf(unsigned long size) override;
		virtual const void *getBuf() override;
        virtual void *detachBuf() override;
    private:
        unsigned long mFileSize = 0;
        virtual void clear() override;
        std::string mFileName;
    };
    
}


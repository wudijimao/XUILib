//
//  XString.hpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//
//  Plase use UTF-8 encoded SourceFile(*.h|*.cpp)
#pragma once
#include "../core/stdafx.hpp"
#include "XData.hpp"
namespace XResource
{
    
    enum class XStringEncoding {
        ASCII = 1,  /* 0..127 only */
        UTF8 = 2,
        Unicode = 0x0100,
        
        UTF16 = Unicode,
        UTF16BigEndian = 0x0101,          /* NSUTF16StringEncoding encoding with explicit endianness specified */
        UTF16LittleEndian = 0x0110,       /* NSUTF16StringEncoding encoding with explicit endianness specified */
        
        UTF32 = 0x1000,
        UTF32BigEndian = 0x1001,          /* NSUTF32StringEncoding encoding with explicit endianness specified */
        UTF32LittleEndian = 0x1002,        /* NSUTF32StringEncoding encoding with explicit endianness specified */
        
        //GB18030 = 0x10000, //兼容GBK/GB2312
        Default = UTF8,
    };
    
    //如果使用字符串的地方只支持UTF8 则不使用这个类，直接使用std::string,避免过多的依赖
    class XString {
    public:
        static std::shared_ptr<XString> stringWithContentOfFile(const char *filePath) {
            XData data;
            data.open(filePath);
            return std::make_shared<XString>(data.getBuf(data.size()));
        }
        static std::shared_ptr<XString> stringWithContentOfFile(const char *filePath, XStringEncoding encoding) {
            XData data;
            data.open(filePath);
            return std::make_shared<XString>(data.getBuf(data.size()), encoding);
        }
    public:
        ~XString() {
            if (_utf8Buf) {
                delete _utf8Buf;
            }
        }
		void append(const XString &str) {

		}
        XString();
        XString(const char *str);
        XString(const std::string& string);
        XString(const std::string&& string);
        XString(const char *str, XStringEncoding encoding);
        //divorce buf
        std::string* detachUTF8String();
        bool init(const char *str, XStringEncoding encoding);
        //fastest
        inline const std::string* getUTF8String() {
            return this->_utf8Buf;
        }
        
        std::shared_ptr<XData> encodeData(XStringEncoding encoding);
        //fast func windows utf-16
        std::wstring getUnicodeString();
        void upper();
        void lower();
        //position start from zero
        inline std::string subStr(size_t position, size_t num) const {
            return _utf8Buf->substr(position, num);
        }
    private:
        std::string *_utf8Buf = new std::string();
    };
}

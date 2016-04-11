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
    enum class XStringCompareOptions {
        Default = 0,
        SearchFromBackward = 1,
        IgnorCase = 2,
    };
    
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
        static const long npos = -1;
        static const XString& emptyStr() {
            static XString emptyStr("");
            return emptyStr;
        }
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
		XString(const XString &str) {
			_utf8Buf = new std::string(*str._utf8Buf);
		}
		XString(XString &&str) {
			_utf8Buf = str._utf8Buf;
			str._utf8Buf = new std::string();
		}
        XString& operator=(const XString &str) {
            _utf8Buf = new std::string(*(str._utf8Buf));
            return *this;
        }
        XString& operator=(XString &&str) {
            _utf8Buf = str._utf8Buf;
            str._utf8Buf = nullptr;
            return *this;
        }
        
		XString operator+(const XString&in)
		{
			XString ret = *this;
			ret.append(in);
			return ret;
		}

		XString& append(const XString &str) {
            return append(str.getUTF8String());
		}
        XString& append(const std::string &str) {
            _utf8Buf->append(str);
            return *this;
        }
        XString& append(const char *str) {
            _utf8Buf->append(str);
            return *this;
        }
        long replaceAll(const std::string &strSrc, const std::string &strDes) {
            long count = 0;
            std::string::size_type pos = 0;
            std::string::size_type srcLen = strSrc.size();
            std::string::size_type desLen = strDes.size();
            pos=_utf8Buf->find(strSrc, pos);
            while ((pos != std::string::npos))
            {
                ++count;
                _utf8Buf->replace(pos, srcLen, strDes);
                pos=_utf8Buf->find(strSrc, (pos+desLen));
            }
            return count;
        }
        long find(const XString &str, XStringCompareOptions options = XStringCompareOptions::Default) {
            return find(str.getUTF8String(), options);
        }
        long find(const char *str, XStringCompareOptions options = XStringCompareOptions::Default) {
            return find(std::string(str), options);
        }
        long find(const std::string &str, XStringCompareOptions options = XStringCompareOptions::Default) {
            unsigned long pos = XString::npos;
            if (str.length() > 0) {
                bool needReleaseStr = false;
                std::string *temp = _utf8Buf;
                if (((int)options & (int)XStringCompareOptions::IgnorCase) == (int)XStringCompareOptions::IgnorCase) {
                    XString str(*this);
                    str.lower();
                    temp = str.detachUTF8String();
                    needReleaseStr = true;
                }
                if (((int)options & (int)XStringCompareOptions::SearchFromBackward) == (int)XStringCompareOptions::SearchFromBackward) {
                    pos = temp->rfind(str);
                } else {
                    pos = temp->find(str);
                }
                
                if (needReleaseStr) {
                    delete temp;
                }
            }
            return pos;
        }
        //char length by utf-8
        unsigned long length() {
            return _utf8Buf->length();
        }
        unsigned long charactersLength() {
            //TODO::获取文字长度  判断中文和emoji
            return _utf8Buf->length();
        }
        XString subStr(unsigned long location, unsigned long length) {
            return _utf8Buf->substr(location, length);
        }
        XString& insert(unsigned long location, const XString &str) {
            return insert(location, str.getUTF8String());
        }
        XString& insert(unsigned long location, const std::string &str) {
			_utf8Buf->insert(location, str);
			return *this;
        }
        XString& insert(unsigned long location, const char *str) {
            _utf8Buf->insert(location, str);
            return *this;
        }
        bool isEqualTo(const XString &str) const {
            return isEqualTo(str.getUTF8String());
        }
        bool isEqualTo(const std::string &str) const {
            if (&str == _utf8Buf) {
                return true;
            } else {
                return (str == *_utf8Buf);
            }
        }
        bool isEqualTo(const char *str) const {
            if (str != nullptr) {
                return isEqualTo(std::string(str));
            }
            return false;
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
        inline const std::string& getUTF8String() const {
            return *(this->_utf8Buf);
        }
        inline const char *UTF8CStr() const {
            return this->_utf8Buf->c_str();
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
        std::string *_utf8Buf;
    };
}

//
//  XString.cpp
//  XImage
//
//  Created by ximiao on 16/2/24.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XString.hpp"
#include <locale>
#include <codecvt>
#include <algorithm>

namespace XResource {
    
    const std::string ws2s( const std::wstring& src )
    {
        std::locale sys_locale("");
        
        const wchar_t* data_from = src.c_str();
        const wchar_t* data_from_end = src.c_str() + src.size();
        const wchar_t* data_from_next = 0;
        
        int wchar_size = 4;
        char* data_to = new char[(src.size() + 1) * wchar_size];
        char* data_to_end = data_to + (src.size() + 1) * wchar_size;
        char* data_to_next = 0;
        
        memset( data_to, 0, (src.size() + 1) * wchar_size );
        
        typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
        mbstate_t out_state = {0};
        auto result = std::use_facet<convert_facet>(sys_locale).out(
                                                                    out_state, data_from, data_from_end, data_from_next,
                                                                    data_to, data_to_end, data_to_next );
        if( result == convert_facet::ok )
        {
            std::string dst = data_to;
            delete[] data_to;
            return dst;
        }
        else
        {
            printf( "convert error!\n" );
            delete[] data_to;
            return std::string("");
        }
    }
    
    const std::wstring s2ws( const std::string& src )
    {
        std::locale sys_locale("");
        
        const char* data_from = src.c_str();
        const char* data_from_end = src.c_str() + src.size();
        const char* data_from_next = 0;
        
        wchar_t* data_to = new wchar_t[src.size() + 1];
        wchar_t* data_to_end = data_to + src.size() + 1;
        wchar_t* data_to_next = 0;
        
        wmemset( data_to, 0, src.size() + 1 );
        
        typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
        mbstate_t in_state = {0};
        auto result = std::use_facet<convert_facet>(sys_locale).in(
                                                                   in_state, data_from, data_from_end, data_from_next,
                                                                   data_to, data_to_end, data_to_next );
        if( result == convert_facet::ok )
        {
            std::wstring dst = data_to;
            delete[] data_to;
            return dst;
        }
        else
        {
            printf( "convert error!\n" );
            delete[] data_to;
            return std::wstring(L"");
        }
    }
    //utf16(Unicode) => utf8
    inline const std::string ws2utf8( const std::wstring& src )
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes( src );
    }
    //utf8 => utf16(Unicode)
    inline const std::wstring utf8_2_ws( const std::string& src )
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
        return conv.from_bytes( src );
    }
    
    
    XString::XString(){}
    XString::XString(const char *str) {
		if (str != nullptr)
		{
			_utf8Buf->assign(str);
		}
    }
    XString::XString(const std::string& string) {
        _utf8Buf->assign(string.c_str());
    }
    XString::XString(const std::string&& string) {
        _utf8Buf = new std::string(string);
    }
    XString::XString(const char *str, XStringEncoding encoding) {
        init(str, encoding);
    }
    //divorce buf
    std::string* XString::detachUTF8String() {
        std::string *temp = _utf8Buf;
        _utf8Buf = new std::string();
        return temp;
    }
    bool XString::init(const char *str, XStringEncoding encoding) {
		if (str == nullptr)
		{
			return false;
		}
        bool ret = false;
        switch (encoding) {
            case XStringEncoding::UTF8:
            case XStringEncoding::ASCII:
                _utf8Buf->assign(str);
                ret = true;
                break;
            case XStringEncoding::UTF16:
                ret = true;
                try {
                    _utf8Buf = new std::string(ws2utf8((wchar_t*)str));
                } catch (std::range_error err) {
                    ret = false;
                }
                break;
            default:
                break;
        }
        return ret;
    }
    
    std::shared_ptr<XData> XString::encodeData(XStringEncoding encoding) {
        std::shared_ptr<XData> data;
        return data;
    }
    //fast func windows utf-16
    std::wstring XString::getUnicodeString() {
        return utf8_2_ws(*_utf8Buf);
    }
    void XString::upper() {
        std::transform(_utf8Buf->begin(), _utf8Buf->end(), _utf8Buf->begin(), ::toupper);
    }
    void XString::lower() {
        std::transform(_utf8Buf->begin(), _utf8Buf->end(), _utf8Buf->begin(), ::tolower);
    }
    
}




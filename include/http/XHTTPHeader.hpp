//
//  XHTTPHeader.hpp
//  XDUILib
//
//  Created by ximiao on 16/4/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/stdafx.hpp"
#include <assert.h>

class SIMPLEDIRECTUI_API XUrl {
private:
	static unsigned char ToHex(unsigned char x)
	{
		return  x > 9 ? x + 55 : x + 48;
	}

	static unsigned char FromHex(unsigned char x)
	{
		unsigned char y;
		if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
		else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
		else if (x >= '0' && x <= '9') y = x - '0';
		else assert(0);
		return y;
	}
public:
	static std::string UrlEncode(const std::string& str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (isalnum((unsigned char)str[i]) ||
				(str[i] == '-') ||
				(str[i] == '_') ||
				(str[i] == '.') ||
				(str[i] == '~'))
				strTemp += str[i];
			else if (str[i] == ' ')
				strTemp += "+";
			else
			{
				strTemp += '%';
				strTemp += ToHex((unsigned char)str[i] >> 4);
				strTemp += ToHex((unsigned char)str[i] % 16);
			}
		}
		return strTemp;
	}

	static std::string UrlDecode(const std::string& str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (str[i] == '+') strTemp += ' ';
			else if (str[i] == '%')
			{
				assert(i + 2 < length);
				unsigned char high = FromHex((unsigned char)str[++i]);
				unsigned char low = FromHex((unsigned char)str[++i]);
				strTemp += high * 16 + low;
			}
			else strTemp += str[i];
		}
		return strTemp;
	}
};

//only support utf-8
class SIMPLEDIRECTUI_API XHTTPHeader {
public:
    static const std::string cUserAgentKey;
    std::map<std::string, std::string> _headers;
    void setHeader(const char *key, const char *val) {
        if (val == nullptr) {
            auto iter = _headers.find(key);
            if (iter != _headers.end()) {
                _headers.erase(iter);
            }
        }
        _headers[key] = val;
    }
    const char* getHeader(const char *key) {
        auto iter = _headers.find(key);
        if (iter != _headers.end()) {
            return iter->second.c_str();
        }
        return nullptr;
    }
	const char* getStr() {
		mStr.clear();
		for (auto header : _headers)
		{
			mStr.append(header.first);
			mStr.append(":");
			mStr.append(XUrl::UrlEncode(header.second));
			mStr.append("\r\n");
		}
		return mStr.c_str();
	}
	bool setStr(const char *c) {
		if (c != nullptr) {
			int state = -1;
			char *iter = (char*)c;
			int size = 0;
			std::string headerKey;
			std::string headerVal;
			while (iter != "\0") {
				switch (state) {
				case 0://search for key
					if (iter == "\:") {
						if (size > 0) {
							headerKey.assign(iter - size, size);
							size = 0;
							state = 1;
						}
						else {
							return false;
						}
					}
					else {
						++size;
					}
					break;
				case 1://search for val
					if (iter == "\r" || iter == "\n") {
						state = 0;
						headerVal.assign(iter - size, size);
                        _headers[headerKey] = headerVal;
						size = 0;
						char *next = iter + 1;
						if (next == "\r" || next == "\n") {
							iter = next;
						}
					}
					else {
						++size;
					}
                    break;
				default:
					break;
				}
				++iter;
			}
            if(state == 1 && size > 0) {
                headerVal.assign(iter - size - 1, size);
                _headers[headerKey] = headerVal;
                return true;
            }
            if(state == 0 && size == 0) {
                return true;
            }
		}
        return false;
    }
private:
	std::string mStr;
};

class SIMPLEDIRECTUI_API XHTTPRequestHeader : public XHTTPHeader {
public:
    void setUserAgent(const char *val) {
        this->setHeader(cUserAgentKey.c_str(), val);
    }
    const char* userAgent() {
        return getHeader(cUserAgentKey.c_str());
    }
};
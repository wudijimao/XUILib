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
#include "XUrl.hpp"


//only support utf-8
class SIMPLEDIRECTUI_API XHTTPHeader {
public:
    static const std::string cUserAgentKey;
    std::map<std::string, std::string> _headers;
    std::string _headeLine;
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
        mStr.append(_headeLine);
        mStr.append("\r\n");
        for (auto header : _headers)
        {
            mStr.append(header.first);
            mStr.append(":");
            mStr.append(header.second);
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
            while (*iter != '\0') {
                switch (state) {
                    case -1:
                        if (*iter == '\r' || *iter == '\n') {
                            state = 0;
                            _headeLine.assign(iter - size, size);
                            size = 0;
                            char *next = iter + 1;
                            if (*next == '\r' || *next == '\n') {
                                iter = next;
                            }
                        } else {
                            ++size;
                        }
                        break;
                    case 0://search for key
                        if (*iter == ':') {
                            if (size > 0) {
                                headerKey.assign(iter - size, size);
                                size = 0;
                                state = 1;
                            }
                            else {
                                return false;
                            }
                        } else {
                            ++size;
                        }
                        break;
                    case 1://search for val
                        if (*iter == '\r' || *iter == '\n') {
                            char *next = iter + 1;
                            if (*next == '\r' || *next == '\n') {
                                iter = next;
                            }
                            next = iter + 1;
                            if (*next == ' ' || *next == '\t') {
                                iter = next;
                            } else {
                                state = 0;
                                headerVal.assign(iter - size, size);
                                _headers[headerKey] = headerVal;
                                size = 0;
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

class SIMPLEDIRECTUI_API XHTTPResponseHeader : public XHTTPHeader {
public:
};




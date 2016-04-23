//
//  XHTTPHeader.hpp
//  XDUILib
//
//  Created by ximiao on 16/4/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/stdafx.hpp"

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
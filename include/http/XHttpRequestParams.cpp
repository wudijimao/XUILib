//
//  XHttpRequestParams.cpp
//  XDUILib
//
//  Created by ximiao on 16/6/14.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XHttpRequestParams.hpp"


void XHttpRequestParams::addParam(const char *name, const char *val) {
    if (val) {
        mParamsMap[name] = val;
    } else {
        removeParam(name);
    }
}

void XHttpRequestParams::addParam(const char *name, ...) {
    
}

void XHttpRequestParams::removeParam(const char *name) {
    auto iter = mParamsMap.find(name);
    if (iter != mParamsMap.end()) {
        mParamsMap.erase(iter);
    }
}

bool XHttpRequestParams::setParams(const char *params) {
    if (params != nullptr) {
        mParamsMap.clear();
        int state = 0;
        char *iter = (char*)params;
        int size = 0;
        std::string key;
        std::string val;
        while (*iter != '\0') {
            switch (state) {
                case 0://search for key
                    if (*iter == '=') {
                            key.assign(iter - size, size);
                            size = 0;
                            state = 1;
                    } else {
                        ++size;
                    }
                    break;
                case 1://search for val
                    if (*iter == '&') {
                        state = 0;
                        val.assign(iter - size, size);
                        mParamsMap[XUrl::UrlDecode(key)] = XUrl::UrlDecode(val);
                        size = 0;
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
            val.assign(iter - size - 1, size);
            mParamsMap[key] = val;
            return true;
        }
        if(state == 0 && size == 0) {
            return true;
        }
    }
    mParamsMap.clear();
    return false;
}

const char * XHttpRequestParams::getStr() {
    mStr.clear();
    int i = 0;
    int size = mParamsMap.size();
    for (auto param : mParamsMap) {
        mStr.append(XUrl::UrlEncode(param.first));
        mStr.append("=");
        mStr.append(XUrl::UrlEncode(param.second));
        ++i;
        if (i < size) {
            mStr.append("&");
        }
    }
    return mStr.c_str();
}
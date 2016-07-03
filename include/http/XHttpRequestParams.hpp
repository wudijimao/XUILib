//
//  XHttpRequestParams.hpp
//  XDUILib
//
//  Created by ximiao on 16/6/14.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once
#include "../core/stdafx.hpp"
#include "XUrl.hpp"

class SIMPLEDIRECTUI_API XHttpRequestParams {
public:
    void addParam(const char *name, const char *val);
    void addParam(const char *name, ...);
    void removeParam(const char *name);
    const char *getStr();
    //e.g.:a=xxx&b=xxx&c=xxx
    bool setParams(const char *params);
private:
    std::string mStr;
    std::map<std::string, std::string> mParamsMap;
};
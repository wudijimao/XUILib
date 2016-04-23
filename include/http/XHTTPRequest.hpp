//
//  XHTTPRequest.hpp
//  XDUILib
//
//  Created by ximiao on 16/4/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/stdafx.hpp"
#include "XHTTPHeader.hpp"
class XHTTPResponse;

typedef enum XHTTPRequestMethod {
    XHTTPRequestMethod_Get = 0,
    XHTTPRequestMethod_Post = 1,
}XHTTPRequestMethod;

class SIMPLEDIRECTUI_API XHTTPRequest {
public:
    XHTTPRequestMethod method = XHTTPRequestMethod_Get;
    std::string baseUrl;
    std::string url;
    XHTTPRequestHeader _header;
    
    std::function<void(std::shared_ptr<XHTTPResponse>)> finishCallBack;
    
};

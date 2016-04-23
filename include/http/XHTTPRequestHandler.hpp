//
//  XHTTPRequestHandler.hpp
//  XDUILib
//
//  Created by ximiao on 16/4/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/stdafx.hpp"

class XHTTPRequest;
class XHTTPResponse;
class XHTTPClient;

class SIMPLEDIRECTUI_API XHTTPRequestHandler {
public:
    virtual void stopRequest();
    virtual void pauseRequest();
    virtual void continueRequest();
    XHTTPRequest *request();
public:
    std::shared_ptr<XHTTPResponse> _response;
    XHTTPClient *_weakHttpClient;
};
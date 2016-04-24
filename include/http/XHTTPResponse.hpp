//
//  XHTTPResponse.hpp
//  XDUILib
//
//  Created by ximiao on 16/4/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/stdafx.hpp"
#include "XHTTPHeader.hpp"
#include "XHTTPRequest.hpp"
#include "../res/XData.hpp"

enum class XHTTPResponsStatus {
    Connecting,
    Success,
    TimeOut,
};

class XHTTPRequestHandler;
class IXHTTPClient;
class XHTTPClient;

class XHTTPResponse {
    friend XHTTPRequestHandler;
    friend XHTTPClient;
    friend IXHTTPClient;
public:
    XHTTPResponsStatus _status;
    //const XHTTPHeader* header();
    std::shared_ptr<XResource::XData>& data(){
        return _buf;
    }
    std::shared_ptr<XResource::XData>& headerData() {
        return _headerBuf;
    }
protected:
    std::shared_ptr<XHTTPRequest> _request;
    std::shared_ptr<XResource::XData> _buf;
    std::shared_ptr<XResource::XData> _headerBuf;
};
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
    const XHTTPHeader* header();
    const char *buf() {
        return &_buf[0];
    }
    size_t bufSize() {
        return _buf.size();
    }
    const char *headerBuf() {
        return &_headerBuf[0];
    }
    size_t headerBufSize() {
        return _headerBuf.size();
    }
protected:
    std::shared_ptr<XHTTPRequest> _request;
    std::vector<char> _buf;
    std::vector<char> _headerBuf;
};
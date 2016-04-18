//
//  XHTTPClient.h
//  TestGCDCPP
//
//  Created by ximiao on 16/2/22.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include "../core/stdafx.hpp"
#include <string>
#include <map>
#include <vector>
#include <list>


typedef enum XHTTPRequestMethod {
    XHTTPRequestMethod_Get = 0,
    XHTTPRequestMethod_Post = 1,
}XHTTPRequestMethod;

typedef enum {
    XStringEncodeing_UTF8 = 0,
    XStringEncodeing_Default = XStringEncodeing_UTF8,
}XStringEncodeing;

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


class SIMPLEDIRECTUI_API XHTTPClient;

enum class XHTTPResponsStatus {
    Connecting,
    Success,
    TimeOut,
};

class XHTTPResponse;

class SIMPLEDIRECTUI_API XHTTPRequest {
public:
    XHTTPRequestMethod method = XHTTPRequestMethod_Get;
    std::string baseUrl;
    std::string url;
    XHTTPRequestHeader _header;
    
    std::function<void(std::shared_ptr<XHTTPResponse>)> finishCallBack;
    
};

class XHTTPRequestHandler;
class IXHTTPClient;
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

class SIMPLEDIRECTUI_API XHTTPRequestHandler {
public:
    virtual void stopRequest() {
        
    }
	virtual void pauseRequest() {
        
    }
	virtual void continueRequest() {
        
    }
    XHTTPRequest *request() {
        if (_response.get()) {
            return _response->_request.get();
        }
        return nullptr;
    }
public:
    std::shared_ptr<XHTTPResponse> _response;
    XHTTPClient *_weakHttpClient;
};

class SIMPLEDIRECTUI_API IXHTTPClient {
public:
    static IXHTTPClient *getSharedInstanc();
    //return nullptr when request is exist
	virtual std::shared_ptr<XHTTPRequestHandler> sendRequest(std::shared_ptr<XHTTPRequest> pRequest) = 0;
};




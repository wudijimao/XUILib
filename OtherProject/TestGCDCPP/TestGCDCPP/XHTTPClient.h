//
//  XHTTPClient.h
//  TestGCDCPP
//
//  Created by ximiao on 16/2/22.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#ifndef XHTTPClient_h
#define XHTTPClient_h

#include <string>
#include <map>
#include <vector>
#include <list>
//put this into impl
#include <curl.h>
#include <condition_variable>
#include <XError.hpp>

typedef enum XHTTPRequestMethod {
    XHTTPRequestMethod_Get = 0,
    XHTTPRequestMethod_Post = 1,
}XHTTPRequestMethod;

typedef enum {
    XStringEncodeing_UTF8 = 0,
    XStringEncodeing_Default = XStringEncodeing_UTF8,
}XStringEncodeing;

class XHTTPHeader {
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

class XHTTPRequestHeader : public XHTTPHeader {
public:
    void setUserAgent(const char *val) {
        this->setHeader(cUserAgentKey.c_str(), val);
    }
    const char* userAgent() {
        return getHeader(cUserAgentKey.c_str());
    }
};


class XHTTPClient;

enum class XHTTPResponsStatus {
    Connecting,
    Success,
    TimeOut,
};

class XHTTPResponse;

class XHTTPRequest {
public:
    XHTTPRequestMethod method = XHTTPRequestMethod_Get;
    std::string baseUrl;
    std::string url;
    XHTTPRequestHeader _header;
    
    std::function<void(std::shared_ptr<XHTTPResponse>)> finishCallBack;
    
};

class XHTTPRequestHandler;
class XHTTPResponse {
    friend XHTTPRequestHandler;
    friend XHTTPClient;
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

class XHTTPRequestHandler {
public:
    void stopRequest() {
        
    }
    void pauseRequest() {
        
    }
    void continueRequest() {
        
    }
    XHTTPRequest *request() {
        if (_response.get()) {
            return _response->_request.get();
        }
        return nullptr;
    }
public:
    std::shared_ptr<XHTTPResponse> _response;
    CURL *_handle;
    XHTTPClient *_weakHttpClient;
};

class XHTTPClient {
public:
    XHTTPClient();
    static XHTTPClient *getSharedInstanc();
    //return nullptr when request is exist
    std::shared_ptr<XHTTPRequestHandler> sendRequest(std::shared_ptr<XHTTPRequest> pRequest);
protected:
    static int sOnProgress(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
    void onProgress(XHTTPRequestHandler *httpRequest, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
    CURLM *_curl;
    std::condition_variable cv;
    std::mutex mutex;
    std::map<CURL*, std::shared_ptr<XHTTPRequestHandler>> _handlerMap;
    std::map<XHTTPRequest*, CURL*> _requestMap;
private:
    void runLoop();
    //only called by runLoop()
    void requestFinised(CURL *handle);
};

#endif /* XHTTPClient_h */



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
#include "XHTTPResponse.hpp"
#include "../core/MutiThread/XDispatch.h"
#include "XHttpRequestParams.hpp"

typedef enum XHTTPRequestMethod {
    XHTTPRequestMethod_Get = 0,
    XHTTPRequestMethod_Post = 1,
}XHTTPRequestMethod;

class XHTTPClient;

class SIMPLEDIRECTUI_API XHTTPRequest {
public:
    XHTTPRequestMethod method = XHTTPRequestMethod_Get;
    std::string baseUrl;
    std::string url;
    XHTTPRequestHeader _header;
    XHttpRequestParams _params;
    
    std::function<void(std::shared_ptr<XHTTPResponse>)> finishCallBack;
    
    virtual std::shared_ptr<XHTTPResponse> createResponse() {
        return std::make_shared<XHTTPResponse>();
    }
protected:
    friend XHTTPClient;
    virtual void onRequestFinished(std::shared_ptr<XHTTPResponse> response);
};

template<typename DataType>
class XHTTPRequestT : public XHTTPRequest {
public:
    typedef XHTTPResponseT<DataType> ResponseType;
    typedef std::shared_ptr<ResponseType> ResponseTypePtr;
    std::function<void(ResponseTypePtr)> onFinish;
private:
    virtual std::shared_ptr<XHTTPResponse> createResponse() override {
        return std::make_shared<ResponseType>();
    }
    virtual void onRequestFinished(std::shared_ptr<XHTTPResponse> response) override {
        XHTTPRequest::onRequestFinished(response);
        auto resp = std::dynamic_pointer_cast<ResponseType>(response);
        if(resp) {
            resp->data();
            if(onFinish) {
                auto fun = std::bind(onFinish, resp);
                XDispatch::XDispatchManager::getSharedInstance()->dispatchAsnyc(XDispatch::XTaskQueue::getMainQueue(), fun);
            }
        }
        
    }
};





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
    
    std::function<void(std::shared_ptr<XHTTPResponse>)> finishCallBack;
private:
    friend XHTTPClient;
    virtual void onRequestFinished(std::shared_ptr<XHTTPResponse> response);
};

template<typename DataType>
class XHTTPrequestT : public XHTTPRequest {
public:
    typedef XHTTPResponseT<DataType> ResponseType;
    std::function<void(std::shared_ptr<ResponseType>)> onFinish;
private:
    virtual void onRequestFinished(std::shared_ptr<XHTTPResponse> response) override {
        XHTTPRequest::onRequestFinished(response);
        auto resp = std::dynamic_pointer_cast<ResponseType>(response);
        if(resp) {
            auto fun = std::bind(onFinish, resp->getData());
            XDispatch::XDispatchManager::getSharedInstance()->dispatchAsnyc(XDispatch::XTaskQueue::getMainQueue(), fun);
        }
        
    }
};

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
protected:
    friend XHTTPClient;
    virtual void onRequestFinished(std::shared_ptr<XHTTPResponse> response);
};

template<typename DataType>
class XHTTPRequestT : public XHTTPRequest {
public:
    typedef XHTTPResponseT<DataType> ResponseType;
    std::function<void(std::shared_ptr<ResponseType>)> onFinish;
private:
    virtual void onRequestFinished(std::shared_ptr<XHTTPResponse> response) override {
        XHTTPRequest::onRequestFinished(response);
        //warning: If Response has virtual function, change it to daynic_pointer_case
        auto resp = std::static_pointer_cast<ResponseType>(response);
        if(resp) {
            resp->data();
            auto fun = std::bind(onFinish, resp);
            XDispatch::XDispatchManager::getSharedInstance()->dispatchAsnyc(XDispatch::XTaskQueue::getMainQueue(), fun);
        }
        
    }
};

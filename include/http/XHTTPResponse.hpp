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
#include "../res/XData.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

enum class XHTTPResponsStatus {
    Connecting,
    Success,
    TimeOut,
};

class XHTTPRequestHandler;
class IXHTTPClient;
class XHTTPClient;
class XHTTPRequest;

class XHTTPResponse {
    friend XHTTPRequestHandler;
    friend XHTTPClient;
    friend IXHTTPClient;
public:
    XHTTPResponsStatus _status;
    const std::shared_ptr<XHTTPResponseHeader> header();
    std::shared_ptr<XResource::XData>& contentData(){
        return _buf;
    }
    std::shared_ptr<XResource::XData>& headerData() {
        return _headerBuf;
    }
    std::shared_ptr<rapidjson::Document>& jsonData();
protected:
    std::shared_ptr<rapidjson::Document> _jsonData;
    std::shared_ptr<XHTTPRequest> _request;
    std::shared_ptr<XResource::XData> _buf;
    std::shared_ptr<XResource::XData> _headerBuf;
};

class XResponseData {
public:
    virtual void parse(const rapidjson::Document& json) = 0;
};

template<typename DataType>
class XHTTPResponseT : public XHTTPResponse {
public:
    typedef std::shared_ptr<DataType> DataTypePtr;
    DataTypePtr data() {
        if (!mData) {
            mData = std::make_shared<DataType>();
            auto json = jsonData();
            if (json) {
                mData->parse(*json);
            }
        }
        return mData;
    }
private:
    DataTypePtr mData;
};







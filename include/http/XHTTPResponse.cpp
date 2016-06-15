//
//  XHTTPResponsecpp
//  XDUILib
//
//  Created by ximiao on 16/4/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XHTTPResponse.hpp"


std::shared_ptr<rapidjson::Document>& XHTTPResponse::jsonData() {
    if (!_jsonData) {
        _jsonData = std::make_shared<rapidjson::Document>();
        if (_jsonData->Parse(contentData()->c_str()).HasParseError()) {
            rapidjson::ParseErrorCode errorCode = _jsonData->GetParseError();
            size_t offset = _jsonData->GetErrorOffset();
            _jsonData.reset();
        }
    }
    return _jsonData;
}

std::shared_ptr<XHTTPResponseHeader> XHTTPResponse::header() {
    auto header = std::make_shared<XHTTPResponseHeader>();
    auto data = headerData();
    if (data) {
        header->setStr(data->c_str());
    }
    return header;
}
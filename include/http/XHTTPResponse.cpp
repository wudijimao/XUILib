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
            _jsonData.reset();
        }
    }
    return _jsonData;
}
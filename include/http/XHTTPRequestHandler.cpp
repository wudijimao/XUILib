//
//  XHTTPRequestHandler.cpp
//  XDUILib
//
//  Created by ximiao on 16/4/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XHTTPRequestHandler.hpp"
#include "XHTTPResponse.hpp"

XHTTPRequest *XHTTPRequestHandler::request() {
    if (_response.get()) {
        return _response->_request.get();
    }
    return nullptr;
}

void XHTTPRequestHandler::stopRequest() {
    
}
void XHTTPRequestHandler::pauseRequest() {
    
}
void XHTTPRequestHandler::continueRequest() {
    
}
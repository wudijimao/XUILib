//
//  XHTTPRequest.cpp
//  XDUILib
//
//  Created by ximiao on 16/4/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XHTTPRequest.hpp"


void XHTTPRequest::onRequestFinished(std::shared_ptr<XHTTPResponse> response) {
    if(finishCallBack) {
        auto fun = std::bind(finishCallBack, response);
        XDispatch::XDispatchManager::getSharedInstance()->dispatchAsnyc(XDispatch::XTaskQueue::getMainQueue(), fun);
    }
}
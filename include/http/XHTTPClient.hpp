//
//  XHTTPClient.h
//  TestGCDCPP
//
//  Created by ximiao on 16/2/22.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include "../core/stdafx.hpp"
#include "XHTTPHeader.hpp"
#include "XHTTPRequest.hpp"
#include "XHTTPResponse.hpp"
#include "XHTTPRequestHandler.hpp"


class SIMPLEDIRECTUI_API IXHTTPClient {
public:
    static IXHTTPClient *getSharedInstanc();
    //return nullptr when request is exist
	virtual std::shared_ptr<XHTTPRequestHandler> sendRequest(std::shared_ptr<XHTTPRequest> pRequest) = 0;
};




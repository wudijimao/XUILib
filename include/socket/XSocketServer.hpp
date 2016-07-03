//
//  XSocketServer.hpp
//  XDUILib
//
//  Created by ximiao on 16/6/15.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once

#include "../core/stdafx.hpp"
#include "XSocketClient.hpp"

class SIMPLEDIRECTUI_API XSocketServer {
public:
    void run();
    std::function<void(XSocketClient &client)> onConnection;
    std::function<void(XSocketClient &client)> onError;
    std::function<void(XSocketClient &client)> onClose;
};
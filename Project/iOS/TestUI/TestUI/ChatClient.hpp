//
//  ChatClient.hpp
//  TestUI
//
//  Created by ximiao on 16/6/15.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once

#include <XDUILib.hpp>

class ChatClient {
public:
    XSocketClient client;
    XResource::XDataPtr data;
    void run() {
        client.onRecvData = [&](void *data, long size) {
            this->data = XResource::XData::data();
            std::string str("aaaaaaa");
            uint32_t dataLen = uint32_t(str.length() + 1 + 4);
            this->data->appendData((void*)&dataLen, 4);
            this->data->appendData(str.c_str(), str.length() + 1);
            client.send(this->data);
        };
        client.connect("138.128.201.89", 7777);
    }
};
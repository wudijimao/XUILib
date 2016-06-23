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
    
    void run() {
        
        client.setOnRecvData([&](const void *dat, unsigned int size) {
            XResource::XDataPtr data = XResource::XData::data();
            data->clear();
            data->appendData(dat, size);
            std::cout << "接收到:" << data->c_str() << std::endl;
            
            data->clear();
            std::string str("caaaaaaab");
            uint32_t dataLen = uint16_t(str.length() + 1);
            data->appendData((void*)&dataLen, 2);
            data->appendData(str.c_str(), str.length() + 1);
            client.send(data);
        });
        client.connect("138.128.201.89", 7777);
    }
};
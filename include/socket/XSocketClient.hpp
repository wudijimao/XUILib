//
//  XSocketClient.hpp
//  XDUILib
//
//  Created by ximiao on 16/6/15.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once

#include "../core/stdafx.hpp"
#include "asio/asio.hpp"
#include <iostream>
#include "../res/XData.hpp"
#include "XPackageReciver.hpp"


class SIMPLEDIRECTUI_API XSocketClient {
public:
    virtual ~XSocketClient(){}
    asio::io_service io_service;
    asio::ip::tcp::socket *socket;
    void connect(const char *ip, int port);
    void connectHost(const char *host, int port);
    void close();
    void send(const XResource::XDataPtr &data);
public:
    std::vector<std::shared_ptr<std::vector<char>>> mBufs;
    //events
    void setOnRecvData(std::function<void(const void *data, unsigned int size)> fun) {
        mReciver.onRecive = fun;
    }
    std::function<void()> onLookup;
    std::function<void()> onError;
    std::function<void()> onClose;
    
private:
    XPackageReciver mReciver;
    void onRecvInternal();
    virtual void onConnection();
    void recv_async();
    void send_async(const void *data, long size);
};



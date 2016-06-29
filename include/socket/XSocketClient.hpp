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
    std::function<void(const std::error_code&)> onError;
    std::function<void()> onClose;
    std::function<void()> onConnect;
private:
    inline bool onErrorInternal(const std::error_code& err) {
        if (this->onError) {
            this->onError(err);
        }
        return err.value();
    }
    void onRecvSome(const std::error_code& e, std::size_t bytes_transferred);
    char buf[128];
    XPackageReciver mReciver;
    void onRecvInternal();
    void recv_async();
    void send_async(const void *data, long size);
};



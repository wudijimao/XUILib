//
//  XSocketClient.cpp
//  XDUILib
//
//  Created by ximiao on 16/6/15.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XSocketClient.hpp"

//
//class shared_const_buffer
//{
//public:
//    // Construct from a std::string.
//    explicit shared_const_buffer(const std::string& data)
//    : data_(new std::vector<char>(data.begin(), data.end())),
//    buffer_(asio::buffer(*data_))
//    {
//    }
//    
//    // Implement the ConstBufferSequence requirements.
//    typedef asio::const_buffer value_type;
//    typedef const asio::const_buffer* const_iterator;
//    const asio::const_buffer* begin() const { return &buffer_; }
//    const asio::const_buffer* end() const { return &buffer_ + 1; }
//    
//private:
//    std::shared_ptr<std::vector<char> > data_;
//    asio::const_buffer buffer_;
//};


void XSocketClient::connect(const char *ip, int port) {
    //所有使用ASIO的程序最少有一个io_service对象
    
    //        //将服务器名字转换为TCP endpoint
    //        asio::ip::tcp::resolver resolver(io_service);
    //
    //        std::string host = "138.128.201.89" ;//specify the host
    //        //resolver需要一个query对象,并将query转换为a list of endpoints
    //                //host name,也就是IP地址
    //                //the name of service,也就是端口...
    //                asio::ip::tcp::resolver::query query(asio::ip::tcp::v4(), host, "7777");
    //                //  tcp::resolver::query query(tcp::v4(),argv[1], "13");
    //                //ip::tcp::resolver::iterator来保存list of endpoints
    //                asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    //        asio::connect(*socket, &endpoint, error);
    
    
    auto addr = asio::ip::address::from_string(ip);
    asio::ip::tcp::endpoint endpoint(addr, port);
    socket = new asio::ip::tcp::socket(io_service);
    asio::error_code error;
    socket->connect(endpoint, error);
    std::string  errmes = error.message();
    
    recv_async();
}

void XSocketClient::connectHost(const char *host, int port) {
    
}

void XSocketClient::close() {
    socket->close();
}

void XSocketClient::send(const XResource::XDataPtr &data) {
    //auto buf = std::make_shared<std::vector<char>>();
    //        buf->assign((char*)data->getBuf(), (char*)data->getBuf() + data->size());
    //        mBufs.push_back(buf);
    auto buf = asio::buffer(data->getBuf(), data->size());
    socket->async_send(buf, [data](const asio::error_code& error, std::size_t bytes_transferred) {
        if (bytes_transferred == data->size()) {
            //sendFinished
            int a = 0;
        }
        int b = 0;
    });
}

void XSocketClient::onConnection() {
}

void XSocketClient::recv_async() {
    char buf[128];
    while (1) {
        asio::error_code error;
        //boost::asio::buffer确定缓冲大小并防止越界.
        size_t len = socket->read_some(asio::buffer(buf), error);
        
        //当Server关闭连接的时候,read_some返回boost::asio::error::eof
        if(error == asio::error::eof) {
            break;
        } else if(error) {
            //error
            break;
        } else {
            mReciver.put(buf, len);
        }
    }
}

void XSocketClient::send_async(const void *data, long size) {
    
}



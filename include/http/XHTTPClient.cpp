//
//  XHTTPClient.cpp
//  TestGCDCPP
//
//  Created by ximiao on 16/2/22.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#include "XHTTPClient.hpp"
#include "XHTTPClientImpl.hpp"
#include <stdio.h>

#include <thread>
#include "../core/MutiThread/XDispatch.h"



//#include <memory>

/**
 * 使用select函数监听multi curl文件描述符的状态
 * 监听成功返回0，监听失败返回-1
 */
int curl_multi_select(CURLM * curl_m)
{
    struct timeval timeout_tv;
    fd_set  fd_read;
    fd_set  fd_write;
    fd_set  fd_except;
    int     max_fd = -1;
    
    // 注意这里一定要清空fdset,curl_multi_fdset不会执行fdset的清空操作  //
    FD_ZERO(&fd_read);
    FD_ZERO(&fd_write);
    FD_ZERO(&fd_except);
    
    // 设置select超时时间  //
    timeout_tv.tv_sec = 1;
    timeout_tv.tv_usec = 0;
    
    // 获取multi curl需要监听的文件描述符集合 fd_set //
    curl_multi_fdset(curl_m, &fd_read, &fd_write, &fd_except, &max_fd);
    
    int ret_code = 0;
    /**
     * When max_fd returns with -1,
     * you need to wait a while and then proceed and call curl_multi_perform anyway.
     * How long to wait? I would suggest 100 milliseconds at least,
     * but you may want to test it out in your own particular conditions to find a suitable value.
     */
    if (-1 == max_fd)
    {
#ifdef _WIN32
        Sleep(100);
        ret_code = 0;
#else
        /* Portable sleep for platforms other than Windows. */
        struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
        ret_code = select(0, NULL, NULL, NULL, &wait);
#endif
    } else {
        /**
         * 执行监听，当文件描述符状态发生改变的时候返回
         * 返回0，程序调用curl_multi_perform通知curl执行相应操作
         * 返回-1，表示select错误
         * 注意：即使select超时也需要返回0，具体可以去官网看文档说明
         */
        ret_code = ::select(max_fd + 1, &fd_read, &fd_write, &fd_except, &timeout_tv);
    }
    return ret_code;
}




size_t onWriteData(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    XResource::XData *buf = static_cast<XResource::XData*>(userdata);
    buf->appendData(ptr, realsize);
    return realsize; //?
}

IXHTTPClient* IXHTTPClient::getSharedInstanc() {
    static XHTTPClient client; //thread safe in c++11
    return &client;
}

XHTTPClient::XHTTPClient() {
    this->_curl = curl_multi_init();
    std::thread thread(std::bind(&XHTTPClient::runLoop, this));
    thread.detach();
}

void XHTTPClient::runLoop() {
    int running_handles = 0;
    std::unique_lock<std::mutex> lk(mutex);
    while (1)
    {
        do{
            if (curl_multi_select(this->_curl) == -1) {
				this->cv.wait(lk);
                break;
            } else  {
                curl_multi_perform(this->_curl, &running_handles);
                CURLMsg *m;
                do {
                    int msgq = 0;
                    m = curl_multi_info_read(this->_curl, &msgq);
                    if(m && (m->msg == CURLMSG_DONE)) {
                        this->requestFinised(m->easy_handle);
                    }
                } while(m);
            }
        } while (running_handles > 0);
    }
}

void XHTTPClient::onProgress(XHTTPRequestHandler *httpRequest, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
}

int XHTTPClient::sOnProgress(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    XHTTPRequestHandler *requestHandler = static_cast<XHTTPRequestHandler*>(userdata);
    requestHandler->_weakHttpClient->onProgress(requestHandler, dltotal, dlnow, ultotal, ulnow);
    return 0;
}

void XHTTPClient::requestFinised(CURL *handle) {
    auto requestHandler = this->_handlerMap[handle];
    this->_handlerMap.erase(this->_handlerMap.find(handle));
    this->_requestMap.erase(this->_requestMap.find(requestHandler->request()));
    curl_multi_remove_handle(this->_curl, handle);
    curl_easy_cleanup(handle);
    requestHandler->_handle = nullptr;
	curl_slist_free_all(_requestHeaderMap[requestHandler->request()]); /* free the list again */
	_requestHeaderMap.erase(_requestHeaderMap.find(requestHandler->request()));
    //dowloadfinished
    auto fun = std::bind(requestHandler->request()->finishCallBack, requestHandler->_response);
    XDispatch::XDispatchManager::getSharedInstance()->dispatchAsnyc(XDispatch::XTaskQueue::getMainQueue(), fun);
}


std::shared_ptr<XHTTPRequestHandler> XHTTPClient::sendRequest(std::shared_ptr<XHTTPRequest> pRequest) {
    auto iter = this->_requestMap.find(pRequest.get());
    if (iter != this->_requestMap.end()) {
        return nullptr;
    }
    std::shared_ptr<XHTTPRequestHandlerImpl> handler(new XHTTPRequestHandlerImpl());
    handler->_response.reset(new XHTTPResponse());
    handler->_response->_request = pRequest;
    handler->_weakHttpClient = this;
    CURL *handle = curl_easy_init();
    handler->_handle = handle;
    this->_requestMap[pRequest.get()] = handle;
    this->_handlerMap[handle] = handler;
    handler->_response->_buf = std::make_shared<XResource::XData>();
    curl_easy_setopt(handle, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(handle, CURLOPT_URL, pRequest->url.c_str());
	struct curl_slist *list = NULL;
	for (auto header : pRequest->_header._headers)
	{
		std::string str(header.first);
		str.append(": ");
		str.append(header.second);
		list = curl_slist_append(list, str.c_str());
	}
	list = curl_slist_append(list, "Accept:");
	_requestHeaderMap[pRequest.get()] = list;
	curl_easy_setopt(handle, CURLOPT_PROXY, "127.0.0.1:8888");//设置代理服务器
	
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, list);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, onWriteData);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)(handler->_response->_buf.get()));
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, XHTTPClient::sOnProgress);

	curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, 3);

    XHTTPRequest *req = pRequest.get();
    curl_easy_setopt(handle, CURLOPT_XFERINFODATA, (void *)handler.get());
    curl_multi_add_handle(this->_curl, handle);
	cv.notify_one();
    return handler;
}





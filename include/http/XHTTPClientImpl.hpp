//
//  XHTTPClient.h
//  TestGCDCPP
//
//  Created by ximiao on 16/2/22.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once
#include "XHTTPClient.hpp"
//put this into impl
#include <curl/curl.h>
#include <condition_variable>


class XHTTPRequestHandlerImpl : public XHTTPRequestHandler {
public:
	virtual void stopRequest() {

	}
	virtual void pauseRequest() {

	}
	virtual void continueRequest() {

	}
public:
	CURL *_handle;
};

class SIMPLEDIRECTUI_API XHTTPClient : public IXHTTPClient {
public:
	XHTTPClient();
	virtual std::shared_ptr<XHTTPRequestHandler> sendRequest(std::shared_ptr<XHTTPRequest> pRequest);
protected:
	static int sOnProgress(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
	int onProgress(XHTTPRequestHandler *httpRequest, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
	CURLM *_curl;
	std::condition_variable cv;
	std::mutex mutex;
	std::map<CURL*, std::shared_ptr<XHTTPRequestHandlerImpl>> _handlerMap;
	std::map<XHTTPRequest*, CURL*> _requestMap;
	std::map<XHTTPRequest*, curl_slist*> _requestHeaderMap;
private:
	void runLoop();
	//only called by runLoop()
	void requestFinised(CURLMsg *m);
};



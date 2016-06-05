//
//  XRunLoop.hpp
//  TestGCDCPP
//
//  Created by ximiao on 16/3/3.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once
#include "../stdafx.hpp"
#include <memory.h>
#include <map>
#include <vector>

#include <stdio.h>
#include <chrono>
#include <thread>
#include <mutex>

class XRunLoopSource {
public:
    virtual void _do() = 0;
};

class IXRunLoop {
public:
    virtual void run() = 0;
    //void runUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time);
    virtual bool weakUp(XRunLoopSource *source) {
		if (isExsit(source))
		{
			if (mIsSleeping) {
				if (!isInRunList(source))
				{
					mRunList.push_back(source);
				}
			}
			else {
				if (!isInWaitRunList(source))
				{
					mWaitRunList.push_back(source);
				}
			}
		}
        return true;
    };
    bool _do() {
        mIsSleeping = false;
        for (auto source : mRunList) {
            source->_do();
        }
		mRunList.clear();
        mIsSleeping = true;
        wait();
        return true;
    }
    virtual void wait() = 0;
    virtual bool waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) = 0;
    void addSource(const std::shared_ptr<XRunLoopSource> &source) {
        mSourcesMap[source.get()] = source;
    }
protected:
	inline bool isExsit(XRunLoopSource *source) {
		if (mSourcesMap.find(source) != mSourcesMap.end())
		{
			return true;
		}
		return false;
	}
	inline bool isInRunList(XRunLoopSource *source) {
		for (auto s : mRunList)
		{
			if (s == source)
			{
				return true;
			}
		}
		return false;
	}
	inline bool isInWaitRunList(XRunLoopSource *source) {
		for (auto s : mWaitRunList)
		{
			if (s == source)
			{
				return true;
			}
		}
		return false;
	}
    bool mIsSleeping = true;
    std::vector<XRunLoopSource*> mRunList;
    std::vector<XRunLoopSource*> mWaitRunList;
    std::map<XRunLoopSource*, std::shared_ptr<XRunLoopSource>> mSourcesMap;
};


std::shared_ptr<IXRunLoop> getMainRunLoop();
std::shared_ptr<IXRunLoop> getCurrentRunLoop();

class XRunLoop : IXRunLoop {
public:
    virtual void run() override;
    virtual bool weakUp(XRunLoopSource *source) override;
    virtual void wait() override;
    virtual bool waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) override;
protected:
    XRunLoop() {
    }
private:
    std::mutex mutex;
    std::condition_variable cv;
};

class XMainRunloop : public IXRunLoop {
    virtual void run() override;
    virtual bool weakUp(XRunLoopSource *source) override;
    virtual void wait() override;
    virtual bool waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) override;
};

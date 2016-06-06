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
#include <atomic>

class XRunLoopSource {
public:
    bool mIsSleeping = true;
    virtual void _do() = 0;
};

class IXRunLoop {
public:
    IXRunLoop() {
        mIsSleeping = true;
    }
    virtual void run() = 0;
    //void runUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time);
    virtual bool weakUp(XRunLoopSource *source) {
		if (isExsit(source))
		{
            source->mIsSleeping = false;
		}
        return true;
    };
    bool _do() {
        for (auto source : mSourcesList) {
            if (!source->mIsSleeping) {
                source->_do();
            }
        }
        wait();
        return true;
    }
    virtual void wait() = 0;
    virtual bool waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) = 0;
    void addSource(const std::shared_ptr<XRunLoopSource> &source) {
        if(!isExsit(source.get())) {
            mSourcesMap[source.get()] = source;
            mSourcesList.push_back(source.get());
        }
    }
protected:
	inline bool isExsit(XRunLoopSource *source) {
		if (mSourcesMap.find(source) != mSourcesMap.end())
		{
			return true;
		}
		return false;
	}
    std::atomic_bool mIsSleeping;
    std::vector<XRunLoopSource*> mSourcesList;
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

//
//  XThreadPool.cpp
//  XDUILib
//
//  Created by ximiao on 16/6/6.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XThreadPool.hpp"
#include "XRunLoop.hpp"
#include "XTaskQueue.hpp"
#include "XDispatch.h"

namespace XDispatch {
    
    void XThreadPool::runLoop() {
        ++threadNum;
        do {
            std::unique_lock<std::mutex> lk(mutex);
            auto iter = taskQueue.begin();
            auto end = taskQueue.end();
            MyFun *fun = NULL;
            bool hasFun = false;
            while (iter != end) {
                if ((*iter)->pop(fun)) {
                    hasFun = true;
                    break;
                }
                ++iter;
            }
            if (hasFun) {
                lk.unlock();
                (*fun)();
                delete fun;
            }
            else {
                cv.wait(lk);
            }
        } while (1);
        --threadNum;
    }
    void XThreadPool::onQueueChanged() {
        mutex.lock();
        cv.notify_one();
        mutex.unlock();
    }
    void XThreadPool::initGlobelPool() {
        //		static XThreadPool mainThread;
        //		mainThread.taskQueue.push_back(XTaskQueue::getMainQueue());
        //		XTaskQueue::getMainQueue()->runInPool = &mainThread;
        //		mainThread.startNewThread();
        
        static XThreadPool backgroundThreadsPool;
        backgroundThreadsPool.taskQueue.push_back(XTaskQueue::getGlobleQueue(XTaskPriority_High));
        backgroundThreadsPool.taskQueue.push_back(XTaskQueue::getGlobleQueue(XTaskPriority_Default));
        backgroundThreadsPool.taskQueue.push_back(XTaskQueue::getGlobleQueue(XTaskPriority_Low));
        backgroundThreadsPool.taskQueue.push_back(XTaskQueue::getGlobleQueue(XTaskPriority_Background));
        XTaskQueue::getGlobleQueue(XTaskPriority_Background)->runInPool = &backgroundThreadsPool;
        XTaskQueue::getGlobleQueue(XTaskPriority_Low)->runInPool = &backgroundThreadsPool;
        XTaskQueue::getGlobleQueue(XTaskPriority_Default)->runInPool = &backgroundThreadsPool;
        XTaskQueue::getGlobleQueue(XTaskPriority_High)->runInPool = &backgroundThreadsPool;
        for (int i = 0; i < 4; ++i) {
            backgroundThreadsPool.startNewThread();
        }
        
        XDispatch::XDispatchManager::mMainThreadSource = std::make_shared<XRunLoopDispatchSource>();
        XDispatch::XDispatchManager::mMainThreadSource->mTaskQueue = XTaskQueue::getMainQueue();
        getMainRunLoop()->addSource(XDispatch::XDispatchManager::mMainThreadSource);
    }
    void XThreadPool::startNewThread() {
        std::thread thread(std::bind(&XThreadPool::runLoop, this));
        thread.detach();
    }
    XThreadPool::XThreadPool() {
        taskNum = 0;
    }
    XThreadPool::~XThreadPool() {
    }
    
}

    

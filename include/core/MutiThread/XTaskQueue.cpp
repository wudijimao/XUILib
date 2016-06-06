//
//  XTaskQueue.cpp
//  TestGCDCPP
//
//  Created by ximiao on 16/3/4.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XTaskQueue.hpp"

namespace XDispatch {
    std::shared_ptr<XTaskQueue> XTaskQueue::getMainQueue() {
        //TODO::maybe mutithread cause problem
        static std::shared_ptr<XTaskQueue> mainQueue(new XTaskQueue());
        return mainQueue;
    }
    std::shared_ptr<XTaskQueue> XTaskQueue::getGlobleQueue(XTaskPriority priority) {
        static std::shared_ptr<XTaskQueue> backgroundQueue(new XTaskQueue());
        static std::shared_ptr<XTaskQueue> lowQueue(new XTaskQueue());
        static std::shared_ptr<XTaskQueue> defaultQueue(new XTaskQueue());
        static std::shared_ptr<XTaskQueue> highQueue(new XTaskQueue());
        switch (priority) {
            case XTaskPriority_Background:
                return backgroundQueue;
            case XTaskPriority_Low:
                return lowQueue;
            case XTaskPriority_High:
                return highQueue;
            case XTaskPriority_Default:
            default:
                return defaultQueue;
        }
    }
    
    bool XTaskQueue::pop(MyFun *&fun) {
        std::unique_lock<std::mutex> lk(mutex);
        if (queue.size() > 0) {
            fun = queue.front();
            queue.pop();
            return true;
        }
        return false;
    }
    
    void XTaskQueue::push(MyFun *fun) {
        mutex.lock();
        queue.push(fun);
        mutex.unlock();
    }
    
    XTaskQueue::~XTaskQueue() {
        
    }
}



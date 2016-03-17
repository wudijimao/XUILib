//
//  XTaskQueue.hpp
//  TestGCDCPP
//
//  Created by ximiao on 16/3/4.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once
#include <queue>
#include <mutex>

namespace XDispatch {
    typedef enum XTaskPriority {
        XTaskPriority_High,
        XTaskPriority_Default,
        XTaskPriority_Low,
        XTaskPriority_Background,
    }XTaskPriority;
    typedef std::function<void(void)> MyFun;
    
    class XThreadPool;
    //TODO::ªª”√ŒﬁÀ¯∂”¡–£®lock-free£©°¢Ã·∏ﬂ–ß¬
    class XTaskQueue {
        std::mutex mutex;
        std::queue<MyFun*> queue;
    public:
        ~XTaskQueue();
        XThreadPool *runInPool;
        static std::shared_ptr<XTaskQueue> getMainQueue();
        static std::shared_ptr<XTaskQueue> getGlobleQueue(XTaskPriority priority);
        bool pop(MyFun *&fun);
        void push(MyFun *fun);
    };
    
}
//
//  XThreadPool.hpp
//  XDUILib
//
//  Created by ximiao on 16/6/6.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once
#include "../stdafx.hpp"
#include <mutex>
#include <atomic>


namespace XDispatch {
    class XTaskQueue;
    
    class SIMPLEDIRECTUI_API XThreadPool {
    private:
        std::atomic_int_fast8_t threadNum;
        int_fast8_t maxNum;
        std::atomic_int taskNum;
        std::mutex mutex;
        std::condition_variable cv;
        void runLoop();
    public:
        std::vector<std::shared_ptr<XTaskQueue>> taskQueue;
        void onQueueChanged();
        //≥ı ºªØ¡Ω∏ˆ»´æ÷Pool
        static void initGlobelPool();
        void startNewThread();
        XThreadPool();
        virtual ~XThreadPool();
    };
}
//
//  XTask.hpp
//  TestGCDCPP
//
//  Created by ximiao on 16/3/4.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#include "XTaskQueue.hpp"

namespace XDispatch {
    struct XTask {
    public:
        XTask(MyFun *in_fun,
              std::shared_ptr<XTaskQueue> &in_queue,
              std::chrono::time_point<std::chrono::system_clock> &&in_time);
        ~XTask();
        MyFun *fun;
        std::shared_ptr<XTaskQueue> queue;
        std::chrono::time_point<std::chrono::system_clock> time;
        bool operator <(const XTask& rh);
    };
}

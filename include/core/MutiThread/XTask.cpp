//
//  XTask.cpp
//  TestGCDCPP
//
//  Created by ximiao on 16/3/4.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include "XTask.hpp"


namespace XDispatch {
    
    XTask::XTask(MyFun *in_fun,
                 std::shared_ptr<XTaskQueue> &in_queue,
                 std::chrono::time_point<std::chrono::system_clock> &&in_time) : fun(in_fun), mRunningInTaskQueue(in_queue), time(in_time) {
        
    }
    bool XTask::operator <(const XTask& rh) {
        return time < rh.time;
    }
    XTask::~XTask() {
        
    }
    
}
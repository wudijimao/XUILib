//
//  XRunLoop.hpp
//  TestGCDCPP
//
//  Created by ximiao on 16/3/3.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include <stdio.h>
#include <chrono>
#include <thread>


enum class XRunloopMode {
    Default,
};

class XRunLoop {
public:
    static std::shared_ptr<XRunLoop>& currentRunLoop();
    bool run();
    bool run(XRunloopMode mode);
    //bool run(XRunloopMode mode, const std::chrono::duration<std::_Rep, std::_Period> time;
private:
    XRunLoop(){
        
    }
    void defaultRunLoop() {
        
    }
    inline void readQuerry() {
        
    }
};
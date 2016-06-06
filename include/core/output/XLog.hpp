//
//  XLog.hpp
//  XDUILib
//
//  Created by ximiao on 16/6/6.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once

enum class XLogLevel {
    Off = -4,
    Fatal = -3,
    Error = -2,
    Warning = -1,
    Info = 0,
    Debug = 1,
    Trace = 2,
    All = 3
};

enum class LogType {
    None = 0,
    CommandLine = 1,
    File = 2,
    CommandLineAndFile = 3,
};


class XLog {
public:
    inline static XLog&sharedInstance() {
        return sLog;
    }
    void log(XLogLevel level, const char*text, ...);
private:
    static XLog sLog;
    LogType mOutputType = LogType::CommandLine;
};
//
//  XLog.cpp
//  XDUILib
//
//  Created by ximiao on 16/6/6.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XLog.hpp"
#include <stdio.h>
#include <stdarg.h>
#import <sys/uio.h>
#import <asl.h>
#include <unistd.h>
#include <string.h>

XLog XLog::sLog;

void XLog::log(XLogLevel level, const char*text, ...) {
    char logText[2048];
    va_list ap;
    va_start(ap, text);
    vsprintf(logText, text, ap);
    va_end(ap);
    
    struct iovec v;
    v.iov_base = (void*)logText;
    v.iov_len = strlen(logText);
    writev(STDERR_FILENO, &v, 1);
}
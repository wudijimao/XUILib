//
//  ResourceHelper.m
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#import <UIKit/UIKit.h>
#include "../../include/core/XResManager.hpp"
#include <sys/stat.h>
#define _mkdir(str) mkdir(str, S_IRWXU | S_IRWXG | S_IRWXO)

std::shared_ptr<XDictionaryInfo> XSandBox::homeDict() {
    if (!mHomeDict) {
        NSString *nsStr = NSHomeDirectory();
        mHomeDict = XDictionaryInfo::dictInfoForPath(nsStr.UTF8String);
        
        NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        
    }
    return mHomeDict;
}

std::shared_ptr<XDictionaryInfo> XSandBox::appDict() {
    if (!mAppDict) {
        NSString *nsStr = [NSBundle mainBundle].bundlePath;
        mAppDict = XDictionaryInfo::dictInfoForPath(nsStr.UTF8String);
    }
    return mAppDict;
}

XPathCrateResault XDictionaryInfo::create() {
    if (isExist()) {
        return XPathCrateResault::Exists;
    }
    int status = _mkdir(mPath.getUTF8String().c_str());
    if (status == 1) {
        return XPathCrateResault::Success;
    }
    return XPathCrateResault::NoPermission;
}

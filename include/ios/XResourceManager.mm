//
//  ResourceHelper.m
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#import <UIKit/UIKit.h>
#include "../core/XResManager.hpp"


std::shared_ptr<XDictionaryInfo> XSandBox::homeDict() {
    if (!mHomeDict) {
        NSString *nsStr = NSHomeDirectory();
        mHomeDict = XDictionaryInfo::dictInfoForPath(nsStr.UTF8String);
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

//XResource::XString XResManager::pathForResource(const char *fileName, const char *ext) {
//    return XResource::XString([bundle() pathForResource:[NSString stringWithUTF8String:fileName] ofType:[NSString stringWithUTF8String:ext]].UTF8String);
//}


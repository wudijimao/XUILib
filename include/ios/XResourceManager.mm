//
//  ResourceHelper.m
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#import <UIKit/UIKit.h>
#include "../core/XResManager.hpp"

NSString* pathForBundle() {
    static NSString *path;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        path = [[NSBundle mainBundle] pathForResource:@"res" ofType:@"bundle"];
    });
    return path;
}

NSBundle *bundle() {
    static NSBundle *bundle;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        bundle = [NSBundle bundleWithPath:pathForBundle()];
    });
    return bundle;
}



XResource::XString XResManager::pathForResource(const char *fileName, const char *ext) {
    return XResource::XString([bundle() pathForResource:[NSString stringWithUTF8String:fileName] ofType:[NSString stringWithUTF8String:ext]].UTF8String);
}


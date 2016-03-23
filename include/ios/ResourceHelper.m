//
//  ResourceHelper.m
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#import "ResourceHelper.h"

@implementation ResourceHelper
+ (NSString*)bundlePath {
    static NSString *path;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        path = [[NSBundle mainBundle] pathForResource:@"res" ofType:@"bundle"];
    });
    return path;
}

+ (NSBundle*)bundle {
    static NSBundle *bundle;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        bundle = [NSBundle bundleWithPath:[self bundlePath]];
    });
    return bundle;
}

+ (NSString*)pathForResource:(NSString*)str ofType:(NSString*)type {
    return [[self bundle] pathForResource:str ofType:type];
}
@end

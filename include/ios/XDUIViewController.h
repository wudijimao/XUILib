//
//  XDUIViewController.h
//  XDUILib
//
//  Created by ximiao on 16/3/17.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

class XWindow_ios;
@interface XDUIViewController : GLKViewController
- (void)setBelongWindow:(XWindow_ios*)window;
- (void)removeBelongWindow;
@end

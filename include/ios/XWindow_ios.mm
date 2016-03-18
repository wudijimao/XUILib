//
//  XWindow.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/18.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XWindow_ios.hpp"
#import "XDUIViewController.h"

XWindow::XWindow() {
    CGRect rect = [UIScreen mainScreen].bounds;
    this->window = [[UIWindow alloc] initWithFrame:rect];
    
    auto controller = [[XDUIViewController alloc] init];
    this->window.rootViewController = controller;
}

XWindow::~XWindow() {
}

void XWindow::showInFront() {
    [this->window makeKeyAndVisible];
}
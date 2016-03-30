//
//  XDUIViewController.m
//  XDUILib
//
//  Created by ximiao on 16/3/17.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#import "XDUIViewController.h"
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "GLCanvas_ios.hpp"
#import "XWindow_ios.hpp"
#include "GLView.h"

@interface XDUIViewController ()

@end

@implementation XDUIViewController {
    XWindow_ios *_window; //weak
    CADisplayLink *_dispalyLink;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.view = [[GLView alloc] initWithFrame:self.view.frame];
    }
    return self;
}

- (void)viewDidLoad {
    _dispalyLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update)];
    [_dispalyLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)update {
    if (_window) {
        //_window->update();
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillAppear:(BOOL)animated {
}


- (void)setBelongWindow:(XWindow_ios *)window {
    _window = window;
}
- (void)removeBelongWindow {
    _window = nullptr;
}


@end

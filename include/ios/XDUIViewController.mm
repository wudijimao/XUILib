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

@interface XDUIViewController ()

@end

@implementation XDUIViewController {
    XWindow_ios *_window;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)update {
   
}
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    if (_window) {
        _window->update();
    }
}

- (void)setBelongWindow:(XWindow_ios *)window {
    _window = window;
}
- (void)removeBelongWindow {
    _window = nullptr;
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end

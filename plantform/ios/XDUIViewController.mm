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
#include "GLView.hpp"

@interface XDUIViewController ()

@end

void remoteTouches(NSSet<UITouch *> *touches, XWindow_ios *window, UIView *view) {
    for (UITouch *touch in touches) {
        auto point = [touch locationInView:view];
        XTouch *t = new XTouch();
        t->mPosition.X(point.x);
        t->mPosition.Y(point.y);
        t->_belongWindow = window;
        t->phase = (TouchPhase)touch.phase;
        t->tapCount = (unsigned int)touch.tapCount;
        window->input(std::shared_ptr<XTouch>(t));
    }
    window->dispatchTouchs();
}

@implementation XDUIViewController {
    XWindow_ios *_window; //weak
    CADisplayLink *_dispalyLink;
}

- (instancetype)init {
    self = [super init];
    if (self) {
    }
    return self;
}

- (void)viewWillAppear:(BOOL)animated {
    
}

- (void)viewDidLoad {
    GLView *view = [[GLView alloc] initWithFrame:self.view.frame];
    self.view = view;
    CAEAGLLayer *layer = (CAEAGLLayer*)view.layer;
    layer.contentsScale = 2.0f;
//    UITextView *textView = [[UITextView alloc] initWithFrame:CGRectMake(120, 130, 88, 300)];
//    textView.backgroundColor = [UIColor lightGrayColor];
//    textView.textContainerInset = UIEdgeInsetsMake(0, 0, 0, 0);
//    textView.contentInset = UIEdgeInsetsMake(0, 0, 0, 0);
//    textView.font = [UIFont systemFontOfSize:14.0];
//    NSMutableAttributedString *str = [[NSMutableAttributedString alloc] initWithString:@"button"];
//    [str addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:14] range:NSMakeRange(0, str.length)];
//    NSMutableParagraphStyle *ps = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
//    ps.lineSpacing = 20.0;
//    [str addAttribute:NSParagraphStyleAttributeName value:ps range:NSMakeRange(0, str.length)];
//    [str addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:20] range:NSMakeRange(5, 4)];
//    textView.attributedText = str;
   // [self.view addSubview:textView];
    _window->init(layer);
    _window->update();
    _dispalyLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update)];
    //_dispalyLink.frameInterval = 15;
    [_dispalyLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
    [view becomeFirstResponder];
}

- (void)update {
    //NSLog(@"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    if (_window) {
        _window->update();
    }
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    remoteTouches(touches, _window, self.view);
}
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    remoteTouches(touches, _window, self.view);
}
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    remoteTouches(touches, _window, self.view);
}
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    remoteTouches(touches, _window, self.view);
}
- (void)touchesEstimatedPropertiesUpdated:(NSSet *)touches {
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



- (void)setBelongWindow:(XWindow_ios *)window {
    _window = window;
}
- (void)removeBelongWindow {
    _window = nullptr;
}


@end

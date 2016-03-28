//
//  XDUIViewController.h
//  XDUILib
//
//  Created by ximiao on 16/3/17.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#import <UIKit/UIKit.h>
#import "IXRender.hpp"

@interface XDUIViewController : UIViewController

- (std::shared_ptr<IXCanvas>)initOpenGLES;

@end

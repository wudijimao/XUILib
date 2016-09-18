//
//  GLView.h
//  Tutorial1
//
//  Created by Anton Holmquist on 6/11/12.
//  Copyright (c) 2012 Anton Holmquist. All rights reserved.
//
#pragma once
#import <UIKit/UIKit.h>

@protocol TextInputDelegate <NSObject>
- (void)inputText:(NSString *)text;
- (void)deleteBackward;
@end

@interface GLView : UIView<UIKeyInput, UITextInput>
- (void)setTextInputDelegate:(id<TextInputDelegate>) delegate;
@end

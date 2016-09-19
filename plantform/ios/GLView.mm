//
//  GLView.m
//  Tutorial1
//
//  Created by Anton Holmquist on 6/11/12.
//  Copyright (c) 2012 Anton Holmquist. All rights reserved.
//

#import "GLView.hpp"
#include "../../include/core/UIResponder.hpp"

@interface GLView()

@property (nonatomic, strong) NSString *intlInput;

@end

@implementation GLView {
    id<TextInputDelegate> _delegate;
}

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (BOOL)canBecomeFirstResponder
{
    return YES;
}

#pragma mark - UIKeyInput protocol


- (BOOL)hasText
{
    return NO;
}

- (void)insertText:(NSString *)text
{
    [_delegate inputText:text];
    const char * pszText = [text cStringUsingEncoding:NSUTF8StringEncoding];
    if (XUI::UIResponder::sFirstResponder != nullptr) {
        XUI::UIResponder::sFirstResponder->insertText(pszText);
    }
}

- (void)setMarkedText:(NSString *)markedText selectedRange:(NSRange)selectedRange {
    self.intlInput = markedText;
}
- (void) unmarkText {
    if (!self.intlInput) return;
    [self insertText:self.intlInput];
//    for (int i=0;i<self.intlInput.length;i++) {
//        [self sendChar:[self.intlInput characterAtIndex:i]];
//    }
    self.intlInput = nil;
}

- (void)deleteBackward
{
    [_delegate deleteBackward];
    if (XUI::UIResponder::sFirstResponder != nullptr) {
        XUI::UIResponder::sFirstResponder->deleteBackward();
    }
}

#pragma mark - UITextInputTrait protocol

-(UITextAutocapitalizationType) autocapitalizationType
{
    return UITextAutocapitalizationTypeNone;
}

#pragma mark - UITextInput protocol

#pragma mark UITextInput - properties

@synthesize beginningOfDocument;
@synthesize endOfDocument;
@synthesize inputDelegate;
@synthesize markedTextRange;
@synthesize markedTextStyle;
// @synthesize selectedTextRange;       // must implement
@synthesize tokenizer;

/* Text may have a selection, either zero-length (a caret) or ranged.  Editing operations are
 * always performed on the text from this selection.  nil corresponds to no selection. */
- (void)setSelectedTextRange:(UITextRange *)aSelectedTextRange;
{
    
}
- (UITextRange *)selectedTextRange;
{
    return [[UITextRange alloc] init];
}

#pragma mark UITextInput - Replacing and Returning Text

- (NSString *)textInRange:(UITextRange *)range;
{
    return @"";
}
- (void)replaceRange:(UITextRange *)range withText:(NSString *)theText;
{
}

#pragma mark UITextInput - Working with Marked and Selected Text



/* If text can be selected, it can be marked. Marked text represents provisionally
 * inserted text that has yet to be confirmed by the user.  It requires unique visual
 * treatment in its display.  If there is any marked text, the selection, whether a
 * caret or an extended range, always resides within.
 *
 * Setting marked text either replaces the existing marked text or, if none is present,
 * inserts it from the current selection. */

- (void)setMarkedTextRange:(UITextRange *)markedTextRange;
{
    
}
- (UITextRange *)markedTextRange;
{
    return nil; // Nil if no marked text.
}
- (void)setMarkedTextStyle:(NSDictionary *)markedTextStyle;
{
}
- (NSDictionary *)markedTextStyle;
{
    return nil;
}

#pragma mark Methods for creating ranges and positions.

- (UITextRange *)textRangeFromPosition:(UITextPosition *)fromPosition toPosition:(UITextPosition *)toPosition;
{
    return nil;
}
- (UITextPosition *)positionFromPosition:(UITextPosition *)position offset:(NSInteger)offset;
{
    return nil;
}
- (UITextPosition *)positionFromPosition:(UITextPosition *)position inDirection:(UITextLayoutDirection)direction offset:(NSInteger)offset;
{
    return nil;
}

/* Simple evaluation of positions */
- (NSComparisonResult)comparePosition:(UITextPosition *)position toPosition:(UITextPosition *)other;
{
    return (NSComparisonResult)0;
}
- (NSInteger)offsetFromPosition:(UITextPosition *)from toPosition:(UITextPosition *)toPosition;
{
    return 0;
}

- (UITextPosition *)positionWithinRange:(UITextRange *)range farthestInDirection:(UITextLayoutDirection)direction;
{
    return nil;
}
- (UITextRange *)characterRangeByExtendingPosition:(UITextPosition *)position inDirection:(UITextLayoutDirection)direction;
{
    return nil;
}

#pragma mark Writing direction

- (UITextWritingDirection)baseWritingDirectionForPosition:(UITextPosition *)position inDirection:(UITextStorageDirection)direction;
{
    return UITextWritingDirectionNatural;
}
- (void)setBaseWritingDirection:(UITextWritingDirection)writingDirection forRange:(UITextRange *)range;
{
}

#pragma mark Geometry

/* Geometry used to provide, for example, a correction rect. */
- (CGRect)firstRectForRange:(UITextRange *)range;
{
    return CGRectNull;
}
- (CGRect)caretRectForPosition:(UITextPosition *)position;
{
    return CGRectZero;//caretRect_;
}

#pragma mark Hit testing

/* JS - Find the closest position to a given point */
- (UITextPosition *)closestPositionToPoint:(CGPoint)point;
{
    return nil;
}
- (UITextPosition *)closestPositionToPoint:(CGPoint)point withinRange:(UITextRange *)range;
{
    return nil;
}
- (UITextRange *)characterRangeAtPoint:(CGPoint)point;
{
    return nil;
}

- (NSArray *)selectionRectsForRange:(UITextRange *)range
{
    return nil;
}

#pragma mark - UIKeyboard notification

- (void)onUIKeyboardNotification:(NSNotification *)notif;
{
//    NSString * type = notif.name;
//    
//    NSDictionary* info = [notif userInfo];
//    CGRect begin = [self convertRect:
//                    [[info objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue]
//                            fromView:self];
//    CGRect end = [self convertRect:
//                  [[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue]
//                          fromView:self];
//    double aniDuration = [[info objectForKey:UIKeyboardAnimationDurationUserInfoKey] doubleValue];
//    
//    CGSize viewSize = self.frame.size;
//    
//#if defined(CC_TARGET_OS_TVOS)
//    // statusBarOrientation not defined on tvOS, and also, orientation makes
//    // no sense on tvOS
//    begin.origin.y = viewSize.height - begin.origin.y - begin.size.height;
//    end.origin.y = viewSize.height - end.origin.y - end.size.height;
//#else
//    CGFloat tmp;
//    switch (getFixedOrientation([[UIApplication sharedApplication] statusBarOrientation]))
//    {
//        case UIInterfaceOrientationPortrait:
//            begin.origin.y = viewSize.height - begin.origin.y - begin.size.height;
//            end.origin.y = viewSize.height - end.origin.y - end.size.height;
//            break;
//            
//        case UIInterfaceOrientationPortraitUpsideDown:
//            begin.origin.x = viewSize.width - (begin.origin.x + begin.size.width);
//            end.origin.x = viewSize.width - (end.origin.x + end.size.width);
//            break;
//            
//        case UIInterfaceOrientationLandscapeLeft:
//            std::swap(begin.size.width, begin.size.height);
//            std::swap(end.size.width, end.size.height);
//            std::swap(viewSize.width, viewSize.height);
//            
//            tmp = begin.origin.x;
//            begin.origin.x = begin.origin.y;
//            begin.origin.y = viewSize.height - tmp - begin.size.height;
//            tmp = end.origin.x;
//            end.origin.x = end.origin.y;
//            end.origin.y = viewSize.height - tmp - end.size.height;
//            break;
//            
//        case UIInterfaceOrientationLandscapeRight:
//            std::swap(begin.size.width, begin.size.height);
//            std::swap(end.size.width, end.size.height);
//            std::swap(viewSize.width, viewSize.height);
//            
//            tmp = begin.origin.x;
//            begin.origin.x = begin.origin.y;
//            begin.origin.y = tmp;
//            tmp = end.origin.x;
//            end.origin.x = end.origin.y;
//            end.origin.y = tmp;
//            break;
//            
//        default:
//            break;
//    }
//#endif
//    
//    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
//    float scaleX = glview->getScaleX();
//    float scaleY = glview->getScaleY();
//    
//    
//    
//    // Convert to pixel coordinate
//    begin = CGRectApplyAffineTransform(begin, CGAffineTransformScale(CGAffineTransformIdentity, self.contentScaleFactor, self.contentScaleFactor));
//    end = CGRectApplyAffineTransform(end, CGAffineTransformScale(CGAffineTransformIdentity, self.contentScaleFactor, self.contentScaleFactor));
//    
//    float offestY = glview->getViewPortRect().origin.y;
//    CCLOG("offestY = %f", offestY);
//    if (offestY < 0.0f)
//    {
//        begin.origin.y += offestY;
//        begin.size.height -= offestY;
//        end.size.height -= offestY;
//    }
//    
//    // Convert to desigin coordinate
//    begin = CGRectApplyAffineTransform(begin, CGAffineTransformScale(CGAffineTransformIdentity, 1.0f/scaleX, 1.0f/scaleY));
//    end = CGRectApplyAffineTransform(end, CGAffineTransformScale(CGAffineTransformIdentity, 1.0f/scaleX, 1.0f/scaleY));
//    
//    
//    cocos2d::IMEKeyboardNotificationInfo notiInfo;
//    notiInfo.begin = cocos2d::Rect(begin.origin.x,
//                                   begin.origin.y,
//                                   begin.size.width,
//                                   begin.size.height);
//    notiInfo.end = cocos2d::Rect(end.origin.x,
//                                 end.origin.y,
//                                 end.size.width,
//                                 end.size.height);
//    notiInfo.duration = (float)aniDuration;
//    
//    cocos2d::IMEDispatcher* dispatcher = cocos2d::IMEDispatcher::sharedDispatcher();
//    if (UIKeyboardWillShowNotification == type)
//    {
//        self.keyboardShowNotification = notif; // implicit copy
//        dispatcher->dispatchKeyboardWillShow(notiInfo);
//    }
//    else if (UIKeyboardDidShowNotification == type)
//    {
//        //CGSize screenSize = self.window.screen.bounds.size;
//        dispatcher->dispatchKeyboardDidShow(notiInfo);
//        caretRect_ = end;
//        
//#if defined(CC_TARGET_OS_TVOS)
//        // smallSystemFontSize not available on TVOS
//        int fontSize = 12;
//#else
//        int fontSize = [UIFont smallSystemFontSize];
//#endif
//        caretRect_.origin.y = viewSize.height - (caretRect_.origin.y + caretRect_.size.height + fontSize);
//        caretRect_.size.height = 0;
//        isKeyboardShown_ = YES;
//    }
//    else if (UIKeyboardWillHideNotification == type)
//    {
//        dispatcher->dispatchKeyboardWillHide(notiInfo);
//    }
//    else if (UIKeyboardDidHideNotification == type)
//    {
//        caretRect_ = CGRectZero;
//        dispatcher->dispatchKeyboardDidHide(notiInfo);
//        isKeyboardShown_ = NO;
//    }
}

#if !defined(CC_TARGET_OS_TVOS)
UIInterfaceOrientation getFixedOrientation(UIInterfaceOrientation statusBarOrientation)
{
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
    {
        statusBarOrientation = UIInterfaceOrientationPortrait;
    }
    return statusBarOrientation;
}
#endif

-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)dis
{
//    [UIView beginAnimations:nil context:nil];
//    [UIView setAnimationDelegate:self];
//    [UIView setAnimationDuration:duration];
//    [UIView setAnimationBeginsFromCurrentState:YES];
//    
//    //NSLog(@"[animation] dis = %f, scale = %f \n", dis, cocos2d::GLView::getInstance()->getScaleY());
//    
//    if (dis < 0.0f) dis = 0.0f;
//    
//    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
//    dis *= glview->getScaleY();
//    
//    dis /= self.contentScaleFactor;
//    
//#if defined(CC_TARGET_OS_TVOS)
//    self.frame = CGRectMake(originalRect_.origin.x, originalRect_.origin.y - dis, originalRect_.size.width, originalRect_.size.height);
//#else
//    switch (getFixedOrientation([[UIApplication sharedApplication] statusBarOrientation]))
//    {
//        case UIInterfaceOrientationPortrait:
//            self.frame = CGRectMake(originalRect_.origin.x, originalRect_.origin.y - dis, originalRect_.size.width, originalRect_.size.height);
//            break;
//            
//        case UIInterfaceOrientationPortraitUpsideDown:
//            self.frame = CGRectMake(originalRect_.origin.x, originalRect_.origin.y + dis, originalRect_.size.width, originalRect_.size.height);
//            break;
//            
//        case UIInterfaceOrientationLandscapeLeft:
//            self.frame = CGRectMake(originalRect_.origin.x - dis, originalRect_.origin.y , originalRect_.size.width, originalRect_.size.height);
//            break;
//            
//        case UIInterfaceOrientationLandscapeRight:
//            self.frame = CGRectMake(originalRect_.origin.x + dis, originalRect_.origin.y , originalRect_.size.width, originalRect_.size.height);
//            break;
//            
//        default:
//            break;
//    }
//#endif
    
    [UIView commitAnimations];
}


-(void) doAnimationWhenAnotherEditBeClicked
{
//    if (self.keyboardShowNotification != nil)
//    {
//        [[NSNotificationCenter defaultCenter]postNotification:self.keyboardShowNotification];
//    }
}

@end

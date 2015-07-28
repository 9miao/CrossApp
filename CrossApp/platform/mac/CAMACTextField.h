//
//  CAMACTextField.h
//  CrossApp
//
//  Created by qiaoxin on 15/7/27.
//  Copyright (c) 2015年 cocos2d-x. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@protocol CAMACTextFieldDelegate<NSObject>
//UITextInput
- (void)setMarkedText:(NSString *)markedText selectedRange:(NSRange)selectedRange;
- (void)unmarkText;
//UIKeyInput
- (BOOL)hasText;
- (void)insertText:(NSString *)text;
- (void)deleteBackward;

@end
@interface CAMACTextField : NSTextField<NSTextFieldDelegate>
@property(nonatomic,assign)id<CAMACTextFieldDelegate> cadelegate;

@end

//
//  CAIOSTextField.h
//  CrossApp
//
//  Created by lihui on 15/5/4.
//  Copyright (c) 2015年 cocos2d-x. All rights reserved.
//

#import <UIKit/UIKit.h>
@protocol CAIOSTextFieldDelegate<NSObject>
//UITextInput
- (void)setMarkedText:(NSString *)markedText selectedRange:(NSRange)selectedRange;
- (void)unmarkText;
//UIKeyInput
- (BOOL)hasText;
- (void)insertText:(NSString *)text;
- (void)deleteBackward;

@end
@interface CAIOSTextField : UITextField<UITextFieldDelegate>
@property(nonatomic,assign)id<CAIOSTextFieldDelegate> cadelegate;

@end

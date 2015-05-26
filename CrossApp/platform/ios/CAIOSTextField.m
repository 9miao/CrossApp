//
//  CAIOSTextField.m
//  CrossApp
//
//  Created by lihui on 15/5/4.
//  Copyright (c) 2015年 cocos2d-x. All rights reserved.
//

#import "CAIOSTextField.h"

@implementation CAIOSTextField
@synthesize cadelegate = _cadelegate;

-(id)init
{
    if (![super init]) {
        return nil;
    }
    [self setAutocorrectionType:UITextAutocorrectionTypeNo];
    return self;
}

- (void)setMarkedText:(NSString *)markedText selectedRange:(NSRange)selectedRange
{
    
    
    [super setMarkedText:markedText selectedRange:selectedRange];
    [_cadelegate setMarkedText:markedText selectedRange:selectedRange];
}

- (void)unmarkText
{
    [_cadelegate unmarkText];
    [super unmarkText];
    self.text = @""; 
}

- (BOOL)hasText
{
    BOOL ht = [super hasText];
    [_cadelegate hasText];
    return ht;
}

- (void)insertText:(NSString *)text
{
    [_cadelegate insertText:text];
}

- (void)deleteBackward
{
    [_cadelegate deleteBackward];
}

- (BOOL)keyboardInputShouldDelete:(UITextField *)textField {
    BOOL shouldDelete = YES;
    //[self deleteBackward];
    if ([UITextField instancesRespondToSelector:_cmd])
    {
        BOOL (*keyboardInputShouldDelete)(id, SEL, UITextField *) = (BOOL (*)(id, SEL, UITextField *))[UITextField instanceMethodForSelector:_cmd];
        
        if (keyboardInputShouldDelete)
        {
            shouldDelete = keyboardInputShouldDelete(self, _cmd, textField);
        }
    }
    
    BOOL isIos8 = ([[[UIDevice currentDevice] systemVersion] intValue] == 8);
    BOOL isLessThanIos8_3 = ([[[UIDevice currentDevice] systemVersion] floatValue] < 8.3f);
    
    if ( ![textField.text length] && isIos8 && isLessThanIos8_3)
    {
        [self deleteBackward];
    }
    return YES;
}

@end

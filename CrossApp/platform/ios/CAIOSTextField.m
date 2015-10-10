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
   
    if ( [[UIDevice currentDevice].systemVersion floatValue] >= 9.0)
    {
        [self setDelegate:self];
    }
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

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    if ( [[UIDevice currentDevice].systemVersion floatValue] >= 9.0)
    {
        if (string.length > 0)
        {
            if (![[[UITextInputMode currentInputMode] primaryLanguage] isEqualToString: @"en-US"])
            {
                if (self.markedTextRange == nil)
                {
                    const char* cString = [string cStringUsingEncoding:NSUTF8StringEncoding];
                    int ascii = cString[0];
                    if (    ascii < 65
                        || (ascii >= 91 && ascii < 97)
                        || (ascii >= 123 && ascii < 128))
                    {
                        [_cadelegate insertText:string];
                    }
                    
                    //NSLog(@"no en-US");
                }
            }
            else
            {
                [_cadelegate insertText:string];
                //NSLog(@"en-US");
            }
        }
    }
    return YES;
}


- (void)insertText:(NSString *)text
{
    [_cadelegate insertText:text];
}

- (void)deleteBackward
{
    [_cadelegate deleteBackward];
}

- (BOOL)keyboardInputShouldDelete:(UITextField *)textField
{
    BOOL shouldDelete = YES;

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

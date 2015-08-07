//
//  CAMACTextField.m
//  CrossApp
//
//  Created by qiaoxin on 15/7/27.
//  Copyright (c) 2015年 cocos2d-x. All rights reserved.
//

#import "CAMACTextField.h"

@implementation CAMACTextField
@synthesize cadelegate = _cadelegate;

-(id)init
{
    if (![super init]) {
        return nil;
    }
   // [self setAutocorrectionType:UITextAutocorrectionTypeNo];
    return self;
}

- (BOOL)textShouldBeginEditing:(NSText *)textObject
{
    NSLog(@"textShouldBeginEditing:%@",textObject.string);
    return true;
}

- (BOOL)textShouldEndEditing:(NSText *)textObject
{
    NSLog(@"textShouldEndEditing:%@",textObject.string);
    return true;
}

- (void)textDidBeginEditing:(NSNotification *)notification
{
    NSLog(@"textDidBeginEditing:");

}

- (void)textDidEndEditing:(NSNotification *)notification
{
   // NSLog(@"textDidEndEditing:");
}

- (void)textDidChange:(NSNotification *)notification
{
    NSLog(@"selectable:::a%hhd", self.selectable);
    
    NSLog(@"did change:%@",self.stringValue);
    [_cadelegate insertText:self.stringValue];
}

- (void)setMarkedText:(NSString *)markedText selectedRange:(NSRange)selectedRange
{
    //[super setMarkedText:markedText selectedRange:selectedRange];
    [_cadelegate setMarkedText:markedText selectedRange:selectedRange];
}

- (void)unmarkText
{
    [_cadelegate unmarkText];
  //  [super unmarkText];
    [self setTitleWithMnemonic:@""];
   // self.text = @"";
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

- (void)keyDown:(NSEvent *)theEvent
{

}
//
//- (BOOL)keyboardInputShouldDelete:(UITextField *)textField {
//    BOOL shouldDelete = YES;
//    //[self deleteBackward];
//    if ([UITextField instancesRespondToSelector:_cmd])
//    {
//        BOOL (*keyboardInputShouldDelete)(id, SEL, UITextField *) = (BOOL (*)(id, SEL, UITextField *))[UITextField instanceMethodForSelector:_cmd];
//        
//        if (keyboardInputShouldDelete)
//        {
//            shouldDelete = keyboardInputShouldDelete(self, _cmd, textField);
//        }
//    }
//    
//    BOOL isIos8 = ([[[UIDevice currentDevice] systemVersion] intValue] == 8);
//    BOOL isLessThanIos8_3 = ([[[UIDevice currentDevice] systemVersion] floatValue] < 8.3f);
//    
//    if ( ![textField.text length] && isIos8 && isLessThanIos8_3)
//    {
//        [self deleteBackward];
//    }
//    return YES;
//}

@end

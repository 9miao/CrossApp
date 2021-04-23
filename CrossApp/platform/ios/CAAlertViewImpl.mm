

#include "../CAAlertViewImpl.h"
#import <UIKit/UIKit.h>
#import <EAGLView.h>
@interface __alertView_callback: NSObject
{
    std::function<void(int)> _callback;
}

- (id)init:(std::function<void(int)>)callback;

- (void)clickedButtonAtIndex:(NSInteger)buttonIndex;

@end

@implementation __alertView_callback


- (id)init:(std::function<void(int)>)callback
{
    if ([super init] == nil)
    {
        return nil;
    }
    _callback = callback;
    
    return self;
}

- (void)clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (_callback != nullptr)
    {
        _callback((int)buttonIndex);
    }
    
    [self release];
}

@end

NS_CC_BEGIN


void __show_alertView(const std::string& title, const std::string& message, const std::vector<std::string>& buttonTitles, const std::function<void(int)>& callback)
{
    
    NSString * t = (!title.empty()) ? [NSString stringWithUTF8String : title.c_str()] : nil;
    NSString * m = (!message.empty()) ? [NSString stringWithUTF8String : message.c_str()] : nil;
    
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:t
                                                                             message:m
                                                                      preferredStyle:UIAlertControllerStyleAlert];
    
    __alertView_callback* c = [[__alertView_callback alloc] init:callback];
    
    int index = 0;
    for (auto& buttonTitle : buttonTitles)
    {
        NSString* title = [NSString stringWithUTF8String:buttonTitle.c_str()];
        [alertController addAction:[UIAlertAction actionWithTitle:title style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            [c clickedButtonAtIndex:index];
        }]];
        ++index;
    }
    
    UIWindow * window = [UIApplication sharedApplication].keyWindow;
    [window.rootViewController presentViewController:alertController animated:YES completion:nil];
}

NS_CC_END



#include "../CAAlertViewImpl.h"
#import <Cocoa/Cocoa.h>
#import "EAGLView.h"

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
    
    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert setMessageText:t];
    [alert setInformativeText:m];
    [alert setAlertStyle:NSInformationalAlertStyle];
    
    for (auto& buttonTitle : buttonTitles)
    {
        [alert addButtonWithTitle:[NSString stringWithUTF8String:buttonTitle.c_str()]];
    }
    
    NSWindow *window = [[EAGLView sharedEGLView] window];
    
    __alertView_callback* delegate = [[__alertView_callback alloc] init:callback];
    
    [alert beginSheetModalForWindow:window completionHandler:^(NSModalResponse returnCode)
    {
        int index = ((int)returnCode) - ((int)NSAlertFirstButtonReturn);
        [delegate clickedButtonAtIndex:index];
    }];
}

NS_CC_END

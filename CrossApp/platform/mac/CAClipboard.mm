
#include "platform/CAClipboard.h"
#include "basics/CAApplication.h"
#import <Cocoa/Cocoa.h>

NS_CC_BEGIN


std::string CAClipboard::getText()
{
    
    NSPasteboard *pastboard = [NSPasteboard generalPasteboard];
    if ([[pastboard types] containsObject:NSPasteboardTypeString]) {
        
        NSString *s = [pastboard stringForType:NSPasteboardTypeString];

        return [s cStringUsingEncoding:NSUTF8StringEncoding];
    }
    else
    {
        return "";
    }
}

void CAClipboard::setText(const std::string& cszStrText)
{
    NSPasteboard *pastboard = [NSPasteboard generalPasteboard];
    [pastboard clearContents];
    [pastboard setString:[NSString stringWithUTF8String:cszStrText.c_str()] forType:NSPasteboardTypeString];
}

NS_CC_END
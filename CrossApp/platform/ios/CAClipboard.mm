
#include "platform/CAClipboard.h"
#include "basics/CAApplication.h"
#import <UIKit/UIKit.h>

NS_CC_BEGIN


std::string CAClipboard::getText()
{
	return [[UIPasteboard generalPasteboard].string cStringUsingEncoding:NSUTF8StringEncoding];
}

void CAClipboard::setText(const std::string& cszStrText)
{
	UIPasteboard *pastBoard = [UIPasteboard generalPasteboard];
	pastBoard.string = [NSString stringWithUTF8String:cszStrText.c_str()];
}

NS_CC_END
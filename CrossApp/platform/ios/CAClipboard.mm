
#include "platform/CAClipboard.h"
#include "basics/CAApplication.h"


NS_CC_BEGIN


std::string CAClipboard::GetText()
{
	return [[UIPasteboard generalPasteboard].string cStringUsingEncoding:NSUTF8StringEncoding];
}

void CAClipboard::SetText(const std::string& cszStrText)
{
	UIPasteboard *pastBoard = [UIPasteboard generalPasteboard];
	pastBoard.string = [NSString stringWithUTF8String:str];
}

NS_CC_END

#include <Cocoa/Cocoa.h>

#include "platform/CCCommon.h"

#include <stdarg.h>
#include <stdio.h>
#import "EAGLView.h"


NS_CC_BEGIN

void CCLog(const char * pszFormat, ...)
{
    printf("CrossApp: ");
    char szBuf[kMaxLogLen];

    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");
    fflush(stdout);
}

void CCLuaLog(const char * pszFormat)
{
    puts(pszFormat);
}

// ios no MessageBox, use CCLog instead
void CCMessageBox(const char * pszMsg, const char * pszTitle)
{
    NSString * title = (pszTitle) ? [NSString stringWithUTF8String : pszTitle] : nil;
    NSString * msg = (pszMsg) ? [NSString stringWithUTF8String : pszMsg] : nil;

	NSAlert *alert = [[[NSAlert alloc] init] autorelease];
	[alert addButtonWithTitle:@"OK"];
	[alert setMessageText:msg];
	[alert setInformativeText:title];
	[alert setAlertStyle:NSWarningAlertStyle];

	NSWindow *window = [[EAGLView sharedEGLView] window];
	[alert beginSheetModalForWindow:window
					  modalDelegate:[window delegate]
					 didEndSelector:nil
						contextInfo:nil];
}

NS_CC_END

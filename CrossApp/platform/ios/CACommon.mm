

#include "CACommon.h"

#include <stdarg.h>
#include <stdio.h>

#import <UIKit/UIAlert.h>

NS_CC_BEGIN

void CCLog(const char * pszFormat, ...)
{
    printf("CrossApp: ");
    char szBuf[kMaxLogLen+1] = {0};
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");
}

// ios no MessageBox, use CCLog instead
void CAMessageBox(const char * pszTitle, const char * pszMsg)
{
    NSString * title = (pszTitle) ? [NSString stringWithUTF8String : pszTitle] : nil;
    NSString * msg = (pszMsg) ? [NSString stringWithUTF8String : pszMsg] : nil;
    UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: title
                                                          message: msg
                                                         delegate: nil
                                                cancelButtonTitle: @"OK"
                                                otherButtonTitles: nil];
    [messageBox autorelease];
    [messageBox show];
}

NS_CC_END

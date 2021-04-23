
#include "platform/CACommon.h"
#include <stdarg.h>
#include <stdio.h>
#include "basics/CAApplication.h"
#include "basics/CANotificationCenter.h"
#include "basics/CAValue.h"
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
    
    if (CAApplication* application = CAApplication::getApplication())
    {
        if (application->isCrossAppCCLogNotification() == true)
        {
            [[NSNotificationCenter defaultCenter] postNotificationName:[NSString stringWithUTF8String:CROSSAPP_CCLOG_NOTIFICATION] object:[NSString stringWithUTF8String:szBuf]];
        }
    }
}

NS_CC_END

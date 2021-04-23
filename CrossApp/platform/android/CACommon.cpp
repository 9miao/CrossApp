

#include "platform/CACommon.h"
#include "jni/Java_org_CrossApp_lib_CrossAppHelper.h"
#include <android/log.h>
#include <stdio.h>
#include <jni.h>

NS_CC_BEGIN

#define MAX_LEN         (CrossApp::kMaxLogLen + 1)

void CCLog(const char * pszFormat, ...)
{
    char buf[MAX_LEN];

    va_list args;
    va_start(args, pszFormat);
    vsnprintf(buf, MAX_LEN, pszFormat, args);
    va_end(args);

    //__android_log_print(ANDROID_LOG_ERROR, "CrossApp debug info", "%s", buf);
    __android_log_print(ANDROID_LOG_DEBUG, "CrossApp debug info", "%s", buf);
}

NS_CC_END

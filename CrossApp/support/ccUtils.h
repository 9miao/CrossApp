
#ifndef __SUPPORT_CC_UTILS_H__
#define __SUPPORT_CC_UTILS_H__

/** @file ccUtils.h
Misc free functions
*/
#include "platform/CCPlatformMacros.h"
#include <vector>
#include <string>

namespace CrossApp {
/*
ccNextPOT function is licensed under the same license that is used in CAImage.m.
*/

/** returns the Next Power of Two value.

Examples:
- If "value" is 15, it will return 16.
- If "value" is 16, it will return 16.
- If "value" is 17, it will return 32.

@since v0.99.5
*/

CC_DLL unsigned long ccNextPOT(unsigned long value);

CC_DLL std::vector<std::string> Parse2StrVector(const std::string& str, const std::string& strDelimiter, bool bIncludeEmptyStr = false);

CC_DLL unsigned int hex2Int(const std::string& hexStr);
    
CC_DLL std::string int2hex(int i);
}

#endif // __SUPPORT_CC_UTILS_H__

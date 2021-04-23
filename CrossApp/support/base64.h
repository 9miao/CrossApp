

#ifndef __SUPPORT_BASE64_H__
#define __SUPPORT_BASE64_H__

#include "platform/CCPlatformMacros.h"
#include <string>

namespace CrossApp
{

    static inline bool isBase64(char c)
    {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
    
    std::string CC_DLL base64Decode(const std::string& string);

    std::string CC_DLL base64Encode(const std::string& string);
    
}//namespace   CrossApp 


#endif // __SUPPORT_BASE64_H__



#ifndef __SUPPORT_BASE64_H__
#define __SUPPORT_BASE64_H__

#include "platform/CCPlatformMacros.h"
#include <string>

namespace CrossApp
{

    static inline bool isBase64(unsigned char c)
    {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
    
    CC_DLL std::string base64Decode(std::string const& encoded_string);
    // returns a newly allocated array - of size "resultSize" - that
    // the caller is responsible for delete[]ing.
    
	CC_DLL std::string base64Encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    // returns a 0-terminated string that
    // the caller is responsible for delete[]ing.
    
}//namespace   CrossApp 


#endif // __SUPPORT_BASE64_H__

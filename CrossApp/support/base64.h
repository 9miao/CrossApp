

#ifndef __SUPPORT_BASE64_H__
#define __SUPPORT_BASE64_H__

#include "platform/CCPlatformMacros.h"
#include <string>
#ifdef __cplusplus
extern "C" {
#endif    
    

namespace CrossApp
{

    static inline bool isBase64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
    
    CC_DLL unsigned char* base64Decode(std::string const& encoded_string);
    // returns a newly allocated array - of size "resultSize" - that
    // the caller is responsible for delete[]ing.
    
	CC_DLL char* base64Encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    // returns a 0-terminated string that
    // the caller is responsible for delete[]ing.
    
}//namespace   CrossApp 

#ifdef __cplusplus
}
#endif    

#endif // __SUPPORT_BASE64_H__

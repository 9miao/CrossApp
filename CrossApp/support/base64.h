

#ifndef __SUPPORT_BASE64_H__
#define __SUPPORT_BASE64_H__

#ifdef __cplusplus
extern "C" {
#endif    
    
namespace CrossApp
{

    unsigned char* base64Decode(char* in, unsigned int& resultSize, bool trimTrailingZeros = true);
    // returns a newly allocated array - of size "resultSize" - that
    // the caller is responsible for delete[]ing.
    
    char* base64Encode(char const* orig, unsigned origLength);
    // returns a 0-terminated string that
    // the caller is responsible for delete[]ing.
    
}//namespace   CrossApp 

#ifdef __cplusplus
}
#endif    

#endif // __SUPPORT_BASE64_H__

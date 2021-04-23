

#ifndef CrossApp_ccTypeInfo_h
#define CrossApp_ccTypeInfo_h

#include "platform/CCPlatformMacros.h"
#include <stdarg.h>
#include <stdio.h>
#include <typeinfo>
#include <ctype.h>
#include <string.h>

NS_CC_BEGIN

class TypeInfo
{
public:
	virtual long getClassTypeInfo() = 0;
};

static inline unsigned int getHashCodeByString(const char *key)
{
	unsigned int len = (unsigned int)strlen(key);
	const char *end=key+len;
	unsigned int hash;

	for (hash = 0; key < end; key++)
	{
		hash *= 16777619;
		hash ^= (unsigned int) (unsigned char) toupper(*key);
	}
	return (hash);
}

static std::string crossapp_format_string(const char* format, ...)
{
    std::string str;
    va_list ap;
    va_start(ap, format);
    size_t length = 102400;
    char* pBuf = (char*)malloc(length);
    if (pBuf != NULL)
    {
        vsnprintf(pBuf, length, format, ap);
        str = pBuf;
        free(pBuf);
    }
    va_end(ap);
    return str;
}

NS_CC_END

#endif

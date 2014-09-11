
#ifndef __CCSTRING_H__
#define __CCSTRING_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#include <string.h>
#endif

#include <stdarg.h>
#include <string>
#include <functional>
#include "basics/CAObject.h"

NS_CC_BEGIN

class CC_DLL CCString : public CAObject
{
public:

    CCString();

    CCString(const char* str);

    CCString(const std::string& str);

    CCString(const CCString& str);

    virtual ~CCString();

    CCString& operator= (const CCString& other);

    bool initWithFormat(const char* format, ...) CC_FORMAT_PRINTF(2, 3);

    int intValue() const;

    unsigned int uintValue() const;

    float floatValue() const;

    double doubleValue() const;

    bool boolValue() const;

    const char* getCString() const;

    unsigned int length() const;

    int compare(const char *) const;

    virtual CAObject* copyWithZone(CAZone* pZone);
    
    virtual bool isEqual(const CAObject* pObject);

    static CCString* create(const std::string& str);

    static CCString* createWithFormat(const char* format, ...) CC_FORMAT_PRINTF(1, 2);

    static CCString* createWithData(const unsigned char* pData, unsigned long nLen);

    static CCString* createWithContentsOfFile(const char* pszFileName);

private:

    /** only for internal use */
    bool initWithFormatAndValist(const char* format, va_list ap);

public:
    std::string m_sString;
};

struct CCStringCompare : public std::binary_function<CCString *, CCString *, bool> {
    public:
        bool operator() (CCString * a, CCString * b) const {
            return strcmp(a->getCString(), b->getCString()) < 0;
        }
};

#define CCStringMake(str) CCString::create(str)
#define ccs               CCStringMake

// end of data_structure group
/// @}

NS_CC_END

#endif //__CCSTRING_H__

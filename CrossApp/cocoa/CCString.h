/****************************************************************************
Copyright (c) 2010 CrossApp.org

http://9miao.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
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

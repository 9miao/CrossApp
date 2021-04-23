

#ifndef __PLATFORM_IPHONE_PLATFORM_CCLOCK_H__
#define __PLATFORM_IPHONE_PLATFORM_CCLOCK_H__

#include "CCPlatformMacros.h"

NS_CC_BEGIN

class CCLock
{
public:
    CCLock(void);
    ~CCLock(void);

    void lock(void);
    void unlock(void);
};

NS_CC_END

#endif // __PLATFORM_IPHONE_PLATFORM_CCLOCK_H__

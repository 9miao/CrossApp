

#ifndef __CC_PLATFORM_THREAD_H__
#define __CC_PLATFORM_THREAD_H__

#include "CCCommon.h"
#include "CCPlatformMacros.h"

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/* On iOS, should create autorelease pool when create a new thread
 * and release it when the thread end.
 * @js NA
 * @lua NA
 */
class CC_DLL CCThread
{
public:
    CCThread() : m_pAutoreasePool(0) {}
    ~CCThread();

    void createAutoreleasePool();

private:
    void *m_pAutoreasePool;
};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_PLATFORM_THREAD_H__


#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "CCPlatformMacros.h"

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

struct CC_DLL cc_timeval
{
#ifdef __native_client__
    time_t    tv_sec;        // seconds
#else
    long    tv_sec;        // seconds
#endif
    int tv_usec;    // microSeconds
};

class CC_DLL CCTime
{
public:
    static int gettimeofdayCrossApp(struct cc_timeval *tp, void *tzp);
    static double timersubCrossApp(struct cc_timeval *start, struct cc_timeval *end);
};

// end of platform group
/// @}

NS_CC_END

#endif // __PLATFORM_H__

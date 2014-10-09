

#include "platform.h"

#include "CCStdC.h"

NS_CC_BEGIN

int CCTime::gettimeofdayCrossApp(struct cc_timeval *tp, void *tzp)
{
    CC_UNUSED_PARAM(tzp);
    if (tp)
    {
        gettimeofday((struct timeval *)tp,  0);
    }
    return 0;
}

double CCTime::timersubCrossApp(struct cc_timeval *start, struct cc_timeval *end)
{
    if (! start || ! end)
    {
        return 0;
    }
    
    return ((end->tv_sec*1000.0+end->tv_usec/1000.0) - (start->tv_sec*1000.0+start->tv_usec/1000.0));
}

NS_CC_END

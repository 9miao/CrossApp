
#include "CCProfiling.h"

using namespace std;

NS_CC_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
//#pragma mark - Profiling Categories
#endif

/* set to NO the categories that you don't want to profile */
bool kCCProfilerCategorySprite = false;
bool kCCProfilerCategoryBatchSprite = false;
bool kCCProfilerCategoryParticles = false;


static CCProfiler* g_sSharedProfiler = NULL;

CCProfiler* CCProfiler::sharedProfiler(void)
{
    if (! g_sSharedProfiler)
    {
        g_sSharedProfiler = new CCProfiler();
        g_sSharedProfiler->init();
    }

    return g_sSharedProfiler;
}

CCProfilingTimer* CCProfiler::createAndAddTimerWithName(const char* timerName)
{
    CCProfilingTimer *t = new CCProfilingTimer();
    t->initWithName(timerName);
    m_mActiveTimers.insert(timerName, t);
    t->release();

    return t;
}

void CCProfiler::releaseTimer(const char* timerName)
{
    m_mActiveTimers.erase(timerName);
}

void CCProfiler::releaseAllTimers()
{
    m_mActiveTimers.clear();
}

bool CCProfiler::init()
{
    return true;
}

CCProfiler::~CCProfiler(void)
{
    m_mActiveTimers.clear();
}

void CCProfiler::displayTimers()
{
    CAMap<std::string, CCProfilingTimer*>::iterator itr;
    for (itr=m_mActiveTimers.begin(); itr!=m_mActiveTimers.end(); itr++)
    {
        CCProfilingTimer* timer = itr->second;
        CCLog("%s", timer->description());
    }
}

// implementation of CCProfilingTimer

bool CCProfilingTimer::initWithName(const char* timerName)
{
    m_NameStr = timerName;
    return true;
}

CCProfilingTimer::CCProfilingTimer()
: numberOfCalls(0)
, m_dAverageTime1(0)
, m_dAverageTime2(0)
, totalTime(0)
, minTime(100000000)
, maxTime(0)
{}


CCProfilingTimer::~CCProfilingTimer(void)
{
    
}

const char* CCProfilingTimer::description()
{
    static char s_desciption[512] = {0};
    
    sprintf(s_desciption, "%s ::\tavg1: %dµ,\tavg2: %dµ,\tmin: %dµ,\tmax: %dµ,\ttotal: %.2fs,\tnr calls: %d", m_NameStr.c_str(), m_dAverageTime1, m_dAverageTime2, minTime, maxTime, totalTime/1000000., numberOfCalls);
    return s_desciption;
}

void CCProfilingTimer::reset()
{
    numberOfCalls = 0;
    m_dAverageTime1 = 0;
    m_dAverageTime2 = 0;
    totalTime = 0;
    minTime = 100000000;
    maxTime = 0;
    gettimeofday((struct timeval*)&m_sStartTime, NULL);
}

void CCProfilingBeginTimingBlock(const char *timerName)
{
    CCProfiler* p = CCProfiler::sharedProfiler();
    CCProfilingTimer* timer = p->m_mActiveTimers.getValue(timerName);
    if( ! timer )
    {
        timer = p->createAndAddTimerWithName(timerName);
    }

    // must the be last thing to execute
    timer->numberOfCalls++;
    gettimeofday((struct timeval*)&timer->m_sStartTime, NULL);
}

void CCProfilingEndTimingBlock(const char *timerName)
{
    // must the be 1st thing to execute
    struct timeval now;
    gettimeofday( &now, NULL);

    CCProfiler* p = CCProfiler::sharedProfiler();
    CCProfilingTimer* timer = p->m_mActiveTimers.getValue(timerName);

    CCAssert(timer, "CCProfilingTimer  not found");
    
    int duration = 1000000 * (now.tv_sec - timer->m_sStartTime.tv_sec) + (now.tv_usec - timer->m_sStartTime.tv_usec);

    timer->totalTime += duration;
    timer->m_dAverageTime1 = (timer->m_dAverageTime1 + duration) / 2.0f;
    timer->m_dAverageTime2 = timer->totalTime / timer->numberOfCalls;
    timer->maxTime = MAX( timer->maxTime, duration);
    timer->minTime = MIN( timer->minTime, duration);
}

void CCProfilingResetTimingBlock(const char *timerName)
{
    CCProfiler* p = CCProfiler::sharedProfiler();
    CCProfilingTimer *timer = p->m_mActiveTimers.getValue(timerName);

    CCAssert(timer, "CCProfilingTimer not found");

    timer->reset();
}

NS_CC_END


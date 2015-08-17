
#ifndef __SUPPORT_CCPROFILING_H__
#define __SUPPORT_CCPROFILING_H__

#include "ccConfig.h"
#include "basics/CAObject.h"
#include "platform/platform.h"
#include "basics/CASTLContainer.h"
#include <string>

NS_CC_BEGIN

/**
 * @addtogroup global
 * @{
 */

class CCProfilingTimer;

/** CCProfiler
 CrossApp builtin profiler.

 To use it, enable set the CC_ENABLE_PROFILERS=1 in the ccConfig.h file
 *@js NA
 *@lua NA
 */

class CC_DLL CCProfiler : public CAObject
{
public:
    ~CCProfiler(void);
    /** display the timers */
    void displayTimers(void);
    bool init(void);

public:
    static CCProfiler* sharedProfiler(void);
    /** Creates and adds a new timer */
    CCProfilingTimer* createAndAddTimerWithName(const char* timerName);
    /** releases a timer */
    void releaseTimer(const char* timerName);
    /** releases all timers */
    void releaseAllTimers();

    CAMap<std::string, CCProfilingTimer*> m_mActiveTimers;
};
/**
 *@js NA
 *@lua NA
 */
class CCProfilingTimer : public CAObject
{
public:
    CCProfilingTimer();
    ~CCProfilingTimer();
    bool initWithName(const char* timerName);
    const char* description(void);
    struct cc_timeval* getStartTime(void) { return &m_sStartTime; };
    /** resets the timer properties */
    void reset();

    std::string m_NameStr;
    int               numberOfCalls;
    int               m_dAverageTime1;
    int               m_dAverageTime2;
    long long         totalTime;
    int               minTime;
    int               maxTime;
    struct cc_timeval m_sStartTime;
};

extern CC_DLL void CCProfilingBeginTimingBlock(const char *timerName);
extern CC_DLL void CCProfilingEndTimingBlock(const char *timerName);
extern CC_DLL void CCProfilingResetTimingBlock(const char *timerName);

/*
 * CrossApp profiling categories
 * used to enable / disable profilers with granularity
 */

extern bool kCCProfilerCategorySprite;
extern bool kCCProfilerCategoryBatchSprite;
extern bool kCCProfilerCategoryParticles;

// end of global group
/// @}

NS_CC_END

#endif // __SUPPORT_CCPROFILING_H__

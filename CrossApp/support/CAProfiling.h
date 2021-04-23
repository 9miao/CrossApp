

#ifndef __SUPPORT_CCPROFILING_H__
#define __SUPPORT_CCPROFILING_H__
/// @cond DO_NOT_SHOW

#include <string>
#include <chrono>
#include "ccConfig.h"
#include "basics/CAObject.h"
#include "platform/platform.h"
#include "basics/CASTLContainer.h"

NS_CC_BEGIN

/**
 * @addtogroup global
 * @{
 */

class CAProfilingTimer;

/** CAProfiler
 cross builtin CAProfiler.
 
 To use it, enable set the CC_ENABLE_CAProfilerS=1 in the ccConfig.h file
 */

class CC_DLL CAProfiler : public CAObject
{
public:
    /**
     * @js NA
     * @lua NA
     */
    ~CAProfiler(void);
    /** display the timers
     * @js NA
     * @lua NA
     */
    void displayTimers(void);
    /**
     * @js NA
     * @lua NA
     */
    bool init(void);
    
public:
    /** returns the singleton
     * @js NA
     * @lua NA
     */
    static CAProfiler* getInstance(void);
    
    /** Creates and adds a new timer
     * @js NA
     * @lua NA
     */
    CAProfilingTimer* createAndAddTimerWithName(const char* timerName);
    /** releases a timer
     * @js NA
     * @lua NA
     */
    void releaseTimer(const char* timerName);
    /** releases all timers
     * @js NA
     * @lua NA
     */
    void releaseAllTimers();
    
    CAMap<std::string, CAProfilingTimer*> _activeTimers;
};

class CAProfilingTimer : public CAObject
{
public:
    /**
     * @js NA
     * @lua NA
     */
    CAProfilingTimer();
    /**
     * @js NA
     * @lua NA
     */
    ~CAProfilingTimer(void);
    /**
     * @js NA
     * @lua NA
     */
    bool initWithName(const char* timerName);
    /**
     * @js NA
     * @lua NA
     */
    virtual std::string getDescription() const;
    /**
     * @js NA
     * @lua NA
     */
    inline const std::chrono::high_resolution_clock::time_point& getStartTime(void) { return _startTime; };
    
    /** resets the timer properties
     * @js NA
     * @lua NA
     */
    void reset();
    
    std::string _nameStr;
    std::chrono::high_resolution_clock::time_point _startTime;
    long _averageTime1;
    long _averageTime2;
    long minTime;
    long maxTime;
    long totalTime;
    long numberOfCalls;
};

extern void CC_DLL ProfilingBeginTimingBlock(const char *timerName);
extern void CC_DLL ProfilingEndTimingBlock(const char *timerName);
extern void CC_DLL ProfilingResetTimingBlock(const char *timerName);

/*
 * cross profiling categories
 * used to enable / disable CAProfilers with granularity
 */

extern bool kCAProfilerCategorySprite;
extern bool kCAProfilerCategoryBatchSprite;
extern bool kCAProfilerCategoryParticles;

// end of global group
/// @}

NS_CC_END

/// @endcond
#endif // __SUPPORT_CCPROFILING_H__

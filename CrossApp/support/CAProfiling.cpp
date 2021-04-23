
#include "support/CAProfiling.h"

using namespace std;

NS_CC_BEGIN

// Profiling Categories
/* set to false the categories that you don't want to profile */
bool kCAProfilerCategorySprite = false;
bool kCAProfilerCategoryBatchSprite = false;
bool kCAProfilerCategoryParticles = false;


static CAProfiler* g_sSharedProfiler = nullptr;

CAProfiler* CAProfiler::getInstance()
{
    if (! g_sSharedProfiler)
    {
        g_sSharedProfiler = new (std::nothrow) CAProfiler();
        g_sSharedProfiler->init();
    }
    
    return g_sSharedProfiler;
}


CAProfilingTimer* CAProfiler::createAndAddTimerWithName(const char* timerName)
{
    CAProfilingTimer *t = new (std::nothrow) CAProfilingTimer();
    t->initWithName(timerName);
    _activeTimers.insert(timerName, t);
    t->release();
    
    return t;
}

void CAProfiler::releaseTimer(const char* timerName)
{
    _activeTimers.erase(timerName);
}

void CAProfiler::releaseAllTimers()
{
    _activeTimers.clear();
}

bool CAProfiler::init()
{
    return true;
}

CAProfiler::~CAProfiler(void)
{
}

void CAProfiler::displayTimers()
{
    for (auto iter = _activeTimers.begin(); iter != _activeTimers.end(); ++iter)
    {
        CAProfilingTimer* timer = iter->second;
        CCLog("%s", timer->getDescription().c_str());
    }
}

// implementation of CAProfilingTimer

CAProfilingTimer::CAProfilingTimer()
: _averageTime1(0)
, _averageTime2(0)
, minTime(100000000)
, maxTime(0)
, totalTime(0)
, numberOfCalls(0)
{
}

bool CAProfilingTimer::initWithName(const char* timerName)
{
    _nameStr = timerName;
    return true;
}

CAProfilingTimer::~CAProfilingTimer(void)
{
    
}

std::string CAProfilingTimer::getDescription() const
{
    static char s_description[512] = {0};
    
    sprintf(s_description, "%s ::\tavg1: %ldu,\tavg2: %ldu,\tmin: %ldu,\tmax: %ldu,\ttotal: %.2fs,\tnr calls: %ld", _nameStr.c_str(), _averageTime1, _averageTime2, minTime, maxTime, totalTime/1000000., numberOfCalls);
    return s_description;
}

void CAProfilingTimer::reset()
{
    numberOfCalls = 0;
    _averageTime1 = 0;
    _averageTime2 = 0;
    totalTime = 0;
    minTime = 100000000;
    maxTime = 0;
    _startTime = chrono::high_resolution_clock::now();
}

void ProfilingBeginTimingBlock(const char *timerName)
{
    CAProfiler* p = CAProfiler::getInstance();
    CAProfilingTimer* timer = p->_activeTimers.at(timerName);
    if( ! timer )
    {
        timer = p->createAndAddTimerWithName(timerName);
    }
    
    timer->numberOfCalls++;
    
    // should be the last instruction in order to be more reliable
    timer->_startTime = chrono::high_resolution_clock::now();
}

void ProfilingEndTimingBlock(const char *timerName)
{
    // should be the 1st instruction in order to be more reliable
    auto now = chrono::high_resolution_clock::now();
    
    CAProfiler* p = CAProfiler::getInstance();
    CAProfilingTimer* timer = p->_activeTimers.at(timerName);
    
    CCASSERT(timer, "CCCAProfilingTimer  not found");
    
    
    long duration = static_cast<long>(chrono::duration_cast<chrono::microseconds>(now - timer->_startTime).count());
    
    timer->totalTime += duration;
    timer->_averageTime1 = (timer->_averageTime1 + duration) / 2.0f;
    timer->_averageTime2 = timer->totalTime / timer->numberOfCalls;
    timer->maxTime = MAX( timer->maxTime, duration);
    timer->minTime = MIN( timer->minTime, duration);
}

void ProfilingResetTimingBlock(const char *timerName)
{
    CAProfiler* p = CAProfiler::getInstance();
    CAProfilingTimer *timer = p->_activeTimers.at(timerName);
    
    CCASSERT(timer, "CCCAProfilingTimer not found");
    
    timer->reset();
}

NS_CC_END


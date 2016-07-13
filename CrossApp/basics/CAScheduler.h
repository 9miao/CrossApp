

#ifndef __CASCHEDULER_H__
#define __CASCHEDULER_H__

#include <functional>
#include <mutex>
#include <set>

#include "basics/CAObject.h"
#include "basics/CASTLContainer.h"
#include "support/data_support/uthash.h"

NS_CC_BEGIN

class CAScheduler;

typedef std::function<void(float)> ccSchedulerFunc;

class CC_DLL Timer : public CAObject
{
protected:
    
    Timer();
public:
    
    inline float getInterval() const { return _interval; };

    inline void setInterval(float interval) { _interval = interval; };
    
    void setupTimerWithInterval(float seconds, unsigned int repeat, float delay);
    
    virtual void trigger(float dt) = 0;
    virtual void cancel() = 0;
    
    void update(float dt);
    
protected:
    
    CAScheduler* _scheduler;
    float _elapsed;
    bool _runForever;
    bool _useDelay;
    unsigned int _timesExecuted;
    unsigned int _repeat;
    float _delay;
    float _interval;
};


class CC_DLL TimerTargetSelector : public Timer
{
public:
    
    TimerTargetSelector();

    bool initWithSelector(CAScheduler* scheduler, SEL_SCHEDULE selector, CAObject* target, float seconds, unsigned int repeat, float delay);
    
    inline SEL_SCHEDULE getSelector() const { return _selector; };
    
    virtual void trigger(float dt);
    virtual void cancel();
    
protected:
    CAObject* _target;
    SEL_SCHEDULE _selector;
};


class CC_DLL TimerTargetCallback : public Timer
{
public:
    
    TimerTargetCallback();
    
    bool initWithCallback(CAScheduler* scheduler, const ccSchedulerFunc& callback, void *target, const std::string& key, float seconds, unsigned int repeat, float delay);
    
    inline const ccSchedulerFunc& getCallback() const { return _callback; };
    inline const std::string& getKey() const { return _key; };
    
    virtual void trigger(float dt);
    virtual void cancel();
    
protected:
    void* _target;
    ccSchedulerFunc _callback;
    std::string _key;
};

#if CC_ENABLE_SCRIPT_BINDING

class CC_DLL TimerScriptHandler : public Timer
{
public:
    bool initWithScriptHandler(int handler, float seconds);
    inline int getScriptHandler() const { return _scriptHandler; };
    
    virtual void trigger(float dt) override;
    virtual void cancel() override;
    
private:
    int _scriptHandler;
};

#endif


struct _listEntry;
struct _hashSelectorEntry;
struct _hashUpdateEntry;

#if CC_ENABLE_SCRIPT_BINDING
class SchedulerScriptHandlerEntry;
#endif

class CC_DLL CAScheduler : public CAObject
{
public:
    
    static void schedule(SEL_SCHEDULE pfnSelector, CAObject *pTarget, float fInterval, unsigned int repeat, float delay, bool bPaused = false);
    
    static void schedule(SEL_SCHEDULE pfnSelector, CAObject *pTarget, float fInterval, bool bPaused = false);
    
    static void unschedule(SEL_SCHEDULE pfnSelector, CAObject *pTarget);
    
    static void unscheduleAllForTarget(CAObject *pTarget);
    
    static void unscheduleAll(void);
    
    static bool isScheduled(SEL_SCHEDULE pfnSelector, CAObject *pTarget);
    
public:

    static const int PRIORITY_SYSTEM;

    static const int PRIORITY_NON_SYSTEM_MIN;

    CAScheduler();

    virtual ~CAScheduler();
    
    static CAScheduler* getScheduler();

    inline float getTimeScale() { return _timeScale; }

    inline void setTimeScale(float timeScale) { _timeScale = timeScale; }

    void update(float dt);

    void scheduleSelector(const ccSchedulerFunc& callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& key);

    void scheduleSelector(const ccSchedulerFunc& callback, void *target, float interval, bool paused, const std::string& key);

    void scheduleSelector(SEL_SCHEDULE selector, CAObject *target, float interval, unsigned int repeat, float delay, bool paused);

    void scheduleSelector(SEL_SCHEDULE selector, CAObject *target, float interval, bool paused);

    void scheduleSelectorUpdate(CAObject *target, int priority, bool paused);
    
#if CC_ENABLE_SCRIPT_BINDING
    unsigned int scheduleScriptFunc(unsigned int handler, float interval, bool paused);
#endif

    void unscheduleSelector(const std::string& key, void *target);

    void unscheduleSelector(SEL_SCHEDULE selector, CAObject *target);

    void unscheduleUpdate(void *target);

    void unscheduleSelectorAllForTarget(void *target);
    
    void unscheduleSelectorAll();

    void unscheduleSelectorAllWithMinPriority(int minPriority);
    
#if CC_ENABLE_SCRIPT_BINDING
    void unscheduleScriptEntry(unsigned int scheduleScriptEntryID);
#endif

    bool isScheduledSelector(const std::string& key, void *target);

    bool isScheduledSelector(SEL_SCHEDULE selector, CAObject *target);

    void pauseTarget(void *target);

    void resumeTarget(void *target);

    bool isTargetPaused(void *target);

    std::set<void*> pauseAllTargets();

    std::set<void*> pauseAllTargetsWithMinPriority(int minPriority);

    void resumeTargets(const std::set<void*>& targetsToResume);

    void performFunctionInUIThread( const std::function<void()> &function);

protected:

    void schedulePerFrame(const ccSchedulerFunc& callback, void *target, int priority, bool paused);
    
    void removeHashElement(struct _hashSelectorEntry *element);
    void removeUpdateFromHash(struct _listEntry *entry);

    void priorityIn(struct _listEntry **list, const ccSchedulerFunc& callback, void *target, int priority, bool paused);
    void appendIn(struct _listEntry **list, const ccSchedulerFunc& callback, void *target, bool paused);
    
    
    float _timeScale;

    struct _listEntry *_updatesNegList;        // list of priority < 0
    struct _listEntry *_updates0List;            // list priority == 0
    struct _listEntry *_updatesPosList;        // list priority > 0
    struct _hashUpdateEntry *_hashForUpdates; // hash used to fetch quickly the list entries for pause,delete,etc

    struct _hashSelectorEntry *_hashForTimers;
    struct _hashSelectorEntry *_currentTarget;
    bool _currentTargetSalvaged;

    bool _updateHashLocked;
    
#if CC_ENABLE_SCRIPT_BINDING
    CAVector<SchedulerScriptHandlerEntry*> _scriptHandlerEntries;
#endif
    
    std::vector<std::function<void()> > _functionsToPerform;
    std::mutex _performMutex;
};

NS_CC_END

#endif // __CASCHEDULER_H__
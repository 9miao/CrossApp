#include "CAScheduler.h"
#include "ccMacros.h"
#include "CAApplication.h"
#include "support/data_support/utlist.h"
#include "support/data_support/ccCArray.h"
#include "cocoa/CCSet.h"
#include "script_support/CAScriptSupport.h"

NS_CC_BEGIN

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

// data structures
// A list double-linked list used for "updates with priority"
typedef struct _listEntry
{
    struct _listEntry   *prev, *next;
    CAScheduler::Callback     callback;
    CAObject            *target;
    int                 priority;
    bool                paused;
    bool                markedForDeletion; // selector will no longer be called and entry will be removed at end of the next tick
} tListEntry;

typedef struct _hashUpdateEntry
{
    tListEntry          **list;        // Which list does it belong to ?
    tListEntry          *entry;        // entry in the list
    CAObject            *target;
    CAScheduler::Callback     callback;
    UT_hash_handle      hh;
} tHashUpdateEntry;

// Hash Element used for "selectors with interval"
typedef struct _hashSelectorEntry
{
    ccArray             *timers;
    CAObject            *target;
    int                 timerIndex;
    Timer               *currentTimer;
    bool                currentTimerSalvaged;
    bool                paused;
    UT_hash_handle      hh;
} tHashTimerEntry;

class CC_DLL TimerTargetSelector : public Timer
{
public:
    TimerTargetSelector();
    
    /** Initializes a timer with a target, a selector and an interval in seconds, repeat in number of times to repeat, delay in seconds. */
    bool initWithSelector(CAScheduler* scheduler, SEL_Schedule selector, CAObject* target, float seconds, unsigned int repeat, float delay);
    
    inline SEL_Schedule getSelector() const { return _selector; };
    
    virtual void trigger(float dt) override;
    virtual void cancel() override;
    
protected:
    CAObject* _target;
    SEL_Schedule _selector;
};

class CC_DLL TimerTargetCallback : public Timer
{
public:
    TimerTargetCallback();
    
    // Initializes a timer with a target, a lambda and an interval in seconds, repeat in number of times to repeat, delay in seconds.
    bool initWithCallback(CAScheduler* scheduler, const CAScheduler::Callback& callback, const std::string& callbackName, CAObject *target, float seconds, unsigned int repeat, float delay);
    
    inline const CAScheduler::Callback& getCallback() const { return _callback; };
    inline const std::string& getCallbackName() const { return _callbackName; };
    
    virtual void trigger(float dt) override;
    virtual void cancel() override;
    
protected:
    CAObject* _target;
    CAScheduler::Callback _callback;
    std::string _callbackName;
};


// implementation Timer

Timer::Timer()
: _scheduler(nullptr)
, _elapsed(-1)
, _runForever(false)
, _useDelay(true)
, _timesExecuted(0)
, _repeat(0)
, _delay(0.0f)
, _interval(0.0f)
{
}

void Timer::setupTimerWithInterval(float seconds, unsigned int repeat, float delay)
{
    _elapsed = -1;
    _interval = MAX(seconds, 1/60.f);
    _useDelay = true;
    _delay = MAX(delay, 1/60.f);
    _repeat = repeat;
    _runForever = (_repeat == kCCRepeatForever) ? true : false;
}

void Timer::update(float dt)
{
    if (_elapsed == -1)
    {
        _elapsed = 0;
        _timesExecuted = 0;
    }
    else
    {
        
        // accumulate elapsed time
        _elapsed += dt;
        
        if (_runForever && !_useDelay)
        {
            if (_interval <= 0.017)
            {
                trigger(MIN(_elapsed, 0.05f));
                _elapsed = 0;
            }
            else if (_elapsed >= _interval)
            {
                trigger(_elapsed);
                _elapsed = 0;
            }
            
        }
        else
        {
            if (_useDelay)
            {
                if (_elapsed >= _delay)
                {
                    _elapsed = _elapsed - _delay;
                    trigger(_interval);
                    _timesExecuted += 1;
                    _useDelay = false;
                }
            }
            else
            {
                if (_interval <= 0.017)
                {
                    trigger(MIN(_elapsed, 0.05f));
                    _elapsed = 0;
                    _timesExecuted += 1;
                }
                else if (_elapsed >= _interval)
                {
                    trigger(_elapsed);
                    _elapsed = 0;
                    _timesExecuted += 1;
                }
            }
            
            if (!_runForever && _timesExecuted > _repeat)
            {
                cancel();
            }
        }
    }
}


// TimerTargetCallback
TimerTargetCallback::TimerTargetCallback()
:_callback(nullptr)
{
}

bool TimerTargetCallback::initWithCallback(CAScheduler* scheduler, const CAScheduler::Callback& callback, const std::string& callbackName, CAObject *target, float seconds, unsigned int repeat, float delay)
{
    _scheduler = scheduler;
    _callback = callback;
    _callbackName = callbackName;
    _target = target;
    setupTimerWithInterval(seconds, repeat, delay);
    return true;
}

void TimerTargetCallback::trigger(float dt)
{
    if (_callback)
    {
        _callback(dt);
    }
}

void TimerTargetCallback::cancel()
{
    _scheduler->unschedule(_callbackName, _target);
}


// TimerTargetSelector
TimerTargetSelector::TimerTargetSelector()
: _target(nullptr)
, _selector(nullptr)
{
}

bool TimerTargetSelector::initWithSelector(CAScheduler* scheduler, SEL_Schedule selector, CAObject* target, float seconds, unsigned int repeat, float delay)
{
    _scheduler = scheduler;
    _target = target;
    _selector = selector;
    setupTimerWithInterval(seconds, repeat, delay);
    return true;
}

void TimerTargetSelector::trigger(float dt)
{
    if (_target && _selector)
    {
        (_target->*_selector)(dt);
    }
}

void TimerTargetSelector::cancel()
{
    _scheduler->unschedule(_selector, _target);
}




// implementation of Scheduler

// Priority level reserved for system services.
const int CAScheduler::PRIORITY_SYSTEM = INT_MIN;

// Minimum priority level for user scheduling.
const int CAScheduler::PRIORITY_NON_SYSTEM_MIN = 0;


CAScheduler::CAScheduler(void)
: _timeScale(1.0f)
, _updatesNegList(nullptr)
, _updates0List(nullptr)
, _updatesPosList(nullptr)
, _hashForUpdates(nullptr)
, _hashForTimers(nullptr)
, _currentTarget(nullptr)
, _currentTargetSalvaged(false)
, _updateHashLocked(false)
{
    // I don't expect to have more than 30 functions to all per frame
    _functionsToPerform.reserve(30);
}

CAScheduler::~CAScheduler(void)
{
    unscheduleAll();
}

CAScheduler* CAScheduler::getScheduler()
{
    return CAApplication::getApplication()->getScheduler();
}

void CAScheduler::removeHashElement(_hashSelectorEntry *element)
{
    ccArrayFree(element->timers);
    HASH_DEL(_hashForTimers, element);
    free(element);
}

void CAScheduler::scheduleOnce(const CAScheduler::Callback& callback, const std::string& callbackName, CAObject *target, float delay, bool paused)
{
    this->schedule(callback, callbackName, target, 0.0f, 0, delay, paused);
}

void CAScheduler::scheduleOnce(SEL_Schedule selector, CAObject *target, float delay, bool paused)
{
    this->schedule(selector, target, 0.0f, 0, delay, paused);
}

void CAScheduler::schedule(const CAScheduler::Callback& callback, const std::string& callbackName, CAObject *target, float interval, bool paused)
{
    this->schedule(callback, callbackName, target, interval, kCCRepeatForever, 0.0f, paused);
}

void CAScheduler::schedule(SEL_Schedule selector, CrossApp::CAObject *target, float interval, bool paused)
{
    this->schedule(selector, target, interval, kCCRepeatForever, 0.0f, paused);
}

void CAScheduler::schedule(const CAScheduler::Callback& callback, const std::string& callbackName, CAObject *target, float interval, unsigned int repeat, float delay, bool paused)
{
    if (callbackName.empty())
    {
        CCLog("error, callbackName can not be empty.");
        return;
    }
    
    CCAssert(target, "Argument target must be non-nullptr");
    CCAssert(!callbackName.empty(), "key should not be empty!");
    
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    
    if (! element)
    {
        element = (tHashTimerEntry *)calloc(sizeof(*element), 1);
        element->target = target;
        
        HASH_ADD_PTR(_hashForTimers, target, element);
        
        // Is this the 1st element ? Then set the pause level to all the selectors of this target
        element->paused = paused;
    }
    else
    {
        CCAssert(element->paused == paused, "element's paused should be paused!");
    }
    
    if (element->timers == nullptr)
    {
        element->timers = ccArrayNew(10);
    }
    else
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetCallback *timer = dynamic_cast<TimerTargetCallback*>(element->timers->arr[i]);
            
            if (timer && callbackName == timer->getCallbackName())
            {
                CCLOG("CAScheduler#scheduleSelector. Selector already scheduled. Updating interval from: %.4f to %.4f", timer->getInterval(), interval);
                timer->setInterval(interval);
                return;
            }
        }
        ccArrayEnsureExtraCapacity(element->timers, 1);
    }
    
    TimerTargetCallback *timer = new (std::nothrow) TimerTargetCallback();
    timer->initWithCallback(this, callback, callbackName, target, interval, repeat, delay);
    ccArrayAppendObject(element->timers, timer);
    timer->release();
}

void CAScheduler::schedule(SEL_Schedule selector, CAObject *target, float interval, unsigned int repeat, float delay, bool paused)
{
    CCAssert(target, "Argument target must be non-nullptr");

    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    
    if (! element)
    {
        element = (tHashTimerEntry *)calloc(sizeof(*element), 1);
        element->target = target;
        
        HASH_ADD_PTR(_hashForTimers, target, element);
        
        // Is this the 1st element ? Then set the pause level to all the selectors of this target
        element->paused = paused;
    }
    else
    {
        CCAssert(element->paused == paused, "element's paused should be paused.");
    }
    
    if (element->timers == nullptr)
    {
        element->timers = ccArrayNew(10);
    }
    else
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetSelector *timer = dynamic_cast<TimerTargetSelector*>(element->timers->arr[i]);
            
            if (timer && selector == timer->getSelector())
            {
                CCLOG("CAScheduler#scheduleSelector. Selector already scheduled. Updating interval from: %.4f to %.4f", timer->getInterval(), interval);
                timer->setInterval(interval);
                return;
            }
        }
        ccArrayEnsureExtraCapacity(element->timers, 1);
    }
    
    TimerTargetSelector *timer = new (std::nothrow) TimerTargetSelector();
    timer->initWithSelector(this, selector, target, interval, repeat, delay);
    ccArrayAppendObject(element->timers, timer);
    timer->release();
}

void CAScheduler::unschedule(const std::string &callbackName, CAObject *target)
{
    // explicit handle nil arguments when removing an object
    if (target == nullptr || callbackName.empty())
    {
        return;
    }
    
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    
    if (element)
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetCallback *timer = dynamic_cast<TimerTargetCallback*>(element->timers->arr[i]);
            
            if (timer && callbackName == timer->getCallbackName())
            {
                if (timer == element->currentTimer && (! element->currentTimerSalvaged))
                {
                    element->currentTimer->retain();
                    element->currentTimerSalvaged = true;
                }
                
                ccArrayRemoveObjectAtIndex(element->timers, i, true);
                
                // update timerIndex in case we are in tick:, looping over the actions
                if (element->timerIndex >= i)
                {
                    element->timerIndex--;
                }
                
                if (element->timers->num == 0)
                {
                    if (_currentTarget == element)
                    {
                        _currentTargetSalvaged = true;
                    }
                    else
                    {
                        removeHashElement(element);
                    }
                }
                
                return;
            }
        }
    }
}

void CAScheduler::unschedule(SEL_Schedule selector, CAObject *target)
{
    // explicit handle nil arguments when removing an object
    if (target == nullptr || selector == nullptr)
    {
        return;
    }
    
    //CCAssert(target);
    //CCAssert(selector);
    
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    
    if (element)
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetSelector *timer = dynamic_cast<TimerTargetSelector*>(element->timers->arr[i]);
            
            if (timer && selector == timer->getSelector())
            {
                if (timer == element->currentTimer && (! element->currentTimerSalvaged))
                {
                    element->currentTimer->retain();
                    element->currentTimerSalvaged = true;
                }
                
                ccArrayRemoveObjectAtIndex(element->timers, i, true);
                
                // update timerIndex in case we are in tick:, looping over the actions
                if (element->timerIndex >= i)
                {
                    element->timerIndex--;
                }
                
                if (element->timers->num == 0)
                {
                    if (_currentTarget == element)
                    {
                        _currentTargetSalvaged = true;
                    }
                    else
                    {
                        removeHashElement(element);
                    }
                }
                
                return;
            }
        }
    }
}

void CAScheduler::unscheduleAllForTarget(CAObject *target)
{
    // explicit nullptr handling
    if (target == nullptr)
    {
        return;
    }
    
    // Custom Selectors
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    
    if (element)
    {
        if (ccArrayContainsObject(element->timers, element->currentTimer)
            && (! element->currentTimerSalvaged))
        {
            element->currentTimer->retain();
            element->currentTimerSalvaged = true;
        }
        ccArrayRemoveAllObjects(element->timers);
        
        if (_currentTarget == element)
        {
            _currentTargetSalvaged = true;
        }
        else
        {
            removeHashElement(element);
        }
    }
    
    unscheduleUpdate(target);
}

bool CAScheduler::isScheduled(const std::string& callbackName, CAObject *target)
{
    CCAssert(!callbackName.empty(), "Argument key must not be empty");
    CCAssert(target, "Argument target must be non-nullptr");
    
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    
    if (!element)
    {
        return false;
    }
    
    if (element->timers == nullptr)
    {
        return false;
    }
    
    for (int i = 0; i < element->timers->num; ++i)
    {
        TimerTargetCallback *timer = dynamic_cast<TimerTargetCallback*>(element->timers->arr[i]);
        
        if (timer && callbackName == timer->getCallbackName())
            return true;
    }

    return false;
}

bool CAScheduler::isScheduled(SEL_Schedule selector, CAObject *target)
{
    CCAssert(selector, "Argument selector must be non-nullptr");
    CCAssert(target, "Argument target must be non-nullptr");
    
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    
    if (!element)
    {
        return false;
    }
    
    if (element->timers == nullptr)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetSelector *timer = dynamic_cast<TimerTargetSelector*>(element->timers->arr[i]);
            
            if (timer && selector == timer->getSelector())
            {
                return true;
            }
        }
        
        return false;
    }
}

void CAScheduler::removeUpdateFromHash(struct _listEntry *entry)
{
    tHashUpdateEntry *element = nullptr;
    
    HASH_FIND_PTR(_hashForUpdates, &entry->target, element);
    if (element)
    {
        // list entry
        DL_DELETE(*element->list, element->entry);
        CC_SAFE_DELETE(element->entry);
        
        // hash entry
        HASH_DEL(_hashForUpdates, element);
        free(element);
    }
}

void CAScheduler::unscheduleAll(void)
{
    unscheduleAllWithMinPriority(PRIORITY_SYSTEM);
}

void CAScheduler::unscheduleAllWithMinPriority(int minPriority)
{
    // Custom Selectors
    tHashTimerEntry *element = nullptr;
    tHashTimerEntry *nextElement = nullptr;
    for (element = _hashForTimers; element != nullptr;)
    {
        // element may be removed in unscheduleAllSelectorsForTarget
        nextElement = (tHashTimerEntry *)element->hh.next;
        unscheduleAllForTarget(element->target);
        
        element = nextElement;
    }
    
    // Updates selectors
    tListEntry *entry, *tmp;
    if(minPriority < 0)
    {
        DL_FOREACH_SAFE(_updatesNegList, entry, tmp)
        {
            if(entry->priority >= minPriority)
            {
                unscheduleUpdate(entry->target);
            }
        }
    }
    
    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE(_updates0List, entry, tmp)
        {
            unscheduleUpdate(entry->target);
        }
    }
    
    DL_FOREACH_SAFE(_updatesPosList, entry, tmp)
    {
        if(entry->priority >= minPriority)
        {
            unscheduleUpdate(entry->target);
        }
    }
}

void CAScheduler::resumeTarget(CAObject *target)
{
    CCAssert(target != nullptr, "target can't be nullptr!");
    
    // custom selectors
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    if (element)
    {
        element->paused = false;
    }
    
    // update selector
    tHashUpdateEntry *elementUpdate = nullptr;
    HASH_FIND_PTR(_hashForUpdates, &target, elementUpdate);
    if (elementUpdate)
    {
        CCAssert(elementUpdate->entry != nullptr, "elementUpdate's entry can't be nullptr!");
        elementUpdate->entry->paused = false;
    }
}

void CAScheduler::pauseTarget(CAObject *target)
{
    CCAssert(target != nullptr, "target can't be nullptr!");
    
    // custom selectors
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    if (element)
    {
        element->paused = true;
    }
    
    // update selector
    tHashUpdateEntry *elementUpdate = nullptr;
    HASH_FIND_PTR(_hashForUpdates, &target, elementUpdate);
    if (elementUpdate)
    {
        CCAssert(elementUpdate->entry != nullptr, "elementUpdate's entry can't be nullptr!");
        elementUpdate->entry->paused = true;
    }
}

bool CAScheduler::isTargetPaused(CAObject *target)
{
    CCAssert( target != nullptr, "target must be non nil" );
    
    // Custom selectors
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    if( element )
    {
        return element->paused;
    }
    
    // We should check update selectors if target does not have custom selectors
    tHashUpdateEntry *elementUpdate = nullptr;
    HASH_FIND_PTR(_hashForUpdates, &target, elementUpdate);
    if ( elementUpdate )
    {
        return elementUpdate->entry->paused;
    }
    
    return false;  // should never get here
}

void CAScheduler::pauseAll()
{
    this->pauseAllTargetsWithMinPriority(PRIORITY_SYSTEM);
}

void CAScheduler::resumeAll()
{
    this->resumeAllTargetsWithMinPriority(PRIORITY_SYSTEM);
}

void CAScheduler::pauseAllTargetsWithMinPriority(int minPriority)
{
    // Custom Selectors
    for(tHashTimerEntry *element = _hashForTimers; element != nullptr;
        element = (tHashTimerEntry*)element->hh.next)
    {
        element->paused = true;
    }
    
    // Updates selectors
    tListEntry *entry, *tmp;
    if(minPriority < 0)
    {
        DL_FOREACH_SAFE( _updatesNegList, entry, tmp )
        {
            if(entry->priority >= minPriority)
            {
                entry->paused = true;
            }
        }
    }
    
    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE( _updates0List, entry, tmp )
        {
            entry->paused = true;
        }
    }
    
    DL_FOREACH_SAFE( _updatesPosList, entry, tmp )
    {
        if(entry->priority >= minPriority)
        {
            entry->paused = true;
        }
    }
}

void CAScheduler::resumeAllTargetsWithMinPriority(int minPriority)
{
    // Custom Selectors
    for(tHashTimerEntry *element = _hashForTimers; element != nullptr;
        element = (tHashTimerEntry*)element->hh.next)
    {
        element->paused = false;
    }
    
    // Updates selectors
    tListEntry *entry, *tmp;
    if(minPriority < 0)
    {
        DL_FOREACH_SAFE( _updatesNegList, entry, tmp )
        {
            if(entry->priority >= minPriority)
            {
                entry->paused = false;
            }
        }
    }
    
    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE( _updates0List, entry, tmp )
        {
            entry->paused = false;
        }
    }
    
    DL_FOREACH_SAFE( _updatesPosList, entry, tmp )
    {
        if(entry->priority >= minPriority)
        {
            entry->paused = false;
        }
    }
}

void CAScheduler::performFunctionInUIThread(const std::function<void()> &function)
{
    _performMutex.lock();
    
    _functionsToPerform.push_back(function);
    
    _performMutex.unlock();
}

// main loop
void CAScheduler::update(float dt)
{
    _updateHashLocked = true;
    
    if (_timeScale != 1.0f)
    {
        dt *= _timeScale;
    }
    
    //
    // Selector callbacks
    //
    
    // Iterate over all the Updates' selectors
    tListEntry *entry, *tmp;
    
    // updates with priority < 0
    DL_FOREACH_SAFE(_updatesNegList, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->callback(dt);
        }
    }
    
    // updates with priority == 0
    DL_FOREACH_SAFE(_updates0List, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->callback(dt);
        }
    }
    
    // updates with priority > 0
    DL_FOREACH_SAFE(_updatesPosList, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->callback(dt);
        }
    }
    
    // Iterate over all the custom selectors
    for (tHashTimerEntry *elt = _hashForTimers; elt != nullptr; )
    {
        _currentTarget = elt;
        _currentTargetSalvaged = false;
        
        if (! _currentTarget->paused)
        {
            // The 'timers' array may change while inside this loop
            for (elt->timerIndex = 0; elt->timerIndex < elt->timers->num; ++(elt->timerIndex))
            {
                elt->currentTimer = (Timer*)(elt->timers->arr[elt->timerIndex]);
                elt->currentTimerSalvaged = false;
                
                elt->currentTimer->update(dt);
                
                if (elt->currentTimerSalvaged)
                {
                    // The currentTimer told the remove itself. To prevent the timer from
                    // accidentally deallocating itself before finishing its step, we retained
                    // it. Now that step is done, it's safe to release it.
                    elt->currentTimer->release();
                }
                
                elt->currentTimer = nullptr;
            }
        }
        
        // elt, at this moment, is still valid
        // so it is safe to ask this here (issue #490)
        elt = (tHashTimerEntry *)elt->hh.next;
        
        // only delete currentTarget if no actions were scheduled during the cycle (issue #481)
        if (_currentTargetSalvaged && _currentTarget->timers->num == 0)
        {
            removeHashElement(_currentTarget);
        }
    }
    // delete all updates that are marked for deletion
    // updates with priority < 0
    DL_FOREACH_SAFE(_updatesNegList, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }
    
    // updates with priority == 0
    DL_FOREACH_SAFE(_updates0List, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }
    
    // updates with priority > 0
    DL_FOREACH_SAFE(_updatesPosList, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }
    
    _updateHashLocked = false;
    _currentTarget = nullptr;
    
    //
    // Functions allocated from another thread
    //
    
    // Testing size is faster than locking / unlocking.
    // And almost never there will be functions scheduled to be called.
    if( !_functionsToPerform.empty() )
    {
        _performMutex.lock();
        // fixed #4123: Save the callback functions, they must be invoked after '_performMutex.unlock()', otherwise if new functions are added in callback, it will cause thread deadlock.
        auto temp = _functionsToPerform;
        _functionsToPerform.clear();
        _performMutex.unlock();
        for( const auto &function : temp )
        {
            function();
        }
        
    }
}

void CAScheduler::scheduleUpdate(CAObject *target, int priority, bool paused)
{
    this->schedulePerFrame([target](float dt)
    {
        target->update(MIN(dt, 0.05));
    },target, priority, paused);
}

void CAScheduler::unscheduleUpdate(CAObject *target)
{
    if (target == nullptr)
    {
        return;
    }
    
    tHashUpdateEntry *element = nullptr;
    HASH_FIND_PTR(_hashForUpdates, &target, element);
    if (element)
    {
        if (_updateHashLocked)
        {
            element->entry->markedForDeletion = true;
        }
        else
        {
            this->removeUpdateFromHash(element->entry);
        }
    }
}

void CAScheduler::priorityIn(tListEntry **list, const CAScheduler::Callback& callback, CAObject *target, int priority, bool paused)
{
    tListEntry *listElement = new (std::nothrow) tListEntry();
    
    listElement->callback = callback;
    listElement->target = target;
    listElement->priority = priority;
    listElement->paused = paused;
    listElement->next = listElement->prev = nullptr;
    listElement->markedForDeletion = false;
    
    // empty list ?
    if (! *list)
    {
        DL_APPEND(*list, listElement);
    }
    else
    {
        bool added = false;
        
        for (tListEntry *element = *list; element; element = element->next)
        {
            if (priority < element->priority)
            {
                if (element == *list)
                {
                    DL_PREPEND(*list, listElement);
                }
                else
                {
                    listElement->next = element;
                    listElement->prev = element->prev;
                    
                    element->prev->next = listElement;
                    element->prev = listElement;
                }
                
                added = true;
                break;
            }
        }
        
        // Not added? priority has the higher value. Append it.
        if (! added)
        {
            DL_APPEND(*list, listElement);
        }
    }
    
    // update hash entry for quick access
    tHashUpdateEntry *hashElement = (tHashUpdateEntry *)calloc(sizeof(*hashElement), 1);
    hashElement->target = target;
    hashElement->list = list;
    hashElement->entry = listElement;
    HASH_ADD_PTR(_hashForUpdates, target, hashElement);
}

void CAScheduler::appendIn(_listEntry **list, const CAScheduler::Callback& callback, CAObject *target, bool paused)
{
    tListEntry *listElement = new (std::nothrow) tListEntry();
    
    listElement->callback = callback;
    listElement->target = target;
    listElement->paused = paused;
    listElement->priority = 0;
    listElement->markedForDeletion = false;
    
    DL_APPEND(*list, listElement);
    
    // update hash entry for quicker access
    tHashUpdateEntry *hashElement = (tHashUpdateEntry *)calloc(sizeof(*hashElement), 1);
    hashElement->target = target;
    hashElement->list = list;
    hashElement->entry = listElement;
    HASH_ADD_PTR(_hashForUpdates, target, hashElement);
}

void CAScheduler::schedulePerFrame(const CAScheduler::Callback& callback, CAObject *target, int priority, bool paused)
{
    tHashUpdateEntry *hashElement = nullptr;
    HASH_FIND_PTR(_hashForUpdates, &target, hashElement);
    if (hashElement)
    {
        // check if priority has changed
        if ((*hashElement->list)->priority != priority)
        {
            if (_updateHashLocked)
            {
                CCLOG("warning: you CANNOT change update priority in scheduled function");
                hashElement->entry->markedForDeletion = false;
                hashElement->entry->paused = paused;
                return;
            }
            else
            {
                // will be added again outside if (hashElement).
                unscheduleUpdate(target);
            }
        }
        else
        {
            hashElement->entry->markedForDeletion = false;
            hashElement->entry->paused = paused;
            return;
        }
    }
    
    // most of the updates are going to be 0, that's way there
    // is an special list for updates with priority 0
    if (priority == 0)
    {
        appendIn(&_updates0List, callback, target, paused);
    }
    else if (priority < 0)
    {
        priorityIn(&_updatesNegList, callback, target, priority, paused);
    }
    else
    {
        // priority > 0
        priorityIn(&_updatesPosList, callback, target, priority, paused);
    }
}
NS_CC_END

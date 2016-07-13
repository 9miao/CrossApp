#include "CAScheduler.h"
#include "ccMacros.h"
#include "CAApplication.h"
#include "support/data_support/utlist.h"
#include "support/data_support/ccCArray.h"
#include "cocoa/CCSet.h"
#include "script_support/CCScriptSupport.h"

NS_CC_BEGIN

// data structures

// A list double-linked list used for "updates with priority"
typedef struct _listEntry
{
    struct _listEntry   *prev, *next;
    ccSchedulerFunc     callback;
    void                *target;
    int                 priority;
    bool                paused;
    bool                markedForDeletion; // selector will no longer be called and entry will be removed at end of the next tick
} tListEntry;

typedef struct _hashUpdateEntry
{
    tListEntry          **list;        // Which list does it belong to ?
    tListEntry          *entry;        // entry in the list
    void                *target;
    ccSchedulerFunc     callback;
    UT_hash_handle      hh;
} tHashUpdateEntry;

// Hash Element used for "selectors with interval"
typedef struct _hashSelectorEntry
{
    ccArray             *timers;
    void                *target;
    int                 timerIndex;
    Timer               *currentTimer;
    bool                currentTimerSalvaged;
    bool                paused;
    UT_hash_handle      hh;
} tHashTimerEntry;

// implementation Timer

Timer::Timer()
: _scheduler(nullptr)
, _elapsed(-1)
, _runForever(false)
, _useDelay(false)
, _timesExecuted(0)
, _repeat(0)
, _delay(0.0f)
, _interval(0.0f)
{
}

void Timer::setupTimerWithInterval(float seconds, unsigned int repeat, float delay)
{
    _elapsed = -1;
    _interval = seconds;
    _delay = delay;
    _useDelay = (_delay > 0.0f) ? true : false;
    _repeat = repeat;
    _runForever = (_repeat == kCCRepeatForever) ? true : false;
}

void Timer::update(float dt)
{
    if (_elapsed == -1)
    {
        _elapsed = 0;
        _timesExecuted = 0;
        return;
    }
    
    // accumulate elapsed time
    _elapsed += dt;
    
    // deal with delay
    if (_useDelay)
    {
        if (_elapsed < _delay)
        {
            return;
        }
        trigger(_interval);
        _elapsed = _elapsed - _delay;
        _timesExecuted += 1;
        _useDelay = false;
        // after delay, the rest time should compare with interval
        if (!_runForever && _timesExecuted > _repeat)
        {    //unschedule timer
            cancel();
        }
        return;
    }
    
    // if _interval == 0, should trigger once every frame
    float interval = (_interval > 0) ? _interval : _elapsed;
    if (_elapsed >= interval || interval <= 0.0167)
    {
        trigger(interval); 
        _elapsed -= interval;
        _timesExecuted += 1;
        
        if (!_runForever && _timesExecuted > _repeat)
        {
            cancel();
        }
    } 
}

// TimerTargetSelector

TimerTargetSelector::TimerTargetSelector()
: _target(nullptr)
, _selector(nullptr)
{
}

bool TimerTargetSelector::initWithSelector(CAScheduler* scheduler, SEL_SCHEDULE selector, CAObject* target, float seconds, unsigned int repeat, float delay)
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

// TimerTargetCallback

TimerTargetCallback::TimerTargetCallback()
: _target(nullptr)
, _callback(nullptr)
{
}

bool TimerTargetCallback::initWithCallback(CAScheduler* scheduler, const ccSchedulerFunc& callback, void *target, const std::string& key, float seconds, unsigned int repeat, float delay)
{
    _scheduler = scheduler;
    _target = target;
    _callback = callback;
    _key = key;
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
    _scheduler->unscheduleSelector(_key, _target);
}

#if CC_ENABLE_SCRIPT_BINDING

// TimerScriptHandler

bool TimerScriptHandler::initWithScriptHandler(int handler, float seconds)
{
    _scriptHandler = handler;
    _elapsed = -1;
    _interval = seconds;
    
    return true;
}

void TimerScriptHandler::trigger(float dt)
{
    if (0 != _scriptHandler)
    {
        SchedulerScriptData data(_scriptHandler,dt);
        ScriptEvent event(kScheduleEvent,&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
}

void TimerScriptHandler::cancel()
{
    
}

#endif

// implementation of Scheduler

void CAScheduler::schedule(SEL_SCHEDULE pfnSelector, CAObject *pTarget, float fInterval, unsigned int repeat, float delay, bool bPaused)
{
    CAScheduler::getScheduler()->scheduleSelector(pfnSelector, pTarget, fInterval, repeat, delay, bPaused);
}

void CAScheduler::schedule(SEL_SCHEDULE pfnSelector, CAObject *pTarget, float fInterval, bool bPaused)
{
    CAScheduler::getScheduler()->scheduleSelector(pfnSelector, pTarget, fInterval, bPaused);
}

void CAScheduler::unschedule(SEL_SCHEDULE pfnSelector, CAObject *pTarget)
{
    CAScheduler::getScheduler()->unscheduleSelector(pfnSelector, pTarget);
}

void CAScheduler::unscheduleAllForTarget(CAObject *pTarget)
{
    CAScheduler::getScheduler()->unscheduleSelectorAllForTarget(pTarget);
}

void CAScheduler::unscheduleAll(void)
{
    CAScheduler::getScheduler()->unscheduleSelectorAll();
}

bool CAScheduler::isScheduled(SEL_SCHEDULE pfnSelector, CAObject *pTarget)
{
    return CAScheduler::getScheduler()->isScheduledSelector(pfnSelector, pTarget);
}

// Priority level reserved for system services.
const int CAScheduler::PRIORITY_SYSTEM = INT_MIN;

// Minimum priority level for user scheduling.
const int CAScheduler::PRIORITY_NON_SYSTEM_MIN = PRIORITY_SYSTEM + 1;

static CAScheduler* s_pScheduler = NULL;

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
#if CC_ENABLE_SCRIPT_BINDING
, _scriptHandlerEntries(20)
#endif
{
    s_pScheduler = this;
    // I don't expect to have more than 30 functions to all per frame
    _functionsToPerform.reserve(30);
}

CAScheduler::~CAScheduler(void)
{
    unscheduleAll();
    s_pScheduler = NULL;
}

CAScheduler* CAScheduler::getScheduler()
{
    if (s_pScheduler == NULL)
    {
        return (new CAScheduler());
    }
    return s_pScheduler;
}

void CAScheduler::removeHashElement(_hashSelectorEntry *element)
{
    ccArrayFree(element->timers);
    HASH_DEL(_hashForTimers, element);
    free(element);
}

void CAScheduler::scheduleSelector(const ccSchedulerFunc& callback, void *target, float interval, bool paused, const std::string& key)
{
    this->scheduleSelector(callback, target, interval, kCCRepeatForever, 0.0f, paused, key);
}

void CAScheduler::scheduleSelector(const ccSchedulerFunc& callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& key)
{
    CCAssert(target, "Argument target must be non-nullptr");
    CCAssert(!key.empty(), "key should not be empty!");
    
    delay = MAX(delay, 1/60.0f);
    interval = MAX(interval, 1/60.0f);
    
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
            
            if (timer && key == timer->getKey())
            {
                CCLOG("CCScheduler#scheduleSelector. Selector already scheduled. Updating interval from: %.4f to %.4f", timer->getInterval(), interval);
                timer->setInterval(interval);
                return;
            }
        }
        ccArrayEnsureExtraCapacity(element->timers, 1);
    }
    
    TimerTargetCallback *timer = new (std::nothrow) TimerTargetCallback();
    timer->initWithCallback(this, callback, target, key, interval, repeat, delay);
    ccArrayAppendObject(element->timers, timer);
    timer->release();
}

void CAScheduler::unscheduleSelector(const std::string &key, void *target)
{
    // explicit handle nil arguments when removing an object
    if (target == nullptr || key.empty())
    {
        return;
    }
    
    //CCASSERT(target);
    //CCASSERT(selector);
    
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(_hashForTimers, &target, element);
    
    if (element)
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetCallback *timer = dynamic_cast<TimerTargetCallback*>(element->timers->arr[i]);
            
            if (timer && key == timer->getKey())
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

void CAScheduler::priorityIn(tListEntry **list, const ccSchedulerFunc& callback, void *target, int priority, bool paused)
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

void CAScheduler::appendIn(_listEntry **list, const ccSchedulerFunc& callback, void *target, bool paused)
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

void CAScheduler::schedulePerFrame(const ccSchedulerFunc& callback, void *target, int priority, bool paused)
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

bool CAScheduler::isScheduledSelector(const std::string& key, void *target)
{
    CCAssert(!key.empty(), "Argument key must not be empty");
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
            TimerTargetCallback *timer = dynamic_cast<TimerTargetCallback*>(element->timers->arr[i]);
            
            if (timer && key == timer->getKey())
            {
                return true;
            }
        }
        
        return false;
    }
    
    return false;  // should never get here
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

void CAScheduler::unscheduleUpdate(void *target)
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

void CAScheduler::unscheduleSelectorAll(void)
{
    unscheduleSelectorAllWithMinPriority(PRIORITY_SYSTEM);
}

void CAScheduler::unscheduleSelectorAllWithMinPriority(int minPriority)
{
    // Custom Selectors
    tHashTimerEntry *element = nullptr;
    tHashTimerEntry *nextElement = nullptr;
    for (element = _hashForTimers; element != nullptr;)
    {
        // element may be removed in unscheduleAllSelectorsForTarget
        nextElement = (tHashTimerEntry *)element->hh.next;
        unscheduleSelectorAllForTarget(element->target);
        
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
#if CC_ENABLE_SCRIPT_BINDING
    _scriptHandlerEntries.clear();
#endif
}

void CAScheduler::unscheduleSelectorAllForTarget(void *target)
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
    
    // update selector
    unscheduleUpdate(target);
}

#if CC_ENABLE_SCRIPT_BINDING
unsigned int CAScheduler::scheduleScriptFunc(unsigned int handler, float interval, bool paused)
{
    SchedulerScriptHandlerEntry* entry = SchedulerScriptHandlerEntry::create(handler, interval, paused);
    _scriptHandlerEntries.pushBack(entry);
    return entry->getEntryId();
}

void CAScheduler::unscheduleScriptEntry(unsigned int scheduleScriptEntryID)
{
    for (ssize_t i = _scriptHandlerEntries.size() - 1; i >= 0; i--)
    {
        SchedulerScriptHandlerEntry* entry = _scriptHandlerEntries.at(i);
        if (entry->getEntryId() == (int)scheduleScriptEntryID)
        {
            entry->markedForDeletion();
            break;
        }
    }
}

#endif

void CAScheduler::resumeTarget(void *target)
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

void CAScheduler::pauseTarget(void *target)
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

bool CAScheduler::isTargetPaused(void *target)
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

std::set<void*> CAScheduler::pauseAllTargets()
{
    return pauseAllTargetsWithMinPriority(PRIORITY_SYSTEM);
}

std::set<void*> CAScheduler::pauseAllTargetsWithMinPriority(int minPriority)
{
    std::set<void*> idsWithSelectors;
    
    // Custom Selectors
    for(tHashTimerEntry *element = _hashForTimers; element != nullptr;
        element = (tHashTimerEntry*)element->hh.next)
    {
        element->paused = true;
        idsWithSelectors.insert(element->target);
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
                idsWithSelectors.insert(entry->target);
            }
        }
    }
    
    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE( _updates0List, entry, tmp )
        {
            entry->paused = true;
            idsWithSelectors.insert(entry->target);
        }
    }
    
    DL_FOREACH_SAFE( _updatesPosList, entry, tmp )
    {
        if(entry->priority >= minPriority)
        {
            entry->paused = true;
            idsWithSelectors.insert(entry->target);
        }
    }
    
    return idsWithSelectors;
}

void CAScheduler::resumeTargets(const std::set<void*>& targetsToResume)
{
    for(const auto &obj : targetsToResume) {
        this->resumeTarget(obj);
    }
}

void CAScheduler::performFunctionInUIThread(const std::function<void ()> &function)
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
    
#if CC_ENABLE_SCRIPT_BINDING
    //
    // Script callbacks
    //
    
    // Iterate over all the script callbacks
    if (!_scriptHandlerEntries.empty())
    {
        for (auto i = _scriptHandlerEntries.size() - 1; i >= 0; i--)
        {
            SchedulerScriptHandlerEntry* eachEntry = _scriptHandlerEntries.at(i);
            if (eachEntry->isMarkedForDeletion())
            {
                _scriptHandlerEntries.erase(i);
            }
            else if (!eachEntry->isPaused())
            {
                eachEntry->getTimer()->update(dt);
            }
        }
    }
#endif
    //
    // Functions allocated from another thread
    //
    
    // Testing size is faster than locking / unlocking.
    // And almost never there will be functions scheduled to be called.
    if( !_functionsToPerform.empty() ) {
        _performMutex.lock();
        // fixed #4123: Save the callback functions, they must be invoked after '_performMutex.unlock()', otherwise if new functions are added in callback, it will cause thread deadlock.
        auto temp = _functionsToPerform;
        _functionsToPerform.clear();
        _performMutex.unlock();
        for( const auto &function : temp ) {
            function();
        }
        
    }
}

void CAScheduler::scheduleSelector(SEL_SCHEDULE selector, CAObject *target, float interval, unsigned int repeat, float delay, bool paused)
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

void CAScheduler::scheduleSelector(SEL_SCHEDULE selector, CAObject *target, float interval, bool paused)
{
    this->scheduleSelector(selector, target, interval, kCCRepeatForever, 0.0f, paused);
}

void CAScheduler::scheduleSelectorUpdate(CAObject *target, int priority, bool paused)
{
    this->schedulePerFrame([target](float dt){
        target->update(dt);
    }, target, priority, paused);
}

bool CAScheduler::isScheduledSelector(SEL_SCHEDULE selector, CAObject *target)
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
    
    return false;  // should never get here
}

void CAScheduler::unscheduleSelector(SEL_SCHEDULE selector, CAObject *target)
{
    // explicit handle nil arguments when removing an object
    if (target == nullptr || selector == nullptr)
    {
        return;
    }
    
    //CCASSERT(target);
    //CCASSERT(selector);
    
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

NS_CC_END

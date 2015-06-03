
#include "CAScheduler.h"
#include "ccMacros.h"
#include "CAApplication.h"
#include "support/data_support/utlist.h"
#include "cocoa/CCSet.h"

using namespace std;

NS_CC_BEGIN

// data structures

// A list double-linked list used for "updates with priority"
typedef struct _listEntry
{
    struct _listEntry   *prev, *next;
    CAObject            *target;        // not retained (retained by hashUpdateEntry)
    int                 priority;
    bool                paused;
    bool                markedForDeletion; // selector will no longer be called and entry will be removed at end of the next tick
} tListEntry;

typedef struct _hashUpdateEntry
{
    tListEntry          **list;        // Which list does it belong to ?
    tListEntry          *entry;        // entry in the list
    CAObject            *target;        // hash key (retained)
    UT_hash_handle      hh;
} tHashUpdateEntry;

// Hash Element used for "selectors with interval"
typedef struct _hashSelectorEntry
{
    ccArray             *timers;
    CAObject            *target;    // hash key (retained)
    unsigned int        timerIndex;
    CATimer             *currentTimer;
    bool                currentTimerSalvaged;
    bool                paused;
    UT_hash_handle      hh;
} tHashTimerEntry;

static CAScheduler* _scheduler = NULL;

// implementation CATimer

CATimer::CATimer()
: m_pTarget(NULL)
, m_fElapsed(-1)
, m_bRunForever(false)
, m_bUseDelay(false)
, m_uTimesExecuted(0)
, m_uRepeat(0)
, m_fDelay(0.0f)
, m_fInterval(0.0f)
, m_pfnSelector(NULL)
, m_nScriptHandler(0)
{

}

CATimer* CATimer::timerWithTarget(CAObject *pTarget, SEL_SCHEDULE pfnSelector)
{
    CATimer *pTimer = new CATimer();

    pTimer->initWithTarget(pTarget, pfnSelector, 0.0f, kCCRepeatForever, 0.0f);
    pTimer->autorelease();

    return pTimer;
}

CATimer* CATimer::timerWithTarget(CAObject *pTarget, SEL_SCHEDULE pfnSelector, float fSeconds)
{
    CATimer *pTimer = new CATimer();

    pTimer->initWithTarget(pTarget, pfnSelector, fSeconds, kCCRepeatForever, 0.0f);
    pTimer->autorelease();

    return pTimer;
}

CATimer* CATimer::timerWithScriptHandler(int nHandler, float fSeconds)
{
    CATimer *pTimer = new CATimer();

    pTimer->initWithScriptHandler(nHandler, fSeconds);
    pTimer->autorelease();

    return pTimer;
}

bool CATimer::initWithScriptHandler(int nHandler, float fSeconds)
{
    m_nScriptHandler = nHandler;
    m_fElapsed = -1;
    m_fInterval = fSeconds;

    return true;
}

bool CATimer::initWithTarget(CAObject *pTarget, SEL_SCHEDULE pfnSelector)
{
    return initWithTarget(pTarget, pfnSelector, 0, kCCRepeatForever, 0.0f);
}

bool CATimer::initWithTarget(CAObject *pTarget, SEL_SCHEDULE pfnSelector, float fSeconds, unsigned int nRepeat, float fDelay)
{
    m_pTarget = pTarget;
    m_pfnSelector = pfnSelector;
    m_fElapsed = -1;
    m_fInterval = fSeconds;
    m_fDelay = fDelay;
    m_bUseDelay = (fDelay > 0.0f) ? true : false;
    m_uRepeat = nRepeat;
    m_bRunForever = (nRepeat == kCCRepeatForever) ? true : false;
    return true;
}

void CATimer::update(float dt)
{
    if (m_fElapsed == -1)
    {
        m_fElapsed = 0;
        m_uTimesExecuted = 0;
    }
    else
    {
        m_fElapsed += dt;
        
        if (m_bRunForever && !m_bUseDelay)
        {//standard timer usage
            
            if (m_fElapsed >= m_fInterval || m_fInterval == 1/60.0f)
            {
                
                if (m_pTarget && m_pfnSelector)
                {
                    (m_pTarget->*m_pfnSelector)(MIN(m_fElapsed, 0.05f));
                }
            
                m_fElapsed = 0;
            }
        }
        else
        {//advanced usage
            if (m_bUseDelay)
            {
                if( m_fElapsed >= m_fDelay )
                {
                    m_fElapsed = m_fElapsed - m_fDelay;
                    
                    if (m_pTarget && m_pfnSelector)
                    {
						(m_pTarget->*m_pfnSelector)(MIN(m_fElapsed, 0.05f));
                    }

                    m_uTimesExecuted += 1;
                    m_bUseDelay = false;
                }
            }
            else
            {
                if (m_fElapsed >= m_fInterval)
                {
                    if (m_pTarget && m_pfnSelector)
                    {
						(m_pTarget->*m_pfnSelector)(MIN(m_fElapsed, 0.05f));
                    }

                    m_fElapsed = 0;
                    m_uTimesExecuted += 1;

                }
            }

            if (!m_bRunForever && m_uTimesExecuted > m_uRepeat)
            {    //unschedule timer
                CAScheduler::unschedule(m_pfnSelector, m_pTarget);
            }
        }
    }
}

float CATimer::getInterval() const
{
    return m_fInterval;
}

void CATimer::setInterval(float fInterval)
{
    m_fInterval = fInterval;
}

SEL_SCHEDULE CATimer::getSelector() const
{
    return m_pfnSelector;
}

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

// implementation of CAScheduler

CAScheduler::CAScheduler(void)
: m_fTimeScale(1.0f)
, m_pUpdatesNegList(NULL)
, m_pUpdates0List(NULL)
, m_pUpdatesPosList(NULL)
, m_pHashForUpdates(NULL)
, m_pHashForTimers(NULL)
, m_pCurrentTarget(NULL)
, m_bCurrentTargetSalvaged(false)
, m_bUpdateHashLocked(false)
{

}

CAScheduler::~CAScheduler(void)
{
    unscheduleSelectorAll();
}

CAScheduler* CAScheduler::getScheduler()
{
    if (_scheduler == NULL)
    {
        _scheduler = new CAScheduler();
    }
    return _scheduler;
}

void CAScheduler::removeHashElement(_hashSelectorEntry *pElement)
{
    ccArrayFree(pElement->timers);
    HASH_DEL(m_pHashForTimers, pElement);
    free(pElement);

    // make sure the target is released after we have removed the hash element
    // otherwise we access invalid memory when the release call deletes the target
    // and the target calls removeAllSelectors() during its destructor
}

void CAScheduler::scheduleSelector(SEL_SCHEDULE pfnSelector, CAObject *pTarget, float fInterval, bool bPaused)
{
    this->scheduleSelector(pfnSelector, pTarget, fInterval, kCCRepeatForever, 0, bPaused);
}

void CAScheduler::scheduleSelector(SEL_SCHEDULE pfnSelector, CAObject *pTarget, float fInterval, unsigned int repeat, float delay, bool bPaused)
{
    CCAssert(pfnSelector, "Argument selector must be non-NULL");
    CCAssert(pTarget, "Argument target must be non-NULL");
    
    delay = MAX(delay, 1/60.0f);
    fInterval = MAX(fInterval, 1/60.0f);
    
    tHashTimerEntry *pElement = NULL;
    HASH_FIND_INT(m_pHashForTimers, &pTarget, pElement);

    if (! pElement)
    {
        pElement = (tHashTimerEntry *)calloc(sizeof(*pElement), 1);
        pElement->target = pTarget;
        HASH_ADD_INT(m_pHashForTimers, target, pElement);

        // Is this the 1st element ? Then set the pause level to all the selectors of this target
        pElement->paused = bPaused;
    }
    else
    {
        CCAssert(pElement->paused == bPaused, "");
    }

    if (pElement->timers == NULL)
    {
        pElement->timers = ccArrayNew(10);
    }
    else 
    {
        for (unsigned int i = 0; i < pElement->timers->num; ++i)
        {
            CATimer *timer = (CATimer*)pElement->timers->arr[i];

            if (pfnSelector == timer->getSelector())
            {
                //CCLOG("CAScheduler#scheduleSelector. Selector already scheduled. Updating interval from: %.4f to %.4f", timer->getInterval(), fInterval);
                timer->setInterval(fInterval);
                return;
            }        
        }
        ccArrayEnsureExtraCapacity(pElement->timers, 1);
    }

    CATimer *pTimer = new CATimer();
    pTimer->initWithTarget(pTarget, pfnSelector, fInterval, repeat, delay);
    ccArrayAppendObject(pElement->timers, pTimer);
    pTimer->release();    
}

void CAScheduler::unscheduleSelector(SEL_SCHEDULE pfnSelector, CAObject *pTarget)
{
    // explicity handle nil arguments when removing an object
    if (pTarget == 0 || pfnSelector == 0)
    {
        return;
    }

    //CCAssert(pTarget);
    //CCAssert(pfnSelector);

    tHashTimerEntry *pElement = NULL;
    HASH_FIND_INT(m_pHashForTimers, &pTarget, pElement);

    if (pElement)
    {
        for (unsigned int i = 0; i < pElement->timers->num; ++i)
        {
            CATimer *pTimer = (CATimer*)(pElement->timers->arr[i]);

            if (pfnSelector == pTimer->getSelector())
            {
                if (pTimer == pElement->currentTimer && (! pElement->currentTimerSalvaged))
                {
                    pElement->currentTimer->retain();
                    pElement->currentTimerSalvaged = true;
                }

                ccArrayRemoveObjectAtIndex(pElement->timers, i, true);

                // update timerIndex in case we are in tick:, looping over the actions
                if (pElement->timerIndex >= i)
                {
                    pElement->timerIndex--;
                }

                if (pElement->timers->num == 0)
                {
                    if (m_pCurrentTarget == pElement)
                    {
                        m_bCurrentTargetSalvaged = true;
                    }
                    else
                    {
                        removeHashElement(pElement);
                    }
                }

                return;
            }
        }
    }
}

void CAScheduler::priorityIn(tListEntry **ppList, CAObject *pTarget, int nPriority, bool bPaused)
{
    tListEntry *pListElement = (tListEntry *)malloc(sizeof(*pListElement));

    pListElement->target = pTarget;
    pListElement->priority = nPriority;
    pListElement->paused = bPaused;
    pListElement->next = pListElement->prev = NULL;
    pListElement->markedForDeletion = false;

    // empty list ?
    if (! *ppList)
    {
        DL_APPEND(*ppList, pListElement);
    }
    else
    {
        bool bAdded = false;

        for (tListEntry *pElement = *ppList; pElement; pElement = pElement->next)
        {
            if (nPriority < pElement->priority)
            {
                if (pElement == *ppList)
                {
                    DL_PREPEND(*ppList, pListElement);
                }
                else
                {
                    pListElement->next = pElement;
                    pListElement->prev = pElement->prev;

                    pElement->prev->next = pListElement;
                    pElement->prev = pListElement;
                }

                bAdded = true;
                break;
            }
        }

        // Not added? priority has the higher value. Append it.
        if (! bAdded)
        {
            DL_APPEND(*ppList, pListElement);
        }
    }

    // update hash entry for quick access
    tHashUpdateEntry *pHashElement = (tHashUpdateEntry *)calloc(sizeof(*pHashElement), 1);
    pHashElement->target = pTarget;
    pHashElement->list = ppList;
    pHashElement->entry = pListElement;
    HASH_ADD_INT(m_pHashForUpdates, target, pHashElement);
}

void CAScheduler::appendIn(_listEntry **ppList, CAObject *pTarget, bool bPaused)
{
    tListEntry *pListElement = (tListEntry *)malloc(sizeof(*pListElement));

    pListElement->target = pTarget;
    pListElement->paused = bPaused;
    pListElement->markedForDeletion = false;

    DL_APPEND(*ppList, pListElement);

    // update hash entry for quicker access
    tHashUpdateEntry *pHashElement = (tHashUpdateEntry *)calloc(sizeof(*pHashElement), 1);
    pHashElement->target = pTarget;
    pHashElement->list = ppList;
    pHashElement->entry = pListElement;
    HASH_ADD_INT(m_pHashForUpdates, target, pHashElement);
}

void CAScheduler::removeUpdateFromHash(struct _listEntry *entry)
{
    tHashUpdateEntry *element = NULL;

    HASH_FIND_INT(m_pHashForUpdates, &entry->target, element);
    if (element)
    {
        // list entry
        DL_DELETE(*element->list, element->entry);
        free(element->entry);

        // hash entry
        HASH_DEL(m_pHashForUpdates, element);
        free(element);

        // target#release should be the last one to prevent
        // a possible double-free. eg: If the [target dealloc] might want to remove it itself from there
    }
}

void CAScheduler::unscheduleSelectorAll(void)
{
    unscheduleSelectorAllWithMinPriority(kCCPrioritySystem);
}

void CAScheduler::unscheduleSelectorAllWithMinPriority(int nMinPriority)
{
    // Custom Selectors
    tHashTimerEntry *pElement = NULL;
    tHashTimerEntry *pNextElement = NULL;
    for (pElement = m_pHashForTimers; pElement != NULL;)
    {
        // pElement may be removed in unscheduleAllSelectorsForTarget
        pNextElement = (tHashTimerEntry *)pElement->hh.next;
        unscheduleAllForTarget(pElement->target);

        pElement = pNextElement;
    }
}

void CAScheduler::unscheduleSelectorAllForTarget(CAObject *pTarget)
{
    // explicit NULL handling
    if (pTarget == NULL)
    {
        return;
    }

    // Custom Selectors
    tHashTimerEntry *pElement = NULL;
    HASH_FIND_INT(m_pHashForTimers, &pTarget, pElement);

    if (pElement)
    {
        if (ccArrayContainsObject(pElement->timers, pElement->currentTimer)
            && (! pElement->currentTimerSalvaged))
        {
            pElement->currentTimer->retain();
            pElement->currentTimerSalvaged = true;
        }
        ccArrayRemoveAllObjects(pElement->timers);

        if (m_pCurrentTarget == pElement)
        {
            m_bCurrentTargetSalvaged = true;
        }
        else
        {
            removeHashElement(pElement);
        }
    }
}

bool CAScheduler::isScheduledSelector(SEL_SCHEDULE pfnSelector, CAObject *pTarget)
{
    CCAssert(pfnSelector, "Argument selector must be non-nullptr");
    CCAssert(pTarget, "Argument target must be non-nullptr");
    
    tHashTimerEntry *pElement = NULL;
    HASH_FIND_INT(m_pHashForTimers, &pTarget, pElement);
    
    if (!pElement)
    {
        return false;
    }
    
    if (pElement->timers == NULL)
    {
        return false;
    }
    else
    {
        for (unsigned i = 0; i < pElement->timers->num; ++i)
        {
            CATimer *timer = static_cast<CATimer*>(pElement->timers->arr[i]);
            
            if (pfnSelector == timer->getSelector())
            {
                return true;
            }
        }
        
        return false;
    }
    
    return false;  // should never get here
}

void CAScheduler::resumeTarget(CAObject *pTarget)
{
    CCAssert(pTarget != NULL, "");

    // custom selectors
    tHashTimerEntry *pElement = NULL;
    HASH_FIND_INT(m_pHashForTimers, &pTarget, pElement);
    if (pElement)
    {
        pElement->paused = false;
    }

    // update selector
    tHashUpdateEntry *pElementUpdate = NULL;
    HASH_FIND_INT(m_pHashForUpdates, &pTarget, pElementUpdate);
    if (pElementUpdate)
    {
        CCAssert(pElementUpdate->entry != NULL, "");
        pElementUpdate->entry->paused = false;
    }
}

void CAScheduler::pauseTarget(CAObject *pTarget)
{
    CCAssert(pTarget != NULL, "");

    // custom selectors
    tHashTimerEntry *pElement = NULL;
    HASH_FIND_INT(m_pHashForTimers, &pTarget, pElement);
    if (pElement)
    {
        pElement->paused = true;
    }

    // update selector
    tHashUpdateEntry *pElementUpdate = NULL;
    HASH_FIND_INT(m_pHashForUpdates, &pTarget, pElementUpdate);
    if (pElementUpdate)
    {
        CCAssert(pElementUpdate->entry != NULL, "");
        pElementUpdate->entry->paused = true;
    }
}

bool CAScheduler::isTargetPaused(CAObject *pTarget)
{
    CCAssert( pTarget != NULL, "target must be non nil" );

    // Custom selectors
    tHashTimerEntry *pElement = NULL;
    HASH_FIND_INT(m_pHashForTimers, &pTarget, pElement);
    if( pElement )
    {
        return pElement->paused;
    }
    
    // We should check update selectors if target does not have custom selectors
	tHashUpdateEntry *elementUpdate = NULL;
	HASH_FIND_INT(m_pHashForUpdates, &pTarget, elementUpdate);
	if ( elementUpdate )
    {
		return elementUpdate->entry->paused;
    }
    
    return false;  // should never get here
}

CCSet* CAScheduler::pauseAllTargets()
{
    return pauseAllTargetsWithMinPriority(kCCPrioritySystem);
}

CCSet* CAScheduler::pauseAllTargetsWithMinPriority(int nMinPriority)
{
    CCSet* idsWithSelectors = new CCSet();// setWithCapacity:50];
    idsWithSelectors->autorelease();

    // Custom Selectors
    for(tHashTimerEntry *element = m_pHashForTimers; element != NULL;
        element = (tHashTimerEntry*)element->hh.next)
    {
        element->paused = true;
        idsWithSelectors->addObject(element->target);
    }

    // Updates selectors
    tListEntry *entry, *tmp;
    if(nMinPriority < 0) 
    {
        DL_FOREACH_SAFE( m_pUpdatesNegList, entry, tmp ) 
        {
            if(entry->priority >= nMinPriority) 
            {
                entry->paused = true;
                idsWithSelectors->addObject(entry->target);
            }
        }
    }

    if(nMinPriority <= 0) 
    {
        DL_FOREACH_SAFE( m_pUpdates0List, entry, tmp )
        {
            entry->paused = true;
            idsWithSelectors->addObject(entry->target);
        }
    }

    DL_FOREACH_SAFE( m_pUpdatesPosList, entry, tmp ) 
    {
        if(entry->priority >= nMinPriority) 
        {
            entry->paused = true;
            idsWithSelectors->addObject(entry->target);
        }
    }

    return idsWithSelectors;
}

void CAScheduler::resumeTargets(CCSet* pTargetsToResume)
{
    CCSetIterator iter;
    for (iter = pTargetsToResume->begin(); iter != pTargetsToResume->end(); ++iter)
    {
        resumeTarget(*iter);
    }
}

// main loop
void CAScheduler::update(float dt)
{
    m_bUpdateHashLocked = true;

    if (m_fTimeScale != 1.0f)
    {
        dt *= m_fTimeScale;
    }

    // Iterate over all the Updates' selectors
    tListEntry *pEntry, *pTmp;

    // updates with priority < 0
    DL_FOREACH_SAFE(m_pUpdatesNegList, pEntry, pTmp)
    {
        if ((! pEntry->paused) && (! pEntry->markedForDeletion))
        {
            pEntry->target->update(dt);
        }
    }

    // updates with priority == 0
    DL_FOREACH_SAFE(m_pUpdates0List, pEntry, pTmp)
    {
        if ((! pEntry->paused) && (! pEntry->markedForDeletion))
        {
            pEntry->target->update(dt);
        }
    }

    // updates with priority > 0
    DL_FOREACH_SAFE(m_pUpdatesPosList, pEntry, pTmp)
    {
        if ((! pEntry->paused) && (! pEntry->markedForDeletion))
        {
            pEntry->target->update(dt);
        }
    }

    // Iterate over all the custom selectors
    for (tHashTimerEntry *elt = m_pHashForTimers; elt != NULL; )
    {
        m_pCurrentTarget = elt;
        m_bCurrentTargetSalvaged = false;

        if (! m_pCurrentTarget->paused)
        {
            // The 'timers' array may change while inside this loop
            for (elt->timerIndex = 0; elt->timerIndex < elt->timers->num; ++(elt->timerIndex))
            {
                elt->currentTimer = (CATimer*)(elt->timers->arr[elt->timerIndex]);
                elt->currentTimerSalvaged = false;

                elt->currentTimer->update(dt);

                if (elt->currentTimerSalvaged)
                {
                    // The currentTimer told the remove itself. To prevent the timer from
                    // accidentally deallocating itself before finishing its step, we retained
                    // it. Now that step is done, it's safe to release it.
                    elt->currentTimer->release();
                }

                elt->currentTimer = NULL;
            }
        }

        // elt, at this moment, is still valid
        // so it is safe to ask this here (issue #490)
        elt = (tHashTimerEntry *)elt->hh.next;

        // only delete currentTarget if no actions were scheduled during the cycle (issue #481)
        if (m_bCurrentTargetSalvaged && m_pCurrentTarget->timers->num == 0)
        {
            removeHashElement(m_pCurrentTarget);
        }
    }

    // delete all updates that are marked for deletion
    // updates with priority < 0
    DL_FOREACH_SAFE(m_pUpdatesNegList, pEntry, pTmp)
    {
        if (pEntry->markedForDeletion)
        {
            this->removeUpdateFromHash(pEntry);
        }
    }

    // updates with priority == 0
    DL_FOREACH_SAFE(m_pUpdates0List, pEntry, pTmp)
    {
        if (pEntry->markedForDeletion)
        {
            this->removeUpdateFromHash(pEntry);
        }
    }

    // updates with priority > 0
    DL_FOREACH_SAFE(m_pUpdatesPosList, pEntry, pTmp)
    {
        if (pEntry->markedForDeletion)
        {
            this->removeUpdateFromHash(pEntry);
        }
    }

    m_bUpdateHashLocked = false;

    m_pCurrentTarget = NULL;
}


NS_CC_END

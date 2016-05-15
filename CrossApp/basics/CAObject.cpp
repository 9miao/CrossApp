

#include "CAObject.h"
#include "CAAutoreleasePool.h"
#include "ccMacros.h"
#include "CAScheduler.h"
#include "script_support/CCScriptSupport.h"
NS_CC_BEGIN

CAObject* CACopying::copyWithZone(CAZone *pZone)
{
    CC_UNUSED_PARAM(pZone);
    CCAssert(0, "not implement");
    return 0;
}

CAObject::CAObject(void)
: m_uReference(1) // when the object is created, the reference count of it is 1
, m_uAutoReleaseCount(0)
, m_nTag(kCAObjectTagInvalid)
, m_pUserData(NULL)
, m_pUserObject(NULL)
{
    static unsigned int uObjectCount = 0;

    m_u__ID = ++uObjectCount;
    
    char str[32];
    sprintf(str, "%u", m_u__ID);
    m_s__StrID = str;
}

CAObject::~CAObject(void)
{
    CAScheduler::unscheduleAllForTarget(this);

	releaseAllDelays();
    
    CC_SAFE_RELEASE(m_pUserObject);
    
    if (m_uAutoReleaseCount > 0)
    {
        CAPoolManager::sharedPoolManager()->removeObject(this);
    }
    
    CCScriptEngineProtocol* pEngine = CCScriptEngineManager::sharedManager()->getScriptEngine();
    if (pEngine != NULL && pEngine->getScriptType() == kScriptTypeJavascript)
    {
        pEngine->removeScriptObjectByCCObject(this);
    }
}

CAObject* CAObject::copy()
{
    return copyWithZone(0);
}

void CAObject::release(void)
{
    --m_uReference;

    if (m_uReference == 0)
    {
        delete this;
    }
}

CAObject* CAObject::retain(void)
{
    ++m_uReference;
    return this;
}

CAObject* CAObject::autorelease(void)
{
    CAPoolManager::sharedPoolManager()->addObject(this);
    return this;
}

bool CAObject::isSingleReference(void) const
{
    return m_uReference == 1;
}

unsigned int CAObject::retainCount(void) const
{
    return m_uReference;
}

bool CAObject::isEqual(const CAObject *pObject)
{
    return this == pObject;
}

void CAObject::performSelector(SEL_CallFunc callFunc, float afterDelay)
{
	tDelayTimerElement t;
	t.func1 = callFunc;
	t.fInterval = afterDelay;
	m_vDelayTEVect.push_back(t);
	CAScheduler::schedule(schedule_selector(CAObject::updateDelayTimers), this, 0, kCCRepeatForever, 1 / 60);
}

void CAObject::performSelector(SEL_CallFuncO callFunc, CAObject* objParam, float afterDelay)
{
	tDelayTimerElement t;
	if (objParam)
	{
		objParam->retain();
	}
	t.pObj = objParam;
	t.func2 = callFunc;
	t.fInterval = afterDelay;
	m_vDelayTEVect.push_back(t);
	CAScheduler::schedule(schedule_selector(CAObject::updateDelayTimers), this, 0, kCCRepeatForever, 1 / 60);
}

void CAObject::updateDelayTimers(float dt)
{
	std::vector<tDelayTimerElement>::iterator it = m_vDelayTEVect.begin();
	while (it != m_vDelayTEVect.end())
	{
		it->fCurrentTime += dt;

		if (it->fInterval <= it->fCurrentTime)
		{
			if (it->func1)
			{
				(this->*it->func1)();
			}
			if (it->func2)
			{
				(this->*it->func2)(it->pObj);
			}
			CC_SAFE_RELEASE(it->pObj);
			it = m_vDelayTEVect.erase(it);
		}
		else
		{
			it++;
		}
	}

	if (m_vDelayTEVect.empty())
	{
		CAScheduler::unschedule(schedule_selector(CAObject::updateDelayTimers), this);
	}
}

void CAObject::releaseAllDelays()
{
	for (int i = 0; i < m_vDelayTEVect.size(); i++)
	{
		CC_SAFE_RELEASE(m_vDelayTEVect[i].pObj);
	}
	m_vDelayTEVect.clear();
}

CAZone::CAZone(CAObject *pObject)
{
    m_pCopyObject = pObject;
}

NS_CC_END

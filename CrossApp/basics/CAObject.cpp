

#include "CAObject.h"
#include "CAAutoreleasePool.h"
#include "ccMacros.h"
#include "CAScheduler.h"
#include "script_support/CCScriptSupport.h"

NS_CC_BEGIN


typedef struct DelayTimerElement
{
	DelayTimerElement()
		: pOwnerObj(NULL)
		, func1(NULL)
		, func2(NULL)
		, pObj(NULL)
		, fInterval(0)
		, fCurrentTime(0)
	{

	}
	CAObject* pOwnerObj;
	SEL_CallFunc func1;
	SEL_CallFuncO func2;
	CAObject* pObj;
	float fInterval;
	float fCurrentTime;
}
tDelayTimerElement;


class CAObjectHelper : public CAObject
{
public:

	CAObjectHelper(void) {}

	virtual ~CAObjectHelper(void)
	{
		releaseAllDelays();
	}

	static CAObjectHelper* getInstance()
	{
		static CAObjectHelper instance;
		return &instance;
	}

	void performSelector(CAObject* pOwnerObj, SEL_CallFunc callFunc, float afterDelay)
	{
		CCAssert(pOwnerObj, "");
		pOwnerObj->retain();
		tDelayTimerElement t;
		t.pOwnerObj = pOwnerObj;
		t.func1 = callFunc;
		t.fInterval = afterDelay;
		m_vDelayTimerVect.AddElement(t);
	}

	void performSelector(CAObject* pOwnerObj, SEL_CallFuncO callFunc, CAObject* objParam, float afterDelay)
	{
		CCAssert(pOwnerObj, "");
		pOwnerObj->retain();
		tDelayTimerElement t;
		t.pOwnerObj = pOwnerObj;
		if (objParam)
		{
			objParam->retain();
		}
		t.pObj = objParam;
		t.func2 = callFunc;
		t.fInterval = afterDelay;
		m_vDelayTimerVect.AddElement(t);
	}

	void updateDelayTimers(float dt)
	{
		std::vector<tDelayTimerElement> vTimers = m_vDelayTimerVect.GetQueueElements();

		std::vector<tDelayTimerElement>::iterator it = vTimers.begin();
		while (it != vTimers.end())
		{
			it->fCurrentTime += dt;

			if (it->fInterval <= it->fCurrentTime)
			{
				if (it->func1)
				{
					(it->pOwnerObj->*it->func1)();
				}
				if (it->func2)
				{
					(it->pOwnerObj->*it->func2)(it->pObj);
				}
				CC_SAFE_RELEASE(it->pOwnerObj);
				CC_SAFE_RELEASE(it->pObj);
				it = vTimers.erase(it);
			}
			else
			{
				it++;
			}
		}
		for (size_t i = 0; i < vTimers.size(); i++)
		{
			m_vDelayTimerVect.AddElement(vTimers[i]);
		}
	}

	void releaseAllDelays()
	{
		std::vector<tDelayTimerElement> vTimers = m_vDelayTimerVect.GetQueueElements();
		for (int i = 0; i < vTimers.size(); i++)
		{
			CC_SAFE_RELEASE(vTimers[i].pOwnerObj);
			CC_SAFE_RELEASE(vTimers[i].pObj);
		}
		m_vDelayTimerVect.Clear();
	}

private:
	CASyncQueue<tDelayTimerElement> m_vDelayTimerVect;
};


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
	CAObjectHelper::getInstance()->performSelector(this, callFunc, afterDelay);
}

void CAObject::performSelector(SEL_CallFuncO callFunc, CAObject* objParam, float afterDelay)
{
	CAObjectHelper::getInstance()->performSelector(this, callFunc, objParam, afterDelay);
}

void CAObject::updateDelayTimers(float t)
{
	CAObjectHelper::getInstance()->updateDelayTimers(t);
}

CAZone::CAZone(CAObject *pObject)
{
    m_pCopyObject = pObject;
}



NS_CC_END

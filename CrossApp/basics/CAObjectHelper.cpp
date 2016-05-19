

#include "CAObjectHelper.h"
#include "CAAutoreleasePool.h"
#include "ccMacros.h"
#include "CAScheduler.h"

NS_CC_BEGIN

CAObjectHelper::CAObjectHelper(void)
{
	CAScheduler::schedule(schedule_selector(CAObjectHelper::updateDelayTimers), this, 0, kCCRepeatForever, 1 / 60);
}

CAObjectHelper::~CAObjectHelper(void)
{
    CAScheduler::unscheduleAllForTarget(this);
    
	releaseAllDelays();
}

CAObjectHelper* CAObjectHelper::getInstance()
{
	static CAObjectHelper instance;
	return &instance;
}

void CAObjectHelper::performSelector(CAObject* pOwnerObj, SEL_CallFunc callFunc, float afterDelay)
{
	CCAssert(pOwnerObj, "");
	pOwnerObj->retain();
	tDelayTimerElement t;
	t.pOwnerObj = pOwnerObj;
	t.func1 = callFunc;
	t.fInterval = afterDelay;
	m_vDelayTimerVect.AddElement(t);
}

void CAObjectHelper::performSelector(CAObject* pOwnerObj, SEL_CallFuncO callFunc, CAObject* objParam, float afterDelay)
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

void CAObjectHelper::updateDelayTimers(float dt)
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

void CAObjectHelper::releaseAllDelays()
{
	std::vector<tDelayTimerElement> vTimers = m_vDelayTimerVect.GetQueueElements();
	for (int i = 0; i < vTimers.size(); i++)
	{
		CC_SAFE_RELEASE(vTimers[i].pOwnerObj);
		CC_SAFE_RELEASE(vTimers[i].pObj);
	}
	m_vDelayTimerVect.Clear();
}


NS_CC_END

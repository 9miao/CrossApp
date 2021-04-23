

#include "CAObject.h"
#include "basics/CAAutoreleasePool.h"
#include "ccMacros.h"
#include "basics/CAScheduler.h"
#include "basics/CANotificationCenter.h"
#include "script_support/CAScriptSupport.h"

NS_CC_BEGIN

CAObject::CAObject(void)
: m_uReference(1) // when the object is created, the reference count of it is 1
, m_uAutoReleaseCount(0)
, m_nTag(TagInvalid)
, m_pUserData(nullptr)
, m_pUserObject(nullptr)
{
    static unsigned int uObjectCount = 0;

    m_u__ID = ++uObjectCount;

    char str[8];
    sprintf(str, "%x", m_u__ID);
    m_s__StrID = str;
}

CAObject::~CAObject(void)
{
	if (CANotificationCenter* notificationCenter = CANotificationCenter::getInstance())
	{
		notificationCenter->removeAllObservers(this);
	}
	if (CAScheduler* scheduler = CAScheduler::getScheduler())
	{
		scheduler->unscheduleAllForTarget(this);;
	}
    
    CC_SAFE_RELEASE(m_pUserObject);
    
    if (m_uAutoReleaseCount > 0)
    {
        CAPoolManager::getInstance()->removeObject(this);
    }
    
    CAScriptEngineManager* pEngineManager = CAScriptEngineManager::getScriptEngineManager();
    if (pEngineManager)
    {
        CAScriptEngineProtocol* pEngine = pEngineManager->getScriptEngine();
        if (pEngine != NULL && pEngine->getScriptType() == kScriptTypeJavascript)
        {
            pEngine->removeScriptObjectByObject(this);
        }
    }
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
    CAPoolManager::getInstance()->addObject(this);
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
    CAScheduler::getScheduler()->scheduleOnce([=](float dt)
    {
        (this->*callFunc)();
    }, crossapp_format_string("perform:%x", callFunc), this, afterDelay);
}

void CAObject::performSelector(SEL_CallFuncO callFunc, CAObject* objParam, float afterDelay)
{
    CAScheduler::getScheduler()->scheduleOnce([&](float dt)
    {
        (this->*callFunc)(objParam);
    }, crossapp_format_string("performO:%x", callFunc), this, afterDelay);
}

void CAObject::cancelPreviousPerformRequests(SEL_CallFunc callFunc)
{
    CAScheduler::getScheduler()->unschedule(crossapp_format_string("perform:%x", callFunc), this);
}

void CAObject::cancelPreviousPerformRequests(SEL_CallFuncO callFunc)
{
    CAScheduler::getScheduler()->unschedule(crossapp_format_string("performO:%x", callFunc), this);
}


NS_CC_END

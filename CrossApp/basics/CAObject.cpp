

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
{
    static unsigned int uObjectCount = 0;

    m_uID = ++uObjectCount;
}

CAObject::~CAObject(void)
{
    CAScheduler::unscheduleAllForTarget(this);
    
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

CAZone::CAZone(CAObject *pObject)
{
    m_pCopyObject = pObject;
}

NS_CC_END

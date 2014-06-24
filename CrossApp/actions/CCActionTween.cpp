

#include "CCActionTween.h"

NS_CC_BEGIN

CCActionTween* CCActionTween::create(float aDuration, const char* key, float from, float to)
{
    CCActionTween* pRet = new CCActionTween();
    if (pRet && pRet->initWithDuration(aDuration, key, from, to))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCActionTween::initWithDuration(float aDuration, const char* key, float from, float to)
{
    if (CCActionInterval::initWithDuration(aDuration)) 
    {
        m_strKey    = key;
        m_fTo       = to;
        m_fFrom     = from;
        return true;
    }

    return false;
}

void CCActionTween::startWithTarget(CAView *pTarget)
{
    CCAssert(dynamic_cast<CCActionTweenDelegate*>(pTarget), "target must implement CCActionTweenDelegate");
    CCActionInterval::startWithTarget(pTarget);
    m_fDelta = m_fTo - m_fFrom;
}

void CCActionTween::update(float dt)
{
    dynamic_cast<CCActionTweenDelegate*>(m_pTarget)->updateTweenAction(m_fTo  - m_fDelta * (1 - dt), m_strKey.c_str());
}

CCActionInterval* CCActionTween::reverse()
{
    return CCActionTween::create(m_fDuration, m_strKey.c_str(), m_fTo, m_fFrom);
}


NS_CC_END

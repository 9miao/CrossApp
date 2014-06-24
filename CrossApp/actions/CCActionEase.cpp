

/*
 * Elastic, Back and Bounce actions based on code from:
 * http://github.com/NikhilK/silverlightfx/
 *
 * by http://github.com/NikhilK
 */

#include "CCActionEase.h"

NS_CC_BEGIN

#ifndef M_PI_X_2
#define M_PI_X_2 (float)M_PI * 2.0f
#endif

//
// EaseAction
//

CCActionEase* CCActionEase::create(CCActionInterval *pAction)
{
    CCActionEase *pRet = new CCActionEase();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

bool CCActionEase::initWithAction(CCActionInterval *pAction)
{
    CCAssert(pAction != NULL, "");

    if (CCActionInterval::initWithDuration(pAction->getDuration()))
    {
        m_pInner = pAction;
        pAction->retain();

        return true;
    }

    return false;
}

CAObject* CCActionEase::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCActionEase* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCActionEase*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCActionEase();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCActionEase::~CCActionEase(void)
{
    CC_SAFE_RELEASE(m_pInner);
}

void CCActionEase::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_pInner->startWithTarget(m_pTarget);
}

void CCActionEase::stop(void)
{
    m_pInner->stop();
    CCActionInterval::stop();
}

void CCActionEase::update(float time)
{
    m_pInner->update(time);
}

CCActionInterval* CCActionEase::reverse(void)
{
    return CCActionEase::create(m_pInner->reverse());
}

CCActionInterval* CCActionEase::getInnerAction()
{
    return m_pInner;
}

//
// EaseRateAction
//

CCEaseRateAction* CCEaseRateAction::create(CCActionInterval *pAction, float fRate)
{
    CCEaseRateAction *pRet = new CCEaseRateAction();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fRate))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

bool CCEaseRateAction::initWithAction(CCActionInterval *pAction, float fRate)
{
    if (CCActionEase::initWithAction(pAction))
    {
        m_fRate = fRate;
        return true;
    }

    return false;
}

CAObject* CCEaseRateAction::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseRateAction* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseRateAction*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseRateAction();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval*)(m_pInner->copy()->autorelease()), m_fRate);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCEaseRateAction::~CCEaseRateAction(void)
{
}

CCActionInterval* CCEaseRateAction::reverse(void)
{
    return CCEaseRateAction::create(m_pInner->reverse(), 1 / m_fRate);
}

//
// EeseIn
//

CCEaseIn* CCEaseIn::create(CCActionInterval *pAction, float fRate)
{
    CCEaseIn *pRet = new CCEaseIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fRate))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

CAObject* CCEaseIn::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseIn* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseIn*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseIn();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval*)(m_pInner->copy()->autorelease()), m_fRate);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseIn::update(float time)
{
    m_pInner->update(powf(time, m_fRate));
}

CCActionInterval* CCEaseIn::reverse(void)
{
    return CCEaseIn::create(m_pInner->reverse(), 1 / m_fRate);
}

//
// EaseOut
//
CCEaseOut* CCEaseOut::create(CCActionInterval *pAction, float fRate)
{
    CCEaseOut *pRet = new CCEaseOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fRate))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;   
}

CAObject* CCEaseOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval*)(m_pInner->copy()->autorelease()), m_fRate);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseOut::update(float time)
{
    m_pInner->update(powf(time, 1 / m_fRate));
}

CCActionInterval* CCEaseOut::reverse()
{
    return CCEaseOut::create(m_pInner->reverse(), 1 / m_fRate);
}

//
// EaseInOut
//
CCEaseInOut* CCEaseInOut::create(CCActionInterval *pAction, float fRate)
{
    CCEaseInOut *pRet = new CCEaseInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fRate))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseInOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseInOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseInOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseInOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval*)(m_pInner->copy()->autorelease()), m_fRate);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseInOut::update(float time)
{
    time *= 2;
    if (time < 1)
    {
        m_pInner->update(0.5f * powf(time, m_fRate));
    }
    else
    {
        m_pInner->update(1.0f - 0.5f * powf(2-time, m_fRate));
    }
}

// InOut and OutIn are symmetrical
CCActionInterval* CCEaseInOut::reverse(void)
{
    return CCEaseInOut::create(m_pInner->reverse(), m_fRate);
}

//
// EaseExponentialIn
//
CCEaseExponentialIn* CCEaseExponentialIn::create(CCActionInterval* pAction)
{
    CCEaseExponentialIn *pRet = new CCEaseExponentialIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;    
}

CAObject* CCEaseExponentialIn::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseExponentialIn* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseExponentialIn*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseExponentialIn();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseExponentialIn::update(float time)
{
    m_pInner->update(time == 0 ? 0 : powf(2, 10 * (time/1 - 1)) - 1 * 0.001f);
}

CCActionInterval* CCEaseExponentialIn::reverse(void)
{
    return CCEaseExponentialOut::create(m_pInner->reverse());
}

//
// EaseExponentialOut
//
CCEaseExponentialOut* CCEaseExponentialOut::create(CCActionInterval* pAction)
{
    CCEaseExponentialOut *pRet = new CCEaseExponentialOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseExponentialOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseExponentialOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseExponentialOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseExponentialOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseExponentialOut::update(float time)
{
    m_pInner->update(time == 1 ? 1 : (-powf(2, -10 * time / 1) + 1));
}

CCActionInterval* CCEaseExponentialOut::reverse(void)
{
    return CCEaseExponentialIn::create(m_pInner->reverse());
}

//
// EaseExponentialInOut
//

CCEaseExponentialInOut* CCEaseExponentialInOut::create(CCActionInterval *pAction)
{
    CCEaseExponentialInOut *pRet = new CCEaseExponentialInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseExponentialInOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseExponentialInOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseExponentialInOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseExponentialInOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseExponentialInOut::update(float time)
{
    time /= 0.5f;
    if (time < 1)
    {
        time = 0.5f * powf(2, 10 * (time - 1));
    }
    else
    {
        time = 0.5f * (-powf(2, -10 * (time - 1)) + 2);
    }

    m_pInner->update(time);
}

CCActionInterval* CCEaseExponentialInOut::reverse()
{
    return CCEaseExponentialInOut::create(m_pInner->reverse());
}

//
// EaseSineIn
//

CCEaseSineIn* CCEaseSineIn::create(CCActionInterval* pAction)
{
    CCEaseSineIn *pRet = new CCEaseSineIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseSineIn::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseSineIn* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCEaseSineIn*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseSineIn();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseSineIn::update(float time)
{
    m_pInner->update(-1 * cosf(time * (float)M_PI_2) + 1);
}

CCActionInterval* CCEaseSineIn::reverse(void)
{
    return CCEaseSineOut::create(m_pInner->reverse());
}

//
// EaseSineOut
//

CCEaseSineOut* CCEaseSineOut::create(CCActionInterval* pAction)
{
    CCEaseSineOut *pRet = new CCEaseSineOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseSineOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseSineOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseSineOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseSineOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseSineOut::update(float time)
{
    m_pInner->update(sinf(time * (float)M_PI_2));
}

CCActionInterval* CCEaseSineOut::reverse(void)
{
    return CCEaseSineIn::create(m_pInner->reverse());
}

//
// EaseSineInOut
//

CCEaseSineInOut* CCEaseSineInOut::create(CCActionInterval* pAction)
{
    CCEaseSineInOut *pRet = new CCEaseSineInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseSineInOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseSineInOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseSineInOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseSineInOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseSineInOut::update(float time)
{
    m_pInner->update(-0.5f * (cosf((float)M_PI * time) - 1));
}

CCActionInterval* CCEaseSineInOut::reverse()
{
    return CCEaseSineInOut::create(m_pInner->reverse());
}

//
// EaseElastic
//

CCEaseElastic* CCEaseElastic::create(CCActionInterval *pAction)
{
    return CCEaseElastic::create(pAction, 0.3f);
}

CCEaseElastic* CCEaseElastic::create(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    CCEaseElastic *pRet = new CCEaseElastic();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fPeriod))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

bool CCEaseElastic::initWithAction(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    if (CCActionEase::initWithAction(pAction))
    {
        m_fPeriod = fPeriod;
        return true;
    }

    return false;
}

CAObject* CCEaseElastic::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseElastic* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseElastic*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseElastic();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()), m_fPeriod);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCActionInterval* CCEaseElastic::reverse(void)
{
    CCAssert(0, "Override me");

    return NULL;
}

//
// EaseElasticIn
//

CCEaseElasticIn* CCEaseElasticIn::create(CCActionInterval *pAction)
{
    return CCEaseElasticIn::create(pAction, 0.3f);
}

CCEaseElasticIn* CCEaseElasticIn::create(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    CCEaseElasticIn *pRet = new CCEaseElasticIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fPeriod))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseElasticIn::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseElasticIn* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseElasticIn*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseElasticIn();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()), m_fPeriod);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseElasticIn::update(float time)
{
    float newT = 0;
    if (time == 0 || time == 1)
    {
        newT = time;
    }
    else
    {
        float s = m_fPeriod / 4;
        time = time - 1;
        newT = -powf(2, 10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod);
    }

    m_pInner->update(newT);
}

CCActionInterval* CCEaseElasticIn::reverse(void)
{
    return CCEaseElasticOut::create(m_pInner->reverse(), m_fPeriod);
}

//
// EaseElasticOut
//

CCEaseElasticOut* CCEaseElasticOut::create(CCActionInterval *pAction)
{
    return CCEaseElasticOut::create(pAction, 0.3f);
}

CCEaseElasticOut* CCEaseElasticOut::create(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    CCEaseElasticOut *pRet = new CCEaseElasticOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fPeriod))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject *CCEaseElasticOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseElasticOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseElasticOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseElasticOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()), m_fPeriod);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseElasticOut::update(float time)
{
    float newT = 0;
    if (time == 0 || time == 1)
    {
        newT = time;
    }
    else
    {
        float s = m_fPeriod / 4;
        newT = powf(2, -10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod) + 1;
    }

    m_pInner->update(newT);
}

CCActionInterval* CCEaseElasticOut::reverse(void)
{
    return CCEaseElasticIn::create(m_pInner->reverse(), m_fPeriod);
}

//
// EaseElasticInOut
//

CCEaseElasticInOut* CCEaseElasticInOut::create(CCActionInterval *pAction)
{
    return CCEaseElasticInOut::create(pAction, 0.3f);
}

CCEaseElasticInOut* CCEaseElasticInOut::create(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    CCEaseElasticInOut *pRet = new CCEaseElasticInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fPeriod))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseElasticInOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseElasticInOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseElasticInOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseElasticInOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()), m_fPeriod);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;

}

void CCEaseElasticInOut::update(float time)
{
    float newT = 0;
    if (time == 0 || time == 1)
    {
        newT = time;
    }
    else
    {
        time = time * 2;
        if (! m_fPeriod)
        {
            m_fPeriod = 0.3f * 1.5f;
        }

        float s = m_fPeriod / 4;

        time = time - 1;
        if (time < 0)
        {
            newT = -0.5f * powf(2, 10 * time) * sinf((time -s) * M_PI_X_2 / m_fPeriod);
        }
        else
        {
            newT = powf(2, -10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod) * 0.5f + 1;
        }
    }

    m_pInner->update(newT);
}

CCActionInterval* CCEaseElasticInOut::reverse(void)
{
    return CCEaseElasticInOut::create(m_pInner->reverse(), m_fPeriod);
}

//
// EaseBounce
//

CCEaseBounce* CCEaseBounce::create(CCActionInterval* pAction)
{
    CCEaseBounce *pRet = new CCEaseBounce();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseBounce::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseBounce* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseBounce*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseBounce();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

float CCEaseBounce::bounceTime(float time)
{
    if (time < 1 / 2.75)
    {
        return 7.5625f * time * time;
    } else 
    if (time < 2 / 2.75)
    {
        time -= 1.5f / 2.75f;
        return 7.5625f * time * time + 0.75f;
    } else
    if(time < 2.5 / 2.75)
    {
        time -= 2.25f / 2.75f;
        return 7.5625f * time * time + 0.9375f;
    }

    time -= 2.625f / 2.75f;
    return 7.5625f * time * time + 0.984375f;
}

CCActionInterval* CCEaseBounce::reverse()
{
    return CCEaseBounce::create(m_pInner->reverse());
}

//
// EaseBounceIn
//

CCEaseBounceIn* CCEaseBounceIn::create(CCActionInterval* pAction)
{
    CCEaseBounceIn *pRet = new CCEaseBounceIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseBounceIn::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseBounceIn* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseBounceIn*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseBounceIn();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseBounceIn::update(float time)
{
    float newT = 1 - bounceTime(1 - time);
    m_pInner->update(newT);
}

CCActionInterval* CCEaseBounceIn::reverse(void)
{
    return CCEaseBounceOut::create(m_pInner->reverse());
}

//
// EaseBounceOut
//

CCEaseBounceOut* CCEaseBounceOut::create(CCActionInterval* pAction)
{
    CCEaseBounceOut *pRet = new CCEaseBounceOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseBounceOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseBounceOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCEaseBounceOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseBounceOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseBounceOut::update(float time)
{
    float newT = bounceTime(time);
    m_pInner->update(newT);
}

CCActionInterval* CCEaseBounceOut::reverse(void)
{
    return CCEaseBounceIn::create(m_pInner->reverse());
}

//
// EaseBounceInOut
//

CCEaseBounceInOut* CCEaseBounceInOut::create(CCActionInterval* pAction)
{
    CCEaseBounceInOut *pRet = new CCEaseBounceInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

CAObject* CCEaseBounceInOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseBounceInOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseBounceInOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseBounceInOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseBounceInOut::update(float time)
{
    float newT = 0;
    if (time < 0.5f)
    {
        time = time * 2;
        newT = (1 - bounceTime(1 - time)) * 0.5f;
    }
    else
    {
        newT = bounceTime(time * 2 - 1) * 0.5f + 0.5f;
    }

    m_pInner->update(newT);
}

CCActionInterval* CCEaseBounceInOut::reverse()
{
    return CCEaseBounceInOut::create(m_pInner->reverse());
}

//
// EaseBackIn
//

CCEaseBackIn* CCEaseBackIn::create(CCActionInterval *pAction)
{
    CCEaseBackIn *pRet = new CCEaseBackIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

CAObject* CCEaseBackIn::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseBackIn* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseBackIn*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseBackIn();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseBackIn::update(float time)
{
    float overshoot = 1.70158f;
    m_pInner->update(time * time * ((overshoot + 1) * time - overshoot));
}

CCActionInterval* CCEaseBackIn::reverse(void)
{
    return CCEaseBackOut::create(m_pInner->reverse());
}

//
// EaseBackOut
//

CCEaseBackOut* CCEaseBackOut::create(CCActionInterval* pAction)
{
    CCEaseBackOut *pRet = new CCEaseBackOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

CAObject* CCEaseBackOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseBackOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseBackOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseBackOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseBackOut::update(float time)
{
    float overshoot = 1.70158f;

    time = time - 1;
    m_pInner->update(time * time * ((overshoot + 1) * time + overshoot) + 1);
}

CCActionInterval* CCEaseBackOut::reverse(void)
{
    return CCEaseBackIn::create(m_pInner->reverse());
}

//
// EaseBackInOut
//

CCEaseBackInOut* CCEaseBackInOut::create(CCActionInterval* pAction)
{
    CCEaseBackInOut *pRet = new CCEaseBackInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

CAObject* CCEaseBackInOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCEaseBackInOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCEaseBackInOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCEaseBackInOut();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCEaseBackInOut::update(float time)
{
    float overshoot = 1.70158f * 1.525f;

    time = time * 2;
    if (time < 1)
    {
        m_pInner->update((time * time * ((overshoot + 1) * time - overshoot)) / 2);
    }
    else
    {
        time = time - 2;
        m_pInner->update((time * time * ((overshoot + 1) * time + overshoot)) / 2 + 1);
    }
}

CCActionInterval* CCEaseBackInOut::reverse()
{
    return CCEaseBackInOut::create(m_pInner->reverse());
}

NS_CC_END

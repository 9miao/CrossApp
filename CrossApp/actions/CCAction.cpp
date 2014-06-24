

#include "CCAction.h"
#include "CCActionInterval.h"
#include "view/CAView.h"
#include "support/CCPointExtension.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN
//
// Action Base Class
//

CCAction::CCAction()
:m_pOriginalTarget(NULL)
,m_pTarget(NULL)
,m_nTag(kCCActionTagInvalid)
{
}

CCAction::~CCAction()
{
    CCLOGINFO("CrossApp: deallocing");
}

CCAction* CCAction::create()
{
    CCAction * pRet = new CCAction();
    pRet->autorelease();
    return pRet;
}

const char* CCAction::description()
{
    return CCString::createWithFormat("<CCAction | Tag = %d>", m_nTag)->getCString();
}

CAObject* CCAction::copyWithZone(CAZone *pZone)
{
    CAZone *pNewZone = NULL;
    CCAction *pRet = NULL;
    if (pZone && pZone->m_pCopyObject)
    {
        pRet = (CCAction*)(pZone->m_pCopyObject);
    }
    else
    {
        pRet = new CCAction();
        pNewZone = new CAZone(pRet);
    }
    //copy member data
    pRet->m_nTag = m_nTag;
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCAction::startWithTarget(CAView *aTarget)
{
    m_pOriginalTarget = m_pTarget = aTarget;
}

void CCAction::stop()
{
    m_pTarget = NULL;
}

bool CCAction::isDone()
{
    return true;
}

void CCAction::step(float dt)
{
    CC_UNUSED_PARAM(dt);
    CCLOG("[Action step]. override me");
}

void CCAction::update(float time)
{
    CC_UNUSED_PARAM(time);
    CCLOG("[Action update]. override me");
}

//
// FiniteTimeAction
//

CCFiniteTimeAction *CCFiniteTimeAction::reverse()
{
    CCLOG("CrossApp: FiniteTimeAction#reverse: Implement me");
    return NULL;
}

//
// Speed
//
CCSpeed::~CCSpeed()
{
    CC_SAFE_RELEASE(m_pInnerAction);
}

CCSpeed* CCSpeed::create(CCActionInterval* pAction, float fSpeed)
{
    CCSpeed *pRet = new CCSpeed();
    if (pRet && pRet->initWithAction(pAction, fSpeed))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCSpeed::initWithAction(CCActionInterval *pAction, float fSpeed)
{
    CCAssert(pAction != NULL, "");
    pAction->retain();
    m_pInnerAction = pAction;
    m_fSpeed = fSpeed;    
    return true;
}

CAObject *CCSpeed::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCSpeed* pRet = NULL;
    if(pZone && pZone->m_pCopyObject) //in case of being called at sub class
    {
        pRet = (CCSpeed*)(pZone->m_pCopyObject);
    }
    else
    {
        pRet = new CCSpeed();
        pZone = pNewZone = new CAZone(pRet);
    }
    CCAction::copyWithZone(pZone);

    pRet->initWithAction( (CCActionInterval*)(m_pInnerAction->copy()->autorelease()) , m_fSpeed );
    
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCSpeed::startWithTarget(CAView* pTarget)
{
    CCAction::startWithTarget(pTarget);
    m_pInnerAction->startWithTarget(pTarget);
}

void CCSpeed::stop()
{
    m_pInnerAction->stop();
    CCAction::stop();
}

void CCSpeed::step(float dt)
{
    m_pInnerAction->step(dt * m_fSpeed);
}

bool CCSpeed::isDone()
{
    return m_pInnerAction->isDone();
}

CCActionInterval *CCSpeed::reverse()
{
     return (CCActionInterval*)(CCSpeed::create(m_pInnerAction->reverse(), m_fSpeed));
}

void CCSpeed::setInnerAction(CCActionInterval *pAction)
{
    if (m_pInnerAction != pAction)
    {
        CC_SAFE_RELEASE(m_pInnerAction);
        m_pInnerAction = pAction;
        CC_SAFE_RETAIN(m_pInnerAction);
    }
}

NS_CC_END



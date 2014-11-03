

#include "CCActionInstant.h"
#include "view/CAView.h"
#include "view/CAImageView.h"

NS_CC_BEGIN
//
// InstantAction
//
CCActionInstant::CCActionInstant() {
}

CAObject * CCActionInstant::copyWithZone(CAZone *pZone) {
    CAZone *pNewZone = NULL;
    CCActionInstant *pRet = NULL;

    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCActionInstant*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCActionInstant();
        pZone = pNewZone = new CAZone(pRet);
    }

    CCFiniteTimeAction::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

bool CCActionInstant::isDone() {
    return true;
}

void CCActionInstant::step(float dt) {
    CC_UNUSED_PARAM(dt);
    update(1);
}

void CCActionInstant::update(float time) {
    CC_UNUSED_PARAM(time);
    // nothing
}

CCFiniteTimeAction * CCActionInstant::reverse() {
    return (CCFiniteTimeAction*) (copy()->autorelease());
}

//
// Show
//

CCShow* CCShow::create() 
{
    CCShow* pRet = new CCShow();

    if (pRet) {
        pRet->autorelease();
    }

    return pRet;
}

void CCShow::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(true);
}

CCFiniteTimeAction* CCShow::reverse() {
    return (CCFiniteTimeAction*) (CCHide::create());
}

CAObject* CCShow::copyWithZone(CAZone *pZone) {

    CAZone *pNewZone = NULL;
    CCShow *pRet = NULL;
    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCShow*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCShow();
        pZone = pNewZone = new CAZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

//
// Hide
//
CCHide * CCHide::create() 
{
    CCHide *pRet = new CCHide();

    if (pRet) {
        pRet->autorelease();
    }

    return pRet;
}

void CCHide::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(false);
}

CCFiniteTimeAction *CCHide::reverse() {
    return (CCFiniteTimeAction*) (CCShow::create());
}

CAObject* CCHide::copyWithZone(CAZone *pZone) {
    CAZone *pNewZone = NULL;
    CCHide *pRet = NULL;

    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCHide*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCHide();
        pZone = pNewZone = new CAZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

//
// ToggleVisibility
//
CCToggleVisibility * CCToggleVisibility::create()
{
    CCToggleVisibility *pRet = new CCToggleVisibility();

    if (pRet)
    {
        pRet->autorelease();
    }

    return pRet;
}

void CCToggleVisibility::update(float time) 
{
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(!m_pTarget->isVisible());
}

CAObject* CCToggleVisibility::copyWithZone(CAZone *pZone)
{
    CAZone *pNewZone = NULL;
    CCToggleVisibility *pRet = NULL;

    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCToggleVisibility*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCToggleVisibility();
        pZone = pNewZone = new CAZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

//
// Remove Self
//
CCRemoveSelf * CCRemoveSelf::create(bool isNeedCleanUp /*= true*/) 
{
	CCRemoveSelf *pRet = new CCRemoveSelf();

	if (pRet && pRet->init(isNeedCleanUp)) {
		pRet->autorelease();
	}

	return pRet;
}

bool CCRemoveSelf::init(bool isNeedCleanUp) {
	m_bIsNeedCleanUp = isNeedCleanUp;
	return true;
}

void CCRemoveSelf::update(float time) {
	CC_UNUSED_PARAM(time);
	m_pTarget->removeFromSuperview();
}

CCFiniteTimeAction *CCRemoveSelf::reverse() {
	return (CCFiniteTimeAction*) (CCRemoveSelf::create(m_bIsNeedCleanUp));
}

CAObject* CCRemoveSelf::copyWithZone(CAZone *pZone) {
	CAZone *pNewZone = NULL;
	CCRemoveSelf *pRet = NULL;

	if (pZone && pZone->m_pCopyObject) {
		pRet = (CCRemoveSelf*) (pZone->m_pCopyObject);
	} else {
		pRet = new CCRemoveSelf();
		pZone = pNewZone = new CAZone(pRet);
	}

	CCActionInstant::copyWithZone(pZone);
	pRet->init(m_bIsNeedCleanUp);
	CC_SAFE_DELETE(pNewZone);
	return pRet;
}

//
// FlipX
//

CCFlipX *CCFlipX::create(bool x)
{
    CCFlipX *pRet = new CCFlipX();

    if (pRet && pRet->initWithFlipX(x)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCFlipX::initWithFlipX(bool x) {
    m_bFlipX = x;
    return true;
}

void CCFlipX::update(float time) {
    CC_UNUSED_PARAM(time);
    ((CAImageView*) (m_pTarget))->setFlipX(m_bFlipX);
}

CCFiniteTimeAction* CCFlipX::reverse() {
    return CCFlipX::create(!m_bFlipX);
}

CAObject * CCFlipX::copyWithZone(CAZone *pZone) {
    CAZone *pNewZone = NULL;
    CCFlipX *pRet = NULL;

    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCFlipX*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCFlipX();
        pZone = pNewZone = new CAZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    pRet->initWithFlipX(m_bFlipX);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

//
// FlipY
//

CCFlipY * CCFlipY::create(bool y)
{
    CCFlipY *pRet = new CCFlipY();

    if (pRet && pRet->initWithFlipY(y)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCFlipY::initWithFlipY(bool y) {
    m_bFlipY = y;
    return true;
}

void CCFlipY::update(float time) {
    CC_UNUSED_PARAM(time);
    ((CAImageView*) (m_pTarget))->setFlipY(m_bFlipY);
}

CCFiniteTimeAction* CCFlipY::reverse() {
    return CCFlipY::create(!m_bFlipY);
}

CAObject* CCFlipY::copyWithZone(CAZone *pZone) {
    CAZone *pNewZone = NULL;
    CCFlipY *pRet = NULL;

    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCFlipY*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCFlipY();
        pZone = pNewZone = new CAZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    pRet->initWithFlipY(m_bFlipY);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

//
// Place
//

CCPlace* CCPlace::create(const CCPoint& pos)
{
    CCPlace *pRet = new CCPlace();

    if (pRet && pRet->initWithPosition(pos)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCPlace::initWithPosition(const CCPoint& pos) {
    m_tPosition = pos;
    return true;
}

CAObject * CCPlace::copyWithZone(CAZone *pZone) {
    CAZone *pNewZone = NULL;
    CCPlace *pRet = NULL;

    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCPlace*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCPlace();
        pZone = pNewZone = new CAZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    pRet->initWithPosition(m_tPosition);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCPlace::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->setFrameOrigin(m_tPosition);
}

//
// CallFunc
//
CCCallFunc * CCCallFunc::create(CAObject* pSelectorTarget, SEL_CallFunc selector) 
{
    CCCallFunc *pRet = new CCCallFunc();

    if (pRet && pRet->initWithTarget(pSelectorTarget)) {
        pRet->m_pCallFunc = selector;
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCCallFunc::initWithTarget(CAObject* pSelectorTarget) {
    if (pSelectorTarget) 
    {
        pSelectorTarget->retain();
    }

    if (m_pSelectorTarget) 
    {
        m_pSelectorTarget->release();
    }

    m_pSelectorTarget = pSelectorTarget;
    return true;
}

CCCallFunc::~CCCallFunc(void)
{
    CC_SAFE_RELEASE(m_pSelectorTarget);
}

CAObject * CCCallFunc::copyWithZone(CAZone *pZone) {
    CAZone* pNewZone = NULL;
    CCCallFunc* pRet = NULL;

    if (pZone && pZone->m_pCopyObject) {
        //in case of being called at sub class
        pRet = (CCCallFunc*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCCallFunc();
        pZone = pNewZone = new CAZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    pRet->initWithTarget(m_pSelectorTarget);
    pRet->m_pCallFunc = m_pCallFunc;

    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCCallFunc::update(float time)
{
    CC_UNUSED_PARAM(time);
    this->execute();
}

void CCCallFunc::execute()
{
    if (m_pCallFunc)
    {
        (m_pSelectorTarget->*m_pCallFunc)();
    }
}

//
// CallFuncN
//
void CCCallFuncN::execute()
{
    if (m_pCallFuncN)
    {
        (m_pSelectorTarget->*m_pCallFuncN)(m_pTarget);
    }
}

CCCallFuncN * CCCallFuncN::create(CAObject* pSelectorTarget, SEL_CallFuncN selector)
{
    CCCallFuncN *pRet = new CCCallFuncN();

    if (pRet && pRet->initWithTarget(pSelectorTarget, selector))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCCallFuncN::initWithTarget(CAObject* pSelectorTarget,
        SEL_CallFuncN selector) {
    if (CCCallFunc::initWithTarget(pSelectorTarget)) {
        m_pCallFuncN = selector;
        return true;
    }

    return false;
}

CAObject * CCCallFuncN::copyWithZone(CAZone* zone) {
    CAZone* pNewZone = NULL;
    CCCallFuncN* pRet = NULL;

    if (zone && zone->m_pCopyObject) {
        //in case of being called at sub class
        pRet = (CCCallFuncN*) (zone->m_pCopyObject);
    } else {
        pRet = new CCCallFuncN();
        zone = pNewZone = new CAZone(pRet);
    }

    CCCallFunc::copyWithZone(zone);
    pRet->initWithTarget(m_pSelectorTarget, m_pCallFuncN);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

//
// CallFuncND
//

CCCallFuncND * CCCallFuncND::create(CAObject* pSelectorTarget, SEL_CallFuncND selector, void* d)
{
    CCCallFuncND* pRet = new CCCallFuncND();

    if (pRet && pRet->initWithTarget(pSelectorTarget, selector, d)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCCallFuncND::initWithTarget(CAObject* pSelectorTarget,
        SEL_CallFuncND selector, void* d) {
    if (CCCallFunc::initWithTarget(pSelectorTarget)) {
        m_pData = d;
        m_pCallFuncND = selector;
        return true;
    }

    return false;
}

CAObject * CCCallFuncND::copyWithZone(CAZone* zone) {
    CAZone* pNewZone = NULL;
    CCCallFuncND* pRet = NULL;

    if (zone && zone->m_pCopyObject) {
        //in case of being called at sub class
        pRet = (CCCallFuncND*) (zone->m_pCopyObject);
    } else {
        pRet = new CCCallFuncND();
        zone = pNewZone = new CAZone(pRet);
    }

    CCCallFunc::copyWithZone(zone);
    pRet->initWithTarget(m_pSelectorTarget, m_pCallFuncND, m_pData);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCCallFuncND::execute() {
    if (m_pCallFuncND) {
        (m_pSelectorTarget->*m_pCallFuncND)(m_pTarget, m_pData);
    }
}

//
// CCCallFuncO
//
CCCallFuncO::CCCallFuncO() :
        m_pObject(NULL) {
}

CCCallFuncO::~CCCallFuncO() {
    CC_SAFE_RELEASE(m_pObject);
}

void CCCallFuncO::execute() {
    if (m_pCallFuncO) {
        (m_pSelectorTarget->*m_pCallFuncO)(m_pObject);
    }
}

CCCallFuncO * CCCallFuncO::create(CAObject* pSelectorTarget, SEL_CallFuncO selector, CAObject* pObject)
{
    CCCallFuncO *pRet = new CCCallFuncO();

    if (pRet && pRet->initWithTarget(pSelectorTarget, selector, pObject)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCCallFuncO::initWithTarget(CAObject* pSelectorTarget,
        SEL_CallFuncO selector, CAObject* pObject) {
    if (CCCallFunc::initWithTarget(pSelectorTarget)) {
        m_pObject = pObject;
        CC_SAFE_RETAIN(m_pObject);

        m_pCallFuncO = selector;
        return true;
    }

    return false;
}

CAObject * CCCallFuncO::copyWithZone(CAZone* zone) {
    CAZone* pNewZone = NULL;
    CCCallFuncO* pRet = NULL;

    if (zone && zone->m_pCopyObject) {
        //in case of being called at sub class
        pRet = (CCCallFuncO*) (zone->m_pCopyObject);
    } else {
        pRet = new CCCallFuncO();
        zone = pNewZone = new CAZone(pRet);
    }

    CCCallFunc::copyWithZone(zone);
    pRet->initWithTarget(m_pSelectorTarget, m_pCallFuncO, m_pObject);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

NS_CC_END

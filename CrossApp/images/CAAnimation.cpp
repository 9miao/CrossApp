
#include "CAAnimation.h"
#include "CAImageCache.h"
#include "CAImage.h"
#include "ccMacros.h"
#include "CAImageFrame.h"

NS_CC_BEGIN

CAAnimationFrame::CAAnimationFrame()
: m_pSpriteFrame(NULL)
, m_fDelayUnits(0.0f)
, m_pUserInfo(NULL)
{

}

bool CAAnimationFrame::initWithSpriteFrame(CAImageFrame* spriteFrame, float delayUnits, CCDictionary* userInfo)
{
    setSpriteFrame(spriteFrame);
    setDelayUnits(delayUnits);
    setUserInfo(userInfo);

    return true;
}

CAAnimationFrame::~CAAnimationFrame()
{    
    CCLOGINFO( "CrossApp: deallocing %s", this);

    CC_SAFE_RELEASE(m_pSpriteFrame);
    CC_SAFE_RELEASE(m_pUserInfo);
}

CAObject* CAAnimationFrame::copyWithZone(CAZone* pZone)
{
    CAZone* pNewZone = NULL;
    CAAnimationFrame* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CAAnimationFrame*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CAAnimationFrame();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithSpriteFrame((CAImageFrame*)m_pSpriteFrame->copy()->autorelease(),
        m_fDelayUnits, m_pUserInfo != NULL ? (CCDictionary*)m_pUserInfo->copy()->autorelease() : NULL);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

// implementation of CAAnimation

CAAnimation* CAAnimation::create(void)
{
    CAAnimation *pAnimation = new CAAnimation();
    pAnimation->init();
    pAnimation->autorelease();

    return pAnimation;
} 

CAAnimation* CAAnimation::createWithSpriteFrames(CCArray *frames, float delay/* = 0.0f*/)
{
    CAAnimation *pAnimation = new CAAnimation();
    pAnimation->initWithSpriteFrames(frames, delay);
    pAnimation->autorelease();

    return pAnimation;
}

CAAnimation* CAAnimation::create(CCArray* arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops)
{
    CAAnimation *pAnimation = new CAAnimation();
    pAnimation->initWithAnimationFrames(arrayOfAnimationFrameNames, delayPerUnit, loops);
    pAnimation->autorelease();
    return pAnimation;
}

bool CAAnimation::init()
{
    return initWithSpriteFrames(NULL, 0.0f);
}

bool CAAnimation::initWithSpriteFrames(CCArray *pFrames, float delay/* = 0.0f*/)
{
    CCARRAY_VERIFY_TYPE(pFrames, CAImageFrame*);

    m_uLoops = 1;
    m_fDelayPerUnit = delay;
    CCArray* pTmpFrames = CCArray::create();
    setFrames(pTmpFrames);

    if (pFrames != NULL)
    {
        CAObject* pObj = NULL;
        CCARRAY_FOREACH(pFrames, pObj)
        {
            CAImageFrame* frame = (CAImageFrame*)pObj;
            CAAnimationFrame *animFrame = new CAAnimationFrame();
            animFrame->initWithSpriteFrame(frame, 1, NULL);
            m_pFrames->addObject(animFrame);
            animFrame->release();

            m_fTotalDelayUnits++;
        }
    }

    return true;
}

bool CAAnimation::initWithAnimationFrames(CCArray* arrayOfAnimationFrames, float delayPerUnit, unsigned int loops)
{
    CCARRAY_VERIFY_TYPE(arrayOfAnimationFrames, CAAnimationFrame*);

    m_fDelayPerUnit = delayPerUnit;
    m_uLoops = loops;

    setFrames(CCArray::createWithArray(arrayOfAnimationFrames));

    CAObject* pObj = NULL;
    CCARRAY_FOREACH(m_pFrames, pObj)
    {
        CAAnimationFrame* animFrame = (CAAnimationFrame*)pObj;
        m_fTotalDelayUnits += animFrame->getDelayUnits();
    }
    return true;
}

CAAnimation::CAAnimation()
: m_fTotalDelayUnits(0.0f)
, m_fDelayPerUnit(0.0f)
, m_fDuration(0.0f)
, m_pFrames(NULL)
, m_bRestoreOriginalFrame(false)
, m_uLoops(0)
{

}

CAAnimation::~CAAnimation(void)
{
    CCLOGINFO("CrossApp, deallocing %p", this);
    CC_SAFE_RELEASE(m_pFrames);
}

void CAAnimation::addSpriteFrame(CAImageFrame *pFrame)
{
    CAAnimationFrame *animFrame = new CAAnimationFrame();
    animFrame->initWithSpriteFrame(pFrame, 1.0f, NULL);
    m_pFrames->addObject(animFrame);
    animFrame->release();

    // update duration
    m_fTotalDelayUnits++;
}

void CAAnimation::addSpriteFrameWithFileName(const char *pszFileName)
{
    CAImage* pTexture = CAImageCache::sharedImageCache()->addImage(pszFileName);
    CCRect rect = CCRectZero;
    rect.size = pTexture->getContentSize();
    CAImageFrame *pFrame = CAImageFrame::createWithImage(pTexture, rect);
    addSpriteFrame(pFrame);
}

void CAAnimation::addSpriteFrameWithImage(CAImage* pobImage, const CCRect& rect)
{
    CAImageFrame *pFrame = CAImageFrame::createWithImage(pobImage, rect);
    addSpriteFrame(pFrame);
}

float CAAnimation::getDuration(void)
{
    return m_fTotalDelayUnits * m_fDelayPerUnit;
}

CAObject* CAAnimation::copyWithZone(CAZone* pZone)
{
    CAZone* pNewZone = NULL;
    CAAnimation* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CAAnimation*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CAAnimation();
        pNewZone = new CAZone(pCopy);
    }

    pCopy->initWithAnimationFrames(m_pFrames, m_fDelayPerUnit, m_uLoops);
    pCopy->setRestoreOriginalFrame(m_bRestoreOriginalFrame);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

NS_CC_END
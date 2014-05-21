//
//  CAScrollView.cpp
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-4-23.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#include "CAScrollView.h"
#include "sprite_nodes/CCScale9Sprite.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "CCDirector.h"
#include "CCScheduler.h"
#include "touch_dispatcher/CCTouch.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

#pragma CAScrollView

CAScrollView::CAScrollView()
:m_pContainer(NULL)
,m_obViewSize(CCSizeZero)
,m_pScrollViewDelegate(NULL)
,m_bBounces(true)
,m_bBounceHorizontal(true)
,m_bBounceVertical(true)
,m_bscrollEnabled(true)
,m_bTracking(false)
,m_bDecelerating(false)
,m_bBouncesZoom(true)
,m_bZooming(false)
,m_fMaximumZoomScale(1.0f)
,m_fMinimumZoomScale(1.0f)
,m_fZoomScale(1.0f)
,m_fTouchLength(0.0f)
,m_tInertia(CCPointZero)
,m_bShowsHorizontalScrollIndicator(true)
,m_bShowsVerticalScrollIndicator(true)
{
    m_pTouches = new CCArray(2);
    m_pChildInThis = new CCArray();
}

CAScrollView::~CAScrollView()
{
    CC_SAFE_DELETE(m_pTouches);
    CC_SAFE_DELETE(m_pChildInThis);
}

void CAScrollView::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CAScrollView::update), this, 1/60.0f, false);
}

void CAScrollView::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
    
    m_tPointOffset.clear();
    m_pTouches->removeAllObjects();
    m_tInertia = CCPointZero;
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
}

bool CAScrollView::initWithFrame(const cocos2d::CCRect &rect)
{
    if (!CAView::initWithFrame(rect, ccc4(255, 255, 255, 0)))
    {
        return false;
    }
    
    this->setTouchEnabled(true);
    this->setDisplayRange(false);
    
    m_pContainer = CAView::createWithFrame(this->getBounds(), ccc4(255, 255, 255, 0));
    m_pChildInThis->addObject(m_pContainer);
    this->addSubview(m_pContainer);
    
    m_pIndicatorHorizontal = CAIndicator::createWithFrame(CCRect(12, rect.size.height - 12, rect.size.width - 24, 10), CAIndicator::CAIndicatorTypeHorizontal);
    m_pChildInThis->addObject(m_pIndicatorHorizontal);
    this->insertSubview(m_pIndicatorHorizontal, 1);
    
    m_pIndicatorVertical = CAIndicator::createWithFrame(CCRect(rect.size.width - 12, 12, 10, rect.size.height - 24), CAIndicator::CAIndicatorTypeVertical);
    m_pChildInThis->addObject(m_pIndicatorVertical);
    this->insertSubview(m_pIndicatorVertical, 1);
    
    
    return true;
}

void CAScrollView::addSubview(CAView* subview)
{
    do
    {
        CC_BREAK_IF(m_pChildInThis->containsObject(subview));
        m_pContainer->addSubview(subview);
        return;
    }
    while (0);
    
    CAView::addSubview(subview);
}

void CAScrollView::insertSubview(CAView* subview, int z)
{
    do
    {
        CC_BREAK_IF(m_pChildInThis->containsObject(subview));
        m_pContainer->insertSubview(subview, z);
        return;
    }
    while (0);
    
    CAView::insertSubview(subview, z);
}

void CAScrollView::removeAllSubviews()
{
    m_pContainer->removeAllSubviews();
}

void CAScrollView::setViewSize(cocos2d::CCSize var)
{
    do
    {
        CC_BREAK_IF(m_obViewSize.equals(var));
        
        m_obViewSize = var;
        
        CC_BREAK_IF(m_pContainer == NULL);

        CCRect rect = CCRectZero;
        rect.size = var;
        m_pContainer->setFrame(rect);
        
    }
    while (0);
    
}

CCSize CAScrollView::getViewSize()
{
    return m_obViewSize;
}

void CAScrollView::setBounces(bool var)
{
    m_bBounces = var;
    if (var == false)
    {
        m_bBounceHorizontal = false;
        m_bBounceVertical = false;
    }
}

bool CAScrollView::isBounces()
{
    return m_bBounces;
}

void CAScrollView::setShowsHorizontalScrollIndicator(bool var)
{
    m_bShowsHorizontalScrollIndicator = var;
    if (m_bShowsHorizontalScrollIndicator)
    {
        m_pIndicatorHorizontal->setVisible(true);
    }
    else
    {
        m_pIndicatorHorizontal->setVisible(false);
    }
}

bool CAScrollView::isShowsHorizontalScrollIndicator()
{
    return m_bShowsHorizontalScrollIndicator;
}

void CAScrollView::setShowsVerticalScrollIndicator(bool var)
{
    m_bShowsVerticalScrollIndicator = var;
    if (m_bShowsVerticalScrollIndicator)
    {
        m_pIndicatorVertical->setVisible(true);
    }
    else
    {
        m_pIndicatorVertical->setVisible(false);
    }
}

bool CAScrollView::isShowsVerticalScrollIndicator()
{
    return m_bShowsVerticalScrollIndicator;
}

bool CAScrollView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    do
    {
        
        CCPoint point = this->convertTouchToNodeSpace(pTouch);
        CC_BREAK_IF(m_bscrollEnabled == false);
        CC_BREAK_IF(!this->isVisible());
        CC_BREAK_IF(!getBounds().containsPoint(point));
        CC_BREAK_IF(m_pTouches->count() > 2);

        if (!m_pTouches->containsObject(pTouch))
        {
            m_pTouches->addObject(pTouch);
        }

        if (m_pTouches->count() == 1)
        {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CAScrollView::deaccelerateScrolling), this);
            m_tInertia = CCPointZero;
            m_pContainer->setAnchorPoint(CCPoint(0.5f, 0.5f));
        }
        else if (m_pTouches->count() == 2)
        {
            CCTouch* touch0 = dynamic_cast<CCTouch*>(m_pTouches->objectAtIndex(0));
            CCTouch* touch1 = dynamic_cast<CCTouch*>(m_pTouches->objectAtIndex(1));
           
            m_fTouchLength = ccpDistance(touch0->getLocation(), touch1->getLocation());
            
            CCPoint mid_point = ccpMidpoint(touch0->getLocation(), touch1->getLocation());
            
            CCPoint p = m_pContainer->convertToNodeSpace(mid_point);
            m_pContainer->setAnchorPointInPoints(p);
            
            if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewDidZoom(this);
            }
            m_bZooming = true;
        }
        
        return true;
    }
    while (0);

    return false;
}

void CAScrollView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint p_container = m_pContainer->getCenter().origin;
    CCPoint p_off = CCPointZero;
    
    if (m_pTouches->count() == 1)
    {
        p_off = ccpSub(pTouch->getLocation(), pTouch->getPreviousLocation());
    }
    else if (m_pTouches->count() == 2)
    {
        CCTouch* touch0 = dynamic_cast<CCTouch*>(m_pTouches->objectAtIndex(0));
        CCTouch* touch1 = dynamic_cast<CCTouch*>(m_pTouches->objectAtIndex(1));
        
        if (m_fMinimumZoomScale < m_fMaximumZoomScale)
        {
            float touch_lenght = ccpDistance(touch0->getLocation(), touch1->getLocation());
            float scale_off = touch_lenght - m_fTouchLength;
            
            m_fZoomScale = m_pContainer->getScale();
            m_fZoomScale += m_fZoomScale * scale_off * 0.0015f;
            
            if (!m_bBouncesZoom)
            {
                m_fZoomScale = MIN(m_fZoomScale, m_fMaximumZoomScale);
                m_fZoomScale = MAX(m_fZoomScale, m_fMinimumZoomScale);
            }
            else
            {
                m_fZoomScale = MIN(m_fZoomScale, m_fMaximumZoomScale * 2);
                m_fZoomScale = MAX(m_fZoomScale, m_fMinimumZoomScale / 2);
            }
            
            m_pContainer->setScale(m_fZoomScale);
            m_fTouchLength = touch_lenght;
        }
        
		CCPoint mid_point = ccpMidpoint(touch0->getLocation(), touch1->getLocation());

        p_off = ccpSub(this->convertToNodeSpace(mid_point), m_pContainer->getPosition());
    }
    
    
    if (m_bBounces)
    {
        CCSize size = this->getContentSize();
        CCRect rect = m_pContainer->getFrame();
        rect.size.width = MAX(rect.size.width, size.width);
        rect.size.height = MAX(rect.size.height, size.height);
        CCPoint scale = CCPoint(1.0f, 1.0f);
        
        if (p_container.x - rect.size.width / 2 > 0)
        {
            scale.x = MAX(0, 0.5f - rect.origin.x / size.width);
            p_off.x *= scale.x;
        }
        
        if (p_container.y - rect.size.height / 2 > 0)
        {
            scale.y = MAX(0, 0.5f - rect.origin.y / size.height);
            p_off.y *= scale.y;
        }
        
        if ((p_container.x + rect.size.width / 2 - size.width) < 0)
        {
            scale.x = MAX(0, (rect.size.width + rect.origin.x) / size.width - 0.5f);
            p_off.x *= scale.x;
        }
        
        if ((p_container.y + rect.size.height / 2 - size.height) < 0)
        {
            scale.y = MAX(0, (rect.size.height + rect.origin.y) / size.height - 0.5f);
            p_off.y *= scale.y;
        }
    }
    
    p_container = ccpAdd(p_container, p_off);
    
    m_tPointOffset.push_back(p_off);

    if (m_tPointOffset.size() > 3)
    {
        m_tPointOffset.pop_front();
    }
    
    if (m_pScrollViewDelegate)
    {
        m_pScrollViewDelegate->scrollViewWillBeginDragging(this);
    }
    m_bTracking = true;
    
    if (m_bBounces == false)
    {
        p_container = this->getScrollWindowNotOutPoint(p_container);
    }
    else
    {
        if (m_bBounceHorizontal == false)
        {
            p_container.x = this->getScrollWindowNotOutHorizontal(p_container.x);
        }
        
        if (m_bBounceVertical == false)
        {
            p_container.y = this->getScrollWindowNotOutVertical(p_container.y);
        }
    }
    
    if (m_pScrollViewDelegate && p_container.equals(m_pContainer->getCenter().origin) == false)
    {
        m_pScrollViewDelegate->scrollViewDidScroll(this);
    }
    
    m_pContainer->setCenter(CCRect(p_container.x, p_container.y, 0, 0));
}

void CAScrollView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pTouches->containsObject(pTouch))
    {
        if (m_pTouches->count() == 1)
        {
            CCPoint p = CCPointZero;
            
            if (m_tPointOffset.size() > 0)
            {
//                if (m_tPointOffset.size() > 1)
//                {
//                    m_tPointOffset.pop_back();
//                }
                
                for (unsigned int i=0; i<m_tPointOffset.size(); i++)
                {
                    p = ccpAdd(p, m_tPointOffset.at(i));
                }
                p = p/m_tPointOffset.size();
            }
            m_tInertia = p;
			m_tPointOffset.clear();

            if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewDidEndDragging(this);
            }
            m_bTracking = false;
            
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CAScrollView::deaccelerateScrolling), this, 1/60.0f, false);
        }
        else if (m_pTouches->count() == 2)
        {
            m_bZooming = false;
            
            for (unsigned int i=0; i<2; i++)
            {
                CCTouch* touch = dynamic_cast<CCTouch*>(m_pTouches->objectAtIndex(i));
                if (!touch->isEqual(pTouch))
                {
                    CCPoint p = m_pContainer->convertTouchToNodeSpace(touch);
                    m_pContainer->setAnchorPointInPoints(p);
                    break;
                }
            }
            
            this->backtorangeZooming();
        }
        m_pTouches->removeObject(pTouch);
    }
}

void CAScrollView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

void CAScrollView::deaccelerateScrolling(float delay)
{
    if (m_tInertia.getLength() > maxSpeedCache(delay))
    {
        m_tInertia = ccpMult(m_tInertia, maxSpeedCache(delay) / m_tInertia.getLength());
    }
    
    CCPoint speed;
    if (m_tInertia.getLength() > maxSpeed(delay))
    {
        speed = ccpMult(m_tInertia, maxSpeed(delay) / m_tInertia.getLength());
    }
    else
    {
        speed = m_tInertia;
    }
    
    CCPoint point = m_pContainer->getCenter().origin;

    if (m_bBounces)
    {
        CCSize size = this->getContentSize();
        CCSize cSize = m_pContainer->getFrame().size;
        
        CCPoint resilience = CCPointZero;
        
        if (point.x - cSize.width / 2 > 0)
        {
            resilience.x = (point.x - cSize.width / 2) / size.width;
        }
        
        if (point.y - cSize.height / 2 > 0)
        {
            resilience.y = (point.y - cSize.height / 2) / size.height;
        }
        
        if ((point.x + cSize.width / 2 - size.width) < 0)
        {
            resilience.x = (point.x + cSize.width / 2 - size.width) / size.width;
        }
        
        if ((point.y + cSize.height / 2 - size.height) < 0)
        {
            resilience.y = (point.y + cSize.height / 2 - size.height) / size.height;
        }
        
        resilience = ccpMult(resilience, maxBouncesSpeed(delay));

        if (speed.getLength() < resilience.getLength())
        {
            speed = ccpSub(speed, resilience);
            m_tInertia = CCPointZero;
        }
    }
    
    point = ccpAdd(point, speed);
    
    if (this->isScrollWindowNotMaxOutSide())
    {
        m_tInertia = CCPointZero;
    }
    
    if (m_bBounces == false)
    {
        point = this->getScrollWindowNotOutPoint(point);
    }
    else
    {
        if (m_bBounceHorizontal == false)
        {
            point.x = this->getScrollWindowNotOutHorizontal(point.x);
        }
        
        if (m_bBounceVertical == false)
        {
            point.y = this->getScrollWindowNotOutVertical(point.y);
        }
    }
    
    if (m_pScrollViewDelegate && point.equals(m_pContainer->getCenter().origin) == false)
    {
        m_pScrollViewDelegate->scrollViewDidScroll(this);
    }
    
    m_pContainer->setCenterOrigin(point);
    
    m_tInertia = ccpMult(m_tInertia, 1 - decelerationRatio(delay));
    
    m_bDecelerating = true;
    
    if (speed.getLength() < 0.001f)
    {
        m_bDecelerating = false;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CAScrollView::deaccelerateScrolling), this);
    }
}

#define TAG_SCALETO_CONTAINER 0xffffffff

void CAScrollView::backtorangeZooming()
{
    do
    {
        CC_BREAK_IF(!m_bBouncesZoom);

        m_fZoomScale = MIN(m_fZoomScale, m_fMaximumZoomScale);
        m_fZoomScale = MAX(m_fZoomScale, m_fMinimumZoomScale);
        
        m_pContainer->stopActionByTag(TAG_SCALETO_CONTAINER);
        
        CCScaleTo* scaleTo = CCScaleTo::create(0.3f, m_fZoomScale);
        m_pContainer->runAction(scaleTo);
        scaleTo->setTag(TAG_SCALETO_CONTAINER);
        
    }
    while (0);
}

float CAScrollView::maxSpeed(float delay)
{
    return (CCPoint(m_obContentSize).getLength() * 5 * delay);
}

float CAScrollView::maxSpeedCache(float delay)
{
    return (maxSpeed(delay) * 1.5f);
}

float CAScrollView::decelerationRatio(float delay)
{
    return 10 * delay;
}

float CAScrollView::maxBouncesSpeed(float delay)
{
    return (CCPoint(m_obContentSize).getLength() * 6 * delay);
}

CCPoint CAScrollView::maxBouncesLenght()
{
    return ccpMult(m_obContentSize, 0.5f);
}

void CAScrollView::update(float fDelta)
{
    m_pIndicatorHorizontal->setIndicator(m_obContentSize, m_pContainer->getFrame());
    m_pIndicatorVertical->setIndicator(m_obContentSize, m_pContainer->getFrame());
}

const CCPoint& CAScrollView::getScrollWindowNotOutPoint(CCPoint& point)
{
    point.x = this->getScrollWindowNotOutHorizontal(point.x);
    point.y = this->getScrollWindowNotOutVertical(point.y);
    
    return point;
}

float CAScrollView::getScrollWindowNotOutHorizontal(float x)
{
    CCSize size = this->getContentSize();
    CCSize cSize = m_pContainer->getFrame().size;
    cSize.width = MAX(cSize.width, size.width);
    cSize.height = MAX(cSize.height, size.height);
    
    if (x - cSize.width / 2 > 0)
    {
        x = cSize.width / 2;
    }
    
    if ((x + cSize.width / 2 - size.width) < 0)
    {
        x = size.width - cSize.width / 2;
    }
    
    return x;
}

float CAScrollView::getScrollWindowNotOutVertical(float y)
{
    CCSize size = this->getContentSize();
    CCSize cSize = m_pContainer->getFrame().size;
    cSize.width = MAX(cSize.width, size.width);
    cSize.height = MAX(cSize.height, size.height);
    
    if (y - cSize.height / 2 > 0)
    {
        y = cSize.height / 2;
    }
    
    if ((y + cSize.height / 2 - size.height) < 0)
    {
        y = size.height - cSize.height / 2;
    }
    
    return y;
}

bool CAScrollView::isScrollWindowNotOutSide()
{
    CCSize size = this->getContentSize();
    CCRect rect = m_pContainer->getFrame();
    CCPoint point = m_pContainer->getCenter().origin;
    
    if (point.x - rect.size.width / 2 > 0.5f)
    {
        return true;
    }
    
    if ((point.x + rect.size.width / 2 - size.width) < -0.5f)
    {
        return true;
    }
    
    if (point.y - rect.size.height / 2 > 0.5f)
    {
        return true;
    }
    
    if ((point.y + rect.size.height / 2 - size.height) < -0.5f)
    {
        return true;
    }
    
    return false;
}


bool CAScrollView::isScrollWindowNotMaxOutSide()
{
    CCSize size = this->getContentSize();
    CCRect rect = m_pContainer->getFrame();
    CCPoint point = m_pContainer->getCenter().origin;
    
    if (point.x - rect.size.width / 2 - maxBouncesLenght().x > 0)
    {
        return true;
    }
    
    if ((point.x + rect.size.width / 2 - size.width + maxBouncesLenght().x) < 0)
    {
        return true;
    }
    
    if (point.y - rect.size.height / 2 - maxBouncesLenght().y > 0)
    {
        return true;
    }
    
    if ((point.y + rect.size.height / 2 - size.height + maxBouncesLenght().y) < 0)
    {
        return true;
    }
    
    return false;
}


#pragma CAIndicator

CAIndicator::CAIndicator()
:m_pIndicator(NULL)
{

}

CAIndicator::~CAIndicator()
{

}

void CAIndicator::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
}

void CAIndicator::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
    this->setOpacity(0);
}

CAIndicator* CAIndicator::createWithFrame(const CCRect& rect, CAIndicatorType type)
{
    CAIndicator* indicator = new CAIndicator();
    if (indicator && indicator->initWithFrame(rect, type))
    {
        indicator->autorelease();
        return indicator;
    }
    CC_SAFE_DELETE(indicator);
    return NULL;
}

bool CAIndicator::initWithFrame(const CCRect& rect, CAIndicatorType type)
{
    if (!CAView::init())
    {
        return false;
    }
    this->setFrame(rect);
    
    m_eType = type;
    
    CAImage* image = CAImage::create("indicator.png");
    
    CCRect r;
    r.origin = ccpSub(ccpMult(image->getContentSize(), 0.5f), CCPoint(0.5f, 0.5f));
    r.size = CCSize(1.0f, 1.0f);
    
    m_pIndicator = CCScale9Sprite::createWithImage(r, image);
    this->addSubview(m_pIndicator);
    
    return true;
}

void CAIndicator::setIndicator(const CCSize& parentSize, const CCRect& childrenFrame)
{
    CCScale9Sprite* indicator = dynamic_cast<CCScale9Sprite*>(m_pIndicator);
    
    int x = (int)indicator->getCenterOrigin().x * 10;
    int y = (int)indicator->getCenterOrigin().y * 10;
    
    
    if (m_eType == CAIndicatorTypeHorizontal)
    {
        float size_scale_x = parentSize.width / childrenFrame.size.width;
        size_scale_x = MIN(size_scale_x, 1.0f);
        
        float lenght_scale_x = parentSize.width / 2 - childrenFrame.origin.x;
        lenght_scale_x /= childrenFrame.size.width;
        
        CCSize size = m_obContentSize;
        size.width *= size_scale_x;
        
        if (lenght_scale_x < size_scale_x / 2)
        {
            size.width *= lenght_scale_x / (size_scale_x / 2);
        }
        if (1 - lenght_scale_x < size_scale_x / 2)
        {
            size.width *= (1 - lenght_scale_x) / (size_scale_x / 2);
        }
        size.width = MAX(size.height, size.width);
        indicator->setPreferredSize(size);
        
        CCPoint point = m_obContentSize;
        point.y *= 0.5f;
        point.x *= lenght_scale_x;
        point.x = MAX(point.x, size.width/2);
        point.x = MIN(point.x, m_obContentSize.width - size.width/2);
        indicator->setCenterOrigin(point);
    }
    else if (m_eType == CAIndicatorTypeVertical)
    {
        float size_scale_y = parentSize.height / childrenFrame.size.height;
        size_scale_y = MIN(size_scale_y, 1.0f);
        
        float lenght_scale_y = parentSize.height / 2 - childrenFrame.origin.y;
        lenght_scale_y /= childrenFrame.size.height;
        
        CCSize size = m_obContentSize;
        size.height *= size_scale_y;
        
        if (lenght_scale_y < size_scale_y / 2)
        {
            size.height *= lenght_scale_y / (size_scale_y / 2);
        }
        if (1 - lenght_scale_y < size_scale_y / 2)
        {
            size.height *= (1 - lenght_scale_y) / (size_scale_y / 2);
        }
        size.height = MAX(size.height, size.width);
		indicator->setPreferredSize(size);
        
        
        CCPoint point = m_obContentSize;
        point.x *= 0.5f;
        point.y *= lenght_scale_y;
        point.y = MAX(point.y, size.height/2);
        point.y = MIN(point.y, m_obContentSize.height - size.height/2);
        
        indicator->setCenterOrigin(point);
    }
    
    do
    {
        if (x == (int)indicator->getCenterOrigin().x * 10 && y == (int)indicator->getCenterOrigin().y * 10)
        {
            CC_BREAK_IF(indicator->getActionByTag(0xfff));
            
            CC_BREAK_IF(indicator->getOpacity() < 255);
            
            CCDelayTime* delayTime = CCDelayTime::create(0.2f);
            CCFadeOut* fadeOut = CCFadeOut::create(0.3f);
            CCEaseSineOut* easeSineOut = CCEaseSineOut::create(fadeOut);
            CCSequence* actions = CCSequence::create(delayTime, easeSineOut, NULL);
            actions->setTag(0xfff);
            indicator->runAction(actions);
        }
        else
        {
            CC_BREAK_IF(indicator->getOpacity() == 255);
            
            indicator->stopAllActions();
            indicator->setOpacity(255);
        }
    }
    while (0);
}

void CAIndicator::setOpacity(GLubyte opacity)
{
    CCScale9Sprite* indicator = dynamic_cast<CCScale9Sprite*>(m_pIndicator);
    indicator->setOpacity(opacity);
    indicator->setColor(ccc3(opacity, opacity, opacity));
}

#pragma CAScrollViewDelegate

void CAScrollViewDelegate::removeDelegate(CAScrollView* view)
{
    view->setScrollViewDelegate(NULL);
}

NS_CC_END
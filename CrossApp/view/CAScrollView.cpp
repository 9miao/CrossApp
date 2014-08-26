//
//  CAScrollView.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAScrollView.h"
#include "CAScale9ImageView.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "dispatcher/CATouch.h"
#include "support/CCPointExtension.h"
#include "kazmath/GL/matrix.h"
#include "CCEGLView.h"

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
,m_bZooming(false)
,m_fMaximumZoomScale(1.0f)
,m_fMinimumZoomScale(1.0f)
,m_fZoomScale(1.0f)
,m_fTouchLength(0.0f)
,m_tInertia(CCPointZero)
,m_tCloseToPoint(CCPointZero)
,m_pIndicatorHorizontal(NULL)
,m_pIndicatorVertical(NULL)
,m_bShowsHorizontalScrollIndicator(true)
,m_bShowsVerticalScrollIndicator(true)
{
    m_bTouchMovedStopSubviews = true;
    
    m_pTouches = new CCArray(2);
    m_pChildInThis = new CCArray();
}

CAScrollView::~CAScrollView()
{
    CC_SAFE_DELETE(m_pTouches);
    CC_SAFE_DELETE(m_pChildInThis);
    m_pScrollViewDelegate = NULL;
}

void CAScrollView::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
}

void CAScrollView::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
    
    m_tPointOffset.clear();
    m_pTouches->removeAllObjects();
    m_tInertia = CCPointZero;
}

CAScrollView* CAScrollView::createWithFrame(const CCRect& rect)
{
    CAScrollView* scrollView = new CAScrollView();
    if (scrollView && scrollView->initWithFrame(rect))
    {
        scrollView->autorelease();
        return scrollView;
    }
    CC_SAFE_DELETE(scrollView);
    return NULL;
}

CAScrollView* CAScrollView::createWithCenter(const CCRect& rect)
{
    CAScrollView* scrollView = new CAScrollView();
    if (scrollView && scrollView->initWithCenter(rect))
    {
        scrollView->autorelease();
        return scrollView;
    }
    CC_SAFE_DELETE(scrollView);
    return NULL;
}

bool CAScrollView::init()
{
    if (!CAView::init())
    {
        return false;
    }
    this->setDisplayRange(false);
    
    m_pContainer = new CAView();
    m_pContainer->setFrame(this->getBounds());
    m_pChildInThis->addObject(m_pContainer);
    this->addSubview(m_pContainer);
    m_pContainer->release();
    
    m_pIndicatorHorizontal = CAIndicator::create(CAIndicator::CAIndicatorTypeHorizontal);
    m_pChildInThis->addObject(m_pIndicatorHorizontal);
    this->insertSubview(m_pIndicatorHorizontal, 1);
    
    m_pIndicatorVertical = CAIndicator::create(CAIndicator::CAIndicatorTypeVertical);
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

void CAScrollView::removeSubview(CAView* subview)
{
    m_pContainer->removeSubview(subview);
}

void CAScrollView::removeSubviewByTag(int tag)
{
    m_pContainer->removeSubviewByTag(tag);
}

CAView* CAScrollView::getSubviewByTag(int aTag)
{
    return m_pContainer->getSubviewByTag(aTag);
}

CCArray* CAScrollView::getSubviews()
{
    return m_pContainer->getSubviews();
}

void CAScrollView::setViewSize(const CCSize& var)
{
    CC_RETURN_IF(m_obViewSize.equals(var));
    
    m_obViewSize = var;
    m_obViewSize.width = MAX(m_obViewSize.width, m_obContentSize.width);
    m_obViewSize.height = MAX(m_obViewSize.height, m_obContentSize.height);
    
    CC_RETURN_IF(m_pContainer == NULL);
    
    CCRect rect = m_pContainer->getFrame();
    rect.size = m_obViewSize;
    m_pContainer->setFrame(rect);
    
    if (!CAScheduler::isScheduled(schedule_selector(CAScrollView::deaccelerateScrolling), this))
    {
        CAScheduler::schedule(schedule_selector(CAScrollView::deaccelerateScrolling), this, 1/60.0f);
    }
}

const CCSize& CAScrollView::getViewSize()
{
    return m_obViewSize;
}

void CAScrollView::setScrollEnabled(bool var)
{
    m_bTouchMovedStopSubviews = m_bscrollEnabled = var;
}

bool CAScrollView::isScrollEnabled()
{
    return m_bscrollEnabled;
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

void CAScrollView::setContentOffset(const CCPoint& offset, bool animated)
{
//    CAScheduler::unschedule(schedule_selector(CAScrollView::deaccelerateScrolling), this);
    
    if (animated)
    {
        m_tCloseToPoint = ccpMult(offset, -1);
        m_tInertia = CCPointZero;
        CAScheduler::schedule(schedule_selector(CAScrollView::closeToPoint), this, 1/60.0f);
    }
    else
    {
        m_pContainer->setFrameOrigin(ccpMult(offset, -1));
    }
}

CCPoint CAScrollView::getContentOffset()
{
    return ccpMult(m_pContainer->getFrameOrigin(), -1);
}

void CAScrollView::setBackGroundImage(CAImage* image)
{
    CAView::setImage(image);
    CCRect rect = CCRectZero;
    rect.size = image->getContentSize();
    CAView::setImageRect(rect);
}

void CAScrollView::setBackGroundColor(const CAColor4B &color)
{
    CAView::setColor(color);
}

void CAScrollView::setContentSize(const CrossApp::CCSize &var)
{
    CAView::setContentSize(var);
    
    CCSize viewSize = this->getViewSize();
    viewSize.width = MAX(m_obContentSize.width, viewSize.width);
    viewSize.height = MAX(m_obContentSize.height, viewSize.height);
    this->setViewSize(viewSize);
    
    CCPoint point = m_pContainer->getCenterOrigin();
    point = this->getScrollWindowNotOutPoint(point);
    m_pContainer->setCenterOrigin(point);
    
    const char indicatorSize = _px(6);
    
    if (m_pIndicatorHorizontal)
    {
        const CCRect indicatorHorizontalFrame = CCRect(indicatorSize * 2,
                                                       var.height - indicatorSize * 2,
                                                       var.width - indicatorSize * 4,
                                                       indicatorSize);
        
        m_pIndicatorHorizontal->setFrame(indicatorHorizontalFrame);
    }
    if (m_pIndicatorVertical)
    {
        const CCRect indicatorVerticalFrame = CCRect(var.width - indicatorSize * 2,
                                                     indicatorSize * 2,
                                                     indicatorSize,
                                                     var.height - indicatorSize * 4);
        
        m_pIndicatorVertical->setFrame(indicatorVerticalFrame);
    }
    this->update(0);
}

void CAScrollView::closeToPoint(float dt)
{
    CCSize size = this->getBounds().size;
    CCPoint point = m_pContainer->getFrameOrigin();
    CCPoint resilience = ccpSub(m_tCloseToPoint, point);
    
    if (resilience.getLength() <= minSpeed(dt))
    {
        m_pContainer->setFrameOrigin(m_tCloseToPoint);
        CAScheduler::unschedule(schedule_selector(CAScrollView::closeToPoint), this);
        m_tCloseToPoint = this->getViewSize();
        this->hideIndicator();
        this->contentOffsetFinish();
    }
    else
    {
        resilience.x /= size.width;
        resilience.y /= size.height;
        resilience = ccpMult(resilience, maxBouncesSpeed(dt));
        if (resilience.getLength() <= minSpeed(dt))
        {
            resilience = ccpMult(resilience, minSpeed(dt) / resilience.getLength());
        }
        resilience = ccpAdd(resilience, point);
        m_pContainer->setFrameOrigin(resilience);
    }
}

bool CAScrollView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    do
    {
        CC_BREAK_IF(m_pTouches->count() > 2);
        
        if (m_bscrollEnabled == false)
            return true;

        if (!m_pTouches->containsObject(pTouch))
        {
            m_pTouches->addObject(pTouch);
        }

        if (m_pTouches->count() == 1)
        {
            CAScheduler::unschedule(schedule_selector(CAScrollView::deaccelerateScrolling), this);
            m_tInertia = CCPointZero;
            CAScheduler::unschedule(schedule_selector(CAScrollView::closeToPoint), this);
            m_tCloseToPoint = this->getViewSize();
            m_pContainer->setAnchorPoint(CCPoint(0.5f, 0.5f));
        }
        else if (m_pTouches->count() == 2)
        {
            CATouch* touch0 = dynamic_cast<CATouch*>(m_pTouches->objectAtIndex(0));
            CATouch* touch1 = dynamic_cast<CATouch*>(m_pTouches->objectAtIndex(1));
           
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

void CAScrollView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_bscrollEnabled == false);
    
    CCPoint p_container = m_pContainer->getCenterOrigin();
    CCPoint p_off = CCPointZero;
    
    if (m_pTouches->count() == 1)
    {
        p_off = ccpSub(pTouch->getLocation(), pTouch->getPreviousLocation());
    }
    else if (m_pTouches->count() == 2)
    {
        CATouch* touch0 = dynamic_cast<CATouch*>(m_pTouches->objectAtIndex(0));
        CATouch* touch1 = dynamic_cast<CATouch*>(m_pTouches->objectAtIndex(1));
        CCPoint mid_point = ccpMidpoint(touch0->getLocation(), touch1->getLocation());
        
        if (m_fMinimumZoomScale < m_fMaximumZoomScale)
        {
            float touch_lenght = ccpDistance(touch0->getLocation(), touch1->getLocation());
            float scale_off = touch_lenght - m_fTouchLength;
            
            m_fZoomScale = m_pContainer->getScale();
            m_fZoomScale += m_fZoomScale * scale_off * 0.0015f;
            
            m_fZoomScale = MIN(m_fZoomScale, m_fMaximumZoomScale);
            m_fZoomScale = MAX(m_fZoomScale, m_fMinimumZoomScale);
            
            m_pContainer->setScale(m_fZoomScale);
            m_fTouchLength = touch_lenght;
        }
        
        p_off = ccpSub(this->convertToNodeSpace(mid_point),
                       ccpAdd(m_pContainer->getFrameOrigin(),
                              m_pContainer->getAnchorPointInPoints() * m_fZoomScale));
    }
    
    
    if (m_bBounces)
    {
        CCSize size = this->getBounds().size;
        CCRect rect = m_pContainer->getCenter();
        rect.size.width = MAX(rect.size.width, size.width);
        rect.size.height = MAX(rect.size.height, size.height);
        CCPoint scale = CCPoint(1.0f, 1.0f);
        
        if (p_container.x - rect.size.width / 2 > 0)
        {
            scale.x = MAX(0, 0.5f - (rect.origin.x - rect.size.width/2) / size.width);
            p_off.x *= scale.x;
        }
        
        if (p_container.y - rect.size.height / 2 > 0)
        {
            scale.y = MAX(0, 0.5f - (rect.origin.y - rect.size.height/2) / size.height);
            p_off.y *= scale.y;
        }
        
        if ((p_container.x + rect.size.width / 2 - size.width) < 0)
        {
            scale.x = MAX(0, (rect.size.width/2 + rect.origin.x) / size.width - 0.5f);
            p_off.x *= scale.x;
        }
        
        if ((p_container.y + rect.size.height / 2 - size.height) < 0)
        {
            scale.y = MAX(0, (rect.size.height/2 + rect.origin.y) / size.height - 0.5f);
            p_off.y *= scale.y;
        }
    }
    
    p_container = ccpAdd(p_container, p_off);
    
    m_tPointOffset.push_back(p_off);

    if (m_tPointOffset.size() > 3)
    {
        m_tPointOffset.pop_front();
    }
    
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
    
    if (p_container.equals(m_pContainer->getCenterOrigin()) == false)
    {
        m_pContainer->setCenterOrigin(p_container);
        this->showIndicator();
        
        if (m_bTracking == false)
        {
            if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewWillBeginDragging(this);
            }
            m_bTracking = true;
        }
        
        if (m_pScrollViewDelegate)
        {
            m_pScrollViewDelegate->scrollViewDidScroll(this);
        }
    }
}

void CAScrollView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    if (m_pTouches->containsObject(pTouch))
    {
        if (m_pTouches->count() == 1)
        {
            CCPoint p = CCPointZero;
            
            if (m_tPointOffset.size() > 0)
            {
                for (unsigned int i=0; i<m_tPointOffset.size(); i++)
                {
                    p = ccpAdd(p, m_tPointOffset.at(i));
                }
                p = p/m_tPointOffset.size();
            }
            m_tInertia = p * 1.5f;
			m_tPointOffset.clear();

            if (!m_tInertia.equals(CCPointZero))
            {
                m_bDecelerating = true;
                if (m_pScrollViewDelegate)
                {
                    m_pScrollViewDelegate->scrollViewDidScroll(this);
                }
                
            }
            
            CAScheduler::schedule(schedule_selector(CAScrollView::deaccelerateScrolling), this, 1/60.0f);
            CAScheduler::unschedule(schedule_selector(CAScrollView::update), this);
            CAScheduler::schedule(schedule_selector(CAScrollView::update), this, 1/60.0f);
            
            if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewDidEndDragging(this);
            }
            m_bTracking = false;
        }
        else if (m_pTouches->count() == 2)
        {
            m_bZooming = false;
        }
        m_pTouches->removeObject(pTouch);
    }
}

void CAScrollView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    if (m_pTouches->containsObject(pTouch))
    {
        if (m_pTouches->count() == 1)
        {
			m_tPointOffset.clear();

            if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewDidEndDragging(this);
            }
            m_bTracking = false;
        }
        else if (m_pTouches->count() == 2)
        {
            m_bZooming = false;
        }
        m_pTouches->removeObject(pTouch);
    }
}

CAResponder* CAScrollView::nextResponder()
{
    return NULL;
}

void CAScrollView::deaccelerateScrolling(float dt)
{
    dt = MIN(dt, 1/30.0f);
    dt = MAX(dt, 1/100.0f);

    if (m_tInertia.getLength() > maxSpeedCache(dt))
    {
        m_tInertia = ccpMult(m_tInertia, maxSpeedCache(dt) / m_tInertia.getLength());
    }
    
    CCPoint speed = CCPointZero;
    if (m_tInertia.getLength() > maxSpeed(dt))
    {
        speed = ccpMult(m_tInertia, maxSpeed(dt) / m_tInertia.getLength());
    }
    else
    {
        speed = m_tInertia;
    }
    
    if (!m_tInertia.equals(CCPointZero))
    {
        m_tInertia = ccpMult(m_tInertia, 1 - decelerationRatio(dt));
    }

    CCPoint point = m_pContainer->getCenterOrigin();

    if (m_bBounces)
    {
        CCSize size = this->getBounds().size;
        CCSize cSize = m_pContainer->getFrame().size;
        cSize.width = MAX(cSize.width, size.width);
        cSize.height = MAX(cSize.height, size.height);
        
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
        
        resilience = ccpMult(resilience, maxBouncesSpeed(dt));

        if (speed.getLength() < resilience.getLength())
        {
            speed = ccpSub(speed, resilience);
            m_tInertia = CCPointZero;
        }
    }
    
    if (speed.getLength() <= minSpeed(dt) / 2)
    {
        point = this->getScrollWindowNotOutPoint(point);
        m_pContainer->setCenterOrigin(point);
        this->hideIndicator();
        m_bDecelerating = false;
        CAScheduler::unschedule(schedule_selector(CAScrollView::deaccelerateScrolling), this);
    }
    else
    {
        if (speed.getLength() <= minSpeed(dt))
        {
            speed = ccpMult(speed, minSpeed(dt) / speed.getLength());
        }
        
        point = ccpAdd(point, speed);
        
        if (this->isScrollWindowNotMaxOutSide(m_pContainer->getCenterOrigin()))
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
        
        
        this->showIndicator();
        m_pContainer->setCenterOrigin(point);
    }
}

void CAScrollView::showIndicator()
{
    if (!CAScheduler::isScheduled(schedule_selector(CAScrollView::update), this))
    {
        CAScheduler::schedule(schedule_selector(CAScrollView::update), this, 1/60.0f);
    }
    m_pIndicatorHorizontal->setHide(false);
    m_pIndicatorVertical->setHide(false);
    this->update(0);
}

void CAScrollView::hideIndicator()
{
    CAScheduler::unschedule(schedule_selector(CAScrollView::update), this);
    m_pIndicatorHorizontal->setHide(true);
    m_pIndicatorVertical->setHide(true);
}

void CAScrollView::update(float dt)
{
    if (m_pIndicatorHorizontal)
    {
        m_pIndicatorHorizontal->setIndicator(m_obContentSize, m_pContainer->getFrame());
    }
    if (m_pIndicatorVertical)
    {
        m_pIndicatorVertical->setIndicator(m_obContentSize, m_pContainer->getFrame());
    }
}

const CCPoint& CAScrollView::getScrollWindowNotOutPoint(CCPoint& point)
{
    point.x = this->getScrollWindowNotOutHorizontal(point.x);
    point.y = this->getScrollWindowNotOutVertical(point.y);
    
    return point;
}

float CAScrollView::getScrollWindowNotOutHorizontal(float x)
{
    CCSize size = this->getBounds().size;
    CCSize cSize = m_pContainer->getFrame().size;
    cSize.width = MAX(cSize.width, size.width);
    cSize.height = MAX(cSize.height, size.height);
    
    x = MIN(x, cSize.width / 2) ;
    x = MAX(x, size.width - cSize.width / 2);
    
    return x;
}

float CAScrollView::getScrollWindowNotOutVertical(float y)
{
    CCSize size = this->getBounds().size;
    CCSize cSize = m_pContainer->getFrame().size;
    cSize.width = MAX(cSize.width, size.width);
    cSize.height = MAX(cSize.height, size.height);
    
    y = MIN(y, cSize.height / 2);
    y = MAX(y, size.height - cSize.height / 2);

    return y;
}

bool CAScrollView::isScrollWindowNotOutSide()
{
    CCSize size = this->getBounds().size;
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


bool CAScrollView::isScrollWindowNotMaxOutSide(const CCPoint& point)
{
    CCSize size = this->getBounds().size;
    CCRect rect = m_pContainer->getFrame();
    
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

CAIndicator::CAIndicator(CAIndicatorType type)
:m_pIndicator(NULL)
,m_eType(type)
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
    this->setAlpha(0.0f);
}

CAIndicator* CAIndicator::create(CAIndicatorType type)
{
    CAIndicator* indicator = new CAIndicator(type);
    if (indicator && indicator->init())
    {
        indicator->autorelease();
        return indicator;
    }
    CC_SAFE_DELETE(indicator);
    return NULL;
}

bool CAIndicator::init()
{
    if (!CAView::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    CAImage* image = CAImage::create("source_material/indicator.png");
    
    m_pIndicator = CAScale9ImageView::createWithImage(image);
    this->addSubview(m_pIndicator);
    this->setAlpha(0.0f);
    
    return true;
}

void CAIndicator::setIndicator(const CCSize& parentSize, const CCRect& childrenFrame)
{
    if ( !this->isVisible())
        return;
    
    CAScale9ImageView* indicator = dynamic_cast<CAScale9ImageView*>(m_pIndicator);
    
    if (m_eType == CAIndicatorTypeHorizontal)
    {
        float size_scale_x = parentSize.width / childrenFrame.size.width;
        size_scale_x = MIN(size_scale_x, 1.0f);
        
        float lenght_scale_x = parentSize.width / 2 - childrenFrame.origin.x;
        lenght_scale_x /= childrenFrame.size.width;
        
        CCRect rect;
        rect.size = m_obContentSize;
        rect.size.width *= size_scale_x;
        
        if (lenght_scale_x < size_scale_x / 2)
        {
            rect.size.width *= lenght_scale_x / (size_scale_x / 2);
        }
        if (1 - lenght_scale_x < size_scale_x / 2)
        {
            rect.size.width *= (1 - lenght_scale_x) / (size_scale_x / 2);
        }
        rect.size.width = MAX(rect.size.height, rect.size.width);
        
        rect.origin = m_obContentSize;
        rect.origin.y *= 0.5f;
        rect.origin.x *= lenght_scale_x;
        rect.origin.x = MAX(rect.origin.x, rect.size.width/2);
        rect.origin.x = MIN(rect.origin.x, m_obContentSize.width - rect.size.width/2);
        
        indicator->setCenter(rect);
    }
    else if (m_eType == CAIndicatorTypeVertical)
    {
        float size_scale_y = parentSize.height / childrenFrame.size.height;
        size_scale_y = MIN(size_scale_y, 1.0f);
        
        float lenght_scale_y = parentSize.height / 2 - childrenFrame.origin.y;
        lenght_scale_y /= childrenFrame.size.height;
        
        CCRect rect;
        rect.size = m_obContentSize;
        rect.size.height *= size_scale_y;
        
        if (lenght_scale_y < size_scale_y / 2)
        {
            rect.size.height *= lenght_scale_y / (size_scale_y / 2);
        }
        if (1 - lenght_scale_y < size_scale_y / 2)
        {
            rect.size.height *= (1 - lenght_scale_y) / (size_scale_y / 2);
        }
        rect.size.height = MAX(rect.size.height, rect.size.width);

        rect.origin = m_obContentSize;
        rect.origin.x *= 0.5f;
        rect.origin.y *= lenght_scale_y;
        rect.origin.y = MAX(rect.origin.y, rect.size.height/2);
        rect.origin.y = MIN(rect.origin.y, m_obContentSize.height - rect.size.height/2);
        
        indicator->setCenter(rect);
    }
}

void CAIndicator::setHide(bool var)
{
    CAScale9ImageView* indicator = dynamic_cast<CAScale9ImageView*>(m_pIndicator);
    
    if (var == false)
    {
        CC_RETURN_IF(fabs(1.0f-this->getAlpha()) < FLT_EPSILON);
        
        this->stopActionByTag(0xfff);
        this->setAlpha(1.0f);
    }
    else
    {
        CC_RETURN_IF(indicator->getActionByTag(0xfff));
        
        CC_RETURN_IF(1.0f-this->getAlpha() > FLT_EPSILON);
        
        CCDelayTime* delayTime = CCDelayTime::create(0.2f);
        CCFadeOut* fadeOut = CCFadeOut::create(0.3f);
        CCEaseSineOut* easeSineOut = CCEaseSineOut::create(fadeOut);
        CCSequence* actions = CCSequence::create(delayTime, easeSineOut, NULL);
        actions->setTag(0xfff);
        this->runAction(actions);
    }
}

NS_CC_END
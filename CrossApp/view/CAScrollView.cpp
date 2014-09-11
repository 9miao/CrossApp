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
,m_tCloseToSpeed(CCPointZero)
,m_pIndicatorHorizontal(NULL)
,m_pIndicatorVertical(NULL)
,m_bShowsHorizontalScrollIndicator(true)
,m_bShowsVerticalScrollIndicator(true)
,m_pHeaderRefreshView(NULL)
,m_pFooterRefreshView(NULL)
{
    m_bTouchMovedStopSubviews = true;
    this->setHaveNextResponder(false);
    m_pTouches = new CCArray(2);
    m_pChildInThis = new CCArray();
}

CAScrollView::~CAScrollView()
{
    CC_SAFE_RELEASE(m_pHeaderRefreshView);
    CC_SAFE_RELEASE(m_pFooterRefreshView);
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
    this->setContainerFrame(CCPointZero, this->getBounds().size);
    m_pChildInThis->addObject(m_pContainer);
    this->addSubview(m_pContainer);
    m_pContainer->release();
    
    this->updateIndicator();
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
    
    CCPoint point = m_pContainer->getFrameOrigin();
    this->setContainerFrame(point, m_obViewSize);
    
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
    if (animated)
    {
        m_tInertia = CCPointZero;
        m_tCloseToPoint = ccpMult(offset, -1);
        m_tCloseToSpeed = ccpMult(ccpSub(m_tCloseToPoint, m_pContainer->getFrameOrigin()), 1/12.0f);
        CAScheduler::schedule(schedule_selector(CAScrollView::closeToPoint), this, 1/60.0f);
    }
    else
    {
        this->setContainerFrame(ccpMult(offset, -1));
        this->update(1/60.0f);
        if (m_pScrollViewDelegate)
        {
            m_pScrollViewDelegate->scrollViewDidMoved(this);
        }
        this->contentOffsetFinish();
    }
}

void CAScrollView::closeToPoint(float dt)
{
    CCPoint point = m_pContainer->getFrameOrigin();

    if (fabsf(m_tCloseToPoint.x - point.x) <= fabsf(m_tCloseToSpeed.x * 60 * dt)
        &&
        fabsf(m_tCloseToPoint.y - point.y) <= fabsf(m_tCloseToSpeed.y * 60 * dt))
    {
        this->setContainerFrame(m_tCloseToPoint);
        CAScheduler::unschedule(schedule_selector(CAScrollView::closeToPoint), this);
        m_tCloseToPoint = this->getViewSize();
        m_tCloseToSpeed = CCPointZero;
        this->update(1/60.0f);
        this->hideIndicator();
        this->contentOffsetFinish();
    }
    else
    {
        point = ccpAdd(point, m_tCloseToSpeed * 60 * dt);
        this->setContainerFrame(point);
    }
    
    if (m_pScrollViewDelegate)
    {
        m_pScrollViewDelegate->scrollViewDidMoved(this);
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
    
    CC_RETURN_IF(m_pContainer == NULL);
    
    CCPoint point = m_pContainer->getFrameOrigin();
    point = this->getScrollWindowNotOutPoint(point);
    this->setContainerFrame(point);
    
    this->updateIndicator();
    this->update(0);
}

void CAScrollView::setContainerFrame(const CCPoint& point, const CCSize& size)
{
    if (!size.equals(CCSizeZero))
    {
        CCRect rect;
        rect.origin = point;
        rect.size = size;
        m_pContainer->setFrame(rect);
    }
    else
    {
        m_pContainer->setFrameOrigin(point);
    }
    
    CCRect rect = m_pContainer->getFrame();
    
    m_bSlidingMinX = fabsf(rect.getMinX()) < FLT_EPSILON;
    m_bSlidingMaxX = fabsf(rect.getMaxX() - this->getBounds().size.width) < FLT_EPSILON;
    m_bSlidingMinY = fabsf(rect.getMinY()) < FLT_EPSILON;
    m_bSlidingMaxY = fabsf(rect.getMaxY() - this->getBounds().size.height) < FLT_EPSILON;
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
            CAScheduler::unschedule(schedule_selector(CAScrollView::closeToPoint), this);
            this->stopDeaccelerateScroll();
            m_tCloseToPoint = this->getViewSize();
            m_pContainer->setAnchorPoint(CCPoint(0.5f, 0.5f));
        }
        else if (m_pTouches->count() == 2)
        {
            CATouch* touch0 = dynamic_cast<CATouch*>(m_pTouches->objectAtIndex(0));
            CATouch* touch1 = dynamic_cast<CATouch*>(m_pTouches->objectAtIndex(1));

            m_fTouchLength = ccpDistance(this->convertToNodeSpace(touch0->getLocation()) ,
                                         this->convertToNodeSpace(touch1->getLocation()));
            
            CCPoint mid_point = ccpMidpoint(this->convertToNodeSpace(touch0->getLocation()),
                                            this->convertToNodeSpace(touch1->getLocation()));
            
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
    
    CCPoint p_container = m_pContainer->getFrameOrigin();
    CCPoint p_off = CCPointZero;
    
    if (m_pTouches->count() == 1)
    {
        p_off = ccpSub(this->convertToNodeSpace(pTouch->getLocation()),
                       this->convertToNodeSpace(pTouch->getPreviousLocation()));
    }
    else if (m_pTouches->count() == 2)
    {
        CATouch* touch0 = dynamic_cast<CATouch*>(m_pTouches->objectAtIndex(0));
        CATouch* touch1 = dynamic_cast<CATouch*>(m_pTouches->objectAtIndex(1));
        CCPoint mid_point = ccpMidpoint(this->convertToNodeSpace(touch0->getLocation()),
                                        this->convertToNodeSpace(touch1->getLocation()));
        
        if (m_fMinimumZoomScale < m_fMaximumZoomScale)
        {
            float touch_lenght = ccpDistance(touch0->getLocation(), touch1->getLocation());
            float scale_off = _px(touch_lenght - m_fTouchLength) * 0.0015f;
            
            m_fZoomScale = m_pContainer->getScale();
            m_fZoomScale += m_fZoomScale * scale_off;
            
            m_fZoomScale = MIN(m_fZoomScale, m_fMaximumZoomScale);
            m_fZoomScale = MAX(m_fZoomScale, m_fMinimumZoomScale);
            
            m_pContainer->setScale(m_fZoomScale);
            m_fTouchLength = touch_lenght;
        }
        
        p_off = ccpSub(mid_point, ccpAdd(m_pContainer->getFrameOrigin(),
                                         m_pContainer->getAnchorPointInPoints() * m_fZoomScale));
    }
    
    
    if (m_bBounces)
    {
        CCSize size = this->getBounds().size;
        CCRect rect = m_pContainer->getFrame();
        rect.size.width = MAX(rect.size.width, size.width);
        rect.size.height = MAX(rect.size.height, size.height);
        CCPoint scale = CCPoint(1.0f, 1.0f);
        
        if (p_container.x > 0)
        {
            scale.x = MAX(0, 0.5f - rect.getMinX() / size.width);
            p_off.x *= scale.x;
        }
        
        if (p_container.y > 0)
        {
            scale.y = MAX(0, 0.5f - rect.getMinY() / size.height);
            p_off.y *= scale.y;
        }
        
        if ((p_container.x + rect.size.width - size.width) < 0)
        {
            scale.x = MAX(0, rect.getMaxX() / size.width - 0.5f);
            p_off.x *= scale.x;
        }
        
        if ((p_container.y + rect.size.height - size.height) < 0)
        {
            scale.y = MAX(0, rect.getMaxY() / size.height - 0.5f);
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
    
    if (p_container.equals(m_pContainer->getFrameOrigin()) == false)
    {
        this->setContainerFrame(p_container);
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
            m_pScrollViewDelegate->scrollViewDidMoved(this);
        }
    }
    
    this->changedFromPullToRefreshView();
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

            this->startDeaccelerateScroll();
            
            if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewDidEndDragging(this);
            }
            m_bTracking = false;
            
            this->detectionFromPullToRefreshView();
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
        this->stopDeaccelerateScroll();
        this->hideIndicator();
    }
}

void CAScrollView::stopDeaccelerateScroll()
{
    m_tInertia = CCPointZero;
    m_bDecelerating = false;
    CAScheduler::unschedule(schedule_selector(CAScrollView::deaccelerateScrolling), this);
}

void CAScrollView::startDeaccelerateScroll()
{
    CAScheduler::unschedule(schedule_selector(CAScrollView::closeToPoint), this);
    CAScheduler::unschedule(schedule_selector(CAScrollView::deaccelerateScrolling), this);
    CAScheduler::unschedule(schedule_selector(CAScrollView::update), this);
    CAScheduler::schedule(schedule_selector(CAScrollView::deaccelerateScrolling), this, 1/60.0f);
    CAScheduler::schedule(schedule_selector(CAScrollView::update), this, 1/60.0f);
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

    CCPoint point = m_pContainer->getFrameOrigin();

    if (m_bBounces)
    {
        CCSize size = this->getBounds().size;
        CCSize cSize = m_pContainer->getFrame().size;
        cSize.width = MAX(cSize.width, size.width);
        cSize.height = MAX(cSize.height, size.height);
        
        float y_max = this->isHeaderRefreshing() ? _px(128) : 0.0f;
        float y_min = this->isFooterRefreshing() ? size.height - cSize.height - _px(128) : size.height - cSize.height;
        
        CCPoint resilience = CCPointZero;
        
        if (point.x > 0)
        {
            resilience.x = (point.x) / size.width;
        }
        
        if (point.y - y_max > 0)
        {
            resilience.y = (point.y - y_max) / size.height;
        }
        
        if ((point.x + cSize.width - size.width) < 0)
        {
            resilience.x = (point.x + cSize.width - size.width) / size.width;
        }
        
        if ((point.y - y_min) < 0)
        {
            resilience.y = (point.y - y_min) / size.height;
        }
        
        resilience = ccpMult(resilience, maxBouncesSpeed(dt));
        
        if (speed.getLength() < resilience.getLength())
        {
            speed = ccpMult(resilience, -1.0f);
            m_tInertia = CCPointZero;
        }
    }

    if (speed.getLength() <= minSpeed(dt) / 2)
    {
        point = this->getScrollWindowNotOutPoint(point);
        this->setContainerFrame(point);
        this->hideIndicator();
        //this->detectionFromPullToRefreshView();
        this->stopDeaccelerateScroll();
    }
    else
    {
        point = ccpAdd(point, speed);
        
        if (this->isScrollWindowNotMaxOutSide(m_pContainer->getFrameOrigin()))
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
        
        //this->changedFromPullToRefreshView();
        this->showIndicator();
        this->setContainerFrame(point);
    }
    
    if (m_pScrollViewDelegate)
    {
        m_pScrollViewDelegate->scrollViewDidMoved(this);
    }
}

void CAScrollView::updateIndicator()
{
    if (m_pIndicatorHorizontal == NULL)
    {
        m_pIndicatorHorizontal = CAIndicator::create(CAIndicator::CAIndicatorTypeHorizontal);
        m_pChildInThis->addObject(m_pIndicatorHorizontal);
        this->insertSubview(m_pIndicatorHorizontal, 1);
    }
    
    if (m_pIndicatorVertical == NULL)
    {
        m_pIndicatorVertical = CAIndicator::create(CAIndicator::CAIndicatorTypeVertical);
        m_pChildInThis->addObject(m_pIndicatorVertical);
        this->insertSubview(m_pIndicatorVertical, 1);
    }
    
    const char indicatorSize = _px(6);

    const CCRect indicatorHorizontalFrame = CCRect(indicatorSize * 2,
                                                   this->getBounds().size.height - indicatorSize * 2,
                                                   this->getBounds().size.width - indicatorSize * 4,
                                                   indicatorSize);
    m_pIndicatorHorizontal->setFrame(indicatorHorizontalFrame);
    
    const CCRect indicatorVerticalFrame = CCRect(this->getBounds().size.width - indicatorSize * 2,
                                                 indicatorSize * 2,
                                                 indicatorSize,
                                                 this->getBounds().size.height - indicatorSize * 4);
    m_pIndicatorVertical->setFrame(indicatorVerticalFrame);
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
    
    x = MIN(x, 0) ;
    x = MAX(x, size.width - cSize.width);
    
    return x;
}

float CAScrollView::getScrollWindowNotOutVertical(float y)
{
    CCSize size = this->getBounds().size;
    CCSize cSize = m_pContainer->getFrame().size;
    cSize.width = MAX(cSize.width, size.width);
    cSize.height = MAX(cSize.height, size.height);
    
    float y_max = this->isHeaderRefreshing() ? _px(128) : 0.0f;
    float y_min = this->isFooterRefreshing() ? size.height - cSize.height - _px(128) : size.height - cSize.height;

    y = MIN(y, y_max);
    y = MAX(y, y_min);

    return y;
}

bool CAScrollView::isScrollWindowNotOutSide()
{
    CCSize size = this->getBounds().size;
    CCRect rect = m_pContainer->getFrame();
    
    if (rect.getMinX() > 0.5f)
    {
        return true;
    }
    
    if ((rect.getMaxX() - size.width) < -0.5f)
    {
        return true;
    }
    
    float y_max = this->isHeaderRefreshing() ? _px(128) : 0.0f;
    float y_min = (this->isFooterRefreshing() ? _px(128) : 0.0f) - size.height;
    
    if (rect.getMinY() + y_max > 0.5f)
    {
        return true;
    }
    
    if (rect.getMaxY() + y_min < -0.5f)
    {
        return true;
    }
    
    return false;
}


bool CAScrollView::isScrollWindowNotMaxOutSide(const CCPoint& point)
{
    CCSize size = this->getBounds().size;
    CCRect rect = m_pContainer->getFrame();
    
    if (point.x - maxBouncesLenght() > 0)
    {
        return true;
    }
    
    if ((point.x + rect.size.width - size.width + maxBouncesLenght()) < 0)
    {
        return true;
    }
    
    float y_max = this->isHeaderRefreshing() ? _px(128) : 0.0f;
    float y_min = (this->isFooterRefreshing() ? _px(128) : 0.0f);
    
    if (point.y - maxBouncesLenght() - y_max > 0)
    {
        return true;
    }
    
    if ((point.y + rect.size.height - size.height + maxBouncesLenght() + y_min) < 0)
    {
        return true;
    }
    
    return false;
}

void CAScrollView::setHeaderRefreshView(CrossApp::CAPullToRefreshView *var)
{
    this->removeSubview(m_pHeaderRefreshView);
    CC_SAFE_RELEASE(m_pHeaderRefreshView);
    CC_SAFE_RETAIN(var);
    m_pHeaderRefreshView = var;
}

CAPullToRefreshView* CAScrollView::getHeaderRefreshView()
{
    return m_pHeaderRefreshView;
}

void CAScrollView::setFooterRefreshView(CrossApp::CAPullToRefreshView *var)
{
    this->removeSubview(m_pFooterRefreshView);
    CC_SAFE_RELEASE(m_pFooterRefreshView);
    CC_SAFE_RETAIN(var);
    m_pFooterRefreshView = var;
}

CAPullToRefreshView* CAScrollView::getFooterRefreshView()
{
    return m_pFooterRefreshView;
}

void CAScrollView::endHeaderRefresh()
{
    if (m_pHeaderRefreshView)
    {
        m_pHeaderRefreshView->setPullToRefreshStateType(CAPullToRefreshView::CAPullToRefreshStateNone);
    }
}

void CAScrollView::endFooterRefresh()
{
    if (m_pFooterRefreshView)
    {
        m_pFooterRefreshView->setPullToRefreshStateType(CAPullToRefreshView::CAPullToRefreshStateNone);
    }
}

void CAScrollView::layoutPullToRefreshView()
{
    CADipSize viewSize = this->getViewSize();
    if (m_pHeaderRefreshView)
    {
        m_pHeaderRefreshView->setFrame(CADipRect(0, -128.0f, viewSize.width, 128.0f));
        if (m_pHeaderRefreshView->getSuperview() == NULL)
        {
            m_pContainer->addSubview(m_pHeaderRefreshView);
        }
        if (m_pHeaderRefreshView->isLayoutFinish() == false)
        {
            m_pHeaderRefreshView->startLayout();
        }
        this->endHeaderRefresh();
    }

    if (m_pFooterRefreshView)
    {
        m_pFooterRefreshView->setFrame(CADipRect(0, viewSize.height, viewSize.width, 128.0f));
        if (m_pFooterRefreshView->getSuperview() == NULL)
        {
            m_pContainer->addSubview(m_pFooterRefreshView);
        }
        if (m_pFooterRefreshView->isLayoutFinish() == false)
        {
            m_pFooterRefreshView->startLayout();
        }
        this->endFooterRefresh();
    }
}

void CAScrollView::changedFromPullToRefreshView()
{
    CCRect rect = this->getBounds();
	rect.origin = this->getContentOffset();
    
    if (m_pHeaderRefreshView)
    {
        if (m_pHeaderRefreshView->getFrame().origin.y < rect.origin.y)
        {
            m_pHeaderRefreshView->setPullToRefreshStateType(CAPullToRefreshView::CAPullToRefreshStateNormal);
        }
        else
        {
            m_pHeaderRefreshView->setPullToRefreshStateType(CAPullToRefreshView::CAPullToRefreshStatePulling);
        }
    }
    if (m_pFooterRefreshView)
    {
        if (m_pFooterRefreshView->getFrame().origin.y + m_pFooterRefreshView->getFrame().size.height > rect.origin.y + rect.size.height)
        {
            m_pFooterRefreshView->setPullToRefreshStateType(CAPullToRefreshView::CAPullToRefreshStateNormal);
        }
        else
        {
            m_pFooterRefreshView->setPullToRefreshStateType(CAPullToRefreshView::CAPullToRefreshStatePulling);
        }
    }
}

void CAScrollView::detectionFromPullToRefreshView()
{
    if (m_pHeaderRefreshView && m_pHeaderRefreshView->isCanRefresh())
    {
        this->stopDeaccelerateScroll();
        this->setContentOffset(CCPoint(0, -_px(128)), true);
        m_pHeaderRefreshView->setPullToRefreshStateType(CAPullToRefreshView::CAPullToRefreshStateRefreshing);
        if (m_pScrollViewDelegate)
        {
            m_pScrollViewDelegate->scrollViewHeaderBeginRefreshing(this);
        }
    }
    
    if (m_pFooterRefreshView && m_pFooterRefreshView->isCanRefresh())
    {
        this->stopDeaccelerateScroll();
        this->setContentOffset(CCPoint(0, this->getViewSize().height - this->getBounds().size.height + _px(128)), true);
        m_pFooterRefreshView->setPullToRefreshStateType(CAPullToRefreshView::CAPullToRefreshStateRefreshing);
        
        if (m_pScrollViewDelegate)
        {
            m_pScrollViewDelegate->scrollViewFooterBeginRefreshing(this);
        }
    }
}

bool CAScrollView::isHeaderRefreshing()
{
    return m_pHeaderRefreshView && m_pHeaderRefreshView->isRefreshing();
}

bool CAScrollView::isFooterRefreshing()
{
    return m_pFooterRefreshView && m_pFooterRefreshView->isRefreshing();
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
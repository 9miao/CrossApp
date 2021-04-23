//
//  CAScrollView.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAScrollView.h"
#include "CAScale9ImageView.h"
#include "basics/CAApplication.h"
#include "dispatcher/CATouchDispatcher.h"
#include "basics/CAScheduler.h"
#include "dispatcher/CATouch.h"
#include "basics/CAPointExtension.h"
#include "CCEGLView.h"
#include "animation/CAViewAnimation.h"
#include "animation/CACustomAnimation.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

#pragma CAScrollView

CAScrollView::CAScrollView()
:m_pContainer(nullptr)
,m_obViewSize(DSizeZero)
,m_bTouchEnabledAtSubviews(true)
,m_pScrollViewDelegate(nullptr)
,m_bBounces(true)
,m_bBounceHorizontal(true)
,m_bBounceVertical(true)
,m_bTracking(false)
,m_bDecelerating(false)
,m_bZooming(false)
,m_fMaximumZoomScale(1.0f)
,m_fMinimumZoomScale(1.0f)
,m_fZoomScale(1.0f)
,m_fBeganTouchLength(0.0f)
,m_fBeganZoomScale(1.0f)
,m_fBeganGestureAngle(0.0f)
,m_fBeganAngle(0.0f)
,m_fPreviewScale(0.0f)
,m_tInertia(DPointZero)
,m_tCloseToPoint(DPointZero)
,m_tInitialPoint(DPointZero)
,m_pIndicatorHorizontal(nullptr)
,m_pIndicatorVertical(nullptr)
,m_bShowsHorizontalScrollIndicator(true)
,m_bShowsVerticalScrollIndicator(true)
,m_eMultitouchGesture(MultitouchGesture::None)
,m_pHeaderRefreshView(nullptr)
,m_pFooterRefreshView(nullptr)
,m_pBackgroundView(nullptr)
,m_bPCMode(false)
{
    this->setPriorityScroll(true);
    this->setReachBoundaryHandOverToSuperview(true);
    this->setHaveNextResponder(false);
}

CAScrollView::~CAScrollView()
{
	CACustomAnimation::unschedule("contentOffset:" + m_s__StrID);
    CC_SAFE_RELEASE(m_pHeaderRefreshView);
    CC_SAFE_RELEASE(m_pFooterRefreshView);
    m_vChildInThis.clear();
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
    m_tInertia = DPointZero;
    this->setScrollRunning(false);
}

CAScrollView* CAScrollView::createWithFrame(const DRect& rect)
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

CAScrollView* CAScrollView::createWithCenter(const DRect& rect)
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

CAScrollView* CAScrollView::createWithLayout(const CrossApp::DLayout &layout)
{
    CAScrollView* scrollView = new CAScrollView();
    if (scrollView && scrollView->initWithLayout(layout))
    {
        scrollView->autorelease();
        return scrollView;
    }
    CC_SAFE_DELETE(scrollView);
    return NULL;
}

bool CAScrollView::init()
{
    this->setDisplayRange(false);
    this->setBackgroundImage(CAImage::WHITE_IMAGE());
    
    m_pContainer = new CAView();
    m_pContainer->setAnchorPoint(DPointZero);
    m_pContainer->setPoint(DPointZero);
    m_vChildInThis.pushBack(m_pContainer);
    this->addSubview(m_pContainer);
    m_pContainer->release();
    
    this->setViewSize(m_obViewSize);
    
    this->initIndicator();
    return true;
}

void CAScrollView::addSubview(CAView* subview)
{
    do
    {
        CC_BREAK_IF(m_vChildInThis.contains(subview));
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
        CC_BREAK_IF(m_vChildInThis.contains(subview));
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

void CAScrollView::removeSubviewByTextTag(const std::string& textTag)
{
    m_pContainer->removeSubviewByTextTag(textTag);
}

CAView* CAScrollView::getSubviewByTag(int aTag)
{
    return m_pContainer->getSubviewByTag(aTag);
}

CAView* CAScrollView::getSubviewByTextTag(const std::string& textTag)
{
    return m_pContainer->getSubviewByTextTag(textTag);
}

const CAVector<CAView*>& CAScrollView::getSubviews()
{
    return m_pContainer->getSubviews();
}

void CAScrollView::setViewSize(const DSize& var)
{
    m_obViewSize = var;
    
    if (m_pContainer)
    {
        DPoint anchorPoint = m_pContainer->getAnchorPoint();
        m_pContainer->setAnchorPoint(DPointZero);
        
        DPoint point = m_pContainer->m_obPoint;
        this->setContainerPoint(point, var);
        
        m_pContainer->setAnchorPoint(anchorPoint);
        
        m_fPreviewScale = MIN(m_obContentSize.width / m_pContainer->m_obContentSize.width, m_obContentSize.height / m_pContainer->m_obContentSize.height);
        
        if (this->isScrollWindowNotOutSide())
        {
            this->getScrollWindowNotOutPoint(point);
            this->setContainerPoint(point);
        }
        
        if (m_bRunning)
        {
            this->updateIndicator();
        }
    }
}

const DSize& CAScrollView::getViewSize()
{
    return m_pContainer->m_obContentSize;
}

bool CAScrollView::isReachBoundaryLeft()
{
    do
    {
        CC_BREAK_IF(m_fMaximumZoomScale - m_fMinimumZoomScale < FLT_EPSILON);
        CC_BREAK_IF(CAApplication::getApplication()->getTouchDispatcher()->getTouchCount() < 2);
        return false;
    }
    while (0);

    float minX = m_pContainer->m_obPoint.x - m_pContainer->getAnchorPointInPoints().x * m_pContainer->getScale();
    
    return minX >= 0.0f;
}

bool CAScrollView::isReachBoundaryRight()
{
    do
    {
        CC_BREAK_IF(m_fMaximumZoomScale - m_fMinimumZoomScale < FLT_EPSILON);
        CC_BREAK_IF(CAApplication::getApplication()->getTouchDispatcher()->getTouchCount() < 2);
        return false;
    }
    while (0);

    float maxX = m_pContainer->m_obPoint.x + (m_pContainer->m_obContentSize.width - m_pContainer->getAnchorPointInPoints().x) * m_pContainer->getScale();
    
    return maxX <= m_obContentSize.width;
}

bool CAScrollView::isReachBoundaryUp()
{
    do
    {
        CC_BREAK_IF(m_fMaximumZoomScale - m_fMinimumZoomScale < FLT_EPSILON);
        CC_BREAK_IF(CAApplication::getApplication()->getTouchDispatcher()->getTouchCount() < 2);
        return false;
    }
    while (0);

    float minY = m_pContainer->m_obPoint.y - m_pContainer->getAnchorPointInPoints().y * m_pContainer->getScale();
    
    return minY >= 0.0f;
}

bool CAScrollView::isReachBoundaryDown()
{
    do
    {
        CC_BREAK_IF(m_fMaximumZoomScale - m_fMinimumZoomScale < FLT_EPSILON);
        CC_BREAK_IF(CAApplication::getApplication()->getTouchDispatcher()->getTouchCount() < 2);
        return false;
    }
    while (0);

    float maxY = m_pContainer->m_obPoint.y + (m_pContainer->m_obContentSize.height - m_pContainer->getAnchorPointInPoints().y) * m_pContainer->getScale();
    
    return maxY <= m_obContentSize.height;
}

void CAScrollView::setTouchEnabledAtSubviews(bool var)
{
    CC_RETURN_IF(m_bTouchEnabledAtSubviews == var);
    m_bTouchEnabledAtSubviews = var;
    m_pContainer->setTouchEnabled(var);
}

bool CAScrollView::isTouchEnabledAtSubviews()
{
    return m_bTouchEnabledAtSubviews;
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

void CAScrollView::setShowsScrollIndicators(bool var)
{
    m_bShowsScrollIndicators = var;
    if (!var)
    {
        m_pIndicatorHorizontal->setVisible(var);
        m_pIndicatorVertical->setVisible(var);
    }
    else
    {
        m_pIndicatorHorizontal->setVisible(var && m_bShowsHorizontalScrollIndicator);
        m_pIndicatorVertical->setVisible(var && m_bShowsVerticalScrollIndicator);
    }
    this->updateIndicatorLayout();
}

bool CAScrollView::isShowsScrollIndicators()
{
    return m_bShowsScrollIndicators;
}

void CAScrollView::setShowsHorizontalScrollIndicator(bool var)
{
    m_bShowsHorizontalScrollIndicator = var;
    m_pIndicatorHorizontal->setVisible(var);
    this->updateIndicatorLayout();
}

bool CAScrollView::isShowsHorizontalScrollIndicator()
{
    return m_bShowsHorizontalScrollIndicator;
}

void CAScrollView::setShowsVerticalScrollIndicator(bool var)
{
    m_bShowsVerticalScrollIndicator = var;
    m_pIndicatorVertical->setVisible(var);
    this->updateIndicatorLayout();
}

bool CAScrollView::isShowsVerticalScrollIndicator()
{
    return m_bShowsVerticalScrollIndicator;
}

void CAScrollView::setContentOffset(const DPoint& offset, bool animated)
{
    DPoint point = ccpMult(offset, -1);
    point = ccpAdd(point, ccpMult(m_pContainer->getAnchorPointInPoints(), m_pContainer->getScale()));

    if (animated)
    {
        m_tInertia = DPointZero;
        m_tCloseToPoint = point;
        m_tInitialPoint = m_pContainer->m_obPoint;
        CACustomAnimation::schedule([&](const CACustomAnimation::Model& model)
        {
            if (model.now < model.total)
            {
                DPoint offSet = ccpSub(m_tCloseToPoint, m_tInitialPoint);
                offSet = ccpMult(offSet, model.now / model.total);
                this->setContainerPoint(ccpAdd(m_tInitialPoint, offSet));
                
                if (m_obDidMoved)
                {
                    m_obDidMoved();
                }
                else if (m_pScrollViewDelegate)
                {
                    m_pScrollViewDelegate->scrollViewDidMoved(this);
                }
                this->changedFromPullToRefreshView();
            }
            else
            {
                this->setContainerPoint(m_tCloseToPoint);
                this->update(model.dt);
                this->hideIndicator();
                m_tCloseToPoint = this->getViewSize();
                m_tInitialPoint = m_tCloseToPoint;
                this->changedFromPullToRefreshView();
                
                if (m_obDidMoved)
                {
                    m_obDidMoved();
                }
                else if (m_pScrollViewDelegate)
                {
                    m_pScrollViewDelegate->scrollViewDidMoved(this);
                }
                this->performSelector(callfunc_selector(CAScrollView::contentOffsetFinish), 0);
            }
        }, "contentOffset:" + m_s__StrID, 0.2f);
    }
    else
    {
        this->setContainerPoint(point);
        this->update(1/60.0f);
        
        if (m_obDidMoved)
        {
            m_obDidMoved();
        }
        else if (m_pScrollViewDelegate)
        {
            m_pScrollViewDelegate->scrollViewDidMoved(this);
        }
        this->performSelector(callfunc_selector(CAScrollView::contentOffsetFinish), 0);
    }
}

DPoint CAScrollView::getContentOffset()
{
    return ccpMult(ccpSub(m_pContainer->m_obPoint, ccpMult(m_pContainer->getAnchorPointInPoints(), m_pContainer->getScale())), -1);
}

void CAScrollView::setBackgroundImage(CAImage* image, bool isScale9)
{
    CAView::removeSubview(m_pBackgroundView);
    m_pBackgroundView = nullptr;
    if (image)
    {
        if (isScale9)
        {
            CAScale9ImageView* backgroundView = CAScale9ImageView::createWithImage(image);
            backgroundView->setLayout(DLayoutFill);
            CAView::insertSubview(backgroundView, -1);
            m_pBackgroundView = backgroundView;
        }
        else
        {
            CAImageView* backgroundView = CAImageView::createWithImage(image);
            backgroundView->setLayout(DLayoutFill);
            CAView::insertSubview(backgroundView, -1);
            m_pBackgroundView = backgroundView;
        }
    }
}

void CAScrollView::setZoomScale(float zoom)
{
    zoom = MAX(zoom, m_fMinimumZoomScale);
    zoom = MIN(zoom, m_fMaximumZoomScale);
    m_fZoomScale = zoom;
    m_pContainer->setScale(m_fZoomScale);
}

void CAScrollView::setContentSize(const CrossApp::DSize &var)
{
    CAView::setContentSize(var);
    this->setViewSize(m_obViewSize);
}

void CAScrollView::setContainerPoint(const DPoint& point, const DSize& size)
{
    if (!size.equals(DSize(-1, -1)))
    {
        DSize contentSize;
        contentSize.width = MAX(size.width, m_obContentSize.width);
        contentSize.height = MAX(size.height, m_obContentSize.height);
        m_pContainer->setContentSize(contentSize);
    }
    m_pContainer->setPoint(point);
}

bool CAScrollView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (m_bPCMode)
    {
        return true;
    }
    
    do
    {
        CC_BREAK_IF(m_vTouches.size() > 2);
        
        if (!m_vTouches.contains(pTouch))
        {
            m_vTouches.pushBack(pTouch);
        }
        
//        if (m_fZoomScale < m_fPreviewScale)
//        {
//            m_fZoomScale = m_fPreviewScale;
//            m_pContainer->setAnchorPoint(DPoint(0.5f, 0.5f));
//            
//            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
//            CAViewAnimation::setAnimationDuration(0.05f);
//            CAViewAnimation::beginAnimations("", NULL);
//            m_pContainer->setScale(m_fZoomScale);
//            CAViewAnimation::setAnimationDidStopSelector(CAApplication::getApplication()->getTouchDispatcher(),
//                                                         CAViewAnimation0_selector(CATouchDispatcher::setDispatchEventsTrue));
//            CAViewAnimation::commitAnimations();
//            
//            return false;
//        }
        
        if (m_vTouches.size() == 1)
        {
            CACustomAnimation::unschedule("contentOffset:" + m_s__StrID);
            this->stopDeaccelerateScroll();
            m_tCloseToPoint = this->getViewSize();
            m_tInitialPoint = m_tCloseToPoint;
            
            CAScheduler::getScheduler()->schedule(schedule_selector(CAScrollView::updatePointOffset), this, 0);
        }
        else if (m_vTouches.size() == 2)
        {
            CATouch* touch0 = dynamic_cast<CATouch*>(m_vTouches.at(0));
            CATouch* touch1 = dynamic_cast<CATouch*>(m_vTouches.at(1));
            
            if (m_eMultitouchGesture == MultitouchGesture::Rotate)
            {
                m_pContainer->setAnchorPoint(DPoint(0.5f, 0.5f));
            }
            
            if (m_eMultitouchGesture == MultitouchGesture::Zoom || m_eMultitouchGesture == MultitouchGesture::ZoomAndRotate)
            {
                m_fBeganZoomScale = m_fZoomScale;
                m_fBeganTouchLength = ccpDistance(this->convertToNodeSpace(touch0->getLocation()) ,
                                                  this->convertToNodeSpace(touch1->getLocation()));
                
                DPoint mid_point = ccpMidpoint(m_pContainer->convertToNodeSpace(touch0->getLocation()),
                                               m_pContainer->convertToNodeSpace(touch1->getLocation()));
                m_pContainer->setAnchorPointInPoints(mid_point);
                
                if (m_obDidZoom)
                {
                    m_obDidZoom();
                }
                else if (m_pScrollViewDelegate)
                {
                    m_pScrollViewDelegate->scrollViewDidZoom(this);
                }
                m_bZooming = true;
            }

            if (m_eMultitouchGesture == MultitouchGesture::Rotate || m_eMultitouchGesture == MultitouchGesture::ZoomAndRotate)
            {
                DPoint beganRotationVector = ccpSub(this->convertToNodeSpace(touch0->getLocation()),
                                                    this->convertToNodeSpace(touch1->getLocation()));
                
                m_fBeganGestureAngle = CC_RADIANS_TO_DEGREES(ccpAngleSigned(beganRotationVector, DPoint(1, 0)));
                m_fBeganAngle = m_pContainer->getRotation();
            }

            m_tPointOffset.clear();
            CAScheduler::getScheduler()->unschedule(schedule_selector(CAScrollView::updatePointOffset), this);
        }
        
        return true;
    }
    while (0);
    
    return false;
}

void CAScrollView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_bPCMode);
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
    DPoint p_container = m_pContainer->m_obPoint;
    DPoint p_off = DPointZero;
    
    if (m_vTouches.size() == 1)
    {
        p_off = ccpSub(this->convertToNodeSpace(pTouch->getLocation()),
                       this->convertToNodeSpace(pTouch->getPreviousLocation()));
        
        this->updatePointOffset();
    }
    else if (m_vTouches.size() == 2)
    {
        CATouch* touch0 = dynamic_cast<CATouch*>(m_vTouches.at(0));
        CATouch* touch1 = dynamic_cast<CATouch*>(m_vTouches.at(1));
        
        if (m_eMultitouchGesture == MultitouchGesture::Zoom || m_eMultitouchGesture == MultitouchGesture::ZoomAndRotate)
        {
            float touch_length = ccpDistance(this->convertToNodeSpace(touch0->getLocation()) ,
                                             this->convertToNodeSpace(touch1->getLocation()));
            
            m_fZoomScale = m_fBeganZoomScale * touch_length / m_fBeganTouchLength;
            
            m_fZoomScale = MIN(m_fZoomScale, m_fMaximumZoomScale);
            m_fZoomScale = MAX(m_fZoomScale, m_fMinimumZoomScale);
            
            m_pContainer->setScale(m_fZoomScale);
            
            if (m_obDidZoom)
            {
                m_obDidZoom();
            }
            else if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewDidZoom(this);
            }
            m_bZooming = true;
            
        }
        
        if (m_eMultitouchGesture == MultitouchGesture::Rotate || m_eMultitouchGesture == MultitouchGesture::ZoomAndRotate)
        {
            DPoint currRotationVector = ccpSub(this->convertToNodeSpace(touch0->getLocation()),
                                               this->convertToNodeSpace(touch1->getLocation()));
            
            float currGestureAngle = CC_RADIANS_TO_DEGREES(ccpAngleSigned(currRotationVector, DPoint(1, 0)));
            m_pContainer->setRotation(m_fBeganAngle - (currGestureAngle - m_fBeganGestureAngle));
        }
        
        if (m_eMultitouchGesture == MultitouchGesture::Zoom || m_eMultitouchGesture == MultitouchGesture::None)
        {
            DPoint midPoint = ccpMidpoint(this->convertToNodeSpace(touch0->getLocation()),
                                    this->convertToNodeSpace(touch1->getLocation()));
            
            DPoint midPointPrevious = ccpMidpoint(this->convertToNodeSpace(touch0->getPreviousLocation()),
                                          this->convertToNodeSpace(touch1->getPreviousLocation()));
            p_off = ccpSub(midPoint, midPointPrevious);
            p_off = ccpMult(p_off, 0.5f);
        }
    }
    
    if (m_bBounces)
    {
        DSize size = m_obContentSize;
        DPoint curr_point = m_pContainer->m_obPoint;
        DPoint relust_point = curr_point;
        this->getScrollWindowNotOutPoint(relust_point);
        
        float length_x = fabsf(curr_point.x - relust_point.x);
        float length_y = fabsf(curr_point.y - relust_point.y);
        
        DPoint scale = DPoint(1.0f, 1.0f);
        
        if (length_x > FLT_EPSILON)
        {
            scale.x = MAX(0.5f - length_x / size.width, 0.0f);
            p_off.x = p_off.x * scale.x;
        }
        
        if (length_y > FLT_EPSILON)
        {
            scale.y = MAX(0.5f - length_y / size.height, 0.0f);
            p_off.y = p_off.y * scale.y;
        }
    }
    
    p_container = ccpAdd(p_container, p_off);
    
    if (m_bBounces == false)
    {
        this->getScrollWindowNotOutPoint(p_container);
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
    
    if (p_container.equals(m_pContainer->m_obPoint) == false)
    {
        if (m_bTouchEnabledAtSubviews)
        {
            m_pContainer->setTouchEnabled(false);
        }
        this->setContainerPoint(p_container);
        this->showIndicator();
        this->update(0);
        
        if (m_bTracking == false)
        {
            if (m_obWillBeginDragging)
            {
                m_obWillBeginDragging();
            }
            else if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewWillBeginDragging(this);
            }
            m_bTracking = true;
        }
        
        if (m_obDragging)
        {
            m_obDragging();
        }
        else if (m_pScrollViewDelegate)
        {
            m_pScrollViewDelegate->scrollViewDragging(this);
        }
        
        if (m_obDidMoved)
        {
            m_obDidMoved();
        }
        else if (m_pScrollViewDelegate)
        {
            m_pScrollViewDelegate->scrollViewDidMoved(this);
        }
    }
    
    this->changedFromPullToRefreshView();
}

void CAScrollView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_bPCMode);
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
    if (m_vTouches.size() == 1)
    {
        CAScheduler::getScheduler()->unschedule(schedule_selector(CAScrollView::updatePointOffset), this);
        
        if (m_bTracking == true)
        {
            DPoint p = DPointZero;
            if (m_tPointOffset.size() > 0)
            {
                for (auto& var : m_tPointOffset)
                {
                    p = ccpAdd(p, var);
                }
                p = p/m_tPointOffset.size();
            }
            m_tInertia = p;
            m_tPointOffset.clear();
            
            this->startDeaccelerateScroll();
            
            if (m_obDidEndDragging)
            {
                m_obDidEndDragging();
            }
            else if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewDidEndDragging(this);
            }
            m_bTracking = false;
            
            this->detectionFromPullToRefreshView();
        }
        else
        {
            this->startDeaccelerateScroll();
            
            if (m_obTouchUpWithoutMoved)
            {
                m_obTouchUpWithoutMoved(pTouch->getLocation());
            }
            else if (m_pScrollViewDelegate)
            {
                m_pScrollViewDelegate->scrollViewTouchUpWithoutMoved(this, this->convertToNodeSpace(pTouch->getLocation()));
            }
        }
    }
    else if (m_vTouches.size() == 2)
    {
        m_fBeganZoomScale = 1.0f;
        m_fBeganTouchLength = 0.0f;
        m_bZooming = false;
        
        m_pContainer->setAnchorPoint(DPoint(0.5f, 0.5f));
    }
}

void CAScrollView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.contains(pTouch) == false);
    if (m_vTouches.size() == 1)
    {
        m_tPointOffset.clear();
        
        if (m_obDidEndDragging)
        {
            m_obDidEndDragging();
        }
        else if (m_pScrollViewDelegate)
        {
            m_pScrollViewDelegate->scrollViewDidEndDragging(this);
        }
        m_bTracking = false;
        
        if (m_bTouchEnabledAtSubviews)
        {
            m_pContainer->setTouchEnabled(true);
        }
    }
    else if (m_vTouches.size() == 2)
    {
        m_bZooming = false;
    }
    this->startDeaccelerateScroll();
    this->hideIndicator();
}

void CAScrollView::mouseScrollWheel(CATouch* pTouch, float off_x, float off_y, CAEvent* pEvent)
{
    DPoint p_container = m_pContainer->m_obPoint;
    p_container = ccpAdd(p_container, DPoint(off_x, off_y) * 10);
    this->getScrollWindowNotOutPoint(p_container);
    this->setContainerPoint(p_container);
    this->showIndicator();
    this->update(0);
    
    if (m_obDidMoved)
    {
        m_obDidMoved();
    }
    else if (m_pScrollViewDelegate)
    {
        m_pScrollViewDelegate->scrollViewDidMoved(this);
    }
}

void CAScrollView::switchPCMode(bool var)
{
    m_bPCMode = var;
    this->setMouseScrollWheelEnabled(m_bPCMode);
    this->setPriorityScroll(!m_bPCMode);
    this->updateIndicatorLayout();
}

void CAScrollView::updatePointOffset(float dt)
{
    CC_RETURN_IF(m_vTouches.empty());
    CC_RETURN_IF(m_bScrollEnabled == false);
    CATouch* pTouch = dynamic_cast<CATouch*>(m_vTouches.at(0));
    DPoint p_off = ccpSub(this->convertToNodeSpace(pTouch->getLocation()),
                   this->convertToNodeSpace(pTouch->getPreviousLocation()));

    if (p_off.getLength() <= 5)
    {
        p_off = DPointZero;
    }

    m_tPointOffset.push_back(p_off);

    while (m_tPointOffset.size() > 3)
    {
        m_tPointOffset.pop_front();
    }
}

void CAScrollView::stopDeaccelerateScroll()
{
    CAScheduler::getScheduler()->unschedule(schedule_selector(CAScrollView::deaccelerateScrolling), this);
    m_tInertia = DPointZero;
    m_bDecelerating = false;
    this->setScrollRunning(false);
    if (m_bTouchEnabledAtSubviews)
    {
        m_pContainer->setTouchEnabled(true);
    }
}

void CAScrollView::startDeaccelerateScroll()
{
    CC_RETURN_IF(!m_bRunning);
    CACustomAnimation::unschedule("contentOffset:" + m_s__StrID);
    CAScheduler::getScheduler()->unschedule(schedule_selector(CAScrollView::deaccelerateScrolling), this);
    CAScheduler::getScheduler()->schedule(schedule_selector(CAScrollView::deaccelerateScrolling), this, 1/60.0f);
    m_bDecelerating = true;
    this->setScrollRunning(true);
    if (m_bTouchEnabledAtSubviews)
    {
        m_pContainer->setTouchEnabled(false);
    }
}

void CAScrollView::deaccelerateScrolling(float dt)
{
    if (m_tInertia.getLength() > maxSpeed(dt) * 1.5f)
    {
        m_tInertia = ccpMult(m_tInertia, maxSpeed(dt) * 1.5f / m_tInertia.getLength());
    }
    
    DPoint speed = DPointZero;
    if (m_tInertia.getLength() > maxSpeed(dt))
    {
        speed = ccpMult(m_tInertia, maxSpeed(dt) / m_tInertia.getLength());
    }
    else
    {
        speed = m_tInertia;
    }
    
    DPoint point = m_pContainer->m_obPoint;

    if (m_bBounces)
    {
        DPoint resilience = DPointZero;
        DSize size = m_obContentSize;
        DPoint curr_point = m_pContainer->m_obPoint;
        DPoint relust_point = curr_point;
        this->getScrollWindowNotOutPoint(relust_point);
        float off_x = relust_point.x - curr_point.x;
        float off_y = relust_point.y - curr_point.y;
        
        if (fabsf(off_x) > FLT_EPSILON)
        {
            resilience.x = off_x / 800;
        }
        
        if (fabsf(off_y) > FLT_EPSILON)
        {
            resilience.y = off_y / 800;
        }
        
        resilience = ccpMult(resilience, maxSpeed(dt));
        
        if (speed.getLength() < resilience.getLength())
        {
            speed = resilience;
            m_tInertia = DPointZero;
        }
    }
    
    bool isEnded = speed.getLength() < 0.25f;
    if (isEnded)
    {
        m_tInertia = DPointZero;
        this->getScrollWindowNotOutPoint(point);
        this->setContainerPoint(point);
        this->hideIndicator();
        this->stopDeaccelerateScroll();
    }
    else
    {
        point = ccpAdd(point, speed);

        if (this->isScrollWindowNotMaxOutSide(point))
        {
            m_tInertia = DPointZero;
        }
        
        if (m_bBounces == false)
        {
            this->getScrollWindowNotOutPoint(point);
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
        
        if (point.equals(m_pContainer->m_obPoint))
        {
            m_tInertia = DPointZero;
        }
        else
        {
            this->showIndicator();
            this->setContainerPoint(point);
        }
                
        if (fabsf(m_tInertia.x) > 16)
        {
            m_tInertia.x = m_tInertia.x * (1 - decelerationRatio(dt));
        }
        else if (fabsf(m_tInertia.x) > FLT_EPSILON)
        {
            m_tInertia.x = MAX((fabsf(m_tInertia.x) - 0.5f), 0) * fabsf(m_tInertia.x) / m_tInertia.x;
        }
        
        if (fabsf(m_tInertia.y) > 16)
        {
            m_tInertia.y = m_tInertia.y * (1 - decelerationRatio(dt));
        }
        else if (fabsf(m_tInertia.y) > FLT_EPSILON)
        {
            m_tInertia.y = MAX((fabsf(m_tInertia.y) - 0.5f), 0) * fabsf(m_tInertia.y) / m_tInertia.y;
        }
    }
    
    this->update(dt);
    
    if (m_obDidMoved)
    {
        m_obDidMoved();
    }
    else if (m_pScrollViewDelegate)
    {
        m_pScrollViewDelegate->scrollViewDidMoved(this);
    }
    
    if (isEnded)
    {
        if (m_obStopMoved)
        {
            m_obStopMoved();
        }
        else if (m_pScrollViewDelegate)
        {
            m_pScrollViewDelegate->scrollViewStopMoved(this);
        }
    }
}

void CAScrollView::initIndicator()
{
    if (m_pIndicatorHorizontal == NULL)
    {
        m_pIndicatorHorizontal = CAIndicator::create(CAIndicator::Orientation::Horizontal, this);
        m_vChildInThis.pushBack(m_pIndicatorHorizontal);
        this->insertSubview(m_pIndicatorHorizontal, 1);
    }
    
    if (m_pIndicatorVertical == NULL)
    {
        m_pIndicatorVertical = CAIndicator::create(CAIndicator::Orientation::Vertical, this);
        m_vChildInThis.pushBack(m_pIndicatorVertical);
        this->insertSubview(m_pIndicatorVertical, 1);
    }
    
    this->updateIndicatorLayout();
}

void CAScrollView::updateIndicatorLayout()
{
    if (m_pIndicatorHorizontal && m_pIndicatorVertical)
    {
        char indicatorSize = 6;
        
        if (m_bPCMode)
        {
            indicatorSize = 16;
        }
        
        m_pIndicatorHorizontal->switchPCMode(m_bPCMode);
        m_pIndicatorVertical->switchPCMode(m_bPCMode);
        
        if (m_pIndicatorHorizontal->isVisible() && m_pIndicatorVertical->isVisible() == false)
        {
            m_pIndicatorHorizontal->setLayout(DLayout(DHorizontalLayout_L_R(6, 6),
                                                      DVerticalLayout_B_H(6, indicatorSize)));
        }
        else if (m_pIndicatorVertical->isVisible() && m_pIndicatorHorizontal->isVisible() == false)
        {
            m_pIndicatorVertical->setLayout(DLayout(DHorizontalLayout_R_W(6, indicatorSize),
                                                    DVerticalLayout_T_B(6, 6)));
        }
        else if (m_pIndicatorHorizontal->isVisible() && m_pIndicatorVertical->isVisible())
        {
            m_pIndicatorHorizontal->setLayout(DLayout(DHorizontalLayout_L_R(6, 6 + indicatorSize),
                                                      DVerticalLayout_B_H(6, indicatorSize)));
            m_pIndicatorVertical->setLayout(DLayout(DHorizontalLayout_R_W(6, indicatorSize),
                                                    DVerticalLayout_T_B(6, 6 +  + indicatorSize)));
        }
        
        if (m_bRunning)
        {
            this->updateIndicator();
        }
    }
}

void CAScrollView::showIndicator()
{
    if (m_pIndicatorHorizontal)
    {
        m_pIndicatorHorizontal->setHide(false);
    }
    if (m_pIndicatorVertical)
    {
        m_pIndicatorVertical->setHide(false);
    }
}

void CAScrollView::hideIndicator()
{
    if (!m_bPCMode)
    {
        m_pIndicatorHorizontal->setHide(true);
        m_pIndicatorVertical->setHide(true);
    }
}

void CAScrollView::updateIndicator()
{
    if ((m_pIndicatorHorizontal && m_pIndicatorHorizontal->isVisible())
        || (m_pIndicatorVertical && m_pIndicatorVertical->isVisible()))
    {
        DRect rect;
        rect.size = ccpMult(m_pContainer->m_obContentSize, m_pContainer->getScale());
        rect.origin = ccpSub(m_pContainer->m_obPoint,
                             ccpMult(m_pContainer->getAnchorPointInPoints(),
                                     m_pContainer->getScale()));
        
        if (m_pIndicatorHorizontal)
        {
            m_pIndicatorHorizontal->setIndicator(m_obContentSize, rect);
        }
        if (m_pIndicatorVertical)
        {
            m_pIndicatorVertical->setIndicator(m_obContentSize, rect);
        }
    }
}

void CAScrollView::update(float dt)
{
    this->updateIndicatorLayout();
}

void CAScrollView::getScrollWindowNotOutPoint(DPoint& point)
{
    point.x = this->getScrollWindowNotOutHorizontal(point.x);
    point.y = this->getScrollWindowNotOutVertical(point.y);
}

float CAScrollView::getScrollWindowNotOutHorizontal(float x)
{
    DSize size = m_obContentSize;
    DSize cSize = DSize(ccpMult(m_pContainer->m_obContentSize, m_pContainer->getScale()));
    float anchorPointInPointsX = m_pContainer->getAnchorPointInPoints().x * m_pContainer->getScale();
    if (cSize.width >= size.width)
    {
        x = MIN(x, anchorPointInPointsX) ;
        x = MAX(x, size.width - cSize.width + anchorPointInPointsX);
    }
    else
    {
        x = size.width/2 - cSize.width/2 + anchorPointInPointsX;
    }
    
    return x;
}

float CAScrollView::getScrollWindowNotOutVertical(float y)
{
    DSize size = m_obContentSize;
    DSize cSize = DSize(ccpMult(m_pContainer->m_obContentSize, m_pContainer->getScale()));
    float anchorPointInPointsY = m_pContainer->getAnchorPointInPoints().y * m_pContainer->getScale();
    if (cSize.height >= size.height)
    {
        float y_max = this->isHeaderRefreshing() ? 128 : 0.0f;
        float y_min = this->isFooterRefreshing() ? size.height - 128 : size.height;
        
        y = MIN(y, y_max + anchorPointInPointsY);
        y = MAX(y, y_min + anchorPointInPointsY - cSize.height);
    }
    else
    {
        y = size.height/2 - cSize.height/2 + anchorPointInPointsY;
    }

    return y;
}

bool CAScrollView::isScrollWindowNotOutSide()
{
    DPoint point = m_pContainer->m_obPoint;
    this->getScrollWindowNotOutPoint(point);
    
    return !point.equals(m_pContainer->m_obPoint);
}


bool CAScrollView::isScrollWindowNotMaxOutSide(const DPoint& point)
{
    DPoint p = point;
    this->getScrollWindowNotOutPoint(p);
    
    return (fabsf(p.x - point.x) > maxBouncesLength()
            ||
            fabsf(p.y - point.y) > maxBouncesLength());
}

void CAScrollView::setHeaderRefreshView(CrossApp::CAPullToRefreshView *var)
{
    this->removeSubview(m_pHeaderRefreshView);
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pHeaderRefreshView);
    m_pHeaderRefreshView = var;
}

CAPullToRefreshView* CAScrollView::getHeaderRefreshView()
{
    return m_pHeaderRefreshView;
}

void CAScrollView::setFooterRefreshView(CrossApp::CAPullToRefreshView *var)
{
    this->removeSubview(m_pFooterRefreshView);
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pFooterRefreshView);
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
        m_pHeaderRefreshView->setState(CAPullToRefreshView::State::None);
    }
}

void CAScrollView::endFooterRefresh()
{
    if (m_pFooterRefreshView)
    {
        m_pFooterRefreshView->setState(CAPullToRefreshView::State::None);
    }
}

void CAScrollView::layoutPullToRefreshView()
{
    if (m_pHeaderRefreshView)
    {
        m_pHeaderRefreshView->startLayout();
        this->endHeaderRefresh();
    }

    if (m_pFooterRefreshView)
    {
        m_pFooterRefreshView->startLayout();
        this->endFooterRefresh();
    }
}

void CAScrollView::changedFromPullToRefreshView()
{
    DSize size = this->getViewSize();
	float y = this->getContentOffset().y;
    
    if (m_pHeaderRefreshView)
    {
        const DLayout& layout = DLayout(DHorizontalLayoutFill, DVerticalLayout_T_H(-128, 128));
        if (m_pHeaderRefreshView && m_pHeaderRefreshView->getSuperview() == NULL)
        {
            m_pHeaderRefreshView->setLayout(layout);
            this->addSubview(m_pHeaderRefreshView);
        }
        if (layout.vertical.top < y)
        {
            m_pHeaderRefreshView->setState(CAPullToRefreshView::State::Normal);
        }
        else
        {
            m_pHeaderRefreshView->setState(CAPullToRefreshView::State::Pulling);
        }
    }
    if (m_pFooterRefreshView)
    {
        const DLayout& layout = DLayout(DHorizontalLayoutFill, DVerticalLayout_B_H(-128, 128));
        if (m_pFooterRefreshView && m_pFooterRefreshView->getSuperview() == NULL)
        {
            m_pFooterRefreshView->setLayout(layout);
            this->addSubview(m_pFooterRefreshView);
        }
        if (size.height - m_obContentSize.height - layout.vertical.bottom > y)
        {
            m_pFooterRefreshView->setState(CAPullToRefreshView::State::Normal);
        }
        else
        {
            m_pFooterRefreshView->setState(CAPullToRefreshView::State::Pulling);
        }
    } 
}

void CAScrollView::detectionFromPullToRefreshView()
{
    if (m_pHeaderRefreshView && m_pHeaderRefreshView->isCanRefresh())
    {
        this->stopDeaccelerateScroll();
        this->setContentOffset(DPoint(0, -128), true);
        m_pHeaderRefreshView->setState(CAPullToRefreshView::State::Refreshing);
        if (m_obHeaderBeginRefreshing || m_pScrollViewDelegate)
        {
            this->performSelector(callfunc_selector(CAScrollView::headerWillBeginRefreshing), 0.1f);
        }
    }
    
    if (m_pFooterRefreshView && m_pFooterRefreshView->isCanRefresh())
    {
        this->stopDeaccelerateScroll();
        this->setContentOffset(DPoint(0, this->getViewSize().height - this->getBounds().size.height + 128), true);
        m_pFooterRefreshView->setState(CAPullToRefreshView::State::Refreshing);
        if (m_obFooterBeginRefreshing || m_pScrollViewDelegate)
        {
            this->performSelector(callfunc_selector(CAScrollView::footerWillBeginRefreshing), 0.1f);
        }
    }
}

void CAScrollView::headerWillBeginRefreshing()
{
    if (m_obHeaderBeginRefreshing)
    {
        m_obHeaderBeginRefreshing();
    }
    else if (m_pScrollViewDelegate)
    {
        m_pScrollViewDelegate->scrollViewHeaderBeginRefreshing(this);
    }
}

void CAScrollView::footerWillBeginRefreshing()
{
    if (m_obFooterBeginRefreshing)
    {
        m_obFooterBeginRefreshing();
    }
    else if (m_pScrollViewDelegate)
    {
        m_pScrollViewDelegate->scrollViewFooterBeginRefreshing(this);
    }
}

void CAScrollView::startPullToHeaderRefreshView()
{
    this->setContentOffset(DPoint(0, -128.0f), true);
    this->performSelector(callfunc_selector(CAScrollView::detectionFromPullToRefreshView), 0.3);
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

CAIndicator::CAIndicator(CAIndicator::Orientation type, CAScrollView* var)
:m_pIndicator(nullptr)
,m_eType(type)
,m_bPCMode(false)
,m_pMyScrollView(var)
,m_bTouch(false)
,m_bHide(true)
{
    this->setHaveNextResponder(false);
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

CAIndicator* CAIndicator::create(CAIndicator::Orientation type, CAScrollView* var)
{
    CAIndicator* indicator = new CAIndicator(type, var);
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
    this->setColor(CAColor4B::CLEAR);
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAIndicator");
    CAImage* image = CAImage::create(map.at("backgroundView"));
    
    
    CAScale9ImageView* indicator = CAScale9ImageView::createWithImage(image);
    if (image)
    {
        DSize size = image->getContentSize();
        indicator->setImage(image);
        indicator->setCapInsets(DRect(size.width / 2 - 1, size.height / 2 - 1, 2, 2));
    }
    this->addSubview(indicator);
    m_pIndicator = indicator;
    
    this->setAlpha(0.0f);
    
    return true;
}

void CAIndicator::setIndicator(const DSize& parentSize, const DRect& childrenFrame)
{
    CC_RETURN_IF(!this->isVisible());
    CC_RETURN_IF(m_bTouch);
    
    CAScale9ImageView* indicator = dynamic_cast<CAScale9ImageView*>(m_pIndicator);
    
    if (m_eType == CAIndicator::Orientation::Horizontal)
    {
        float size_scale_x = parentSize.width / childrenFrame.size.width;
        size_scale_x = MIN(size_scale_x, 1.0f);
        
        float length_scale_x = parentSize.width / 2 - childrenFrame.origin.x;
        length_scale_x /= childrenFrame.size.width;
        
        DRect rect;
        rect.size = m_obContentSize;
        rect.size.width *= size_scale_x;
        
        if (length_scale_x < size_scale_x / 2)
        {
            rect.size.width *= length_scale_x / (size_scale_x / 2);
        }
        if (1 - length_scale_x < size_scale_x / 2)
        {
            rect.size.width *= (1 - length_scale_x) / (size_scale_x / 2);
        }
        rect.size.width = MAX(rect.size.height, rect.size.width);
        
        rect.origin = m_obContentSize;
        rect.origin.y *= 0.5f;
        rect.origin.x *= length_scale_x;
        rect.origin.x = MAX(rect.origin.x, rect.size.width/2);
        rect.origin.x = MIN(rect.origin.x, m_obContentSize.width - rect.size.width/2);
        
        indicator->setCenter(rect);
    }
    else if (m_eType == CAIndicator::Orientation::Vertical)
    {
        float size_scale_y = parentSize.height / childrenFrame.size.height;
        size_scale_y = MIN(size_scale_y, 1.0f);
        
        float length_scale_y = parentSize.height / 2 - childrenFrame.origin.y;
        length_scale_y /= childrenFrame.size.height;
        
        DRect rect;
        rect.size = m_obContentSize;
        rect.size.height *= size_scale_y;
        
        if (length_scale_y < size_scale_y / 2)
        {
            rect.size.height *= length_scale_y / (size_scale_y / 2);
        }
        if (1 - length_scale_y < size_scale_y / 2)
        {
            rect.size.height *= (1 - length_scale_y) / (size_scale_y / 2);
        }
        rect.size.height = MAX(rect.size.height, rect.size.width);

        rect.origin = m_obContentSize;
        rect.origin.x *= 0.5f;
        rect.origin.y *= length_scale_y;
        rect.origin.y = MAX(rect.origin.y, rect.size.height/2);
        rect.origin.y = MIN(rect.origin.y, m_obContentSize.height - rect.size.height/2);
        
        indicator->setCenter(rect);
    }
}

void CAIndicator::setHide(bool var)
{
    CC_RETURN_IF(m_bHide == var);
    m_bHide = var;
    
    CAViewAnimation::removeAnimations(m_s__StrID);
    if (var == false)
    {
        CC_RETURN_IF(fabs(1.0f-this->getAlpha()) < FLT_EPSILON);
        
        this->setAlpha(1.0f);
    }
    else
    {
        CC_RETURN_IF(1.0f-this->getAlpha() > FLT_EPSILON);
        CAViewAnimation::beginAnimations(m_s__StrID);
        CAViewAnimation::setAnimationDuration(0.3f);
        this->setAlpha(0.0f);
        CAViewAnimation::commitAnimations();
    }
}

bool CAIndicator::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    m_bTouch = (m_pIndicator->getBounds().containsPoint(m_pIndicator->convertTouchToNodeSpace(pTouch)));
    return m_bTouch;
}

void CAIndicator::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = m_pIndicator->getCenterOrigin();
    DPoint p_off = ccpSub(this->convertToNodeSpace(pTouch->getLocation()),
                           this->convertToNodeSpace(pTouch->getPreviousLocation()));
    
    DSize size = m_pIndicator->getFrame().size;
    
    if (m_eType == CAIndicator::Orientation::Horizontal)
    {
        point.x += p_off.x;
        point.x = MAX(point.x, size.width/2);
        point.x = MIN(point.x, m_obContentSize.width - size.width/2);
    }
    else
    {
        point.y += p_off.y;
        point.y = MAX(point.y, size.height/2);
        point.y = MIN(point.y, m_obContentSize.height - size.height/2);
    }
    m_pIndicator->setCenterOrigin(point);
    
    DSize indictor_size = ccpSub(m_obContentSize, size);
    if (!indictor_size.equals(DSizeZero))
    {
        DPoint indictor_point = ccpSub(point, ccpMult(size, 0.5f));
        DSize view_size = ccpSub(m_pMyScrollView->getViewSize(), m_pMyScrollView->getBounds().size);
        DPoint view_offset = m_pMyScrollView->getContentOffset();
        if (m_eType == CAIndicator::Orientation::Horizontal)
        {
            view_offset.x = indictor_point.x / indictor_size.width * view_size.width;
        }
        else
        {
            view_offset.y = indictor_point.y / indictor_size.height * view_size.height;
        }
        m_pMyScrollView->setContentOffset(view_offset, false);
    }
}

void CAIndicator::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    m_bTouch = false;
}

void CAIndicator::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    m_bTouch = false;
}

void CAIndicator::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
    m_pIndicator->setColor(CAColor4B::GRAY);
}

void CAIndicator::mouseMovedOutSide(CATouch* pTouch, CAEvent* pEvent)
{
    m_pIndicator->setColor(CAColor4B::WHITE);
}

void CAIndicator::switchPCMode(bool var)
{
    m_bPCMode = var;
    this->setPriorityScroll(m_bPCMode);
    this->setHaveNextResponder(!m_bPCMode);
    this->setMouseMovedEnabled(m_bPCMode);
    if (m_bPCMode)
    {
        this->setHide(false);
    }
}

NS_CC_END

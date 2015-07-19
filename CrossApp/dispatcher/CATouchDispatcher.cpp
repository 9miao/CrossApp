//
//  CATouchDispatcher.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CATouchDispatcher.h"
#include "cocoa/CCSet.h"
#include "CATouch.h"
#include "ccMacros.h"
#include <algorithm>
#include "basics/CAApplication.h"
#include "view/CAWindow.h"
#include "basics/CAScheduler.h"
#include "support/CCPointExtension.h"


NS_CC_BEGIN

CATouchController::CATouchController()
:m_pTouch(NULL)
,m_pEvent(NULL)
,m_tFirstPoint(CCPointZero)
{
    
}

CATouchController::~CATouchController()
{
    m_vTouchesViews.clear();
    m_vTouchMovedsView.clear();
    m_vTouchMovedsViewCache.clear();
    CC_SAFE_RELEASE_NULL(m_pTouch);
    CC_SAFE_RELEASE_NULL(m_pEvent);
}

int CATouchController::getTouchID()
{
    return m_pTouch ? m_pTouch->getID() : -1;
}

std::vector<CAResponder*> CATouchController::getEventListener(CATouch* touch, CAView* view)
{
    CAResponder* responder = view;
    
    std::vector<CAResponder*> vector;
    
    do
    {
        vector.push_back(responder);

        CAResponder* lastResponder = NULL;
        
        if (CAView* view = dynamic_cast<CAView*>(responder))
        {
            if (view->getViewDelegate())
            {
                lastResponder = view->nextResponder();
            }
            else
            {
                
                CAVector<CAView*>::const_reverse_iterator itr;
                for (itr=view->CAView::getSubviews().rbegin();
                     itr!=view->CAView::getSubviews().rend(); itr++)
                {
                    CAView* subview = *itr;
                    if (subview->isVisible() && subview->isTouchEnabled())
                    {
                        CCPoint point = subview->convertTouchToNodeSpace(touch);
                        
                        if (subview->getBounds().containsPoint(point))
                        {
                            lastResponder = subview;
                            break;
                        }
                    }
                }
            }
        }
        else if (CAViewController* viewController = dynamic_cast<CAViewController*>(responder))
        {
            CAVector<CAView*>::const_reverse_iterator itr;
            for (itr=viewController->getView()->CAView::getSubviews().rbegin();
                 itr!=viewController->getView()->CAView::getSubviews().rend();
                 itr++)
            {
                CAView* subview = *itr;
                if (subview->isVisible() && subview->isTouchEnabled())
                {
                    //CC_BREAK_IF(!subview->isTouchEnabled());
                    
                    CCPoint point = subview->convertTouchToNodeSpace(touch);
                    
                    if (subview->getBounds().containsPoint(point))
                    {
                        lastResponder = subview;
                        break;
                    }
                }
            }
        }
        
        responder = lastResponder;
    }
    while (responder);
    
    return vector;
}

void CATouchController::passingTouchesViews(float dt)
{
    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view)
    {
        view->ccTouchBegan(m_pTouch, m_pEvent);
    }
    
    CC_RETURN_IF(m_vTouchesViews.empty());
    
    CAResponder* responder = m_vTouchesViews.front();
    while (responder->nextResponder())
    {
        responder = responder->nextResponder();
        m_vTouchesViews.pushBack(responder);
    }
    
    for (int i=0; i<m_vTouchesViews.size();)
    {
        if (!m_vTouchesViews.at(i)->ccTouchBegan(m_pTouch, m_pEvent))
        {
            m_vTouchesViews.erase(i);
        }
        else
        {
            i++;
        }
    }
}

void CATouchController::touchBegan()
{
    m_tFirstPoint = m_pTouch->getLocation();
    
    std::vector<CAResponder*> vector;
    
    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (isContainsFirstPoint)
    {
        vector = this->getEventListener(m_pTouch, view);
    }
    else
    {
        vector = this->getEventListener(m_pTouch, CAApplication::getApplication()->getRootWindow());
    }
    
    std::vector<CAResponder*>::iterator itr;
    for (itr=vector.begin(); itr!=vector.end(); itr++)
    {
        CC_CONTINUE_IF(!(*itr)->isPriorityScroll());
        CC_CONTINUE_IF(!(*itr)->isScrollEnabled());
        CC_CONTINUE_IF(!(*itr)->isHorizontalScrollEnabled() && !(*itr)->isVerticalScrollEnabled());
        m_vTouchMovedsViewCache.pushBack((*itr));
    }
    m_vTouchesViews.pushBack(vector.back());
    
    if (!m_vTouchMovedsViewCache.empty())
    {
        CAScheduler::schedule(schedule_selector(CATouchController::passingTouchesViews), this, 0, 0, 0.05f);
    }
    else
    {
        this->passingTouchesViews();
    }
}

void CATouchController::touchMoved()
{
    CC_RETURN_IF(ccpDistance(m_tFirstPoint, m_pTouch->getLocation()) < _px(32));
    
    m_tFirstPoint = CCPointZero;

    if (!m_vTouchMovedsViewCache.empty())
    {
        bool isScheduledPassing = CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViews), this);
        
        bool isTouchEventScrollHandOverToSuperview = true;
        
        for (CAVector<CAResponder*>::iterator itr=m_vTouchesViews.begin();
             itr!=m_vTouchesViews.end(); itr++)
        {
            CC_CONTINUE_IF((*itr)->isTouchEventScrollHandOverToSuperview());
            isTouchEventScrollHandOverToSuperview = false;
            break;
        }
        
        
        if (isScheduledPassing || isTouchEventScrollHandOverToSuperview)
        {
            CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViews), this);
            
            while (!m_vTouchMovedsViewCache.empty())
            {
                CAResponder* responder = m_vTouchMovedsViewCache.back();
                CCPoint pointOffSet = CCPointZero;
                if (CAView* v = dynamic_cast<CAView*>(responder))
                {
                    pointOffSet = ccpSub(v->convertToNodeSpace(m_pTouch->getLocation()),
                                         v->convertToNodeSpace(m_pTouch->getPreviousLocation()));
                }
                else if (CAViewController* c = dynamic_cast<CAViewController*>(responder))
                {
                    pointOffSet = ccpSub(c->getView()->convertToNodeSpace(m_pTouch->getLocation()),
                                         c->getView()->convertToNodeSpace(m_pTouch->getPreviousLocation()));
                }
                else
                {
                    pointOffSet = ccpSub(m_pTouch->getLocation(), m_pTouch->getPreviousLocation());
                }
                
                pointOffSet.x = fabsf(pointOffSet.x);
                pointOffSet.y = fabsf(pointOffSet.y);
                
                do
                {
                    CC_BREAK_IF(!responder->isHorizontalScrollEnabled() && pointOffSet.x >= pointOffSet.y);
                    CC_BREAK_IF(!responder->isVerticalScrollEnabled() && pointOffSet.x < pointOffSet.y);
                    m_vTouchMovedsView.pushBack(m_vTouchMovedsViewCache.back());
                }
                while (0);
                
                m_vTouchMovedsViewCache.popBack();
            }
            
            if (!m_vTouchMovedsView.empty())
            {
                bool isTouchCancelled = true;
                CAVector<CAResponder*>::iterator itr;
                for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
                {
                    CAResponder* responder = (*itr);
                    if (responder->isPriorityScroll())
                    {
                        CCPoint pointOffSet = CCPointZero;
                        if (CAView* v = dynamic_cast<CAView*>(responder))
                        {
                            pointOffSet = ccpSub(v->convertToNodeSpace(m_pTouch->getLocation()),
                                                 v->convertToNodeSpace(m_pTouch->getPreviousLocation()));
                        }
                        else if (CAViewController* c = dynamic_cast<CAViewController*>(responder))
                        {
                            pointOffSet = ccpSub(c->getView()->convertToNodeSpace(m_pTouch->getLocation()),
                                                 c->getView()->convertToNodeSpace(m_pTouch->getPreviousLocation()));
                        }
                        else
                        {
                            pointOffSet = ccpSub(m_pTouch->getLocation(), m_pTouch->getPreviousLocation());
                        }
                        
                        if (!responder->isReachBoundaryHandOverToSuperview())
                        {
                            isTouchCancelled = false;
                            break;
                        }
                        
                        if (responder->isHorizontalScrollEnabled()
                            && fabsf(pointOffSet.x) >= fabsf(pointOffSet.y))
                        {
                            if (!responder->isReachBoundaryLeft() && pointOffSet.x >= 0)
                            {
                                isTouchCancelled = false;
                                break;
                            }
                            if (!responder->isReachBoundaryRight() && pointOffSet.x <= 0)
                            {
                                isTouchCancelled = false;
                                break;
                            }
                            
                        }
                        
                        if (responder->isVerticalScrollEnabled()
                            && fabsf(pointOffSet.x) < fabsf(pointOffSet.y))
                        {
                            if (!responder->isReachBoundaryUp() && pointOffSet.y >= 0)
                            {
                                isTouchCancelled = false;
                                break;
                            }
                            if (!responder->isReachBoundaryDown() && pointOffSet.y <= 0)
                            {
                                isTouchCancelled = false;
                                break;
                            }
                        }
                    }
                }
                
                if (isTouchCancelled)
                {
                    if (!isScheduledPassing)
                    {
                        CAVector<CAResponder*>::iterator itr;
                        for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
                        {
                            (*itr)->ccTouchCancelled(m_pTouch, m_pEvent);
                        }
                    }
                    m_vTouchesViews.clear();
                }
                
                if (isScheduledPassing || m_vTouchesViews.empty())
                {
                    for (int i=0; i<m_vTouchMovedsView.size(); i++)
                    {
                        CAResponder* responder = m_vTouchMovedsView.at(i);
                        CCPoint pointOffSet = CCPointZero;
                        if (CAView* v = dynamic_cast<CAView*>(responder))
                        {
                            pointOffSet = ccpSub(v->convertToNodeSpace(m_pTouch->getLocation()),
                                                 v->convertToNodeSpace(m_pTouch->getPreviousLocation()));
                        }
                        else if (CAViewController* c = dynamic_cast<CAViewController*>(responder))
                        {
                            pointOffSet = ccpSub(c->getView()->convertToNodeSpace(m_pTouch->getLocation()),
                                                 c->getView()->convertToNodeSpace(m_pTouch->getPreviousLocation()));
                        }
                        else
                        {
                            pointOffSet = ccpSub(m_pTouch->getLocation(), m_pTouch->getPreviousLocation());
                        }
                        
                        if (responder->isReachBoundaryHandOverToSuperview())
                        {
                            if (responder->isHorizontalScrollEnabled()
                                && fabsf(pointOffSet.x) >= fabsf(pointOffSet.y))
                            {
                                CC_CONTINUE_IF(responder->isReachBoundaryLeft() && pointOffSet.x > 0);
                                CC_CONTINUE_IF(responder->isReachBoundaryRight() && pointOffSet.x < 0);
                            }
                            else if (responder->isVerticalScrollEnabled()
                                     && fabsf(pointOffSet.x) < fabsf(pointOffSet.y))
                            {
                                CC_CONTINUE_IF(responder->isReachBoundaryUp() && pointOffSet.y > 0);
                                CC_CONTINUE_IF(responder->isReachBoundaryDown() && pointOffSet.y < 0);
                            }
                        }

                        if (responder->ccTouchBegan(m_pTouch, m_pEvent))
                        {
                            m_vTouchesViews.pushBack(responder);
                        }
                        
                        break;
                    }
                    
                    if (m_vTouchesViews.empty())
                    {
                        m_vTouchesViews.pushBack(m_vTouchMovedsView.front());
                        
                        while (m_vTouchesViews.back())
                        {
                            if (m_vTouchesViews.back()->ccTouchBegan(m_pTouch, m_pEvent))
                            {
                                break;
                            }
                            m_vTouchesViews.popBack();
                        }
                        
                    }
                }
            }
        }

    }

    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view && view->isScrollEnabled())
    {
        view->ccTouchMoved(m_pTouch, m_pEvent);
    }
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
        CC_CONTINUE_IF(!(*itr)->isScrollEnabled());
        (*itr)->ccTouchMoved(m_pTouch, m_pEvent);
    }
    
}

void CATouchController::touchEnded()
{
    bool isScheduledPassing = CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViews), this);
    if (isScheduledPassing)
    {
        CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViews), this);
        this->passingTouchesViews();
    }
    
    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view)
    {
        view->ccTouchEnded(m_pTouch, m_pEvent);
    }
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
        (*itr)->ccTouchEnded(m_pTouch, m_pEvent);
    }
}

void CATouchController::touchCancelled()
{
    CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViews), this);
    
    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view)
    {
        view->ccTouchCancelled(m_pTouch, m_pEvent);
    }
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
        (*itr)->ccTouchCancelled(m_pTouch, m_pEvent);
    }
}


CATouchDispatcher::CATouchDispatcher(void)
:m_iDispatchEvents(0)
,m_bLocked(false)
,m_pFirstResponder(NULL)
{
    
}

CATouchDispatcher::~CATouchDispatcher(void)
{
    CC_SAFE_RELEASE_NULL(m_pFirstResponder);
}

bool CATouchDispatcher::init(void)
{
    return true;
}

void CATouchDispatcher::setDispatchEvents(bool dispatchEvents)
{
    if (dispatchEvents)
    {
        ++m_iDispatchEvents;
    }
    else
    {
        --m_iDispatchEvents;
    }
}

void CATouchDispatcher::setDispatchEventsTrue()
{
    this->setDispatchEvents(true);
}

void CATouchDispatcher::setDispatchEventsFalse()
{
    this->setDispatchEvents(false);
}

void CATouchDispatcher::touchesBegan(CCSet *touches, CAEvent *pEvent)
{
    CC_RETURN_IF(!isDispatchEvents());
    m_bLocked = true;
    
    CATouch *pTouch;
    CCSetIterator setIter;
    for (setIter = touches->begin(); setIter != touches->end(); setIter++)
    {
        pTouch = (CATouch *)(*setIter);
        
        CATouchController* touchController = new CATouchController();
        touchController->setTouch(pTouch);
        touchController->setEvent(pEvent);
        m_vTouchControllers[pTouch->getID()] = touchController;
        touchController->touchBegan();
    }
    m_bLocked = false;
}

void CATouchDispatcher::touchesMoved(CCSet *touches, CAEvent *pEvent)
{
    m_bLocked = true;
    
    CATouch *pTouch;
    CCSetIterator setIter;
    for (setIter = touches->begin(); setIter != touches->end(); setIter++)
    {
        pTouch = (CATouch *)(*setIter);
        
        CATouchController* touchController = m_vTouchControllers[pTouch->getID()];
        CC_CONTINUE_IF(touchController == NULL);
        touchController->touchMoved();
    }
    m_bLocked = false;
}

void CATouchDispatcher::touchesEnded(CCSet *touches, CAEvent *pEvent)
{
    m_bLocked = true;
    
    CATouch *pTouch;
    CCSetIterator setIter;
    for (setIter = touches->begin(); setIter != touches->end(); setIter++)
    {
        pTouch = (CATouch *)(*setIter);
        
        CATouchController* touchController = m_vTouchControllers[pTouch->getID()];
        CC_CONTINUE_IF(touchController == NULL);
        touchController->touchEnded();
        delete touchController;
        m_vTouchControllers.erase(pTouch->getID());
    }
    m_bLocked = false;
}

void CATouchDispatcher::touchesCancelled(CCSet *touches, CAEvent *pEvent)
{
    m_bLocked = true;
    
    CATouch *pTouch;
    CCSetIterator setIter;
    for (setIter = touches->begin(); setIter != touches->end(); ++setIter)
    {
        pTouch = (CATouch *)(*setIter);
        
        CATouchController* touchController = m_vTouchControllers[pTouch->getID()];
        CC_CONTINUE_IF(touchController == NULL);
        touchController->touchEnded();
        delete touchController;
        m_vTouchControllers.erase(pTouch->getID());
    }
    m_bLocked = false;
}

int CATouchDispatcher::getTouchCount()
{
    return (int)m_vTouchControllers.size();
}

NS_CC_END

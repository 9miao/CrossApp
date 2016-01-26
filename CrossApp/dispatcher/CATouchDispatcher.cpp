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
#include "support/CAPointExtension.h"


NS_CC_BEGIN

CATouchController::CATouchController()
:m_pTouch(NULL)
,m_pEvent(NULL)
,m_tFirstPoint(DPointZero)
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

CAResponder* getLastResponder(CATouch* touch, const CAVector<CAView*>& subviews)
{
    CAResponder* lastResponder = NULL;
    
    for (CAVector<CAView*>::const_reverse_iterator itr=subviews.rbegin();
         itr!=subviews.rend();
         itr++)
    {
        CAView* subview = *itr;
        if (subview->isVisible())
        {
            if (CAViewController* viewController = dynamic_cast<CAViewController*>(subview->getContentContainer()))
            {
                CC_CONTINUE_IF(!viewController->isTouchEnabled());
                
                DPoint point = subview->convertTouchToNodeSpace(touch);
                
                if (subview->getBounds().containsPoint(point))
                {
                    lastResponder = viewController;
                    break;
                }
            }
            else
            {
                CC_CONTINUE_IF(!subview->isTouchEnabled());
                
                DPoint point = subview->convertTouchToNodeSpace(touch);
                
                if (subview->getBounds().containsPoint(point))
                {
                    lastResponder = subview;
                    break;
                }
            }
        }
    }
    
    return lastResponder;
}

std::vector<CAResponder*> CATouchController::getEventListener(CATouch* touch, CAView* view)
{
    CAResponder* responder = view;
    
    std::vector<CAResponder*> vector;
    
    do
    {
        //CCLog("------ %s", typeid(*responder).name());
        vector.push_back(responder);

        if (CAView* view = dynamic_cast<CAView*>(responder))
        {
            responder = getLastResponder(touch, view->CAView::getSubviews());
        }
        else if (CAViewController* viewController = dynamic_cast<CAViewController*>(responder))
        {
            responder = getLastResponder(touch, viewController->getView()->CAView::getSubviews());
        }
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
        this->touchBeganWithResponder(view);
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
        if (!this->touchBeganWithResponder(m_vTouchesViews.at(i)))
        {
            m_vTouchesViews.erase(i);
            m_vTouchMovedsViewCache.eraseObject(m_vTouchesViews.at(i));
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
    CC_RETURN_IF(ccpDistance(m_tFirstPoint, m_pTouch->getLocation()) < 16);
    
    m_tFirstPoint = DPointZero;

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
                DPoint pointOffSet = DPointZero;
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
                        DPoint pointOffSet = DPointZero;
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
                            this->touchCancelledWithResponder(*itr);
                        }
                    }
                    m_vTouchesViews.clear();
                }
                
                if (isScheduledPassing || m_vTouchesViews.empty())
                {
                    for (int i=0; i<m_vTouchMovedsView.size(); i++)
                    {
                        CAResponder* responder = m_vTouchMovedsView.at(i);
                        DPoint pointOffSet = DPointZero;
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

                        if (this->touchBeganWithResponder(responder))
                        {
                            m_vTouchesViews.pushBack(responder);
                        }
                        
                        break;
                    }
                    
                    if (m_vTouchesViews.empty())
                    {
                        m_vTouchesViews.pushBack(m_vTouchMovedsView.front());
                        
                        if (!this->touchBeganWithResponder(m_vTouchesViews.back()))
                        {
                            m_vTouchesViews.clear();
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
        this->touchMovedWithResponder(view);
    }
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
        CC_CONTINUE_IF(!(*itr)->isScrollEnabled());
        this->touchMovedWithResponder(*itr);
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
        this->touchEndedWithResponder(view);
    }
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
        this->touchEndedWithResponder(*itr);
    }
}

void CATouchController::touchCancelled()
{
    CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViews), this);
    
    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view)
    {
        this->touchCancelledWithResponder(view);
    }
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
        this->touchCancelledWithResponder(*itr);
    }
}

bool CATouchController::touchBeganWithResponder(CAResponder* responder)
{
    responder->m_vTouches.pushBack(m_pTouch);
    bool ret = responder->ccTouchBegan(m_pTouch, m_pEvent);
    if (!ret)
    {
        responder->m_vTouches.eraseObject(m_pTouch);
    }
    return ret;
}

void CATouchController::touchMovedWithResponder(CAResponder* responder)
{
    responder->ccTouchMoved(m_pTouch, m_pEvent);
}

void CATouchController::touchEndedWithResponder(CAResponder* responder)
{
    responder->ccTouchEnded(m_pTouch, m_pEvent);
    responder->m_vTouches.eraseObject(m_pTouch);
}

void CATouchController::touchCancelledWithResponder(CAResponder* responder)
{
    responder->ccTouchCancelled(m_pTouch, m_pEvent);
    responder->m_vTouches.eraseObject(m_pTouch);
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
        --m_iDispatchEvents;
        m_iDispatchEvents = MAX(m_iDispatchEvents, 0);
    }
    else
    {
        ++m_iDispatchEvents;
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
    CC_RETURN_IF(!isDispatchEvents() );
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

void CATouchDispatcher::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
    std::vector<CAResponder*> responders;
    for (std::set<CAResponder*>::iterator itr=m_pMouseMoveds.begin();
         itr!=m_pMouseMoveds.end(); itr++)
    {
        if (CAView* view = dynamic_cast<CAView*>(*itr))
        {
            DPoint point = view->convertTouchToNodeSpace(pTouch);
            if (view->getBounds().containsPoint(point))
            {
                responders.push_back(view);
            }
            else
            {
                point = view->convertToNodeSpace(pTouch->getPreviousLocation());
                if (view->getBounds().containsPoint(point))
                {
                    responders.push_back(view);
                }
            }
        }
        else if (CAViewController* viewController = dynamic_cast<CAViewController*>(*itr))
        {
            DPoint point = viewController->getView()->convertTouchToNodeSpace(pTouch);
            if (viewController->getView()->getBounds().containsPoint(point))
            {
                responders.push_back(viewController);
            }
            else
            {
                point = viewController->getView()->convertToNodeSpace(pTouch->getPreviousLocation());
                if (viewController->getView()->getBounds().containsPoint(point))
                {
                    responders.push_back(viewController);
                }
            }
        }
    }
    
    CAResponder* responder = NULL;
    for (std::vector<CAResponder*>::iterator itr=responders.begin();
         itr!=responders.end(); itr++)
    {
        if (responder == NULL)
        {
            responder = *itr;
        }
        else if (responder->getZLevel() < (*itr)->getZLevel())
        {
            responder = *itr;
        }
    }
    responders.clear();
    
    if (CAView* view = dynamic_cast<CAView*>(responder))
    {
        DPoint point = view->convertTouchToNodeSpace(pTouch);
        if (view->getBounds().containsPoint(point))
        {
            view->mouseMoved(pTouch, pEvent);
        }
        else
        {
            point = view->convertToNodeSpace(pTouch->getPreviousLocation());
            if (view->getBounds().containsPoint(point))
            {
                view->mouseMovedOutSide(pTouch, pEvent);
            }
        }
    }
    else if (CAViewController* viewController = dynamic_cast<CAViewController*>(responder))
    {
        DPoint point = viewController->getView()->convertTouchToNodeSpace(pTouch);
        if (viewController->getView()->getBounds().containsPoint(point))
        {
            viewController->mouseMoved(pTouch, pEvent);
        }
        else
        {
            point = viewController->getView()->convertToNodeSpace(pTouch->getPreviousLocation());
            if (viewController->getView()->getBounds().containsPoint(point))
            {
                viewController->mouseMovedOutSide(pTouch, pEvent);
            }
        }
    }
}

void CATouchDispatcher::mouseScrollWheel(CATouch* pTouch, float off_x, float off_y, CAEvent* pEvent)
{
    std::vector<CAResponder*> responders;
    for (std::set<CAResponder*>::iterator itr=m_pMouseScrollWheels.begin();
         itr!=m_pMouseScrollWheels.end(); itr++)
    {
        if (CAView* view = dynamic_cast<CAView*>(*itr))
        {
            DPoint point = view->convertTouchToNodeSpace(pTouch);
            if (view->getBounds().containsPoint(point))
            {
                responders.push_back(view);
            }
            else
            {
                point = view->convertToNodeSpace(pTouch->getPreviousLocation());
                if (view->getBounds().containsPoint(point))
                {
                    responders.push_back(view);
                }
            }
        }
        else if (CAViewController* viewController = dynamic_cast<CAViewController*>(*itr))
        {
            DPoint point = viewController->getView()->convertTouchToNodeSpace(pTouch);
            if (viewController->getView()->getBounds().containsPoint(point))
            {
                responders.push_back(viewController);
            }
            else
            {
                point = viewController->getView()->convertToNodeSpace(pTouch->getPreviousLocation());
                if (viewController->getView()->getBounds().containsPoint(point))
                {
                    responders.push_back(viewController);
                }
            }
        }
    }
    
    CAResponder* responder = NULL;
    for (std::vector<CAResponder*>::iterator itr=responders.begin();
         itr!=responders.end(); itr++)
    {
        if (responder == NULL)
        {
            responder = *itr;
        }
        else if (responder->getZLevel() < (*itr)->getZLevel())
        {
            responder = *itr;
        }
    }
    responders.clear();

    if (CAView* view = dynamic_cast<CAView*>(responder))
    {
        DPoint point = view->convertTouchToNodeSpace(pTouch);
        if (view->getBounds().containsPoint(point))
        {
            view->mouseScrollWheel(pTouch, off_x, off_y, pEvent);
        }
    }
    else if (CAViewController* viewController = dynamic_cast<CAViewController*>(responder))
    {
        DPoint point = viewController->getView()->convertTouchToNodeSpace(pTouch);
        if (viewController->getView()->getBounds().containsPoint(point))
        {
            viewController->mouseScrollWheel(pTouch, off_x, off_y, pEvent);
        }
    }
}

int CATouchDispatcher::getTouchCount()
{
    return (int)m_vTouchControllers.size();
}

void CATouchDispatcher::addMouseMovedResponder(CAResponder* responder)
{
    m_pMouseMoveds.insert(responder);
}

void CATouchDispatcher::removeMouseMovedResponder(CAResponder* responder)
{
    m_pMouseMoveds.erase(responder);
}

void CATouchDispatcher::addMouseScrollWheel(CAResponder* responder)
{
    m_pMouseScrollWheels.insert(responder);
}

void CATouchDispatcher::removeMouseScrollWheel(CAResponder* responder)
{
    m_pMouseScrollWheels.erase(responder);
}

NS_CC_END

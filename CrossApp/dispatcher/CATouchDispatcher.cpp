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
#include "basics/CAPointExtension.h"


NS_CC_BEGIN

CATouchController::CATouchController()
:m_pTouch(nullptr)
,m_pEvent(nullptr)
,m_tFirstPoint(DPointZero)
,m_bBanMultipleTouch(false)
{
    
}

CATouchController::~CATouchController()
{
    m_vTouchesViews.clear();
    m_vTouchMovedsView.clear();
    m_vTouchMovedsViewCache.clear();
    CC_SAFE_RELEASE_NULL(m_pTouch);
    CC_SAFE_RELEASE_NULL(m_pEvent);
    
    if (m_bBanMultipleTouch == true)
    {
        CAApplication::getApplication()->getTouchDispatcher()->m_bBanMultipleTouch = false;
    }
}

int CATouchController::getTouchID()
{
    return m_pTouch ? m_pTouch->getID() : -1;
}

CAResponder* getLastResponder(CATouch* touch, const CAVector<CAView*>& subviews)
{
    CAResponder* lastResponder = NULL;
    
    for (auto itr=subviews.rbegin(); itr!=subviews.rend(); itr++)
    {
        CAView* subview = *itr;
        if (subview->isVisible())
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
    
    return lastResponder;
}

std::vector<CAResponder*> CATouchController::getEventListener(CATouch* touch, CAView* view)
{
    CAResponder* responder = view;
    
    std::vector<CAResponder*> vector;
    
    while (responder)
    {
        CCLOG("------ %s", typeid(*responder).name());
        vector.push_back(responder);

        if (CAView* view = dynamic_cast<CAView*>(responder))
        {
            responder = getLastResponder(touch, view->CAView::getSubviews());
        }
    };
    
    return vector;
}

void CATouchController::passingTouchesViews(float dt)
{
    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view && view->isTouchEnabled() && view->isVisible())
    {
        this->touchBeganWithResponder(view);
        if (view->isMultipleTouchEnabled() == false)
        {
            m_bBanMultipleTouch = true;
            CAApplication::getApplication()->getTouchDispatcher()->m_bBanMultipleTouch = true;
        }
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
            if (m_vTouchesViews.at(i)->isMultipleTouchEnabled() == false)
            {
                m_bBanMultipleTouch = true;
                CAApplication::getApplication()->getTouchDispatcher()->m_bBanMultipleTouch = true;
            }
            i++;
        }
    }
}

void CATouchController::touchBegan()
{
    m_tFirstPoint = m_pTouch->getLocation();
    CAView* responder = (CAView*)CAApplication::getApplication()->getTouchDispatcher()->getScrollRunningResponder();
    
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
    
    if (responder && responder->isRunning() && responder->isVisible()
        && responder->getBounds().containsPoint(responder->convertTouchToNodeSpace(m_pTouch)))
    {
        m_vTouchesViews.pushBack(responder);
        CAApplication::getApplication()->getTouchDispatcher()->removeScrollRunningResponder(responder);
        this->passingTouchesViews();
        
        for (auto& res : vector)
        {
            CC_CONTINUE_IF(!res->isPriorityScroll());
            CC_CONTINUE_IF(!res->isScrollEnabled());
            CC_CONTINUE_IF(!res->isHorizontalScrollEnabled() && !res->isVerticalScrollEnabled());
            m_vTouchMovedsViewCache.pushBack(res);
        }
    }
    else if (!vector.empty())
    {
        for (auto& res : vector)
        {
            CC_CONTINUE_IF(!res->isPriorityScroll());
            CC_CONTINUE_IF(!res->isScrollEnabled());
            CC_CONTINUE_IF(!res->isHorizontalScrollEnabled() && !res->isVerticalScrollEnabled());
            m_vTouchMovedsViewCache.pushBack(res);
        }
        m_vTouchesViews.pushBack(vector.back());
        
        if (!m_vTouchMovedsViewCache.empty())
        {
            CAScheduler::getScheduler()->schedule(schedule_selector(CATouchController::passingTouchesViews), this, 0, 0, 0.05f);
        }
        else
        {
            this->passingTouchesViews();
        }
    }
}

void CATouchController::touchMoved()
{
    CC_RETURN_IF(ccpDistance(m_tFirstPoint, m_pTouch->getLocation()) < 20);
    
    m_tFirstPoint = DPointZero;

    if (!m_vTouchMovedsViewCache.empty())
    {// !m_vTouchMovedsViewCache.empty()  ---end
        
        bool isScheduledPassing = CAScheduler::getScheduler()->isScheduled(schedule_selector(CATouchController::passingTouchesViews), this);
        
        if (isScheduledPassing)
        {
            m_vTouchesViews.clear();
        }
        
        bool isTouchEventScrollHandOverToSuperview = true;
        for (auto& responder : m_vTouchesViews)
        {
            CC_CONTINUE_IF(responder->isTouchEventScrollHandOverToSuperview());
            isTouchEventScrollHandOverToSuperview = false;
            break;
        }

        if (isScheduledPassing || isTouchEventScrollHandOverToSuperview)
        {// isScheduledPassing || isTouchEventScrollHandOverToSuperview  ---begin
            
            CAScheduler::getScheduler()->unschedule(schedule_selector(CATouchController::passingTouchesViews), this);
            
            while (!m_vTouchMovedsViewCache.empty())
            {
                CAResponder* responder = m_vTouchMovedsViewCache.back();
                DPoint pointOffSet = DPointZero;
                if (CAView* v = dynamic_cast<CAView*>(responder))
                {
                    pointOffSet = ccpSub(v->convertToNodeSpace(m_pTouch->getLocation()),
                                         v->convertToNodeSpace(m_pTouch->getPreviousLocation()));
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
            
            if (isScheduledPassing)
            {
                for (auto itr=m_vTouchMovedsView.begin(); itr<m_vTouchMovedsView.end();)
                {
                    CAResponder* responder = *itr;
                    DPoint pointOffSet = DPointZero;
                    if (CAView* v = dynamic_cast<CAView*>(responder))
                    {
                        pointOffSet = ccpSub(v->convertToNodeSpace(m_pTouch->getLocation()),
                                             v->convertToNodeSpace(m_pTouch->getPreviousLocation()));
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
                            if (responder->isReachBoundaryLeft() && pointOffSet.x > 0)
                            {
                                ++itr;
                                continue;
                            }
                            if (responder->isReachBoundaryRight() && pointOffSet.x < 0)
                            {
                                ++itr;
                                continue;
                            }
                        }
                        else if (responder->isVerticalScrollEnabled()
                                 && fabsf(pointOffSet.x) < fabsf(pointOffSet.y))
                        {
                            if (responder->isReachBoundaryUp() && pointOffSet.y > 0)
                            {
                                ++itr;
                                continue;
                            }
                            if (responder->isReachBoundaryDown() && pointOffSet.y < 0)
                            {
                                ++itr;
                                continue;
                            }
                        }
                    }
                    
                    if (this->touchBeganWithResponder(responder))
                    {
                        m_vTouchesViews.pushBack(responder);
                        break;
                    }
                    else
                    {
                        itr = m_vTouchMovedsView.erase(itr);
                    }
                }
                
                
            }
            else
            {// isTouchEventScrollHandOverToSuperview == true ---begin
                if (!m_vTouchMovedsView.empty())
                {// m_vTouchMovedsView.empty() ---begin
                    bool isTouchCancelled = true;
                    for (auto& responder : m_vTouchesViews)
                    {
                        if (responder->isPriorityScroll())
                        {
                            DPoint pointOffSet = DPointZero;
                            if (CAView* v = dynamic_cast<CAView*>(responder))
                            {
                                pointOffSet = ccpSub(v->convertToNodeSpace(m_pTouch->getLocation()),
                                                     v->convertToNodeSpace(m_pTouch->getPreviousLocation()));
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
                    }// m_vTouchMovedsView.empty() ---end
                    
                    if (isTouchCancelled)
                    {
                        for (auto& var : m_vTouchesViews)
                        {
                            this->touchCancelledWithResponder(var);
                        }
                        m_vTouchesViews.clear();
                    }
                }
            }// isTouchEventScrollHandOverToSuperview == true ---end

            if (m_vTouchesViews.empty() && !m_vTouchMovedsView.empty())
            {
                if (this->touchBeganWithResponder(m_vTouchMovedsView.front()))
                {
                    m_vTouchesViews.pushBack(m_vTouchMovedsView.front());
                }
            }
        }// isScheduledPassing || isTouchEventScrollHandOverToSuperview  ---end
        
    }// !m_vTouchMovedsViewCache.empty()  ---end

    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view && view->isScrollEnabled())
    {
        this->touchMovedWithResponder(view);
    }
    
    for (auto& responder : m_vTouchesViews)
    {
        CC_CONTINUE_IF(!responder->isScrollEnabled());
        this->touchMovedWithResponder(responder);
    }
    
}

void CATouchController::touchEnded()
{
    bool isScheduledPassing = CAScheduler::getScheduler()->isScheduled(schedule_selector(CATouchController::passingTouchesViews), this);
    if (isScheduledPassing)
    {
        CAScheduler::getScheduler()->unschedule(schedule_selector(CATouchController::passingTouchesViews), this);
        this->passingTouchesViews();
    }
    
    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view)
    {
        this->touchEndedWithResponder(view);
    }
    
    for (auto& responder : m_vTouchesViews)
    {
        this->touchEndedWithResponder(responder);
    }
}

void CATouchController::touchCancelled()
{
    CAScheduler::getScheduler()->unschedule(schedule_selector(CATouchController::passingTouchesViews), this);
    
    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view)
    {
        this->touchCancelledWithResponder(view);
    }
    
    for (auto& responder : m_vTouchesViews)
    {
        this->touchCancelledWithResponder(responder);
    }
}

bool CATouchController::touchBeganWithResponder(CAResponder* responder)
{
    responder->m_vTouches.pushBack(m_pTouch);
    bool ret = false;
    if (responder->m_obTouchBegan)
    {
        ret = responder->m_obTouchBegan(m_pTouch, m_pEvent);
    }
    else
    {
        ret = responder->ccTouchBegan(m_pTouch, m_pEvent);
    }

    if (!ret)
    {
        responder->m_vTouches.eraseObject(m_pTouch);
    }
    return ret;
}

void CATouchController::touchMovedWithResponder(CAResponder* responder)
{
    if (responder->m_obTouchMoved)
    {
        responder->m_obTouchMoved(m_pTouch, m_pEvent);
    }
    else
    {
        responder->ccTouchMoved(m_pTouch, m_pEvent);
    }
}

void CATouchController::touchEndedWithResponder(CAResponder* responder)
{
    if (responder->m_obTouchEnded)
    {
        responder->m_obTouchEnded(m_pTouch, m_pEvent);
    }
    else
    {
        responder->ccTouchEnded(m_pTouch, m_pEvent);
    }
    responder->m_vTouches.eraseObject(m_pTouch);
}

void CATouchController::touchCancelledWithResponder(CAResponder* responder)
{
    if (responder->m_obTouchCancelled)
    {
        responder->m_obTouchCancelled(m_pTouch, m_pEvent);
    }
    else
    {
        responder->ccTouchCancelled(m_pTouch, m_pEvent);
    }
    responder->m_vTouches.eraseObject(m_pTouch);
}




CATouchDispatcher::CATouchDispatcher(void)
:m_iDispatchEvents(0)
,m_bLocked(false)
,m_bBanMultipleTouch(false)
,m_pFirstResponder(nullptr)
,m_pScrollRunningResponder(nullptr)
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
    CC_RETURN_IF(m_bBanMultipleTouch == true);
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
    for (auto& responder : m_pMouseMoveds)
    {
        if (CAView* view = dynamic_cast<CAView*>(responder))
        {
            CC_CONTINUE_IF(view->isVisible() == false);
            DPoint point = view->convertTouchToNodeSpace(pTouch);
            if (view->getBounds().containsPoint(point))
            {
                view->mouseMoved(pTouch, pEvent);
            }
            else
            {
                view->mouseMovedOutSide(pTouch, pEvent);
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

void CATouchDispatcher::setScrollRunningResponder(CAResponder* var)
{
    if (m_pScrollRunningResponder == NULL)
    {
        m_pScrollRunningResponder = var;
    }
}

void CATouchDispatcher::removeScrollRunningResponder(CAResponder* var)
{
    if (m_pScrollRunningResponder == var)
    {
        m_pScrollRunningResponder = NULL;
    }
}

CAEvent* CATouchDispatcher::eventWithTouch(CATouch* touch)
{
    CAEvent* event = NULL;
    if (touch)
    {
        int touchID = touch->getID();
        if (m_vTouchControllers.count(touchID) > 0)
        {
            event = m_vTouchControllers.at(touchID)->getEvent();
        }
    }
    return event;
}

CAEvent* CATouchDispatcher::eventWithTouchID(int touchID)
{
    CAEvent* event = NULL;
    if (m_vTouchControllers.count(touchID) > 0)
    {
        event = m_vTouchControllers.at(touchID)->getEvent();
    }
    return event;
}

NS_CC_END

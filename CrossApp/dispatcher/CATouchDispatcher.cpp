    //
//  CATouchDispatcher.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-23.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CATouchDispatcher.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCSet.h"
#include "CATouch.h"
#include "images/CAImage.h"
#include "support/data_support/ccCArray.h"
#include "ccMacros.h"
#include <algorithm>
#include "basics/CAApplication.h"
#include "view/CAWindow.h"
#include "control/CAControl.h"
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
        
        CAResponder* nextResponder = NULL;
        
        if (CAView* view = dynamic_cast<CAView*>(responder))
        {
            if (view->getViewDelegate())
            {
                nextResponder = view->nextResponder();
            }
            else
            {
                view->sortAllSubviews();
                
                CAObject* obj;
                CCARRAY_FOREACH_REVERSE(view->getSubviews(), obj)
                {
                    CAView* subview = dynamic_cast<CAView*>(obj);
                    if (subview && subview->isVisible() && subview->isTouchEnabled())
                    {
                        CCPoint point = subview->convertTouchToNodeSpace(touch);
                        
                        if (subview->getBounds().containsPoint(point))
                        {
                            nextResponder = subview;
                            break;
                        }
                    }
                }
                
            }
        }
        else if (CAViewController* viewController = dynamic_cast<CAViewController*>(responder))
        {
            CAObject* obj;
            CCARRAY_FOREACH_REVERSE(viewController->getView()->getSubviews(), obj)
            {
                CAView* subview = dynamic_cast<CAView*>(obj);
                if (subview && subview->isVisible() && subview->isTouchEnabled())
                {
                    CCPoint point = subview->convertTouchToNodeSpace(touch);
                    
                    if (subview->getBounds().containsPoint(point))
                    {
                        nextResponder = subview;
                        break;
                    }
                }
            }
        }
        
        responder = nextResponder;
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
        m_vTouchesViews.pushBack(responder->nextResponder());
        responder = responder->nextResponder();
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
        if ((*itr)->isTouchMovedStopSubviews())
        {
            m_vTouchMovedsViewCache.pushBack((*itr));
        }
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
                CC_BREAK_IF(!responder->isTouchMovedListenHorizontal() && pointOffSet.x >= pointOffSet.y);
                CC_BREAK_IF(!responder->isTouchMovedListenVertical() && pointOffSet.x < pointOffSet.y);
                m_vTouchMovedsView.pushBack(m_vTouchMovedsViewCache.back());
            }
            while (0);
            
            m_vTouchMovedsViewCache.popBack();
        }
        
        
        CAVector<CAResponder * > tTouchesViews = m_vTouchesViews;
        if (!m_vTouchMovedsView.empty())
        {
            if (!isScheduledPassing)
            {
                
                CAVector<CAResponder*>::iterator itr;
                //
                for (itr = m_vTouchMovedsView.begin(); itr != m_vTouchMovedsView.end(); itr++)
                {
                    m_vTouchesViews.eraseObject(*itr, true);
                }
                //
                
                for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
                {
                    (*itr)->ccTouchCancelled(m_pTouch, m_pEvent);
                }
            }
            
            {
                m_vTouchesViews.clear();
                
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
                    
                    if (responder->isTouchMovedListenHorizontal()
                        && fabsf(pointOffSet.x) >= fabsf(pointOffSet.y))
                    {
                        CC_CONTINUE_IF(responder->isSlidingMinX() && pointOffSet.x > 0);
                        CC_CONTINUE_IF(responder->isSlidingMaxX() && pointOffSet.x < 0);
                    }
                    
                    if (responder->isTouchMovedListenVertical()
                        && fabsf(pointOffSet.x) < fabsf(pointOffSet.y))
                    {
                        CC_CONTINUE_IF(responder->isSlidingMinY() && pointOffSet.y > 0);
                        CC_CONTINUE_IF(responder->isSlidingMaxY() && pointOffSet.y < 0);
                    }
                    
                    m_vTouchesViews.pushBack(responder);
                    
                    
                    if (tTouchesViews.contains(responder)==false)
                    {
                        responder->ccTouchBegan(m_pTouch, m_pEvent);
                    }
                    break;
                }
                
                if (m_vTouchesViews.empty())
                {
                    m_vTouchesViews.pushBack(m_vTouchMovedsView.front());
                    if (tTouchesViews.contains(m_vTouchesViews.back())==false)
                    {
                        m_vTouchesViews.back()->ccTouchBegan(m_pTouch, m_pEvent);//
                    }
                    
                }
            }
        }
        
    }
    
    CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
    bool isContainsFirstPoint = view && view->convertRectToWorldSpace(view->getBounds()).containsPoint(m_tFirstPoint);
    if (!isContainsFirstPoint && view)
    {
        view->ccTouchMoved(m_pTouch, m_pEvent);
    }
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
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
:m_bDispatchEvents(true)
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
    m_bDispatchEvents = dispatchEvents;
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
    CC_RETURN_IF(!m_bDispatchEvents);
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

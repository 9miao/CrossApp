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
    m_vTouchMovedsViewCache.clear();
    CC_SAFE_RELEASE_NULL(m_pTouch);
    CC_SAFE_RELEASE_NULL(m_pEvent);
}

int CATouchController::getTouchID()
{
    return m_pTouch ? m_pTouch->getID() : -1;
}

std::vector<CAView*> CATouchController::getEventListener(CATouch* touch, CAView* view)
{
    std::vector<CAView*> vector;
    
    do
    {
        vector.push_back(view);

        view->sortAllSubviews();
        
        CAView* nextView = NULL;
        
        CAObject* obj;
        
        CCARRAY_FOREACH_REVERSE(view->getSubviews(), obj)
        {
            CAView* subview = dynamic_cast<CAView*>(obj);
            if (subview && subview->isVisible() && subview->isTouchEnabled())
            {
                CCPoint point = subview->convertTouchToNodeSpace(touch);
                
                if (subview->getBounds().containsPoint(point))
                {
                    nextView = subview;
                    break;
                }
            }
        }
        view = nextView;
    }
    while (view);
    
    return vector;
}

void CATouchController::passingTouchesViews(float dt)
{
    CC_RETURN_IF(m_vTouchesViews.empty());
    m_vTouchMovedsViewCache.clear();
    
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
    
    std::vector<CAView*> vector;
    
    if (CAView* view = dynamic_cast<CAView*>(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder()))
    {
        vector = this->getEventListener(m_pTouch, view);
    }
    else
    {
        vector = this->getEventListener(m_pTouch, CAApplication::getApplication()->getRootWindow());
    }

    std::vector<CAView*>::iterator itr;
    for (itr=vector.begin(); itr!=vector.end(); itr++)
    {
        if ((*itr)->isTouchMovedStopSubviews())
        {
            m_vTouchMovedsViewCache.insert(0, (*itr));
        }
    }
    m_vTouchesViews.pushBack(vector.back());
    
    if (m_vTouchMovedsViewCache.size() > 0)
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
    CC_RETURN_IF(ccpDistance(m_tFirstPoint, m_pTouch->getLocation()) < _px(8));
    
    bool isScheduledPassing = CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViews), this);
    if (isScheduledPassing && !CCPointZero.equals(m_tFirstPoint))
    {
        CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViews), this);

        for (int i=0; i<m_vTouchMovedsViewCache.size(); i++)
        {
            CAResponder* view = m_vTouchMovedsViewCache.at(i);
            
            if (    !view->isTouchMovedListenHorizontal()
                ||  !view->isTouchMovedListenVertical())
            {
                CCPoint pointOffSet = ccpSub(m_pTouch->getLocation(),
                                             m_pTouch->getPreviousLocation());
                pointOffSet.x = fabsf(pointOffSet.x);
                pointOffSet.y = fabsf(pointOffSet.y);
                
                CC_CONTINUE_IF(view->isTouchMovedListenVertical()
                               && pointOffSet.x >= pointOffSet.y);
                CC_CONTINUE_IF(view->isTouchMovedListenHorizontal()
                               && pointOffSet.x < pointOffSet.y);
            }
            m_vTouchesViews.clear();
            m_vTouchesViews.pushBack(view);
            this->passingTouchesViews();
            break;
        }
    }
    
    if (!m_vTouchMovedsViewCache.empty())
    {
        this->passingTouchesViews();
    }
    
    m_tFirstPoint = CCPointZero;
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
        (*itr)->ccTouchMoved(m_pTouch, m_pEvent);
    }
}

void CATouchController::touchEnded()
{
    bool isScheduledPassing = CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViews), this);
    if (!m_vTouchMovedsViewCache.empty() && isScheduledPassing)
    {
        CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViews), this);
        m_vTouchMovedsViewCache.clear();
        this->passingTouchesViews();
    }
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
        (*itr)->ccTouchEnded(m_pTouch, m_pEvent);
    }
}

void CATouchController::touchCancelled()
{
    bool isScheduledPassing = CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViews), this);
    if (!m_vTouchMovedsViewCache.empty() && isScheduledPassing)
    {
        CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViews), this);
        return;
    }
    
    CAVector<CAResponder*>::iterator itr;
    for (itr=m_vTouchesViews.begin(); itr!=m_vTouchesViews.end(); itr++)
    {
        (*itr)->ccTouchMoved(m_pTouch, m_pEvent);
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
    if (!m_bDispatchEvents)
    {
        std::map<int, CATouchController*>::iterator itr;
        for (itr=m_vTouchControllers.begin();
             itr!=m_vTouchControllers.end();
             itr++)
        {
            CATouchController* touchController = itr->second;
            CC_CONTINUE_IF(!touchController);
            touchController->touchCancelled();
            CC_SAFE_RELEASE(touchController);
            itr->second = NULL;
        }
    }
}

void CATouchDispatcher::setDispatchEventsTrue()
{
    m_bDispatchEvents = true;
}

void CATouchDispatcher::setDispatchEventsFalse()
{
    m_bDispatchEvents = false;
    std::map<int, CATouchController*>::iterator itr;
    for (itr=m_vTouchControllers.begin();
         itr!=m_vTouchControllers.end();
         itr++)
    {
        CATouchController* touchController = itr->second;
        CC_CONTINUE_IF(!touchController);
        touchController->touchCancelled();
        CC_SAFE_RELEASE(touchController);
        itr->second = NULL;
    }
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
    CC_RETURN_IF(!m_bDispatchEvents);
    m_bLocked = true;
    
    CATouch *pTouch;
    CCSetIterator setIter;
    for (setIter = touches->begin(); setIter != touches->end(); setIter++)
    {
        pTouch = (CATouch *)(*setIter);
        
        CATouchController* touchController = m_vTouchControllers[pTouch->getID()];
        CC_CONTINUE_IF(!touchController);
        touchController->touchMoved();
    }
    m_bLocked = false;
}

void CATouchDispatcher::touchesEnded(CCSet *touches, CAEvent *pEvent)
{
    CC_RETURN_IF(!m_bDispatchEvents);
    m_bLocked = true;
    
    CATouch *pTouch;
    CCSetIterator setIter;
    for (setIter = touches->begin(); setIter != touches->end(); setIter++)
    {
        pTouch = (CATouch *)(*setIter);
        
        CATouchController* touchController = m_vTouchControllers[pTouch->getID()];
        CC_CONTINUE_IF(!touchController);
        touchController->touchEnded();
        CC_SAFE_RELEASE(touchController);
        m_vTouchControllers[pTouch->getID()] = NULL;
    }
    m_bLocked = false;
}

void CATouchDispatcher::touchesCancelled(CCSet *touches, CAEvent *pEvent)
{
    CC_RETURN_IF(!m_bDispatchEvents);
    m_bLocked = true;
    
    CATouch *pTouch;
    CCSetIterator setIter;
    for (setIter = touches->begin(); setIter != touches->end(); ++setIter)
    {
        pTouch = (CATouch *)(*setIter);
        
        CATouchController* touchController = m_vTouchControllers[pTouch->getID()];
        CC_CONTINUE_IF(!touchController);
        touchController->touchEnded();
        delete touchController;
        m_vTouchControllers[pTouch->getID()] = NULL;
    }
    m_bLocked = false;
}



NS_CC_END

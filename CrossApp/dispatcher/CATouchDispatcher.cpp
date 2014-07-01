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


#define TOUCHES_BEGIN_VIEW  for (int i=0; i<m_vTouchesViewCache.size();)                        \
                            {                                                                   \
                                CAView* view = m_vTouchesViewCache.at(i);                       \
                                if (view->ccTouchBegan(m_pTouch, m_pEvent) == true)             \
                                {                                                               \
                                    i++;                                                        \
                                }                                                               \
                                else                                                            \
                                {                                                               \
                                    m_vTouchesViewCache.erase(i); \
                                }                                                               \
                            }

#define TOUCHES_MOVED_VIEW  for (std::vector<CAView*>::const_iterator itr=m_vTouchesViewCache.begin();    \
                                itr!=m_vTouchesViewCache.end();                                     \
                                itr++)                                                              \
                            {                                                                       \
                                (*itr)->ccTouchMoved(m_pTouch, m_pEvent);                           \
                            }                                                                       \

#define TOUCHES_ENDED_VIEW  for (std::vector<CAView*>::const_iterator itr=m_vTouchesViewCache.begin();    \
                                itr!=m_vTouchesViewCache.end();                                     \
                                itr++)                                                              \
                            {                                                                       \
                                (*itr)->ccTouchEnded(m_pTouch, m_pEvent);                           \
                            }

#define TOUCHES_CANCELLED_VIEW  for (std::vector<CAView*>::const_iterator itr=m_vTouchesViewCache.begin();    \
                                    itr!=m_vTouchesViewCache.end();                                     \
                                    itr++)                                                              \
                                {                                                                       \
                                    (*itr)->ccTouchCancelled(m_pTouch, m_pEvent);                            \
                                }

NS_CC_BEGIN

CATouchController::CATouchController()
:m_pTouch(NULL)
,m_pEvent(NULL)
,m_tFirstPoint(CCPointZero)
{

}

CATouchController::~CATouchController()
{
    m_vTouchesViewCache.clear();
    m_vWillTouchesViewCache.clear();
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
            if (subview && subview->isVisible())
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

void CATouchController::passingTouchesViewCache(float dt)
{
    bool isControl = false;
    
    for (int i=m_vWillTouchesViewCache.size()-1; i>=0; i--)
    {
        CAView* view = m_vWillTouchesViewCache.at(i);
        if (view->isControl() || view->isSlideContainers())
        {
            CAControl* control = dynamic_cast<CAControl*>(view);
            CAScrollView* scroll = dynamic_cast<CAScrollView*>(view);
            if ((control && control->isTouchEnabled())
                ||
                (scroll && scroll->isScrollEnabled()))
            {
                m_vTouchesViewCache.clear();
                m_vTouchesViewCache.insert(0,
                                           m_vWillTouchesViewCache.begin()+i,
                                           m_vWillTouchesViewCache.end());
                isControl = true;
            }
            break;
        }
    }
    
    if (isControl == false)
    {
        std::vector<CAView*>::const_iterator itr;
        for (itr=m_vWillTouchesViewCache.begin();
             itr!=m_vWillTouchesViewCache.end();
             itr++)
        {
            m_vTouchesViewCache.push_back(*itr);
        }
    }
    m_vWillTouchesViewCache.clear();
    
    TOUCHES_BEGIN_VIEW;
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
        if ((*itr)->isSlideContainers())
        {
            m_vTouchesViewCache.push_back(*itr);
            if (itr+1 != vector.end())
            {
                m_vWillTouchesViewCache.insert(0, itr+1, vector.end());
            }
            break;
        }
        else if ((*itr)->isControl())
        {
            CAControl* control = dynamic_cast<CAControl*>(*itr);
            if (control && control->isTouchEnabled())
            {
                m_vTouchesViewCache.clear();
                m_vTouchesViewCache.insert(0, itr, vector.end());
            }
            break;
        }
        else
        {
            m_vTouchesViewCache.push_back(*itr);
        }
    }

    
    if (m_vWillTouchesViewCache.size() > 0)
    {
        CAScheduler::schedule(schedule_selector(CATouchController::passingTouchesViewCache), this, 0, 0, 0.066f);
    }
    else
    {

        TOUCHES_BEGIN_VIEW;
    }
}

void CATouchController::touchMoved()
{
    CC_RETURN_IF(ccpDistance(m_tFirstPoint, m_pTouch->getLocation()) < 8.0f);
    
    bool isScheduledPassing = CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViewCache), this);
    if (   !m_vWillTouchesViewCache.empty()
        && isScheduledPassing
        && !CCPointZero.equals(m_tFirstPoint))
    {
        CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViewCache), this);
        
        CAView* view = m_vTouchesViewCache.back();
        if (view->getTouchSidingDirection() != CATouchSidingDirectionMultiple)
        {
            CCPoint pointOffSet = ccpSub(m_pTouch->getLocation(),
                                         m_pTouch->getPreviousLocation());
            pointOffSet.x = fabsf(pointOffSet.x);
            pointOffSet.y = fabsf(pointOffSet.y);
            
            if (pointOffSet.x >= pointOffSet.y)
            {
                if (view->getTouchSidingDirection() == CATouchSidingDirectionHorizontal)
                {
                    m_vWillTouchesViewCache.clear();
                    TOUCHES_BEGIN_VIEW;
                }
                else if (view->getTouchSidingDirection() == CATouchSidingDirectionVertical)
                {
                    this->passingTouchesViewCache();
                }
            }
            else
            {
                if (view->getTouchSidingDirection() == CATouchSidingDirectionHorizontal)
                {
                    this->passingTouchesViewCache();
                }
                else if (view->getTouchSidingDirection() == CATouchSidingDirectionVertical)
                {
                    m_vWillTouchesViewCache.clear();
                    TOUCHES_BEGIN_VIEW;
                }
            }

        }

        m_vWillTouchesViewCache.clear();
        TOUCHES_BEGIN_VIEW;
    }
    
    m_tFirstPoint = CCPointZero;
    
    TOUCHES_MOVED_VIEW;
}

void CATouchController::touchEnded()
{
    if (!m_vWillTouchesViewCache.empty()
        && CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViewCache), this))
    {
        CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViewCache), this);
        this->passingTouchesViewCache();
    }
    
    TOUCHES_ENDED_VIEW;
}

void CATouchController::touchCancelled()
{
    if (!m_vWillTouchesViewCache.empty()
        && CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViewCache), this))
    {
        CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViewCache), this);
        this->passingTouchesViewCache();
    }
    
    TOUCHES_CANCELLED_VIEW;
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

//
//  CATouchDispatcher.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-23.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#include "CATouchDispatcher.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCSet.h"
#include "CCTouch.h"
#include "images/CAImage.h"
#include "support/data_support/ccCArray.h"
#include "ccMacros.h"
#include <algorithm>
#include "CCDirector.h"
#include "sprite_nodes/CAWindow.h"
#include "ui_controller/CAControl.h"
#include "CAScheduler.h"
#include "support/CCPointExtension.h"

#define TOUCHES_VIEW_SEL(CCTouch_SEL)   for (std::vector<CAView*>::iterator itr=m_vTouchesViewCache.begin();   \
                                            itr!=m_vTouchesViewCache.end();                                 \
                                            itr++)                                                          \
                                            {                                                               \
                                                (*itr)->CCTouch_SEL(m_pTouch, m_pEvent);                    \
                                            }                                                               \

#define CLEAR_VECTOR(VECTOR)    for (std::vector<CAView*>::iterator itr=VECTOR.begin(); itr!=VECTOR.end(); itr++)   \
                                {                                                                                   \
                                    CC_SAFE_RELEASE(*itr);                                                              \
                                }                                                                                   \
                                VECTOR.clear();                                                                     \

#define PUSH_BACK_IN_VECTOR(VECTOR, VIEW)   ((CAView*)VIEW)->retain();         \
                                            VECTOR.push_back(((CAView*)VIEW));

#define RETAIN_VECTOR(VECTOR)   for (std::vector<CAView*>::iterator itr=VECTOR.begin(); itr!=VECTOR.end(); itr++)   \
                                {                                                                                   \
                                    CC_SAFE_RETAIN(*itr);                                                           \
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
    CLEAR_VECTOR(m_vTouchesViewCache);
    CLEAR_VECTOR(m_vWillTouchesViewCache);
    CC_SAFE_RELEASE_NULL(m_pTouch);
    CC_SAFE_RELEASE_NULL(m_pEvent);
}

int CATouchController::getTouchID()
{
    return m_pTouch ? m_pTouch->getID() : -1;
}

std::vector<CAView*> CATouchController::getEventListener(CCTouch* touch)
{
    std::vector<CAView*> vector;
    
    CAView* view = dynamic_cast<CAView*>(CCDirector::sharedDirector()->getRootWindow());
    
    do
    {
        PUSH_BACK_IN_VECTOR(vector, view);

        view->sortAllSubviews();
        
        CAView* nextView = NULL;
        
        CCObject* obj;
        
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
    
    for (std::vector<CAView*>::iterator itr=m_vWillTouchesViewCache.end()-1;
         itr!=m_vWillTouchesViewCache.begin()-1;
         itr--)
    {
        if ((*itr)->isControl() || (*itr)->isSlideContainers())
        {
            CAControl* control = dynamic_cast<CAControl*>(*itr);
            CAScrollView* scroll = dynamic_cast<CAScrollView*>(*itr);
            if ((control && control->isTouchEnabled())
                    ||
                (scroll && scroll->isScrollEnabled()))
            {
                CLEAR_VECTOR(m_vTouchesViewCache);
                m_vTouchesViewCache.insert(m_vTouchesViewCache.begin(), itr, m_vWillTouchesViewCache.end());
                RETAIN_VECTOR(m_vTouchesViewCache);
                isControl = true;
            }
            break;
        }
    }
    
    if (isControl == false)
    {
        for (std::vector<CAView*>::iterator itr=m_vWillTouchesViewCache.begin();
             itr!=m_vWillTouchesViewCache.end();
             itr++)
        {
            PUSH_BACK_IN_VECTOR(m_vTouchesViewCache, *itr);
        }
    }
    CLEAR_VECTOR(m_vWillTouchesViewCache);
    
    TOUCHES_VIEW_SEL(ccTouchBegan);
}

void CATouchController::touchBegan()
{
    m_tFirstPoint = m_pTouch->getLocation();
    
    std::vector<CAView*> vector;
    
    if (CAView* view = dynamic_cast<CAView*>(CCDirector::sharedDirector()->getTouchDispatcher()->getFirstResponder()))
    {
        PUSH_BACK_IN_VECTOR(vector, view);
    }
    else
    {
        vector = this->getEventListener(m_pTouch);
    }

    std::vector<CAView*>::iterator itr;
    for (itr=vector.begin(); itr!=vector.end(); itr++)
    {
        if ((*itr)->isSlideContainers())
        {
            PUSH_BACK_IN_VECTOR(m_vTouchesViewCache, *itr);
            if (itr+1 != vector.end())
            {
                m_vWillTouchesViewCache.insert(m_vWillTouchesViewCache.begin(), itr+1, vector.end());
                RETAIN_VECTOR(m_vWillTouchesViewCache);
            }
            break;
        }
        else if ((*itr)->isControl())
        {
            CAControl* control = dynamic_cast<CAControl*>(*itr);
            if (control && control->isTouchEnabled())
            {
                CLEAR_VECTOR(m_vTouchesViewCache);
                m_vTouchesViewCache.insert(m_vTouchesViewCache.begin(), itr, vector.end());
                RETAIN_VECTOR(m_vTouchesViewCache);
            }
            break;
        }
        else
        {
            PUSH_BACK_IN_VECTOR(m_vTouchesViewCache, *itr);
        }
    }

    
    if (m_vWillTouchesViewCache.size() > 0)
    {
        CAScheduler::schedule(schedule_selector(CATouchController::passingTouchesViewCache), this, 0, 1, 0.066f);
    }
    else
    {
        TOUCHES_VIEW_SEL(ccTouchBegan);
    }
}

void CATouchController::touchMoved()
{
    do
    {
        CC_BREAK_IF(ccpDistance(m_tFirstPoint, m_pTouch->getLocation()) < 8.0f);
        m_tFirstPoint = CCPointZero;
        
        if (!m_vWillTouchesViewCache.empty()
            && CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViewCache), this))
        {
            CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViewCache), this);
            
            CLEAR_VECTOR(m_vWillTouchesViewCache);
            TOUCHES_VIEW_SEL(ccTouchBegan);
        }

        TOUCHES_VIEW_SEL(ccTouchMoved);
    }
    while (0);
    
}

void CATouchController::touchEnded()
{
    if (!m_vWillTouchesViewCache.empty()
        && CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViewCache), this))
    {
        CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViewCache), this);
        
        CLEAR_VECTOR(m_vWillTouchesViewCache);
        TOUCHES_VIEW_SEL(ccTouchBegan);
    }
    
    TOUCHES_VIEW_SEL(ccTouchEnded);
}

void CATouchController::touchCancelled()
{
    if (!m_vWillTouchesViewCache.empty()
        && CAScheduler::isScheduled(schedule_selector(CATouchController::passingTouchesViewCache), this))
    {
        CAScheduler::unschedule(schedule_selector(CATouchController::passingTouchesViewCache), this);
        
        CLEAR_VECTOR(m_vWillTouchesViewCache);
        TOUCHES_VIEW_SEL(ccTouchBegan);
    }
    
    TOUCHES_VIEW_SEL(ccTouchCancelled);
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

void CATouchDispatcher::touchesBegan(CCSet *touches, CCEvent *pEvent)
{
    if (m_bDispatchEvents)
    {
        m_bLocked = true;

        CCTouch *pTouch;
        CCSetIterator setIter;
        for (setIter = touches->begin(); setIter != touches->end(); setIter++)
        {
            pTouch = (CCTouch *)(*setIter);
            
            CATouchController* touchController = new CATouchController();
            touchController->setTouch(pTouch);
            touchController->setEvent(pEvent);
            m_vTouchControllers[pTouch->getID()] = touchController;
            touchController->touchBegan();
        }
        m_bLocked = false;
    }
}

void CATouchDispatcher::touchesMoved(CCSet *touches, CCEvent *pEvent)
{
    if (m_bDispatchEvents)
    {
        m_bLocked = true;
        
        CCTouch *pTouch;
        CCSetIterator setIter;
        for (setIter = touches->begin(); setIter != touches->end(); setIter++)
        {
            pTouch = (CCTouch *)(*setIter);
         
            CATouchController* touchController = m_vTouchControllers[pTouch->getID()];
            touchController->touchMoved();
        }
        m_bLocked = false;
    }
}

void CATouchDispatcher::touchesEnded(CCSet *touches, CCEvent *pEvent)
{
    if (m_bDispatchEvents)
    {
        m_bLocked = true;
        
        CCTouch *pTouch;
        CCSetIterator setIter;
        for (setIter = touches->begin(); setIter != touches->end(); setIter++)
        {
            pTouch = (CCTouch *)(*setIter);
            
            CATouchController* touchController = m_vTouchControllers[pTouch->getID()];
            touchController->touchEnded();
            
            
            CC_SAFE_RELEASE(touchController);
            m_vTouchControllers[pTouch->getID()] = NULL;
        }
        m_bLocked = false;
    }
}

void CATouchDispatcher::touchesCancelled(CCSet *touches, CCEvent *pEvent)
{
    if (m_bDispatchEvents)
    {
        m_bLocked = true;
        
        CCTouch *pTouch;
        CCSetIterator setIter;
        for (setIter = touches->begin(); setIter != touches->end(); ++setIter)
        {
            pTouch = (CCTouch *)(*setIter);
            
            CATouchController* touchController = m_vTouchControllers[pTouch->getID()];
            touchController->touchEnded();
            
            delete touchController;
            m_vTouchControllers[pTouch->getID()] = NULL;
        }
        m_bLocked = false;
    }
}



NS_CC_END

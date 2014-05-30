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

NS_CC_BEGIN


CATouchDispatcher::CATouchDispatcher(void)
:m_bDispatchEvents(true)
,m_bLocked(false)
{
    
}

CATouchDispatcher::~CATouchDispatcher(void)
{
    
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
            
            CAView* view = dynamic_cast<CAView*>(CCDirector::sharedDirector()->getRootWindow());
            
            CC_CONTINUE_IF(view->getTouchesSet()->containsObject(pTouch));
            
            bool bClaimed = false;
            
            do
            {
                bClaimed = view->ccTouchBegan(pTouch, pEvent);
                
                CC_BREAK_IF(!bClaimed);
                
                view->getTouchesSet()->addObject(pTouch);
                
                view->sortAllSubviews();
                
                CAView* nextView = NULL;
                
                CCObject* obj;

                CCARRAY_FOREACH_REVERSE(view->getSubviews(), obj)
                {
                    CAView* subview = dynamic_cast<CAView*>(obj);
                    if (subview && subview->isVisible())
                    {
                        CCPoint point = subview->convertTouchToNodeSpace(pTouch);
                        if (subview->getBounds().containsPoint(point))
                        {
//                            CC_BREAK_IF(subview->isMutableTouches() == false && subview->getTouchesSet()->count() > 0);
                            nextView = subview;
                            break;
                        }
                    }
                }
                view = nextView;
            }
            while (view);
            CCLog("begin pID = %d, count= %d", pTouch->getID(), pTouch->retainCount());
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
            
            CAView* view = dynamic_cast<CAView*>(CCDirector::sharedDirector()->getRootWindow());
            
            do
            {
                view->ccTouchMoved(pTouch, pEvent);
                
                CAView* nextView = NULL;
                
                CCObject* obj;
                
                CCARRAY_FOREACH_REVERSE(view->getSubviews(), obj)
                {
                    CAView* subview = dynamic_cast<CAView*>(obj);
                    if (subview && subview->getTouchesSet()->containsObject(pTouch))
                    {
                        nextView = subview;
                        break;
                    }
                }
                view = nextView;
            }
            while (view);
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
            
            CAView* view = dynamic_cast<CAView*>(CCDirector::sharedDirector()->getRootWindow());
            
            do
            {
                view->ccTouchEnded(pTouch, pEvent);
                
                CAView* nextView = NULL;
                
                CCObject* obj;
                
                CCARRAY_FOREACH_REVERSE(view->getSubviews(), obj)
                {
                    CAView* subview = dynamic_cast<CAView*>(obj);
                    if (subview && subview->getTouchesSet()->containsObject(pTouch))
                    {
                        nextView = subview;
                        break;
                    }
                }
                
                
                view->getTouchesSet()->removeObject(pTouch);
                
                view = nextView;
            }
            while (view);
            
            CCLog("end pID = %d, count= %d", pTouch->getID(), pTouch->retainCount());
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
            
            CAView* view = dynamic_cast<CAView*>(CCDirector::sharedDirector()->getRootWindow());
            
            do
            {
                view->ccTouchCancelled(pTouch, pEvent);
                
                CAView* nextView = NULL;
                
                CCObject* obj;
                
                CCARRAY_FOREACH_REVERSE(view->getSubviews(), obj)
                {
                    CAView* subview = dynamic_cast<CAView*>(obj);
                    if (subview && subview->getTouchesSet()->containsObject(pTouch))
                    {
                        nextView = subview;
                        break;
                    }
                }
                
                view->getTouchesSet()->removeObject(pTouch);
                
                view = nextView;
            }
            while (view);
        }
        m_bLocked = false;
    }
}

NS_CC_END

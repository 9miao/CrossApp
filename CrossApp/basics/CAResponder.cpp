//
//  CAResponder.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-6-15.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAResponder.h"
#include "dispatcher/CATouchDispatcher.h"
#include "CAApplication.h"

NS_CC_BEGIN;

CAResponder::CAResponder()
:m_bTouchEnabled(true)
,m_bScrollEnabled(true)
,m_bHaveNextResponder(true)
,m_bHorizontalScrollEnabled(true)
,m_bVerticalScrollEnabled(true)
,m_bPriorityScroll(false)
,m_bReachBoundaryHandOverToSuperview(true)
,m_bTouchEventScrollHandOverToSuperview(true)
,m_bMultipleTouchEnabled(true)
,m_bMouseMovedEnabled(false)
,m_bMouseScrollWheelEnabled(false)
,m_bScrollRunning(false)
,m_uZLevel(0)
{

}

CAResponder::~CAResponder()
{
    this->setMouseMovedEnabled(false);
    this->setMouseScrollWheelEnabled(false);
}

bool CAResponder::isFirstResponder()
{
    return this->isEqual(CAApplication::getApplication()->getTouchDispatcher()->getFirstResponder());
}

bool CAResponder::resignFirstResponder()
{
    if (!this->isFirstResponder())
    {
        return false;
    }
    
    CAApplication::getApplication()->getTouchDispatcher()->setFirstResponder(nullptr);
    
    return true;
}

bool CAResponder::becomeFirstResponder()
{
    if (this->isFirstResponder())
    {
        return false;
    }
    
    CAApplication::getApplication()->getTouchDispatcher()->setFirstResponder(this);
    
    return true;
}

CAResponder* CAResponder::nextResponder()
{
    return NULL;
}

bool CAResponder::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    return false;
}

void CAResponder::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAResponder::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAResponder::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAResponder::mouseMoved(CATouch* pTouch, CAEvent* pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAResponder::mouseMovedOutSide(CATouch *pTouch, CAEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAResponder::mouseScrollWheel(CATouch* pTouch, float off_x, float off_y, CAEvent* pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAResponder::setMouseMovedEnabled(bool var)
{
    m_bMouseMovedEnabled = var;
    if (m_bMouseMovedEnabled)
    {
        CAApplication::getApplication()->getTouchDispatcher()->addMouseMovedResponder(this);
    }
    else
    {
        CAApplication::getApplication()->getTouchDispatcher()->removeMouseMovedResponder(this);
    }
}

bool CAResponder::isMouseMovedEnabled()
{
    return m_bMouseMovedEnabled;
}

void CAResponder::setMouseScrollWheelEnabled(bool var)
{
    m_bMouseScrollWheelEnabled = var;
    if (m_bMouseScrollWheelEnabled)
    {
        CAApplication::getApplication()->getTouchDispatcher()->addMouseScrollWheel(this);
    }
    else
    {
        CAApplication::getApplication()->getTouchDispatcher()->removeMouseScrollWheel(this);
    }
}

bool CAResponder::isMouseScrollWheelEnabled()
{
    return m_bMouseScrollWheelEnabled;
}

void CAResponder::setScrollRunning(bool var)
{
    m_bScrollRunning = var;
    if (m_bScrollRunning)
    {
        CAApplication::getApplication()->getTouchDispatcher()->setScrollRunningResponder(this);
    }
    else
    {
        CAApplication::getApplication()->getTouchDispatcher()->removeScrollRunningResponder(this);
    }
}

NS_CC_END;

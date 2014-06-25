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
:m_bSlideContainers(false)
,m_bControl(false)
,m_eTouchSidingDirection(CATouchSidingDirectionMultiple)
{

}

CAResponder::~CAResponder()
{

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
    
    CAApplication::getApplication()->getTouchDispatcher()->setFirstResponder(NULL);
    
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

NS_CC_END;
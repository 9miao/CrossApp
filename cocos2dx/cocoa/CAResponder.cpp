//
//  CAResponder.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-6-15.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAResponder.h"
#include "touch_dispatcher/CATouchDispatcher.h"
#include "CCDirector.h"

NS_CC_BEGIN;

CAResponder::CAResponder()
:m_bSlideContainers(false)
,m_bControl(false)
{

}

CAResponder::~CAResponder()
{

}

bool CAResponder::isFirstResponder()
{
    return this->isEqual(CCDirector::sharedDirector()->getTouchDispatcher()->getFirstResponder());
}

bool CAResponder::resignFirstResponder()
{
    if (this->isFirstResponder())
    {
        return false;
    }
    
    CCDirector::sharedDirector()->getTouchDispatcher()->setFirstResponder(this);
    
    return true;
}

bool CAResponder::becomeFirstResponder()
{
    if (!this->isFirstResponder())
    {
        return false;
    }
    
    CCDirector::sharedDirector()->getTouchDispatcher()->setFirstResponder(NULL);
    
    return true;
}

bool CAResponder::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    return false;
}

void CAResponder::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAResponder::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAResponder::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

NS_CC_END;
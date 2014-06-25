//
//  CAResponder.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-6-15.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossApp__CAResponder__
#define __CrossApp__CAResponder__

#include <iostream>
#include "CAObject.h"
#include "dispatcher/CATouch.h"

NS_CC_BEGIN;

typedef enum
{
    CATouchSidingDirectionMultiple    = 0,
    CATouchSidingDirectionHorizontal,
    CATouchSidingDirectionVertical,
}CATouchSidingDirection;

class CATouch;

class CC_DLL CAResponder: public CAObject
{
    
public:

    CAResponder();
    
    virtual ~CAResponder();
    
    bool isFirstResponder();
    
    virtual bool resignFirstResponder();

    virtual bool becomeFirstResponder();
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bSlideContainers, SlideContainers);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bControl, Control);
    
    CC_SYNTHESIZE(CATouchSidingDirection, m_eTouchSidingDirection, TouchSidingDirection);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
};

NS_CC_END;

#endif /* defined(__CrossApp__CAResponder__) */

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

class CATouch;

class CC_DLL CAResponder: public CAObject
{
    
public:

    CAResponder();
    
    virtual ~CAResponder();
    
    bool isFirstResponder();
    
    virtual bool resignFirstResponder();

    virtual bool becomeFirstResponder();
    
    virtual CAResponder* nextResponder();
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bTouchMovedStopSubviews, TouchMovedStopSubviews);

    CC_SYNTHESIZE_IS(bool, m_bTouchMovedListenHorizontal, TouchMovedListenHorizontal);
    
    CC_SYNTHESIZE_IS(bool, m_bTouchMovedListenVertical, TouchMovedListenVertical);
    
    CC_SYNTHESIZE_IS(bool, m_bTouchEnabled, TouchEnabled);
    
    CC_SYNTHESIZE_IS(bool, m_bAllowIntercepted, AllowIntercepted);
    
    CC_SYNTHESIZE_IS(bool, m_bHaveNextResponder, HaveNextResponder);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bSlidingMinX, SlidingMinX);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bSlidingMaxX, SlidingMaxX);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bSlidingMinY, SlidingMinY);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bSlidingMaxY, SlidingMaxY);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
};

NS_CC_END;

#endif /* defined(__CrossApp__CAResponder__) */

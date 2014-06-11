//
//  CAResponder.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-6-15.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __cocos2dx__CAResponder__
#define __cocos2dx__CAResponder__

#include <iostream>
#include "cocoa/CCObject.h"
#include "touch_dispatcher/CCTouch.h"

NS_CC_BEGIN;

class CCTouch;

class CC_DLL CAResponder: public CCObject
{
    
public:

    CAResponder();
    
    virtual ~CAResponder();
    
    bool isFirstResponder();
    
    virtual bool resignFirstResponder();

    virtual bool becomeFirstResponder();
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bSlideContainers, SlideContainers);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bControl, Control);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
};

NS_CC_END;

#endif /* defined(__cocos2dx__CAResponder__) */

//
//  CAControl.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-6.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __cocos2dx__CAControl__
#define __cocos2dx__CAControl__

#include <iostream>
#include "sprite_nodes/CAView.h"

NS_CC_BEGIN

typedef enum
{
    CAControlEventTouchDown = 0,
    CAControlEventTouchDownRepeat,
    CAControlEventTouchMoved,
    CAControlEventTouchMovedOutSide,
    CAControlEventTouchUpInSide,
    CAControlEventTouchUpSide
}CAControlEvents;

typedef enum
{
    CAControlStateNormal = 0,
    CAControlStateHighlighted,
    CAControlStateDisabled,
    CAControlStateSelected,
    CAControlStateAll
}CAControlState;

class CAControl;

typedef bool (CCObject::*SEL_CAControl)(CAControl*, CCPoint);
#define CAControl_selector(_SELECTOR) (SEL_CAControl)(&_SELECTOR)

class CC_DLL CAControl: public CAView
{
    
public:
    
    CAControl();
    
    virtual ~CAControl();
    
    CC_SYNTHESIZE(bool, m_bControlStateLocked, ControlStateLocked);
    
    CC_PROPERTY(CAControlState, m_eControlState, ControlState);
    
    void setControlStateNormal();
    
    void setControlStateHighlighted();
    
    void setControlStateDisabled();
    
    void setControlStateSelected();
    
    void addTarget(CCObject* target, SEL_CAControl selector, CAControlEvents event);
    
    void removeTarget(CCObject* target, SEL_CAControl selector, CAControlEvents event);
    
    CC_PROPERTY_IS(bool, m_bTouchEnabled, TouchEnabled);
    
protected:
    
    virtual void setContentSize(const CCSize& var);
    
    virtual void setBackGroundViewForState(CAControlState controlState, CAView *var);
    
    virtual CAView* getBackGroundViewForState(CAControlState controlState);
    
protected:
    
    CAView* m_pBackGroundView[CAControlStateAll];
    
    CCObject* m_pTarget[6];
    
    SEL_CAControl m_selTouch[6];
};

#define CAControlTouchDown (CAControlEventTouchDown)
#define CAControlTouchMoved (CAControlEventTouchMoved)
#define CAControlTouchMovedOutSide (CAControlEventTouchMovedOutSide)
#define CAControlTouchUpInSide (CAControlEventTouchUpInSide)
#define CAControlTouchUpSide (CAControlEventTouchUpSide)


NS_CC_END

#endif /* defined(__cocos2dx__CAControl__) */

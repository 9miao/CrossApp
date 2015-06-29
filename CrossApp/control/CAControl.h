//
//  CAControl.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-6.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossApp__CAControl__
#define __CrossApp__CAControl__

#include <iostream>
#include "view/CAView.h"

NS_CC_BEGIN

typedef enum
{
    CAControlEventTouchDown = 0,
    CAControlEventTouchDownRepeat,
    CAControlEventTouchMoved,
    CAControlEventTouchMovedOutSide,
    CAControlEventTouchUpInSide,
    CAControlEventTouchUpOutSide,
    CAControlEventTouchValueChanged,
    CAControlEventTouchLongPress,
    CAControlEventTouchCancelled,
    CAControlEventMax
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

typedef void (CAObject::*SEL_CAControl)(CAControl*, CCPoint);
#define CAControl_selector(_SELECTOR) (SEL_CAControl)(&_SELECTOR)

class CC_DLL CAControl: public CAView
{
    
public:
    
    CAControl();
    
    virtual ~CAControl();
    
    bool init();
    
    void setControlStateNormal();
    
    void setControlStateHighlighted();
    
    void setControlStateDisabled();
    
    void setControlStateSelected();
    
    CC_SYNTHESIZE_PASS_BY_REF(CAControlState, m_eControlState, ControlState);
    
    CC_SYNTHESIZE(bool, m_bControlStateLocked, ControlStateLocked);
    
protected:
    
    virtual void addTarget(CAObject* target, SEL_CAControl selector, CAControlEvents event);
    
    virtual void removeTarget(CAObject* target, SEL_CAControl selector, CAControlEvents event);
    
    virtual void removeAllTargets();
    
protected:
    
    CAObject* m_pTarget[CAControlEventMax];
    
    SEL_CAControl m_selTouch[CAControlEventMax];
};

#define CAControlTouchDown (CAControlEventTouchDown)
#define CAControlTouchMoved (CAControlEventTouchMoved)
#define CAControlTouchMovedOutSide (CAControlEventTouchMovedOutSide)
#define CAControlTouchUpInSide (CAControlEventTouchUpInSide)
#define CAControlTouchUpSide (CAControlEventTouchUpSide)


NS_CC_END

#endif /* defined(__CrossAppx__CAControl__) */

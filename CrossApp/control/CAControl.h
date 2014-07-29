//
//  CAControl.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-6.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossAppx__CAControl__
#define __CrossAppx__CAControl__

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
    CAControlEventTouchUpSide,
    CAControlEventTouchValueChanged
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
    
    CC_SYNTHESIZE(CAControlState, m_eControlState, ControlState);
    
    CC_SYNTHESIZE(bool, m_bControlStateLocked, ControlStateLocked);

    CC_SYNTHESIZE_IS(bool, m_bTouchEnabled, TouchEnabled);
    
protected:
    
    virtual void addTarget(CAObject* target, SEL_CAControl selector, CAControlEvents event);
    
    virtual void removeTarget(CAObject* target, SEL_CAControl selector, CAControlEvents event);
    
    virtual void removeAllTargets();
    
protected:
    
    CAObject* m_pTarget[7];
    
    SEL_CAControl m_selTouch[7];
};

#define CAControlTouchDown (CAControlEventTouchDown)
#define CAControlTouchMoved (CAControlEventTouchMoved)
#define CAControlTouchMovedOutSide (CAControlEventTouchMovedOutSide)
#define CAControlTouchUpInSide (CAControlEventTouchUpInSide)
#define CAControlTouchUpSide (CAControlEventTouchUpSide)


NS_CC_END

#endif /* defined(__CrossAppx__CAControl__) */

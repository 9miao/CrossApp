//
//  CAControl.h
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-5-6.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#ifndef __cocos2dx__CAControl__
#define __cocos2dx__CAControl__

#include <iostream>
#include "base_nodes/CCNode.h"

NS_CC_BEGIN

typedef enum
{
    TouchBegin          = 0,
    TouchMoved          = 1,
    TouchMovedOutSide   = 2,
    TouchUpInSide       = 3,
    TouchUpSide         = 4,
}CAControlType;

typedef enum
{
    CAControlStateNormal         = 0,
    CAControlStateHighlighted    = 1,
    CAControlStateDisabled       = 2,
    CAControlStateSelected       = 3
}CAControlState;

class CC_DLL CAControl: public CCNodeRGBA
{
    
public:
    
    CAControl();
    
    virtual ~CAControl();
    
    CC_SYNTHESIZE(bool, m_specially, TouchSpecially);
    
    CC_PROPERTY(CAControlState, m_eControlState, ControlState);
    
    void setControlStateNormal();
    
    void setControlStateHighlighted();
    
    void setControlStateDisabled();
    
    void setControlStateSelected();
    
    bool isNormal();
    
    bool isHighlighted();
    
    bool isDisabled();
    
    bool isSelected();
    
    CC_PROPERTY(CCNodeRGBA*, m_pBackGroundView, BackGroundView);
    
    CC_PROPERTY(CCNodeRGBA*, m_pHighlightedBackGroundView, HighlightedBackGroundView);
    
    CC_PROPERTY(CCNodeRGBA*, m_pDisabledBackGroundView, DisabledBackGroundView);
    
    CC_PROPERTY(CCNodeRGBA*, m_pSelectedBackGroundView, SelectedBackGroundView);
    
    virtual void setContentSize(const CCSize& var);
    
};


NS_CC_END

#endif /* defined(__cocos2dx__CAControl__) */

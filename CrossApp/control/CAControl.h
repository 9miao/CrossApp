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

class CC_DLL CAControl: public CAView
{

public:
    
    enum class State : int
    {
        Normal = 0,
        Highlighted,
        Selected,
        Disabled
    };
    
public:
    
    CAControl();
    
    virtual ~CAControl();
    
    bool init();

    CC_PROPERTY_IS(bool, m_bRecSpe, RecSpe);
    
    CC_SYNTHESIZE(CAControl::State, m_eState, ControlState);
    
    void setControlStateNormal() { this->setControlState(CAControl::State::Normal); }
    void setControlStateHighlighted() { this->setControlState(CAControl::State::Highlighted); }
    void setControlStateSelected() { this->setControlState(CAControl::State::Selected); }
    void setControlStateDisabled() { this->setControlState(CAControl::State::Disabled); }
    
protected:

    using CAView::initWithColor;
};

NS_CC_END

#endif /* defined(__CrossApp__CAControl__) */

//
//  CAControl.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-6.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAControl.h"

NS_CC_BEGIN

CAControl::CAControl()
:m_eControlState(CAControlStateNormal)
,m_bControlStateLocked(false)
,m_bTouchEnabled(true)
{
    m_bControl = true;
    
    for (int i=0; i<CAControlStateAll; i++)
    {
        m_pBackGroundView[i] = NULL;
    }
    
    for (int i=0; i<7; i++)
    {
        m_selTouch[i] = NULL;
        m_pTarget[i] = NULL;
    }
}

CAControl::~CAControl()
{
    this->removeAllSubviews();
    for (int i=0; i<CAControlStateAll; i++)
    {
        CC_SAFE_RELEASE_NULL(m_pBackGroundView[i]);
    }
}

void CAControl::setControlState(CAControlState var)
{
    CC_RETURN_IF(var == CAControlStateAll);
    
    for (int i=0; i<CAControlStateAll; i++)
    {
        this->removeSubview(m_pBackGroundView[i]);
    }
    
    m_eControlState = var;
    
    if (m_bControlStateLocked)
    {
        m_eControlState = CAControlStateNormal;
    }
    
    if (m_pBackGroundView[m_eControlState] && m_eControlState != CAControlStateNormal)
    {
        m_pBackGroundView[m_eControlState]->setFrame(this->getBounds());
        this->insertSubview(m_pBackGroundView[m_eControlState], -1);
    }
    else if (m_pBackGroundView[CAControlStateNormal])
    {
        m_pBackGroundView[CAControlStateNormal]->setFrame(this->getBounds());
        this->insertSubview(m_pBackGroundView[CAControlStateNormal], -1);
    }
}

CAControlState CAControl::getControlState()
{
    return m_eControlState;
}

void CAControl::setControlStateNormal()
{
    this->setControlState(CAControlStateNormal);
}

void CAControl::setControlStateHighlighted()
{
    this->setControlState(CAControlStateHighlighted);
}

void CAControl::setControlStateDisabled()
{
    this->setControlState(CAControlStateDisabled);
}

void CAControl::setControlStateSelected()
{
    this->setControlState(CAControlStateSelected);
}

void CAControl::setBackGroundViewForState(CAControlState controlState, CAView *var)
{
    if (controlState == CAControlStateAll)
    {
        for (int i=0; i<CAControlStateAll; i++)
        {
            this->setBackGroundViewForState((CAControlState)i, var);
        }
        return;
    }
    
    if (m_pBackGroundView[controlState] != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeSubview(m_pBackGroundView[controlState]);
        CC_SAFE_RELEASE(m_pBackGroundView[controlState]);
        m_pBackGroundView[controlState] = var;
        this->setControlState(m_eControlState);
    }
}

CAView* CAControl::getBackGroundViewForState(CAControlState controlState)
{
    if (controlState == CAControlStateAll)
    {
        return NULL;
    }
    return m_pBackGroundView[controlState];
}

void CAControl::addTarget(CCObject *target, SEL_CAControl selector, CAControlEvents event)
{
    m_pTarget[event] = target;
    m_selTouch[event] = selector;
}

void CAControl::removeTarget(CCObject *target, SEL_CAControl selector, CAControlEvents event)
{
    m_pTarget[event] = NULL;
    m_selTouch[event] = NULL;
}

void CAControl::removeAllTargets()
{
    for (int i=0; i<7; i++)
    {
        m_pTarget[i] = NULL;
        m_selTouch[i] = NULL;
    }
}

void CAControl::setTouchEnabled(bool enabled)
{
    m_bTouchEnabled = enabled;
}

bool CAControl::isTouchEnabled()
{
    return m_bTouchEnabled;
}

void CAControl::setContentSize(const CCSize& var)
{
    CAView::setContentSize(var);
    
    for(int i=0; i<CAControlStateAll; i++)
    {
        CC_CONTINUE_IF(m_pBackGroundView[i] == NULL);
        m_pBackGroundView[i]->setFrame(this->getBounds());
    }
}
    

NS_CC_END
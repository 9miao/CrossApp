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
    m_bStopSuperviewListenEvents = true;
    
    for (int i=0; i<7; i++)
    {
        m_selTouch[i] = NULL;
        m_pTarget[i] = NULL;
    }
}

CAControl::~CAControl()
{
    this->removeAllSubviews();
}

bool CAControl::init()
{
    return true;
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

void CAControl::addTarget(CAObject *target, SEL_CAControl selector, CAControlEvents event)
{
    m_pTarget[event] = target;
    m_selTouch[event] = selector;
}

void CAControl::removeTarget(CAObject *target, SEL_CAControl selector, CAControlEvents event)
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

NS_CC_END
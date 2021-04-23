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
:m_bRecSpe(true)
,m_eState(CAControl::State::Normal)
{
    this->setHaveNextResponder(false);
    this->setMultipleTouchEnabled(false);
}

CAControl::~CAControl()
{

}

bool CAControl::init()
{
    return true;
}

void CAControl::setRecSpe(bool var)
{
    m_bRecSpe = var;
    if (m_bRunning && m_eLayoutType == 2)
    {
        this->setLayout(m_obLayout);
    }
}

bool CAControl::isRecSpe()
{
    return m_bRecSpe;
}

NS_CC_END

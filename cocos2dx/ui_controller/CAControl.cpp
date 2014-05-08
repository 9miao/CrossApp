//
//  CAControl.cpp
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-5-6.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#include "CAControl.h"

NS_CC_BEGIN

CAControl::CAControl()
:m_pBackGroundView(NULL)
,m_pHighlightedBackGroundView(NULL)
,m_pDisabledBackGroundView(NULL)
,m_pSelectedBackGroundView(NULL)
,m_eControlState(CAControlStateNormal)
,m_specially(true)
{

}

CAControl::~CAControl()
{
    this->removeAllChildren();
    CC_SAFE_DELETE(m_pBackGroundView);
    CC_SAFE_DELETE(m_pHighlightedBackGroundView);
    CC_SAFE_DELETE(m_pDisabledBackGroundView);
    CC_SAFE_DELETE(m_pSelectedBackGroundView);
}

void CAControl::setControlState(CAControlState var)
{
    this->removeChild(m_pBackGroundView);
    this->removeChild(m_pHighlightedBackGroundView);
    this->removeChild(m_pDisabledBackGroundView);
    this->removeChild(m_pSelectedBackGroundView);
    
    m_eControlState = var;
    
    switch (m_eControlState)
    {
        case CAControlStateNormal:
        {
            if (m_pBackGroundView)
            {
                m_pBackGroundView->setFrame(this->getBounds());
                this->addChild(m_pBackGroundView, -1);
            }
            break;
        }
        case CAControlStateHighlighted:
        {
            if (m_pHighlightedBackGroundView)
            {
                m_pHighlightedBackGroundView->setFrame(this->getBounds());
                this->addChild(m_pHighlightedBackGroundView, -1);
            }
            else if (m_pBackGroundView)
            {
                m_pBackGroundView->setFrame(this->getBounds());
                this->addChild(m_pBackGroundView, -1);
            }
            break;
        }
        case CAControlStateDisabled:
        {
            if (m_pDisabledBackGroundView)
            {
                m_pDisabledBackGroundView->setFrame(this->getBounds());
                this->addChild(m_pDisabledBackGroundView, -1);
            }
            else if (m_pBackGroundView)
            {
                m_pBackGroundView->setFrame(this->getBounds());
                this->addChild(m_pBackGroundView, -1);
            }
            break;
        }
        case CAControlStateSelected:
        {
            if (m_pSelectedBackGroundView)
            {
                m_pSelectedBackGroundView->setFrame(this->getBounds());
                this->addChild(m_pSelectedBackGroundView, -1);
            }
            else if (m_pBackGroundView)
            {
                m_pBackGroundView->setFrame(this->getBounds());
                this->addChild(m_pBackGroundView, -1);
            }
            break;
        }
        default:
            break;
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

bool CAControl::isNormal()
{
    return (m_eControlState == CAControlStateNormal);
}

bool CAControl::isHighlighted()
{
    return (m_eControlState == CAControlStateHighlighted);
}

bool CAControl::isDisabled()
{
    return (m_eControlState == CAControlStateDisabled);
}

bool CAControl::isSelected()
{
    return (m_eControlState == CAControlStateSelected);
}

void CAControl::setBackGroundView(cocos2d::CCNodeRGBA *var)
{
    if (m_pBackGroundView != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeChild(m_pBackGroundView);
        CC_SAFE_RELEASE(m_pBackGroundView);
        m_pBackGroundView = var;
    }
}

CCNodeRGBA* CAControl::getBackGroundView()
{
    return m_pBackGroundView;
}

void CAControl::setHighlightedBackGroundView(cocos2d::CCNodeRGBA *var)
{
    if (m_pHighlightedBackGroundView != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeChild(m_pHighlightedBackGroundView);
        CC_SAFE_RELEASE(m_pHighlightedBackGroundView);
        m_pHighlightedBackGroundView = var;
    }
}

CCNodeRGBA* CAControl::getHighlightedBackGroundView()
{
    return m_pHighlightedBackGroundView;
}

void CAControl::setDisabledBackGroundView(cocos2d::CCNodeRGBA *var)
{
    if (m_pDisabledBackGroundView != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeChild(m_pDisabledBackGroundView);
        CC_SAFE_RELEASE(m_pDisabledBackGroundView);
        m_pDisabledBackGroundView = var;
    }
}

CCNodeRGBA* CAControl::getDisabledBackGroundView()
{
    return m_pDisabledBackGroundView;
}

void CAControl::setSelectedBackGroundView(cocos2d::CCNodeRGBA *var)
{
    if (m_pSelectedBackGroundView != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeChild(m_pSelectedBackGroundView);
        CC_SAFE_RELEASE(m_pSelectedBackGroundView);
        m_pSelectedBackGroundView = var;
    }
}

CCNodeRGBA* CAControl::getSelectedBackGroundView()
{
    return m_pSelectedBackGroundView;
}

void CAControl::setContentSize(const CCSize& var)
{
    CCNodeRGBA::setContentSize(var);
    
    if (m_pBackGroundView)
    {
        m_pBackGroundView->setFrame(this->getBounds());
    }
    
    if (m_pHighlightedBackGroundView)
    {
        m_pHighlightedBackGroundView->setFrame(this->getBounds());
    }
    
    if (m_pDisabledBackGroundView)
    {
        m_pDisabledBackGroundView->setFrame(this->getBounds());
    }
    
    if (m_pSelectedBackGroundView)
    {
        m_pSelectedBackGroundView->setFrame(this->getBounds());
    }
}
    

NS_CC_END
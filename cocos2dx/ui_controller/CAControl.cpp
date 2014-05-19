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
    this->removeAllSubviews();
    CC_SAFE_DELETE(m_pBackGroundView);
    CC_SAFE_DELETE(m_pHighlightedBackGroundView);
    CC_SAFE_DELETE(m_pDisabledBackGroundView);
    CC_SAFE_DELETE(m_pSelectedBackGroundView);
}

void CAControl::setControlState(CAControlState var)
{
    this->removeSubview(m_pBackGroundView);
    this->removeSubview(m_pHighlightedBackGroundView);
    this->removeSubview(m_pDisabledBackGroundView);
    this->removeSubview(m_pSelectedBackGroundView);

    m_eControlState = var;
    
    switch (m_eControlState)
    {
        case CAControlStateNormal:
        {
            if (m_pBackGroundView)
            {
                m_pBackGroundView->setFrame(this->getBounds());
                this->insertSubview(m_pBackGroundView, -1);
            }
            break;
        }
        case CAControlStateHighlighted:
        {
            if (m_pHighlightedBackGroundView)
            {
                m_pHighlightedBackGroundView->setFrame(this->getBounds());
                this->insertSubview(m_pHighlightedBackGroundView, -1);
            }
            else if (m_pBackGroundView)
            {
                m_pBackGroundView->setFrame(this->getBounds());
                this->insertSubview(m_pBackGroundView, -1);
            }
            break;
        }
        case CAControlStateDisabled:
        {
            if (m_pDisabledBackGroundView)
            {
                m_pDisabledBackGroundView->setFrame(this->getBounds());
                this->insertSubview(m_pDisabledBackGroundView, -1);
            }
            else if (m_pBackGroundView)
            {
                m_pBackGroundView->setFrame(this->getBounds());
                this->insertSubview(m_pBackGroundView, -1);
            }
            break;
        }
        case CAControlStateSelected:
        {
            if (m_pSelectedBackGroundView)
            {
                m_pSelectedBackGroundView->setFrame(this->getBounds());
                this->insertSubview(m_pSelectedBackGroundView, -1);
            }
            else if (m_pBackGroundView)
            {
                m_pBackGroundView->setFrame(this->getBounds());
                this->insertSubview(m_pBackGroundView, -1);
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

void CAControl::setBackGroundView(cocos2d::CAView *var)
{
    if (m_pBackGroundView != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeSubview(m_pBackGroundView);
        CC_SAFE_RELEASE(m_pBackGroundView);
        m_pBackGroundView = var;
    }
}

CAView* CAControl::getBackGroundView()
{
    return m_pBackGroundView;
}

void CAControl::setHighlightedBackGroundView(cocos2d::CAView *var)
{
    if (m_pHighlightedBackGroundView != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeSubview(m_pHighlightedBackGroundView);
        CC_SAFE_RELEASE(m_pHighlightedBackGroundView);
        m_pHighlightedBackGroundView = var;
    }
}

CAView* CAControl::getHighlightedBackGroundView()
{
    return m_pHighlightedBackGroundView;
}

void CAControl::setDisabledBackGroundView(cocos2d::CAView *var)
{
    if (m_pDisabledBackGroundView != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeSubview(m_pDisabledBackGroundView);
        CC_SAFE_RELEASE(m_pDisabledBackGroundView);
        m_pDisabledBackGroundView = var;
    }
}

CAView* CAControl::getDisabledBackGroundView()
{
    return m_pDisabledBackGroundView;
}

void CAControl::setSelectedBackGroundView(cocos2d::CAView *var)
{
    if (m_pSelectedBackGroundView != var)
    {
        CC_SAFE_RETAIN(var);
        this->removeSubview(m_pSelectedBackGroundView);
        CC_SAFE_RELEASE(m_pSelectedBackGroundView);
        m_pSelectedBackGroundView = var;
    }
}

CAView* CAControl::getSelectedBackGroundView()
{
    return m_pSelectedBackGroundView;
}

void CAControl::setContentSize(const CCSize& var)
{
    CAView::setContentSize(var);
    
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
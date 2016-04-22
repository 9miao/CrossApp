//
//  CATableView.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 16-4-20.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CACell.h"

NS_CC_BEGIN

CACell::CACell()
:m_pBackgroundView(NULL)
,m_pContentView(NULL)
,m_bControlStateEffect(true)
,m_bAllowsSelected(true)
{
    this->setHaveNextResponder(true);
    this->setDisplayRange(false);
    this->setColor(CAColor_clear);
}

CACell::~CACell()
{
    CC_SAFE_RELEASE_NULL(m_pContentView);
    CC_SAFE_RELEASE_NULL(m_pBackgroundView);
}

CACell* CACell::create(const std::string& reuseIdentifier)
{
    CACell* cell = new CACell();
    if (cell && cell->initWithReuseIdentifier(reuseIdentifier))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

bool CACell::initWithReuseIdentifier(const std::string& reuseIdentifier)
{
    m_pContentView = new CAView();
    m_pContentView->setLayout(DLayoutFill);
    this->addSubview(m_pContentView);
    
    this->setBackgroundView(CAView::create());
    this->setReuseIdentifier(reuseIdentifier);
    this->normalCell();
    
    return true;
}

void CACell::setBackgroundView(CrossApp::CAView *var)
{
    CC_RETURN_IF(var == m_pBackgroundView);
    m_pContentView->removeSubview(m_pBackgroundView);
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pBackgroundView);
    m_pBackgroundView = var;
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setLayout(DLayoutFill);
    m_pContentView->insertSubview(m_pBackgroundView, -1);
}

CAView* CACell::getBackgroundView()
{
    return m_pBackgroundView;
}

void CACell::setControlState(const CAControlState& var)
{
    if (m_bAllowsSelected == false && var == CAControlStateSelected)
    {
        CAControl::setControlState(CAControlStateNormal);
    }
    else
    {
        CAControl::setControlState(var);
    }
    
    CC_RETURN_IF(m_bControlStateEffect == false);
    switch (m_eControlState)
    {
        case CAControlStateNormal:
            this->normalCell();
            break;
        case CAControlStateHighlighted:
            this->highlightedCell();
            break;
        case CAControlStateSelected:
            this->selectedCell();
            break;
        case CAControlStateDisabled:
            this->disabledCell();
            break;
        default:
            break;
    }
}

void CACell::normalCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(255, 255, 255, 255));
}

void CACell::highlightedCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CACell::selectedCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(50, 193, 255, 255));
}


void CACell::disabledCell()
{
    CC_RETURN_IF(m_pBackgroundView == NULL);
    m_pBackgroundView->setColor(ccc4(127, 127, 127, 255));
}

void CACell::resetCell()
{
    this->setVisible(true);
    this->normalCell();
    this->recoveryCell();
    m_pContentView->setLayout(DLayoutFill);
    m_pContentView->setScale(1.0f);
    m_pContentView->setRotation(0);
}

NS_CC_END
//
//  CAPullToReView.h
//  CrossApp
//
//  Created by LiYuanfeng on 14-9-5.
//  Copyright (c) 2014年 http://www.9miao.com All rights reserved.
//

#include "CAPullToRefreshView.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "basics/CAPointExtension.h"
#include "animation/CAViewAnimation.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
#pragma CAPullToRefreshView

NS_CC_BEGIN

CAPullToRefreshView::CAPullToRefreshView(CAPullToRefreshView::Type type)
:m_eType(type)
,m_pPullToImage(nullptr)
,m_pLoadingView(nullptr)
,m_pPullToImageView(nullptr)
,m_pPullToRefreshLabel(nullptr)
,m_pReleaseToRefreshLabel(nullptr)
,m_pRefreshingLabel(nullptr)
,m_sPullToRefreshText("")
,m_sReleaseToRefreshText("")
,m_sRefreshingText("")
,m_cLabelColor(CAColor4B::GRAY)
,m_eState(CAPullToRefreshView::State::None)
{
    
}

CAPullToRefreshView::~CAPullToRefreshView()
{
    CC_SAFE_RELEASE(m_pPullToImage);
    CC_SAFE_RELEASE(m_pLoadingView);
}

CAPullToRefreshView* CAPullToRefreshView::create(CAPullToRefreshView::Type type)
{
    CAPullToRefreshView* view = new CAPullToRefreshView(type);
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

bool CAPullToRefreshView::init()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAPullToRefreshView");
    switch (m_eType)
    {
        case CAPullToRefreshView::Type::Header:
        {
            
            this->setPullToImage(CAImage::create(map.at("header")));
            this->setPullToRefreshText("下拉可以刷新");
            this->setReleaseToRefreshText("松开立即刷新");
            this->setRefreshingText("正在帮你刷新...");
        }
            break;
        case CAPullToRefreshView::Type::Footer:
        {
            this->setPullToImage(CAImage::create(map.at("footer")));
            this->setPullToRefreshText("上拉可以加载更多数据");
            this->setReleaseToRefreshText("松开立即加载更多数据");
            this->setRefreshingText("正在帮你加载数据...");
        }
            break;
        default:
            break;
    }
    
    CAActivityIndicatorView* activityView = CAActivityIndicatorView::create();
    activityView->setStyle(CAActivityIndicatorView::Style::Gray);
    this->setLoadingView(activityView);
    
    return true;
}

bool CAPullToRefreshView::isLayoutFinish()
{
    return m_pPullToImageView != NULL;
}

void CAPullToRefreshView::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(contentSize);
    
    this->updateLayout();
}

void CAPullToRefreshView::startLayout()
{
    if (m_pPullToImageView == NULL)
    {
        m_pPullToImageView = CAImageView::createWithImage(m_pPullToImage);
        m_pPullToImageView->setScaleType(CAImageView::ScaleType::FitImageInside);
        
        this->addSubview(m_pPullToImageView);
    }
    if (m_pPullToRefreshLabel == NULL)
    {
        m_pPullToRefreshLabel = CALabel::create();
        m_pPullToRefreshLabel->setFontSize(24);
		m_pPullToRefreshLabel->setColor(m_cLabelColor);
        m_pPullToRefreshLabel->setTextAlignment(CATextAlignment::Center);
        m_pPullToRefreshLabel->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        m_pPullToRefreshLabel->setText(m_sPullToRefreshText);
        this->addSubview(m_pPullToRefreshLabel);
    }
    if (m_pReleaseToRefreshLabel == NULL)
    {
        m_pReleaseToRefreshLabel = CALabel::create();
        m_pReleaseToRefreshLabel->setFontSize(24);
        m_pReleaseToRefreshLabel->setColor(m_cLabelColor);
        m_pReleaseToRefreshLabel->setTextAlignment(CATextAlignment::Center);
        m_pReleaseToRefreshLabel->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        m_pReleaseToRefreshLabel->setText(m_sReleaseToRefreshText);
        this->addSubview(m_pReleaseToRefreshLabel);
    }
    if (m_pRefreshingLabel == NULL)
    {
        m_pRefreshingLabel = CALabel::create();
        m_pRefreshingLabel->setFontSize(24);
		m_pRefreshingLabel->setColor(m_cLabelColor);
        m_pRefreshingLabel->setTextAlignment(CATextAlignment::Center);
        m_pRefreshingLabel->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        m_pRefreshingLabel->setText(m_sRefreshingText);
        this->addSubview(m_pRefreshingLabel);
    }
    
    this->setState(CAPullToRefreshView::State::Normal);
    this->updateLayout();
}

void CAPullToRefreshView::updateLayout()
{
    DSize viewSize = m_obContentSize;
    DRect imageViewAndLoadingCenter;
    DRect labelCenter;
    imageViewAndLoadingCenter.size = DSize(25, 120);
    imageViewAndLoadingCenter.origin = DPoint(viewSize.width / 2 - 160, viewSize.height/2);
    labelCenter.size = DSize(256, 80);
    labelCenter.origin = viewSize/2;
    
    if (m_pPullToImageView)
    {
        m_pPullToImageView->setCenter(imageViewAndLoadingCenter);
    }
    if (m_pLoadingView)
    {
        m_pLoadingView->setCenterOrigin(imageViewAndLoadingCenter.origin);
    }
    if (m_pPullToRefreshLabel)
    {
        m_pPullToRefreshLabel->setCenter(labelCenter);
    }
    if (m_pReleaseToRefreshLabel)
    {
        m_pReleaseToRefreshLabel->setCenter(labelCenter);
    }
    if (m_pRefreshingLabel)
    {
        m_pRefreshingLabel->setCenter(labelCenter);
    }
}

void CAPullToRefreshView::setState(CAPullToRefreshView::State stateType)
{
    CC_RETURN_IF(m_eState == stateType);
    
    CC_RETURN_IF(stateType != CAPullToRefreshView::State::None && m_eState == CAPullToRefreshView::State::Refreshing);
    m_eState = stateType;
    switch (m_eState)
    {
        case CAPullToRefreshView::State::None:
        case CAPullToRefreshView::State::Normal:
        {
            if (m_pLoadingView)
            {
                m_pLoadingView->stopAnimating();
            }
            m_pPullToImageView->setVisible(true);
            m_pPullToRefreshLabel->setVisible(true);
            m_pReleaseToRefreshLabel->setVisible(false);
            m_pRefreshingLabel->setVisible(false);
            
            CAViewAnimation::removeAnimations(m_s__StrID);
            
            CAViewAnimation::beginAnimations(m_s__StrID);
            CAViewAnimation::setAnimationDuration(0.2f * m_pPullToImageView->getRotation() / 180.f);
            m_pPullToImageView->setRotation(0);
            CAViewAnimation::commitAnimations();
        }
            break;
        case CAPullToRefreshView::State::Pulling:
        {
            if (m_pLoadingView)
            {
                m_pLoadingView->stopAnimating();
            }
            m_pPullToImageView->setVisible(true);
            m_pPullToRefreshLabel->setVisible(false);
            m_pReleaseToRefreshLabel->setVisible(true);
            m_pRefreshingLabel->setVisible(false);
            
            CAViewAnimation::removeAnimations(m_s__StrID);
            
            CAViewAnimation::beginAnimations(m_s__StrID);
            CAViewAnimation::setAnimationDuration(0.2f * (1 - m_pPullToImageView->getRotation() / 180.f));
            m_pPullToImageView->setRotation(180);
            CAViewAnimation::commitAnimations();
        }
            break;
        case CAPullToRefreshView::State::Refreshing:
        {
            if (m_pLoadingView)
            {
                if (m_pLoadingView->getSuperview() == NULL)
                {
                    this->addSubview(m_pLoadingView);
                }
                m_pLoadingView->startAnimating();
            }
            m_pPullToImageView->setVisible(false);
            m_pPullToRefreshLabel->setVisible(false);
            m_pReleaseToRefreshLabel->setVisible(false);
            m_pRefreshingLabel->setVisible(true);
        }
            break;
        default:
            break;
    }
}

bool CAPullToRefreshView::isCanRefresh()
{
    return m_eState == CAPullToRefreshView::State::Pulling;
}

bool CAPullToRefreshView::isRefreshing()
{
    return m_eState == CAPullToRefreshView::State::Refreshing;
}

NS_CC_END

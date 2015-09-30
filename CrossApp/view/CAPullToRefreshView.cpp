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
#include "support/CCPointExtension.h"
#include "animation/CAViewAnimation.h"

#pragma CAPullToRefreshView

NS_CC_BEGIN

CAPullToRefreshView::CAPullToRefreshView(const CAPullToRefreshType& type)
:m_eLayoutLinearType(CALayoutLinearHorizontal)
,m_eType(type)
,m_pPullToImage(NULL)
,m_pLoadingView(NULL)
,m_pPullToImageView(NULL)
,m_pPullToRefreshLabel(NULL)
,m_pReleaseToRefreshLabel(NULL)
,m_pRefreshingLabel(NULL)
,m_sPullToRefreshText("")
,m_sReleaseToRefreshText("")
,m_sRefreshingText("")
,m_cLabelColor(CAColor_gray)
,m_eStateType(CAPullToRefreshStateNone)
{
    
}

CAPullToRefreshView::~CAPullToRefreshView()
{
    CC_SAFE_RELEASE(m_pPullToImage);
    CC_SAFE_RELEASE(m_pLoadingView);
}

CAPullToRefreshView* CAPullToRefreshView::create(const CAPullToRefreshType& type)
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
    switch (m_eType)
    {
        case CAPullToRefreshTypeHeader:
        {
            this->setPullToImage(CAImage::create("source_material/grayArrow_header.png"));
            this->setPullToRefreshText("下拉可以刷新");
            this->setReleaseToRefreshText("松开立即刷新");
            this->setRefreshingText("正在帮你刷新...");
        }
            break;
        case CAPullToRefreshTypeFooter:
        {
            this->setPullToImage(CAImage::create("source_material/grayArrow_footer.png"));
            this->setPullToRefreshText("上拉可以加载更多数据");
            this->setReleaseToRefreshText("松开立即加载更多数据");
            this->setRefreshingText("正在帮你加载数据...");
        }
            break;
        default:
            break;
    }
    
    CAActivityIndicatorView* activityView = CAActivityIndicatorView::create();
    activityView->setStyle(CAActivityIndicatorViewStyleGray);
    this->setLoadingView(activityView);
    
    return true;
}

bool CAPullToRefreshView::isLayoutFinish()
{
    return m_pPullToImageView != NULL;
}

void CAPullToRefreshView::startLayout()
{
    CCSize viewSize = this->getBounds().size;
    CCRect imageViewAndLoadingCenter;
    CCRect labelCenter;
    if (m_eLayoutLinearType == CALayoutLinearHorizontal)
    {
        imageViewAndLoadingCenter.size = CCSize(25, 120);
        imageViewAndLoadingCenter.origin = CCPoint(viewSize.width / 2 - 160, viewSize.height/2);
        labelCenter.size = CCSize(256, 80);
        labelCenter.origin = viewSize/2;
    }
    else
    {
        imageViewAndLoadingCenter.size = CCSize(120, 25);
        imageViewAndLoadingCenter.origin = CCPoint(viewSize.width / 2 - 160, 170);
        labelCenter.size = CCSize(80, 256);
        labelCenter.origin = viewSize/2;
    }
    
    if (m_pPullToImageView == NULL)
    {
        m_pPullToImageView = CAImageView::createWithCenter(imageViewAndLoadingCenter);
        m_pPullToImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        m_pPullToImageView->setImage(m_pPullToImage);
        this->addSubview(m_pPullToImageView);
    }
    m_pPullToImageView->setCenter(imageViewAndLoadingCenter);
    
    if (m_pLoadingView)
    {
        m_pLoadingView->setCenterOrigin(imageViewAndLoadingCenter.origin);
    }
    
    if (m_pPullToRefreshLabel == NULL)
    {
        m_pPullToRefreshLabel = CALabel::createWithCenter(labelCenter);
        m_pPullToRefreshLabel->setFontSize(_px(24));
		m_pPullToRefreshLabel->setFontColor(m_cLabelColor);
        m_pPullToRefreshLabel->setTextAlignment(CATextAlignmentCenter);
        m_pPullToRefreshLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_pPullToRefreshLabel->setText(m_sPullToRefreshText);
        this->addSubview(m_pPullToRefreshLabel);
    }
    m_pPullToRefreshLabel->setCenter(labelCenter);
    
    if (m_pReleaseToRefreshLabel == NULL)
    {
        m_pReleaseToRefreshLabel = CALabel::createWithCenter(labelCenter);
        m_pReleaseToRefreshLabel->setFontSize(_px(24));
        m_pReleaseToRefreshLabel->setFontColor(m_cLabelColor);
        m_pReleaseToRefreshLabel->setTextAlignment(CATextAlignmentCenter);
        m_pReleaseToRefreshLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_pReleaseToRefreshLabel->setText(m_sReleaseToRefreshText);
        this->addSubview(m_pReleaseToRefreshLabel);
    }
    m_pReleaseToRefreshLabel->setCenter(labelCenter);
    
    if (m_pRefreshingLabel == NULL)
    {
        m_pRefreshingLabel = CALabel::createWithCenter(labelCenter);
        m_pRefreshingLabel->setFontSize(_px(24));
		m_pRefreshingLabel->setFontColor(m_cLabelColor);
        m_pRefreshingLabel->setTextAlignment(CATextAlignmentCenter);
        m_pRefreshingLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_pRefreshingLabel->setText(m_sRefreshingText);
        this->addSubview(m_pRefreshingLabel);
    }
    m_pRefreshingLabel->setCenter(labelCenter);
    
    this->setPullToRefreshStateType(CAPullToRefreshStateNormal);
}

void CAPullToRefreshView::setPullToRefreshStateType(const CAPullToRefreshStateType& stateType)
{
    CC_RETURN_IF(m_eStateType == stateType);
    
    CC_RETURN_IF(stateType != CAPullToRefreshStateNone && m_eStateType == CAPullToRefreshStateRefreshing);
    m_eStateType = stateType;
    switch (m_eStateType)
    {
        case CAPullToRefreshStateNone:
        case CAPullToRefreshStateNormal:
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
            
            CAViewAnimation::beginAnimations(m_s__StrID, NULL);
            CAViewAnimation::setAnimationDuration(0.2f * m_pPullToImageView->getRotation() / 180.f);
            m_pPullToImageView->setRotation(0);
            CAViewAnimation::commitAnimations();
        }
            break;
        case CAPullToRefreshStatePulling:
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
            
            CAViewAnimation::beginAnimations(m_s__StrID, NULL);
            CAViewAnimation::setAnimationDuration(0.2f * (1 - m_pPullToImageView->getRotation() / 180.f));
            m_pPullToImageView->setRotation(180);
            CAViewAnimation::commitAnimations();
        }
            break;
        case CAPullToRefreshStateRefreshing:
        {
            if (m_pLoadingView)
            {
                if (m_pLoadingView->isRunning() == false)
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
    return m_eStateType == CAPullToRefreshStatePulling;
}

bool CAPullToRefreshView::isRefreshing()
{
    return m_eStateType == CAPullToRefreshStateRefreshing;
}

NS_CC_END
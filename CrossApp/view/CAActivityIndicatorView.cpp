//
//  CAActivityIndicatorView.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-18.
//  Copyright (c) 2014年 CrossApp. All rights reserved.
//

#include "CAActivityIndicatorView.h"
#include "view/CADrawingPrimitives.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "CAScale9ImageView.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

CAActivityIndicatorView::CAActivityIndicatorView()
: m_bStopAnimation(false)
, m_hidesWhenStopped(true)
, m_style(CAActivityIndicatorView::Style::GrayLarge)
, m_color(CAColor4B(255, 255, 255, 255))
, m_duration(0.1f)
, m_pImageView(nullptr)
, m_pBackView(nullptr)
, m_fLoadingMinTime(0.0f)
, m_fLoadingTime(0.0f)
, m_pTarget(nullptr)
, m_pCallFunc(nullptr)
, m_nTimesOneCycle(12)
, m_fCycleTime(1.0f)
, m_obActivityIndicatorOffset(DSizeZero)
{
    
}

CAActivityIndicatorView::~CAActivityIndicatorView()
{
    CC_SAFE_RELEASE(m_pImageView);
    CC_SAFE_RELEASE(m_pBackView);
}

CAActivityIndicatorView* CAActivityIndicatorView::create()
{
    CAActivityIndicatorView* view = new CAActivityIndicatorView();
    if (view && view->init()) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CAActivityIndicatorView* CAActivityIndicatorView::createWithFrame(const DRect& rect)
{
    CAActivityIndicatorView* view = new CAActivityIndicatorView();
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CAActivityIndicatorView* CAActivityIndicatorView::createWithCenter(const DRect& rect)
{
    CAActivityIndicatorView* view = new CAActivityIndicatorView();
    if (view && view->initWithCenter(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CAActivityIndicatorView* CAActivityIndicatorView::createWithLayout(const CrossApp::DLayout &layout)
{
    CAActivityIndicatorView* view = new CAActivityIndicatorView();
    if (view && view->initWithLayout(layout)){
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}


bool CAActivityIndicatorView::init()
{
    if (!CAView::init())
    {
        return false;
    }
    this->CAView::setColor(CAColor4B::CLEAR);
    m_style = CAActivityIndicatorView::Style::GrayLarge;
    
    return true;
}

void CAActivityIndicatorView::onEnter()
{
    CAView::onEnter();
    
    startAnimating();
}

void CAActivityIndicatorView::onExit()
{
    CAView::onExit();
    
    stopAnimating();
}

void CAActivityIndicatorView::setStyle(CAActivityIndicatorView::Style style)
{
    m_style = style;
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CAActivityIndicatorView");
    if (m_style != CAActivityIndicatorView::Style::Image)
    {
        DRect center = this->getBounds();
        center.origin = center.size/2;
        CAImage* image = NULL;
        
        switch (m_style)
        {
            case CAActivityIndicatorView::Style::Gray:
            {
                image = CAImage::create(map.at("image_gray"));
                center.size = DSize(40, 40);
            }
                break;
            case CAActivityIndicatorView::Style::White:
            {
                image = CAImage::create(map.at("image_white"));
                center.size = DSize(40, 40);
            }                break;
            case CAActivityIndicatorView::Style::GrayLarge:
            {
                image = CAImage::create(map.at("image_gray"));
                center.size = DSize(68, 68);
            }
                break;
            case CAActivityIndicatorView::Style::WhiteLarge:
            {
                image = CAImage::create(map.at("image_white"));
                center.size = DSize(68, 68);
            }
                break;
            
            default:
                break;
        }
        
        this->setTimesOneCycle(m_nTimesOneCycle);
        
        CAImageView* imageView = CAImageView::createWithCenter(center);
        imageView->setImage(image);
        this->insertSubview(imageView, 1);
        
        this->removeSubview(m_pImageView);
        CC_SAFE_RETAIN(imageView);
        CC_SAFE_RELEASE_NULL(m_pImageView);
        m_pImageView = imageView;
    }
}

void CAActivityIndicatorView::setTimesOneCycle(float times)
{
    m_vRotate.clear();
    m_nTimesOneCycle = times;
    for(int i=0; i<m_nTimesOneCycle; i++)
    {
        float rotate = i * 360/m_nTimesOneCycle;
        m_vRotate.push_back(rotate);
    }
    m_duration = m_fCycleTime / m_nTimesOneCycle;
}

void CAActivityIndicatorView::setCycleTime(float time)
{
    m_fCycleTime = time;
    m_duration = m_fCycleTime / m_nTimesOneCycle;
}

void CAActivityIndicatorView::startAnimating()
{
    m_bStopAnimation = false;
    m_fLoadingTime = 0.0f;
    m_animationIndex = 0;
    this->setVisible(true);
    CAScheduler::getScheduler()->schedule(schedule_selector(CAActivityIndicatorView::animation), this, 1/60.0f);
}

void CAActivityIndicatorView::stopAnimating()
{
    m_bStopAnimation = true;
}

bool CAActivityIndicatorView::isAnimating()
{
    return CAScheduler::getScheduler()->isScheduled(schedule_selector(CAActivityIndicatorView::animation), this);
}

void CAActivityIndicatorView::onCancel(const std::function<void()>& callBack)
{
    m_pCallFunc = callBack;
}

void CAActivityIndicatorView::animation(float dt)
{
    m_fLoadingTime += dt;
    if (m_pImageView)
    {
        int tmp = m_fLoadingTime/m_duration + 0.5;
        tmp = tmp%m_nTimesOneCycle;
        m_pImageView->setRotation(m_vRotate.at(tmp));
    }
    
    do
    {
        CC_BREAK_IF(m_fLoadingTime < m_fLoadingMinTime);
        CC_BREAK_IF(m_bStopAnimation == false);
        CAScheduler::getScheduler()->unschedule(schedule_selector(CAActivityIndicatorView::animation), this);
        this->setVisible(false);
        if (m_pCallFunc)
        {
            m_pCallFunc();
        }
    }
    while (0);
}

void CAActivityIndicatorView::setActivityIndicatorOffset(const DSize& offset)
{
    m_obActivityIndicatorOffset = offset;
    
    if (m_pImageView)
    {
        m_pImageView->setCenterOrigin(m_obContentSize/2 + offset);
    }
}

void CAActivityIndicatorView::setActivityIndicatorView(CrossApp::CAView *var)
{
    this->removeSubview(m_pImageView);
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pImageView);
    m_pImageView = var;
    if (m_pImageView)
    {
        m_pImageView->setCenterOrigin(m_obContentSize/2 + m_obActivityIndicatorOffset);
        this->insertSubview(m_pImageView, 1);
    }
    
    this->setTimesOneCycle(60);
}

CAView* CAActivityIndicatorView::getActivityIndicatorView()
{
    return m_pImageView;
}

void CAActivityIndicatorView::setActivityBackView(CrossApp::CAView *var)
{
    this->removeSubview(m_pBackView);
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pBackView);
    m_pBackView = var;
    
    if (m_pBackView)
    {
        if (m_pBackView->getLayoutType() <= 1)
        {
            m_pBackView->setCenterOrigin(m_obContentSize/2);
        }
        this->insertSubview(m_pBackView, 0);
    }
}

CAView* CAActivityIndicatorView::getActivityBackView()
{
    return m_pBackView;
}

void CAActivityIndicatorView::setContentSize(const DSize & var)
{
    CAView::setContentSize(var);
    
    this->setStyle(m_style);
    
    if (m_pBackView)
    {
        m_pBackView->setCenterOrigin(m_obContentSize/2);
    }
    if (m_pImageView)
    {
        m_pImageView->setCenterOrigin(m_obContentSize/2 + m_obActivityIndicatorOffset);
    }
}

NS_CC_END

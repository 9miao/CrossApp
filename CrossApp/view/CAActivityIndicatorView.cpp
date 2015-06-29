//
//  CAActivityIndicatorView.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-18.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#include "CAActivityIndicatorView.h"
#include "view/CADrawingPrimitives.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "CAScale9ImageView.h"

NS_CC_BEGIN

CAActivityIndicatorView::CAActivityIndicatorView()
: m_bStopAnimation(false)
, m_hidesWhenStopped(true)
, m_style(CAActivityIndicatorViewStyleWhite)
, m_color(ccc4(255, 255, 255, 255))
, m_duration(0.1f)
, m_pImageView(NULL)
, m_pBackView(NULL)
, m_fLoadingMinTime(0.0f)
, m_fLoadingTime(0.0f)
, m_pTarget(NULL)
, m_pCallFunc(NULL)
, m_nTimesOneCycle(12)
, m_fCycleTime(1.0f)
{
    
}

CAActivityIndicatorView::~CAActivityIndicatorView()
{
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

CAActivityIndicatorView* CAActivityIndicatorView::createWithFrame(const CCRect& rect)
{
    CAActivityIndicatorView* view = new CAActivityIndicatorView();
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
    } else {
        CC_SAFE_DELETE(view);
    }
    return view;
}

CAActivityIndicatorView* CAActivityIndicatorView::createWithCenter(const CCRect& rect)
{
    CAActivityIndicatorView* view = new CAActivityIndicatorView();
    if (view && view->initWithCenter(rect)) {
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
    this->CAView::setColor(CAColor_clear);
    m_style = CAActivityIndicatorViewStyleGray;
    
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
}

void CAActivityIndicatorView::visit()
{
    CAView::visit();
}

void CAActivityIndicatorView::setStyle(CAActivityIndicatorViewStyle style)
{
    m_style = style;
    
    if (m_style != CAActivityIndicatorViewStyleImage)
    {
        CCRect center = getBounds();
        center.origin = center.size/2;
        
        CC_SAFE_RELEASE(m_pImageView);
        this->removeSubview(m_pImageView);
        CC_SAFE_RELEASE(m_pBackView);
        this->removeSubview(m_pBackView);
        switch (m_style)
        {
            case CAActivityIndicatorViewStyleGray:
                m_pImageView = CAImageView::createWithImage(CAImage::create("source_material/loading_black.png"));
                break;
            case CAActivityIndicatorViewStyleWhite:
                m_pImageView = CAImageView::createWithImage(CAImage::create("source_material/loading_write.png"));
                break;
            case CAActivityIndicatorViewStyleWhiteLarge:
            {
                m_pImageView = CAImageView::createWithImage(CAImage::create("source_material/loading_write_big.png"));
                break;
            }
            case CAActivityIndicatorViewStyleWGrayLarge:
            {
                m_pImageView = CAImageView::createWithImage(CAImage::create("source_material/loading_black_big.png"));
                break;
            }
            default:
                break;
        }
        setTimesOneCycle(m_nTimesOneCycle);
        
        if (m_pImageView)
        {
            m_pImageView->setCenterOrigin(getBounds().size/2);
            this->insertSubview(m_pImageView, 1);
        }
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
    CAScheduler::schedule(schedule_selector(CAActivityIndicatorView::animation), this, 1/60.0f);
}

void CAActivityIndicatorView::stopAnimating()
{
    m_bStopAnimation = true;
}

bool CAActivityIndicatorView::isAnimating()
{
    return CAScheduler::isScheduled(schedule_selector(CAActivityIndicatorView::animation), this);
}

void CAActivityIndicatorView::setTargetOnCancel(CAObject* target, SEL_CallFunc callBack)
{
    m_pTarget = target;
    m_pCallFunc = callBack;
}

void CAActivityIndicatorView::draw()
{
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
        CAScheduler::unschedule(schedule_selector(CAActivityIndicatorView::animation), this);
        this->setVisible(false);
        if (m_pTarget && m_pCallFunc)
        {
            ((CAObject*)m_pTarget->*m_pCallFunc)();
        }
    }
    while (0);
}

void CAActivityIndicatorView::setActivityIndicatorView(CrossApp::CAView *var)
{
    m_style = CAActivityIndicatorViewStyleImage;
    
    CC_SAFE_RELEASE(m_pImageView);
    this->removeSubview(m_pImageView);
    m_pImageView = var;
    if (m_pImageView)
    {
        m_pImageView->setCenterOrigin(getBounds().size/2);
        this->insertSubview(m_pImageView, 1);
    }
}

CAView* CAActivityIndicatorView::getActivityIndicatorView()
{
    return m_pImageView;
}

void CAActivityIndicatorView::setActivityBackView(CrossApp::CAView *var)
{
    if (m_pBackView) {
        removeSubview(m_pBackView);
    }
    CC_SAFE_RELEASE(m_pBackView);
    m_pBackView = var;
    
    if (m_pBackView) {
        m_pBackView->setCenterOrigin(getBounds().size/2);
        this->insertSubview(m_pBackView, 0);
    }
}

CAView* CAActivityIndicatorView::getActivityBackView()
{
    return m_pBackView;
}

void CAActivityIndicatorView::setContentSize(const CCSize & var)
{
    CAView::setContentSize(var);
    this->setStyle(m_style);
    if (m_pBackView)
    {
        m_pBackView->setCenterOrigin(getBounds().size/2);
    }
    if (m_pImageView)
    {
        m_pImageView->setCenterOrigin(getBounds().size/2);
    }
}

NS_CC_END
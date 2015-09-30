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

NS_CC_BEGIN

CAActivityIndicatorView::CAActivityIndicatorView()
: m_bStopAnimation(false)
, m_hidesWhenStopped(true)
, m_style(CAActivityIndicatorViewStyleGrayLarge)
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

bool CAActivityIndicatorView::init()
{
    if (!CAView::init())
    {
        return false;
    }
    this->CAView::setColor(CAColor_clear);
    m_style = CAActivityIndicatorViewStyleGrayLarge;
    
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
        this->removeSubview(m_pImageView);
        CC_SAFE_RELEASE(m_pImageView);
        
        this->removeSubview(m_pBackView);
        CC_SAFE_RELEASE(m_pBackView);
        
        DRect center = getBounds();
        center.origin = center.size/2;
        CAImage* image = NULL;
        
        switch (m_style)
        {
            case CAActivityIndicatorViewStyleGray:
            {
                image = CAImage::create("source_material/loading_black.png");
                center.size = DSize(40, 40);
            }
                break;
            case CAActivityIndicatorViewStyleWhite:
            {
                image = CAImage::create("source_material/loading_write.png");
                center.size = DSize(40, 40);
            }                break;
            case CAActivityIndicatorViewStyleGrayLarge:
            {
                image = CAImage::create("source_material/loading_black.png");
                center.size = DSize(68, 68);
            }
                break;
            case CAActivityIndicatorViewStyleWhiteLarge:
            {
                image = CAImage::create("source_material/loading_write.png");
                center.size = DSize(68, 68);
            }
                break;
            
            default:
                break;
        }
        setTimesOneCycle(m_nTimesOneCycle);
        
        
        CAImageView* imageView = CAImageView::createWithCenter(center);
        imageView->retain();
        imageView->setImage(image);
        this->insertSubview(imageView, 1);
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
    
    this->removeSubview(m_pImageView);
    CC_SAFE_RELEASE_NULL(m_pImageView);
    CC_SAFE_RETAIN(var);
    m_pImageView = var;
    if (m_pImageView)
    {
        m_pImageView->setCenterOrigin(getBounds().size/2);
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
    CC_SAFE_RELEASE_NULL(m_pBackView);
    CC_SAFE_RETAIN(var);
    m_pBackView = var;
    
    if (m_pBackView)
    {
        m_pBackView->setCenterOrigin(getBounds().size/2);
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
        m_pBackView->setCenterOrigin(getBounds().size/2);
    }
    if (m_pImageView)
    {
        m_pImageView->setCenterOrigin(getBounds().size/2);
    }
}

NS_CC_END
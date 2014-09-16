//
//  CAActivityIndicatorView.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-18.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#include "CAActivityIndicatorView.h"
#include "draw_nodes/CCDrawingPrimitives.h"
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
    setStyle(CAActivityIndicatorViewStyleGray);
    
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
    
    CCRect center = getBounds();
    if (center.size.width > center.size.height)
    {
        center.size.width = center.size.height;
    }
    else
    {
        center.size.height = center.size.width;
    }
    center.origin.x = center.size.width/2;
    center.origin.y = center.size.height/2;
    
    float radius_inside, radius_outside;
    
    switch (m_style)
    {
        case CAActivityIndicatorViewStyleGray:
            radius_outside = _px(34);
            radius_inside = _px(15);
            m_color = ccc4(127, 127, 127, 60);
            break;
        case CAActivityIndicatorViewStyleWhite:
            radius_outside = _px(20);
            radius_inside = _px(8.5);
            m_color = ccc4(255, 255, 255, 127);
            break;
        case CAActivityIndicatorViewStyleWhiteLarge:
            radius_outside = _px(34);
            radius_inside = _px(12);
            m_color = ccc4(255, 255, 255, 127);
            break;
            
        default:
            break;
    }
    
    //
    //       90
    //    120   60
    //  150       30
    // 180           0(begin, anti-clock-wise)
    //  210       330
    //    240   300
    //       270
    for (int angle=0, index=0; angle<360; angle+=30, index++)
    {
        
        float radian = 2 * M_PI / 360 * angle;
        
        m_vertex[index][0] = center.origin;
        m_vertex[index][1] = center.origin;
        m_vertex[index][0].x += radius_inside * cos(radian);
        m_vertex[index][0].y += radius_inside * sin(radian);
        m_vertex[index][1].x += radius_outside * cos(radian);
        m_vertex[index][1].y += radius_outside * sin(radian);
        
//        printf("%f, %f, %f, %f \n", 
//               m_vertex[index][0].x,
//               m_vertex[index][0].y,
//               m_vertex[index][1].x,
//               m_vertex[index][1].y);
    }
    
}

void CAActivityIndicatorView::startAnimating()
{
    m_bStopAnimation = false;
    m_fLoadingTime = 0.0f;
    m_animationIndex = 0;
    this->setVisible(true);
    CAScheduler::schedule(schedule_selector(CAActivityIndicatorView::animation), this, m_duration);
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
    CAView::draw();
    
    CC_RETURN_IF(m_style == CAActivityIndicatorViewStyleImage);
    
    float start_alpha, end_alpha;
    
    switch (m_style)
    {
        case CAActivityIndicatorViewStyleGray:
            start_alpha = 120;
            end_alpha = 60;
            break;
        case CAActivityIndicatorViewStyleWhite:
            start_alpha = 255;
            end_alpha = 120;
            break;
        case CAActivityIndicatorViewStyleWhiteLarge:
            start_alpha = 255;
            end_alpha = 120;
            break;
            
        default:
            start_alpha = 0;
            end_alpha = 0;
            break;
    }
    
    float step = (start_alpha - end_alpha)/5;
    float alpha = start_alpha;
    int index = m_animationIndex;
    int count = 12;
    while (count--)
    {
        glLineWidth( 5.0f * CAApplication::getApplication()->getAdaptationRatio() );
        ccDrawColor4B(m_color.r, m_color.g, m_color.b, alpha);
        ccDrawLine(m_vertex[index][0], m_vertex[index][1]);
        
        if (alpha > end_alpha)
        {
            alpha -= step;
            if (alpha < step)
            {
                alpha = step;
            }
        }
        
        index++;
        if (index == 12) {
            index = 0;
        }
    }
}

void CAActivityIndicatorView::animation(float dt)
{
    
    if (m_style == CAActivityIndicatorViewStyleImage && m_pImageView)
    {
        float rotate = m_pImageView->getRotation();
        rotate += 10;
        if (rotate == 360)
        {
            rotate = 0;
        }
        m_pImageView->setRotation(rotate);
    }
    else
    {
        m_animationIndex--;
        if (m_animationIndex < 0)
        {
            m_animationIndex = 12 - 1;
        }
    }
    m_fLoadingTime += dt;
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

    updateDraw();
}

void CAActivityIndicatorView::setActivityIndicatorView(CrossApp::CAView *var)
{
    m_style = CAActivityIndicatorViewStyleImage;
    
    m_duration = 1 / 30.0f;
    
    CC_SAFE_RETAIN(var);
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
    CC_SAFE_RETAIN(m_pBackView);
    
    if (m_pBackView) {
        m_pBackView->setCenterOrigin(getBounds().size/2);
        this->insertSubview(m_pBackView, 0);
    }
}

CAView* CAActivityIndicatorView::getActivityBackView()
{
    return m_pBackView;
}

NS_CC_END
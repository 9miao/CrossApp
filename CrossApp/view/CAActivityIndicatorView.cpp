//
//  CAActivityIndicatorView.cpp
//  CrossApp
//
//  Created by dai xinping on 14-8-18.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#include "CAActivityIndicatorView.h"
#include "CCDrawingPrimitives.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"

NS_CC_BEGIN

CAActivityIndicatorView::CAActivityIndicatorView()
: m_animating(false)
, m_hidesWhenStopped(true)
, m_style(CAActivityIndicatorViewStyleWhite)
, m_color(ccc4(255, 255, 255, 255))
, m_duration(0.1f)
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
    if (!CAView::init()) {
        return false;
    }
    
    setStyle(CAActivityIndicatorViewStyleGray);
    
    return true;
}

bool CAActivityIndicatorView::initWithCenter(const CrossApp::CCRect &rect)
{
    if (!CAView::initWithCenter(rect)) {
        return false;
    }
    
    setStyle(CAActivityIndicatorViewStyleGray);
    
    return true;
}

bool CAActivityIndicatorView::initWithFrame(const CrossApp::CCRect &rect)
{
    if (!CAView::initWithFrame(rect)) {
        return false;
    }
    
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
    if (center.size.width > center.size.height) {
        center.size.width = center.size.height;
    } else {
        center.size.height = center.size.width;
    }
    center.origin.x = center.size.width/2;
    center.origin.y = center.size.height/2;
    
    float radius_inside, radius_outside;
    
    switch (m_style) {
        case CAActivityIndicatorViewStyleGray:
            radius_outside = 34 * CAApplication::getApplication()->getAdaptationRatio();
            radius_inside = 15 * CAApplication::getApplication()->getAdaptationRatio();
            m_color = ccc4(127, 127, 127, 60);
            break;
        case CAActivityIndicatorViewStyleWhite:
            radius_outside = 34 * CAApplication::getApplication()->getAdaptationRatio();
            radius_inside = 15 * CAApplication::getApplication()->getAdaptationRatio();
            m_color = ccc4(255, 255, 255, 127);
            break;
        case CAActivityIndicatorViewStyleWhiteLarge:
            radius_outside = 50 * CAApplication::getApplication()->getAdaptationRatio();
            radius_inside = 24 * CAApplication::getApplication()->getAdaptationRatio();
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
    for (int angle=0, index=0; angle<360; angle+=30, index++) {
        
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
    m_animating = true;
    m_animationIndex = 0;
    CAScheduler::schedule(schedule_selector(CAActivityIndicatorView::animation), this, 0.1f);
}

void CAActivityIndicatorView::stopAnimating()
{
    m_animating = false;
    CAScheduler::unschedule(schedule_selector(CAActivityIndicatorView::animation), this);
}

bool CAActivityIndicatorView::isAnimating()
{
    return m_animating;
}

void CAActivityIndicatorView::draw()
{
//    CAView::draw();
//    CC_NODE_DRAW_SETUP();
//    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

    
    float start_alpha, end_alpha;
    
    switch (m_style) {
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
            break;
    }
    
    float step = (start_alpha - end_alpha)/5;
    float alpha = start_alpha;
    int index = m_animationIndex;
    int count = 12;
    while (count--) {
        glLineWidth( 5.0f * CAApplication::getApplication()->getAdaptationRatio() );
        ccDrawColor4B(m_color.r, m_color.g, m_color.b, alpha);
        ccDrawLine(m_vertex[index][0], m_vertex[index][1]);
        
        if (alpha > end_alpha) {
            alpha -= step;
            if (alpha < step) {
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
    m_animationIndex--;
    if (m_animationIndex < 0) {
        m_animationIndex = 12 - 1;
    }
    updateDraw();
}

NS_CC_END
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
#include "CCActionInterval.h"

NS_CC_BEGIN

CAActivityIndicatorView::CAActivityIndicatorView()
: m_animating(false)
, m_hidesWhenStopped(true)
, m_style(CAActivityIndicatorViewStyleWhite)
, m_color(ccc4(255, 255, 255, 255))
, m_duration(0.1f)
, m_image(NULL)
, m_imageView(NULL)
{
    
}

CAActivityIndicatorView::~CAActivityIndicatorView()
{
    CC_SAFE_RELEASE(m_image);
    CC_SAFE_RELEASE(m_imageView);
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
    this->CAView::setColor(CAColor_clear);
    setStyle(CAActivityIndicatorViewStyleGray);
    
    return true;
}

bool CAActivityIndicatorView::initWithCenter(const CrossApp::CCRect &rect)
{
    if (!CAView::initWithCenter(rect)) {
        return false;
    }
    
    return true;
}

bool CAActivityIndicatorView::initWithFrame(const CrossApp::CCRect &rect)
{
    if (!CAView::initWithFrame(rect)) {
        return false;
    }
    
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
    CAView::draw();
    
    if (m_style == CAActivityIndicatorViewStyleImage) {
        return;
    }
    
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
            start_alpha = 0;
            end_alpha = 0;
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
    if (m_style == CAActivityIndicatorViewStyleImage && m_imageView) {
        float rotate = m_imageView->getRotation();
        rotate += 20;
        if (rotate == 360) {
            rotate = 0;
        }
        m_imageView->setRotation(rotate);
    } else {
        m_animationIndex--;
        if (m_animationIndex < 0) {
            m_animationIndex = 12 - 1;
        }
    }
    updateDraw();
}

void CAActivityIndicatorView::setImage(CrossApp::CAImage *image)
{
    m_style = CAActivityIndicatorViewStyleImage;
    CC_SAFE_RELEASE(m_image);
    m_image = image;
    CC_SAFE_RETAIN(m_image);
    
    if (m_imageView) {
        removeSubview(m_imageView);
        m_imageView->release();
    }
    
//    CAScale9ImageView* view = CAScale9ImageView::createWithImage(image);
    CAImageView* view = CAImageView::createWithImage(image);
//    view->setFrame(CCRect(0, 0, getBounds().size.width, getBounds().size.height));
    view->setCenterOrigin(CCPoint(getBounds().size.width/2, getBounds().size.height/2));
    addSubview(view);

    m_imageView = view;
    CC_SAFE_RETAIN(m_imageView);
}

NS_CC_END
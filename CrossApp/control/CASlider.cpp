//
//  CASlider.m
//  CrossApp 
//
//  Created by juguanhui on 14-5-30.
//  Copyright (c) 2014年 CrossApp-x. All rights reserved.
//

#include "CASlider.h"
#include "view/CAImageView.h"
#include "dispatcher/CATouch.h"
#include "view/CAScale9ImageView.h"
#include "support/CAPointExtension.h"
#include "view/CARenderImage.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

CASlider::CASlider()
    : CAControl()
    , m_bTouchClick(false)
    , m_fValue(0.0f)
    , m_fMinValue(0.0f)
    , m_fMaxValue(1.0f)
    , m_fTrackHeight(3.0f)
    , m_pMinTrackTintImage(NULL)
    , m_pMaxTrackTintImage(NULL)
    , m_pThumbTintImage(NULL)
    , m_pMinTrackTintImageView(NULL)
    , m_pMaxTrackTintImageView(NULL)
    , m_pThumbTintImageView(NULL)
{
    this->setPriorityScroll(true);
    this->setVerticalScrollEnabled(false);
}

CASlider::~CASlider()
{
    CC_SAFE_RELEASE_NULL(m_pMinTrackTintImage);
	CC_SAFE_RELEASE_NULL(m_pMaxTrackTintImage);
	CC_SAFE_RELEASE_NULL(m_pThumbTintImage);
}

void CASlider::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

void CASlider::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    
    if (NULL == m_pMinTrackTintImage)
    {
        this->setMinTrackTintImage(CAImage::create("source_material/btn_square_highlighted.png"));
    }
    
    if (NULL == m_pMaxTrackTintImage)
    {
        this->setMaxTrackTintImage(CAImage::create("source_material/btn_square_disabled.png"));
    }
    
    if (NULL == m_pThumbTintImage)
    {
        this->setThumbTintImage(CAImage::create("source_material/slider_indicator.png"));
    }
    
    const DSize size = this->getBounds().size;
    
    if (m_fTrackHeight > size.height)
    {
        m_fTrackHeight = size.height;
    }
    
    if (NULL == m_pMinTrackTintImageView)
    {
        m_pMinTrackTintImageView = CAScale9ImageView::createWithImage(m_pMinTrackTintImage);
    }
    
    if (m_pMinTrackTintImageView && m_pMinTrackTintImageView->getSuperview() == NULL)
    {
        this->addSubview(m_pMinTrackTintImageView);
    }
    
    if (NULL == m_pThumbTintImageView)
    {
        m_pThumbTintImageView = CAImageView::createWithImage(m_pThumbTintImage);
        const DSize size = this->getBounds().size;
        m_pThumbTintImageView->setFrame(DRect(0, 0, size.height, size.height));
    }
    if (m_pThumbTintImageView && m_pThumbTintImageView->getSuperview() == NULL)
    {
        this->addSubview(m_pThumbTintImageView);
    }
    
    if (NULL == m_pMaxTrackTintImageView)
    {
        m_pMaxTrackTintImageView = CAScale9ImageView::createWithImage(m_pMaxTrackTintImage);
    }
    if (m_pMaxTrackTintImageView && m_pMaxTrackTintImageView->getSuperview() == NULL)
    {
        this->addSubview(m_pMaxTrackTintImageView);
    }
    
    this->layoutSubViews();
}

CASlider* CASlider::createWithFrame(const DRect& rect)
{
    CASlider* slider = new CASlider();
    
    if (slider && slider->initWithFrame(rect))
    {
        slider->autorelease();
        return slider;
    }
    
    CC_SAFE_DELETE(slider);
    return NULL;
}

CASlider* CASlider::createWithCenter(const DRect& rect)
{
    CASlider* slider = new CASlider();
    
    if (slider && slider->initWithCenter(rect))
    {
        slider->autorelease();
        return slider;
    }
    
    CC_SAFE_DELETE(slider);
    return NULL;
}

bool CASlider::initWithFrame(const DRect& rect)
{
    if (!CAControl::init())
    {
        return false;
    }

    this->setFrame(rect);
    return true;
}

bool CASlider::initWithCenter(const DRect& rect)
{
    if (!CAControl::init())
    {
        return false;
    }

    this->setCenter(rect);
    return true;
}

void CASlider::layoutSubViews()
{
    if (m_fValue >= m_fMinValue
        && m_fValue <= m_fMaxValue
        && m_pThumbTintImageView
        && m_pMinTrackTintImageView
        && m_pMaxTrackTintImageView)
    {
        const DSize size = this->getBounds().size;
        const DSize thumbSize = m_pThumbTintImageView->getBounds().size;
        const float halfThumbWidth = thumbSize.width / 2;
        const float totalWidth = size.width;
        const float percent = m_fValue / (m_fMaxValue - m_fMinValue);
        const float centerX = ((totalWidth - thumbSize.width) * percent) + halfThumbWidth;
        const float trackOriginY = (size.height - m_fTrackHeight) / 2;
        const float minRight = centerX - halfThumbWidth;
        const float maxLeft = centerX + halfThumbWidth;
        
        m_pThumbTintImageView->setCenterOrigin(DPoint(centerX, size.height / 2));
        m_pMinTrackTintImageView->setFrame(DRect(0, trackOriginY, minRight, m_fTrackHeight));
        m_pMaxTrackTintImageView->setFrame(DRect(maxLeft, trackOriginY, size.width - maxLeft, m_fTrackHeight));
    }
}

void CASlider::setValue(float value)
{
    if (m_fValue != value)
    {
        m_fValue = value;
        this->layoutSubViews();
    }
}

void CASlider::setMinValue(float minValue)
{
    if (m_fMinValue != minValue)
    {
        m_fMinValue = minValue;
        this->layoutSubViews();
    }
}

void CASlider::setMaxValue(float maxValue)
{
    if (m_fMaxValue != maxValue)
    {
        m_fMaxValue = maxValue;
        this->layoutSubViews();
    }
}

void CASlider::setTrackHeight(float trackHeight)
{
    if (m_fTrackHeight != trackHeight)
    {
        m_fTrackHeight = trackHeight;
        this->layoutSubViews();
    }
}

void CASlider::setMinTrackTintImage(CAImage* image)
{
    if (m_pMinTrackTintImage != image)
    {
        CC_SAFE_RETAIN(image);
        CC_SAFE_RELEASE(m_pMinTrackTintImage);
        m_pMinTrackTintImage = image;
        if (m_pMinTrackTintImageView)
        {
            ((CAScale9ImageView*)m_pMinTrackTintImageView)->setImage(m_pMinTrackTintImage);
        }
        this->layoutSubViews();
    }
}

void CASlider::setMaxTrackTintImage(CAImage* image)
{
    if (m_pMaxTrackTintImage != image)
    {
        CC_SAFE_RETAIN(image);
        CC_SAFE_RELEASE(m_pMaxTrackTintImage);
        m_pMaxTrackTintImage = image;
        if (m_pMaxTrackTintImageView)
        {
            ((CAScale9ImageView*)m_pMaxTrackTintImageView)->setImage(m_pMaxTrackTintImage);
        }
        this->layoutSubViews();
    }
}

void CASlider::setThumbTintImage(CAImage* image)
{
    if (m_pThumbTintImage != image)
    {
        CC_SAFE_RETAIN(image);
        CC_SAFE_RELEASE(m_pThumbTintImage);
        m_pThumbTintImage = image;
        if (m_pThumbTintImageView)
        {
            ((CAScale9ImageView*)m_pThumbTintImageView)->setImage(m_pThumbTintImage);
            const DSize size = this->getBounds().size;
            m_pThumbTintImageView->setFrame(DRect(0, 0, size.height, size.height));
        }
        this->layoutSubViews();
    }
}

bool CASlider::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
	m_bTouchClick = true;
    return m_bTouchClick;
}

void CASlider::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (!this->isTouchClick())
        return;
    
    DRect bounds = getBounds();
    float value = (m_fMaxValue - m_fMinValue) * (point.x / bounds.size.width);
    value = (point.x <= 0) ? m_fMinValue : ((point.x >= bounds.size.width) ? m_fMaxValue : value);
    this->setValue(value);
    
    if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, point);
    }
}

void CASlider::ccTouchEnded(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    if (!this->isTouchClick())
        return;
    
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    DRect bounds = getBounds();
    if (bounds.containsPoint(point))
    {
        float value = (m_fMaxValue - m_fMinValue) * (point.x / bounds.size.width);
        this->setValue(value);
        if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged])
        {
            ((CAObject *)m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, point);
        }
    }
    
    if (m_pTarget[CAControlEventTouchUpInSide] && m_selTouch[CAControlEventTouchUpInSide])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchUpInSide]->*m_selTouch[CAControlEventTouchUpInSide])(this, point);
    }
}

void CASlider::addTarget(CAObject* target, SEL_CAControl selector)
{
    this->addTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASlider::addTargetForTouchUpSide(CAObject* target, SEL_CAControl selector)
{
    this->addTarget(target, selector, CAControlEventTouchUpInSide);
}

void CASlider::removeTarget(CAObject* target, SEL_CAControl selector)
{
    this->removeTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASlider::setContentSize(const DSize & var)
{
    CAControl::setContentSize(DSize(var.width, var.height));
}

NS_CC_END


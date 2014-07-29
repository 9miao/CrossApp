//
//  CASlider.m
//  CrossAppx
//
//  Created by juguanhui on 14-5-30.
//  Copyright (c) 2014年 CrossApp-x. All rights reserved.
//

#include "CASlider.h"
#include "view/CAImageView.h"
#include "dispatcher/CATouch.h"
#include "view/CAScale9ImageView.h"
#include "support/CCPointExtension.h"
#include "view/CARenderImage.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

CASlider::CASlider()
    : CAControl()
    , m_bTouchClick(false)
    , m_value(0.0f)
    , m_minValue(0.0f)
    , m_maxValue(1.0f)
    , m_trackHeight(3.0f)
    , m_pMinTrackTintImage(NULL)
    , m_pMaxTrackTintImage(NULL)
    , m_pThumbTintImage(NULL)
    , m_pMinTrackTintImageView(NULL)
    , m_pMaxTrackTintImageView(NULL)
    , m_pThumbTintImageView(NULL)
{
    
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
    
    const CCSize size = this->getBounds().size;
    
    if (m_trackHeight > size.height)
    {
        m_trackHeight = size.height;
    }
    
    if (NULL == m_pMinTrackTintImageView)
    {
        m_pMinTrackTintImageView = CAScale9ImageView::createWithImage(m_pMinTrackTintImage);
    }
    if (m_pMinTrackTintImageView)
    {
        this->addSubview(m_pMinTrackTintImageView);
    }
    
    if (NULL == m_pThumbTintImageView)
    {
        m_pThumbTintImageView = CAScale9ImageView::createWithImage(m_pThumbTintImage);
    }
    if (m_pThumbTintImageView)
    {
        this->addSubview(m_pThumbTintImageView);
    }
    
    if (NULL == m_pMaxTrackTintImageView)
    {
        m_pMaxTrackTintImageView = CAScale9ImageView::createWithImage(m_pMaxTrackTintImage);
    }
    if (m_pMaxTrackTintImageView)
    {
        this->addSubview(m_pMaxTrackTintImageView);
    }
    
    this->layoutSubViews();
}

CASlider* CASlider::createWithFrame(const CCRect& rect)
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

CASlider* CASlider::createWithCenter(const CCRect& rect)
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

bool CASlider::initWithFrame(const CCRect& rect)
{
    if (!CAControl::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    this->setFrame(rect);
    return true;
}

bool CASlider::initWithCenter(const CCRect& rect)
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
    if (m_value >= m_minValue
        && m_value <= m_maxValue
        && m_pThumbTintImageView
        && m_pMinTrackTintImageView
        && m_pMaxTrackTintImageView)
    {
        const CCSize size = this->getBounds().size;
        const CCSize thumbSize = m_pThumbTintImageView->getBounds().size;
        const float halfThumbWidth = thumbSize.width / 2;
        const float totalWidth = size.width;
        const float percent = m_value / (m_maxValue - m_minValue);
        const float centerX = ((totalWidth - thumbSize.width) * percent) + halfThumbWidth;
        const float trackOriginY = (size.height - m_trackHeight) / 2;
        const float minRight = centerX - halfThumbWidth;
        const float maxLeft = centerX + halfThumbWidth;
        
        m_pThumbTintImageView->setCenterOrigin(CCPoint(centerX, size.height / 2));
        m_pMinTrackTintImageView->setFrame(CCRect(0, trackOriginY, minRight, m_trackHeight));
        m_pMaxTrackTintImageView->setFrame(CCRect(maxLeft, trackOriginY, size.width - maxLeft, m_trackHeight));
    }
}

void CASlider::setValue(float value)
{
    if (m_value != value)
    {
        m_value = value;
        this->layoutSubViews();
    }
}

void CASlider::setMinValue(float minValue)
{
    if (m_minValue != minValue)
    {
        m_minValue = minValue;
        this->layoutSubViews();
    }
}

void CASlider::setMaxValue(float maxValue)
{
    if (m_maxValue != maxValue)
    {
        m_maxValue = maxValue;
        this->layoutSubViews();
    }
}

void CASlider::setTrackHeight(float trackHeight)
{
    if (m_trackHeight != trackHeight)
    {
        m_trackHeight = trackHeight;
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
            ((CAScale9ImageView*)m_pMinTrackTintImageView)->setImage(m_pMaxTrackTintImage);
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
            const CCSize size = this->getBounds().size;
            ((CAScale9ImageView*)m_pMinTrackTintImageView)->setImage(m_pThumbTintImage);
            m_pThumbTintImageView->setBounds(CCRect(0, 0, size.height, size.height));
        }
        this->layoutSubViews();
    }
}

bool CASlider::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (!this->isTouchEnabled())
        return false;
    
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
	m_bTouchClick = true;
    return m_bTouchClick;
}

void CASlider::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (!this->isTouchClick())
        return;
    
    CCRect bounds = getBounds();
    float value = (m_maxValue - m_minValue) * (point.x / bounds.size.width);
    value = (point.x <= 0) ? m_minValue : ((point.x >= bounds.size.width) ? m_maxValue : value);
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
    
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    CCRect bounds = getBounds();
    if (bounds.containsPoint(point))
    {
        float value = (m_maxValue - m_minValue) * (point.x / bounds.size.width);
        this->setValue(value);
        if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged])
        {
            ((CAObject *)m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, point);
        }
    }
}

void CASlider::addTarget(CAObject* target, SEL_CAControl selector)
{
    this->addTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASlider::removeTarget(CAObject* target, SEL_CAControl selector)
{
    this->removeTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASlider::setContentSize(const CCSize & var)
{
    CAControl::setContentSize(CCSize(var.width, _px(48)));
}

NS_CC_END


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
#include "basics/CAPointExtension.h"
#include "view/CARenderImage.h"
#include "basics/CAApplication.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

CASlider::CASlider()
    : CAControl()
    , m_bTouchClick(false)
    , m_fValue(0.0f)
    , m_fMinValue(0.0f)
    , m_fMaxValue(1.0f)
    , m_fTrackHeight(4.0f)
    , m_pMinTrackTintImage(nullptr)
    , m_pMaxTrackTintImage(nullptr)
    , m_pThumbTintImage(nullptr)
    , m_pMinTrackTintImageView(nullptr)
    , m_pMaxTrackTintImageView(nullptr)
    , m_pThumbTintImageView(nullptr)
{
    this->setPriorityScroll(true);
    this->setVerticalScrollEnabled(false);
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CASlider");
    int h = atoi(map.at("trackHeight").c_str());
    m_fTrackHeight = (h == 0) ? m_fTrackHeight : h;
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
}

CASlider* CASlider::create()
{
    CASlider* slider = new CASlider();
    
    if (slider && slider->init())
    {
        slider->autorelease();
        return slider;
    }
    
    CC_SAFE_DELETE(slider);
    return NULL;
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

CASlider* CASlider::createWithLayout(const CrossApp::DLayout &layout)
{
    CASlider* slider = new CASlider();
    
    if (slider && slider->initWithLayout(layout))
    {
        slider->autorelease();
        return slider;
    }
    
    CC_SAFE_DELETE(slider);
    return NULL;
}

bool CASlider::init()
{
    if (!CAControl::init())
    {
        return false;
    }
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CASlider");
    m_pMinTrackTintImageView = CAScale9ImageView::create();
    this->addSubview(m_pMinTrackTintImageView);
    this->setMinTrackTintImage(CAImage::create(map.at("minTrackTintImage")));
    
    
    m_pMaxTrackTintImageView = CAScale9ImageView::create();
    this->addSubview(m_pMaxTrackTintImageView);
    this->setMaxTrackTintImage(CAImage::create(map.at("maxTrackTintImage")));
    
    
    m_pThumbTintImageView = CAImageView::create();
    this->insertSubview(m_pThumbTintImageView, 2);
    this->setThumbTintImage(CAImage::create(map.at("thumbTintImage")));
    
    return true;
}

bool CASlider::initWithFrame(const DRect& rect)
{
    if (!this->init())
    {
        return false;
    }

    this->setFrame(rect);
    return true;
}

bool CASlider::initWithCenter(const DRect& rect)
{
    if (!this->init())
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
        && m_pMaxTrackTintImageView
        && m_pMinTrackTintImage
        && m_pMaxTrackTintImage)
    {
        float thumbDiameter = m_obContentSize.height;
        float totalWidth = m_obContentSize.width;
        float percent = (m_fValue - m_fMinValue) / (m_fMaxValue - m_fMinValue);
        float centerX = ((totalWidth - thumbDiameter) * percent) + thumbDiameter/2;
        
        m_pThumbTintImageView->setCenter(DRect(centerX, thumbDiameter / 2, thumbDiameter, thumbDiameter));
        
        DLayout minLayout;
        minLayout.vertical = DVerticalLayout_H_C(m_fTrackHeight, 0.5f);
        minLayout.horizontal.left = 0;
        minLayout.horizontal.width = MAX(centerX, m_pMinTrackTintImage->getContentSize().width);
        m_pMinTrackTintImageView->setLayout(minLayout);
        
        DLayout maxLayout;
        maxLayout.vertical = DVerticalLayout_H_C(m_fTrackHeight, 0.5f);
        maxLayout.horizontal.right = 0;
        maxLayout.horizontal.width = MAX(totalWidth - centerX, m_pMaxTrackTintImage->getContentSize().width);
        m_pMaxTrackTintImageView->setLayout(maxLayout);
        
        if (percent >= 0.5)
        {
            m_pMinTrackTintImageView->setZOrder(1);
            m_pMaxTrackTintImageView->setZOrder(0);
        }
        else
        {
            m_pMinTrackTintImageView->setZOrder(0);
            m_pMaxTrackTintImageView->setZOrder(1);
        }
    }
}

void CASlider::setValue(float value)
{
    if (m_fValue != value)
    {
        m_fValue = value;
        m_fValue = MAX(m_fValue, m_fMinValue);
        m_fValue = MIN(m_fValue, m_fMaxValue);
        if (m_bTouchClick == false)
        {
            this->layoutSubViews();
        }
    }
}

void CASlider::_setValue(float value)
{
    if (m_fValue != value)
    {
        m_fValue = value;
        m_fValue = MAX(m_fValue, m_fMinValue);
        m_fValue = MIN(m_fValue, m_fMaxValue);
        this->layoutSubViews();
    }
}

void CASlider::setMinValue(float minValue)
{
    if (m_fMinValue != minValue)
    {
        m_fMinValue = minValue;
        m_fValue = MAX(m_fValue, m_fMinValue);
        this->layoutSubViews();
    }
}

void CASlider::setMaxValue(float maxValue)
{
    if (m_fMaxValue != maxValue)
    {
        m_fMaxValue = maxValue;
        m_fValue = MIN(m_fValue, m_fMaxValue);
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
            if (m_pMinTrackTintImage)
            {
                DRect capInsets;
                capInsets.size = DSize(2, 2);
                capInsets.origin = ccpMult(ccpSub(m_pMinTrackTintImage->getContentSize(), capInsets.size), 0.5f);
                m_pMinTrackTintImageView->setCapInsets(capInsets);
            }
        }
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
            if (m_pMaxTrackTintImage)
            {
                DRect capInsets;
                capInsets.size = DSize(2, 2);
                capInsets.origin = ccpMult(ccpSub(m_pMaxTrackTintImage->getContentSize(), capInsets.size), 0.5f);
                m_pMaxTrackTintImageView->setCapInsets(capInsets);
            }
        }
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
            m_pThumbTintImageView->setImage(m_pThumbTintImage);
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
    float value = (m_fMaxValue - m_fMinValue) * (point.x / bounds.size.width) + m_fMinValue;
    this->_setValue(value);
    
    if (m_function)
        m_function(m_fValue);
}

void CASlider::ccTouchEnded(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    if (!this->isTouchClick())
        return;
    
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    float value = (m_fMaxValue - m_fMinValue) * (point.x / m_obContentSize.width) + m_fMinValue;
    this->_setValue(value);
    
    if (m_function)
        m_function(m_fValue);
    
    if (m_functionTouchUpSide)
        m_functionTouchUpSide(m_fValue);
    
    m_bTouchClick = false;
}

void CASlider::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    m_bTouchClick = false;
}

void CASlider::setTarget(const std::function<void(float)>& function)
{
    if (function) m_function = function;
}

void CASlider::setTargetForTouchUpSide(const std::function<void(float)>& function)
{
    if (function) m_functionTouchUpSide = function;
}

void CASlider::setContentSize(const DSize & var)
{
    DSize size = var;
    if (m_bRecSpe)
    {
        const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CASlider");
        int h = atoi(map.at("height").c_str());
        size.height = (h == 0) ? size.height : h;
    }
    CAControl::setContentSize(size);
    
    this->layoutSubViews();
}

NS_CC_END


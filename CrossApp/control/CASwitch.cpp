//
//  CASwitch.cpp
//  CrossApp 
//
//  Created by juguanhui on 14-5-29.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CASwitch.h"
#include "view/CAImageView.h"
#include "dispatcher/CATouch.h"
#include "view/CAScale9ImageView.h"
#include "basics/CAPointExtension.h"
#include "view/CARenderImage.h"
#include "basics/CAApplication.h"
#include "animation/CAViewAnimation.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN

CASwitch::CASwitch(const CASwitch::Type& type)
    : CAControl()
    , m_onImage(nullptr)
    , m_offImage(nullptr)
    , m_thumbTintImage(nullptr)
    , m_bIsOn(false)
    , m_pOnImageView(nullptr)
    , m_pOffImageView(nullptr)
    , m_pThumbTintImageView(nullptr)
    , m_eType(type)
{
    this->setTouchEventScrollHandOverToSuperview(false);
}

CASwitch::~CASwitch()
{
    CC_SAFE_RELEASE_NULL(m_onImage);
	CC_SAFE_RELEASE_NULL(m_offImage);
	CC_SAFE_RELEASE_NULL(m_thumbTintImage);
}

CASwitch* CASwitch::create(const CASwitch::Type& type)
{
    CASwitch* switchControl = new CASwitch(type);
    
    if (switchControl && switchControl->init())
    {
        switchControl->autorelease();
        return switchControl;
    }
    
    CC_SAFE_DELETE(switchControl);
    return NULL;
}

CASwitch* CASwitch::createWithFrame(const DRect& rect, const CASwitch::Type& type)
{
    CASwitch* switchControl = new CASwitch(type);
    
    if (switchControl && switchControl->initWithFrame(rect))
    {
        switchControl->autorelease();
        return switchControl;
    }
    
    CC_SAFE_DELETE(switchControl);
    return NULL;
}

CASwitch* CASwitch::createWithCenter(const DRect& rect, const CASwitch::Type& type)
{
    CASwitch* switchControl = new CASwitch(type);
    
    if (switchControl && switchControl->initWithCenter(rect))
    {
        switchControl->autorelease();
        return switchControl;
    }
    
    CC_SAFE_DELETE(switchControl);
    return NULL;
}

CASwitch* CASwitch::createWithLayout(const CrossApp::DLayout &layout, const CASwitch::Type& type)
{
    CASwitch* switchControl = new CASwitch(type);
    
    if (switchControl && switchControl->initWithLayout(layout))
    {
        switchControl->autorelease();
        return switchControl;
    }
    
    CC_SAFE_DELETE(switchControl);
    return NULL;
}

bool CASwitch::init()
{
    if (!CAControl::init())
    {
        return false;
    }
    
    switch (m_eType) {
        case CASwitch::Type::SquareRect:
        {
            const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CASwitch_SquareRect");
            this->setOnImage(CAImage::create(map.at("onImage")));
            this->setOffImage(CAImage::create(map.at("offImage")));
            this->setThumbTintImage(CAImage::create(map.at("thumbTintImage")));
        }
            break;
        case CASwitch::Type::RoundedRect:
        {
            const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CASwitch_RoundedRect");
            this->setOnImage(CAImage::create(map.at("onImage")));
            this->setOffImage(CAImage::create(map.at("offImage")));
            this->setThumbTintImage(CAImage::create(map.at("thumbTintImage")));
        }
            break;
        default:
            break;
    }
    
    m_pOnImageView = CAImageView::createWithImage(m_onImage);
    this->addSubview(m_pOnImageView);

    m_pOffImageView = CAImageView::createWithImage(m_offImage);
    this->addSubview(m_pOffImageView);
    
    m_pThumbTintImageView = CAImageView::createWithImage(m_thumbTintImage);
    this->addSubview(m_pThumbTintImageView);
 
    return true;
}

void CASwitch::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

void CASwitch::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    this->updateSwitchState(false);
}

bool CASwitch::isOn()
{
    return m_bIsOn;
}

void CASwitch::setIsOn(bool on, bool animated)
{
    if (m_bIsOn != on)
    {
        m_bIsOn = on;
        CC_RETURN_IF(!m_bRunning);
        this->updateSwitchState(animated);
    }
}

CAImage* CASwitch::getOnImage()
{
    return m_onImage;
}

void CASwitch::setOnImage(CAImage* onImage)
{
    if (m_onImage != onImage)
    {
        CC_SAFE_RETAIN(onImage);
        CC_SAFE_RELEASE(m_onImage);
        m_onImage = onImage;
        if (m_pOnImageView)
        {
            m_pOnImageView->setImage(m_onImage);
        }
    }
}

CAImage* CASwitch::getOffImage()
{
    return m_offImage;
}

void CASwitch::setOffImage(CAImage* offImage)
{
    if (m_offImage != offImage)
    {
        CC_SAFE_RETAIN(offImage);
        CC_SAFE_RELEASE(m_offImage);
        m_offImage = offImage;
        if (m_pOffImageView)
        {
            m_pOffImageView->setImage(m_offImage);
        }
    }
}

CAImage* CASwitch::getThumbTintImage()
{
    return m_thumbTintImage;
}

void CASwitch::setThumbTintImage(CAImage* thumbTintImage)
{
    if (m_thumbTintImage != thumbTintImage)
    {
        CC_SAFE_RETAIN(thumbTintImage);
        CC_SAFE_RELEASE(m_thumbTintImage);
        m_thumbTintImage = thumbTintImage;
        if (m_pThumbTintImageView)
        {
            m_pThumbTintImageView->setImage(m_thumbTintImage);
        }
    }
}

void CASwitch::updateSwitchState(bool animated)
{
    DPoint point = m_obContentSize/2;
    m_pOnImageView->setCenterOrigin(point);
    m_pOffImageView->setCenterOrigin(point);
    if (animated)
    {
        CAViewAnimation::Curve curve = m_bIsOn ? CAViewAnimation::Curve::EaseIn : CAViewAnimation::Curve::EaseOut;
        float offAlpha = m_bIsOn ? 0.0f : 1.0f;
        float onAlpha = m_bIsOn ? 1.0f : 0.0f;
        
        CAViewAnimation::beginAnimations("");
        CAViewAnimation::setAnimationDuration(0.2f);
        CAViewAnimation::setAnimationCurve(curve);
        m_pOffImageView->setAlpha(offAlpha);
        m_pOnImageView->setAlpha(onAlpha);
        CAViewAnimation::commitAnimations();
        
        if (m_pThumbTintImageView)
        {
            DPoint point = DPointZero;
            point.x = m_bIsOn ? (m_obContentSize.width - m_pThumbTintImageView->getBounds().size.width) : 0;
            
            CAViewAnimation::beginAnimations("");
            CAViewAnimation::setAnimationDuration(0.2f);
            CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
            m_pThumbTintImageView->setFrameOrigin(point);
            CAViewAnimation::commitAnimations();
        }
    }
    else
    {
        float offAlpha = m_bIsOn ? 0.0f : 1.0f;
        m_pOffImageView->setAlpha(offAlpha);
        
        float onAlpha = m_bIsOn ? 1.0f : 0.0f;
        m_pOnImageView->setAlpha(onAlpha);
        
        if (m_pThumbTintImageView)
        {
            DPoint point = DPointZero;
            point.x = m_bIsOn ? (m_obContentSize.width - m_pThumbTintImageView->getBounds().size.width) : 0;
            m_pThumbTintImageView->setFrameOrigin(point);
        }
    }
}

bool CASwitch::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    m_bPrevIsOn = m_bIsOn;
    
    return true;
}

void CASwitch::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    if (this->convertToNodeSpace(pTouch->getLocation()).x < this->convertToNodeSpace(pTouch->getPreviousLocation()).x)
    {
        if (m_bIsOn == true)
        {
            m_bIsOn = false;
            this->updateSwitchState(true);
        }
    }
    else
    {
        if (m_bIsOn == false)
        {
            m_bIsOn = true;
            this->updateSwitchState(true);
        }
    }
}

void CASwitch::ccTouchEnded(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    DPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (this->getBounds().containsPoint(point) && !pTouch->isDelta())
    {
        m_bIsOn = !m_bIsOn;
    }
    
    if (m_bPrevIsOn != m_bIsOn)
    {
        this->updateSwitchState(true);
        if (m_function)
        {
            m_function(m_bIsOn);
        }
    }
}

void CASwitch::setTarget(const std::function<void(bool)>& function)
{
    if (function) m_function = function;
}


void CASwitch::setContentSize(const DSize & var)
{
    DSize size = var;
    if (m_bRecSpe)
    {
        int h,w;
        switch (m_eType) {
            case CASwitch::Type::SquareRect:
            {
                const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CASwitch_SquareRect");
                h = atoi(map.at("height").c_str());
                w = atoi(map.at("width").c_str());
            }
                break;
            case CASwitch::Type::RoundedRect:
            {
                const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CASwitch_RoundedRect");
                h = atoi(map.at("height").c_str());
                w = atoi(map.at("width").c_str());
            }
                break;
            default:
                break;
        }
        size.height = (h == 0) ? size.height : h;
        size.width  = (w == 0) ? size.width : w;
    }
    CAControl::setContentSize(size);
    DRect bounds = this->getBounds();
    m_pOnImageView->setFrame(bounds);
    m_pOffImageView->setFrame(bounds);
    m_pThumbTintImageView->setFrame(DRect(0, 0, bounds.size.height, bounds.size.height));
    this->updateSwitchState(false);
}

NS_CC_END

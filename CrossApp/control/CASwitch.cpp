//
//  CASwitch.cpp
//  CrossAppx
//
//  Created by juguanhui on 14-5-29.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CASwitch.h"
#include "view/CAImageView.h"
#include "dispatcher/CATouch.h"
#include "view/CAScale9ImageView.h"
#include "support/CCPointExtension.h"
#include "view/CARenderImage.h"
#include "basics/CAApplication.h"
#include "animation/CAViewAnimation.h"

NS_CC_BEGIN

CASwitch::CASwitch()
    : CAControl()
    , m_onImage(NULL)
    , m_offImage(NULL)
    , m_thumbTintImage(NULL)
    , m_isOn(false)
    , m_pOnImageView(NULL)
    , m_pOffImageView(NULL)
    , m_pThumbTintImageView(NULL)
{
    
}

CASwitch::~CASwitch()
{
    CC_SAFE_RELEASE_NULL(m_onImage);
	CC_SAFE_RELEASE_NULL(m_offImage);
	CC_SAFE_RELEASE_NULL(m_thumbTintImage);
}


void CASwitch::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

void CASwitch::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
    
    if (NULL == m_onImage)
    {
        this->setOnImage(CAImage::create("source_material/switch_on.png"));
    }
    
    if (NULL == m_offImage)
    {
        this->setOffImage(CAImage::create("source_material/switch_off.png"));
    }
    
    if (NULL == m_thumbTintImage)
    {
        this->setThumbTintImage(CAImage::create("source_material/switch_indicator.png"));
    }
    
    m_pOnImageView->setImage(m_onImage);
    m_pOffImageView->setImage(m_offImage);
    m_pThumbTintImageView->setImage(m_thumbTintImage);
    
    this->updateSwitchState(false, false);
}

void CASwitch::setIsOn(bool on, bool animated)
{
    if (m_isOn != on)
    {
        m_isOn = on;
        CC_RETURN_IF(!m_bRunning);
        this->updateSwitchState(animated, false);
    }
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

void CASwitch::updateSwitchState(bool animated, bool callfunced)
{
    float time = 0;
    
    if (animated)
    {
        time = 0.2f;
    }

    CCPoint point = m_obContentSize/2;
    m_pOnImageView->setCenterOrigin(point);
    m_pOffImageView->setCenterOrigin(point);

    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0.2f);
    if (m_isOn)
    {
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseIn);
        m_pOffImageView->setAlpha(0.0f);
        m_pOnImageView->setAlpha(1.0f);
    }
    else
    {
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        m_pOffImageView->setAlpha(1.0f);
        m_pOnImageView->setAlpha(0.0f);
    }
    CAViewAnimation::commitAnimations();

    if (m_pThumbTintImageView)
    {
        CCPoint point = CCPointZero;
        point.x = m_isOn ? (m_obContentSize.width - m_pThumbTintImageView->getBounds().size.width) : 0;
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.2f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CASwitch::updateValueChanged));
        m_pThumbTintImageView->setFrameOrigin(point);
        CAViewAnimation::commitAnimations();
    }
}

void CASwitch::updateValueChanged()
{
    if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged])
    {
        ((CAObject *)m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, CCPointZero);
    }
}

CASwitch* CASwitch::createWithFrame(const CCRect& rect)
{
    CASwitch* switchControl = new CASwitch();
    
    if (switchControl && switchControl->initWithFrame(rect))
    {
        switchControl->autorelease();
        return switchControl;
    }
    
    CC_SAFE_DELETE(switchControl);
    return NULL;
}

CASwitch* CASwitch::createWithCenter(const CCRect& rect)
{
    CASwitch* switchControl = new CASwitch();
    
    if (switchControl && switchControl->initWithCenter(rect))
    {
        switchControl->autorelease();
        return switchControl;
    }
    
    CC_SAFE_DELETE(switchControl);
    return NULL;
}

bool CASwitch::initWithFrame(const CCRect& rect)
{
    if (!CAControl::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    this->setFrame(rect);
    
    CCRect bounds = this->getBounds();
    
    m_pOnImageView = CAImageView::createWithFrame(bounds);
    this->addSubview(m_pOnImageView);
    
    m_pOffImageView = CAImageView::createWithFrame(bounds);
    this->addSubview(m_pOffImageView);
    
    m_pThumbTintImageView = CAImageView::createWithFrame(CCRect(0, 0, bounds.size.height, bounds.size.height));
    this->addSubview(m_pThumbTintImageView);
    return true;
}

bool CASwitch::initWithCenter(const CCRect& rect)
{
    if (!CAControl::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    this->setCenter(rect);
    
    CCRect bounds = this->getBounds();
    
    m_pOnImageView = CAImageView::createWithImage(m_onImage);
    m_pOnImageView->setFrame(bounds);
    this->addSubview(m_pOnImageView);
    
    m_pOffImageView = CAImageView::createWithImage(m_offImage);
    m_pOffImageView->setFrame(bounds);
    this->addSubview(m_pOffImageView);
    
    m_pThumbTintImageView = CAImageView::createWithImage(m_thumbTintImage);
    m_pThumbTintImageView->setFrame(CCRect(0, 0, bounds.size.height, bounds.size.height));
    this->addSubview(m_pThumbTintImageView);
    
    return true;
}

bool CASwitch::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
	m_bTouchClick = true;
    
    
    return m_bTouchClick;
}

void CASwitch::ccTouchEnded(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    if (!this->isTouchClick())
        return;
    
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (getBounds().containsPoint(point))
    {
        this->setControlState(CAControlStateNormal);
        m_isOn = !m_isOn;
        this->updateSwitchState(true, true);
    }
}

void CASwitch::addTarget(CAObject* target, SEL_CAControl selector)
{
    this->addTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASwitch::removeTarget(CAObject* target, SEL_CAControl selector)
{
    this->removeTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASwitch::setContentSize(const CCSize & var)
{
    CAControl::setContentSize(CADipSize(102, 56));
}

NS_CC_END

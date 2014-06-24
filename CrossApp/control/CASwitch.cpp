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
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
NS_CC_BEGIN

CASwitch::CASwitch()
    : CAControl()
    , m_onImage(NULL)
    , m_offImage(NULL)
    , m_thumbTintImage(NULL)
    , m_isOn(false)
    , m_onImageView(NULL)
    , m_offImageView(NULL)
    , m_thumbTintImageView(NULL)
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
    
    if (NULL == m_onImage) {
        this->setOnImage(CAImage::create("source_material/switch_on.png"));
    }
    
    if (NULL == m_offImage) {
        this->setOffImage(CAImage::create("source_material/switch_off.png"));
    }
    
    if (NULL == m_thumbTintImage) {
        this->setThumbTintImage(CAImage::create("source_material/switch_indicator.png"));
    }
    CCRect bounds = this->getBounds();
    
    m_onImageView = CAScale9ImageView::createWithImage(m_onImage);
    if (m_onImageView)
    {
        m_onImageView->setFrame(bounds);
        this->addSubview(m_onImageView);
    }
    
    m_offImageView = CAScale9ImageView::createWithImage(m_offImage);
    if (m_offImageView)
    {
        m_offImageView->setFrame(bounds);
        this->addSubview(m_offImageView);
    }
    
    m_thumbTintImageView = CAScale9ImageView::createWithImage(m_thumbTintImage);
    if (m_thumbTintImageView)
    {
        m_thumbTintImageView->setAnchorPoint(CCPointZero);
        m_thumbTintImageView->setFrame(CCRect(0, 0, bounds.size.height, bounds.size.height));
        this->addSubview(m_thumbTintImageView);
    }
    
    this->updateSwitchState();
}

void CASwitch::setIsOn(bool on)
{
    if (m_isOn != on)
    {
        m_isOn = on;
        this->updateSwitchState();
    }
}

void CASwitch::setOnImage(CAImage* onImage)
{
    if (m_onImage != onImage)
    {
        CC_SAFE_RETAIN(onImage);
        CC_SAFE_RELEASE(m_onImage);
        m_onImage = onImage;
        if (m_onImageView)
        {
            m_onImageView->setImage(m_onImage);
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
        if (m_offImageView)
        {
            m_offImageView->setImage(m_offImage);
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
        if (m_thumbTintImageView)
        {
            m_thumbTintImageView->setImage(m_thumbTintImage);
        }
    }
}

void CASwitch::updateSwitchState()
{
    m_onImageView->setVisible(m_isOn);
    m_offImageView->setVisible(!m_isOn);
    
    CCPoint point = m_obContentSize/2;
    m_onImageView->setCenterOrigin(point);
    m_offImageView->setCenterOrigin(point);
    
    if (m_isOn)
    {
        CCFadeTo* fadeTo = CCFadeTo::create(0.2f, 0.0f);
        CCEaseSineIn* in = CCEaseSineIn::create(fadeTo);
        m_offImageView->runAction(in);
    }
    else
    {
        CCFadeTo* fadeTo = CCFadeTo::create(0.2f, 1.0f);
        CCEaseSineOut* out = CCEaseSineOut::create(fadeTo);
        m_offImageView->runAction(out);
    }

    if (m_thumbTintImageView)
    {
        CCPoint point = CCPointZero;
        point.x = m_isOn ? (m_obContentSize.width - m_thumbTintImageView->getBounds().size.width) : 0;
        m_thumbTintImageView->stopAllActions();
        CCFrameOrginTo* moveTo = CCFrameOrginTo::create(0.2f, point);
        CCEaseSineOut* out = CCEaseSineOut::create(moveTo);
        m_thumbTintImageView->runAction(out);
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
    return true;
}

bool CASwitch::initWithCenter(const CCRect& rect)
{
    if (!CAControl::init())
    {
        return false;
    }
    
    this->setCenter(rect);
    return true;
}

bool CASwitch::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (!this->isTouchEnabled())
        return false;
    
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
        this->setIsOn(!m_isOn);

        if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged])
        {
            ((CAObject *)m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, point);
        }
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
    CAControl::setContentSize(CCSize(80, 48));
}

NS_CC_END

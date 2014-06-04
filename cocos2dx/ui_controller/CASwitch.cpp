//
//  CASwitch.cpp
//  cocos2dx
//
//  Created by juguanhui on 14-5-29.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#include "CASwitch.h"
#include "sprite_nodes/CAImageView.h"
#include "touch_dispatcher/CCTouch.h"
#include "sprite_nodes/CAScale9ImageView.h"
#include "support/CCPointExtension.h"
#include "misc_nodes/CCRenderTexture.h"

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
        this->setOnImage(CAImage::create("btn_square_selected.png"));
    }
    
    if (NULL == m_offImage) {
        this->setOffImage(CAImage::create("btn_square_disabled.png"));
    }
    
    if (NULL == m_thumbTintImage) {
        this->setThumbTintImage(CAImage::create("btn_square_normal.png"));
    }
    CCSize size = this->getBounds().size;
    CCRect bounds = CCRectZero;
    
    m_onImageView = CAImageView::createWithImage(this->getImage(m_onImage, size));
    if (m_onImageView)
    {
        m_onImageView->setFrame(bounds);
        this->addSubview(m_onImageView);
    }
    
    m_offImageView = CAImageView::createWithImage(this->getImage(m_offImage, size));
    if (m_offImageView)
    {
        m_offImageView->setFrame(bounds);
        this->addSubview(m_offImageView);
    }
    
    m_thumbTintImageView = CAImageView::createWithImage(this->getImage(m_thumbTintImage, CCSize(size.width / 2, size.height)));
    if (m_thumbTintImageView)
    {
        m_thumbTintImageView->setAnchorPoint(CCPointZero);
        m_thumbTintImageView->setFrame(bounds);
        this->addSubview(m_thumbTintImageView);
    }
    
    this->updateSwitchState();
}

CAImage* CASwitch::getImage(CAImage* image, CCSize size)
{
    CCRect rect;
    rect.origin = ccpSub(ccpMult(image->getContentSize(), 0.5f), CCPoint(0.5f, 0.5f));
    rect.size = CCSize(1, 1);
    
	CAScale9ImageView *scale9Image = CAScale9ImageView::createWithImage(rect, image);
    scale9Image->setAnchorPoint(CCPointZero);
	scale9Image->setPreferredSize(size);
    this->addSubview(scale9Image);
    
	CCRenderTexture* render = CCRenderTexture::create(size.width, size.height, kCAImagePixelFormat_RGBA8888);
	render->beginWithClear(0, 0, 0, 0);
    scale9Image->visit();
	render->end();
    scale9Image->removeFromSuperview();
	return render->getSprite()->getImage();
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
            m_onImageView->setImage(this->getImage(m_onImage, this->getBounds().size));
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
            m_offImageView->setImage(this->getImage(m_offImage, this->getBounds().size));
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
            m_thumbTintImageView->setImage(this->getImage(m_thumbTintImage, CCSize(this->getBounds().size.width / 2, this->getBounds().size.height)));
        }
    }
}

void CASwitch::updateSwitchState()
{
    m_onImageView->setVisible(m_isOn);
    m_offImageView->setVisible(!m_isOn);
    
    CCPoint point = m_obContentSize/2;
    if (m_onImageView)
    {
        m_onImageView->setCenterOrigin(m_isOn ? point : CCPointZero);
    }
    
    if (m_offImageView)
    {
        m_offImageView->setCenterOrigin(m_isOn ? CCPointZero : point);
    }
    
    if (m_thumbTintImageView)
    {
        m_thumbTintImageView->setFrameOrigin(m_isOn ? CCPoint(point.x, 0) : CCPointZero);
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

bool CASwitch::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!this->isTouchEnabled())
        return false;
    
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
	m_bTouchClick = (getBounds().containsPoint(point));
    return m_bTouchClick;
}

void CASwitch::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!this->isTouchClick())
        return;
    
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (getBounds().containsPoint(point))
    {
        this->setControlState(CAControlStateNormal);
        this->setIsOn(!m_isOn);
        
        if (m_pTarget[CAControlEventTouchUpInSide] && m_selTouch[CAControlEventTouchUpInSide])
        {
            ((CCObject *)m_pTarget[CAControlEventTouchUpInSide]->*m_selTouch[CAControlEventTouchUpInSide])(this, point);
        }
    }
}

NS_CC_END

//
//  CAButton.cpp
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#include "CAButton.h"
#include "sprite_nodes/CCScale9Sprite.h"
#include "sprite_nodes/CAView.h"
#include "touch_dispatcher/CCTouch.h"
#include "support/CCPointExtension.h"
#define PLAYSOUND 

NS_CC_BEGIN

CAButton::CAButton(void)
:m_stateSelected(false)
,m_textTag("")
,m_closeTapSound(false)
,m_touchClick(false)
,m_color(ccWHITE)
,m_spriteNormal(NULL)
,m_spriteHighlighted(NULL)
,m_spriteDisabled(NULL)
,m_spriteSelected(NULL)
,m_spriteNPoint(CCPointZero)
,m_spriteHPoint(CCPointZero)
,m_target(NULL)
,m_touchBegin(NULL)
,m_touchMoved(NULL)
,m_touchMovedOutSide(NULL)
,m_touchUpInSide(NULL)
,m_touchUpSide(NULL)
,m_IsRehisterTouchDispatcher(false)
{
}

CAButton::~CAButton(void)
{
}

void CAButton::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

void CAButton::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    if (this->CAControl::getBackGroundView() == NULL)
    {
        this->setBackGroundDefault();
    }
    
    if (this->getHighlightedBackGroundView() == NULL)
    {
        if (CCScale9Sprite* bg = dynamic_cast<CCScale9Sprite*>(this->CAControl::getBackGroundView()))
        {
            CCScale9Sprite* bgHighLighted = CCScale9Sprite::createWithImage(bg->getImage());
            bgHighLighted->setPreferredSize(bg->getPreferredSize());
            bgHighLighted->setColor(ccc3(127, 127, 127));
            this->setBackGround(CAControlStateHighlighted, bgHighLighted);
        }
        else if (CAImageView* bg = dynamic_cast<CAImageView*>(this->CAControl::getBackGroundView()))
        {
            CAImageView* bgHighLighted = CAImageView::createWithImage(bg->getImage());
            bgHighLighted->setBounds(bg->getBounds());
            bgHighLighted->setColor(ccc3(127, 127, 127));
            this->setBackGround(CAControlStateHighlighted, bgHighLighted);
        }
        else if (CAView* bg = dynamic_cast<CAView*>(this->CAControl::getBackGroundView()))
        {
            CAView* bgHighLighted = CAView::createWithFrame(bg->getFrame());
            bgHighLighted->setColor(ccc3(bg->getColor().r/2, bg->getColor().g/2, bg->getColor().b/2));
            this->setBackGround(CAControlStateHighlighted, bgHighLighted);
        }
    }
    
    this->updateWithPoint();
    this->setControlStateNormal();
}

CAButton* CAButton::createWithFrame(const CCRect& rect)
{

    CAButton* btn = new CAButton();
    
    if (btn && btn->initWithFrame(rect))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

CAButton* CAButton::createWithCenter(const CCRect& rect)
{
    
    CAButton* btn = new CAButton();
    
    if (btn && btn->initWithCenter(rect))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

bool CAButton::initWithFrame(const CCRect& rect)
{
    if (!CAView::init())
    {
        return false;
    }

    this->setFrame(rect);
    this->setTouchEnabled(true);
    return true;
}

bool CAButton::initWithCenter(const CCRect& rect)
{
    if (!CAView::init())
    {
        return false;
    }
    
    this->setCenter(rect);
    this->setTouchEnabled(true);
    return true;
}

void CAButton::setBackGroundDefault()
{
    CCScale9Sprite* bgNormal = CCScale9Sprite::createWithImage(CAImage::create("button_normal.png"));
    this->setBackGround(CAControlStateNormal, bgNormal);
    CCScale9Sprite* bgHighlighted = CCScale9Sprite::createWithImage(CAImage::create("button_highlighted.png"));
    this->setBackGround(CAControlStateHighlighted, bgHighlighted);
    CCScale9Sprite* bgDisabled = CCScale9Sprite::createWithImage(CAImage::create("button_disabled.png"));
    this->setBackGround(CAControlStateDisabled, bgDisabled);
    CCScale9Sprite* bgSelected = CCScale9Sprite::createWithImage(CAImage::create("button_selected.png"));
    this->setBackGround(CAControlStateSelected, bgSelected);
}

void CAButton::setBackGround(CAControlState controlState, CAView *var)
{
    if (controlState == CAControlStateNormal)
    {
        this->setBackGroundView(var);
    }

    if (controlState == CAControlStateHighlighted)
    {
        this->setHighlightedBackGroundView(var);
    }
    
    if (controlState == CAControlStateDisabled)
    {
        this->setDisabledBackGroundView(var);
    }
    
    if (controlState == CAControlStateSelected)
    {
        this->setSelectedBackGroundView(var);
    }

    do
    {
        CC_BREAK_IF(var == NULL);
        
        if (this->getBounds().equals(CCRectZero))
        {
            this->setBounds(CCRect(0, 0, var->getFrame().size.width, var->getFrame().size.height));
        }
        
        this->updateWithPoint();
        this->updateWithPreferredSize();
    }
    while (0);
}


void CAButton::updateWithPreferredSize()
{

    do
    {
        CC_BREAK_IF(m_pBackGroundView == NULL);
        CC_BREAK_IF(this->getBounds().equals(m_pBackGroundView->getBounds()));
        
        if (CCScale9Sprite* _var = dynamic_cast<CCScale9Sprite*>(m_pBackGroundView))
        {
            _var->setPreferredSize(m_obContentSize);
        }
        else
        {
            m_pBackGroundView->setFrame(this->getBounds());
        }
    }
    while (0);

    do
    {
        CC_BREAK_IF(m_pHighlightedBackGroundView == NULL);
        CC_BREAK_IF(this->getBounds().equals(m_pHighlightedBackGroundView->getBounds()));
        
        if (CCScale9Sprite* _var = dynamic_cast<CCScale9Sprite*>(m_pHighlightedBackGroundView))
        {
            _var->setPreferredSize(m_obContentSize);
        }
        else
        {
            m_pHighlightedBackGroundView->setFrame(this->getBounds());
        }
    }
    while (0);
    
    do
    {
        CC_BREAK_IF(m_pDisabledBackGroundView == NULL);
        CC_BREAK_IF(this->getBounds().equals(m_pDisabledBackGroundView->getBounds()));
        
        if (CCScale9Sprite* _var = dynamic_cast<CCScale9Sprite*>(m_pDisabledBackGroundView))
        {
            _var->setPreferredSize(m_obContentSize);
        }
        else
        {
            m_pDisabledBackGroundView->setFrame(this->getBounds());
        }
    }
    while (0);

    do
    {
        CC_BREAK_IF(m_pSelectedBackGroundView == NULL);
        CC_BREAK_IF(this->getBounds().equals(m_pSelectedBackGroundView->getBounds()));
        
        if (CCScale9Sprite* _var = dynamic_cast<CCScale9Sprite*>(m_pSelectedBackGroundView))
        {
            _var->setPreferredSize(m_obContentSize);
        }
        else
        {
            m_pSelectedBackGroundView->setFrame(this->getBounds());
        }
    }
    while (0);
}

void CAButton::updateWithPoint()
{
    
    CCPoint point = m_obContentSize/2;
    
    if (m_spriteNormal)
    {
        if (m_spriteNPoint.equals(CCPointZero))
        {
            m_spriteNormal->setAnchorPoint(CCPoint(0.5f, 0.5f));
            m_spriteNormal->setPosition(point);
        }
        else
        {
            m_spriteNormal->setPosition(m_spriteNPoint);
        }
    }
    
    if (m_spriteHighlighted)
    {
        if (m_spriteHPoint.equals(CCPointZero))
        {
            m_spriteHighlighted->setAnchorPoint(CCPoint(0.5f, 0.5f));
            m_spriteHighlighted->setPosition(point);
        }
        else
        {
            m_spriteHighlighted->setPosition(m_spriteHPoint);
        }
    }
    
    if (m_spriteDisabled)
    {
        if (m_spriteDPoint.equals(CCPointZero))
        {
            m_spriteDisabled->setAnchorPoint(CCPoint(0.5f, 0.5f));
            m_spriteDisabled->setPosition(point);
        }
        else
        {
            m_spriteDisabled->setPosition(m_spriteDPoint);
        }
    }
    
    if (m_spriteSelected)
    {
        if (m_spriteSPoint.equals(CCPointZero))
        {
            m_spriteSelected->setAnchorPoint(CCPoint(0.5f, 0.5f));
            m_spriteSelected->setPosition(point);
        }
        else
        {
            m_spriteSelected->setPosition(m_spriteSPoint);
        }
    }
}

bool CAButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    do
    {
        CC_BREAK_IF(!this->isVisible());
        CC_BREAK_IF(!m_bTouchEnabled);
        CC_BREAK_IF(!isNormal());
        CC_BREAK_IF(!getBounds().containsPoint(point));
        
        return this->setTouchBegin(point);
    }
    while (0);
    
    return false;
}

void CAButton::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (!this->isTouchClick()) return;
    
    if (getBounds().containsPoint(point))
    {
        this->setTouchMoved(point);
        this->setControlState(CAControlStateHighlighted);
    }
    else
    {
        this->setTouchMovedOutSide(point);
        this->setControlState(CAControlStateNormal);
    }
}

void CAButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    if (!this->isTouchClick())
        return;
    
    this->setTouchUpSide(point);
    
    if (getBounds().containsPoint(point))
    {
        this->setTouchUpInSide(point);
    }
    
	this->setControlState(CAControlStateNormal);
    
    if (m_stateSelected && getBounds().containsPoint(point))
    {
        this->setControlState(CAControlStateSelected);
    }
}

void CAButton::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    this->ccTouchEnded(pTouch, pEvent);
}

void CAButton::setTouchEnabled(bool enabled)
{
    
    CAView::setTouchEnabled(enabled);
    
    if (!m_specially)
        return;
    
    if (m_bTouchEnabled)
    {
        this->setControlState(CAControlStateNormal);
    }
    else
    {
        this->setControlState(CAControlStateDisabled);
    }
}

void CAButton::addTarget(void *target, SEL_CAButton selector, CAControlType type)
{
    
    switch (type)
    {
        case TouchBegin:
            m_touchBegin = selector;
            break;
        case TouchMoved:
            m_touchMoved = selector;
            break;
        case TouchMovedOutSide:
            m_touchMovedOutSide = selector;
            break;
        case TouchUpInSide:
            m_touchUpInSide = selector;
            break;
        case TouchUpSide:
            m_touchUpSide = selector;
            break;
        default:
            return;
    }
    
    m_target=target;
}

void CAButton::setControlState(CAControlState var)
{
    CAControl::setControlState(var);

    this->removeSubview(m_spriteNormal);
    this->removeSubview(m_spriteHighlighted);
    this->removeSubview(m_spriteDisabled);
    this->removeSubview(m_spriteSelected);
    
    m_eControlState = var;
    
    switch (m_eControlState)
    {
        case CAControlStateNormal:
        {
            if (m_spriteNormal)
            {
                this->addSubview(m_spriteNormal);
            }
            break;
        }
        case CAControlStateHighlighted:
        {
            if (m_spriteHighlighted)
            {
                this->addSubview(m_spriteHighlighted);
            }
            else if (m_spriteNormal)
            {
                this->addSubview(m_spriteNormal);
            }
            break;
        }
        case CAControlStateDisabled:
        {
            if (m_spriteDisabled)
            {
                this->addSubview(m_spriteDisabled);
            }
            else if (m_spriteNormal)
            {
                this->addSubview(m_spriteNormal);
            }
            break;
        }
        case CAControlStateSelected:
        {
            if (m_spriteSelected)
            {
                this->addSubview(m_spriteSelected);
            }
            else if (m_spriteNormal)
            {
                this->addSubview(m_spriteNormal);
            }
            break;
        }
        default:
            break;
    }
}

void CAButton::InterruptTouchState()
{
	do
	{
		CC_BREAK_IF(m_touchClick == false);
		m_touchClick = false;
		this->setControlState(CAControlStateHighlighted);
	} while (0);
}

bool CAButton::setTouchBegin(CCPoint point)
{
	m_touchClick = true;

    if (m_touchBegin)
    {
		m_touchClick = ((CCObject *)m_target->*m_touchBegin)(this, point);
    }
    
	if (m_touchClick)
	{
		this->setControlState(CAControlStateHighlighted);
	}

	return m_touchClick;
}

void CAButton::setTouchUpInSide(CCPoint point)
{
    if (m_touchUpInSide)
    {
        ((CCObject *)m_target->*m_touchUpInSide)(this,point);
    }
}

void CAButton::setTouchUpSide(CCPoint point)
{
    if (m_touchUpSide)
    {
        ((CCObject *)m_target->*m_touchUpSide)(this,point);
    }
}

void CAButton::setTouchMoved(cocos2d::CCPoint point)
{
    if (m_touchMoved)
    {
        ((CCObject *)m_target->*m_touchMoved)(this,point);
    }
}

void CAButton::setTouchMovedOutSide(cocos2d::CCPoint point)
{
    if (m_touchMovedOutSide)
    {
        ((CCObject *)m_target->*m_touchMovedOutSide)(this,point);
    }
}

bool CAButton::isTextTagEqual(const char *text)
{
    return (m_textTag.compare(text)==0);
}

void CAButton::setContentSize(const CCSize & var)
{
    CAView::setContentSize(var);
    
    this->updateWithPoint();
    
    this->updateWithPreferredSize();
}

void CAButton::setSprite(CAControlState controlState, CAView* var)
{
    if (!var)
        return;
    
    var->setAnchorPoint(CCPoint(0.5f, 0.5f));
    
    CCPoint point = m_obContentSize/2;
    
    this->setSprite(controlState, var, point);
}

void CAButton::setSprite(CAControlState controlState, CAView* var, CCPoint point)
{
    if (!var)
        return;
    
    var->setAnchorPoint(CCPoint(0.5f, 0.5f));
    var->setPosition(point);
    var->retain();
    
    CCPoint cPoint = m_obContentSize/2;
    
    if (controlState == CAControlStateNormal)
    {
        this->removeSubview(m_spriteNormal);
        CC_SAFE_DELETE(m_spriteNormal);
        
        if (!cPoint.equals(point))
        {
            m_spriteNPoint = point;
        }
        else
        {
            m_spriteNPoint = CCPointZero;
        }
        
        m_spriteNormal = var;
        
        return;
    }
    else if (controlState == CAControlStateHighlighted)
    {
        this->removeSubview(m_spriteHighlighted);
        CC_SAFE_DELETE(m_spriteHighlighted);
        
        if (!cPoint.equals(point))
        {
            m_spriteHPoint = point;
        }
        else
        {
            m_spriteHPoint = CCPointZero;
        }
        
        m_spriteHighlighted = var;
    }
    else if (controlState == CAControlStateDisabled)
    {
        this->removeSubview(m_spriteDisabled);
        CC_SAFE_DELETE(m_spriteDisabled);
        
        if (!cPoint.equals(point))
        {
            m_spriteDPoint = point;
        }
        else
        {
            m_spriteDPoint = CCPointZero;
        }
        
        m_spriteDisabled = var;
    }
    else if (controlState == CAControlStateSelected)
    {
        this->removeSubview(m_spriteSelected);
        CC_SAFE_DELETE(m_spriteSelected);
        
        if (!cPoint.equals(point))
        {
            m_spriteSPoint = point;
        }
        else
        {
            m_spriteSPoint = CCPointZero;
        }
        
        m_spriteSelected = var;
    }
}

CAView* CAButton::getSprite(CAControlState controlState)
{
    switch (controlState)
    {
        case CAControlStateNormal:
            return m_spriteNormal;
            break;
        case CAControlStateHighlighted:
            return m_spriteHighlighted;
            break;
        case CAControlStateDisabled:
            return m_spriteDisabled;
            break;
        case CAControlStateSelected:
            return m_spriteSelected;
            break;
        default:
            return NULL;
            break;
    }
}

void CAButton::setOpacity(GLubyte opacity)
{
    CAView::setOpacity(opacity);
    
    if (this->getSubviews())
    {
        for (int i=0; i<this->getSubviews()->count(); i++)
        {
            CAView* view=(CAView*)this->getSubviews()->objectAtIndex(i);
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(view))
            {
                _children->setOpacity(opacity);
            }
        }
    }
}

void CAButton::setColor(const ccColor3B &color3){
    
    m_color=color3;
    
    if (this->getSubviews())
    {
        for (int i=0; i<this->getSubviews()->count(); i++)
        {
            CAView* view=(CAView*)this->getSubviews()->objectAtIndex(i);
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(view))
            {
                _children->setColor(color3);
            }
        }
    }
}

ccColor3B& CAButton::getColor(){
    
    return m_color;
}

NS_CC_END
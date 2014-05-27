//
//  CAButton.cpp
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#include "CAButton.h"
#include "sprite_nodes/CAScale9ImageView.h"
#include "sprite_nodes/CAView.h"
#include "touch_dispatcher/CCTouch.h"
#include "support/CCPointExtension.h"
#define PLAYSOUND 

NS_CC_BEGIN

CAButton::CAButton(void)
:m_bAllowsSelected(false)
,m_bSelected(false)
,m_textTag("")
,m_closeTapSound(false)
,m_bTouchClick(false)
,m_color(ccWHITE)
,m_pSpriteNormal(NULL)
,m_pSpriteHighlighted(NULL)
,m_pSpriteDisabled(NULL)
,m_pSpriteSelected(NULL)
,m_tSpriteNPoint(CCPointZero)
,m_tSpriteHPoint(CCPointZero)
,m_tSpriteDPoint(CCPointZero)
,m_tSpriteSPoint(CCPointZero)
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
    CC_SAFE_RELEASE_NULL(m_pSpriteNormal);
    CC_SAFE_RELEASE_NULL(m_pSpriteHighlighted);
    CC_SAFE_RELEASE_NULL(m_pSpriteDisabled);
    CC_SAFE_RELEASE_NULL(m_pSpriteSelected);
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
        this->setBackGroundViewDefault();
    }
    
    if (this->getHighlightedBackGroundView() == NULL)
    {
        if (CAScale9ImageView* bg = dynamic_cast<CAScale9ImageView*>(this->CAControl::getBackGroundView()))
        {
            CAScale9ImageView* bgHighLighted = CAScale9ImageView::createWithImage(bg->getImage());
            bgHighLighted->setPreferredSize(bg->getPreferredSize());
            bgHighLighted->setColor(ccc3(127, 127, 127));
            this->setBackGroundViewForState(CAControlStateHighlighted, bgHighLighted);
        }
        else if (CAImageView* bg = dynamic_cast<CAImageView*>(this->CAControl::getBackGroundView()))
        {
            CAImageView* bgHighLighted = CAImageView::createWithImage(bg->getImage());
            bgHighLighted->setBounds(bg->getBounds());
            bgHighLighted->setColor(ccc3(127, 127, 127));
            this->setBackGroundViewForState(CAControlStateHighlighted, bgHighLighted);
        }
        else if (CAView* bg = dynamic_cast<CAView*>(this->CAControl::getBackGroundView()))
        {
            CAView* bgHighLighted = CAView::createWithFrame(bg->getFrame());
            bgHighLighted->setColor(ccc3(bg->getColor().r/2, bg->getColor().g/2, bg->getColor().b/2));
            this->setBackGroundViewForState(CAControlStateHighlighted, bgHighLighted);
        }
    }
    
    this->updateWithPoint();
    this->setControlState(m_eControlState);
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

void CAButton::setBackGroundViewDefault()
{
    CAScale9ImageView* bgNormal = CAScale9ImageView::createWithImage(CAImage::create("button_normal.png"));
    this->setBackGroundViewForState(CAControlStateNormal, bgNormal);
    CAScale9ImageView* bgHighlighted = CAScale9ImageView::createWithImage(CAImage::create("button_highlighted.png"));
    this->setBackGroundViewForState(CAControlStateHighlighted, bgHighlighted);
    CAScale9ImageView* bgDisabled = CAScale9ImageView::createWithImage(CAImage::create("button_disabled.png"));
    this->setBackGroundViewForState(CAControlStateDisabled, bgDisabled);
    CAScale9ImageView* bgSelected = CAScale9ImageView::createWithImage(CAImage::create("button_selected.png"));
    this->setBackGroundViewForState(CAControlStateSelected, bgSelected);
}

void CAButton::setBackGroundViewForState(CAControlState controlState, CAView *var)
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
        
        if (CAScale9ImageView* _var = dynamic_cast<CAScale9ImageView*>(m_pBackGroundView))
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
        
        if (CAScale9ImageView* _var = dynamic_cast<CAScale9ImageView*>(m_pHighlightedBackGroundView))
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
        
        if (CAScale9ImageView* _var = dynamic_cast<CAScale9ImageView*>(m_pDisabledBackGroundView))
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
        
        if (CAScale9ImageView* _var = dynamic_cast<CAScale9ImageView*>(m_pSelectedBackGroundView))
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
    
    if (m_pSpriteNormal)
    {
        if (m_tSpriteNPoint.equals(CCPointZero))
        {
            m_pSpriteNormal->setAnchorPoint(CCPoint(0.5f, 0.5f));
            m_pSpriteNormal->setPosition(point);
        }
        else
        {
            m_pSpriteNormal->setPosition(m_tSpriteNPoint);
        }
    }
    
    if (m_pSpriteHighlighted)
    {
        if (m_tSpriteHPoint.equals(CCPointZero))
        {
            m_pSpriteHighlighted->setAnchorPoint(CCPoint(0.5f, 0.5f));
            m_pSpriteHighlighted->setPosition(point);
        }
        else
        {
            m_pSpriteHighlighted->setPosition(m_tSpriteHPoint);
        }
    }
    
    if (m_pSpriteDisabled)
    {
        if (m_tSpriteDPoint.equals(CCPointZero))
        {
            m_pSpriteDisabled->setAnchorPoint(CCPoint(0.5f, 0.5f));
            m_pSpriteDisabled->setPosition(point);
        }
        else
        {
            m_pSpriteDisabled->setPosition(m_tSpriteDPoint);
        }
    }
    
    if (m_pSpriteSelected)
    {
        if (m_tSpriteSPoint.equals(CCPointZero))
        {
            m_pSpriteSelected->setAnchorPoint(CCPoint(0.5f, 0.5f));
            m_pSpriteSelected->setPosition(point);
        }
        else
        {
            m_pSpriteSelected->setPosition(m_tSpriteSPoint);
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
        CC_BREAK_IF(m_eControlState != CAControlStateNormal && m_eControlState != CAControlStateSelected);
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
    
    if (getBounds().containsPoint(point))
    {
        if (m_bAllowsSelected)
        {
            if (m_bSelected)
            {
                m_bSelected = false;
                this->setControlState(CAControlStateNormal);
            }
            else
            {
                m_bSelected = true;
                this->setControlState(CAControlStateSelected);
            }
        }
        else
        {
            this->setControlState(CAControlStateNormal);
        }
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

    this->removeSubview(m_pSpriteNormal);
    this->removeSubview(m_pSpriteHighlighted);
    this->removeSubview(m_pSpriteDisabled);
    this->removeSubview(m_pSpriteSelected);
    
    m_eControlState = var;
    
    switch (m_eControlState)
    {
        case CAControlStateNormal:
        {
            if (m_pSpriteNormal)
            {
                this->addSubview(m_pSpriteNormal);
            }
            break;
        }
        case CAControlStateHighlighted:
        {
            if (m_pSpriteHighlighted)
            {
                this->addSubview(m_pSpriteHighlighted);
            }
            else if (m_pSpriteNormal)
            {
                this->addSubview(m_pSpriteNormal);
            }
            break;
        }
        case CAControlStateDisabled:
        {
            if (m_pSpriteDisabled)
            {
                this->addSubview(m_pSpriteDisabled);
            }
            else if (m_pSpriteNormal)
            {
                this->addSubview(m_pSpriteNormal);
            }
            break;
        }
        case CAControlStateSelected:
        {
            if (m_pSpriteSelected)
            {
                this->addSubview(m_pSpriteSelected);
            }
            else if (m_pSpriteNormal)
            {
                this->addSubview(m_pSpriteNormal);
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
		CC_BREAK_IF(m_bTouchClick == false);
		m_bTouchClick = false;
		this->setControlState(CAControlStateNormal);
	} while (0);
}

bool CAButton::setTouchBegin(CCPoint point)
{
	m_bTouchClick = true;

    if (m_touchBegin)
    {
		m_bTouchClick = ((CCObject *)m_target->*m_touchBegin)(this, point);
    }
    
	if (m_bTouchClick)
	{
		this->setControlState(CAControlStateHighlighted);
	}

	return m_bTouchClick;
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

void CAButton::setView(CAControlState controlState, CAView* var)
{
    if (!var)
        return;
    
    var->setAnchorPoint(CCPoint(0.5f, 0.5f));
    
    CCPoint point = m_obContentSize/2;
    
    this->setView(controlState, var, point);
}

void CAButton::setView(CAControlState controlState, CAView* var, CCPoint point)
{
    if (!var)
        return;
    
    var->setAnchorPoint(CCPoint(0.5f, 0.5f));
    var->setPosition(point);
    var->retain();
    
    CCPoint cPoint = m_obContentSize/2;
    
    if (controlState == CAControlStateNormal)
    {
        this->removeSubview(m_pSpriteNormal);
        CC_SAFE_DELETE(m_pSpriteNormal);
        
        if (!cPoint.equals(point))
        {
            m_tSpriteNPoint = point;
        }
        else
        {
            m_tSpriteNPoint = CCPointZero;
        }
        
        m_pSpriteNormal = var;
        
        return;
    }
    else if (controlState == CAControlStateHighlighted)
    {
        this->removeSubview(m_pSpriteHighlighted);
        CC_SAFE_DELETE(m_pSpriteHighlighted);
        
        if (!cPoint.equals(point))
        {
            m_tSpriteHPoint = point;
        }
        else
        {
            m_tSpriteHPoint = CCPointZero;
        }
        
        m_pSpriteHighlighted = var;
    }
    else if (controlState == CAControlStateDisabled)
    {
        this->removeSubview(m_pSpriteDisabled);
        CC_SAFE_DELETE(m_pSpriteDisabled);
        
        if (!cPoint.equals(point))
        {
            m_tSpriteDPoint = point;
        }
        else
        {
            m_tSpriteDPoint = CCPointZero;
        }
        
        m_pSpriteDisabled = var;
    }
    else if (controlState == CAControlStateSelected)
    {
        this->removeSubview(m_pSpriteSelected);
        CC_SAFE_DELETE(m_pSpriteSelected);
        
        if (!cPoint.equals(point))
        {
            m_tSpriteSPoint = point;
        }
        else
        {
            m_tSpriteSPoint = CCPointZero;
        }
        
        m_pSpriteSelected = var;
    }
}

CAView* CAButton::getView(CAControlState controlState)
{
    switch (controlState)
    {
        case CAControlStateNormal:
            return m_pSpriteNormal;
            break;
        case CAControlStateHighlighted:
            return m_pSpriteHighlighted;
            break;
        case CAControlStateDisabled:
            return m_pSpriteDisabled;
            break;
        case CAControlStateSelected:
            return m_pSpriteSelected;
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
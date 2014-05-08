//
//  CAButton.cpp
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-3-23.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#include "CAButton.h"
#include "sprite_nodes/CCScale9Sprite.h"
#include "sprite_nodes/CCView.h"

#define PLAYSOUND 

NS_CC_BEGIN

static int count = 0;

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
    count++;
    printf("CAButton:%d",count);
}

CAButton::~CAButton(void)
{    
    count--;
    printf("~CAButton:%d",count);
}

void CAButton::onExitTransitionDidStart()
{
    CCNodeRGBA::onExitTransitionDidStart();
}

void CAButton::onEnterTransitionDidFinish()
{
    CCNodeRGBA::onEnterTransitionDidFinish();
    
    if (this->CAControl::getBackGroundView() == NULL)
    {
        this->setBackGroundDefault();
    }
    
    if (this->getHighlightedBackGroundView() == NULL)
    {
        if (CCScale9Sprite* bg = dynamic_cast<CCScale9Sprite*>(this->CAControl::getBackGroundView()))
        {
            CCScale9Sprite* bgHighLighted = CCScale9Sprite::createWithTexture(bg->getTexture());
            bgHighLighted->setPreferredSize(bg->getPreferredSize());
            bgHighLighted->setColor(ccc3(127, 127, 127));
            this->setBackGround(CAControlStateHighlighted, bgHighLighted);
        }
        else if (CCSprite* bg = dynamic_cast<CCSprite*>(this->CAControl::getBackGroundView()))
        {
            CCSprite* bgHighLighted = CCSprite::createWithTexture(bg->getTexture());
            bgHighLighted->setBoundsSize(bg->getBounds().size);
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

bool CAButton::initWithFrame(const CCRect& rect)
{
    if (!CCNodeRGBA::init())
    {
        return false;
    }

    this->setFrame(rect);
    this->setTouchEnabled(true);
    return true;
}

void CAButton::setBackGroundDefault()
{
    CCScale9Sprite* bgNormal = CCScale9Sprite::createWithTexture(CAImage::create("button_normal.png"));
    this->setBackGround(CAControlStateNormal, bgNormal);
    CCScale9Sprite* bgHighlighted = CCScale9Sprite::createWithTexture(CAImage::create("button_highlighted.png"));
    this->setBackGround(CAControlStateHighlighted, bgHighlighted);
    CCScale9Sprite* bgDisabled = CCScale9Sprite::createWithTexture(CAImage::create("button_disabled.png"));
    this->setBackGround(CAControlStateDisabled, bgDisabled);
    CCScale9Sprite* bgSelected = CCScale9Sprite::createWithTexture(CAImage::create("button_selected.png"));
    this->setBackGround(CAControlStateSelected, bgSelected);
}

void CAButton::setBackGround(CAControlState controlState, CCNodeRGBA *var)
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
        
        if (m_obContentSize.equals(CCSizeZero))
        {
            this->setContentSize(var->getContentSize());
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
            m_pBackGroundView->setContentSize(m_obContentSize);
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
            m_pHighlightedBackGroundView->setContentSize(m_obContentSize);
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
            m_pDisabledBackGroundView->setContentSize(m_obContentSize);
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
            m_pSelectedBackGroundView->setContentSize(m_obContentSize);
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
    
    if (!this->getTouchClick()) return;
    
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

    if (!this->getTouchClick())
        return;
    
    this->setTouchUpSide(point);
    
    if (getBounds().containsPoint(point))
    {
        this->setTouchUpInSide(point);
    }
    
    this->setTouchClick(false);
    
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
    
    CCNodeRGBA::setTouchEnabled(enabled);
    
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

    this->removeChild(m_spriteNormal);
    this->removeChild(m_spriteHighlighted);
    this->removeChild(m_spriteDisabled);
    this->removeChild(m_spriteSelected);
    
    m_eControlState = var;
    
    switch (m_eControlState)
    {
        case CAControlStateNormal:
        {
            if (m_spriteNormal)
            {
                this->addChild(m_spriteNormal);
            }
            break;
        }
        case CAControlStateHighlighted:
        {
            if (m_spriteHighlighted)
            {
                this->addChild(m_spriteHighlighted);
            }
            else if (m_spriteNormal)
            {
                this->addChild(m_spriteNormal);
            }
            break;
        }
        case CAControlStateDisabled:
        {
            if (m_spriteDisabled)
            {
                this->addChild(m_spriteDisabled);
            }
            else if (m_spriteNormal)
            {
                this->addChild(m_spriteNormal);
            }
            break;
        }
        case CAControlStateSelected:
        {
            if (m_spriteSelected)
            {
                this->addChild(m_spriteSelected);
            }
            else if (m_spriteNormal)
            {
                this->addChild(m_spriteNormal);
            }
            break;
        }
        default:
            break;
    }
}

void CAButton::setTouchClick(bool var)
{
    if (m_touchClick == var)
        return;
    
    m_touchClick = var;
    
    if (m_touchClick)
    {
        this->setControlState(CAControlStateHighlighted);
    }
    else
    {
        this->setControlState(CAControlStateNormal);
    }
    
    if (var && !m_closeTapSound) PLAYSOUND;
}

bool CAButton::getTouchClick()
{
    return m_touchClick;
}

bool CAButton::setTouchBegin(CCPoint point)
{
    if (m_touchBegin)
    {
        bool touchClick = ((CCObject *)m_target->*m_touchBegin)(this,point);
        this->setTouchClick(touchClick);
        return touchClick;
    }
    
    this->setTouchClick(true);
    
    return true;
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
    CCNodeRGBA::setContentSize(var);
    
    this->updateWithPoint();
    
    this->updateWithPreferredSize();
}

void CAButton::setSprite(CAControlState controlState, CCNodeRGBA* var)
{
    if (!var)
        return;
    
    var->setAnchorPoint(CCPoint(0.5f, 0.5f));
    
    CCPoint point = m_obContentSize/2;
    
    this->setSprite(controlState, var, point);
}

void CAButton::setSprite(CAControlState controlState, CCNodeRGBA* var, CCPoint point)
{
    if (!var)
        return;
    
    var->setAnchorPoint(CCPoint(0.5f, 0.5f));
    var->setPosition(point);
    var->retain();
    
    CCPoint cPoint = m_obContentSize/2;
    
    if (controlState == CAControlStateNormal)
    {
        this->removeChild(m_spriteNormal);
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
        this->removeChild(m_spriteHighlighted);
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
        this->removeChild(m_spriteDisabled);
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
        this->removeChild(m_spriteSelected);
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

CCNodeRGBA* CAButton::getSprite(CAControlState controlState)
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
    CCNodeRGBA::setOpacity(opacity);
    
    if (this->getChildren())
    {
        for (int i=0; i<this->getChildren()->count(); i++)
        {
            CCNode* node=(CCNode*)this->getChildren()->objectAtIndex(i);
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(node))
            {
                _children->setOpacity(opacity);
            }
        }
    }
}

void CAButton::setColor(const ccColor3B &color3){
    
    m_color=color3;
    
    if (this->getChildren())
    {
        for (int i=0; i<this->getChildren()->count(); i++)
        {
            CCNode* node=(CCNode*)this->getChildren()->objectAtIndex(i);
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(node))
            {
                _children->setColor(m_color);
            }
        }
    }
}

ccColor3B& CAButton::getColor(){
    
    return m_color;
}

NS_CC_END
//
//  CAButton.cpp
//  LTabelView
//  出自九秒社区 http://www.9miao.com
//
//  Created by LI YUANFENG on 13-3-14.
//
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
,m_specially(true)
,m_closeTapSound(false)
,m_touchClick(false)
,m_touchState(CAButtonStateNormal)
,m_color(ccWHITE)
,m_bgNormal(NULL)
,m_bgHighlighted(NULL)
,m_bgDisabled(NULL)
,m_bgSelected(NULL)
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
    
    if (m_bgNormal == NULL)
    {
        this->setBackGroundDefault();
    }
    
    if (m_bgHighlighted == NULL)
    {
        if (CCScale9Sprite* bg = dynamic_cast<CCScale9Sprite*>(m_bgNormal))
        {
            CCScale9Sprite* bgHighLighted = CCScale9Sprite::createWithTexture(bg->getTexture());
            bgHighLighted->setPreferredSize(bg->getPreferredSize());
            bgHighLighted->setColor(ccc3(127, 127, 127));
            this->setBackGround(CAButtonStateHighlighted, bgHighLighted);
        }
        else if (CCSprite* bg = dynamic_cast<CCSprite*>(m_bgNormal))
        {
            CCSprite* bgHighLighted = CCSprite::createWithTexture(bg->getTexture());
            bgHighLighted->setBoundsSize(bg->getBounds().size);
            bgHighLighted->setColor(ccc3(127, 127, 127));
            this->setBackGround(CAButtonStateHighlighted, bgHighLighted);
        }
        else if (CAView* bg = dynamic_cast<CAView*>(m_bgNormal))
        {
            CAView* bgHighLighted = CAView::createWithFrame(bg->getFrame());
            bgHighLighted->setColor(ccc3(bg->getColor().r/2, bg->getColor().g/2, bg->getColor().b/2));
            this->setBackGround(CAButtonStateHighlighted, bgHighLighted);
        }
    }
    
    this->updateWithPoint();
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
    this->setBackGround(CAButtonStateNormal, bgNormal);
    CCScale9Sprite* bgHighlighted = CCScale9Sprite::createWithTexture(CAImage::create("button_highlighted.png"));
    this->setBackGround(CAButtonStateHighlighted, bgHighlighted);
    CCScale9Sprite* bgDisabled = CCScale9Sprite::createWithTexture(CAImage::create("button_disabled.png"));
    this->setBackGround(CAButtonStateDisabled, bgDisabled);
    CCScale9Sprite* bgSelected = CCScale9Sprite::createWithTexture(CAImage::create("button_selected.png"));
    this->setBackGround(CAButtonStateSelected, bgSelected);
}

void CAButton::setBackGround(CAButtonTouchState touchState, CCNodeRGBA *var)
{
    if (touchState == CAButtonStateNormal)
    {
        this->removeChild(m_bgNormal);
        m_bgNormal = var;
    }

    if (touchState == CAButtonStateHighlighted)
    {
        this->removeChild(m_bgHighlighted);
        m_bgHighlighted = var;
    }
    
    if (touchState == CAButtonStateDisabled)
    {
        this->removeChild(m_bgDisabled);
        m_bgDisabled = var;
    }
    
    if (touchState == CAButtonStateSelected)
    {
        this->removeChild(m_bgSelected);
        m_bgSelected = var;
    }

    do
    {
        CC_BREAK_IF(var == NULL);
        
        this->addChild(var);
        
        if (m_obContentSize.equals(CCSizeZero))
        {
            this->setContentSize(var->getContentSize());
        }
        
        if (touchState != CAButtonStateNormal)
        {
            var->setVisible(false);
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
        CC_BREAK_IF(m_bgNormal == NULL);
        CC_BREAK_IF(this->getBounds().equals(m_bgNormal->getBounds()));
        
        if (CCScale9Sprite* _var = dynamic_cast<CCScale9Sprite*>(m_bgNormal))
        {
            _var->setPreferredSize(m_obContentSize);
        }
        else
        {
            m_bgNormal->setBoundsSize(m_obContentSize);
        }
    }
    while (0);

    do
    {
        CC_BREAK_IF(m_bgHighlighted == NULL);
        CC_BREAK_IF(this->getBounds().equals(m_bgHighlighted->getBounds()));
        
        if (CCScale9Sprite* _var = dynamic_cast<CCScale9Sprite*>(m_bgHighlighted))
        {
            _var->setPreferredSize(m_obContentSize);
        }
        else
        {
            m_bgHighlighted->setBoundsSize(m_obContentSize);
        }
    }
    while (0);
    
    do
    {
        CC_BREAK_IF(m_bgDisabled == NULL);
        CC_BREAK_IF(this->getBounds().equals(m_bgDisabled->getBounds()));
        
        if (CCScale9Sprite* _var = dynamic_cast<CCScale9Sprite*>(m_bgDisabled))
        {
            _var->setPreferredSize(m_obContentSize);
        }
        else
        {
            m_bgDisabled->setBoundsSize(m_obContentSize);
        }
    }
    while (0);
    
    
    
    do
    {
        CC_BREAK_IF(m_bgSelected == NULL);
        CC_BREAK_IF(this->getBounds().equals(m_bgSelected->getBounds()));
        
        if (CCScale9Sprite* _var = dynamic_cast<CCScale9Sprite*>(m_bgSelected))
        {
            _var->setPreferredSize(m_obContentSize);
        }
        else
        {
            m_bgSelected->setBoundsSize(m_obContentSize);
        }
    }
    while (0);
}

void CAButton::updateWithPoint()
{
    
    CCPoint point = m_obContentSize/2;
    
    if (m_bgNormal)
    {
        m_bgNormal->setAnchorPoint(CCPoint(0.5f, 0.5f));
        m_bgNormal->setPosition(point);
    }
    
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
    
    if (m_bgHighlighted)
    {
        m_bgHighlighted->setAnchorPoint(CCPoint(0.5f, 0.5f));
        m_bgHighlighted->setPosition(point);
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
    
    if (m_bgDisabled)
    {
        m_bgDisabled->setAnchorPoint(CCPoint(0.5f, 0.5f));
        m_bgDisabled->setPosition(point);
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
    
    if (m_bgSelected)
    {
        m_bgSelected->setAnchorPoint(CCPoint(0.5f, 0.5f));
        m_bgSelected->setPosition(point);
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
        this->setTouchState(CAButtonStateHighlighted);
        this->setTouchMoved(point);
    }
    else
    {
        this->setTouchState(CAButtonStateNormal);
        this->setTouchMovedOutSide(point);
    }
}

void CAButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);

    if (!this->getTouchClick())
        return;
    
    this->setTouchClick(false);
    
    this->setTouchUpSide(point);
    
    if (getBounds().containsPoint(point))
    {
        if (m_stateSelected)
        {
            this->setTouchState(CAButtonStateSelected);
        }
        this->setTouchUpInSide(point);
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
        this->setTouchState(CAButtonStateNormal);
    }
    else
    {
        this->setTouchState(CAButtonStateDisabled);
    }
}

void CAButton::addTarget(void *target, SEL_CAButton selector, CAButtonType type)
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

bool CAButton::isNormal()
{
    return (m_touchState == CAButtonStateNormal);
}

bool CAButton::isHighlighted()
{
    return (m_touchState == CAButtonStateHighlighted);
}

bool CAButton::isDisabled()
{
    return (m_touchState == CAButtonStateDisabled);
}

bool CAButton::isSelected()
{
    return (m_touchState == CAButtonStateSelected);
}

void CAButton::setTouchState(CAButtonTouchState var){
    
    if (m_touchState == var)
        return;
    
    this->setNormalEffect();
    
    m_touchState = var;
    
    switch (m_touchState)
    {
        case CAButtonStateHighlighted:
            this->setHighlightedEffect();
            break;
        case CAButtonStateDisabled:
            this->setDisabledEffect();
            break;
        case CAButtonStateSelected:
            this->setSelectedEffect();
            break;
        default:
            break;
    }
}

CAButtonTouchState CAButton::getTouchState()
{
    return m_touchState;
}

void CAButton::setTouchClick(bool var)
{
    if (m_touchClick == var)
        return;
    
    m_touchClick = var;
    
    if (m_touchClick)
    {
        this->setTouchState(CAButtonStateHighlighted);
    }
    else
    {
        this->setTouchState(CAButtonStateNormal);
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

void CAButton::setNormalEffect()
{
    if (!m_specially)
        return;
    
    if (m_touchState == CAButtonStateNormal) return;
    
    if (m_bgNormal)
        m_bgNormal->setVisible(true);
    
    if (m_spriteNormal)
        m_spriteNormal->setVisible(true);

    if (m_touchState == CAButtonStateHighlighted)
    {
        
        if (m_bgHighlighted) m_bgHighlighted->setVisible(false);
        
        if (m_spriteHighlighted) m_spriteHighlighted->setVisible(false);
        
    }
    else if (m_touchState == CAButtonStateDisabled)
    {
        
        if (m_bgDisabled) m_bgDisabled->setVisible(false);
        
        if (m_spriteDisabled) m_spriteDisabled->setVisible(false);
        
    }
    else if (m_touchState == CAButtonStateSelected)
    {
        
        if (m_bgSelected) m_bgSelected->setVisible(false);
        
        if (m_spriteSelected) m_spriteSelected->setVisible(false);
        
    }
}

void CAButton::setHighlightedEffect()
{
    if (!m_specially)
        return;
    
    if (m_bgHighlighted)
    {
        if (m_bgNormal) m_bgNormal->setVisible(false);
        
        m_bgHighlighted->setVisible(true);
    }
    
    if (m_spriteHighlighted)
    {
        if (m_spriteNormal) m_spriteNormal->setVisible(false);
        
        m_spriteHighlighted->setVisible(true);
    }
}

void CAButton::setDisabledEffect()
{
    if (!m_specially)
        return;
    
    if (m_bgDisabled)
    {
        if (m_bgNormal) m_bgNormal->setVisible(false);
        
        m_bgDisabled->setVisible(true);
    }
        
    if (m_spriteDisabled)
    {
        if (m_spriteNormal) m_spriteNormal->setVisible(false);
        
        m_spriteDisabled->setVisible(true);
    }
}

void CAButton::setSelectedEffect()
{
    if (!m_specially)
        return;
    
    if (m_bgSelected)
    {
        if (m_bgNormal) m_bgNormal->setVisible(false);
        
        m_bgSelected->setVisible(true);
    }
        
    if (m_spriteSelected)
    {
        if (m_spriteNormal) m_spriteNormal->setVisible(false);
        
        m_spriteSelected->setVisible(true);
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

void CAButton::setSprite(CAButtonTouchState touchState, CCNodeRGBA* var)
{
    if (!var)
        return;
    
    var->setAnchorPoint(CCPoint(0.5f, 0.5f));
    
    CCPoint point = m_obContentSize/2;
    
    this->setSprite(touchState, var, point);
}

void CAButton::setSprite(CAButtonTouchState touchState, CCNodeRGBA* var, CCPoint point)
{
    if (!var)
        return;

    var->setPosition(point);
    this->addChild(var);
    
    CCPoint cPoint = m_obContentSize/2;
    
    if (touchState == CAButtonStateNormal)
    {
        if (m_spriteNormal)
        {
            m_spriteNormal->removeFromParent();
            m_spriteNormal = NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteNPoint = point;
        }
        else
        {
            m_spriteNPoint=CCPointZero;
        }
        
        m_spriteNormal=var;
        
        return;
    }
    else if (touchState == CAButtonStateHighlighted)
    {
        if (m_spriteHighlighted)
        {
            m_spriteHighlighted->removeFromParent();
            m_spriteHighlighted = NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteHPoint=point;
        }
        else
        {
            m_spriteHPoint=CCPointZero;
        }
        
        m_spriteHighlighted=var;
    }
    else if (touchState == CAButtonStateDisabled)
    {
        if (m_spriteDisabled)
        {
            m_spriteDisabled->removeFromParent();
            m_spriteDisabled = NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteDPoint = point;
        }
        else
        {
            m_spriteDPoint = CCPointZero;
        }
        
        m_spriteDisabled=var;
    }
    else if (touchState == CAButtonStateSelected)
    {
        if (m_spriteSelected)
        {
            m_spriteSelected->removeFromParent();
            m_spriteSelected = NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteSPoint = point;
        }
        else
        {
            m_spriteSPoint = CCPointZero;
        }
        
        m_spriteSelected=var;
    }
    
    var->setVisible(false);
}

CCNodeRGBA* CAButton::getSprite(CAButtonTouchState touchState)
{
    switch (touchState)
    {
        case CAButtonStateNormal:
            return m_spriteNormal;
            break;
        case CAButtonStateHighlighted:
            return m_spriteHighlighted;
            break;
        case CAButtonStateDisabled:
            return m_spriteDisabled;
            break;
        case CAButtonStateSelected:
            return m_spriteSelected;
            break;
        default:
            return NULL;
            break;
    }
}


CCNodeRGBA* CAButton::getBackGround(CAButtonTouchState touchState)
{
    switch (touchState)
    {
        case CAButtonStateNormal:
            return m_bgNormal;
            break;
        case CAButtonStateHighlighted:
            return m_bgHighlighted;
            break;
        case CAButtonStateDisabled:
            return m_bgDisabled;
            break;
        case CAButtonStateSelected:
            return m_bgSelected;
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
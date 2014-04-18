//
//  LMenu.cpp
//  LTabelView
//  出自九秒社区 http://www.9miao.com
//
//  Created by LI YUANFENG on 13-3-14.
//
//

#include "LMenu.h"

#define PLAYSOUND 

static int count = 0;

LMenu::LMenu(void)
:m_sign(0)
,m_stateSelected(false)
,m_textTag("")
,m_bIsTouchEnabled(true)
,m_specially(true)
,m_closeTapSound(false)
,m_touchClick(false)
,m_touchState(LMenuStateNormal)
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
,m_preferredSize(CCSizeZero)
,m_target(NULL)
,m_touchBegin(NULL)
,m_touchMoved(NULL)
,m_touchMovedOutSide(NULL)
,m_touchUpInSide(NULL)
,m_touchUpSide(NULL)
,m_nbSwallowsTouches(true)
,m_IsRehisterTouchDispatcher(false)
,m_nTouchPriority(0)
{
    count++;
    printf("LMenu:%d",count);
}

LMenu::~LMenu(void)
{    
    count--;
    printf("~LMenu:%d",count);
}

void LMenu::onExit(){
    
    this->unRegisterWithTouchDispatcher();
    CCNodeRGBA::onExit();
}

void LMenu::onEnter(){
    
    CCNodeRGBA::onEnter();
    this->registerWithTouchDispatcher();
    this->updateWithPoint();
}

LMenu* LMenu::create(const cocos2d::CCSize &size){

    LMenu* btn=new LMenu();
    
    if (btn && btn->init())
    {
        btn->setPreferredSize(size);
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

LMenu* LMenu::createWithRGBAProtocol(CCRGBAProtocol* backGround){
    
    LMenu* btn=new LMenu();
    
    if (btn && btn->initWithBackGround(backGround))
    {
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

LMenu* LMenu::createWithSprite(CCSprite *backGround){

    return LMenu::createWithRGBAProtocol(backGround);
}

LMenu* LMenu::createWithScale9Sprite(CCScale9Sprite *backGround){
    
    return LMenu::createWithRGBAProtocol(backGround);
}

LMenu* LMenu::createWithLayerColor(CCLayerColor *backGround){
    
    return LMenu::createWithRGBAProtocol(backGround);
}

bool LMenu::initWithBackGround(CCRGBAProtocol *backGround){
    
    if (!backGround)
    {
        return false;
    }
    
    if (!CCNodeRGBA::init())
    {
        return false;
    }

    if (CCNode* _backGround=dynamic_cast<CCNode*>(backGround))
    {
        this->setPreferredSize(_backGround->getContentSize());
    }
    else
    {
        return false;
    }
    
    return this->setBackGround(LMenuStateNormal, backGround);
}

bool LMenu::initWithSpriteBackGround(CCSprite *backGround){

    return this->initWithBackGround(backGround);
}

bool LMenu::initWithScale9SpriteBackGround(CCScale9Sprite *backGround){
    
    return this->initWithBackGround(backGround);
}

bool LMenu::initWithLayerColorBackGround(CCLayerColor *backGround){
    
    return this->initWithBackGround(backGround);
}

bool LMenu::setBackGround(LMenuTouchState touchState, CCRGBAProtocol *var){

    if (!var)
    {
        return false;
    }
    
    if (CCNode* node=dynamic_cast<CCNode*>(var))
    {
        this->addChild(node,-1);
        
        if (!m_preferredSize.equals(CCSizeZero) && !m_preferredSize.equals(node->getContentSize()))
        {
            if (CCScale9Sprite* _var=dynamic_cast<CCScale9Sprite*>(node))
            {
                _var->setPreferredSize(m_preferredSize);
            }
            else if (CCNodeRGBA* _var=dynamic_cast<CCNodeRGBA*>(node))
            {
                _var->setContentSize(m_preferredSize);
            }
        }
        
        if (touchState==LMenuStateNormal)
        {
            if (m_bgNormal)
            {
                m_bgNormal->removeFromParent();
            }
            m_bgNormal=node;
        }
        else
        {
            node->setVisible(false);
        }
        
        if (touchState==LMenuStateHighlighted)
        {
            if (m_bgHighlighted)
            {
                m_bgHighlighted->removeFromParent();
            }
            m_bgHighlighted=node;
        }
        
        if (touchState==LMenuStateDisabled)
        {
            if (m_bgDisabled)
            {
                m_bgDisabled->removeFromParent();
            }
            m_bgDisabled=node;
        }
        
        if (touchState==LMenuStateSelected)
        {
            if (m_bgSelected)
            {
                m_bgSelected->removeFromParent();
            }
            m_bgSelected=node;
        }
        
        this->updateWithPoint();
        this->updateWithPreferredSize();
        
        return true;

    }

    return false;
}

bool LMenu::setSpriteBackGround(LMenuTouchState touchState, CCSprite *var){
    
    return this->setBackGround(touchState, var);
}

bool LMenu::setScale9SpriteBackGround(LMenuTouchState touchState, CCScale9Sprite *var){
    
    return this->setBackGround(touchState, var);
}

bool LMenu::setLayerColorBackGround(LMenuTouchState touchState, CCLayerColor *var){
    
    return this->setBackGround(touchState, var);
}

void LMenu::updateWithPreferredSize(){

    if (m_bgNormal && !m_bgNormal->getContentSize().equals(m_preferredSize));
    {
        if (CCScale9Sprite* node=dynamic_cast<CCScale9Sprite*>(m_bgNormal))
        {
            node->setPreferredSize(m_preferredSize);
        }
        if (CCLayerColor* node=dynamic_cast<CCLayerColor*>(m_bgNormal))
        {
            node->setContentSize(m_preferredSize);
        }
    }

    if (m_bgHighlighted && !m_bgHighlighted->getContentSize().equals(m_preferredSize))
    {
        if (CCScale9Sprite* node=dynamic_cast<CCScale9Sprite*>(m_bgHighlighted))
        {
            node->setPreferredSize(m_preferredSize);
        }
        if (CCLayerColor* node=dynamic_cast<CCLayerColor*>(m_bgHighlighted))
        {
            node->setContentSize(m_preferredSize);
        }
    }

    if (m_bgDisabled && !m_bgDisabled->getContentSize().equals(m_preferredSize))
    {
        if (CCScale9Sprite* node=dynamic_cast<CCScale9Sprite*>(m_bgDisabled))
        {
            node->setPreferredSize(m_preferredSize);
        }
        if (CCLayerColor* node=dynamic_cast<CCLayerColor*>(m_bgDisabled))
        {
            node->setContentSize(m_preferredSize);
        }
    }
    
    if (m_bgSelected && !m_bgSelected->getContentSize().equals(m_preferredSize))
    {
        if (CCScale9Sprite* node=dynamic_cast<CCScale9Sprite*>(m_bgSelected))
        {
            node->setPreferredSize(m_preferredSize);
        }
        if (CCLayerColor* node=dynamic_cast<CCLayerColor*>(m_bgSelected))
        {
            node->setContentSize(m_preferredSize);
        }
    }

}

void LMenu::updateWithPoint(){
    
    CCPoint point=CCPoint(m_preferredSize.width/2, m_preferredSize.height/2);
    
    if (m_bgNormal)
    {
        if (CCLayerColor* node=dynamic_cast<CCLayerColor*>(m_bgNormal))
        {
            node->setPosition(CCPointZero);
        }
        else if (CCNodeRGBA* node=dynamic_cast<CCNodeRGBA*>(m_bgNormal))
        {
            node->setPosition(point);
        }
    }
    
    if (m_spriteNormal)
    {
        if (m_spriteNPoint.equals(CCPointZero))
        {
            m_spriteNormal->setPosition(point);
        }
        else
        {
            m_spriteNormal->setPosition(m_spriteNPoint);
        }
    }
    
    if (m_bgHighlighted)
    {
        if (CCLayerColor* node=dynamic_cast<CCLayerColor*>(m_bgHighlighted))
        {
            node->setPosition(CCPointZero);
        }
        else if (CCNodeRGBA* node=dynamic_cast<CCNodeRGBA*>(m_bgHighlighted))
        {
            node->setPosition(point);
        }
    }
    
    if (m_spriteHighlighted)
    {
        if (m_spriteHPoint.equals(CCPointZero))
        {
            m_spriteHighlighted->setPosition(point);
        }
        else
        {
            m_spriteHighlighted->setPosition(m_spriteHPoint);
        }
    }
    
    if (m_bgDisabled)
    {
        if (CCLayerColor* node=dynamic_cast<CCLayerColor*>(m_bgDisabled))
        {
            node->setPosition(CCPointZero);
        }
        else if (CCNodeRGBA* node=dynamic_cast<CCNodeRGBA*>(m_bgDisabled))
        {
            node->setPosition(point);
        }
    }
    
    if (m_spriteDisabled)
    {
        if (m_spriteDPoint.equals(CCPointZero))
        {
            m_spriteDisabled->setPosition(point);
        }
        else
        {
            m_spriteDisabled->setPosition(m_spriteDPoint);
        }
    }
    
    if (m_bgSelected)
    {
        if (CCLayerColor* node=dynamic_cast<CCLayerColor*>(m_bgSelected))
        {
            node->setPosition(CCPointZero);
        }
        else if (CCNodeRGBA* node=dynamic_cast<CCNodeRGBA*>(m_bgSelected))
        {
            node->setPosition(point);
        }
    }
    
    if (m_spriteSelected)
    {
        if (m_spriteSPoint.equals(CCPointZero))
        {
            m_spriteSelected->setPosition(point);
        }
        else
        {
            m_spriteSelected->setPosition(m_spriteSPoint);
        }
    }
}

CCRect LMenu::getRect(){

    return CCRect(0, 0, m_preferredSize.width, m_preferredSize.height);
}

bool LMenu::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    CCPoint uipoint=pTouch->getLocationInView();
    CCPoint point=CCDirector::sharedDirector()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point);
    
    if (!this->isVisible()) return false;
    
    if (!m_bIsTouchEnabled) return false;
    
    if (!getRect().containsPoint(point)) return false;
    
    if (!isNormal()) return false;
    
    return this->setTouchBegin(point);
}

void LMenu::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    CCPoint uipoint=pTouch->getLocationInView();
    CCPoint point=CCDirector::sharedDirector()->convertToGL(uipoint);
    point=this->convertToNodeSpace(point);
    
    if (!this->getTouchClick()) return;
    
    if (getRect().containsPoint(point))
    {
        this->setTouchState(LMenuStateHighlighted);
        this->setTouchMoved(point);
    }
    else
    {
        this->setTouchState(LMenuStateNormal);
        this->setTouchMovedOutSide(point);
    }
}

void LMenu::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    CCPoint uipoint=pTouch->getLocationInView();
    CCPoint point=CCDirector::sharedDirector()->convertToGL(uipoint);
    point = this->convertToNodeSpace(point);

    if (!this->getTouchClick()) return;
    
    this->setTouchClick(false);
    
    this->setTouchUpSide(point);
    
    if (getRect().containsPoint(point))
    {
        if (m_stateSelected)
        {
            this->setTouchState(LMenuStateSelected);
        }
        this->setTouchUpInSide(point);
    }
}

void LMenu::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    this->ccTouchEnded(pTouch, pEvent);
}

void LMenu::setTouchEnabled(bool enabled){
    
    m_bIsTouchEnabled=enabled;
    
    if (!m_specially) return;
    
    if (m_bIsTouchEnabled)
    {
        this->setTouchState(LMenuStateNormal);
    }
    else
    {
        this->setTouchState(LMenuStateDisabled);
    }
}

bool LMenu::getTouchEnabled(){
    
    return m_bIsTouchEnabled;
}

void LMenu::registerWithTouchDispatcher(){
    
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->addTargetedDelegate(this, m_nTouchPriority, m_nbSwallowsTouches); 
    m_IsRehisterTouchDispatcher=true;
}

void LMenu::unRegisterWithTouchDispatcher(){
    
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->removeDelegate(this);
    m_IsRehisterTouchDispatcher=false;
}

void LMenu::setTouchDispatcher(bool bSwallowsTouches, int nPriority) {
    
    m_nbSwallowsTouches=bSwallowsTouches;
    m_nTouchPriority=nPriority;
    
    if (m_IsRehisterTouchDispatcher)
    {
        CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        touchDispatcher->removeDelegate(this);
        touchDispatcher->addTargetedDelegate(this, nPriority, bSwallowsTouches);
    }
}

void LMenu::addTarget(void *target, SEL_LMenu selector, LMenuType type){
    
    switch (type)
    {
        case TouchBegin:
            m_touchBegin=selector;
            break;
        case TouchMoved:
            m_touchMoved=selector;
            break;
        case TouchMovedOutSide:
            m_touchMovedOutSide=selector;
            break;
        case TouchUpInSide:
            m_touchUpInSide=selector;
            break;
        case TouchUpSide:
            m_touchUpSide=selector;
            break;
        default:
            return;
    }
    
    m_target=target;
}

bool LMenu::isNormal(){
    
    return (m_touchState==LMenuStateNormal);
}

bool LMenu::isHighlighted(){
    
    return (m_touchState==LMenuStateHighlighted);
}

bool LMenu::isDisabled(){
    
    return (m_touchState==LMenuStateDisabled);
}

bool LMenu::isSelected(){
    
    return (m_touchState==LMenuStateSelected);
}

void LMenu::setTouchState(LMenuTouchState var){
    
    if (m_touchState==var) return;
    
    this->setNormalEffect();
    
    m_touchState=var;
    
    switch (m_touchState)
    {
        case LMenuStateHighlighted:
            this->setHighlightedEffect();
            break;
        case LMenuStateDisabled:
            this->setDisabledEffect();
            break;
        case LMenuStateSelected:
            this->setSelectedEffect();
            break;
        default:
            break;
    }
    
    
}

LMenuTouchState LMenu::getTouchState(){
    
    return m_touchState;
}

void LMenu::setTouchClick(bool var){
    
    if (m_touchClick==var) return;
    
    m_touchClick=var;
    
    if (m_touchClick)
    {
        this->setTouchState(LMenuStateHighlighted);
    }
    else
    {
        this->setTouchState(LMenuStateNormal);
    }
    
    if (var && !m_closeTapSound) PLAYSOUND;
}

bool LMenu::getTouchClick(){
    
    return m_touchClick;
}

bool LMenu::setTouchBegin(CCPoint point){
    
    if (m_touchBegin)
    {
        bool touchClick=((CCObject *)m_target->*m_touchBegin)(this,point);
        this->setTouchClick(touchClick);
        return touchClick;
    }
    
    this->setTouchClick(true);
    
    return true;
}

void LMenu::setTouchUpInSide(CCPoint point){
    
    if (m_touchUpInSide)
    {
        ((CCObject *)m_target->*m_touchUpInSide)(this,point);
    }
}

void LMenu::setTouchUpSide(CCPoint point){
    
    if (m_touchUpSide)
    {
        ((CCObject *)m_target->*m_touchUpSide)(this,point);
    }
}

void LMenu::setTouchMoved(cocos2d::CCPoint point){
    
    if (m_touchMoved)
    {
        ((CCObject *)m_target->*m_touchMoved)(this,point);
    }
}

void LMenu::setTouchMovedOutSide(cocos2d::CCPoint point){
    
    if (m_touchMovedOutSide)
    {
        ((CCObject *)m_target->*m_touchMovedOutSide)(this,point);
    }
}

void LMenu::setNormalEffect(){

    if (!m_specially) return;
    
    if (m_touchState==LMenuStateNormal) return;
    
    if (m_bgNormal)
        m_bgNormal->setVisible(true);
    
    if (m_spriteNormal)
        m_spriteNormal->setVisible(true);

    if (m_touchState==LMenuStateHighlighted)
    {
        
        if (m_bgHighlighted) m_bgHighlighted->setVisible(false);
        
        if (m_spriteHighlighted) m_spriteHighlighted->setVisible(false);
        
    }
    else if (m_touchState==LMenuStateDisabled)
    {
        
        if (m_bgDisabled) m_bgDisabled->setVisible(false);
        
        if (m_spriteDisabled) m_spriteDisabled->setVisible(false);
        
    }
    else if (m_touchState==LMenuStateSelected)
    {
        
        if (m_bgSelected) m_bgSelected->setVisible(false);
        
        if (m_spriteSelected) m_spriteSelected->setVisible(false);
        
    }
    
    if (!m_spriteHighlighted && !m_spriteSelected && !m_spriteDisabled)
        this->setNormalColor();

}

void LMenu::setHighlightedEffect(){

    if (!m_specially) return;
    
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
    
    if (!m_spriteHighlighted && !m_spriteSelected && !m_spriteDisabled)
        this->setHighlightedColor();
}

void LMenu::setDisabledEffect(){

    if (!m_specially) return;
    
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
        
    if (!m_spriteHighlighted && !m_spriteSelected && !m_spriteDisabled)
        this->setDisabledColor();
}

void LMenu::setSelectedEffect(){

    if (!m_specially) return;
    
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
        
    if (!m_spriteHighlighted && !m_spriteSelected && !m_spriteDisabled)
        this->setSelectedColor();
}

void LMenu::setNormalColor(){
    
    if (this->getChildren())
    {
        for (int i=0; i<this->getChildren()->count(); i++)
        {
            CCNode* node=(CCNode*)this->getChildren()->objectAtIndex(i);
            
            if (   node->isEqual(m_bgHighlighted)
                || node->isEqual(m_bgDisabled)
                || node->isEqual(m_bgSelected))
            {
                break;
            }
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(node))
            {
                ccColor3B color=_children->getColor();
                float red=color.r*2.0f;
                float green=color.g*2.0f;
                float blue=color.b*2.0f;
                _children->setColor(ccc3(red,green,blue));
                
            }
        }
    }
}

void LMenu::setHighlightedColor(){
    
    if (this->getChildren())
    {
        for (int i=0; i<this->getChildren()->count(); i++)
        {
            CCNode* node=(CCNode*)this->getChildren()->objectAtIndex(i);
            
            if (   node->isEqual(m_bgHighlighted)
                || node->isEqual(m_bgDisabled)
                || node->isEqual(m_bgSelected))
            {
                break;
            }
            
            if (CCRGBAProtocol* _children=dynamic_cast<CCRGBAProtocol*>(node))
            {
                ccColor3B color=_children->getColor();
                float red=color.r/2.0f;
                float green=color.g/2.0f;
                float blue=color.b/2.0f;
                _children->setColor(ccc3(red,green,blue));
                
            }
        }
    }
}

void LMenu::setDisabledColor(){

    this->setHighlightedColor();
}

void LMenu::setSelectedColor(){

    this->setHighlightedColor();
}

bool LMenu::isTextTagEqual(const char *text){
    
    return (m_textTag.compare(text)==0);
}

void LMenu::setPreferredSize(cocos2d::CCSize var){
    
    m_preferredSize=var;
    
    this->setContentSize(m_preferredSize);
    
    this->updateWithPoint();
    
    this->updateWithPreferredSize();
}

CCSize LMenu::getPreferredSize(){
    
    return m_preferredSize;
}

void LMenu::setSprite(LMenuTouchState touchState, CCNodeRGBA* var){
    
    if (!var) return;
    
    var->setAnchorPoint(CCPoint(0.5f, 0.5f));
    
    CCPoint point=CCPoint(m_preferredSize.width/2, m_preferredSize.height/2);
    
    this->setSprite(touchState, var, point);
}

void LMenu::setSprite(LMenuTouchState touchState, CCNodeRGBA* var, CCPoint point){
    
    if (!var) return;

    var->setPosition(point);
    this->addChild(var);
    
    CCPoint cPoint=CCPoint(m_preferredSize.width/2, m_preferredSize.height/2);
    
    if (touchState==LMenuStateNormal)
    {
        if (m_spriteNormal)
        {
            m_spriteNormal->removeFromParent();
            m_spriteNormal=NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteNPoint=point;
        }
        else
        {
            m_spriteNPoint=CCPointZero;
        }
        
        m_spriteNormal=var;
        
        return;
    }
    else if (touchState==LMenuStateHighlighted)
    {
        if (m_spriteHighlighted)
        {
            m_spriteHighlighted->removeFromParent();
            m_spriteHighlighted=NULL;
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
    else if (touchState==LMenuStateDisabled)
    {
        if (m_spriteDisabled)
        {
            m_spriteDisabled->removeFromParent();
            m_spriteDisabled=NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteDPoint=point;
        }
        else
        {
            m_spriteDPoint=CCPointZero;
        }
        
        m_spriteDisabled=var;
    }
    else if (touchState==LMenuStateSelected)
    {
        if (m_spriteSelected)
        {
            m_spriteSelected->removeFromParent();
            m_spriteSelected=NULL;
        }
        
        if (!cPoint.equals(point))
        {
            m_spriteSPoint=point;
        }
        else
        {
            m_spriteSPoint=CCPointZero;
        }
        
        m_spriteSelected=var;
    }
    
    var->setVisible(false);
}

CCNodeRGBA* LMenu::getSprite(LMenuTouchState touchState){
    
    switch (touchState)
    {
        case LMenuStateNormal:
            return m_spriteNormal;
            break;
        case LMenuStateHighlighted:
            return m_spriteHighlighted;
            break;
        case LMenuStateDisabled:
            return m_spriteDisabled;
            break;
        case LMenuStateSelected:
            return m_spriteSelected;
            break;
        default:
            return NULL;
            break;
    }
}


CCNode* LMenu::getBackGround(LMenuTouchState touchState){
    
    switch (touchState)
    {
        case LMenuStateNormal:
            return m_bgNormal;
            break;
        case LMenuStateHighlighted:
            return m_bgHighlighted;
            break;
        case LMenuStateDisabled:
            return m_bgDisabled;
            break;
        case LMenuStateSelected:
            return m_bgSelected;
            break;
        default:
            return NULL;
            break;
    }
}

void LMenu::setOpacity(GLubyte opacity)
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

void LMenu::setColor(const ccColor3B &color3){
    
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

ccColor3B& LMenu::getColor(){
    
    return m_color;
}
//  出自九秒社区 http://www.9miao.com
//
//  LyfIndicator.cpp
//  LTabelView
//
//  Created by LI YUANFENG on 13-3-13.
//
//

#include "LyfIndicator.h"

#define dvalue(x) x*2
#define BG_SIZE this->getContentSize()
#define CENTER_POINT CCPoint(this->getContentSize()/2)

#define HOMEPOINT CCPoint(this->getParentNode()->getContentSize().width\
+this->getContentSize().width*this->getScaleX()/2,\
parentNode->getContentSize().height/2)

LyfIndicator::LyfIndicator(CCLayerColor* layerColor)
:m_bg(NULL)
,m_indicator(NULL)
,m_indWidth(0)
,m_indHeight(0)
,m_scale(1)
,m_offPoint(CCPointZero)
{
    this->setParentNode(layerColor);
}

LyfIndicator::~LyfIndicator(){
}

LyfIndicator* LyfIndicator::createWithAddChild(CCLayerColor* layerColor, float scale){
    
    LyfIndicator* indicator=new LyfIndicator(layerColor);
    if (indicator && indicator->init())
    {
        indicator->initWithBg();
        indicator->initWithIndicator(scale);
        indicator->autorelease();
        return indicator;
    }
    
    CC_SAFE_DELETE(indicator);
    return NULL;
}

bool LyfIndicator::init(){
    
    if (!CCSprite::init()) return false;
    
    this->setPosition(HOMEPOINT);
    
    this->getParentNode()->addChild(this);
    
    return true;
}

void LyfIndicator::refresh(){
    
    this->setPosition(ccpAdd(HOMEPOINT, m_offPoint));
    
    if (m_bg) m_bg->removeFromParent();

    this->initWithBg();
    
    m_bg->setPosition(CENTER_POINT);
}

void LyfIndicator::setPointOff(cocos2d::CCPoint point){

    if (m_offPoint.x==point.x && m_offPoint.y==point.y)
        return;
    
    m_offPoint=point;
    this->setPosition(ccpAdd(HOMEPOINT, m_offPoint));
}

void LyfIndicator::initWithBg(){
    
    
    if (BACKGROUNDIMAGE)
    {
        m_bg=CCScale9Sprite::create(BACKGROUNDIMAGE);
    }
    else
    {
        m_bg=CCScale9Sprite::create();
    }
    
    CCSize bgSize = m_bg->getContentSize();
    bgSize.width = 16;
    CCSize size = this->getParentNode()->getContentSize();
    m_bg->setPreferredSize(CCSize(bgSize.width, size.height-OFF_HEIGHT/2));
    this->addChild(m_bg,-1);
    
    this->setContentSize(CCSize(bgSize.width, size.height-OFF_HEIGHT));
    
}

void LyfIndicator::initWithIndicator(float scale){
    
    m_indicator=CCScale9Sprite::create(INDICATORIMAGE);
    
    m_indWidth=m_indicator->getContentSize().width;
    
    CCRect capinsets=CCRect(4,
                            4,
                            2,
                            2);
    
    m_indicator->setCapInsets(capinsets);
    
    this->setScale(scale);
    
    this->addChild(m_indicator);
}

void LyfIndicator::setScale(float scale){
    
    m_scale=scale;
    
    if (m_scale>=1) m_scale=1;

    m_indHeight=BG_SIZE.height*scale;
    
    if (m_indHeight<m_indWidth) return;

    m_indicator->setPreferredSize(CCSizeMake(m_indWidth, m_indHeight));
}

void LyfIndicator::setIndicator(float windowSize, float contentSize, float lenght){

    float rop=lenght/contentSize;
    float upRop=1-2.5*(lenght-contentSize)/windowSize;
    float downRop=1-2.5*(-lenght)/windowSize;
    
    if (rop<0)
    {
        rop=0;
        upRop=1;
    }
    else if (rop>1)
    {
        rop=1;
        downRop=1;
    }
    else
    {
        downRop=1;
        upRop=1;
        
        if (m_scale==1) rop=0;
    }
    
    m_indHeight=BG_SIZE.height*m_scale*upRop*downRop;
    
    if (m_indHeight<m_indWidth)
    {
        m_indHeight=m_indWidth;
    }

    int y = (int)m_indicator->getPositionY()*10;
    
    m_indicator->setPreferredSize(CCSizeMake(m_indWidth, m_indHeight));
    m_indicator->setPositionY(BG_SIZE.height-m_indHeight/2-(BG_SIZE.height-m_indHeight)*rop);
    
    do
    {
        if (y == (int)m_indicator->getPositionY()*10)
        {
            CC_BREAK_IF(m_indicator->getActionByTag(0xfff));
            
            CC_BREAK_IF(m_indicator->getOpacity() < 255);
            
            CCDelayTime* delayTime = CCDelayTime::create(0.2f);
            CCFadeOut* fadeOut = CCFadeOut::create(0.3f);
            CCEaseSineOut* easeSineOut = CCEaseSineOut::create(fadeOut);
            CCSequence* actions = CCSequence::create(delayTime, easeSineOut, NULL);
            actions->setTag(0xfff);
            m_indicator->runAction(actions);
        }
        else
        {
            CC_BREAK_IF(m_indicator->getOpacity() == 255);
            
            m_indicator->stopAllActions();
            m_indicator->setOpacity(255);
        }
    }
    while (0);
    
    


}

void LyfIndicator::onEnter(){
    
    CCSprite::onEnter();
    
    m_bg->setPosition(CENTER_POINT);
    
    m_indicator->setPosition(CCPoint(CENTER_POINT.x, BG_SIZE.height-m_indicator->getContentSize().height/2));
}

void LyfIndicator::setOpacity(GLubyte opacity){
    
    CCSprite::setOpacity(opacity);
    m_indicator->setOpacity(opacity);
}
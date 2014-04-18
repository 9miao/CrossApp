//
//  LNoClickLayer.cpp
//  DDHero
//
//  Created by LI YUANFENG on 13-4-19.
//
//

#include "LNoClickLayer.h"

static LNoClickLayer* noClickLayer = NULL;

LNoClickLayer::LNoClickLayer()
:_offy(0)
{

}

LNoClickLayer::~LNoClickLayer()
{
    noClickLayer=NULL;
}

void LNoClickLayer::onExit()
{
    CCLayerColor::onExit();
}

void LNoClickLayer::onEnter()
{
    CCLayerColor::onEnter();
}

LNoClickLayer* LNoClickLayer::create()
{
    noClickLayer=new LNoClickLayer();
    if (noClickLayer && noClickLayer->init()) {
        noClickLayer->setTouchEnabled(true);
        noClickLayer->autorelease();
        return noClickLayer;
    }
    noClickLayer->release();
    return NULL;
}

LNoClickLayer* LNoClickLayer::shareLNoClickLayer()
{
    if (noClickLayer==NULL)
    {
        noClickLayer=LNoClickLayer::create();
        noClickLayer->_offy=0;
        CCDirector::sharedDirector()->getRunningScene()->addChild(noClickLayer,10000);
    }
    return noClickLayer;
}

LNoClickLayer* LNoClickLayer::shareLNoAllClickLayer()
{
    if (noClickLayer==NULL)
    {
        noClickLayer=LNoClickLayer::create();
        noClickLayer->_offy=0;
        CCDirector::sharedDirector()->getRunningScene()->addChild(noClickLayer,10000);
    }
    return noClickLayer;
}

bool LNoClickLayer::getIsExist()
{
    return (bool)noClickLayer;
}

void LNoClickLayer::registerWithTouchDispatcher()
{
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    touchDispatcher->addTargetedDelegate(this, -1000, true);
}

bool LNoClickLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint uipoint=pTouch->getLocationInView();
    CCPoint point=CCDirector::sharedDirector()->convertToGL(uipoint);
    
    if ( point.y>_offy)
    {
        return true;
    }
    
    return false;
}

void LRemoveNoClickLayer::RemoveNoClickLayer()
{
    if (noClickLayer)
    {
        noClickLayer->removeFromParent();
        noClickLayer=NULL;
    }
}
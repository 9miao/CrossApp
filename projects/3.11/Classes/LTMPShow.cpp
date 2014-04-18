//
//  LTMPShow.cpp
//  project
//
//  Created by 栗元峰 on 14-4-1.
//
//

#include "LTMPShow.h"

LTMPShow::LTMPShow()
{

}

LTMPShow::~LTMPShow()
{

}

void LTMPShow::onEnterTransitionDidFinish()
{
    LRootLayer::onEnterTransitionDidFinish();
}

void LTMPShow::onExitTransitionDidStart()
{
    LRootLayer::onExitTransitionDidStart();
}

bool LTMPShow::init()
{
    LRootLayer::init();
    
    this->setTouchEnabled(false);
    return true;
}

void LTMPShow::show()
{
    CCSprite* view = CCSprite::create("宝典2.png");
    view->setFrame(CCRect(50, 50, 100, 100));
    
    this->addChild(view);
}

bool LTMPShow::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    return true;
}
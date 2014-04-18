//
//  LTableBarController.cpp
//  project
//
//  Created by 栗元峰 on 14-3-18.
//
//

#include "LTableBarController.h"
#include "LNoClickLayer.h"
#include "MainInterface.h"
#include "LTMPShow.h"
static LTableBarController* _tableBarController = NULL;

LTableBarController::LTableBarController()
{
    MySocket::shareMySocket();
}

LTableBarController::~LTableBarController()
{
    
}

void LTableBarController::onEnterTransitionDidFinish()
{
    CCLayerColor::onEnterTransitionDidFinish();
    this->callBackTabBarMenu("CloseNormal.png");
}

LTableBarController* LTableBarController::shareTableBarController()
{
    if (_tableBarController == NULL)
    {
        _tableBarController = new LTableBarController();
        _tableBarController->init();
    }
    return _tableBarController;
}

void LTableBarController::shareNull()
{
    _tableBarController->release();
    _tableBarController = NULL;
}

bool LTableBarController::init()
{
    if (!CCLayerColor::init())
        return false;
    
    CCLayerColor* layerColor = CCLayerColor::createWithFrame(CCRect(0, 0, WINSIZE.width, WINSIZE.height));
    layerColor->setColor(ccc3(218, 213, 200));
    this->addChild(layerColor);

    CCSprite* sprite = CCSprite::create("9m.jpg");
    sprite->setPosition(ccpAdd(this->getContentSize()/2, CCPoint(0, 50)));
    layerColor->addChild(sprite);
    
    CCScale9Sprite* layerColor2 = CCScale9Sprite::create("navigationController_bg.png");
    layerColor2->setPreferredSize(CCSize(WINSIZE.width, 60));
    layerColor2->setFrame(CCRectMake(0, 900, 0, 0));
    this->addChild(layerColor2, 1);
    
    CCLabelTTF* ttf = CCLabelTTF::create("9秒社区HD", "Arial", 40);
    ttf->setPosition(layerColor2->getContentSize()/2);
    layerColor2->addChild(ttf);
    ttf->setColor(ccYELLOW);
    
    m_pLayerParent = CCNode::create();
    m_pLayerParent->setPosition(CCPoint(0, 100));
    this->addChild(m_pLayerParent);
    
    m_pGoBack = LMenu::createWithSprite(CCLabelTTF::create("返回", "Arial", 25));
    m_pGoBack->setPosition(CCPoint(40, 30));
    layerColor2->addChild(m_pGoBack);
    m_pGoBack->addTarget(this, LMenu_selector(LTableBarController::touchUpInSide), TouchUpInSide);
    m_pGoBack->setOpacity(0);
    
    LTabBarMenu* tabbarMenu = LTabBarMenu::create();
    tabbarMenu->setDelegate(this);
    tabbarMenu->setPosition(CCPointZero);
    tabbarMenu->addMenuWithTabBar("h.png");
    tabbarMenu->addMenuWithTabBar("e.png");
    tabbarMenu->addMenuWithTabBar("a.png");
    tabbarMenu->addMenuWithTabBar("o.png");
    tabbarMenu->addMenuWithTabBar("s.png");
    this->addChild(tabbarMenu, 1);
    
    return true;
}

void LTableBarController::replaceLayer(LRootLayer* layer)
{
    this->clearLayerVector();
    
    layer->setFrame(CCRectZero);
    m_pLayerParent->addChild(layer);
    m_pLayerVec.push_back(layer);
}

void LTableBarController::pushLayer(LRootLayer* layer)
{
    float x = WINSIZE.width;
    layer->setPosition(x + layer->getAnchorPointInPoints().x, layer->getAnchorPointInPoints().y);
    m_pLayerParent->addChild(layer);
    m_pLayerVec.push_back(layer);
    
    CCEaseSineIn* fadeOut = CCEaseSineIn::create(CCFadeOut::create(0.2f));
    CCEaseSineOut* scaleTo = CCEaseSineOut::create(CCScaleTo::create(0.25f, 0.5f));
    CCSpawn* spaw = CCSpawn::create(fadeOut, scaleTo, NULL);
    m_pLayerVec.at(m_pLayerVec.size() - 2)->runAction(spaw);
    
    CCPoint p = layer->getAnchorPointInPoints();
    CCDelayTime* delayTime = CCDelayTime::create(0.1f);
    CCEaseSineOut* moveTo = CCEaseSineOut::create(CCMoveTo::create(0.2f, p));
    CCCallFunc* func1 = CCCallFunc::create(this, callfunc_selector(LTableBarController::actionBegin));
    CCCallFunc* pushEnd = CCCallFunc::create(this, callfunc_selector(LTableBarController::pushEnd));
    CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(LTableBarController::actionEnd));
    CCSequence* actions = CCSequence::create(delayTime, func1, moveTo, pushEnd, func2, NULL);
    layer->runAction(actions);
}

void LTableBarController::popLayer()
{
    do
    {
        CC_BREAK_IF(m_pLayerVec.size() == 1);
        
        float x = WINSIZE.width;
    
        m_pLayerVec.at(m_pLayerVec.size()-2)->setPositionX(m_pLayerVec.at(m_pLayerVec.size()-2)->getAnchorPointInPoints().x);
        
        CCEaseSineOut* fadeIn = CCEaseSineOut::create(CCFadeIn::create(0.2f));
        CCEaseSineOut* scaleTo = CCEaseSineOut::create(CCScaleTo::create(0.25f, 1.0f));
        CCSpawn* spaw = CCSpawn::create(fadeIn, scaleTo, NULL);
        m_pLayerVec.at(m_pLayerVec.size() - 2)->runAction(spaw);
        
        CCPoint p = ccpAdd(m_pLayerVec.back()->getAnchorPointInPoints(), CCPoint(x, 0));
        CCEaseSineOut* moveTo = CCEaseSineOut::create(CCMoveTo::create(0.2f, p));
        CCCallFunc* func1 = CCCallFunc::create(this, callfunc_selector(LTableBarController::actionBegin));
        CCCallFunc* popEnd = CCCallFunc::create(this, callfunc_selector(LTableBarController::popEnd));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(LTableBarController::actionEnd));
        CCSequence* actions = CCSequence::create(func1, moveTo, popEnd, func2, NULL);
        m_pLayerVec.back()->runAction(actions);
    }
    while (0);
}

LRootLayer* LTableBarController::getLayer(unsigned int index)
{
    do
    {
        CC_BREAK_IF(index >= m_pLayerVec.size());
        
        return m_pLayerVec.at(index);
    }
    while (0);
    
    return NULL;
}

LRootLayer* LTableBarController::getCurrLayer()
{
    do
    {
        CC_BREAK_IF(0 == m_pLayerVec.size());
        
        return m_pLayerVec.back();
    }
    while (0);
    
    return NULL;
}

void LTableBarController::clearLayerVector()
{
    do
    {
        CC_BREAK_IF(m_pLayerVec.empty());
        m_pLayerVec.clear();
        m_pLayerParent->removeAllChildren();
    }
    while (0);
}

void LTableBarController::actionBegin()
{
    NOALLCLICK;
}

void LTableBarController::actionEnd()
{
    REMOVENOCLICK;
}

void LTableBarController::pushEnd()
{
    do
    {
        CC_BREAK_IF(m_pLayerVec.size() > 2);
        CCFadeIn* fadeIn = CCFadeIn::create(0.2f);
        m_pGoBack->runAction(fadeIn);
    }
    while (0);
    
    m_pLayerVec.back()->show();
    m_pLayerVec.at(m_pLayerVec.size()-2)->setPositionX(-1000);
    
}

void LTableBarController::popEnd()
{
    m_pLayerVec.back()->removeFromParent();
    m_pLayerVec.pop_back();
    
    do
    {
        CC_BREAK_IF(m_pLayerVec.size() == 0);
        CCFadeOut* fadeOut = CCFadeOut::create(0.2f);
        m_pGoBack->runAction(fadeOut);
    }
    while (0);
}

bool LTableBarController::touchUpInSide(LMenu* menu, CCPoint point)
{
    this->popLayer();
    return true;
}

void LTableBarController::callBackTabBarMenu(const char* fileName)
{
    if (0 == strcmp(fileName, "h.png") || 0 == strcmp(fileName, "e.png"))
    {
        MainInterface* layer = MainInterface::create();
        this->replaceLayer(layer);
        layer->settype(0);
    }
    else
    {
        LTMPShow* show = LTMPShow::create();
        this->replaceLayer(show);
        show->show();
    }
}
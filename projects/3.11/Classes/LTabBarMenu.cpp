//
//  LTabBarMenu.cpp
//  project
//
//  Created by 栗元峰 on 14-3-18.
//
//

#include "LTabBarMenu.h"
#include "LTableBarController.h"


LTabBarMenu::LTabBarMenu()
:m_pDelegate(NULL)
,m_nSelected(0)
,m_touchProtected(false)
{

}

LTabBarMenu::~LTabBarMenu()
{

}

bool LTabBarMenu::init()
{
    if (!CCSprite::init())
    {
        return false;
    }
    
    this->setContentSize(CCSize(WINSIZE.width, 100));
    this->setAnchorPoint(CCPointZero);
    
    CCScale9Sprite* bg = CCScale9Sprite::create("tabBarController_bg.png");
    bg->setFrame(CCRect(0, 0, m_obContentSize.width, m_obContentSize.height));
    this->addChild(bg, -2);
    
    m_pSelected = CCScale9Sprite::create("tabBarController_selected_bg.png");
    this->addChild(m_pSelected, -1);
    
    return true;
}

void LTabBarMenu::onEnterTransitionDidFinish()
{
    CCSprite::onEnterTransitionDidFinish();
    
    float size = m_obContentSize.width / m_pMenuVec.size();
    
    for (unsigned int i=0; i<m_pMenuVec.size(); i++)
    {
        LMenu* menu = m_pMenuVec.at(i);
        menu->setPosition(CCPoint(size * (i + 0.5f), m_obContentSize.height / 2));
        menu->setPreferredSize(CCSize(size, m_obContentSize.height));
        
        if (i == 0)
        {
            m_pSelected->setFrame(CCRectMake(0, 0, size, m_obContentSize.height));
            //m_pSelected->setPosition(ccpSub(menu->getPosition(), m_pSelected->getContentSize()/2));
        }
    }
}

void LTabBarMenu::addMenuWithTabBar(const char* fileName)
{
    CCSprite* normal = CCSprite::create(fileName);
    normal->setColor(ccc3(170, 170, 170));
    CCSprite* highlighted = CCSprite::create(fileName);
    CCSprite* selected = CCSprite::create(fileName);
    LMenu* menu = LMenu::createWithRGBAProtocol(normal);
    menu->setBackGround(LMenuStateHighlighted, highlighted);
    menu->setBackGround(LMenuStateSelected, selected);
    this->addChild(menu);
    menu->addTarget(this, LMenu_selector(LTabBarMenu::touchUpInSide), TouchUpInSide);
    menu->setTextTag(fileName);
    menu->setTag(0xffff + m_pMenuVec.size());
    m_pMenuVec.push_back(menu);
}

bool LTabBarMenu::touchUpInSide(LMenu* menu, CCPoint point)
{
    if (m_touchProtected)
        return false;
    
    this->touchProtected();
    
    for (unsigned int i=0; i<m_pMenuVec.size(); i++)
    {
        if (menu->isEqual(m_pMenuVec.at(i)))
        {
            m_pMenuVec.at(i)->setTouchState(LMenuStateSelected);
        }
        else
        {
            m_pMenuVec.at(i)->setTouchState(LMenuStateNormal);
        }
    }
    
    m_nSelected = menu->getTag() - 0xffff;
    
    if (m_pDelegate)
    {
        m_pDelegate->callBackTabBarMenu(menu->getTextTag().c_str());
    }
    
    m_pSelected->stopAllActions();
    CCPoint p = menu->getPosition();
    CCMoveTo* moveTo = CCMoveTo::create(0.2f, p);
    CCEaseSineOut* easeBack = CCEaseSineOut::create(moveTo);
    m_pSelected->runAction(easeBack);
    
    return true;
}

void LTabBarMenu::touchProtected()
{
    this->stopActionByTag(0xff99);
    m_touchProtected = true;
    CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(LTabBarMenu::untouchProtected));
    CCDelayTime* delay = CCDelayTime::create(0.2f);
    CCSequence* actions = CCSequence::create(delay, callFunc, NULL);
    actions->setTag(0xff99);
    this->runAction(actions);
}

void LTabBarMenu::untouchProtected()
{
    m_touchProtected = false;
}
//
//  CABar.cpp
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#include "CABar.h"
#include "sprite_nodes/CCScale9Sprite.h"
#include "CCDirector.h"
NS_CC_BEGIN

#pragma CANavigationBar

CANavigationBar::CANavigationBar()
:m_pBackGround(NULL)
,m_pBackGroundImage(NULL)
,m_pTitle(NULL)
,m_pBackButton(NULL)
,m_pDelegate(NULL)
{

}

CANavigationBar::~CANavigationBar()
{

}

bool CANavigationBar::init()
{
    if (!CAView::init())
    {
        return false;
    }
    this->setOpacity(0);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(CCSize(winSize.width, MIN(winSize.width, winSize.height) * 0.1f));
    
    return true;
}

void CANavigationBar::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    if (m_pBackGround == NULL)
    {
        this->showBackGround();
        
        this->showTitle();
        
        this->showBackButton();
    }
}

void CANavigationBar::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

void CANavigationBar::showBackGround()
{
    if (m_pBackGround)
    {
        m_pBackGround->removeFromParent();
        m_pBackGround = NULL;
    }
    
    if (m_pBackGroundImage == NULL)
    {
        m_pBackGroundImage = CCTexture2D::create("navigationController_bg.png");
    }

    m_pBackGround = CCScale9Sprite::createWithTexture(m_pBackGroundImage);
    ((CCScale9Sprite*)m_pBackGround)->setPreferredSize(m_obContentSize);
    m_pBackGround->setFrame(CCRectZero);
    this->addChild(m_pBackGround, 0);
}

void CANavigationBar::showTitle()
{
    if (m_pTitle == NULL)
    {
        m_pTitle = CCLabelTTF::create("", "Arial", 40);
        m_pTitle->setPosition(m_obContentSize/2);
        this->addChild(m_pTitle, 1);
    }
    
    if (m_pItems.empty() == false)
    {
        ((CCLabelTTF*)m_pTitle)->setString(m_pItems.back()->getTitle().c_str());
    }
}

void CANavigationBar::showBackButton()
{
    if (m_pBackButton == NULL)
    {
        m_pBackButton = CAButton::createWithFrame(CCRect(20, 0, 50, 50));
        CAImageView* imageView = CAImageView::createWithTexture(CCTexture2D::create("button_left.png"));
        m_pBackButton->setBackGround(CAControlStateNormal, imageView);
        m_pBackButton->setPositionY(m_obContentSize.height/2);
        this->addChild(m_pBackButton, 1);
        m_pBackButton->addTarget(this, CAButton_selector(CANavigationBar::goBack), TouchUpInSide);
    }
    
    if (m_pItems.size() <= 1)
    {
        m_pBackButton->setVisible(false);
    }
    else
    {
        m_pBackButton->setVisible(true);
    }
}

bool CANavigationBar::goBack(CAButton* btn, CCPoint point)
{
    if (m_pDelegate)
    {
        m_pDelegate->navigationPopViewController(this, true);
    }

	return true;
}

void CANavigationBar::pushItem(CANavigationBarItem* item)
{
    if (item == NULL)
    {
        item = CANavigationBarItem::create(CCString::createWithFormat("item%ld",m_pItems.size())->getCString());
    }

//    do
//    {
//        CC_BREAK_IF(item->getBackButtonTitle().compare("") != 0);
//        CC_BREAK_IF(m_pItems.empty());
//        item->setBackButtonTitle(m_pItems.back()->getTitle());
//    }
//    while (0);

    item->retain();
    m_pItems.push_back(item);
    
    this->showTitle();
    this->showBackButton();
}

void CANavigationBar::popItem()
{
    m_pItems.back()->autorelease();
    m_pItems.pop_back();
    
    this->showTitle();
    this->showBackButton();
}

#pragma CATabBar

CATabBar::CATabBar()
:m_pBackGround(NULL)
,m_pBackGroundImage(NULL)
,m_pSelectedBackGround(NULL)
,m_pSelectedBackGroundImage(NULL)
,m_pSelectedIndicator(NULL)
,m_pSelectedIndicatorImage(NULL)
,m_pSelectedItem(NULL)
,m_nMaxShowCount(5)
,m_cItemSize(CCSizeZero)
,m_nSelectedIndex(0)
,m_pDelegate(NULL)
{
    
}

CATabBar::~CATabBar()
{
    std::vector<CATabBarItem*>::iterator itr;
    for (itr=m_pItems.begin(); itr!=m_pItems.end(); itr++)
    {
        (*itr)->autorelease();
    }
    m_pItems.clear();
}

bool CATabBar::init(const std::vector<CATabBarItem*>& items)
{
    if (!CAView::init())
    {
        return false;
    }
    this->setOpacity(0);
    this->setTouchEnabled(true);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    this->setContentSize(CCSize(winSize.width, MIN(winSize.width * 0.15f, winSize.height * 0.1f)));
    
    this->setItems(items);
    
    return true;
}

void CATabBar::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    if (m_pBackGround == NULL)
    {
        this->showBackGround();
        
        this->showItems();
        
        this->showSelectedBackGround();
        
        this->setSelectedAtIndex(0);
    }
}

void CATabBar::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

CATabBar* CATabBar::create(const std::vector<CATabBarItem*>& items)
{
    CATabBar* tabBar = new CATabBar();
    if (tabBar && tabBar->init(items))
    {
        tabBar->autorelease();
        return tabBar;
    }
    CC_SAFE_DELETE(tabBar);
    return NULL;
}

void CATabBar::setItems(const std::vector<CATabBarItem*>& items)
{
    do
    {
        CC_BREAK_IF(items.size() == 0);
        m_pItems.insert(m_pItems.begin(), items.begin(), items.end());
        
        std::vector<CATabBarItem*>::iterator itr;
        for (itr=m_pItems.begin(); itr!=m_pItems.end(); itr++)
        {
            (*itr)->retain();
        }
    }
    while (0);
}

void CATabBar::showBackGround()
{
    if (m_pBackGround)
    {
        m_pBackGround->removeFromParent();
        m_pBackGround=NULL;
    }
    
    if (m_pBackGroundImage == NULL)
    {
        m_pBackGroundImage = CCTexture2D::create("tabBarController_bg.png");
    }
    
    m_pBackGround = CCScale9Sprite::createWithTexture(m_pBackGroundImage);
    ((CCScale9Sprite*)m_pBackGround)->setPreferredSize(m_obContentSize);
    m_pBackGround->setFrame(CCRectZero);
    this->addChild(m_pBackGround);
}

void CATabBar::showItems()
{
    do
    {
        CC_BREAK_IF(m_pViews.empty() == false);
        
        unsigned int count = MIN(m_nMaxShowCount, m_pItems.size());
        
        float width = m_obContentSize.width / count;
        float height = m_obContentSize.height;
        m_cItemSize = CCSize(width, height);
        
        for (unsigned int i=0; i<count; i++)
        {
            CAView* view = CAView::createWithFrame(CCRect(i * width, 0, width, height));
            view->setOpacity(0);
            this->addChild(view, 3);
            m_pViews.push_back(view);
            
            CAImageView* imageView = CAImageView::createWithTexture(m_pItems.at(i)->getImage());
            imageView->setPosition(view->getContentSize()/2);
            view->addChild(imageView, 0, 0xffff);
            CCSize imageViewSize = imageView->getBounds().size;
            float scaleX = width / imageViewSize.width * 2/3;
            float scaleY = height / imageViewSize.height * 2/3;
            float scale = MIN(scaleX, scaleY);
            scale = MIN(scale, 1.0f);
            imageViewSize = ccpMult(imageViewSize, scale);
            imageView->setBoundsSize(imageViewSize);
        }
    }
    while (0);
}

void CATabBar::showSelectedBackGround()
{
    if (m_pSelectedBackGround)
    {
        m_pSelectedBackGround->removeFromParent();
        m_pSelectedBackGround=NULL;
    }
    
    if (m_pSelectedBackGroundImage == NULL)
    {
        m_pSelectedBackGroundImage = CCTexture2D::create("tabBarController_selected_bg.png");
    }

    m_pSelectedBackGround = CCScale9Sprite::createWithTexture(m_pSelectedBackGroundImage);
    ((CCScale9Sprite*)m_pSelectedBackGround)->setPreferredSize(m_cItemSize);
    this->addChild(m_pSelectedBackGround, 1);
}

void CATabBar::showSelectedIndicator()
{
    if (m_pSelectedIndicator)
    {
        m_pSelectedIndicator->removeFromParent();
        m_pSelectedIndicator=NULL;
    }
    
    if (m_pSelectedIndicatorImage == NULL)
    {
        m_pSelectedIndicatorImage = CCTexture2D::create("tabBarController_selected_bottom.png");
    }
    
    m_pSelectedIndicator = CCScale9Sprite::createWithTexture(m_pSelectedIndicatorImage);
    ((CCScale9Sprite*)m_pSelectedIndicator)->setPreferredSize(CCSize(m_cItemSize.width, m_cItemSize.height / 10));
    m_pSelectedIndicator->setAnchorPoint(CCPointZero);
    this->addChild(m_pSelectedIndicator, 2);
}

void CATabBar::setSelectedAtIndex(int index)
{
    do
    {
        CC_BREAK_IF(index < 0);
        CC_BREAK_IF(index >= m_pItems.size());

        if (m_pSelectedItem && m_pSelectedItem->getSelectedImage())
        {
            CAView* viewLast = m_pViews.at(m_nSelectedIndex);
            if (CAImageView* imageView = dynamic_cast<CAImageView*>(viewLast->getChildByTag(0xffff)))
            {
                imageView->setTexture(m_pSelectedItem->getImage());
            }
        }
        
        m_nSelectedIndex = index;
        m_pSelectedItem = m_pItems.at(m_nSelectedIndex);
        m_pSelectedBackGround->setFrame(m_pViews.at(m_nSelectedIndex)->getFrame());
        
        if (m_pSelectedIndicator)
        {
            ((CCScale9Sprite*)m_pSelectedIndicator)->setPreferredSize(CCSize(m_cItemSize.width, m_cItemSize.height / 10));
            m_pSelectedIndicator->stopAllActions();
            CCPoint p = m_pSelectedBackGround->getFrame().origin;
            CCMoveTo* moveTo = CCMoveTo::create(0.3f, p);
            CCEaseSineOut* easeBack = CCEaseSineOut::create(moveTo);
            m_pSelectedIndicator->runAction(easeBack);
        }
        
        if (m_pSelectedItem->getSelectedImage())
        {
            CAView* view = m_pViews.at(m_nSelectedIndex);
            if (CAImageView* imageView = dynamic_cast<CAImageView*>(view->getChildByTag(0xffff)))
            {
                imageView->setTexture(m_pSelectedItem->getSelectedImage());
            }
        }
        
        m_pDelegate->tabBarSelectedItem(this, m_pSelectedItem, m_nSelectedIndex);
    }
    while (0);
}

bool CATabBar::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (!this->isVisible())
    {
        return false;
    }
    
    if (!this->getBounds().containsPoint(point))
    {
        return false;
    }
    
    
    
    
    return true;
}

void CATabBar::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CATabBar::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    this->setSelectedAtIndex((int)(point.x / m_cItemSize.width));
}

void CATabBar::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}


NS_CC_END
//
//  CABar.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CABar.h"
#include "sprite_nodes/CAScale9ImageView.h"
#include "CCDirector.h"
#include "label_nodes/CCLabelTTF.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "touch_dispatcher/CCTouch.h"
NS_CC_BEGIN

#pragma CANavigationBar

CANavigationBar::CANavigationBar()
:m_pBackGround(NULL)
,m_pTitle(NULL)
,m_pLeftButton(NULL)
,m_pRightButton(NULL)
,m_pDelegate(NULL)
,m_pBackGroundImage(NULL)
{

}

CANavigationBar::~CANavigationBar()
{
    CC_SAFE_RELEASE_NULL(m_pBackGroundImage);
}

bool CANavigationBar::init()
{
    if (!CAView::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(CCSize(winSize.width, MIN(winSize.width * 0.15f, winSize.height * 0.1f)));
    
    return true;
}

void CANavigationBar::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();

    if (m_pBackGround == NULL)
    {
        this->showBackGround();
        this->updateNavigationBar();
    }
}

void CANavigationBar::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

void CANavigationBar::replaceItemAtIndex(size_t index, CANavigationBarItem* item)
{
    if (index < m_pItems.size())
    {
        CANavigationBarItem* oldItem = m_pItems.at(index);
        m_pItems.at(index) = item;
        CC_SAFE_RELEASE(oldItem);
        CC_SAFE_RETAIN(item);
        if (index == m_pItems.size() - 1)
        {
            this->updateNavigationBar();
        }
    }
}

void CANavigationBar::updateNavigationBar()
{
    this->showTitle();
    this->showLeftButton();
    this->showRightButton();
}

void CANavigationBar::showBackGround()
{
    if (m_pBackGround)
    {
        m_pBackGround->removeFromSuperview();
        m_pBackGround = NULL;
    }
    
    if (m_pBackGroundImage == NULL)
    {
        this->setBackGroundImage(CAImage::create("source_material/navigation_bg.png"));
    }

    m_pBackGround = CAScale9ImageView::createWithImage(this->getBackGroundImage());
    ((CAScale9ImageView*)m_pBackGround)->setFrame(this->getBounds());
    this->insertSubview(m_pBackGround, -1);
}

void CANavigationBar::showTitle()
{
    if (m_pTitle == NULL)
    {
        int fontSize = this->getBounds().size.height * 0.35f;
        
        m_pTitle = CCLabelTTF::create("", "fonts/arial.ttf", fontSize);
        m_pTitle->setColor(CAColor_white);
        m_pTitle->setAnchorPoint(CCPoint(0.5f, 0.5f));
        m_pTitle->setCenterOrigin(this->getBounds().size/2);
        this->addSubview(m_pTitle);
    }
    
    if (!m_pItems.empty())
    {
        std::string str = m_pItems.back()->getTitle();
        CCLog("%s",str.c_str());
        ((CCLabelTTF*)m_pTitle)->setString(str.c_str());

        float width = this->getBounds().size.width - this->getBounds().size.height * 3;
        
        if (m_pTitle->getFrame().size.width > width)
        {
            CCRect rect = m_pTitle->getBounds();
            rect.size.width = width;
            m_pTitle->setImageRect(rect);
        }
    }
}

void CANavigationBar::showLeftButton()
{
    if (m_pLeftButton)
    {
        m_pLeftButton->removeFromSuperview();
        m_pLeftButton = NULL;
    }
    
    CABarButtonItem* buttonItem = m_pItems.back()->getLeftButtonItem();
    
    CC_RETURN_IF(m_pItems.size() <= 1 && buttonItem == NULL);
    
    CCRect rect = this->getBounds();
    rect.size.width = rect.size.height;
    
    m_pLeftButton = CAButton::createWithFrame(rect, CAButtonTypeCustom);
    this->addSubview(m_pLeftButton);
    
    if (buttonItem)
    {
        m_pLeftButton->setTitleForState(CAControlStateNormal, buttonItem->getTitle());
        m_pLeftButton->setTitleColorForState(CAControlStateNormal, CAColor_white);
        m_pLeftButton->setTitleForState(CAControlStateHighlighted, buttonItem->getTitle());
        m_pLeftButton->setTitleColorForState(CAControlStateHighlighted, ccc4(255, 255, 200, 255));
        m_pLeftButton->setImageForState(CAControlStateNormal, buttonItem->getImage());
        if (buttonItem->getHighlightedImage())
        {
            m_pLeftButton->setImageForState(CAControlStateHighlighted, buttonItem->getHighlightedImage());
        }
        else
        {
            m_pLeftButton->setImageColorForState(CAControlStateHighlighted, ccc4(255, 255, 200, 255));
        }
    }
    else
    {
        m_pLeftButton->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_left_white.png"));
        m_pLeftButton->setImageColorForState(CAControlStateHighlighted, ccc4(255, 255, 200, 255));
    }
    
    m_pLeftButton->setControlStateNormal();
    
    CCObject* target;
    SEL_CAControl sel;
    if (buttonItem && buttonItem->getTarget())
    {
        target = buttonItem->getTarget();
        sel = buttonItem->getSel();
    }
    else
    {
        target = this;
        sel = CAControl_selector(CANavigationBar::goBack);
    }
    m_pLeftButton->addTarget(target, sel, CAControlEventTouchUpInSide);
}

void CANavigationBar::showRightButton()
{
    if (m_pRightButton)
    {
        m_pRightButton->removeFromSuperview();
        m_pRightButton = NULL;
    }
    
    CABarButtonItem* buttonItem = m_pItems.back()->getRightButtonItem();
    
    if (buttonItem)
    {
        CCRect rect = this->getBounds();
        rect.origin.x = rect.size.width - rect.size.height;
        rect.size.width = rect.size.height;
    
        m_pRightButton = CAButton::createWithFrame(rect, CAButtonTypeCustom);
        this->addSubview(m_pRightButton);
        m_pRightButton->setTitleForState(CAControlStateNormal, buttonItem->getTitle());
        m_pRightButton->setTitleColorForState(CAControlStateNormal, CAColor_white);
        m_pRightButton->setTitleForState(CAControlStateHighlighted, buttonItem->getTitle());
        m_pRightButton->setTitleColorForState(CAControlStateHighlighted, ccc4(255, 255, 200, 255));
        m_pRightButton->setImageForState(CAControlStateNormal, buttonItem->getImage());
        if (buttonItem->getHighlightedImage())
        {
            m_pRightButton->setImageForState(CAControlStateHighlighted, buttonItem->getHighlightedImage());
        }
        else
        {
            m_pRightButton->setImageColorForState(CAControlStateHighlighted, ccc4(255, 255, 200, 255));
        }
        
        m_pRightButton->setControlStateNormal();
        
        CCObject* target;
        SEL_CAControl sel;
        if (buttonItem->getTarget())
        {
            target = buttonItem->getTarget();
            sel = buttonItem->getSel();
            m_pRightButton->addTarget(target, sel, CAControlEventTouchUpInSide);
        }
    }
}

void CANavigationBar::goBack(CAControl* btn, CCPoint point)
{
    if (m_pDelegate)
    {
        m_pDelegate->navigationPopViewController(this, true);
    }
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
    
    this->updateNavigationBar();
}

void CANavigationBar::popItem()
{
    m_pItems.back()->autorelease();
    m_pItems.pop_back();
    
    if (!m_pItems.empty())
    {
        this->showTitle();
        this->showLeftButton();
        this->showRightButton();
    }
}

#pragma CATabBar

CATabBar::CATabBar()
:m_pBackGround(NULL)
,m_pBackGroundImage(NULL)
,m_pSegmentedControl(NULL)
,m_pSelectedBackGroundImage(NULL)
,m_pSelectedIndicator(NULL)
,m_pSelectedIndicatorImage(NULL)
,m_pSelectedItem(NULL)
,m_nMaxShowCount(5)
,m_cItemSize(CCSizeZero)
,m_nSelectedIndex(0)
,m_sTitleColor(CAColor_white)
,m_sSelectedTitleColor(ccc4(50, 193, 255, 255))
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
    
    CC_SAFE_RELEASE_NULL(m_pBackGroundImage);
    CC_SAFE_RELEASE_NULL(m_pSelectedBackGroundImage);
    CC_SAFE_RELEASE_NULL(m_pSelectedIndicatorImage);
}

bool CATabBar::init(const std::vector<CATabBarItem*>& items)
{
    if (!CAView::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    
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
        
        this->updateTabBar();
        
        this->setSelectedAtIndex(m_nSelectedIndex);
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

void CATabBar::replaceItemAtIndex(size_t index, CATabBarItem* item)
{
    if (index < m_pItems.size())
    {
        CATabBarItem* oldItem = m_pItems.at(index);
        m_pItems.at(index) = item;
        CC_SAFE_RELEASE(oldItem);
        CC_SAFE_RETAIN(item);
        this->updateTabBar();
    }
}

void CATabBar::updateTabBar()
{
    this->showItems();
}

void CATabBar::showBackGround()
{
    if (m_pBackGround)
    {
        m_pBackGround->removeFromSuperview();
        m_pBackGround=NULL;
    }
    
    if (m_pBackGroundImage == NULL)
    {
        m_pBackGroundImage = CAImage::create("source_material/tabBar_bg.png");
    }
    
    m_pBackGround = CAScale9ImageView::createWithImage(m_pBackGroundImage);
    ((CAScale9ImageView*)m_pBackGround)->setFrame(this->getBounds());
    this->insertSubview(m_pBackGround, -1);
}

void CATabBar::showItems()
{
    unsigned int count = MIN(m_nMaxShowCount, m_pItems.size());
    m_pSegmentedControl = CASegmentedControl::createWithFrame(this->getBounds(), count);
    this->addSubview(m_pSegmentedControl);
    m_pSegmentedControl->addTarget(this, CAControl_selector(CATabBar::setTouchSelected));
    for (int i=0; i<count; i++)
    {
        m_pSegmentedControl->setTitleAtIndex(m_pItems.at(i)->getTitle().c_str(), i, CAControlStateAll);
        m_pSegmentedControl->setTitleColorAtIndex(m_sTitleColor, i, CAControlStateAll);
        m_pSegmentedControl->setTitleColorAtIndex(m_sSelectedTitleColor, i, CAControlStateHighlighted);
        m_pSegmentedControl->setTitleColorAtIndex(m_sSelectedTitleColor, i, CAControlStateSelected);
        m_pSegmentedControl->setImageAtIndex(m_pItems.at(i)->getImage(), i, CAControlStateNormal);
        CAImage* selectedImage = m_pItems.at(i)->getSelectedImage() ? m_pItems.at(i)->getSelectedImage() : m_pItems.at(i)->getImage();
        m_pSegmentedControl->setImageAtIndex(selectedImage, i, CAControlStateHighlighted);
        m_pSegmentedControl->setImageAtIndex(selectedImage, i, CAControlStateSelected);
        m_pSegmentedControl->setBackgroundImageAtIndex(NULL, i, CAControlStateNormal);
        if (m_pSelectedBackGroundImage == NULL)
        {
            m_pSelectedBackGroundImage = CAImage::create("source_material/tabBar_selected_bg.png");
        }
        m_pSegmentedControl->setBackgroundImageAtIndex(m_pSelectedBackGroundImage, i, CAControlStateHighlighted);
        m_pSegmentedControl->setBackgroundImageAtIndex(m_pSelectedBackGroundImage, i, CAControlStateSelected);
    }
    m_cItemSize = m_pSegmentedControl->getItemSize();

}

void CATabBar::showSelectedIndicator()
{
    if (m_pSelectedIndicator)
    {
        m_pSelectedIndicator->removeFromSuperview();
        m_pSelectedIndicator=NULL;
    }
    
    if (m_pSelectedIndicatorImage == NULL)
    {
        m_pSelectedIndicatorImage = CAImage::create("source_material/tabBar_selected_indicator.png");
    }
    
    CCRect rect = CCRect(0,
                         this->getBounds().size.height,
                         m_cItemSize.width,
                         m_cItemSize.height / 10);
    
    CAScale9ImageView* selectedIndicator= CAScale9ImageView::createWithImage(m_pSelectedIndicatorImage);
    CCRect insetRect;
    insetRect.origin = selectedIndicator->getBounds().size / 2;
    insetRect.origin = ccpSub(insetRect.origin, CCPoint(1, 1));
    insetRect.size = CCPoint(2, 2);
    selectedIndicator->setCapInsets(insetRect);
    selectedIndicator->setFrame(rect);
    this->insertSubview(selectedIndicator, 1);
    m_pSelectedIndicator = selectedIndicator;
}

void CATabBar::setSelectedAtIndex(int index)
{
    CC_RETURN_IF(index < 0);
    CC_RETURN_IF(index >= m_pItems.size());
    
    m_nSelectedIndex = index;
    m_pSelectedItem = m_pItems.at(m_nSelectedIndex);
    
    CC_RETURN_IF(!m_bRunning);
    
    if (m_pSelectedIndicator)
    {
        CCRect rect = m_pSelectedIndicator->getFrame();
        rect.size = CCSize(m_cItemSize.width, m_cItemSize.height / 10);
        ((CAScale9ImageView*)m_pSelectedIndicator)->setFrame(rect);
        m_pSelectedIndicator->stopAllActions();
        CCPoint p = m_cItemSize;
        p.y -= m_pSelectedIndicator->getFrame().size.height;
        p.x *= m_nSelectedIndex;
        CCFrameOrginTo* moveTo = CCFrameOrginTo::create(0.3f, p);
        CCEaseSineOut* easeBack = CCEaseSineOut::create(moveTo);
        m_pSelectedIndicator->runAction(easeBack);
    }
    
    m_pSegmentedControl->setSelectedAtIndex(index);
    m_pDelegate->tabBarSelectedItem(this, m_pSelectedItem, m_nSelectedIndex);
}

void CATabBar::setTouchSelected(cocos2d::CAControl *control, cocos2d::CCPoint point)
{
    this->setSelectedAtIndex(m_pSegmentedControl->getselectedIndex());
}


NS_CC_END
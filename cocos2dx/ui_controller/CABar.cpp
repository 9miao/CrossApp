//
//  CABar.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
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
    CC_SAFE_RELEASE_NULL(m_pLeftButton);
    CC_SAFE_RELEASE_NULL(m_pRightButton);
}

bool CANavigationBar::init()
{
    if (!CAView::init())
    {
        return false;
    }
    this->setOpacity(0);
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
        
        this->showTitle();
        
        this->showLeftButton();
        
        this->showRightButton();
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
        m_pBackGround->removeFromSuperview();
        m_pBackGround = NULL;
    }
    
    if (m_pBackGroundImage == NULL)
    {
        this->setBackGroundImage(CAImage::create("navigationController_bg.png"));
    }

    m_pBackGround = CAScale9ImageView::createWithImage(this->getBackGroundImage());
    ((CAScale9ImageView*)m_pBackGround)->setPreferredSize(m_obContentSize);
    m_pBackGround->setFrame(CCRectZero);
    this->insertSubview(m_pBackGround, -1);
}

void CANavigationBar::showTitle()
{
    if (m_pTitle == NULL)
    {
        int fontSize = this->getBounds().size.height * 0.4f;
        
        m_pTitle = CCLabelTTF::create("", "fonts/arial.ttf", fontSize);
        m_pTitle->setColor(ccWHITE);
        m_pTitle->setAnchorPoint(CCPoint(0.5f, 0.5f));
        m_pTitle->setCenterOrigin(m_obContentSize/2);
        this->addSubview(m_pTitle);
    }
    
    if (m_pItems.empty() == false)
    {
        ((CCLabelTTF*)m_pTitle)->setString(m_pItems.back()->getTitle().c_str());
        
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
    if (m_pLeftButton == NULL)
    {
        CCRect rect = this->getBounds();
        rect.size.width = rect.size.height * 1.414f;
        
        m_pLeftButton = CAButton::createWithFrame(rect, CAButtonTypeCustom);
        this->addSubview(m_pLeftButton);
    }
    
    this->updateLeftButton();
}

void CANavigationBar::showRightButton()
{
    if (m_pRightButton == NULL)
    {
        CCRect rect = this->getBounds();
        rect.origin.x = rect.size.width - rect.size.height * 1.414f;
        rect.size.width = rect.size.height * 1.414f;
        
        m_pRightButton = CAButton::createWithFrame(rect, CAButtonTypeCustom);
        this->addSubview(m_pRightButton);
    }
    
    this->updateRightButton();
}

void CANavigationBar::updateLeftButton()
{
    m_pLeftButton->removeAllTargets();
    CABarButtonItem* buttonItem = m_pItems.back()->getLeftButtonItem();
    
    if (buttonItem)
    {
        m_pLeftButton->setTitleForState(CAControlStateNormal, buttonItem->getTitle());
        m_pLeftButton->setTitleColorForState(CAControlStateNormal, ccWHITE);
        m_pLeftButton->setTitleForState(CAControlStateHighlighted, buttonItem->getTitle());
        m_pLeftButton->setTitleColorForState(CAControlStateHighlighted, ccc3(255, 255, 200));
        m_pLeftButton->setImageForState(CAControlStateNormal, buttonItem->getImage());
        if (buttonItem->getHighlightedImage())
        {
            m_pLeftButton->setImageForState(CAControlStateHighlighted, buttonItem->getHighlightedImage());
        }
        else
        {
            m_pLeftButton->setImageColorForState(CAControlStateHighlighted, ccc3(255, 255, 200));
        }
    }
    else
    {
        m_pLeftButton->setImageForState(CAControlStateNormal, CAImage::create("button_left.png"));
        m_pLeftButton->setImageColorForState(CAControlStateHighlighted, ccc3(255, 255, 200));
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
    
    if (m_pItems.size() <= 1)
    {
        m_pLeftButton->setVisible(false);
    }
    else
    {
        m_pLeftButton->setVisible(true);
    }
}

void CANavigationBar::updateRightButton()
{
    m_pRightButton->removeAllTargets();
    CABarButtonItem* buttonItem = m_pItems.back()->getRightButtonItem();
    
    if (buttonItem)
    {
        m_pRightButton->setTitleForState(CAControlStateNormal, buttonItem->getTitle());
        m_pRightButton->setTitleColorForState(CAControlStateNormal, ccWHITE);
        m_pRightButton->setTitleForState(CAControlStateHighlighted, buttonItem->getTitle());
        m_pRightButton->setTitleColorForState(CAControlStateHighlighted, ccc3(255, 255, 200));
        m_pRightButton->setImageForState(CAControlStateNormal, buttonItem->getImage());
        if (buttonItem->getHighlightedImage())
        {
            m_pRightButton->setImageForState(CAControlStateHighlighted, buttonItem->getHighlightedImage());
        }
        else
        {
            m_pRightButton->setImageColorForState(CAControlStateHighlighted, ccc3(255, 255, 200));
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
    
    if (buttonItem)
    {
        m_pRightButton->setVisible(true);
    }
    else
    {
        m_pRightButton->setVisible(false);
    }
}

bool CANavigationBar::goBack(CAControl* btn, CCPoint point)
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
    this->showLeftButton();
    this->showRightButton();
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
    this->setOpacity(0);
    
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

void CATabBar::showBackGround()
{
    if (m_pBackGround)
    {
        m_pBackGround->removeFromSuperview();
        m_pBackGround=NULL;
    }
    
    if (m_pBackGroundImage == NULL)
    {
        m_pBackGroundImage = CAImage::create("tabBarController_bg.png");
    }
    
    m_pBackGround = CAScale9ImageView::createWithImage(m_pBackGroundImage);
    ((CAScale9ImageView*)m_pBackGround)->setPreferredSize(m_obContentSize);
    m_pBackGround->setFrame(CCRectZero);
    this->insertSubview(m_pBackGround, -1);
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
            CAView* view = CAView::createWithFrame(CCRect(i * width, 0, width, height), ccc4(0, 0, 0, 0));
            this->insertSubview(view, 3);
            view->setDisplayRange(false);
            m_pViews.push_back(view);
            
            CAImageView* imageView = NULL;
            CCLabelTTF* title = NULL;
            
            if (m_pItems.at(i)->getImage())
            {
                imageView = CAImageView::createWithImage(m_pItems.at(i)->getImage());
                imageView->setTag(0xffff);
                view->addSubview(imageView);
            }
            
            
            if (m_pItems.at(i)->getTitle().compare("") != 0)
            {
                int fontSize = this->getContentSize().height / 5.0f;
                title = CCLabelTTF::create(m_pItems.at(i)->getTitle().c_str(), "fonts/arial.ttf", fontSize);
                title->setColor(ccWHITE);
                title->setTag(0xfffe);
                view->addSubview(title);
            }
            
            
            if (imageView && title == NULL)
            {
                CCSize imageViewSize = imageView->getBounds().size;
                float scaleX = width / imageViewSize.width * 0.667f;
                float scaleY = height / imageViewSize.height * 0.667f;
                float scale = MIN(scaleX, scaleY);
                scale = MIN(scale, 1.0f);
                imageViewSize = ccpMult(imageViewSize, scale);
                
                CCRect rect;
                rect.origin = view->getBounds().size/2;
                rect.size = imageViewSize;
                
                imageView->setCenter(rect);
    
            }
            else if (title && imageView == NULL)
            {
                int fontSize = this->getContentSize().height / 2;
                title->setFontSize(fontSize);
                
                CCSize titleSize = title->getBounds().size;
                float titleScale = height / titleSize.height / 2;
                titleSize = ccpMult(titleSize, titleScale);
                
                CCRect rect;
                rect.origin = view->getBounds().size/2;
                rect.size = titleSize;
                
                title->setCenter(rect);
            }
            else if (title && imageView)
            {

                CCSize imageViewSize = imageView->getBounds().size;
                float scaleX = width / imageViewSize.width / 2;
                float scaleY = height / imageViewSize.height / 2;
                float scale = MIN(scaleX, scaleY);
                scale = MIN(scale, 1.0f);
                imageViewSize = ccpMult(imageViewSize, scale);
 
                CCRect rect;
                rect.size = imageViewSize;
                rect.origin = view->getBounds().size;
                rect.origin.x *= 1/2.0f;
                rect.origin.y *= 7/20.0f;
                imageView->setCenter(rect);

                CCSize titleSize = title->getBounds().size;
                float titleScale = height / titleSize.height * 3/10;
                titleSize = ccpMult(titleSize, titleScale);
                
                CCRect rect2;
                rect2.size = titleSize;
                rect2.origin = view->getBounds().size;
                rect2.origin.x *= 1/2.0f;
                rect2.origin.y *= 15/20.0f;
                title->setCenter(rect2);
                
            }
        }
    }
    while (0);
}

void CATabBar::showSelectedBackGround()
{
    if (m_pSelectedBackGround)
    {
        m_pSelectedBackGround->removeFromSuperview();
        m_pSelectedBackGround=NULL;
    }
    
    if (m_pSelectedBackGroundImage == NULL)
    {
        m_pSelectedBackGroundImage = CAImage::create("tabBarController_selected_bg.png");
    }

    m_pSelectedBackGround = CAScale9ImageView::createWithImage(m_pSelectedBackGroundImage);
    ((CAScale9ImageView*)m_pSelectedBackGround)->setPreferredSize(m_cItemSize);
    this->addSubview(m_pSelectedBackGround);
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
        m_pSelectedIndicatorImage = CAImage::create("tabBarController_selected_bottom.png");
    }
    
    m_pSelectedIndicator = CAScale9ImageView::createWithImage(m_pSelectedIndicatorImage);
    ((CAScale9ImageView*)m_pSelectedIndicator)->setPreferredSize(CCSize(m_cItemSize.width, m_cItemSize.height / 10));
    m_pSelectedIndicator->setAnchorPoint(CCPoint(0.0f, 1.0f));
    m_pSelectedIndicator->setFrame(CCRect(0, this->getBounds().size.height, 0, 0));
    this->insertSubview(m_pSelectedIndicator, 1);
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
            if (CAImageView* imageView = dynamic_cast<CAImageView*>(viewLast->getSubviewByTag(0xffff)))
            {
                imageView->setImage(m_pSelectedItem->getImage());
            }
        }
        
        m_nSelectedIndex = index;
        
        CC_BREAK_IF(!m_bRunning);
        
        m_pSelectedItem = m_pItems.at(m_nSelectedIndex);
        m_pSelectedBackGround->setFrame(m_pViews.at(m_nSelectedIndex)->getFrame());
        
        if (m_pSelectedIndicator)
        {
            ((CAScale9ImageView*)m_pSelectedIndicator)->setPreferredSize(CCSize(m_cItemSize.width, m_cItemSize.height / 10));
            m_pSelectedIndicator->stopAllActions();
            CCPoint p = CCPoint(m_pSelectedBackGround->getFrame().origin.x, m_pSelectedBackGround->getFrame().size.height);
            CCMoveTo* moveTo = CCMoveTo::create(0.3f, p);
            CCEaseSineOut* easeBack = CCEaseSineOut::create(moveTo);
            m_pSelectedIndicator->runAction(easeBack);
        }
        
        if (m_pSelectedItem->getSelectedImage())
        {
            CAView* view = m_pViews.at(m_nSelectedIndex);
            if (CAImageView* imageView = dynamic_cast<CAImageView*>(view->getSubviewByTag(0xffff)))
            {
                imageView->setImage(m_pSelectedItem->getSelectedImage());
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
    
    return true;
}

void CATabBar::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CATabBar::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (this->getBounds().containsPoint(point))
    {
        this->setSelectedAtIndex((int)(point.x / m_cItemSize.width));
    }
}

void CATabBar::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}


NS_CC_END
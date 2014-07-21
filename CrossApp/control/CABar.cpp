//
//  CABar.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CABar.h"
#include "view/CAScale9ImageView.h"
#include "basics/CAApplication.h"
#include "view/CALabel.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "dispatcher/CATouch.h"

NS_CC_BEGIN

#pragma CANavigationBar

CANavigationBar::CANavigationBar()
:m_pBackGround(NULL)
,m_pTitle(NULL)
,m_pDelegate(NULL)
,m_pBackGroundImage(NULL)
,m_cTitleColor(CAColor_white)
,m_cButtonColor(CAColor_white)
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
    CCSize winSize = CAApplication::getApplication()->getWinSize();
    CCSize size = CCSize(winSize.width, 96 * CROSSAPP_ADPTATION_RATIO);
    this->setContentSize(size);
    
    return true;
}

void CANavigationBar::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();

    if (m_pBackGround == NULL)
    {
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

void CANavigationBar::setBackGroundImage(CAImage* var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pBackGroundImage);
    m_pBackGroundImage = var;
    CC_RETURN_IF(!m_bRunning);
    this->showBackGround();
}

void CANavigationBar::setTitleColor(CAColor4B color)
{
    m_cTitleColor = color;
    CC_RETURN_IF(!m_bRunning);
    this->showTitle();
}

void CANavigationBar::setButtonColor(CAColor4B color)
{
    m_cButtonColor = color;
    CC_RETURN_IF(!m_bRunning);
    this->showLeftButton();
    this->showRightButton();
}

void CANavigationBar::updateNavigationBar()
{
    this->showBackGround();
    CC_RETURN_IF(m_pItems.size() == 0);
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
    CCRect rect;
    rect.origin = this->getBounds().size/2;
    rect.size.height = this->getBounds().size.height;
    rect.size.width = this->getBounds().size.width - rect.size.height * 4;
    
    if (m_pTitle)
    {
        this->removeSubview(m_pTitle);
        m_pTitle = NULL;
    }
    
    CAImage* image = m_pItems.back()->getTitleViewImage();
    if (image)
    {
        float height = MIN(image->getContentSize().height, rect.size.height * 0.75f);
        float width =  height * image->getContentSize().width / image->getContentSize().height;
        width = MIN(rect.size.width, width);
        rect.size = CCSize(width, height);
        m_pTitle = CAImageView::createWithImage(image);
        m_pTitle->setCenter(rect);
        this->addSubview(m_pTitle);
    }
    else
    {
        CALabel* title = CALabel::createWithCenter(rect);
        title->setTextAlignment(CATextAlignmentCenter);
        title->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        title->setNumberOfLine(1);
        title->setColor(m_cTitleColor);
        title->setFontSize(32 * CROSSAPP_ADPTATION_RATIO);
        this->addSubview(title);
        m_pTitle = title;
    }
    
    if (!m_pItems.empty())
    {
        std::string str = m_pItems.back()->getTitle();
        ((CALabel*)m_pTitle)->setText(str.c_str());
    }

}

void CANavigationBar::showLeftButton()
{
    std::vector<CAButton*>::iterator itr;
    for (itr = m_pLeftButtons.begin(); itr != m_pLeftButtons.end(); itr++)
    {
        (*itr)->removeFromSuperview();
    }
    m_pLeftButtons.clear();
    
    CCArray* buttonItems = m_pItems.back()->getLeftButtonItems();

    CCRect rect = this->getBounds();
    rect.size.width = rect.size.height * 0.9f;
    rect.size.height *= 0.8f;
    rect.origin.x = rect.size.width * 0.7f;
    rect.origin.y = this->getBounds().size.height / 2;
    
    for (int i=0; i<buttonItems->count(); i++)
    {
        rect.origin.x += i * rect.size.width * 1.1f;
        CAButton* button = CAButton::createWithCenter(rect, CAButtonTypeCustom);
        this->addSubview(button);
        
        CABarButtonItem* item = dynamic_cast<CABarButtonItem*>(buttonItems->objectAtIndex(i));
        if (item == NULL && m_pItems.size() > 1)
        {
            button->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_left_white.png"));
            button->setImageColorForState(CAControlStateHighlighted, ccc4(255, 255, 200, 255));
            button->addTarget(this, CAControl_selector(CANavigationBar::goBack), CAControlEventTouchUpInSide);
        }
        else if (item)
        {
            button->setTitleForState(CAControlStateNormal, item->getTitle());
            button->setTitleColorForState(CAControlStateNormal, m_cButtonColor);
            button->setTitleForState(CAControlStateHighlighted, item->getTitle());
            button->setTitleColorForState(CAControlStateHighlighted, ccc4(m_cButtonColor.r/2, m_cButtonColor.g/2, m_cButtonColor.b/2, 255));
            button->setImageForState(CAControlStateNormal, item->getImage());
            if (item->getHighlightedImage())
            {
                button->setImageForState(CAControlStateHighlighted, item->getHighlightedImage());
            }
            else
            {
                button->setImageColorForState(CAControlStateHighlighted, ccc4(255, 255, 200, 255));
            }
            button->addTarget(item->getTarget(), item->getSel(), CAControlEventTouchUpInSide);
        }
        m_pLeftButtons.push_back(button);
    }
}

void CANavigationBar::showRightButton()
{
    std::vector<CAButton*>::iterator itr;
    for (itr = m_pRightButtons.begin(); itr != m_pRightButtons.end(); itr++)
    {
        (*itr)->removeFromSuperview();
    }
    m_pRightButtons.clear();
    
    CCArray* buttonItems = m_pItems.back()->getRightButtonItems();
    
    CCRect rect = this->getBounds();
    rect.size.width = rect.size.height * 0.9f;
    rect.size.height *= 0.8f;
    rect.origin.x = this->getBounds().size.width - rect.size.width * 0.7f;
    rect.origin.y = this->getBounds().size.height / 2;
    
    for (int i=0; i<buttonItems->count(); i++)
    {
        rect.origin.x -= i * rect.size.width * 1.1f;
        CAButton* button = CAButton::createWithCenter(rect, CAButtonTypeCustom);
        this->addSubview(button);
        
        CABarButtonItem* item = dynamic_cast<CABarButtonItem*>(buttonItems->objectAtIndex(i));
        if (item)
        {
            button->setTitleForState(CAControlStateNormal, item->getTitle());
            button->setTitleColorForState(CAControlStateNormal, m_cButtonColor);
            button->setTitleForState(CAControlStateHighlighted, item->getTitle());
            button->setTitleColorForState(CAControlStateHighlighted, ccc4(m_cButtonColor.r/2, m_cButtonColor.g/2, m_cButtonColor.b/2, 255));
            button->setImageForState(CAControlStateNormal, item->getImage());
            if (item->getHighlightedImage())
            {
                button->setImageForState(CAControlStateHighlighted, item->getHighlightedImage());
            }
            else
            {
                button->setImageColorForState(CAControlStateHighlighted, ccc4(255, 255, 200, 255));
            }
            button->addTarget(item->getTarget(), item->getSel(), CAControlEventTouchUpInSide);
        }
        m_pRightButtons.push_back(button);
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
        this->updateNavigationBar();
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
    
    CCSize winSize = CAApplication::getApplication()->getWinSize();
    CCSize size = CCSize(winSize.width, 96 * CROSSAPP_ADPTATION_RATIO);
    this->setContentSize(size);
    
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
        m_pItems = items;
        
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
        m_pBackGroundImage->retain();
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
            m_pSelectedBackGroundImage->retain();
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
        m_pSelectedIndicatorImage->retain();
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

void CATabBar::setTouchSelected(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    this->setSelectedAtIndex(m_pSegmentedControl->getselectedIndex());
}


NS_CC_END
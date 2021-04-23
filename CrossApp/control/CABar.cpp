//
//  CABar.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CABar.h"
#include "view/CAScale9ImageView.h"
#include "view/CALabel.h"
#include "view/CAScrollView.h"
#include "basics/CAApplication.h"
#include "basics/CAPointExtension.h"
#include "dispatcher/CATouch.h"
#include "animation/CAViewAnimation.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"
NS_CC_BEGIN


int CABar::get_top_clearance(CAView* view)
{
    int clearance = 0;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto winSize = CAApplication::getApplication()->getWinSize();
    /****** iphone X ******/
    if (winSize.height / winSize.width > 1.8)
    {
        clearance = 88;
    }
    else
    {
        clearance = 40;
    }
#endif
    
    return clearance;
}

int CABar::get_bottom_clearance(CAView* view)
{
    int clearance = 0;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto winSize = CAApplication::getApplication()->getWinSize();
    /****** iphone X ******/
    if (winSize.height / winSize.width > 1.8)
    {
        clearance = 80;
    }
    else if (winSize.width / winSize.height > 1.8)
    {
        clearance = 80;
    }
#endif
    
    return clearance;
}

#pragma CANavigationBar

CANavigationBar::CANavigationBar(int clearance)
:m_pContentView(nullptr)
,m_pTitle(nullptr)
,m_pBackgroundView(nullptr)
,m_cTitleColor(CAColor4B::WHITE)
,m_cButtonColor(CAColor4B::WHITE)
,m_pItem(nullptr)
,m_pGoBackBarButtonItem(nullptr)
,m_bEnabledShadow(true)
,m_iClearance(clearance)
{
    this->enabledBottomShadow(m_bEnabledShadow);
}

CANavigationBar::~CANavigationBar()
{
    CC_SAFE_RELEASE_NULL(m_pItem);
    CC_SAFE_RELEASE_NULL(m_pGoBackBarButtonItem);
    CC_SAFE_RELEASE(m_pBackgroundView);
}

CANavigationBar* CANavigationBar::createWithFrame(const DRect& rect, int clearance)
{
    CANavigationBar* navigationBar = new CANavigationBar(clearance);
    if (navigationBar && navigationBar->initWithFrame(rect))
    {
        navigationBar->autorelease();
        return navigationBar;
    }
    CC_SAFE_DELETE(navigationBar);
    return NULL;
}

CANavigationBar* CANavigationBar::createWithCenter(const DRect& rect, int clearance)
{
    CANavigationBar* navigationBar = new CANavigationBar(clearance);
    if (navigationBar && navigationBar->initWithFrame(rect))
    {
        navigationBar->autorelease();
        return navigationBar;
    }
    CC_SAFE_DELETE(navigationBar);
    return NULL;
}

CANavigationBar* CANavigationBar::createWithLayout(const CrossApp::DLayout &layout, int clearance)
{
    CANavigationBar* navigationBar = new CANavigationBar(clearance);
    if (navigationBar && navigationBar->initWithLayout(layout))
    {
        navigationBar->autorelease();
        return navigationBar;
    }
    CC_SAFE_DELETE(navigationBar);
    return NULL;
}

bool CANavigationBar::init()
{
    
    m_pContentView = new CAView();
    m_pContentView->setLayout(DLayout(DHorizontalLayout_L_R(0, 0), DVerticalLayout_T_B(m_iClearance, 0)));
    this->addSubview(m_pContentView);
    m_pContentView->release();
        
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CANavigationBar");
    
    m_pGoBackBarButtonItem = CABarButtonItem::create("", CAImage::create(map.at("leftButtonImage")), nullptr);
    m_pGoBackBarButtonItem->setItemWidth(80);
    m_pGoBackBarButtonItem->retain();
    
    return true;
}

void CANavigationBar::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    
    if (m_pBackgroundView == NULL || m_pBackgroundView->getSuperview() == NULL)
    {
        this->showBackground();
    }
    this->updateNavigationBar();
}

void CANavigationBar::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

void CANavigationBar::setContentSize(const DSize & var)
{
    CAView::setContentSize(var);
}

void CANavigationBar::updateClearance(int clearance)
{
    m_iClearance = clearance;
    m_pContentView->setLayout(DLayout(DHorizontalLayout_L_R(0, 0), DVerticalLayout_T_B(m_iClearance, 0)));
}

void CANavigationBar::setItem(CANavigationBarItem* item)
{
    if (item == NULL)
    {
        item = CANavigationBarItem::create("The Title");
    }
    CC_SAFE_RETAIN(item);
    CC_SAFE_RELEASE_NULL(m_pItem);
    m_pItem = item;
    this->updateNavigationBar();
}

void CANavigationBar::setGoBackBarButtonItem(CABarButtonItem* item)
{
    CC_SAFE_RETAIN(item);
    CC_SAFE_RELEASE_NULL(m_pGoBackBarButtonItem);
    m_pGoBackBarButtonItem = item;
    this->updateNavigationBar();
}

void CANavigationBar::setEnabledShadow(bool var)
{
    m_bEnabledShadow = var;
    this->enabledBottomShadow(var);
}

bool CANavigationBar::isEnabledShadow()
{
    return m_bEnabledShadow;
}

void CANavigationBar::setBackgroundView(CAView* var)
{
    var->setTouchEnabled(false);
    this->removeSubview(m_pBackgroundView);
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pBackgroundView);
    m_pBackgroundView = var;
    CC_RETURN_IF(!m_bRunning);
    this->showBackground();
}

void CANavigationBar::setTitleColor(const CAColor4B& color)
{
    m_cTitleColor = color;
    CC_RETURN_IF(!m_bRunning);
    this->showTitle();
}

void CANavigationBar::setButtonColor(const CAColor4B& color)
{
    m_cButtonColor = color;
    CC_RETURN_IF(!m_bRunning);
    this->showLeftButton();
    this->showRightButton();
}

void CANavigationBar::updateNavigationBar()
{
    CC_RETURN_IF(m_pItem == NULL);
    this->showTitle();
    this->showLeftButton();
    this->showRightButton();
    this->enabledBottomShadow(m_bEnabledShadow && m_pItem->isEnabledShadow());
}

void CANavigationBar::showBackground()
{
    if (m_pBackgroundView == NULL)
    {
        const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CANavigationBar");
        m_pBackgroundView = CAScale9ImageView::createWithImage(CAImage::create(map.at("backgroundView")));
        CC_SAFE_RETAIN(m_pBackgroundView);
    }
    m_pBackgroundView->setLayout(DLayoutFill);
    this->insertSubview(m_pBackgroundView, -1);
}

void CANavigationBar::showTitle()
{
    if (m_pTitle)
    {
        m_pContentView->removeSubview(m_pTitle);
        m_pTitle = NULL;
    }
    
    if (CAView* titleView = m_pItem->getTitleView())
    {
        if (titleView->getLayout().equals(DLayoutZero))
        {
            DLayout layout;
            
            if (!titleView->getBounds().size.equals(DSizeZero))
            {
                layout.horizontal.center = 0.5f;
                layout.horizontal.width = titleView->getBounds().size.width;
                layout.vertical.center = 0.5f;
                layout.vertical.height = titleView->getBounds().size.height;
            }
            else
            {
                layout.horizontal = DHorizontalLayout_L_R(100, 100);
                layout.vertical = DVerticalLayout_T_B(5, 5);
            }
            
            titleView->setLayout(layout);
        }
        
        m_pContentView->addSubview(titleView);
        m_pTitle = titleView;
    }
    else if (CAImage* image = m_pItem->getTitleViewImage())
    {
        m_pTitle = CAImageView::createWithImage(image);
        m_pTitle->setLayout(DLayout(DHorizontalLayout_L_R(180, 180), DVerticalLayout_T_B(12, 12)));
        ((CAImageView*)m_pTitle)->setScaleType(CAImageView::ScaleType::FitImageInside);
        m_pContentView->addSubview(m_pTitle);
    }
    else
    {
        CALabel* title = CALabel::createWithLayout(DLayout(DHorizontalLayout_L_R(180, 180), DVerticalLayoutFill));
        title->setTextAlignment(CATextAlignment::Center);
        title->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
        title->setNumberOfLine(1);
		title->setColor(m_cTitleColor);
        title->setFontSize(36);
        title->setBold(true);
        m_pContentView->addSubview(title);
        m_pTitle = title;
        
        if (m_pItem)
        {
            std::string str = m_pItem->getTitle();
            ((CALabel*)m_pTitle)->setText(str.c_str());
        }
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
    
    const CAVector<CAObject*>& buttonItems = m_pItem->getLeftButtonItems();

    DLayout layout = DLayout(DHorizontalLayout_L_W(0, 80), DVerticalLayoutFill);

    for (size_t i=0; i<buttonItems.size(); i++)
    {
        CABarButtonItem* item = dynamic_cast<CABarButtonItem*>(buttonItems.at(i));
        
        if (i == 0)
        {
            layout.horizontal.left = 3;
            layout.horizontal.width = item ? item->getItemWidth() : m_pGoBackBarButtonItem->getItemWidth();
        }
        else
        {
            layout.horizontal.width = item->getItemWidth();
        }
        
        CAButton* button = CAButton::createWithLayout(layout, CAButton::Type::Custom);
        m_pContentView->addSubview(button);
        button->setTitleFontSize(32);
        
        if (i == 0 && item == NULL)
        {
            CAImage* image = m_pGoBackBarButtonItem->getImage();

            if (image)
            {
                float ratio = image->getAspectRatio();
                button->setImageSize(DSize(m_pGoBackBarButtonItem->getImageWidth(), m_pGoBackBarButtonItem->getImageWidth() / ratio));
                button->setImageOffset(DSize(m_pGoBackBarButtonItem->getImageOffsetX(), 0));
                button->setImageForState(CAControl::State::Normal, image);
                
                if (m_pGoBackBarButtonItem->getHighlightedImage())
                {
                    button->setImageForState(CAControl::State::Highlighted, m_pGoBackBarButtonItem->getHighlightedImage());
                }
                else
                {
                    button->setImageColorForState(CAControl::State::Highlighted, CAColor4B(127, 127, 127, 255));
                }            }
            
            std::string title = m_pGoBackBarButtonItem->getTitle();
            if (!title.empty())
            {
                button->setTitleForState(CAControl::State::Normal, title);
                button->setTitleLabelSize(DSize(m_pGoBackBarButtonItem->getLabelWidth(), 44));
                button->setTitleOffset(DSize(m_pGoBackBarButtonItem->getLabelOffsetX(), 0));
                button->setTitleColorForState(CAControl::State::Normal, m_cButtonColor);
                button->setTitleColorForState(CAControl::State::Highlighted, CAColor4B(m_cButtonColor.r/2, m_cButtonColor.g/2, m_cButtonColor.b/2, 255));
            }
            
            button->addTarget([button, this](){
                button->setControlStateDisabled();
                this->goBack();
            }, CAButton::Event::TouchUpInSide);
        }
        else if (item)
        {
            if (item->getCustomView())
            {
                button->addSubview(item->getCustomView());
            }
            else
            {
                CAImage* image = item->getImage();
                
                if (image)
                {
                    float ratio = image->getAspectRatio();
                    button->setImageSize(DSize(item->getImageWidth(), item->getImageWidth() / ratio));
                    button->setImageOffset(DSize(item->getImageOffsetX(), 0));
                    button->setImageForState(CAControl::State::Normal, image);
                    
                    if (item->getHighlightedImage())
                    {
                        button->setImageForState(CAControl::State::Highlighted, item->getHighlightedImage());
                    }
                    else
                    {
                        button->setImageColorForState(CAControl::State::Highlighted, CAColor4B(127, 127, 127, 255));
                    }
                }
                
                std::string title = item->getTitle();
                if (!title.empty())
                {
                    button->setTitleForState(CAControl::State::Normal, title);
                    button->setTitleLabelSize(DSize(item->getLabelWidth(), 44));
                    button->setTitleOffset(DSize(item->getLabelOffsetX(), 0));
                    button->setTitleForState(CAControl::State::Normal, item->getTitle());
                    button->setTitleForState(CAControl::State::Highlighted, item->getTitle());
                    button->setTitleColorForState(CAControl::State::Normal, m_cButtonColor);
                    button->setTitleColorForState(CAControl::State::Highlighted, CAColor4B(m_cButtonColor.r/2, m_cButtonColor.g/2, m_cButtonColor.b/2, 255));
                }
                
            }
            button->addTarget(item->getCallbackFunction(), CAButton::Event::TouchUpInSide);
        }
        m_pLeftButtons.push_back(button);
        layout.horizontal.left += layout.horizontal.width;
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
    
    const CAVector<CAObject*>& buttonItems = m_pItem->getRightButtonItems();

    DLayout layout = DLayout(DHorizontalLayout_R_W(0, 80), DVerticalLayoutFill);
    
    for (size_t i=0; i<buttonItems.size(); i++)
    {
        CABarButtonItem* item = dynamic_cast<CABarButtonItem*>(buttonItems.at(i));
        
        layout.horizontal.width = item ? item->getItemWidth() : 80;
        
        if (i == 0)
        {
            layout.horizontal.right = 3;
        }
        
        CAButton* button = CAButton::createWithLayout(layout, CAButton::Type::Custom);
        m_pContentView->addSubview(button);
        button->setTitleFontSize(32);
        
        if (item)
        {
            if (item->getCustomView())
            {
                button->addSubview(item->getCustomView());
            }
            else
            {
                CAImage* image = item->getImage();
                
                if (image)
                {
                    float ratio = image->getAspectRatio();
                    button->setImageSize(DSize(item->getImageWidth(), item->getImageWidth() / ratio));
                    button->setImageOffset(DSize(item->getImageOffsetX(), 0));
                    button->setImageForState(CAControl::State::Normal, image);
                    
                    if (item->getHighlightedImage())
                    {
                        button->setImageForState(CAControl::State::Highlighted, item->getHighlightedImage());
                    }
                    else
                    {
                        button->setImageColorForState(CAControl::State::Highlighted, CAColor4B(127, 127, 127, 255));
                    }
                }
                
                std::string title = item->getTitle();
                if (!title.empty())
                {
                    button->setTitleForState(CAControl::State::Normal, title);
                    button->setTitleLabelSize(DSize(item->getLabelWidth(), 44));
                    button->setTitleOffset(DSize(item->getLabelOffsetX(), 0));
                    button->setTitleForState(CAControl::State::Normal, item->getTitle());
                    button->setTitleForState(CAControl::State::Highlighted, item->getTitle());
                    button->setTitleColorForState(CAControl::State::Normal, m_cButtonColor);
                    button->setTitleColorForState(CAControl::State::Highlighted, CAColor4B(m_cButtonColor.r/2, m_cButtonColor.g/2, m_cButtonColor.b/2, 255));
                }
            }
            
            button->addTarget(item->getCallbackFunction(), CAButton::Event::TouchUpInSide);
        }
        m_pRightButtons.push_back(button);
        
        layout.horizontal.right += layout.horizontal.width;
    }
}

void CANavigationBar::goBack()
{
    if (m_obPopViewController)
    {
        m_obPopViewController();
    }
}

#pragma CABadgeView

CABadgeView::CABadgeView()
:m_pBackground(nullptr)
,m_pTextView(nullptr)
{

}

CABadgeView::~CABadgeView()
{
    
}

bool CABadgeView::init()
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CABadgeView");
    m_pBackground = CAScale9ImageView::createWithLayout(DLayout(DHorizontalLayout_W_C(46, 0.5f), DVerticalLayout_H_C(46, 0.5f)));
    m_pBackground->setCapInsets(DRect(22.5, 22.5, 1, 1));
    m_pBackground->setImage(CAImage::create(map.at("badgeImage")));
    this->addSubview(m_pBackground);
    
    m_pTextView = CALabel::createWithLayout(DLayout(DHorizontalLayout_W_C(180, 0.5f), DVerticalLayout_H_C(46, 0.5f)));
    m_pTextView->setTextAlignment(CATextAlignment::Center);
    m_pTextView->setVerticalTextAlignmet(CAVerticalTextAlignment::Center);
    m_pTextView->setFontSize(30);
    m_pTextView->setColor(CAColor4B::WHITE);
    m_pTextView->setBold(true);
    this->addSubview(m_pTextView);
    
    this->setScale(1 /1.23f);
    
    return true;
}

void CABadgeView::setBadgeText(const std::string& text)
{
    this->setVisible(!text.empty());
    
    m_pTextView->setLayout(DLayout(DHorizontalLayout_W_C(180, 0.5f), DVerticalLayout_H_C(46, 0.5f)));
    m_pTextView->setText(text);
    
    int width = CAImage::getStringWidth("", 30, text);
    if (width > 30)
    {
        width += 16;
    }
    width = MIN(width, 196);
    width = MAX(width, 46);

    m_pBackground->setLayout(DLayout(DHorizontalLayout_W_C(width, 0.5f), DVerticalLayout_H_C(46, 0.5f)));
}

#pragma CATabBar

CATabBar::CATabBar(int clearance, VerticalAlignment alignment)
:m_pContentView(nullptr)
,m_pBackgroundView(nullptr)
,m_pSelectedIndicatorView(nullptr)
,m_pBackgroundImage(nullptr)
,m_pSelectedBackgroundImage(nullptr)
,m_pSelectedIndicatorImage(nullptr)
,m_pSelectedItem(nullptr)
,m_cItemSize(DSizeZero)
,m_nSelectedIndex(-1)
,m_bSelectedTitleBold(false)
,m_bShowIndicator(false)
,m_iClearance(clearance)
,m_eAlignment(alignment)
{
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CATabBar");
    m_pBackgroundImage = CAImage::create(map.at("backgroundView_normal"));
    CC_SAFE_RETAIN(m_pBackgroundImage);
    
    m_pSelectedBackgroundImage = CAImage::create(map.at("backgroundView_selected"));
    CC_SAFE_RETAIN(m_pSelectedBackgroundImage);
    
    m_pSelectedIndicatorImage = CAImage::create(map.at("bottomLine"));
    CC_SAFE_RETAIN(m_pSelectedIndicatorImage);
    
    m_sTitleColor = ccc4Int(CrossApp::hex2Int(map.at("titleColor_normal")));
    m_sSelectedTitleColor = ccc4Int(CrossApp::hex2Int(map.at("titleColor_selected")));
}

CATabBar::~CATabBar()
{
    m_pItems.clear();
    m_pButtons.clear();
    m_pBadgeViews.clear();
    CC_SAFE_RELEASE_NULL(m_pBackgroundImage);
    CC_SAFE_RELEASE_NULL(m_pSelectedBackgroundImage);
    CC_SAFE_RELEASE_NULL(m_pSelectedIndicatorImage);
    CC_SAFE_RELEASE_NULL(m_pSelectedBackgroundImage);
    CC_SAFE_RELEASE_NULL(m_pSelectedIndicatorImage);
}

CATabBar* CATabBar::createWithFrame(const DRect& rect, int clearance, VerticalAlignment alignment)
{
    CATabBar* tabBar = new CATabBar(clearance, alignment);
    if (tabBar && tabBar->initWithFrame(rect))
    {
        tabBar->autorelease();
        return tabBar;
    }
    CC_SAFE_DELETE(tabBar);
    return NULL;
}
CATabBar* CATabBar::createWithCenter(const DRect& rect, int clearance, VerticalAlignment alignment)
{
    CATabBar* tabBar = new CATabBar(clearance, alignment);
    if (tabBar && tabBar->initWithCenter(rect))
    {
        tabBar->autorelease();
        return tabBar;
    }
    CC_SAFE_DELETE(tabBar);
    return NULL;
}

CATabBar* CATabBar::createWithLayout(const CrossApp::DLayout &layout, int clearance, VerticalAlignment alignment)
{
    CATabBar* tabBar = new CATabBar(clearance, alignment);
    if (tabBar && tabBar->initWithLayout(layout))
    {
        tabBar->autorelease();
        return tabBar;
    }
    CC_SAFE_DELETE(tabBar);
    return NULL;
}

bool CATabBar::init()
{
    m_pContentView = new CAView();
    this->addSubview(m_pContentView);
    m_pContentView->release();
    
    this->enabledTopShadow(true);
    return true;
}

void CATabBar::setItems(const CAVector<CATabBarItem*>& items)
{
    CC_RETURN_IF(items.empty());
	m_pItems = items;
    
    unsigned int count = (unsigned int)m_pItems.size();
    m_cItemSize = m_pContentView->getBounds().size;
    m_cItemSize.width = MIN(m_cItemSize.width, 1024) / count;
    
    if (m_pButtons.empty())
    {
        for (unsigned int i=0; i<count; i++)
        {
            DRect rect = DRectZero;
            rect.size = m_cItemSize;
            rect.origin.x = m_cItemSize.width * i;
            
            CAButton* btn = CAButton::createWithFrame(rect, CAButton::Type::Custom);
            m_pContentView->addSubview(btn);
            btn->setTouchEventScrollHandOverToSuperview(false);
            btn->setTag(i);
            m_pButtons.pushBack(btn);
            
            btn->addTarget([=]()
            {
                int index = i;
                if (!m_sForbidSelectedIndexs.count(index))
                {
                    this->setSelectedAtIndex(index);
                    
                    if (m_obSelectedItem)
                    {
                        m_obSelectedItem(m_pSelectedItem, m_nSelectedIndex);
                    }
                }
                else
                {
                    if (m_obClickToForbidSelectedItem)
                    {
                        m_obClickToForbidSelectedItem(m_pSelectedItem, m_nSelectedIndex);
                    }
                }
                
            }, CAButton::Event::TouchUpInSide);
            
            btn->setTitleForState(CAControl::State::Normal, m_pItems.at(i)->getTitle());
            btn->setTitleColorForState(CAControl::State::Normal, m_sTitleColor);
            btn->setTitleColorForState(CAControl::State::Highlighted, m_sSelectedTitleColor);
            btn->setTitleColorForState(CAControl::State::Disabled, m_sSelectedTitleColor);
            btn->setImageForState(CAControl::State::Normal, m_pItems.at(i)->getImage());
            CAImage* selectedImage = m_pItems.at(i)->getSelectedImage()
            ? m_pItems.at(i)->getSelectedImage()
            : m_pItems.at(i)->getImage();
            btn->setImageForState(CAControl::State::Highlighted, selectedImage);
            btn->setImageForState(CAControl::State::Disabled, selectedImage);
            btn->setBackgroundImageForState(CAControl::State::Normal, CAImage::CLEAR_IMAGE(), false);
            btn->setBackgroundImageForState(CAControl::State::Highlighted, m_pSelectedBackgroundImage, true);
            btn->setBackgroundImageForState(CAControl::State::Disabled, m_pSelectedBackgroundImage, true);
            
            DRect badgeRect;
            badgeRect.origin = rect.origin + DPoint(rect.size.width - 55, 25);
            
            CABadgeView* badgeView = new CABadgeView();
            badgeView->init();
            badgeView->setCenter(badgeRect);
            m_pContentView->insertSubview(badgeView, 10);
            badgeView->setBadgeText(m_pItems.at(i)->getBadgeValue());
            m_pBadgeViews.pushBack(badgeView);
            badgeView->release();
        }
    }
    

    this->setBackgroundImage(m_pBackgroundImage);
    this->setSelectedBackgroundImage(m_pSelectedBackgroundImage);
    this->setSelectedIndicatorImage(m_pSelectedIndicatorImage);
    
}

void CATabBar::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
}

void CATabBar::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
}

void CATabBar::setContentSize(const DSize & var)
{
    CAView::setContentSize(var);
    
    DRect rect = this->getBounds();
    rect.size.width = MIN(rect.size.width, 1024);
    rect.origin.x = (this->getBounds().size.width - rect.size.width) / 2;
    rect.origin.y = m_eAlignment == VerticalAlignment::Top ? m_iClearance : 0;
    rect.size.height = MIN(rect.size.height - rect.origin.y, m_eAlignment == VerticalAlignment::Top ? 88 : 98);
    
    m_pContentView->setFrame(rect);
    
    unsigned int count = (unsigned int)m_pItems.size();
    m_cItemSize = rect.size;
    m_cItemSize.width /= count;
    for (unsigned int i=0; i<count; i++)
    {
        DRect rect = DRectZero;
        rect.size = m_cItemSize;
        rect.origin.x = m_cItemSize.width * i;
        rect.origin.y = 0;
        m_pButtons.at(i)->setFrame(rect);
        
        DRect badgeRect;
        badgeRect.origin = rect.origin + DPoint(rect.size.width - 55, 25);
        m_pBadgeViews.at(i)->setCenter(badgeRect);
    }
    
    if (m_pSelectedIndicatorView)
    {
        DLayout layout;
        layout.vertical.height = 8;
        layout.vertical.bottom = 0;
        layout.horizontal.width = m_cItemSize.width;
        layout.horizontal.left = m_nSelectedIndex * m_cItemSize.width;
        m_pSelectedIndicatorView->setLayout(layout);
    }
}

void CATabBar::updateClearance(int clearance)
{
    m_iClearance = clearance;
    this->setContentSize(m_obContentSize);
}

void CATabBar::replaceItemAtIndex(size_t index, CATabBarItem* item)
{
    if (index < m_pItems.size())
    {
		m_pItems.replace(index, item);
        if (!m_pButtons.empty())
        {
            CAButton* btn = m_pButtons.at(index);
            btn->setTitleForState(CAControl::State::Normal, item->getTitle());
            btn->setImageForState(CAControl::State::Normal, item->getImage());
            CAImage* selectedImage = item->getSelectedImage()
            ? item->getSelectedImage()
            : item->getImage();
            btn->setImageForState(CAControl::State::Highlighted, selectedImage);
            btn->setImageForState(CAControl::State::Disabled, selectedImage);

            CABadgeView* badgeView = m_pBadgeViews.at(index);
            badgeView->setBadgeText(item->getBadgeValue());
        }
    }
}

DRect CATabBar::getContentViewFrame()
{
    return m_pContentView->getFrame();
}

void CATabBar::setBackgroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pBackgroundImage);
    m_pBackgroundImage = var;
    this->showBackground();
}

CAImage* CATabBar::getBackgroundImage()
{
    return m_pBackgroundImage;
}


void CATabBar::setSelectedBackgroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pSelectedBackgroundImage);
    m_pSelectedBackgroundImage = var;
    
    this->showSelectedBackground();
}

CAImage* CATabBar::getSelectedBackgroundImage()
{
    return m_pSelectedBackgroundImage;
}

void CATabBar::setSelectedIndicatorImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pSelectedIndicatorImage);
    m_pSelectedIndicatorImage = var;
    CC_RETURN_IF(var == NULL);
    
    this->showSelectedIndicatorView();
}

CAImage* CATabBar::getSelectedIndicatorImage()
{
    return m_pSelectedIndicatorImage;
}

void CATabBar::setTitleColorForNormal(const CAColor4B &var)
{
    m_sTitleColor = var;
    if (!m_pButtons.empty())
    {
        for (size_t i=0; i<m_pButtons.size(); i++)
        {
            CAButton* btn = m_pButtons.at(i);
            btn->setTitleColorForState(CAControl::State::Normal, m_sTitleColor);
        }
    }
}

const CAColor4B& CATabBar::getTitleColorForNormal()
{
    return m_sTitleColor;
}

void CATabBar::setTitleColorForSelected(const CAColor4B &var)
{
    m_sSelectedTitleColor = var;
    if (!m_pButtons.empty())
    {
        for (size_t i=0; i<m_pButtons.size(); i++)
        {
            CAButton* btn = m_pButtons.at(i);
            btn->setTitleColorForState(CAControl::State::Highlighted, m_sSelectedTitleColor);
            btn->setTitleColorForState(CAControl::State::Disabled, m_sSelectedTitleColor);
        }
    }
}

const CAColor4B& CATabBar::getTitleColorForSelected()
{
    return m_sSelectedTitleColor;
}

void CATabBar::setTitleBoldForSelected(bool var)
{
    m_bSelectedTitleBold = var;
    if (!m_pButtons.empty())
    {
        for (size_t i=0; i<m_pButtons.size(); i++)
        {
            CAButton* btn = m_pButtons.at(i);
            if (i == m_nSelectedIndex)
            {
                btn->setTitleBold(var);
            }
            else
            {
                btn->setTitleBold(false);
            }
        }
    }
}

bool CATabBar::getTitleBoldForSelected()
{
    return m_bSelectedTitleBold;
}

void CATabBar::showBackground()
{
    this->removeSubview(m_pBackgroundView);
    
    m_pBackgroundView = CAScale9ImageView::createWithImage(m_pBackgroundImage);
    m_pBackgroundView->setLayout(DLayout(DHorizontalLayout_L_R(0, 0), DVerticalLayout_T_B(0, 0)));
    this->insertSubview(m_pBackgroundView, -1);
}

void CATabBar::showSelectedBackground()
{
    for (size_t i=0; i<m_pButtons.size(); i++)
    {
        CAButton* btn = m_pButtons.at(i);
        btn->setTitleForState(CAControl::State::Normal, m_pItems.at(i)->getTitle());
        btn->setTitleColorForState(CAControl::State::Normal, m_sTitleColor);
        btn->setTitleColorForState(CAControl::State::Highlighted, m_sSelectedTitleColor);
        btn->setTitleColorForState(CAControl::State::Disabled, m_sSelectedTitleColor);
        btn->setImageForState(CAControl::State::Normal, m_pItems.at(i)->getImage());
        CAImage* selectedImage = m_pItems.at(i)->getSelectedImage()
        ? m_pItems.at(i)->getSelectedImage()
        : m_pItems.at(i)->getImage();
        btn->setImageForState(CAControl::State::Highlighted, selectedImage);
        btn->setImageForState(CAControl::State::Disabled, selectedImage);
        btn->setBackgroundImageForState(CAControl::State::Normal, CAImage::CLEAR_IMAGE(), false);
        btn->setBackgroundImageForState(CAControl::State::Highlighted, m_pSelectedBackgroundImage, true);
        btn->setBackgroundImageForState(CAControl::State::Disabled, m_pSelectedBackgroundImage, true);
        
        CABadgeView* badgeView = m_pBadgeViews.at(i);
        badgeView->setBadgeText(m_pItems.at(i)->getBadgeValue());
    }
}

void CATabBar::showSelectedIndicatorView()
{
    m_pContentView->removeSubview(m_pSelectedIndicatorView);
    CAScale9ImageView* imageView = CAScale9ImageView::createWithImage(m_pSelectedIndicatorImage);
    DRect insetRect;
    insetRect.origin = m_pSelectedIndicatorImage->getContentSize() / 2;
    insetRect.origin = ccpSub(insetRect.origin, DPoint(1, 1));
    insetRect.size = DPoint(2, 2);
    imageView->setCapInsets(insetRect);
    m_pSelectedIndicatorView = imageView;
    
    DLayout layout;
    layout.vertical.height = 8;
    layout.vertical.bottom = 0;
    layout.horizontal.width = m_cItemSize.width;
    layout.horizontal.left = m_nSelectedIndex * m_cItemSize.width;
    m_pSelectedIndicatorView->setLayout(layout);
    m_pContentView->insertSubview(m_pSelectedIndicatorView, 1);
    m_pSelectedIndicatorView->setVisible(m_bShowIndicator);
}

void CATabBar::showSelectedIndicator()
{
    m_bShowIndicator = true;
    if (m_pSelectedIndicatorView)
    {
        m_pSelectedIndicatorView->setVisible(true);
    }
}

void CATabBar::setSelectedAtIndex(int index)
{
    CC_RETURN_IF(index < -1);
    CC_RETURN_IF((size_t)index >= m_pItems.size());
    
    if (m_nSelectedIndex != -1)
    {
        m_pButtons.at(m_nSelectedIndex)->setControlStateNormal();
    }
    
    if (index != -1)
    {
        m_pButtons.at(index)->setControlStateDisabled();
        for (size_t i=0; i<m_pButtons.size(); i++)
        {
            CAButton* btn = m_pButtons.at(i);
            if (i == index)
            {
                btn->setTitleBold(m_bSelectedTitleBold);
            }
            else
            {
                btn->setTitleBold(false);
            }
        }
        m_nSelectedIndex = index;
        m_pSelectedItem = m_pItems.at(m_nSelectedIndex);
        
        if (m_pSelectedIndicatorView)
        {
            m_pSelectedIndicatorView->setVisible(m_bShowIndicator);
            
            DLayout layout;
            layout.vertical.height = 8;
            layout.vertical.bottom = 0;
            layout.horizontal.width = m_cItemSize.width;
            layout.horizontal.left = m_nSelectedIndex * m_cItemSize.width;
            
            CAViewAnimation::beginAnimations("");
            CAViewAnimation::setAnimationDuration(0.3f);
            CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
            m_pSelectedIndicatorView->setLayout(layout);
            CAViewAnimation::commitAnimations();
        }
    }
    else
    {
        m_pSelectedIndicatorView->setVisible(false);
    }
}

void CATabBar::addForbidSelectedAtIndex(int index)
{
    m_sForbidSelectedIndexs.insert(index);
}


NS_CC_END

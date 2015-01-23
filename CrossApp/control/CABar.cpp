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
#include "support/CCPointExtension.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionEase.h"
#include "dispatcher/CATouch.h"


NS_CC_BEGIN

#pragma CANavigationBar

CANavigationBar::CANavigationBar()
:m_pTitle(NULL)
,m_pDelegate(NULL)
,m_pBackGroundView(NULL)
,m_cTitleColor(CAColor_white)
,m_cButtonColor(CAColor_white)
,m_pItem(NULL)
{

}

CANavigationBar::~CANavigationBar()
{
    CC_SAFE_RELEASE_NULL(m_pItem);
    CC_SAFE_RELEASE(m_pBackGroundView);
}

bool CANavigationBar::init()
{
    if (!CAView::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    CCSize winSize = CAApplication::getApplication()->getWinSize();
    CCSize size = CCSize(winSize.width, _px(88));
    this->setContentSize(size);
    
    return true;
}

void CANavigationBar::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();

    if (m_pBackGroundView == NULL || m_pBackGroundView->getSuperview() == NULL)
    {
        this->showBackGround();
    }
    this->updateNavigationBar();
}

void CANavigationBar::onExitTransitionDidStart()
{
    CAView::onExitTransitionDidStart();
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

void CANavigationBar::setBackGroundView(CAView* var)
{
    CCAssert(dynamic_cast<CAControl*>(var) == NULL, "Not allowed to inherit from the CAControl");
    CCAssert(dynamic_cast<CAScrollView*>(var) == NULL, "Not allowed to inherit from the CAScrollView");
    CCAssert(dynamic_cast<CALabel*>(var) == NULL, "Not allowed to inherit from the CALabel");
    
    this->removeSubview(m_pBackGroundView);
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pBackGroundView);
    m_pBackGroundView = var;
    CC_RETURN_IF(!m_bRunning);
    this->showBackGround();
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
}

void CANavigationBar::showBackGround()
{
    if (m_pBackGroundView == NULL)
    {
        m_pBackGroundView = CAScale9ImageView::createWithImage(CAImage::create("source_material/navigation_bg.png"));
        CC_SAFE_RETAIN(m_pBackGroundView);
    }
    m_pBackGroundView->setFrame(this->getBounds());
    this->insertSubview(m_pBackGroundView, -1);
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
    
    if (CAView* titleView = m_pItem->getTitleView())
    {
        float aspectRatio = 0;
        if (!titleView->getFrame().size.equals(CCSizeZero))
        {
            aspectRatio = titleView->getFrame().size.width / titleView->getFrame().size.height;
        }
        
        rect.size.height *= 2/3.0f;
        rect.size.width = aspectRatio < FLT_EPSILON ? rect.size.width : aspectRatio * rect.size.height;
        titleView->setCenter(rect);
        this->addSubview(titleView);
        m_pTitle = titleView;
    }
    else if (CAImage* image = m_pItem->getTitleViewImage())
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
        title->setFontSize(_px(32));
        this->addSubview(title);
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
    
    CCArray* buttonItems = m_pItem->getLeftButtonItems();

    CCRect rect;
    rect.size.width = this->getBounds().size.height * 0.9f;
    rect.size.height = this->getBounds().size.height * 0.8f;
    rect.origin.x = rect.size.width * 0.7f;
    rect.origin.y = this->getBounds().size.height * 0.5f;
    
    for (unsigned i=0; i<buttonItems->count(); i++)
    {
        rect.origin.x += i * rect.size.width;
        CAButton* button = CAButton::createWithCenter(rect, CAButtonTypeCustom);
        this->addSubview(button);
        
        CABarButtonItem* item = dynamic_cast<CABarButtonItem*>(buttonItems->objectAtIndex(i));
        if (item == NULL && m_pItem)
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
                //button->setImageForState(CAControlStateHighlighted, item->getImage());
                button->setImageColorForState(CAControlStateHighlighted, ccc4(127, 127, 127, 255));
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
    
    CCArray* buttonItems = m_pItem->getRightButtonItems();
    
    CCRect rect;
    rect.size.width = this->getBounds().size.height * 0.9f;
    rect.size.height = this->getBounds().size.height * 0.8f;
    rect.origin.x = this->getBounds().size.width - rect.size.width * 0.7f;
    rect.origin.y = this->getBounds().size.height * 0.5f;

    for (unsigned i=0; i<buttonItems->count(); i++)
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
                button->setImageColorForState(CAControlStateHighlighted, ccc4(127, 127, 127, 255));
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

#pragma CATabBar

CATabBar::CATabBar()
:m_pBackGroundView(NULL)
,m_pSegmentedControl(NULL)
,m_pSelectedBackGroundView(NULL)
,m_pSelectedBackGroundImage(NULL)
,m_pSelectedIndicatorView(NULL)
,m_pSelectedIndicatorImage(NULL)
,m_pSelectedItem(NULL)
,m_nMaxShowCount(5)
,m_cItemSize(CCSizeZero)
,m_nSelectedIndex(-1)
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
    
    
    CC_SAFE_RELEASE_NULL(m_pBackGroundView);
    CC_SAFE_RELEASE_NULL(m_pSelectedBackGroundView);
    CC_SAFE_RELEASE_NULL(m_pSelectedIndicatorView);
    CC_SAFE_RELEASE_NULL(m_pSelectedBackGroundImage);
    CC_SAFE_RELEASE_NULL(m_pSelectedIndicatorImage);
}

bool CATabBar::init(const std::vector<CATabBarItem*>& items, const CCSize& size)
{
    if (!CAView::init())
    {
        return false;
    }
    this->setColor(CAColor_clear);
    
    CCSize winSize = CAApplication::getApplication()->getWinSize();
    CCSize contentSize = size.equals(CCSizeZero) ? CCSize(winSize.width, _px(98)) : size;
    this->setContentSize(contentSize);

    this->setItems(items);
    
    this->showBackGround();
    this->showItems();

    return true;
}

CATabBar* CATabBar::create(const std::vector<CATabBarItem*>& items, const CCSize& size)
{
    CATabBar* tabBar = new CATabBar();
    if (tabBar && tabBar->init(items, size))
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
        CC_BREAK_IF(items.empty());

		m_pItems.clear();
		for (std::vector<CATabBarItem*>::size_type i = 0; i < items.size(); i++)
		{
			m_pItems.pushBack(items[i]);
		}
    }
    while (0);
}

void CATabBar::setItems(const CAVector<CATabBarItem*>& items)
{
	do
	{
		CC_BREAK_IF(items.empty());
		m_pItems = items;
	} while (0);
}

void CATabBar::replaceItemAtIndex(size_t index, CATabBarItem* item)
{
    if (index < m_pItems.size())
    {
		m_pItems.replace(index, item);
        this->showItems();
    }
}

const CCRect& CATabBar::getSegmentedControlFrame()
{
    return m_pSegmentedControl->getFrame();
}

void CATabBar::setBackGroundView(CrossApp::CAView *var)
{
    CCAssert(dynamic_cast<CAControl*>(var) == NULL, "Not allowed to inherit from the CAControl");
    CCAssert(dynamic_cast<CAScrollView*>(var) == NULL, "Not allowed to inherit from the CAScrollView");
    CCAssert(dynamic_cast<CALabel*>(var) == NULL, "Not allowed to inherit from the CALabel");
    
    this->removeSubview(m_pBackGroundView);
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pBackGroundView);
    m_pBackGroundView = var;
    this->showBackGround();
}

CAView* CATabBar::getBackGroundView()
{
    return m_pBackGroundView;
}

void CATabBar::setSelectedBackGroundView(CrossApp::CAView *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pSelectedBackGroundView);
    m_pSelectedBackGroundView = var;
    
    if (!m_pSegmentedControl)
    {
        unsigned int count = MIN(m_nMaxShowCount, m_pItems.size());
        CADipRect rect = this->getBounds();
        rect.origin = rect.size / 2;
        rect.size.width = MIN(rect.size.width, 1024);
        m_pSegmentedControl = CASegmentedControl::createWithCenter(rect, count);
        
        m_pSegmentedControl->addTarget(this, CAControl_selector(CATabBar::setTouchSelected));
        m_pSegmentedControl->addTargetAtForbidSelected(this, CAControl_selector(CATabBar::setTouchUpInSide));
        for (unsigned i=0; i<count; i++)
        {
            m_pSegmentedControl->setTitleAtIndex(m_pItems.at(i)->getTitle().c_str(), i, CAControlStateAll);
            m_pSegmentedControl->setTitleColorAtIndex(m_sTitleColor, i, CAControlStateAll);
            m_pSegmentedControl->setTitleColorAtIndex(m_sSelectedTitleColor, i, CAControlStateHighlighted);
            m_pSegmentedControl->setTitleColorAtIndex(m_sSelectedTitleColor, i, CAControlStateSelected);
            m_pSegmentedControl->setImageAtIndex(m_pItems.at(i)->getImage(), i, CAControlStateNormal);
            CAImage* selectedImage = m_pItems.at(i)->getSelectedImage() ? m_pItems.at(i)->getSelectedImage() : m_pItems.at(i)->getImage();
            m_pSegmentedControl->setImageAtIndex(selectedImage, i, CAControlStateHighlighted);
            m_pSegmentedControl->setImageAtIndex(selectedImage, i, CAControlStateSelected);
            m_pSegmentedControl->setBackgroundViewAtIndex(CAView::createWithColor(CAColor_clear), i, CAControlStateNormal);
            m_pSegmentedControl->setSeparateImageVisible(false);
        }
        this->addSubview(m_pSegmentedControl);
        m_cItemSize = m_pSegmentedControl->getItemSize();
    }
    
    if (m_pSegmentedControl)
    {
        for (unsigned i=0; i<m_pItems.size(); i++)
        {
            m_pSegmentedControl->setBackgroundViewAtIndex(m_pSelectedBackGroundView->copy(), i, CAControlStateHighlighted);
            m_pSegmentedControl->setBackgroundViewAtIndex(m_pSelectedBackGroundView->copy(), i, CAControlStateSelected);
        }
    }
}

CAView* CATabBar::getSelectedBackGroundView()
{
    return m_pSelectedBackGroundView;
};

void CATabBar::setSelectedBackGroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RELEASE(m_pSelectedBackGroundImage);
    CC_SAFE_RETAIN(var);
    m_pSelectedBackGroundImage = var;
    CC_RETURN_IF(var == NULL);
    
    this->setSelectedBackGroundView(CAScale9ImageView::createWithImage(m_pSelectedBackGroundImage));
}

CAImage* CATabBar::getSelectedBackGroundImage()
{
    return m_pSelectedBackGroundImage;
}

void CATabBar::setSelectedIndicatorView(CrossApp::CAView *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pSelectedIndicatorView);
    this->removeSubview(m_pSelectedIndicatorView);
    m_pSelectedIndicatorView = var;
    if (var)
    {
        CCRect rect;
        rect.size = CCSize(m_cItemSize.width, m_cItemSize.height / 10);
        rect.origin.x = 0;
        rect.origin.y = this->getBounds().size.height - rect.size.height;
        
        this->insertSubview(m_pSelectedIndicatorView, 1);
        m_pSelectedIndicatorView->setFrame(rect);
    }
}

CAView* CATabBar::getSelectedIndicatorView()
{
    return m_pSelectedIndicatorView;
}

void CATabBar::setSelectedIndicatorImage(CrossApp::CAImage *var)
{
    CC_SAFE_RELEASE(m_pSelectedIndicatorImage);
    CC_SAFE_RETAIN(var);
    m_pSelectedIndicatorImage = var;
    CC_RETURN_IF(var == NULL);
    
    CAScale9ImageView* selectedIndicator= CAScale9ImageView::createWithImage(m_pSelectedIndicatorImage);
    CCRect insetRect;
    insetRect.origin = selectedIndicator->getBounds().size / 2;
    insetRect.origin = ccpSub(insetRect.origin, CCPoint(1, 1));
    insetRect.size = CCPoint(2, 2);
    selectedIndicator->setCapInsets(insetRect);
    
    this->setSelectedIndicatorView(selectedIndicator);
}

CAImage* CATabBar::getSelectedIndicatorImage()
{
    return m_pSelectedIndicatorImage;
}

void CATabBar::setTitleColorForNormal(const CAColor4B &var)
{
    m_sTitleColor = var;
    if (m_pSegmentedControl)
    {
        for (size_t i=0; i<m_pItems.size(); i++)
        {
            m_pSegmentedControl->setTitleColorAtIndex(m_sTitleColor, i, CAControlStateNormal);
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
    if (m_pSegmentedControl)
    {
        for (size_t i=0; i<m_pItems.size(); i++)
        {
            m_pSegmentedControl->setTitleColorAtIndex(m_sSelectedTitleColor, i, CAControlStateHighlighted);
            m_pSegmentedControl->setTitleColorAtIndex(m_sSelectedTitleColor, i, CAControlStateSelected);
        }
    }
}

const CAColor4B& CATabBar::getTitleColorForSelected()
{
    return m_sSelectedTitleColor;
}

void CATabBar::showBackGround()
{
    if (m_pBackGroundView == NULL)
    {
        m_pBackGroundView = CAScale9ImageView::createWithImage(CAImage::create("source_material/tabBar_bg.png"));
        CC_SAFE_RETAIN(m_pBackGroundView);
    }
    m_pBackGroundView->setFrame(this->getBounds());
    this->insertSubview(m_pBackGroundView, -1);
}

void CATabBar::showItems()
{
    if (m_pSelectedIndicatorImage == NULL)
    {
        this->setSelectedBackGroundImage(CAImage::create("source_material/tabBar_selected_bg.png"));
    }
}

void CATabBar::showSelectedIndicator()
{
    if (m_pSelectedIndicatorImage == NULL)
    {
        this->setSelectedIndicatorImage(CAImage::create("source_material/tabBar_selected_indicator.png"));
    }
}

void CATabBar::setSelectedAtIndex(int index)
{
    CC_RETURN_IF(index < 0);
    CC_RETURN_IF((size_t)index >= m_pItems.size());
    
    m_nSelectedIndex = index;
    m_pSelectedItem = m_pItems.at(m_nSelectedIndex);
    
    if (m_pSelectedIndicatorView)
    {
        CCRect rect = m_pSelectedIndicatorView->getFrame();
        rect.size = CCSize(m_cItemSize.width, m_cItemSize.height / 10);
        m_pSelectedIndicatorView->setFrame(rect);
        m_pSelectedIndicatorView->stopAllActions();
        CCPoint p = m_cItemSize;
        p.y -= m_pSelectedIndicatorView->getFrame().size.height;
        p.x *= m_nSelectedIndex;
        p = ccpAdd(m_pSegmentedControl->getFrameOrigin(), p);
        CCFrameOrginTo* moveTo = CCFrameOrginTo::create(0.3f, p);
        CCEaseSineOut* easeBack = CCEaseSineOut::create(moveTo);
        m_pSelectedIndicatorView->runAction(easeBack);
    }
    
    m_pSegmentedControl->setSelectedAtIndex(index);
}

void CATabBar::setForbidSelectedAtIndex(int index)
{
    m_pSegmentedControl->setForbidSelectedAtIndex(index);
}

void CATabBar::setTouchSelected(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    this->setSelectedAtIndex(m_pSegmentedControl->getSelectedIndex());
    
    if (m_pDelegate)
    {
        m_pDelegate->tabBarSelectedItem(this, m_pSelectedItem, m_nSelectedIndex);
    }
}

void CATabBar::setTouchUpInSide(CAControl* control, CCPoint point)
{
    if (m_pDelegate)
    {
        m_pDelegate->tabBarClickToForbidSelectedItem(this, m_pSelectedItem, m_nSelectedIndex);
    }
}


NS_CC_END
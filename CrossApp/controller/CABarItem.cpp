//
//  CABarItem.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CABarItem.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

#pragma CABarItem

CABarItem::CABarItem()
:m_pImage(nullptr)
,m_sTitle("")
{

}

CABarItem::~CABarItem()
{
    CC_SAFE_RELEASE_NULL(m_pImage);
}


#pragma CABarButtonItem

CABarButtonItem::CABarButtonItem()
:m_pHighlightedImage(nullptr)
,m_pCustomView(nullptr)
,m_uItemWidth(80)
,m_uImageWidth(44)
,m_uLabelWidth(80)
,m_iImageOffsetX(0)
,m_iLabelOffsetX(0)
,m_function(nullptr)
{

}

CABarButtonItem::~CABarButtonItem()
{
    CC_SAFE_RELEASE_NULL(m_pHighlightedImage);
}

CABarButtonItem* CABarButtonItem::create(const std::string& title, CAImage* image, CAImage* highlightedImage)
{
    CABarButtonItem* item = new CABarButtonItem();
    if (item && item->init(title, image, highlightedImage))
    {
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}

bool CABarButtonItem::initWithTitle(const std::string& title, CAImage *image, CAImage *highlightedImage)
{
    this->setTitle(title);
    this->setImage(image);
    this->setHighlightedImage(highlightedImage);
    return true;
}

bool CABarButtonItem::initWithImage(CAImage* image, CAImage* highlightedImage)
{
    this->setImage(image);
    this->setHighlightedImage(highlightedImage);
    return true;
}

bool CABarButtonItem::initWithCustomView(CAView* customView)
{
    CC_SAFE_RETAIN(customView);
    m_pCustomView = customView;
    if (m_pCustomView->getLayout().equals(DLayoutZero))
    {
        m_pCustomView->setLayout(DLayoutFill);
    }
    return true;
}

#pragma CANavigationBarItem

CANavigationBarItem::CANavigationBarItem()
:m_pTitleViewImage(nullptr)
,m_bShowGoBackButton(true)
,m_pTitleView(nullptr)
,m_bNagigationBarHidden(false)
,m_bEnabledShadow(true)
{
    m_vLeftButtonItems.pushBack(CAView::create());
}

CANavigationBarItem::~CANavigationBarItem()
{
    CC_SAFE_RELEASE_NULL(m_pTitleView);
    CC_SAFE_RELEASE_NULL(m_pTitleViewImage);
    m_vLeftButtonItems.clear();
}

bool CANavigationBarItem::init(const std::string& title)
{
    this->setTitle(title);
    return true;
}

CANavigationBarItem* CANavigationBarItem::create(const std::string& title)
{
    CANavigationBarItem* item = new CANavigationBarItem();
    if (item && item->init(title))
    {
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}

void CANavigationBarItem::addLeftButtonItem(CrossApp::CABarButtonItem *item)
{
    CC_RETURN_IF(item == NULL);
    m_vLeftButtonItems.pushBack(item);
}

void CANavigationBarItem::addRightButtonItem(CrossApp::CABarButtonItem *item)
{
    CC_RETURN_IF(item == NULL);
    m_vRightButtonItems.pushBack(item);
}

void CANavigationBarItem::setShowGoBackButton(bool var)
{
    CC_RETURN_IF(m_bShowGoBackButton == var);
    m_bShowGoBackButton = var;
    if (m_bShowGoBackButton)
    {
        m_vLeftButtonItems.insert(0, CAView::create());
    }
    else
    {
        m_vLeftButtonItems.erase(0);
    }
}

bool CANavigationBarItem::isShowGoBackButton()
{
    return m_bShowGoBackButton;
}

#pragma CATabBarItem


CATabBarItem::CATabBarItem()
:m_pSelectedImage(nullptr)
{

}

CATabBarItem::~CATabBarItem()
{
    CC_SAFE_RELEASE_NULL(m_pSelectedImage);
}

bool CATabBarItem::init(const std::string& title, CAImage* image, CAImage* selectedImage)
{
    this->setTitle(title);
    this->setImage(image);
    this->setSelectedImage(selectedImage);
    return true;
}

CATabBarItem* CATabBarItem::create(const std::string& title, CAImage* image, CAImage* selectedImage)
{
    CATabBarItem* item = new CATabBarItem();
    if (item && item->init(title, image, selectedImage))
    {
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}

NS_CC_END

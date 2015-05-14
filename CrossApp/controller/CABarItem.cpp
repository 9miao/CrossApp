//
//  CABarItem.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CABarItem.h"

NS_CC_BEGIN

#pragma CABarItem

CABarItem::CABarItem()
:m_bTouchEnabled(true)
,m_pImage(NULL)
,m_sTitle("")
{

}

CABarItem::~CABarItem()
{
    CC_SAFE_RELEASE_NULL(m_pImage);
}


#pragma CABarButtonItem

CABarButtonItem::CABarButtonItem()
:m_pHighlightedImage(NULL)
,m_pTarget(NULL)
,m_selCallFunc(NULL)
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

bool CABarButtonItem::init(const std::string& title, CAImage *image, CAImage *highlightedImage)
{
    this->setTitle(title);
    this->setImage(image);
    this->setHighlightedImage(highlightedImage);
    return true;
}

void CABarButtonItem::setTarget(CAObject* target, SEL_CAControl callfunc)
{
    m_pTarget = target;
    m_selCallFunc = callfunc;
}

CAObject* CABarButtonItem::getTarget()
{
    return m_pTarget;
}

SEL_CAControl CABarButtonItem::getSel()
{
    return m_selCallFunc;
}

#pragma CANavigationBarItem

CANavigationBarItem::CANavigationBarItem()
:m_pTitleViewImage(NULL)
,m_bShowGoBackButton(true)
,m_pTitleView(NULL)
,m_bNagigationBarHidden(false)
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
:m_pSelectedImage(NULL)
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
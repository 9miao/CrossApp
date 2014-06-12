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
,m_nTag(0)
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

CABarButtonItem* CABarButtonItem::create(std::string title, CAImage* image, CAImage* highlightedImage)
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

bool CABarButtonItem::init(std::string title, CAImage *image, CAImage *highlightedImage)
{
    this->setTitle(title);
    this->setImage(image);
    this->setHighlightedImage(highlightedImage);
    return true;
}

void CABarButtonItem::setTarget(CCObject* target, SEL_CAControl callfunc)
{
    m_pTarget = target;
    m_selCallFunc = callfunc;
}

CCObject* CABarButtonItem::getTarget()
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
,m_pLeftButtonItem(NULL)
,m_pRightButtonItem(NULL)
{

}

CANavigationBarItem::~CANavigationBarItem()
{
    CC_SAFE_RELEASE_NULL(m_pTitleViewImage);
    CC_SAFE_RELEASE_NULL(m_pLeftButtonItem);
    CC_SAFE_RELEASE_NULL(m_pRightButtonItem);
}

bool CANavigationBarItem::init(std::string title)
{
    this->setTitle(title);
    return true;
}

CANavigationBarItem* CANavigationBarItem::create(std::string title)
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


#pragma CATabBarItem


CATabBarItem::CATabBarItem()
:m_pSelectedImage(NULL)
{

}

CATabBarItem::~CATabBarItem()
{
    CC_SAFE_RELEASE_NULL(m_pSelectedImage);
}

bool CATabBarItem::init(std::string title, CAImage* image, CAImage* selectedImage)
{
    this->setTitle(title);
    this->setImage(image);
    this->setSelectedImage(selectedImage);
    return true;
}

CATabBarItem* CATabBarItem::create(std::string title, CAImage* image, CAImage* selectedImage)
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
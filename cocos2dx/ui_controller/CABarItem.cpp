//
//  CABarItem.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
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

void CABarItem::setTouchEnabled(bool enabled)
{
    m_bTouchEnabled = enabled;
}

bool CABarItem::isTouchEnabled()
{
    return m_bTouchEnabled;
}



#pragma CANavigationBarItem

CANavigationBarItem::CANavigationBarItem()
:m_pTitleViewImage(NULL)
,m_pBackButtonImage(NULL)
,m_sBackButtonTitle("")
{

}

CANavigationBarItem::~CANavigationBarItem()
{

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

}

bool CATabBarItem::init(std::string title, CAImage* image, int tag)
{
    this->setTitle(title);
    this->setImage(image);
    this->setTag(tag);
    
    return true;
}

CATabBarItem* CATabBarItem::create(std::string title, CAImage* image, int tag)
{
    CATabBarItem* item = new CATabBarItem();
    if (item && item->init(title, image, tag))
    {
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}

NS_CC_END
//
//  CABarItem.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __cocos2dx__CABarItem__
#define __cocos2dx__CABarItem__

#include <iostream>

#include "images/CAImage.h"
#include <string>

NS_CC_BEGIN

class CC_DLL CABarItem
: public CCObject
{
    
public:

    CABarItem();
    
    virtual ~CABarItem();
    
    CC_SYNTHESIZE(std::string, m_sTitle, Title);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pImage, Image);
    
    CC_SYNTHESIZE(int, m_nTag, Tag);
    
    CC_SYNTHESIZE_IS(bool, m_bTouchEnabled, TouchEnabled);
};

class CC_DLL CABarButtonItem
: public CABarItem
{

};

class CC_DLL CANavigationBarItem
: public CABarItem
{

public:
    
    static CANavigationBarItem* create(std::string title);
    
public:
    
    CANavigationBarItem();
    
    virtual ~CANavigationBarItem();
    
    bool init(std::string title);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pTitleViewImage, TitleViewImage);
    
    CC_SYNTHESIZE(std::string, m_sBackButtonTitle, BackButtonTitle);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pBackButtonImage, BackButtonImage);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pHighlightedBackButtonImage, HighlightedBackButtonImage);
};

class CC_DLL CATabBarItem
: public CABarItem
{

public:
    
    static CATabBarItem* create(std::string title, CAImage* image, int tag);
    
public:

    CATabBarItem();
    
    virtual ~CATabBarItem();
    
    bool init(std::string title, CAImage* image, int tag);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pSelectedImage, SelectedImage);
    
};


NS_CC_END


#endif /* defined(__cocos2dx__CABarItem__) */

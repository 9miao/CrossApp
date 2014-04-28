//
//  CABarItem.h
//  cocos2dx
//
//  Created by liyuanfeng on 14-4-14.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#ifndef __cocos2dx__CABarItem__
#define __cocos2dx__CABarItem__

#include <iostream>

#include "textures/CCTexture2D.h"
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
    
    virtual void setTouchEnabled(bool enabled);
    
    bool isTouchEnabled();
    
protected:
    
    bool m_bTouchEnabled;
    
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

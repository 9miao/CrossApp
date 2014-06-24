//
//  CABarItem.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossAppx__CABarItem__
#define __CrossAppx__CABarItem__

#include <iostream>

#include "images/CAImage.h"
#include <string>
#include "control/CAControl.h"

NS_CC_BEGIN

class CC_DLL CABarItem
: public CAObject
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
    
public:
    
    static CABarButtonItem* create(std::string title, CAImage* image, CAImage* highlightedImage);
    
public:
    
    CABarButtonItem();
    
    virtual ~CABarButtonItem();
    
    bool init(std::string title, CAImage* image, CAImage* highlightedImage);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pHighlightedImage, HighlightedImage);
    
    void setTarget(CAObject* target, SEL_CAControl callfunc);
    
    CAObject* getTarget();
    
    SEL_CAControl getSel();
    
protected:
    
    CAObject* m_pTarget;
    
    SEL_CAControl m_selCallFunc;
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
    
    void addLeftButtonItem(CABarButtonItem* item);
    
    void addRightButtonItem(CABarButtonItem* item);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pTitleViewImage, TitleViewImage);
    
    CC_SYNTHESIZE_READONLY(CCArray*, m_pLeftButtonItems, LeftButtonItems);
    
    CC_SYNTHESIZE_READONLY(CCArray*, m_pRightButtonItems,RightButtonItems);
    
    CC_PROPERTY_IS(bool, m_bShowGoBackButton, ShowGoBackButton);
};

class CC_DLL CATabBarItem
: public CABarItem
{
    
public:
    
    static CATabBarItem* create(std::string title, CAImage* image, CAImage* selectedImage = NULL);
    
public:

    CATabBarItem();
    
    virtual ~CATabBarItem();
    
    bool init(std::string title, CAImage* image, CAImage* selectedImage = NULL);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pSelectedImage, SelectedImage);
    
};


NS_CC_END


#endif /* defined(__CrossAppx__CABarItem__) */

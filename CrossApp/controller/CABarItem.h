//
//  CABarItem.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossApp__CABarItem__
#define __CrossApp__CABarItem__

#include <iostream>

#include "images/CAImage.h"
#include <string>
#include "control/CAButton.h"

NS_CC_BEGIN

class CC_DLL CABarItem
: public CAObject
{
    
public:

    CABarItem();
    
    virtual ~CABarItem();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sTitle, Title);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pImage, Image);
};

class CC_DLL CABarButtonItem
: public CABarItem
{
    
public:
    
    static CABarButtonItem* create(const std::string& title, CAImage* image, CAImage* highlightedImage);
    
public:
    
    CABarButtonItem();
    
    virtual ~CABarButtonItem();
    
    bool init(const std::string& title, CAImage* image = NULL, CAImage* highlightedImage = NULL)
    {
        return initWithTitle(title, image, highlightedImage);
    }
    
    bool initWithTitle(const std::string& title, CAImage* image = NULL, CAImage* highlightedImage = NULL);
    
    bool initWithImage(CAImage* image, CAImage* highlightedImage = NULL);
    
    bool initWithCustomView(CAView* customView);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pHighlightedImage, HighlightedImage);
    
    CC_SYNTHESIZE(unsigned int, m_uItemWidth, ItemWidth);
    
    CC_SYNTHESIZE(unsigned int, m_uImageWidth, ImageWidth);
    
    CC_SYNTHESIZE(int, m_iImageOffsetX, ImageOffsetX);
    
    CC_SYNTHESIZE(unsigned int, m_uLabelWidth, LabelWidth);
    
    CC_SYNTHESIZE(int, m_iLabelOffsetX, LabelOffsetX);
    
    CC_SYNTHESIZE_READONLY(CAView*, m_pCustomView, CustomView);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::function<void()>, m_function, CallbackFunction);
    
};

class CC_DLL CANavigationBarItem
: public CABarItem
{

public:
    
    static CANavigationBarItem* create(const std::string& title);
    
public:
    
    CANavigationBarItem();
    
    virtual ~CANavigationBarItem();
    
    bool init(const std::string& title);
    
    void addLeftButtonItem(CABarButtonItem* item);
    
    void addRightButtonItem(CABarButtonItem* item);
    
    CC_SYNTHESIZE_IS(bool, m_bNagigationBarHidden, NagigationBarHidden);
    
    CC_SYNTHESIZE_RETAIN(CAView*, m_pTitleView, TitleView);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pTitleViewImage, TitleViewImage);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAVector<CAObject*>, m_vLeftButtonItems, LeftButtonItems);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAVector<CAObject*>, m_vRightButtonItems, RightButtonItems);

    CC_PROPERTY_IS(bool, m_bShowGoBackButton, ShowGoBackButton);
    
    CC_SYNTHESIZE_IS(bool, m_bEnabledShadow, EnabledShadow);

};

class CC_DLL CATabBarItem
: public CABarItem
{
    
public:
    
    static CATabBarItem* create(const std::string& title, CAImage* image, CAImage* selectedImage = NULL);
    
public:

    CATabBarItem();
    
    virtual ~CATabBarItem();
    
    bool init(const std::string& title, CAImage* image, CAImage* selectedImage = NULL);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pSelectedImage, SelectedImage);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_pBadgeValue, BadgeValue);
    
};


NS_CC_END


#endif /* defined(__CrossApp__CABarItem__) */

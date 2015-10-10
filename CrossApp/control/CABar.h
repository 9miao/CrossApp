//
//  CABar.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CrossApp__CABar__
#define __CrossApp__CABar__

#include <iostream>
#include "view/CAView.h"
#include "CAButton.h"
#include "CASegmentedControl.h"
#include "controller/CABarItem.h"
#include "basics/CASTLContainer.h"

NS_CC_BEGIN

class CANavigationBar;
class CC_DLL CANavigationBarDelegate
{
public:
    
    virtual void navigationPopViewController(CANavigationBar* navigationBar, bool animated) = 0;
    
};

class CC_DLL CANavigationBar
: public CAView
{
    
public:
    
    static CANavigationBar* create(const DSize& size = DSizeZero);

public:
    
    CANavigationBar();
    
    virtual ~CANavigationBar();
    
    virtual bool init(const DSize& size = DSizeZero);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual void setBackGroundView(CAView* var);
    
    CC_SYNTHESIZE_READONLY(CAView*, m_pBackGroundView, BackGroundView);

    virtual void setTitleColor(const CAColor4B& color);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAColor4B, m_cTitleColor, TitleColor);
    
    virtual void setButtonColor(const CAColor4B& color);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAColor4B, m_cButtonColor, ButtonColor);
    
    CC_SYNTHESIZE(CANavigationBarDelegate* , m_pDelegate, Delegate);
    
    virtual void setItem(CANavigationBarItem* item);
    
    CC_SYNTHESIZE_READONLY(CANavigationBarItem*, m_pItem, Item);

protected:
    
    void showBackGround();
    
    void showTitle();
    
    void showLeftButton();
    
    void showRightButton();
    
    void goBack(CAControl* btn, DPoint point);
    
    void updateNavigationBar();
    
protected:
    
    CAView* m_pContentView;
    
    CAView* m_pTitle;
    
    std::vector<CAButton*> m_pLeftButtons;
    
    std::vector<CAButton*> m_pRightButtons;
};

class CATabBar;
class CC_DLL CATabBarDelegate
{
public:
    
    virtual void tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index) = 0;
    
    virtual void tabBarClickToForbidSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index){};
    
};

class CC_DLL CABadgeView: public CAView
{
public:
    
    CABadgeView();
    
    virtual ~CABadgeView();
    
    bool init();
    
    void setBadgeText(const std::string& text);
    
    virtual void setContentSize(const DSize& contentSize);
    
protected:
    
    CAScale9ImageView* m_pBackground;
    
    CALabel* m_pTextView;
    
};

typedef enum
{
    CABarVerticalAlignmentTop = 0,
    CABarVerticalAlignmentBottom
}CABarVerticalAlignment;

class CC_DLL CATabBar
:public CAView
{
    
public:
    
    static CATabBar* create(const CAVector<CATabBarItem*>& items, const DSize& size = DSizeZero, const CABarVerticalAlignment& var = CABarVerticalAlignmentBottom);
    
	void setItems(const CAVector<CATabBarItem*>& items);
    
public:

    CATabBar();
    
    virtual ~CATabBar();

    virtual bool init(const CAVector<CATabBarItem*>& items, const DSize& size = DSizeZero, const CABarVerticalAlignment& var = CABarVerticalAlignmentBottom);
    
    CC_PROPERTY(CAImage*, m_pBackGroundImage, BackGroundImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sBackGroundColor, BackGroundColor);
    
    CC_PROPERTY(CAImage*, m_pSelectedBackGroundImage, SelectedBackGroundImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sSelectedBackGroundColor, SelectedBackGroundColor);

    CC_PROPERTY(CAImage*, m_pSelectedIndicatorImage, SelectedIndicatorImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sSelectedIndicatorColor, SelectedIndicatorColor);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sTitleColor, TitleColorForNormal);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sSelectedTitleColor, TitleColorForSelected);
        
    CC_SYNTHESIZE(CATabBarDelegate* , m_pDelegate, Delegate);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAVector<CATabBarItem*>, m_pItems, Items);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(DSize, m_cItemSize, ItemSize);
    
    CC_SYNTHESIZE_READONLY(int, m_nSelectedIndex, SelectedIndex);
    
    void setSelectedAtIndex(int index);
    
    CC_SYNTHESIZE_READONLY(std::set<int>, m_sForbidSelectedIndexs, ForbidSelectedIndexs);
    
    void addForbidSelectedAtIndex(int index);
    
    void showSelectedIndicator();
    
    void replaceItemAtIndex(size_t index, CATabBarItem* item);
    
    const DRect& getContentViewFrame();

protected:
    
    void showBackGround();
    
    void showSelectedBackGround();
    
    void showSelectedIndicatorView();
    
    void setTouchSelected(CAControl* control, DPoint point);

protected:
    
    bool m_bShowIndicator;
    
    CABarVerticalAlignment m_eVerticalAlignment;
    
    CATabBarItem* m_pSelectedItem;

    CAView* m_pBackGroundView;
    
    CAView* m_pContentView;
    
    CAView* m_pSelectedIndicatorView;
    
    CAVector<CAButton*> m_pButtons;
    
    CAVector<CABadgeView*> m_pBadgeViews;
};


NS_CC_END

#endif /* defined(__CrossApp__CABar__) */

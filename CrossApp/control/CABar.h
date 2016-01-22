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

typedef enum
{
    CABarVerticalAlignmentTop = 0,
    CABarVerticalAlignmentBottom
}CABarVerticalAlignment;

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
    
    CANavigationBar(bool clearance = false);
    
    virtual ~CANavigationBar();
    
    static CANavigationBar* createWithFrame(const DRect& rect, bool clearance = false);
    
    static CANavigationBar* createWithCenter(const DRect& rect, bool clearance = false);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    virtual void setBackgroundView(CAView* var);
    
    CC_SYNTHESIZE_READONLY(CAView*, m_pBackgroundView, BackgroundView);

    virtual void setTitleColor(const CAColor4B& color);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAColor4B, m_cTitleColor, TitleColor);
    
    virtual void setButtonColor(const CAColor4B& color);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAColor4B, m_cButtonColor, ButtonColor);
    
    CC_SYNTHESIZE(CANavigationBarDelegate* , m_pDelegate, Delegate);
    
    virtual void setItem(CANavigationBarItem* item);
    
    CC_SYNTHESIZE_READONLY(CANavigationBarItem*, m_pItem, Item);

protected:
    
    virtual bool init();
    
    void showBackground();
    
    void showTitle();
    
    void showLeftButton();
    
    void showRightButton();
    
    void goBack(CAControl* btn, DPoint point);
    
    void updateNavigationBar();
    
    virtual void setContentSize(const DSize & var);
    
protected:
    
    CAView* m_pContentView;
    
    CAView* m_pTitle;
    
    std::vector<CAButton*> m_pLeftButtons;
    
    std::vector<CAButton*> m_pRightButtons;
    
    bool m_bClearance;
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

class CC_DLL CATabBar
:public CAView
{
    
public:

    CATabBar(bool clearance = false);
    
    virtual ~CATabBar();

    static CATabBar* createWithFrame(const DRect& rect, bool clearance = false);
    
    static CATabBar* createWithCenter(const DRect& rect, bool clearance = false);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    void setItems(const CAVector<CATabBarItem*>& items);
    
    CC_PROPERTY(CAImage*, m_pBackgroundImage, BackgroundImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sBackgroundColor, BackgroundColor);
    
    CC_PROPERTY(CAImage*, m_pSelectedBackgroundImage, SelectedBackgroundImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sSelectedBackgroundColor, SelectedBackgroundColor);

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
    
    virtual bool init();
    
    void showBackground();
    
    void showSelectedBackground();
    
    void showSelectedIndicatorView();
    
    void setTouchSelected(CAControl* control, DPoint point);

    virtual void setContentSize(const DSize & var);
    
protected:
    
    bool m_bShowIndicator;
    
    CATabBarItem* m_pSelectedItem;

    CAView* m_pBackgroundView;
    
    CAView* m_pContentView;
    
    CAView* m_pSelectedIndicatorView;
    
    CAVector<CAButton*> m_pButtons;
    
    CAVector<CABadgeView*> m_pBadgeViews;
    
    bool m_bClearance;
};


NS_CC_END

#endif /* defined(__CrossApp__CABar__) */

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

class CC_DLL CABar
: public CAView
{
public:
    
    static int get_top_clearance(CAView* view);
    
    static int get_bottom_clearance(CAView* view);
};

class CC_DLL CANavigationBar
: public CABar
{
public:
    
    CANavigationBar(int clearance = 0);
    
    virtual ~CANavigationBar();
    
    static CANavigationBar* createWithFrame(const DRect& rect, int clearance = 0);
    
    static CANavigationBar* createWithCenter(const DRect& rect, int clearance = 0);
    
    static CANavigationBar* createWithLayout(const DLayout& layout, int clearance = 0);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    CC_LISTENING_FUNCTION(void(), PopViewController);
    
    virtual void setBackgroundView(CAView* var);
    
    CC_SYNTHESIZE_READONLY(CAView*, m_pBackgroundView, BackgroundView);

    virtual void setTitleColor(const CAColor4B& color);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAColor4B, m_cTitleColor, TitleColor);
    
    virtual void setButtonColor(const CAColor4B& color);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAColor4B, m_cButtonColor, ButtonColor);

    virtual void setItem(CANavigationBarItem* item);
    
    CC_SYNTHESIZE_READONLY(CANavigationBarItem*, m_pItem, Item);

    virtual void setGoBackBarButtonItem(CABarButtonItem* item);
    
    CC_SYNTHESIZE_READONLY(CABarButtonItem*, m_pGoBackBarButtonItem, GoBackBarButtonItem);
    
    CC_PROPERTY_IS(bool, m_bEnabledShadow, EnabledShadow);

    
    void updateClearance(int clearance);
    
protected:
    
    virtual bool init();
    
    void showBackground();
    
    void showTitle();
    
    void showLeftButton();
    
    void showRightButton();
    
    void goBack();
    
    void updateNavigationBar();
    
    virtual void setContentSize(const DSize & var);
    
protected:
    
    CAView* m_pContentView;
    
    CAView* m_pTitle;
    
    std::vector<CAButton*> m_pLeftButtons;
    
    std::vector<CAButton*> m_pRightButtons;
    
    int m_iClearance;
};

class CC_DLL CABadgeView: public CAView
{
public:
    
    CABadgeView();
    
    virtual ~CABadgeView();
    
    bool init();
    
    void setBadgeText(const std::string& text);

protected:
    
    CAScale9ImageView* m_pBackground;
    
    CALabel* m_pTextView;
    
};

class CC_DLL CATabBar
:public CABar
{
    
public:

    enum class VerticalAlignment
    {
        Top = 0,
        Bottom
    };
    
    CATabBar(int clearance = 0, VerticalAlignment alignment = VerticalAlignment::Bottom);
    
    virtual ~CATabBar();

    static CATabBar* createWithFrame(const DRect& rect, int clearance = 0, VerticalAlignment alignment = VerticalAlignment::Bottom);
    
    static CATabBar* createWithCenter(const DRect& rect, int clearance = 0, VerticalAlignment alignment = VerticalAlignment::Bottom);
    
    static CATabBar* createWithLayout(const DLayout& layout, int clearance = 0, VerticalAlignment alignment = VerticalAlignment::Bottom);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    void setItems(const CAVector<CATabBarItem*>& items);
    
    CC_LISTENING_FUNCTION(void(CATabBarItem* item, unsigned int index), SelectedItem)
    
    CC_LISTENING_FUNCTION(void(CATabBarItem* item, unsigned int index), ClickToForbidSelectedItem)
    
    CC_PROPERTY(CAImage*, m_pBackgroundImage, BackgroundImage);
    
    CC_PROPERTY(CAImage*, m_pSelectedBackgroundImage, SelectedBackgroundImage);

    CC_PROPERTY(CAImage*, m_pSelectedIndicatorImage, SelectedIndicatorImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sTitleColor, TitleColorForNormal);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sSelectedTitleColor, TitleColorForSelected);
    
    CC_PROPERTY(bool, m_bSelectedTitleBold, TitleBoldForSelected);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CAVector<CATabBarItem*>, m_pItems, Items);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(DSize, m_cItemSize, ItemSize);
    
    CC_SYNTHESIZE_READONLY(int, m_nSelectedIndex, SelectedIndex);
    
    void setSelectedAtIndex(int index);
    
    CC_SYNTHESIZE_READONLY(std::set<int>, m_sForbidSelectedIndexs, ForbidSelectedIndexs);
    
    void addForbidSelectedAtIndex(int index);
    
    void showSelectedIndicator();
    
    void replaceItemAtIndex(size_t index, CATabBarItem* item);
    
    DRect getContentViewFrame();

    void updateClearance(int clearance);
    
protected:
    
    virtual bool init();
    
    void showBackground();
    
    void showSelectedBackground();
    
    void showSelectedIndicatorView();
    
    virtual void setContentSize(const DSize & var);
    
protected:
    
    bool m_bShowIndicator;
    
    CATabBarItem* m_pSelectedItem;

    CAView* m_pBackgroundView;
    
    CAView* m_pContentView;
    
    CAView* m_pSelectedIndicatorView;
    
    CAVector<CAButton*> m_pButtons;
    
    CAVector<CABadgeView*> m_pBadgeViews;
    
    int m_iClearance;
    
    VerticalAlignment m_eAlignment;
};


NS_CC_END

#endif /* defined(__CrossApp__CABar__) */

//
//  CABar.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-14.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __cocos2dx__CABar__
#define __cocos2dx__CABar__

#include <iostream>
#include "sprite_nodes/CAView.h"
#include "CAButton.h"
#include "CASegmentedControl.h"
#include "CABarItem.h"
#include <vector>
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
    
    CREATE_FUNC(CANavigationBar);
    
    void pushItem(CANavigationBarItem* item);
    
    void popItem();
    
public:
    
    CANavigationBar();
    
    virtual ~CANavigationBar();
    
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    CC_SYNTHESIZE_READONLY(CAButton*, m_pLeftButton, LeftButton);
    
    CC_SYNTHESIZE_READONLY(CAButton*, m_pRightButton, RightButton);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pBackGroundImage, BackGroundImage);
    
    CC_SYNTHESIZE(CANavigationBarDelegate* , m_pDelegate, Delegate);
    
protected:
    
    void showBackGround();
    
    void showTitle();
    
    void showLeftButton();
    
    void showRightButton();
    
    void goBack(CAControl* btn, CCPoint point);
    
    void updateLeftButton();
    
    void updateRightButton();
    
protected:
    
    CAView* m_pBackGround;
    
    CAView* m_pTitle;
    
    std::vector<CANavigationBarItem*> m_pItems;
};

class CATabBar;
class CC_DLL CATabBarDelegate
{
public:
    
    virtual void tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index) = 0;
    
};

class CC_DLL CATabBar
:public CAView
{
    
public:
    
    static CATabBar* create(const std::vector<CATabBarItem*>& items);
    
    void setItems(const std::vector<CATabBarItem*>& items);
    
public:

    CATabBar();
    
    virtual ~CATabBar();
    
    virtual bool init(const std::vector<CATabBarItem*>& items);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pBackGroundImage, BackGroundImage);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pSelectedBackGroundImage, SelectedBackGroundImage);
    
    CC_SYNTHESIZE_RETAIN(CAImage*, m_pSelectedIndicatorImage, SelectedIndicatorImage);
    
    CC_SYNTHESIZE(unsigned int, m_nMaxShowCount, MaxShowCount)
    
    CC_SYNTHESIZE(CATabBarDelegate* , m_pDelegate, Delegate);
    
    CC_SYNTHESIZE(ccColor3B, m_sTitleColor, TitleColorForNormal);
    
    CC_SYNTHESIZE(ccColor3B, m_sSelectedTitleColor, TitleColorForSelected);
    
    void setSelectedAtIndex(int index);
    
    void showSelectedIndicator();
    
protected:
    
    void showBackGround();
    
    void showItems();
    
    void setTouchSelected(CAControl* control, CCPoint point);
    
protected:
    
    CAView* m_pBackGround;
    
    CAView* m_pSelectedIndicator;
    
    CATabBarItem* m_pSelectedItem;
    
    std::vector<CATabBarItem*> m_pItems;

    CCSize m_cItemSize;

    CASegmentedControl* m_pSegmentedControl;
    
    int m_nSelectedIndex;
    
};


NS_CC_END

#endif /* defined(__cocos2dx__CABar__) */

//
//  CAViewController.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-11.
//  Copyright (c) 2014年 http://9miao.com All rights reserved.
//

#ifndef __cocos2dx__CAViewController__
#define __cocos2dx__CAViewController__

#include <iostream>
#include "sprite_nodes/CAView.h"
#include "CABar.h"
#include "CAScrollView.h"
#include "keypad_dispatcher/CCKeypadDelegate.h"
#include "keypad_dispatcher/CCKeypadDispatcher.h"
NS_CC_BEGIN

typedef enum
{
    CABarVerticalAlignmentTop = 0,
    CABarVerticalAlignmentBottom
}CABarVerticalAlignment;

class CAWindow;
class CATabBarController;
class CANavigationController;

class CC_DLL CAViewController
: public CAView
, public CAViewDelegate
, public CCKeypadDelegate
{

public:

    CAViewController();
    
    virtual ~CAViewController();
    
    virtual bool init();
    
    const char* getNibName();
    
    CC_SYNTHESIZE(std::string, m_sTitle, Title);
    
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pNavigationController, NavigationController);

    CC_SYNTHESIZE_READONLY(CANavigationBarItem*, m_pNavigationBarItem, NavigationBarItem);
    
    CC_SYNTHESIZE_READONLY(CATabBarController*, m_pTabBarController, TabBarController);

    CC_SYNTHESIZE_READONLY(CATabBarItem*, m_pTabBarItem, TabBarItem);
    
    void setNavigationBarItem(CANavigationBarItem* item);
    
    void setTabBarItem(CATabBarItem* item);
    
    void presentModalViewController(CAViewController* controller, bool animated);
    
    void dismissModalViewController(bool animated);
    
    void clearUp();
    
    virtual bool isKeypadEnabled();
    
    virtual void setKeypadEnabled(bool value);
    
public:
    
    friend class CATabBarController;
    
    friend class CANavigationController;
    
    friend class CAWindow;
    
protected:
    
    virtual void viewDidLoad() = 0;
    
    virtual void viewDidUnload() = 0;
    
    virtual void reshapeViewRectDidFinish(){};
    
    CAView* getView();
    
    virtual void keyBackClicked() {}

    virtual void keyMenuClicked() {};
    
private:
    
    void addViewFromSuperview(CAView* node);
    
    void removeViewFromSuperview();
    
private:
    
    void getSuperViewRect(const CCRect& rect);
    
    void viewOnEnterTransitionDidFinish();
    
    void viewOnExitTransitionDidStart();
    
private:
    
    CAView* m_pView;
    
    bool m_bLifeLock;
    
    bool m_bKeypadEnabled;
};

class CC_DLL CANavigationController
: public CAViewController
, public CANavigationBarDelegate
{

public:
    
    CANavigationController();
    
    virtual ~CANavigationController();
    
    virtual bool initWithRootViewController(CAViewController* viewController,
                                            CABarVerticalAlignment var = CABarVerticalAlignmentTop);
    
public:
    
    void replaceViewController(CAViewController* viewController, bool animated);
    
    void pushViewController(CAViewController* viewController, bool animated);
    
    CAViewController* popViewControllerAnimated(bool animated);
    
    inline unsigned long getViewControllerCount() {return m_pViewControllers.size();}
    
    void setNavigationBarHidden(bool hidden, bool animated);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bNavigationBarHidden, NavigationBarHidden);
    
    CC_SYNTHESIZE_READONLY(CANavigationBar*, m_pNavigationBar, NavigationBar);
    
    CC_SYNTHESIZE_READONLY(CABarVerticalAlignment, m_eNavigationBarVerticalAlignment, NavigationBarVerticalAlignment);
    
    void updateItem(CAViewController* viewController);
    
protected:
 
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void reshapeViewRectDidFinish();
    
    void replaceViewControllerFinish();
    
    void pushViewControllerFinish();
    
    void popViewControllerFinish();
    
    virtual void navigationPopViewController(CANavigationBar* navigationBar, bool animated);
    
    void update(float dt);
    
    void scheduleUpdate();
    
    void unScheduleUpdate();
    
private:
    
    std::vector<CAViewController*> m_pViewControllers;
    
    CAView* m_pContainer;
    
};

class CC_DLL CATabBarController
: public CAViewController
, public CATabBarDelegate
{
    
public:

    CATabBarController();
    
    virtual ~CATabBarController();
    
    virtual bool initWithViewControllers(const std::vector<CAViewController*>& viewControllers,
                                         CABarVerticalAlignment var = CABarVerticalAlignmentBottom);
    
    bool showSelectedViewController(CAViewController* viewController, bool animated);
    
    CAViewController* getSelectedViewController();
    
    bool showSelectedViewControllerAtIndex(unsigned int index, bool animated);
    
    unsigned int getSelectedViewControllerAtIndex();
    
    void setTabBarHidden(bool hidden, bool animated);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bTabBarAnimated, TabBarAnimated);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bTabBarHidden, TabBarHidden);
    
    CC_SYNTHESIZE_READONLY(CATabBar*, m_pTabBar, TabBar);
    
    CC_SYNTHESIZE_READONLY(CABarVerticalAlignment, m_eTabBarVerticalAlignment, TabBarVerticalAlignment);
    
    void updateItem(CAViewController* viewController);
    
protected:

    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void reshapeViewRectDidFinish();
    
    virtual void tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index);
    
    CC_DEPRECATED_ATTRIBUTE void renderingAllViewController(float dt = 0);
    
    CC_DEPRECATED_ATTRIBUTE void renderingSelectedViewController(float dt = 0);
    
    void update(float dt);
    
    void scheduleUpdate();
    
    void unScheduleUpdate();
    
private:
    
    std::vector<CAViewController*> m_pViewControllers;
    
    unsigned int m_nSelectedIndex;

    CAScrollView* m_pContainer;
};



NS_CC_END

#endif /* defined(__cocos2dx__CAViewController__) */

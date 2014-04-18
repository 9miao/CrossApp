//
//  CAViewController.h
//  cocos2dx
//
//  Created by 栗元峰 on 14-4-11.
//  Copyright (c) 2014年 www.9miao.com All rights reserved.
//

#ifndef __cocos2dx__CAViewController__
#define __cocos2dx__CAViewController__

#include <iostream>
#include "sprite_nodes/CCView.h"
#include "CABar.h"
NS_CC_BEGIN

class CCScene;
class CATabBarController;
class CANavigationController;

class CAViewController
: public CCObject
, public UIViewDelegate
{

public:

    CAViewController();
    
    virtual ~CAViewController();
    
    virtual bool init();
    
    //void setSuperView(UIView* view);
    
    const char* getNibName();
    
    CC_SYNTHESIZE(std::string, m_sTitle, Title);
    
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pNavigationController, NavigationController);

    CC_SYNTHESIZE_RETAIN(CANavigationBarItem*, m_pNavigationBarItem, NavigationBarItem);
    
    CC_SYNTHESIZE_READONLY(CATabBarController*, m_pTabBarController, TabBarController);

    CC_SYNTHESIZE_RETAIN(CATabBarItem*, m_pTabBarItem, TabBarItem);
    
    void clearUp();
    
public:
    
    friend class CATabBarController;
    
    friend class CANavigationController;
    
    friend class CAWindow;
    
protected:
    
    virtual void viewDidLoad() = 0;
    
    virtual void viewDidUnload() = 0;
    
    UIView* getView();
    
private:
    
    void setViewSuperNode(CCNode* node);
    
    void removeViewAtSuperNode();
    
private:
    
    void getSuperViewRect(const CCRect& rect);
    
    void viewOnEnterTransitionDidFinish();
    
    void viewOnExitTransitionDidStart();
    
private:
    
    UIView* m_pView;
    
    bool m_bLifeLock;
};

class CANavigationController
: public CAViewController
{

public:
    
    CANavigationController();
    
    virtual ~CANavigationController();
    
    virtual bool initWithRootViewController(CAViewController* viewController);
    
public:
    
    void pushViewController(CAViewController* viewController, bool animated);
    
    CAViewController* popViewControllerAnimated(bool animated);
    
    CC_SYNTHESIZE_READONLY(CANavigationBar*, m_pNavigationBar, NavigationBar);
    
protected:
 
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();

    void pushViewControllerFinish();
    
    void popViewControllerFinish();
    
private:
    
    std::vector<CAViewController*> m_pViewControllers;
    
    UIView* m_pContainer;
    
};

class CATabBarController
: public CAViewController
, public CATabBarDelegate
{
    
public:

    CATabBarController();
    
    virtual ~CATabBarController();
    
    virtual bool initWithViewControllers(const std::vector<CAViewController*>& viewControllers, bool animated);
    
    bool showSelectedViewController(CAViewController* viewController, bool animated);
    
    CAViewController* getSelectedViewController();
    
    bool showSelectedViewControllerAtIndex(unsigned int index, bool animated);
    
    unsigned int getSelectedViewControllerAtIndex();

    CC_SYNTHESIZE_READONLY(CATabBar*, m_pTabBar, TabBar);
    
protected:

    void viewDidLoad();
    
    void viewDidUnload();
    
    virtual void tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, int index);
    
private:
    
    std::vector<CAViewController*> m_pViewControllers;
  
    CAViewController* m_pSelectedViewController;
    
    unsigned int m_nSelectedIndex;

    UIView* m_pContainer;
};



NS_CC_END

#endif /* defined(__cocos2dx__CAViewController__) */

//
//  CAViewController.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-11.
//  Copyright (c) 2014年 http://9miao.com All rights reserved.
//

#ifndef __CrossAppx__CAViewController__
#define __CrossAppx__CAViewController__

#include <iostream>
#include "view/CAView.h"
#include "control/CABar.h"
#include "view/CAScrollView.h"
#include "dispatcher/CAKeypadDelegate.h"
#include "dispatcher/CAKeypadDispatcher.h"
#include "basics/CASTLContainer.h"
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
: public CAResponder
, public CAViewDelegate
, public CAKeypadDelegate
{
    
public:
    
    CAViewController();
    
    virtual ~CAViewController();
    
    virtual bool init();
    
    const char* getNibName();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sTitle, Title);
    
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pNavigationController, NavigationController);
    
    CC_SYNTHESIZE_READONLY(CANavigationBarItem*, m_pNavigationBarItem, NavigationBarItem);
    
    CC_SYNTHESIZE_READONLY(CATabBarController*, m_pTabBarController, TabBarController);
    
    CC_SYNTHESIZE_READONLY(CATabBarItem*, m_pTabBarItem, TabBarItem);
    
    bool isViewRunning();
    
    void setNavigationBarItem(CANavigationBarItem* item);
    
    void setTabBarItem(CATabBarItem* item);
    
    void presentModalViewController(CAViewController* controller, bool animated);
    
    void dismissModalViewController(bool animated);
    
    virtual bool isKeypadEnabled();
    
    virtual void setKeypadEnabled(bool value);
    
public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    virtual CAResponder* nextResponder();
    
    friend class CATabBarController;
    
    friend class CANavigationController;
    
    friend class CAWindow;
    
protected:
    
    virtual void viewDidLoad() = 0;
    
    virtual void viewDidUnload() = 0;
    
    virtual void viewDidAppear(){};
    
    virtual void viewDidDisappear(){};
    
    virtual void reshapeViewRectDidFinish(){};
    
    virtual CAView* getView();
    
    virtual void keyBackClicked() {}
    
    virtual void keyMenuClicked() {};
    
protected:
    
    virtual void addViewFromSuperview(CAView* node);
    
    virtual void removeViewFromSuperview();
    
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
    
    virtual void replaceViewController(CAViewController* viewController, bool animated);
    
    virtual void pushViewController(CAViewController* viewController, bool animated);
    
    CAViewController* popViewControllerAnimated(bool animated);
    
    inline unsigned long getViewControllerCount() {return m_pViewControllers.size();}
    
    virtual void setNavigationBarHidden(bool hidden, bool animated);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bNavigationBarHidden, NavigationBarHidden);
    
    CC_SYNTHESIZE_READONLY(CANavigationBar*, m_pNavigationBar, NavigationBar);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CABarVerticalAlignment, m_eNavigationBarVerticalAlignment, NavigationBarVerticalAlignment);
    
    void updateItem(CAViewController* viewController);
    
protected:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    virtual void reshapeViewRectDidFinish();
    
protected:
    
    void replaceViewControllerFinish();
    
    void pushViewControllerFinish();
    
    void popViewControllerFinish();
    
    void navigationPopViewController(CANavigationBar* navigationBar, bool animated);
    
    void update(float dt);
    
    void scheduleUpdate();
    
    void unScheduleUpdate();
    
protected:
    
    CAVector<CAViewController*> m_pViewControllers;
    
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
    
    bool showSelectedViewController(CAViewController* viewController);
    
    CAViewController* getViewControllerAtIndex(unsigned int index);
    
    CAViewController* getSelectedViewController();
    
    virtual bool showSelectedViewControllerAtIndex(unsigned int index);
    
    virtual unsigned int getSelectedViewControllerAtIndex();
    
    virtual void setTabBarHidden(bool hidden, bool animated);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bTabBarHidden, TabBarHidden);
    
    CC_SYNTHESIZE_READONLY(CATabBar*, m_pTabBar, TabBar);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CABarVerticalAlignment, m_eTabBarVerticalAlignment, TabBarVerticalAlignment);
    
    void updateItem(CAViewController* viewController);
    
protected:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    virtual void reshapeViewRectDidFinish();
    
protected:
    
    void tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index);
    
    void renderingSelectedViewController();
    
    void update(float dt);
    
    void scheduleUpdate();
    
    void unScheduleUpdate();
    
protected:
    
    CAVector<CAViewController*> m_pViewControllers;
    
    unsigned int m_nSelectedIndex;
    
    CAView* m_pContainer;
};



NS_CC_END

#endif /* defined(__CrossAppx__CAViewController__) */

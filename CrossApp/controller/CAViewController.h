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
#include "control/CABar.h"
#include "view/CAPageView.h"
#include "dispatcher/CAKeypadDelegate.h"
#include "dispatcher/CAKeypadDispatcher.h"

NS_CC_BEGIN

typedef enum
{
    CABarVerticalAlignmentTop = 0,
    CABarVerticalAlignmentBottom
}CABarVerticalAlignment;

class CAWindow;
class CATabBarController;
class CANavigationController;
class CADrawerController;

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
    
    virtual CAView* getView();
    
    virtual CAResponder* nextResponder();

public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    friend class CATabBarController;
    
    friend class CANavigationController;
    
    friend class CADrawerController;
    
    friend class CAWindow;
    
protected:
    
    virtual void viewDidLoad() {};
    
    virtual void viewDidUnload() {};
    
    virtual void viewDidAppear() {};
    
    virtual void viewDidDisappear() {};
    
    virtual void reshapeViewRectDidFinish() {};
    
    virtual void keyBackClicked() {};
    
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
    
    void popToRootViewControllerAnimated(bool animated);
    
    CAViewController* popFirstViewController();
 
    CAViewController* getViewControllerAtIndex(int index);
    
    CAViewController* getBackViewController();
    
    inline unsigned long getViewControllerCount() {return m_pViewControllers.size();}
    
    virtual void setNavigationBarHidden(bool hidden, bool animated);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bNavigationBarHidden, NavigationBarHidden);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CABarVerticalAlignment, m_eNavigationBarVerticalAlignment, NavigationBarVerticalAlignment);
    
    void updateItem(CAViewController* viewController);
    
    CC_PROPERTY_IS(bool, m_bTouchMoved, TouchMoved);

    CC_PROPERTY(CAImage*, m_pNavigationBarBackGroundImage, NavigationBarBackGroundImage);

    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sNavigationBarBackGroundColor, NavigationBarBackGroundColor);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sNavigationBarTitleColor, NavigationBarTitleColor);

    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sNavigationBarButtonColor, NavigationBarButtonColor);
    
    virtual bool isReachBoundaryLeft();
    
    virtual bool isReachBoundaryRight() {return true;}
    
    virtual bool isReachBoundaryUp() {return true;}
    
    virtual bool isReachBoundaryDown() {return true;}
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    virtual CAResponder* nextResponder() {return NULL;}
    
protected:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    virtual void reshapeViewRectDidFinish();
    
protected:
    
    void createWithContainer(CAViewController* viewController);
    
    void layoutNewContainer();
    
    void popBack();
    
    void replaceViewControllerFinish();
    
    void pushViewControllerFinish();
    
    void popViewControllerFinish();
    
    void popToRootViewControllerFinish();
    
    void homingViewControllerFinish();
    
    void navigationPopViewController(CANavigationBar* navigationBar, bool animated);
    
    void navigationBarHiddenAnimation(float delay, float now, float total);

    void update(float dt);
    
    CCPoint getNavigationBarOpenPoint();
    
    CCPoint getNavigationBarTakeBackPoint();
    
    CCPoint getNavigationBarNowPoint(CAViewController* viewController);
    
protected:

    float m_fProgress;
    
    CADeque<CAViewController*> m_pViewControllers;
    
    CADeque<CANavigationBar*> m_pNavigationBars;
    
    CADeque<CAView*> m_pContainers;

    CADeque<CAView*> m_pSecondContainers;
    
    bool m_bPopViewController;

    CCSize m_tNavigationBarSize;
};

class CC_DLL CATabBarController
: public CAViewController
, public CATabBarDelegate
, public CAPageViewDelegate
, public CAScrollViewDelegate
{
    
public:
    
    CATabBarController();
    
    virtual ~CATabBarController();
    
    virtual bool initWithViewControllers(const CAVector<CAViewController*>& viewControllers,
                                         CABarVerticalAlignment var = CABarVerticalAlignmentBottom);
    
    bool showSelectedViewController(CAViewController* viewController);
    
    CAViewController* getViewControllerAtIndex(unsigned int index);
    
    CAViewController* getSelectedViewController();
    
    virtual bool showSelectedViewControllerAtIndex(unsigned int index);
    
    virtual unsigned int getSelectedViewControllerAtIndex();
    
    virtual void setTabBarHidden(bool hidden, bool animated);
    
    CC_PROPERTY_IS(bool, m_bscrollEnabled, ScrollEnabled);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bTabBarHidden, TabBarHidden);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CABarVerticalAlignment, m_eTabBarVerticalAlignment, TabBarVerticalAlignment);
    
    CC_PROPERTY(CAImage*, m_pTabBarBackGroundImage, TabBarBackGroundImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sTabBarBackGroundColor, TabBarBackGroundColor);
    
    CC_PROPERTY(CAImage*, m_pTabBarSelectedBackGroundImage, TabBarSelectedBackGroundImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sTabBarSelectedBackGroundColor, TabBarSelectedBackGroundColor);
    
    CC_PROPERTY(CAImage*, m_pTabBarSelectedIndicatorImage, TabBarSelectedIndicatorImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sTabBarSelectedIndicatorColor, TabBarSelectedIndicatorColor);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sTabBarTitleColor, TabBarTitleColorForNormal);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sTabBarSelectedTitleColor, TabBarTitleColorForSelected);
    
    void updateItem(CAViewController* viewController);
    
    void showTabBarSelectedIndicator();
 
protected:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    virtual void reshapeViewRectDidFinish();
    
protected:
    
    virtual void tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index);

    virtual void pageViewDidEndTurning(CAPageView* pageView);
    
    virtual void scrollViewWillBeginDragging(CAScrollView* view);
    
    virtual void renderingSelectedViewController();
    
    void update(float dt);
    
    void tabBarHiddenAnimation(float delay, float now, float total);
    
    CCPoint getTabBarOpenPoint();
    
    CCPoint getTabBarTakeBackPoint();
    
    CCPoint getTabBarNowPoint();
    
protected:
    
    float m_fProgress;
    
    bool m_bShowTabBarSelectedIndicator;
    
    CAVector<CAViewController*> m_pViewControllers;
    
    unsigned int m_nSelectedIndex;
    
    unsigned int m_nLastSelectedIndex;
    
    CAPageView* m_pContainer;
    
    CATabBar* m_pTabBar;
};



NS_CC_END

#endif /* defined(__CrossAppx__CAViewController__) */

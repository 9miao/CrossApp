//
//  CAViewController.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-11.
//  Copyright (c) 2014年 http://9miao.com All rights reserved.
//

#ifndef __CrossApp__CAViewController__
#define __CrossApp__CAViewController__

#include <iostream>
#include "control/CABar.h"
#include "view/CAPageView.h"
#include "basics/CAResponder.h"
NS_CC_BEGIN

class CAWindow;
class CATabBarController;
class CANavigationController;
class CADrawerController;
class CAUIEditorParser;

class CC_DLL CAViewController
: public CAObject
, public CAContentContainer
{
    
public:
    
    CAViewController();
    
    virtual ~CAViewController();
    
    virtual bool init();
    
    std::string getNibName();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sTitle, Title);
    
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pNavigationController, NavigationController);
    
    CC_SYNTHESIZE_READONLY(CANavigationBarItem*, m_pNavigationBarItem, NavigationBarItem);
    
    CC_SYNTHESIZE_READONLY(CATabBarController*, m_pTabBarController, TabBarController);
    
    CC_SYNTHESIZE_READONLY(CATabBarItem*, m_pTabBarItem, TabBarItem);
    
    CC_SYNTHESIZE_READONLY(CAViewController*, m_pModalViewController, ModalViewController);

    bool isViewRunning();
    
    void setNavigationBarItem(CANavigationBarItem* item);
    
    void setTabBarItem(CATabBarItem* item);
    
    void presentModalViewController(CAViewController* controller, bool animated);
    
    void dismissModalViewController(bool animated);
    
    CAView* getView();
    
    void setView(CAView* view);
    
    CAView* getViewWithID(const std::string& tag);

public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void viewDidLoad() {};
    
    virtual void viewDidUnload() {};
    
    virtual void viewDidAppear(){};
    
    virtual void viewDidDisappear(){};
    
    virtual void viewSizeDidChanged() {};
    
    virtual void addViewFromSuperview(CAView* node);
    
    virtual void removeViewFromSuperview();
    
public:
        
    void viewOnEnterTransitionDidFinish();
    
    void viewOnExitTransitionDidStart();
    
    void viewOnSizeTransitionDidChanged();
    
    virtual void setViewVisibleTrue();
    
    virtual void setViewVisibleFalse();
    
    void parser();
        
    friend class CATabBarController;
    
    friend class CANavigationController;
    
    friend class CADrawerController;
    
    friend class CAWindow;
    
private:
    
    CAView* m_pView;
    
    CAUIEditorParser* m_pParser;
    
    CAVector<CATouch*> m_vTouches;
};

class CC_DLL CANavigationController
: public CAViewController
{
    
public:
    
    CANavigationController();
    
    virtual ~CANavigationController();
    
    virtual bool initWithRootViewController(CAViewController* viewController);
    
public:
    
    virtual void replaceViewController(CAViewController* viewController, bool animated);
    
    virtual void pushViewController(CAViewController* viewController, bool animated);
    
    CAViewController* popViewControllerAnimated(bool animated);
    
    void popToRootViewControllerAnimated(bool animated);
    
    CAViewController* popFirstViewController();
 
    CAViewController* popViewControllerAtIndex(int index);
    
    CAViewController* getViewControllerAtIndex(int index);
    
    CAViewController* getBackViewController();
    
    inline unsigned long getViewControllerCount() {return m_pViewControllers.size();}
    
    virtual void setNavigationBarHidden(bool hidden, bool animated);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bNavigationBarHidden, NavigationBarHidden);
  
    void updateItem(CAViewController* viewController);
    
    CC_PROPERTY_IS(bool, m_bTouchMoved, TouchMoved);

    CC_PROPERTY(CAImage*, m_pNavigationBarBackgroundImage, NavigationBarBackgroundImage);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sNavigationBarTitleColor, NavigationBarTitleColor);

    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sNavigationBarButtonColor, NavigationBarButtonColor);
    
    CC_PROPERTY(CABarButtonItem*, m_pNavigationBarGoBackBarButtonItem, NavigationBarGoBackBarButtonItem);
    
    CC_PROPERTY_IS(bool, m_bNavigationBarEnabledShadow, NavigationBarEnabledShadow);

    virtual bool isReachBoundaryLeft();
    
    virtual bool isReachBoundaryRight() {return true;}
    
    virtual bool isReachBoundaryUp() {return true;}
    
    virtual bool isReachBoundaryDown() {return true;}
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);

protected:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    virtual void viewSizeDidChanged();
    
protected:
    
    void layout();
    
    void createWithContainer(CAViewController* viewController, const DLayout& layout);
    
    void popBack();
    
    void replaceViewControllerFinish();
    
    void pushViewControllerFinish();
    
    void popViewControllerFinish();
    
    void popToRootViewControllerFinish();
    
    void homingViewControllerFinish();

    void update(float dt);
    
    int getNavigationBarNowY(CAViewController* viewController);
    
protected:

    float m_fProgress;
    
    CADeque<CAViewController*> m_pViewControllers;
    
    CADeque<CANavigationBar*> m_pNavigationBars;
    
    CADeque<CAView*> m_pContainers;

    CADeque<CAView*> m_pSecondContainers;
    
    bool m_bPopViewController;

    bool m_bTouchBeganRange;
    
    int m_iNavigationBarHeight;
    
    int m_iClearance;
};

class CC_DLL CATabBarController
: public CAViewController
{
    
public:
    
    CATabBarController();
    
    virtual ~CATabBarController();
    
    virtual bool initWithViewControllers(const CAVector<CAViewController*>& viewControllers,
                                         CATabBar::VerticalAlignment var = CATabBar::VerticalAlignment::Bottom);
    
    bool showSelectedViewController(CAViewController* viewController);
    
    CAViewController* getViewControllerAtIndex(unsigned int index);
    
    CAViewController* getSelectedViewController();
    
    virtual bool showSelectedViewControllerAtIndex(unsigned int index);
    
    virtual unsigned int getSelectedViewControllerAtIndex();
    
    virtual void setTabBarHidden(bool hidden, bool animated);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bTabBarHidden, TabBarHidden);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CATabBar::VerticalAlignment, m_eTabBarVerticalAlignment, TabBarVerticalAlignment);
    
    CC_PROPERTY(CAImage*, m_pTabBarBackgroundImage, TabBarBackgroundImage);
        
    CC_PROPERTY(CAImage*, m_pTabBarSelectedBackgroundImage, TabBarSelectedBackgroundImage);
        
    CC_PROPERTY(CAImage*, m_pTabBarSelectedIndicatorImage, TabBarSelectedIndicatorImage);
        
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sTabBarTitleColor, TabBarTitleColorForNormal);
    
    CC_PROPERTY_PASS_BY_REF(CAColor4B, m_sTabBarSelectedTitleColor, TabBarTitleColorForSelected);
    
    CC_PROPERTY(bool, m_bTabBarSelectedTitleBold, TabBarTitleBoldForSelected);

    CATabBar* getTabBar();
    
    void updateItem(CAViewController* viewController);
    
    void showTabBarSelectedIndicator();
 
protected:
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    virtual void viewSizeDidChanged();
    
protected:
    
    void layout();
    
    virtual void tabBarSelectedItem(CATabBarItem* item, unsigned int index);

    virtual void renderingSelectedViewController();
    
    void update(float dt);
    
    int getTabBarNowY();
    
protected:
    
    int m_iTabBarHeight;
    
    float m_fProgress;
    
    bool m_bShowTabBarSelectedIndicator;
    
    CAVector<CAViewController*> m_pViewControllers;
    
    unsigned int m_nSelectedIndex;
    
    unsigned int m_nLastSelectedIndex;

    CATabBar* m_pTabBar;
    
    DLayout m_obTabBarLayout;
    
    DLayout m_obViewLayout;
};



NS_CC_END

#endif /* defined(__CrossApp__CAViewController__) */

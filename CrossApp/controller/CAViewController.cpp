//
//  CAViewController.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-11.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAViewController.h"
#include "basics/CAApplication.h"
#include "support/CAPointExtension.h"
#include "basics/CAScheduler.h"
#include "view/CAWindow.h"
#include "view/CAScale9ImageView.h"
#include "dispatcher/CATouchDispatcher.h"
#include "animation/CAViewAnimation.h"
#include "animation/CAAnimation.h"
#include "support/CAUIEditorParser.h"
#include "support/device/CADevice.h"
NS_CC_BEGIN

CAViewController::CAViewController()
:m_pView(NULL)
,m_pNavigationController(NULL)
,m_pNavigationBarItem(NULL)
,m_pTabBarController(NULL)
,m_pTabBarItem(NULL)
,m_sTitle("The Title")
,m_bLifeLock(false)
,m_bKeypadEnabled(false)
,m_pParser(NULL)
{
    m_pView = CAView::createWithColor(CAColor_white);
    m_pView->retain();
    m_pView->setContentContainer(this);
    m_pView->setLayout(DLayoutFill);
}

CAViewController::~CAViewController()
{
    CC_SAFE_RELEASE_NULL(m_pParser);
    m_pView->setContentContainer(NULL);
    CC_SAFE_RELEASE_NULL(m_pView);
    CC_SAFE_RELEASE_NULL(m_pTabBarItem);
    CC_SAFE_RELEASE_NULL(m_pNavigationBarItem);
    if (m_bKeypadEnabled)
    {
        CAApplication::getApplication()->getKeypadDispatcher()->removeDelegate(this);
    }
}

bool CAViewController::init()
{
    return true;
}

CAView* CAViewController::getViewWithID(const std::string &tag)
{
    return m_pParser->m_mViews.getValue(tag);
}

void CAViewController::parser()
{
    m_pParser = new CAUIEditorParser();
    
    std::string name = typeid(*this).name();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    name = name.substr(6, name.length() - 6);
#else
    name = name.substr(2, name.length() - 2);
#endif
    
    std::string filePath = "r/" + name + ".xib";

    m_pParser->initWithPath(filePath, m_pView);
    m_pParser->parseViewControllItems(this);
    
}

void CAViewController::viewOnEnterTransitionDidFinish()
{
    CAScheduler::getScheduler()->resumeTarget(this);
    
    do
    {
        CC_BREAK_IF(m_bLifeLock);
        m_bLifeLock = true;
        this->parser();
        this->viewDidLoad();
    }
    while (0);

}

void CAViewController::viewOnExitTransitionDidStart()
{
    CAScheduler::getScheduler()->pauseTarget(this);

    do
    {
        CC_BREAK_IF(m_bLifeLock);
        m_bLifeLock = true;
        this->viewDidUnload();
        m_pView->removeAllSubviews();
    }
    while (0);
    
}

void CAViewController::viewOnSizeTransitionDidChanged()
{
    if (m_bLifeLock)
    {
        this->viewSizeDidChanged();
    }
}

std::string CAViewController::getNibName()
{
    return typeid(*this).name();
}

CAView* CAViewController::getView()
{
    return m_pView;
}

void CAViewController::addViewFromSuperview(CAView* node)
{
    m_bLifeLock = false;
    node->addSubview(m_pView);
}

void CAViewController::removeViewFromSuperview()
{
    m_bLifeLock = false;
    m_pView->removeFromSuperview();
}

bool CAViewController::isViewRunning()
{
    return (bool)(m_pView->getSuperview() && m_pView->isVisible());
}

/// isKeypadEnabled getter
bool CAViewController::isKeypadEnabled()
{
    return m_bKeypadEnabled;
}

/// isKeypadEnabled setter
void CAViewController::setKeypadEnabled(bool enabled)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (enabled != m_bKeypadEnabled)
    {
        m_bKeypadEnabled = enabled;
        if (enabled)
        {
            CAApplication::getApplication()->getKeypadDispatcher()->addDelegate(this);
        }
        else
        {
            CAApplication::getApplication()->getKeypadDispatcher()->removeDelegate(this);
        }
    }
#endif
}

void CAViewController::setNavigationBarItem(CANavigationBarItem* item)
{
    CC_SAFE_RETAIN(item);
    CC_SAFE_RELEASE(m_pNavigationBarItem);
    m_pNavigationBarItem = item;
    
    if (item)
    {
        if (m_pNavigationController)
        {
            m_pNavigationController->updateItem(this);
        }
    }
}

void CAViewController::setTabBarItem(CATabBarItem* item)
{
    CC_SAFE_RETAIN(item);
    CC_SAFE_RELEASE(m_pTabBarItem);
    m_pTabBarItem = item;
    
    if (item)
    {
        if (m_pTabBarController)
        {
            m_pTabBarController->updateItem(this);
        }
    }
}

bool CAViewController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return true;
}

void CAViewController::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{

}

void CAViewController::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{

}

void CAViewController::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{

}

CAResponder* CAViewController::nextResponder()
{
    return m_pView->getSuperview();
}

void CAViewController::presentModalViewController(CAViewController* controller, bool animated)
{
    CAApplication::getApplication()->getRootWindow()->presentModalViewController(controller, animated);
}


void CAViewController::dismissModalViewController(bool animated)
{
    CAApplication::getApplication()->getRootWindow()->dismissModalViewController(animated);
}


#pragma CANavigationController

CANavigationController::CANavigationController()
:m_bNavigationBarHidden(false)
,m_bPopViewController(false)
,m_pNavigationBarBackgroundImage(NULL)
,m_sNavigationBarBackgroundColor(CAColor_white)
,m_sNavigationBarTitleColor(CAColor_white)
,m_sNavigationBarButtonColor(CAColor_white)
,m_fProgress(1.0f)
,m_bClearance(false)
{
    m_pView->setColor(CAColor_clear);
    m_pView->setDisplayRange(false);
    this->setHaveNextResponder(false);
    this->setTouchMoved(true);
    this->setVerticalScrollEnabled(false);
    this->setNavigationBarBackgroundImage(CAImage::create("source_material/navigation_bg.png"));
}

CANavigationController::~CANavigationController()
{
    m_pViewControllers.clear();
    m_pNavigationBars.clear();
    m_pContainers.clear();
    m_pSecondContainers.clear();
    CC_SAFE_RELEASE_NULL(m_pNavigationBarBackgroundImage);
}

void CANavigationController::setNavigationBarBackgroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pNavigationBarBackgroundImage);
    m_pNavigationBarBackgroundImage = var;
    m_sNavigationBarBackgroundColor = CAColor_white;
    
    if (!m_pNavigationBars.empty())
    {
        CADeque<CANavigationBar*>::iterator itr;
        for (itr=m_pNavigationBars.begin(); itr!=m_pNavigationBars.end(); itr++)
        {
            if (m_pNavigationBarBackgroundImage)
            {
                (*itr)->setBackgroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackgroundImage));
            }
            else
            {
                (*itr)->setBackgroundView(CAView::create());
            }
            (*itr)->getBackgroundView()->setColor(m_sNavigationBarBackgroundColor);
        }
    }
}

CAImage* CANavigationController::getNavigationBarBackgroundImage()
{
    return m_pNavigationBarBackgroundImage;
}

void CANavigationController::setNavigationBarBackgroundColor(const CAColor4B &var)
{
    m_sNavigationBarBackgroundColor = var;
    CC_SAFE_RELEASE_NULL(m_pNavigationBarBackgroundImage);
    
    if (!m_pNavigationBars.empty())
    {
        CADeque<CANavigationBar*>::iterator itr;
        for (itr=m_pNavigationBars.begin(); itr!=m_pNavigationBars.end(); itr++)
        {
            if (m_pNavigationBarBackgroundImage)
            {
                (*itr)->setBackgroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackgroundImage));
            }
            else
            {
                (*itr)->setBackgroundView(CAView::create());
            }
            (*itr)->getBackgroundView()->setColor(m_sNavigationBarBackgroundColor);
        }
    }
}

const CAColor4B& CANavigationController::getNavigationBarBackgroundColor()
{
    return m_sNavigationBarBackgroundColor;
}

void CANavigationController::setNavigationBarTitleColor(const CAColor4B &var)
{
    m_sNavigationBarTitleColor = var;
    
    if (!m_pNavigationBars.empty())
    {
        CADeque<CANavigationBar*>::iterator itr;
        for (itr=m_pNavigationBars.begin(); itr!=m_pNavigationBars.end(); itr++)
        {
            (*itr)->setTitleColor(m_sNavigationBarTitleColor);
        }
    }
}

const CAColor4B& CANavigationController::getNavigationBarTitleColor()
{
    return m_sNavigationBarTitleColor;
}

void CANavigationController::setNavigationBarButtonColor(const CAColor4B &var)
{
    m_sNavigationBarButtonColor = var;
    
    if (!m_pNavigationBars.empty())
    {
        CADeque<CANavigationBar*>::iterator itr;
        for (itr=m_pNavigationBars.begin(); itr!=m_pNavigationBars.end(); itr++)
        {
            (*itr)->setButtonColor(m_sNavigationBarButtonColor);
        }
    }
}

const CAColor4B& CANavigationController::getNavigationBarButtonColor()
{
    return m_sNavigationBarButtonColor;
}

bool CANavigationController::initWithRootViewController(CAViewController* viewController)
{
    CAViewController::init();
    
    if (viewController == NULL)
    {
        return false;
    }
    
    if (viewController->getView() == NULL)
    {
        return false;
    }

    m_pViewControllers.pushBack(viewController);
    
    return true;
}

void CANavigationController::updateItem(CAViewController* viewController)
{
    size_t index = m_pViewControllers.getIndex(viewController);
    CC_RETURN_IF(index == -1);
    if (index == 0)
    {
        viewController->getNavigationBarItem()->setShowGoBackButton(false);
    }
    
    if (m_pNavigationBarBackgroundImage)
    {
        m_pNavigationBars.at(index)->setBackgroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackgroundImage));
    }
    else
    {
        m_pNavigationBars.at(index)->setBackgroundView(CAView::create());
        m_pNavigationBars.at(index)->getBackgroundView()->setColor(m_sNavigationBarBackgroundColor);
    }

    m_pNavigationBars.at(index)->setTitleColor(m_sNavigationBarTitleColor);
    m_pNavigationBars.at(index)->setTitleColor(m_sNavigationBarTitleColor);
    m_pNavigationBars.at(index)->setItem(viewController->getNavigationBarItem());
}

void CANavigationController::viewDidLoad()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (atof(CADevice::getSystemVersionWithIOS()) >= 7.0f)
    {
        m_bClearance = this->getView()->convertToWorldSpace(DPointZero).y < 1;
    }
#endif
    
    m_iNavigationBarHeight = m_bClearance ? 128 : 88;
    
    CAViewController* viewController = m_pViewControllers.front();
    viewController->retain()->autorelease();
    m_pViewControllers.popFront();
    this->createWithContainer(viewController, DLayoutFill);
}

void CANavigationController::viewDidUnload()
{
    for (CADeque<CAViewController*>::iterator itr=m_pViewControllers.begin();
         itr!=m_pViewControllers.end();
         itr++)
    {
        (*itr)->removeViewFromSuperview();
    }
    
    for (CADeque<CAView*>::iterator itr=m_pContainers.begin();
         itr!=m_pContainers.end();
         itr++)
    {
        (*itr)->removeAllSubviews();
        (*itr)->removeFromSuperview();
    }
}

void CANavigationController::viewDidAppear()
{
    CC_RETURN_IF(m_pViewControllers.empty());
    m_pViewControllers.back()->viewDidAppear();
}

void CANavigationController::viewDidDisappear()
{
    CC_RETURN_IF(m_pViewControllers.empty());
    m_pViewControllers.back()->viewDidDisappear();
}

void CANavigationController::createWithContainer(CAViewController* viewController, const DLayout& layout)
{
    CAView* container = new CAView();
    container->setLayout(layout);
    this->getView()->addSubview(container);
    m_pContainers.pushBack(container);
    container->release();
    
    DLayout navLayout;
    navLayout.horizontal = DHorizontalLayoutFill;
    navLayout.vertical.height = m_iNavigationBarHeight;
    
    if (m_bNavigationBarHidden
        ||
        (viewController->getNavigationBarItem() && viewController->getNavigationBarItem()->isNagigationBarHidden()))
    {
        navLayout.vertical.top = -m_iNavigationBarHeight;
    }
    else
    {
        navLayout.vertical.top = 0;
    }
    
    CANavigationBar* navigationBar = CANavigationBar::createWithLayout(navLayout, m_bClearance);
    if (viewController->getNavigationBarItem() == NULL && viewController->getTitle().compare("") != 0)
    {
        viewController->setNavigationBarItem(CANavigationBarItem::create(viewController->getTitle()));
    }
    if (m_pViewControllers.empty())
    {
        viewController->getNavigationBarItem()->setShowGoBackButton(false);
    }
    navigationBar->setItem(viewController->getNavigationBarItem());
    
    if (m_pNavigationBarBackgroundImage)
    {
        navigationBar->setBackgroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackgroundImage));
    }
    else
    {
        navigationBar->setBackgroundView(CAView::create());
    }
    navigationBar->getBackgroundView()->setColor(m_sNavigationBarBackgroundColor);
    navigationBar->setTitleColor(m_sNavigationBarTitleColor);
    navigationBar->setTitleColor(m_sNavigationBarTitleColor);
    container->insertSubview(navigationBar, 1);
    navigationBar->setDelegate(this);
    m_pNavigationBars.pushBack(navigationBar);
    
    DLayout secondLayout;
    secondLayout.horizontal = DHorizontalLayoutFill;
    secondLayout.vertical = DVerticalLayout_T_B(navLayout.vertical.top + navLayout.vertical.height, 0);
    
    CAView* secondContainer = new CAView();
    secondContainer->setLayout(secondLayout);
    container->addSubview(secondContainer);
    secondContainer->release();
    m_pSecondContainers.pushBack(secondContainer);
    
    viewController->m_pNavigationController = this;
    m_pViewControllers.pushBack(viewController);
    viewController->addViewFromSuperview(secondContainer);
    if (m_pViewControllers.size() > 1)
    {
        viewController->viewDidAppear();
    }
}

void CANavigationController::replaceViewController(CrossApp::CAViewController *viewController, bool animated)
{
    if (this->getView()->getSuperview() == NULL)
    {
        return;
    }
    
    if (m_pNavigationBars.size() == 1)
    {
        CANavigationBarItem* item = viewController->getNavigationBarItem();
        if (item == NULL)
        {
            item = CANavigationBarItem::create("");
        }
        item->setShowGoBackButton(false);
        viewController->setNavigationBarItem(item);
    }
    
    float x = this->getView()->getBounds().size.width;

    CAView* lastContainer = m_pContainers.back();
    this->createWithContainer(viewController, DLayout(DHorizontalLayout_L_R(x, -x), DVerticalLayoutFill));
    CAView* newContainer = m_pContainers.back();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/30.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        lastContainer->setFrameOrigin(DPoint(-x/3.0f, 0));
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/60.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::replaceViewControllerFinish));
        newContainer->setFrameOrigin(DPointZero);
        CAViewAnimation::commitAnimations();
    }
    else
    {
        this->replaceViewControllerFinish();
    }
}

void CANavigationController::replaceViewControllerFinish()
{
    CAView* newContainer = m_pContainers.back();
    newContainer->setLayout(DLayoutFill);
    size_t index = m_pViewControllers.size() - 2;
    CAViewController* lastViewController = m_pViewControllers.at(index);
    lastViewController->viewDidDisappear();
    lastViewController->m_pNavigationController = NULL;
    lastViewController->removeViewFromSuperview();
    m_pViewControllers.erase(index);
    
    CAView* backContainer = m_pContainers.at(index);
    backContainer->removeFromSuperview();
    m_pContainers.erase(index);
    m_pSecondContainers.erase(index);
    m_pNavigationBars.erase(index);
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

void CANavigationController::pushViewController(CAViewController* viewController, bool animated)
{
    if (this->getView()->getSuperview() == NULL)
    {
        return;
    }

    float x = this->getView()->getBounds().size.width;
    
    CAView* lastContainer = m_pContainers.back();
    this->createWithContainer(viewController, DLayout(DHorizontalLayout_L_R(x, -x), DVerticalLayoutFill));
    CAView* newContainer = m_pContainers.back();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/30.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        lastContainer->setFrameOrigin(DPoint(-x/3.0f, 0));
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/60.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::pushViewControllerFinish));
        newContainer->setFrameOrigin(DPointZero);
        CAViewAnimation::commitAnimations();
        
    }
    else
    {
        this->pushViewControllerFinish();
    }
}

void CANavigationController::pushViewControllerFinish()
{
    CAView* lastContainer = m_pContainers.at(m_pContainers.size() - 2);
    lastContainer->setVisible(false);
    
    CAView* newContainer = m_pContainers.back();
    newContainer->setLayout(DLayoutFill);
    
    CAViewController* lastViewController = m_pViewControllers.at(m_pViewControllers.size() - 2);
    lastViewController->viewDidDisappear();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

CAViewController* CANavigationController::popViewControllerAnimated(bool animated)
{
    if (m_pViewControllers.size() == 1)
    {
        return NULL;
    }

    float x = this->getView()->getBounds().size.width;
    
    CAViewController* backViewController = m_pViewControllers.back();
    CAView* backContainer = m_pContainers.back();
    {
        backContainer->setFrameOrigin(DPointZero);
    }
    
    size_t index = m_pViewControllers.size() - 2;
    CAViewController* showViewController = m_pViewControllers.at(index);
    showViewController->viewDidAppear();
    
    CAView* showContainer = m_pContainers.at(index);
    showContainer->setVisible(true);
    
    
    {
        int y = this->getNavigationBarNowY(showViewController);
        
        CANavigationBar* navBar = m_pNavigationBars.at(index);
        DLayout navLayout = navBar->getLayout();
        navLayout.vertical.top = y;
        navBar->setLayout(navLayout);
        
        CAView* secondContainer = m_pSecondContainers.at(index);
        DLayout secondLayout = secondContainer->getLayout();
        secondLayout.vertical.top = navLayout.vertical.top + navLayout.vertical.height;
        secondContainer->setLayout(secondLayout);
    }
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        DRect rect = this->getView()->getBounds();
        rect.origin = DPoint(-x/3.0f, 0);
        showContainer->setFrame(rect);
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/30.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        showContainer->setFrameOrigin(DPointZero);
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/60.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::popViewControllerFinish));
        backContainer->setFrameOrigin(DPoint(x, 0));
        CAViewAnimation::commitAnimations();
    }
    else
    {
        this->popViewControllerFinish();
    }
    return backViewController;
}

void CANavigationController::popViewControllerFinish()
{
    CAViewController* backViewController = m_pViewControllers.back();
    backViewController->viewDidDisappear();
    backViewController->m_pNavigationController = NULL;
    backViewController->removeViewFromSuperview();
    backViewController->retain()->autorelease();
    m_pViewControllers.popBack();
    
    m_pContainers.back()->removeFromSuperview();
    m_pContainers.popBack();
    
    m_pSecondContainers.popBack();
    
    m_pNavigationBars.popBack();
    
    m_pContainers.back()->setLayout(DLayoutFill);
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

// sprhawk@163.com: 2015-03-08
void CANavigationController::popToRootViewControllerAnimated(bool animated)
{
    if (m_pViewControllers.size() == 1)
    {
        return ;
    }
    
    float x = this->getView()->getBounds().size.width;
    
    CAView* backContainer = m_pContainers.back();
    backContainer->setFrameOrigin(DPointZero);
    
    size_t index = 0;
    CAViewController* showViewController = m_pViewControllers.at(index);
    showViewController->viewDidAppear();
    
    CAView* showContainer = m_pContainers.at(index);
    showContainer->setVisible(true);
    showContainer->setFrameOrigin(DPoint(-x/3.0f, 0));
    
    {
        int y = this->getNavigationBarNowY(showViewController);
        
        CANavigationBar* navBar = m_pNavigationBars.at(index);
        DLayout navLayout = navBar->getLayout();
        navLayout.vertical.top = y;
        navBar->setLayout(navLayout);
        
        CAView* secondContainer = m_pSecondContainers.at(index);
        DLayout secondLayout = secondContainer->getLayout();
        secondLayout.vertical.top = navLayout.vertical.top + navLayout.vertical.height;
        secondContainer->setLayout(secondLayout);
    }
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        showContainer->setFrameOrigin(DPointZero);
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::popToRootViewControllerFinish));
        backContainer->setFrameOrigin(DPoint(x, 0));
        CAViewAnimation::commitAnimations();
    }
    else
    {
        this->popToRootViewControllerFinish();
    }
}

// sprhawk@163.com: 2015-03-08
void CANavigationController::popToRootViewControllerFinish()
{
    while (m_pViewControllers.size() > 1)
    {
        CAViewController* backViewController = m_pViewControllers.back();
        backViewController->viewDidDisappear();
        backViewController->m_pNavigationController = NULL;
        backViewController->removeViewFromSuperview();
        m_pViewControllers.popBack();
        
        m_pContainers.back()->removeFromSuperview();
        m_pContainers.popBack();
        
        m_pSecondContainers.popBack();
        
        m_pNavigationBars.popBack();
    }
    
    m_pContainers.back()->setLayout(DLayoutFill);
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

void CANavigationController::homingViewControllerFinish()
{
    size_t index = m_pViewControllers.size() - 2;

    CAView* lastContainer = m_pContainers.at(index);
    lastContainer->setVisible(false);
    
    m_pContainers.back()->setLayout(DLayoutFill);
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

CAViewController* CANavigationController::popFirstViewController()
{
    if (m_pViewControllers.size() <= 1)
    {
        return NULL;
    }
    
    CAViewController* frontViewController = m_pViewControllers.front();
    frontViewController->viewDidDisappear();
    frontViewController->m_pNavigationController = NULL;
    frontViewController->removeViewFromSuperview();
    frontViewController->retain()->autorelease();
    m_pViewControllers.popFront();
    
    m_pContainers.front()->removeFromSuperview();
    m_pContainers.popFront();
    
    m_pSecondContainers.popFront();
    
    m_pNavigationBars.popFront();
    
    return frontViewController;
}

CAViewController* CANavigationController::popViewControllerAtIndex(int index)
{
    if (index >= m_pViewControllers.size() || index < 0)
    {
        return NULL;
    }
    
    if (index == m_pViewControllers.size() - 1)
    {
        return this->popViewControllerAnimated(false);
    }
    
    CAViewController* viewController = m_pViewControllers.at(index);
    viewController->viewDidDisappear();
    viewController->m_pNavigationController = NULL;
    viewController->removeViewFromSuperview();
    viewController->retain()->autorelease();
    m_pViewControllers.erase(index);
    
    m_pContainers.at(index)->removeFromSuperview();
    m_pContainers.erase(index);
    
    m_pSecondContainers.erase(index);
    
    m_pNavigationBars.erase(index);
    
    return viewController;
}

void CANavigationController::navigationPopViewController(CANavigationBar* navigationBar, bool animated)
{
    this->popViewControllerAnimated(animated);
}

CAViewController* CANavigationController::getViewControllerAtIndex(int index)
{
    do
    {
        CC_BREAK_IF(index < 0);
        CC_BREAK_IF(index >= m_pViewControllers.size());
        return m_pViewControllers.at(index);
    }
    while (0);
    
    return NULL;
}

CAViewController* CANavigationController::getBackViewController()
{
    return m_pViewControllers.back();
}

void CANavigationController::setNavigationBarHidden(bool hidden, bool animated)
{
    CC_RETURN_IF(m_bNavigationBarHidden == hidden);
    m_bNavigationBarHidden = hidden;
    CC_RETURN_IF(this->getView()->getSuperview() == NULL);
    
    if (animated)
    {
        CAAnimation::schedule(CAAnimation_selector(CANavigationController::navigationBarHiddenAnimation), this, 0.25f);
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationWillStartSelector(CAApplication::getApplication()->getTouchDispatcher(), CAViewAnimation0_selector(CATouchDispatcher::setDispatchEventsFalse));
        CAViewAnimation::setAnimationDidStopSelector(CAApplication::getApplication()->getTouchDispatcher(), CAViewAnimation0_selector(CATouchDispatcher::setDispatchEventsTrue));
        CAViewAnimation::commitAnimations();
    }
    else
    {
        m_fProgress = 1.0f;

        if (this->getView()->getSuperview())
        {
            this->update(0);
        }
    }
}

int CANavigationController::getNavigationBarNowY(CAViewController* viewController)
{
    int y = 0;
    
    if (viewController->getNavigationBarItem() && viewController->getNavigationBarItem()->isNagigationBarHidden())
    {
        y = -m_iNavigationBarHeight;
    }
    else
    {
        if (m_bNavigationBarHidden)
        {
            y = -m_iNavigationBarHeight * m_fProgress;
        }
        else
        {
            y = -m_iNavigationBarHeight * (1.0f - m_fProgress);
        }
    }
    return y;
}

void CANavigationController::navigationBarHiddenAnimation(float delay, float now, float total)
{
    m_fProgress = now / total;

    if (this->getView()->getSuperview())
    {
        this->update(0);
    }
}

void CANavigationController::update(float dt)
{
    CAViewController* viewController = m_pViewControllers.back();
    
    int y = this->getNavigationBarNowY(viewController);
    
    CANavigationBar* navBar = m_pNavigationBars.back();
    DLayout navLayout = navBar->getLayout();
    navLayout.vertical.top = y;
    navBar->setLayout(navLayout);
    
    CAView* secondContainer = m_pSecondContainers.back();
    DLayout secondLayout = secondContainer->getLayout();
    secondLayout.vertical.top = navLayout.vertical.top + navLayout.vertical.height;
    secondContainer->setLayout(secondLayout);
}

bool CANavigationController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (pTouch->getLocation().x > 48)
    {
        return false;
    }
    
    m_bPopViewController = false;
    return true;
}

void CANavigationController::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_pViewControllers.size() <= 1);
    CC_RETURN_IF(m_bTouchMoved == false);
    float offDis = pTouch->getLocation().x - pTouch->getPreviousLocation().x;
    CAView* showContainer = m_pContainers.at(m_pContainers.size() - 2);
    if (!showContainer->getBounds().size.equals(this->getView()->getBounds().size))
    {
        showContainer->setFrame(this->getView()->getBounds());
    }
    showContainer->setVisible(true);
    showContainer->setTouchEnabled(false);
    
    CAView* backContainer = m_pContainers.back();
    
    DPoint point1 = backContainer->getFrameOrigin();
    point1.x += offDis;
    point1.x = MAX(point1.x, 0);
    point1.x = MIN(point1.x, this->getView()->getBounds().size.width);
    backContainer->setFrameOrigin(point1);
    backContainer->setTouchEnabled(false);
    
    DPoint point2 = showContainer->getFrameOrigin();
    point2.x = -this->getView()->getBounds().size.width/3.0f + point1.x/3.0f;
    showContainer->setFrameOrigin(point2);
    
    m_bPopViewController = ((offDis > 10) || point1.x > this->getView()->getBounds().size.width/4);
}

void CANavigationController::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(CAViewAnimation::areBeginAnimationsWithID("navigation_animation"));
    CC_RETURN_IF(m_pViewControllers.size() <= 1);
    
    float x = this->getView()->getBounds().size.width;
    size_t index = m_pViewControllers.size() - 2;
    CAViewController* lastViewController = m_pViewControllers.at(index);
    
    CAView* lastContainer = m_pContainers.at(index);
    lastContainer->setVisible(true);
    lastContainer->setTouchEnabled(true);
    
    CAView* backContainer = m_pContainers.back();
    backContainer->setTouchEnabled(true);
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (m_bPopViewController)
    {
        lastViewController->viewDidAppear();
        
        CAViewAnimation::beginAnimations("navigation_animation", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        lastContainer->setFrameOrigin(DPointZero);
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("navigation_animation2", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::popViewControllerFinish));
        backContainer->setFrameOrigin(DPoint(x, 0));
        CAViewAnimation::commitAnimations();
    }
    else
    {
        CAViewAnimation::beginAnimations("navigation_animation", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        lastContainer->setFrameOrigin(DPoint(-x/3.0f, 0));
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("navigation_animation2", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::homingViewControllerFinish));
        backContainer->setFrameOrigin(DPointZero);
        CAViewAnimation::commitAnimations();
    }
}

void CANavigationController::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

void CANavigationController::setTouchMoved(bool var)
{
    m_bTouchMoved = var;
    this->setPriorityScroll(var);
}

bool CANavigationController::isTouchMoved()
{
    return m_bTouchMoved;
}

bool CANavigationController::isReachBoundaryLeft()
{
    return m_pViewControllers.size() == 1;
}

#pragma CATabBarController

CATabBarController::CATabBarController()
:m_nSelectedIndex(0)
,m_nLastSelectedIndex(UINT_NONE)
,m_pTabBar(NULL)
,m_pContainer(NULL)
,m_bTabBarHidden(false)
,m_pTabBarBackgroundImage(NULL)
,m_pTabBarSelectedBackgroundImage(NULL)
,m_pTabBarSelectedIndicatorImage(NULL)
,m_sTabBarBackgroundColor(CAColor_white)
,m_sTabBarSelectedBackgroundColor(CAColor_white)
,m_sTabBarSelectedIndicatorColor(CAColor_white)
,m_sTabBarTitleColor(CAColor_white)
,m_sTabBarSelectedTitleColor(ccc4(50, 193, 255, 255))
,m_bShowTabBarSelectedIndicator(false)
,m_fProgress(1.0f)
,m_iTabBarHeight(0)
{
    this->setScrollEnabled(false);
    m_pView->setColor(CAColor_clear);
    m_pView->setDisplayRange(false);
    
    this->setTabBarBackgroundImage(CAImage::create("source_material/tabBar_bg.png"));
    
    this->setTabBarSelectedBackgroundImage(CAImage::create("source_material/tabBar_selected_bg.png"));
    
    this->setTabBarSelectedIndicatorImage(CAImage::create("source_material/tabBar_selected_indicator.png"));
}

CATabBarController::~CATabBarController()
{
    m_pViewControllers.clear();
    CC_SAFE_RELEASE_NULL(m_pTabBarBackgroundImage);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedBackgroundImage);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedIndicatorImage);
}

void CATabBarController::setTabBarBackgroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pTabBarBackgroundImage);
    m_pTabBarBackgroundImage = var;
    m_sTabBarBackgroundColor = CAColor_white;
    
    if (m_pTabBar)
    {
        m_pTabBar->setBackgroundImage(m_pTabBarBackgroundImage);
    }
}

CAImage* CATabBarController::getTabBarBackgroundImage()
{
    return m_pTabBarBackgroundImage;
}

void CATabBarController::setTabBarBackgroundColor(const CAColor4B &var)
{
    m_sTabBarBackgroundColor = var;
    CC_SAFE_RELEASE_NULL(m_pTabBarBackgroundImage);
    
    if (m_pTabBar)
    {
        m_pTabBar->setBackgroundColor(m_sTabBarBackgroundColor);
    }
}

const CAColor4B& CATabBarController::getTabBarBackgroundColor()
{
    return m_sTabBarBackgroundColor;
}

void CATabBarController::setTabBarSelectedBackgroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedBackgroundImage);
    m_pTabBarSelectedBackgroundImage = var;
    m_sTabBarSelectedBackgroundColor = CAColor_white;
    
    if (m_pTabBar)
    {
        m_pTabBar->setSelectedBackgroundImage(m_pTabBarSelectedBackgroundImage);
    }
}

CAImage* CATabBarController::getTabBarSelectedBackgroundImage()
{
    return m_pTabBarSelectedBackgroundImage;
}

void CATabBarController::setTabBarSelectedBackgroundColor(const CAColor4B &var)
{
    m_sTabBarSelectedBackgroundColor = var;
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedBackgroundImage);
    
    if (m_pTabBar)
    {
        m_pTabBar->setSelectedBackgroundColor(m_sTabBarSelectedBackgroundColor);
    }
}

const CAColor4B& CATabBarController::getTabBarSelectedBackgroundColor()
{
    return m_sTabBarSelectedBackgroundColor;
}

void CATabBarController::setTabBarSelectedIndicatorImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedIndicatorImage);
    m_pTabBarSelectedIndicatorImage = var;
    m_sTabBarSelectedIndicatorColor = CAColor_white;
    
    if (m_pTabBar)
    {
        m_pTabBar->setSelectedIndicatorImage(m_pTabBarSelectedIndicatorImage);
    }
}

CAImage* CATabBarController::getTabBarSelectedIndicatorImage()
{
    return m_pTabBarSelectedIndicatorImage;
}

void CATabBarController::setTabBarSelectedIndicatorColor(const CAColor4B &var)
{
    m_sTabBarSelectedIndicatorColor = var;
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedIndicatorImage);
    
    if (m_pTabBar)
    {
        m_pTabBar->setSelectedIndicatorColor(m_sTabBarSelectedIndicatorColor);
    }
}

const CAColor4B& CATabBarController::getTabBarSelectedIndicatorColor()
{
    return m_sTabBarSelectedIndicatorColor;
}

void CATabBarController::setTabBarTitleColorForNormal(const CAColor4B &var)
{
    m_sTabBarTitleColor = var;
    
    if (m_pTabBar)
    {
        m_pTabBar->setTitleColorForNormal(m_sTabBarTitleColor);
    }
}

const CAColor4B& CATabBarController::getTabBarTitleColorForNormal()
{
    return m_sTabBarTitleColor;
}

void CATabBarController::setTabBarTitleColorForSelected(const CAColor4B &var)
{
    m_sTabBarSelectedTitleColor = var;
    
    if (m_pTabBar)
    {
        m_pTabBar->setTitleColorForSelected(m_sTabBarSelectedTitleColor);
    }
}

const CAColor4B& CATabBarController::getTabBarTitleColorForSelected()
{
    return m_sTabBarSelectedTitleColor;
}


void CATabBarController::showTabBarSelectedIndicator()
{
    m_bShowTabBarSelectedIndicator = true;
    if (m_pTabBar)
    {
        m_pTabBar->showSelectedIndicator();
    }
}

bool CATabBarController::initWithViewControllers(const CAVector<CAViewController*>& viewControllers, CABarVerticalAlignment var)
{
    CAViewController::init();
    
    m_pViewControllers = viewControllers;
    m_eTabBarVerticalAlignment = var;
    
    return true;
    
}

void CATabBarController::updateItem(CAViewController* viewController)
{
    size_t index = 0;
    while (index < m_pViewControllers.size())
    {
        CC_BREAK_IF(viewController->isEqual(m_pViewControllers.at(index)));
        index++;
    };
    
    m_pTabBar->replaceItemAtIndex(index, viewController->getTabBarItem());
}


void CATabBarController::viewDidLoad()
{
    std::vector<CATabBarItem*> items;
    
    for (unsigned int i=0; i<m_pViewControllers.size(); i++)
    {
        CAViewController* view = m_pViewControllers.at(i);
        if (view->getTabBarItem() == NULL)
        {
            char title[8];
            sprintf(title, "item%d", i);
            CATabBarItem* item = CATabBarItem::create(title, NULL);
            item->setTag(i);
            view->setTabBarItem(item);
        }
        items.push_back(view->getTabBarItem());
        view->m_pTabBarController = this;
    }
    
    bool clearance = false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (atof(CADevice::getSystemVersionWithIOS()) >= 7.0f
        && m_iTabBarHeight == 0
        && m_eTabBarVerticalAlignment == CABarVerticalAlignmentTop)
    {
        clearance = true;
    }
#endif

    DLayout tabBarLayout;
    tabBarLayout.horizontal = DHorizontalLayoutFill;
    
    if (m_iTabBarHeight == 0)
    {
        if (m_eTabBarVerticalAlignment == CABarVerticalAlignmentTop)
        {
            m_iTabBarHeight = clearance ? 138 : 98;
        }
        else
        {
            m_iTabBarHeight = 98;
        }
    }
    
    if (m_eTabBarVerticalAlignment == CABarVerticalAlignmentTop)
    {
        tabBarLayout.vertical.top = m_bTabBarHidden ? -m_iTabBarHeight : 0;
        tabBarLayout.vertical.height = m_iTabBarHeight;
    }
    else
    {
        tabBarLayout.vertical.bottom = m_bTabBarHidden ? -m_iTabBarHeight : 0;
        tabBarLayout.vertical.height = m_iTabBarHeight;
    }
    
    m_pTabBar = CATabBar::createWithLayout(tabBarLayout, clearance);
    m_pTabBar->setItems(items);
    m_pTabBar->setDelegate(this);
    this->getView()->addSubview(m_pTabBar);
    
    DLayout containerLayout;
    containerLayout.horizontal = DHorizontalLayoutFill;
    
    if (m_eTabBarVerticalAlignment == CABarVerticalAlignmentTop)
    {
        containerLayout.vertical = DVerticalLayout_T_B(tabBarLayout.vertical.top + tabBarLayout.vertical.height, 0);
    }
    else
    {
        containerLayout.vertical = DVerticalLayout_T_B(0, tabBarLayout.vertical.bottom + tabBarLayout.vertical.height);
    }
    
    m_pContainer = CAPageView::createWithLayout(containerLayout, CAPageViewDirectionHorizontal);
    m_pContainer->setBackgroundColor(CAColor_clear);
    m_pContainer->setPageViewDelegate(this);
    m_pContainer->setScrollViewDelegate(this);
    m_pContainer->setScrollEnabled(m_bScrollEnabled);
    m_pContainer->setDisplayRange(true);
    this->getView()->addSubview(m_pContainer);
    
    CAVector<CAView*> views;
    for (int i=0; i<m_pViewControllers.size(); i++)
    {
        CAView* view = new CAView();
        views.pushBack(view);
        view->release();
    }
    m_pContainer->setViews(views);
    
    if (m_pTabBarBackgroundImage)
    {
        m_pTabBar->setBackgroundImage(m_pTabBarBackgroundImage);
    }
    else
    {
        m_pTabBar->setBackgroundColor(m_sTabBarBackgroundColor);
    }
    
    
    if (m_pTabBarSelectedBackgroundImage)
    {
        m_pTabBar->setSelectedBackgroundImage(m_pTabBarSelectedBackgroundImage);
    }
    else
    {
        m_pTabBar->setSelectedBackgroundColor(m_sTabBarSelectedBackgroundColor);
    }
    
    
    if (m_pTabBarSelectedIndicatorImage)
    {
        m_pTabBar->setSelectedIndicatorImage(m_pTabBarSelectedIndicatorImage);
    }
    else
    {
        m_pTabBar->setSelectedIndicatorColor(m_sTabBarSelectedIndicatorColor);
    }
    
    
    m_pTabBar->setTitleColorForNormal(m_sTabBarTitleColor);
    m_pTabBar->setTitleColorForSelected(m_sTabBarSelectedTitleColor);
    
    if (m_bShowTabBarSelectedIndicator)
    {
        m_pTabBar->showSelectedIndicator();
    }

    m_pTabBar->setSelectedAtIndex(m_nSelectedIndex);
    this->renderingSelectedViewController();
}

void CATabBarController::viewDidUnload()
{
    this->getView()->removeAllSubviews();
}

void CATabBarController::viewDidAppear()
{
    m_pViewControllers.at(m_nSelectedIndex)->viewDidAppear();
}

void CATabBarController::viewDidDisappear()
{
    m_pViewControllers.at(m_nSelectedIndex)->viewDidDisappear();
}

bool CATabBarController::showSelectedViewController(CAViewController* viewController)
{
    do
    {
        CC_BREAK_IF(m_pViewControllers.at(m_nSelectedIndex)->isEqual(viewController));
        
        unsigned int index = 0;
        while (index < m_pViewControllers.size())
        {
            CC_BREAK_IF(m_pViewControllers.at(index)->isEqual(viewController));
            index++;
        }
        
        return this->showSelectedViewControllerAtIndex(index);
    }
    while (0);
    
    return false;
}

CAViewController* CATabBarController::getSelectedViewController()
{
    return m_pViewControllers.at(m_nSelectedIndex);
}

CAViewController* CATabBarController::getViewControllerAtIndex(unsigned int index)
{
    if (index >= m_pViewControllers.size())
    {
        return NULL;
    }
    
    return m_pViewControllers.at(index);
}

bool CATabBarController::showSelectedViewControllerAtIndex(unsigned int index)
{
    do
    {
        CC_BREAK_IF(index >= m_pViewControllers.size());
        CC_BREAK_IF(index == m_nSelectedIndex);

        m_pContainer->setCurrPage(index, false);
        if (m_pTabBar->getSelectedIndex() != index)
        {
            m_pTabBar->setSelectedAtIndex(index);
        }
        m_nLastSelectedIndex = m_nSelectedIndex;
        m_nSelectedIndex = index;
        
        this->renderingSelectedViewController();
        
        return true;
    }
    while (0);
    
    return false;
}

unsigned int CATabBarController::getSelectedViewControllerAtIndex()
{
    return m_nSelectedIndex;
}

void CATabBarController::tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index)
{
    CC_RETURN_IF(m_nSelectedIndex == index);
    this->showSelectedViewControllerAtIndex(index);
}

void CATabBarController::pageViewDidEndTurning(CAPageView* pageView)
{
    m_pTabBar->setTouchEnabled(true);
    
    for (int i = MAX((int)m_nSelectedIndex - 1, 0);
         i < MIN((int)m_nSelectedIndex + 2, m_pViewControllers.size());
         i++)
    {
        CC_CONTINUE_IF(i == pageView->getCurrPage());
        m_pViewControllers.at(i)->getView()->setVisible(false);
    }
    CC_RETURN_IF(m_nSelectedIndex == pageView->getCurrPage());
    this->showSelectedViewControllerAtIndex(pageView->getCurrPage());
}

void CATabBarController::scrollViewWillBeginDragging(CAScrollView* view)
{
    m_pTabBar->setTouchEnabled(false);
    
    for (int i = MAX((int)m_nSelectedIndex - 1, 0);
         i < MIN((int)m_nSelectedIndex + 2, m_pViewControllers.size());
         i++)
    {
        if (!m_pViewControllers.at(i)->getView()->getSuperview())
        {
            CAView* view = m_pContainer->getSubViewAtIndex(i);
            m_pViewControllers.at(i)->addViewFromSuperview(view);
        }
        
        m_pViewControllers.at(i)->getView()->setVisible(true);
    }
}

void CATabBarController::renderingSelectedViewController()
{
    if (m_nLastSelectedIndex < m_pViewControllers.size())
    {
        m_pViewControllers.at(m_nLastSelectedIndex)->getView()->setVisible(false);
        m_pViewControllers.at(m_nLastSelectedIndex)->viewDidDisappear();
    }
    
    if (!m_pViewControllers.at(m_nSelectedIndex)->getView()->getSuperview())
    {
        CAView* view = m_pContainer->getSubViewAtIndex(m_nSelectedIndex);
        m_pViewControllers.at(m_nSelectedIndex)->addViewFromSuperview(view);
    }
    
    m_pViewControllers.at(m_nSelectedIndex)->getView()->setVisible(true);
    m_pViewControllers.at(m_nSelectedIndex)->viewDidAppear();
}


int CATabBarController::getTabBarNowY()
{
    int y = 0;

    if (m_bTabBarHidden)
    {
        y = m_iTabBarHeight * m_fProgress;
    }
    else
    {
        y = m_iTabBarHeight * (1 -m_fProgress);
    }
    return y;
}

void CATabBarController::setTabBarHidden(bool hidden, bool animated)
{
    CC_RETURN_IF(m_bTabBarHidden == hidden);
    m_bTabBarHidden = hidden;
    CC_RETURN_IF(this->getView()->getSuperview() == NULL);
    
    if (animated)
    {
        CAAnimation::schedule(CAAnimation_selector(CATabBarController::tabBarHiddenAnimation), this, 0.25f);
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.3f);
        CAViewAnimation::setAnimationWillStartSelector(CAApplication::getApplication()->getTouchDispatcher(), CAViewAnimation0_selector(CATouchDispatcher::setDispatchEventsFalse));
        CAViewAnimation::setAnimationDidStopSelector(CAApplication::getApplication()->getTouchDispatcher(), CAViewAnimation0_selector(CATouchDispatcher::setDispatchEventsTrue));
        CAViewAnimation::commitAnimations();
    }
    else
    {
        m_fProgress = 1.0f;

        if (this->getView()->getSuperview())
        {
            this->update(0);
        }
    }
    
}

void CATabBarController::tabBarHiddenAnimation(float delay, float now, float total)
{
    m_fProgress = now / total;

    if (this->getView()->getSuperview())
    {
        this->update(0);
    }
}

void CATabBarController::update(float dt)
{
    int y = this->getTabBarNowY();
    
    DLayout tabBarLayout = m_pTabBar->getLayout();
    
    switch (m_eTabBarVerticalAlignment)
    {
        case CABarVerticalAlignmentTop:
        {
            tabBarLayout.vertical.top = y;
        }
            break;
        case CABarVerticalAlignmentBottom:
        {
            tabBarLayout.vertical.bottom = y;
        }
            break;
        default:
            break;
    }
    
    m_pTabBar->setLayout(tabBarLayout);
}

NS_CC_END;
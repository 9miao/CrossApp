//
//  CAViewController.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-11.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAViewController.h"
#include "basics/CAApplication.h"
#include "support/CCPointExtension.h"
#include "basics/CAScheduler.h"
#include "view/CAWindow.h"
#include "view/CAScale9ImageView.h"
#include "dispatcher/CATouchDispatcher.h"
#include "animation/CAViewAnimation.h"
#include "animation/CAAnimation.h"

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
{
    m_pView = CAView::createWithColor(CAColor_white);
    m_pView->retain();
    m_pView->setViewDelegate(this);
}

CAViewController::~CAViewController()
{
    m_pView->setViewDelegate(NULL);
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

void CAViewController::getSuperViewRect(const CCRect& rect)
{
    m_pView->setFrame(rect);
    if (m_bLifeLock)
    {
        this->reshapeViewRectDidFinish();
    }
}


void CAViewController::viewOnEnterTransitionDidFinish()
{
    CAScheduler::getScheduler()->resumeTarget(this);
    
    do
    {
        CC_BREAK_IF(m_bLifeLock);
        m_bLifeLock = true;
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

const char* CAViewController::getNibName()
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
        
        CAApplication* pDirector = CAApplication::getApplication();
        if (enabled)
        {
            pDirector->getKeypadDispatcher()->addDelegate(this);
        }
        else
        {
            pDirector->getKeypadDispatcher()->removeDelegate(this);
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

    return false;
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
,m_pNavigationBarBackGroundImage(NULL)
,m_sNavigationBarBackGroundColor(CAColor_white)
,m_sNavigationBarTitleColor(CAColor_white)
,m_sNavigationBarButtonColor(CAColor_white)
,m_fProgress(1.0f)
{
    m_pView->setColor(CAColor_clear);
    m_pView->setDisplayRange(false);
    
    this->setTouchMoved(true);

    CCSize winSize = CAApplication::getApplication()->getWinSize();
    m_tNavigationBarSize = CCSize(winSize.width, _px(88));
    
    this->setNavigationBarBackGroundImage(CAImage::create("source_material/navigation_bg.png"));
}

CANavigationController::~CANavigationController()
{
    m_pViewControllers.clear();
    m_pNavigationBars.clear();
    m_pContainers.clear();
    m_pSecondContainers.clear();
    CC_SAFE_RELEASE_NULL(m_pNavigationBarBackGroundImage);
}

void CANavigationController::setNavigationBarBackGroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pNavigationBarBackGroundImage);
    m_pNavigationBarBackGroundImage = var;
    m_sNavigationBarBackGroundColor = CAColor_white;
    
    if (!m_pNavigationBars.empty())
    {
        CADeque<CANavigationBar*>::iterator itr;
        for (itr=m_pNavigationBars.begin(); itr!=m_pNavigationBars.end(); itr++)
        {
            if (m_pNavigationBarBackGroundImage)
            {
                (*itr)->setBackGroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackGroundImage));
            }
            else
            {
                (*itr)->setBackGroundView(CAView::create());
            }
            (*itr)->getBackGroundView()->setColor(m_sNavigationBarBackGroundColor);
        }
    }
}

CAImage* CANavigationController::getNavigationBarBackGroundImage()
{
    return m_pNavigationBarBackGroundImage;
}

void CANavigationController::setNavigationBarBackGroundColor(const CAColor4B &var)
{
    m_sNavigationBarBackGroundColor = var;
    CC_SAFE_RELEASE_NULL(m_pNavigationBarBackGroundImage);
    
    if (!m_pNavigationBars.empty())
    {
        CADeque<CANavigationBar*>::iterator itr;
        for (itr=m_pNavigationBars.begin(); itr!=m_pNavigationBars.end(); itr++)
        {
            if (m_pNavigationBarBackGroundImage)
            {
                (*itr)->setBackGroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackGroundImage));
            }
            else
            {
                (*itr)->setBackGroundView(CAView::create());
            }
            (*itr)->getBackGroundView()->setColor(m_sNavigationBarBackGroundColor);
        }
    }
}

const CAColor4B& CANavigationController::getNavigationBarBackGroundColor()
{
    return m_sNavigationBarBackGroundColor;
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

bool CANavigationController::initWithRootViewController(CAViewController* viewController, CABarVerticalAlignment var)
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
    
    m_eNavigationBarVerticalAlignment = var;
    
    this->createWithContainer(viewController);
    
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
    
    if (m_pNavigationBarBackGroundImage)
    {
        m_pNavigationBars.at(index)->setBackGroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackGroundImage));
    }
    else
    {
        m_pNavigationBars.at(index)->setBackGroundView(CAView::create());
        m_pNavigationBars.at(index)->getBackGroundView()->setColor(m_sNavigationBarBackGroundColor);
    }

    m_pNavigationBars.at(index)->setTitleColor(m_sNavigationBarTitleColor);
    m_pNavigationBars.at(index)->setTitleColor(m_sNavigationBarTitleColor);
    m_pNavigationBars.at(index)->setItem(viewController->getNavigationBarItem());
}

void CANavigationController::viewDidLoad()
{
    this->layoutNewContainer();
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

void CANavigationController::reshapeViewRectDidFinish()
{
    this->update(0);
}

void CANavigationController::createWithContainer(CAViewController* viewController)
{
    CAView* container = new CAView();
    container->setFrame(this->getView()->getBounds());
    this->getView()->addSubview(container);
    m_pContainers.pushBack(container);
    container->release();
    
    CANavigationBar* navigationBar = CANavigationBar::create();
    if (m_pNavigationBarBackGroundImage)
    {
        navigationBar->setBackGroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackGroundImage));
    }
    else
    {
        navigationBar->setBackGroundView(CAView::create());
    }
    navigationBar->getBackGroundView()->setColor(m_sNavigationBarBackGroundColor);
    navigationBar->setTitleColor(m_sNavigationBarTitleColor);
    navigationBar->setTitleColor(m_sNavigationBarTitleColor);
    container->insertSubview(navigationBar, 1);
    navigationBar->setDelegate(this);
    m_pNavigationBars.pushBack(navigationBar);
    
    if (viewController->getNavigationBarItem() == NULL && viewController->getTitle().compare("") != 0)
    {
        viewController->setNavigationBarItem(CANavigationBarItem::create(viewController->getTitle()));
    }
    if (m_pViewControllers.empty())
    {
        viewController->getNavigationBarItem()->setShowGoBackButton(false);
    }
    navigationBar->setItem(viewController->getNavigationBarItem());
    
    CAView* secondContainer = new CAView();
    container->addSubview(secondContainer);
    secondContainer->release();
    m_pSecondContainers.pushBack(secondContainer);
    
    viewController->m_pNavigationController = this;
    m_pViewControllers.pushBack(viewController);
}

void CANavigationController::layoutNewContainer()
{
    CAViewController* viewController = m_pViewControllers.back();
    
    CCRect navigation_bar_rect = CCRectZero;
    navigation_bar_rect.size = m_tNavigationBarSize;
    
    CCRect container_rect = this->getView()->getBounds();
    
    if (m_bNavigationBarHidden || viewController->getNavigationBarItem()->isNagigationBarHidden())
    {
        navigation_bar_rect.origin = this->getNavigationBarTakeBackPoint();
    }
    else
    {
        container_rect.size.height -= m_tNavigationBarSize.height;
        navigation_bar_rect.origin = this->getNavigationBarOpenPoint();
        
        if (m_eNavigationBarVerticalAlignment == CABarVerticalAlignmentTop)
        {
            container_rect.origin.y = m_tNavigationBarSize.height;
        }
    }
    
    CAView* container = m_pContainers.back();
    container->setFrame(this->getView()->getBounds());
    
    CANavigationBar* navigationBar = m_pNavigationBars.back();
    navigationBar->setFrame(navigation_bar_rect);

    CAView* secondContainer = m_pSecondContainers.back();
    secondContainer->setFrame(container_rect);
    
    
    viewController->addViewFromSuperview(secondContainer);
    viewController->viewDidAppear();
}

void CANavigationController::replaceViewController(CrossApp::CAViewController *viewController, bool animated)
{
    if (this->getView()->getSuperview() == NULL)
    {
        return;
    }
    
    float x = this->getView()->getBounds().size.width;
    
    CAView* lastContainer = m_pContainers.back();
    this->createWithContainer(viewController);
    this->layoutNewContainer();
    if (m_pNavigationBars.size() == 2)
    {
        CANavigationBarItem* item = viewController->getNavigationBarItem();
        item->setShowGoBackButton(false);
        viewController->setNavigationBarItem(item);
    }
    CAView* newContainer = m_pContainers.back();
    newContainer->setFrameOrigin(CCPoint(x, 0));
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/30.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveLinear);
        lastContainer->setFrameOrigin(CCPoint(-x/2.0f, 0));
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/60.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::replaceViewControllerFinish));
        newContainer->setFrameOrigin(CCPointZero);
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
    newContainer->setFrameOrigin(CCPointZero);
    
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
    this->createWithContainer(viewController);
    this->layoutNewContainer();
    CAView* newContainer = m_pContainers.back();
    
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        newContainer->setFrameOrigin(CCPoint(x, 0));
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/30.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        lastContainer->setFrameOrigin(CCPoint(-x/2.0f, 0));
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/60.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::pushViewControllerFinish));
        newContainer->setFrameOrigin(CCPointZero);
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
    newContainer->setFrameOrigin(CCPointZero);
    
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
        backContainer->setFrameOrigin(CCPointZero);
    }
    
    size_t index = m_pViewControllers.size() - 2;
    CAViewController* showViewController = m_pViewControllers.at(index);
    showViewController->viewDidAppear();
    
    CAView* showContainer = m_pContainers.at(index);
    showContainer->setVisible(true);
    
    
    {
        CCRect rect = this->getView()->getBounds();
        
        CCPoint point = this->getNavigationBarNowPoint(showViewController);
        
        switch (m_eNavigationBarVerticalAlignment)
        {
            case CABarVerticalAlignmentTop:
            {
                rect.origin.y = point.y + m_tNavigationBarSize.height;
                rect.size.height = rect.size.height - rect.origin.y;
            }
                break;
            case CABarVerticalAlignmentBottom:
            {
                rect.size.height = point.y;
            }
                break;
            default:
                break;
        }
        m_pNavigationBars.at(index)->setFrameOrigin(point);
        
        CAView* secondContainer = m_pSecondContainers.at(index);
        secondContainer->setFrame(rect);
        
        showViewController->getSuperViewRect(secondContainer->getBounds());
    }
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        showContainer->setFrameOrigin(CCPoint(-x/2.0f, 0));
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/30.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        showContainer->setFrameOrigin(CCPointZero);
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(1/60.0f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::popViewControllerFinish));
        backContainer->setFrameOrigin(CCPoint(x, 0));
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
    
    m_pContainers.back()->setFrameOrigin(CCPointZero);
    
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
    backContainer->setFrameOrigin(CCPointZero);
    
    size_t index = 0;
    CAViewController* showViewController = m_pViewControllers.at(index);
    showViewController->viewDidAppear();
    
    CAView* showContainer = m_pContainers.at(index);
    showContainer->setVisible(true);
    showContainer->setFrameOrigin(CCPoint(-x/2.0f, 0));
    
    {
        CCRect rect = this->getView()->getBounds();
        
        CCPoint point = this->getNavigationBarNowPoint(showViewController);
        
        switch (m_eNavigationBarVerticalAlignment)
        {
            case CABarVerticalAlignmentTop:
            {
                rect.origin.y = point.y + m_tNavigationBarSize.height;
                rect.size.height = rect.size.height - rect.origin.y;
            }
                break;
            case CABarVerticalAlignmentBottom:
            {
                rect.size.height = point.y;
            }
                break;
            default:
                break;
        }
        m_pNavigationBars.at(index)->setFrameOrigin(point);
        
        CAView* secondContainer = m_pSecondContainers.at(index);
        secondContainer->setFrame(rect);
        
        showViewController->getSuperViewRect(secondContainer->getBounds());
    }
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        showContainer->setFrameOrigin(CCPointZero);
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::popToRootViewControllerFinish));
        backContainer->setFrameOrigin(CCPoint(x, 0));
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
    
    m_pViewControllers.back()->getView()->setFrameOrigin(CCPointZero);
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

void CANavigationController::homingViewControllerFinish()
{
    size_t index = m_pViewControllers.size() - 2;
    CAViewController* lastViewController = m_pViewControllers.at(index);

    CAView* lastContainer = m_pContainers.at(index);
    lastContainer->setVisible(false);
    
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
        CCPoint point = this->getNavigationBarNowPoint(m_pViewControllers.back());
        m_pNavigationBars.back()->setFrameOrigin(point);
        if (this->getView()->getSuperview())
        {
            this->update(0);
        }
    }
}

CCPoint CANavigationController::getNavigationBarOpenPoint()
{
    CCPoint p = CCPointZero;
    float height = this->getView()->getBounds().size.height;
    
    if (m_eNavigationBarVerticalAlignment == CABarVerticalAlignmentTop)
    {
        p.y = 0;
    }
    else
    {
        p.y = height - m_tNavigationBarSize.height;
    }
    return p;
}

CCPoint CANavigationController::getNavigationBarTakeBackPoint()
{
    CCPoint p = CCPointZero;
    float height = this->getView()->getBounds().size.height;
    
    if (m_eNavigationBarVerticalAlignment == CABarVerticalAlignmentTop)
    {
        p.y = -m_tNavigationBarSize.height;
    }
    else
    {
        p.y = height;
    }
    return p;
}

CCPoint CANavigationController::getNavigationBarNowPoint(CAViewController* viewController)
{
    float offsetY = this->getNavigationBarTakeBackPoint().y - this->getNavigationBarOpenPoint().y;
    CCPoint p = CCPointZero;
    
    if (viewController->getNavigationBarItem() && viewController->getNavigationBarItem()->isNagigationBarHidden())
    {
        p.y = this->getNavigationBarOpenPoint().y + offsetY;
    }
    else
    {
        if (m_bNavigationBarHidden)
        {
            p.y = this->getNavigationBarOpenPoint().y + offsetY * m_fProgress;
        }
        else
        {
            p.y = this->getNavigationBarTakeBackPoint().y - offsetY * m_fProgress;
        }
    }
    return p;
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
    
    CCRect rect = this->getView()->getBounds();
    
    CCPoint point = this->getNavigationBarNowPoint(viewController);

    switch (m_eNavigationBarVerticalAlignment)
    {
        case CABarVerticalAlignmentTop:
        {
            rect.origin.y = point.y + m_tNavigationBarSize.height;
            rect.size.height = rect.size.height - rect.origin.y;
        }
            break;
        case CABarVerticalAlignmentBottom:
        {
            rect.size.height = point.y;
        }
            break;
        default:
            break;
    }
    m_pNavigationBars.back()->setFrameOrigin(point);
    
    CAView* secondContainer = m_pSecondContainers.back();
    secondContainer->setFrame(rect);
    
    viewController->getSuperViewRect(secondContainer->getBounds());
}

bool CANavigationController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    if (pTouch->getLocation().x > _px(96))
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
    
    CAView* backContainer = m_pContainers.back();
    
    CCPoint point1 = backContainer->getFrameOrigin();
    point1.x += offDis;
    point1.x = MAX(point1.x, 0);
    point1.x = MIN(point1.x, this->getView()->getBounds().size.width);
    backContainer->setFrameOrigin(point1);
    
    CCPoint point2 = showContainer->getCenterOrigin();
    point2.x = point1.x/2;
    showContainer->setCenterOrigin(point2);
    
    m_bPopViewController = ((offDis > _px(10)) || point1.x > this->getView()->getBounds().size.width/4);
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
    
    CAView* backContainer = m_pContainers.back();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (m_bPopViewController)
    {
        lastViewController->viewDidAppear();
        
        CAViewAnimation::beginAnimations("navigation_animation", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        lastContainer->setFrameOrigin(CCPointZero);
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("navigation_animation2", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::popViewControllerFinish));
        backContainer->setFrameOrigin(CCPoint(x, 0));
        CAViewAnimation::commitAnimations();
    }
    else
    {
        CAViewAnimation::beginAnimations("navigation_animation", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        lastContainer->setFrameOrigin(CCPoint(-x/2.0f, 0));
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("navigation_animation2", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CANavigationController::homingViewControllerFinish));
        backContainer->setFrameOrigin(CCPointZero);
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
,m_nLastSelectedIndex(0xffffffff)
,m_pTabBar(NULL)
,m_pContainer(NULL)
,m_bTabBarHidden(false)
,m_bscrollEnabled(false)
,m_pTabBarBackGroundImage(NULL)
,m_pTabBarSelectedBackGroundImage(NULL)
,m_pTabBarSelectedIndicatorImage(NULL)
,m_sTabBarBackGroundColor(CAColor_white)
,m_sTabBarSelectedBackGroundColor(CAColor_white)
,m_sTabBarSelectedIndicatorColor(CAColor_white)
,m_sTabBarTitleColor(CAColor_white)
,m_sTabBarSelectedTitleColor(ccc4(50, 193, 255, 255))
,m_bShowTabBarSelectedIndicator(false)
,m_fProgress(1.0f)
{
    m_pView->setColor(CAColor_clear);
    m_pView->setDisplayRange(false);
    
    this->setTabBarBackGroundImage(CAImage::create("source_material/tabBar_bg.png"));
    this->setTabBarSelectedBackGroundImage(CAImage::create("source_material/tabBar_selected_bg.png"));
    this->setTabBarSelectedIndicatorImage(CAImage::create("source_material/tabBar_selected_indicator.png"));
}

CATabBarController::~CATabBarController()
{
    m_pViewControllers.clear();
    CC_SAFE_RELEASE_NULL(m_pTabBar);
    CC_SAFE_RELEASE_NULL(m_pTabBarBackGroundImage);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedBackGroundImage);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedIndicatorImage);
}

void CATabBarController::setTabBarBackGroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pTabBarBackGroundImage);
    m_pTabBarBackGroundImage = var;
    m_sTabBarBackGroundColor = CAColor_white;
    
    if (m_pTabBar)
    {
        m_pTabBar->setBackGroundImage(m_pTabBarBackGroundImage);
    }
}

CAImage* CATabBarController::getTabBarBackGroundImage()
{
    return m_pTabBarBackGroundImage;
}

void CATabBarController::setTabBarBackGroundColor(const CAColor4B &var)
{
    m_sTabBarBackGroundColor = var;
    CC_SAFE_RELEASE_NULL(m_pTabBarBackGroundImage);
    
    if (m_pTabBar)
    {
        m_pTabBar->setBackGroundColor(m_sTabBarBackGroundColor);
    }
}

const CAColor4B& CATabBarController::getTabBarBackGroundColor()
{
    return m_sTabBarBackGroundColor;
}

void CATabBarController::setTabBarSelectedBackGroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedBackGroundImage);
    m_pTabBarSelectedBackGroundImage = var;
    m_sTabBarSelectedBackGroundColor = CAColor_white;
    
    if (m_pTabBar)
    {
        m_pTabBar->setSelectedBackGroundImage(m_pTabBarSelectedBackGroundImage);
    }
}

CAImage* CATabBarController::getTabBarSelectedBackGroundImage()
{
    return m_pTabBarSelectedBackGroundImage;
}

void CATabBarController::setTabBarSelectedBackGroundColor(const CAColor4B &var)
{
    m_sTabBarSelectedBackGroundColor = var;
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedBackGroundImage);
    
    if (m_pTabBar)
    {
        m_pTabBar->setSelectedBackGroundColor(m_sTabBarSelectedBackGroundColor);
    }
}

const CAColor4B& CATabBarController::getTabBarSelectedBackGroundColor()
{
    return m_sTabBarSelectedBackGroundColor;
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
    
    do
    {
        CC_BREAK_IF(viewControllers.size() == 0);
        m_pViewControllers = viewControllers;
        
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
        
        m_pTabBar = CATabBar::create(items);
        m_pTabBar->retain();
        m_pTabBar->setDelegate(this);
        
    }
    while (0);
    
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
    CCPoint tab_bar_rectOrgin = CCPointZero;
    
    CCRect container_rect = this->getView()->getBounds();
    
    CCSize container_view_size = container_rect.size;
    container_view_size.width *= m_pViewControllers.size();
    
    if (m_bTabBarHidden)
    {
        tab_bar_rectOrgin = this->getTabBarTakeBackPoint();
    }
    else
    {
        tab_bar_rectOrgin = this->getTabBarOpenPoint();
        
        container_rect.size.height -= m_pTabBar->getFrame().size.height;
        if (m_eTabBarVerticalAlignment == CABarVerticalAlignmentTop)
        {
            container_rect.origin.y = m_pTabBar->getFrame().size.height;
        }
    }
    
    m_pContainer = CAPageView::createWithFrame(container_rect, CAPageViewDirectionHorizontal);
    m_pContainer->setBackGroundColor(CAColor_clear);
    m_pContainer->setPageViewDelegate(this);
    m_pContainer->setScrollViewDelegate(this);
    m_pContainer->setScrollEnabled(m_bscrollEnabled);
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
    
    if (m_pTabBarBackGroundImage)
    {
        m_pTabBar->setBackGroundImage(m_pTabBarBackGroundImage);
    }
    else
    {
        m_pTabBar->setBackGroundColor(m_sTabBarBackGroundColor);
    }
    
    
    if (m_pTabBarSelectedBackGroundImage)
    {
        m_pTabBar->setSelectedBackGroundImage(m_pTabBarSelectedBackGroundImage);
    }
    else
    {
        m_pTabBar->setSelectedBackGroundColor(m_sTabBarSelectedBackGroundColor);
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
    
    m_pTabBar->setFrameOrigin(tab_bar_rectOrgin);
    this->getView()->addSubview(m_pTabBar);
    
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

void CATabBarController::reshapeViewRectDidFinish()
{
    this->update(0);
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
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
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
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    for (int i = MAX((int)m_nSelectedIndex - 1, 0);
         i < MIN((int)m_nSelectedIndex + 2, m_pViewControllers.size());
         i++)
    {
        if (!m_pViewControllers.at(i)->getView()->getSuperview())
        {
            CAView* view = m_pContainer->getSubViewAtIndex(i);
            m_pViewControllers.at(i)->getView()->setFrame(view->getBounds());
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
        m_pViewControllers.at(m_nSelectedIndex)->getView()->setFrame(view->getBounds());
        m_pViewControllers.at(m_nSelectedIndex)->addViewFromSuperview(view);
    }
    
    m_pViewControllers.at(m_nSelectedIndex)->getView()->setVisible(true);
    m_pViewControllers.at(m_nSelectedIndex)->viewDidAppear();
}

void CATabBarController::setScrollEnabled(bool var)
{
    m_bscrollEnabled = var;
    if (m_pContainer)
    {
        m_pContainer->setScrollEnabled(m_bscrollEnabled);
    }
}

bool CATabBarController::isScrollEnabled()
{
    return m_bscrollEnabled;
}

CCPoint CATabBarController::getTabBarOpenPoint()
{
    CCPoint p = CCPointZero;
    float height = this->getView()->getBounds().size.height;
    
    if (m_eTabBarVerticalAlignment == CABarVerticalAlignmentTop)
    {
        p.y = 0;
    }
    else
    {
        p.y = height - m_pTabBar->getFrame().size.height;
    }
    return p;
}

CCPoint CATabBarController::getTabBarTakeBackPoint()
{
    CCPoint p = CCPointZero;
    float height = this->getView()->getBounds().size.height;
    
    if (m_eTabBarVerticalAlignment == CABarVerticalAlignmentTop)
    {
        p.y = -m_pTabBar->getFrame().size.height;
    }
    else
    {
        p.y = height;
    }
    return p;
}

CCPoint CATabBarController::getTabBarNowPoint()
{
    float offsetY = this->getTabBarTakeBackPoint().y - this->getTabBarOpenPoint().y;
    CCPoint p = CCPointZero;
    
    if (m_bTabBarHidden)
    {
        p.y = this->getTabBarOpenPoint().y + offsetY * m_fProgress;
    }
    else
    {
        p.y = this->getTabBarTakeBackPoint().y - offsetY * m_fProgress;
    }
    return p;
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
        CCPoint point = this->getTabBarNowPoint();
        m_pTabBar->setFrameOrigin(point);
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
    CCRect rect = this->getView()->getFrame();
    
    CCPoint point = this->getTabBarNowPoint();
    
    switch (m_eTabBarVerticalAlignment)
    {
        case CABarVerticalAlignmentTop:
        {
            rect.origin.y = point.y + m_pTabBar->getFrame().size.height;
            rect.size.height = rect.size.height - rect.origin.y;
        }
            break;
        case CABarVerticalAlignmentBottom:
        {
            rect.size.height = point.y;
        }
            break;
        default:
            break;
    }
    
    m_pTabBar->setFrameOrigin(point);
    m_pContainer->setFrame(rect);
    
    for (size_t i=0; i<m_pViewControllers.size(); i++)
    {
        CCRect r = m_pContainer->getSubViewAtIndex((int)i)->getFrame();
        r.size = rect.size;
        m_pContainer->getSubViewAtIndex((int)i)->setFrame(r);
        CAViewController* viewController = m_pViewControllers.at(i);
        viewController->getSuperViewRect(m_pContainer->getBounds());
    }
}

NS_CC_END;
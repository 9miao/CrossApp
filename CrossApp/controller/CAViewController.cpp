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
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionEase.h"
#include "basics/CAScheduler.h"
#include "view/CAWindow.h"
#include "dispatcher/CATouchDispatcher.h"


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
    this->setHaveNextResponder(true);
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
    CAApplication* pDirector = CAApplication::getApplication();
    if (m_bKeypadEnabled)
    {
        pDirector->getKeypadDispatcher()->removeDelegate(this);
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
    if (item)
    {
        CC_SAFE_RETAIN(item);
        CC_SAFE_RELEASE(m_pNavigationBarItem);
        m_pNavigationBarItem = item;
        
        if (m_pNavigationController)
        {
            m_pNavigationController->updateItem(this);
        }
    }
}

void CAViewController::setTabBarItem(CATabBarItem* item)
{
    if (item)
    {
        CC_SAFE_RETAIN(item);
        CC_SAFE_RELEASE(m_pTabBarItem);
        m_pTabBarItem = item;
        
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
:m_pViewControllers(NULL)
,m_pNavigationBar(NULL)
,m_pContainer(NULL)
,m_bNavigationBarHidden(false)
{
    m_pView->setColor(CAColor_clear);
    m_pView->setDisplayRange(false);
    
    m_pNavigationBar = CANavigationBar::create();
    m_pNavigationBar->retain();
    m_pNavigationBar->setAnchorPoint(CCPointZero);
}

CANavigationController::~CANavigationController()
{
    m_pViewControllers.clear();
    CC_SAFE_RELEASE_NULL(m_pNavigationBar);
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
    
    viewController->m_pNavigationController = this;
    m_pViewControllers.pushBack(viewController);
    
    CANavigationBarItem* item = viewController->getNavigationBarItem();
    if (item == NULL)
    {
        viewController->setNavigationBarItem(CANavigationBarItem::create(viewController->getTitle()));
    }
    viewController->getNavigationBarItem()->setShowGoBackButton(false);
    m_pNavigationBar->pushItem(viewController->getNavigationBarItem());
    
    m_eNavigationBarVerticalAlignment = var;
    
    return true;
}

void CANavigationController::updateItem(CAViewController* viewController)
{
    size_t index = 0;
    while (index < m_pViewControllers.size())
    {
        CC_BREAK_IF(viewController->isEqual(m_pViewControllers.at(index)));
        index++;
    };
    
    m_pNavigationBar->replaceItemAtIndex(index, viewController->getNavigationBarItem());
}

void CANavigationController::viewDidLoad()
{
    CCRect navigation_bar_rect = CCRectZero;
    
    CCRect container_rect = this->getView()->getBounds();
    
    if (m_bNavigationBarHidden)
    {
        switch (m_eNavigationBarVerticalAlignment)
        {
            case CABarVerticalAlignmentTop:
            {
                navigation_bar_rect.origin.y = -m_pNavigationBar->getFrame().size.height;
            }
                break;
            case CABarVerticalAlignmentBottom:
            {
                navigation_bar_rect.origin.y = container_rect.size.height;
            }
                break;
            default:
                break;
        }
    }
    else
    {
        container_rect.size.height -= m_pNavigationBar->getFrame().size.height;
        switch (m_eNavigationBarVerticalAlignment)
        {
            case CABarVerticalAlignmentTop:
            {
                container_rect.origin.y = m_pNavigationBar->getFrame().size.height;
            }
                break;
            case CABarVerticalAlignmentBottom:
            {
                navigation_bar_rect.origin.y = container_rect.size.height;
            }
                break;
            default:
                break;
        }
    }
    
    m_pContainer = new CAView();
    m_pContainer->setFrame(container_rect);
    this->getView()->addSubview(m_pContainer);
    m_pContainer->release();
    
    CAViewController* viewController = m_pViewControllers.front();
    viewController->addViewFromSuperview(m_pContainer);
    
    m_pNavigationBar->setFrame(navigation_bar_rect);
    this->getView()->addSubview(m_pNavigationBar);
    m_pNavigationBar->setDelegate(this);
}


void CANavigationController::viewDidUnload()
{
    std::vector<CAViewController*>::iterator itr;
    for (itr=m_pViewControllers.begin(); itr!=m_pViewControllers.end(); itr++)
    {
        (*itr)->removeViewFromSuperview();
    }
    m_pContainer = NULL;
    m_pNavigationBar->removeFromSuperview();
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

void CANavigationController::replaceViewController(CrossApp::CAViewController *viewController, bool animated)
{
    if (this->getView()->getSuperview() == NULL)
    {
        return;
    }
    
    if (m_pContainer->getActionByTag(0))
    {
        return;
    }
    
    float x = m_pContainer->getFrame().size.width;
    
    CAViewController* lastViewController = m_pViewControllers.back();
    viewController->m_pNavigationController = this;
    m_pViewControllers.insert(m_pViewControllers.size()-1, viewController);
    viewController->addViewFromSuperview(m_pContainer);
    viewController->viewDidDisappear();
    viewController->getView()->setFrameOrigin(CCPoint(x, 0));
    
    if (animated)
    {
        CCSequence* actionsLast =
        CCSequence::create(
                           CCDelayTime::create(0.03f),
                           
                           CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPoint(-x/2.0f, 0))),
                           
                           NULL);
        lastViewController->getView()->stopAllActions();
        lastViewController->getView()->runAction(actionsLast);
        
        CCSequence* actions =
        CCSequence::create(
                           CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(), callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)),
                           
                           CCDelayTime::create(0.02f),
                           
                           CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPointZero)),
                           
                           CCCallFunc::create(this, callfunc_selector(CANavigationController::replaceViewControllerFinish)),
                           
                           CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)),
                           
                           NULL);
        viewController->getView()->stopAllActions();
        viewController->getView()->runAction(actions);
    }
    else
    {
        this->replaceViewControllerFinish();
    }
    
}

void CANavigationController::replaceViewControllerFinish()
{
    CAViewController* lastViewController = m_pViewControllers.back();
    lastViewController->retain()->autorelease();
    m_pViewControllers.popBack();
    m_pNavigationBar->popItem();
    lastViewController->viewDidDisappear();
    lastViewController->removeViewFromSuperview();
    
    CAViewController* viewController = m_pViewControllers.back();
    viewController->getView()->setFrameOrigin(CCPointZero);
    if (viewController->getNavigationBarItem() == NULL && viewController->getTitle().compare("") != 0)
    {
        viewController->setNavigationBarItem(CANavigationBarItem::create(viewController->getTitle()));
    }
    m_pNavigationBar->pushItem(viewController->getNavigationBarItem());
}

void CANavigationController::pushViewController(CAViewController* viewController, bool animated)
{
    if (this->getView()->getSuperview() == NULL)
    {
        return;
    }
    
    if (m_pContainer->getActionByTag(0))
    {
        return;
    }
    
    float x = m_pContainer->getFrame().size.width;
    
    CAViewController* lastViewController = m_pViewControllers.back();
    viewController->m_pNavigationController = this;
    m_pViewControllers.pushBack(viewController);
    viewController->addViewFromSuperview(m_pContainer);
    viewController->viewDidAppear();
    viewController->getView()->setFrameOrigin(CCPoint(x, 0));
    
    if (animated)
    {
        CCSequence* actionsLast =
        CCSequence::create(
                           CCDelayTime::create(0.03f),
                           
                           CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPoint(-x/2.0f, 0))),
                           
                           NULL);
        lastViewController->getView()->stopAllActions();
        lastViewController->getView()->runAction(actionsLast);
        
        
        CCSequence* actions =
        CCSequence::create(
                           CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(), callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)),
                           
                           CCDelayTime::create(0.02f),
                           
                           CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPointZero)),
                           
                           CCCallFunc::create(this, callfunc_selector(CANavigationController::pushViewControllerFinish)),
                           
                           CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)),
                           
                           NULL);
        viewController->getView()->stopAllActions();
        viewController->getView()->runAction(actions);
    }
    else
    {
        this->pushViewControllerFinish();
    }
}

void CANavigationController::pushViewControllerFinish()
{
    CAViewController* viewController = m_pViewControllers.back();
    
    if (viewController->getNavigationBarItem() == NULL && viewController->getTitle().compare("") != 0)
    {
        viewController->setNavigationBarItem(CANavigationBarItem::create(viewController->getTitle()));
    }
    m_pNavigationBar->pushItem(viewController->getNavigationBarItem());
    viewController->getView()->setFrameOrigin(CCPointZero);
    
    CAViewController* lastViewController = m_pViewControllers.at(m_pViewControllers.size() - 2);
    lastViewController->getView()->setFrameOrigin(CCPointZero);
    lastViewController->getView()->setVisible(false);
    lastViewController->viewDidDisappear();
}

CAViewController* CANavigationController::popViewControllerAnimated(bool animated)
{
    if (m_pViewControllers.size() == 1)
    {
        return NULL;
    }
    
    if (m_pContainer->getActionByTag(0))
    {
        return NULL;
    }
    
    float x = m_pContainer->getFrame().size.width;
    
    unsigned int index = m_pViewControllers.size() - 2;
    CAViewController* showViewController = m_pViewControllers.at(index);
    if (!showViewController->getView()->getBounds().size.equals(m_pContainer->getBounds().size))
    {
        showViewController->getSuperViewRect(m_pContainer->getBounds());
    }
    showViewController->getView()->setFrameOrigin(CCPoint(-x/2.0f, 0));
    showViewController->getView()->setVisible(true);
    showViewController->viewDidAppear();
    
    CAViewController* backViewController = m_pViewControllers.back();
    
    if (animated)
    {
        CCSequence* actionsLast =
        CCSequence::create(
                           CCDelayTime::create(0.02f),
                           
                           CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPointZero)),
                           
                           NULL);
        showViewController->getView()->stopAllActions();
        showViewController->getView()->runAction(actionsLast);
        
        
        CCSequence* actions =
        CCSequence::create(
                           CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(), callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)),
                           
                           CCDelayTime::create(0.03f),
                           
                           CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPoint(x, 0))),
                           
                           CCCallFunc::create(this, callfunc_selector(CANavigationController::popViewControllerFinish)),
                           
                           CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)),
                           
                           NULL);
        backViewController->getView()->stopAllActions();
        backViewController->getView()->runAction(actions);
    }
    else
    {
        this->popViewControllerFinish();
    }
    return backViewController;
}

void CANavigationController::popViewControllerFinish()
{
    CAViewController* lastViewController = m_pViewControllers.back();
    lastViewController->viewDidDisappear();
    lastViewController->m_pNavigationController = NULL;
    lastViewController->removeViewFromSuperview();
    lastViewController->retain()->autorelease();
    m_pViewControllers.popBack();
    m_pNavigationBar->popItem();
    CAViewController* showViewController = m_pViewControllers.back();
    showViewController->getView()->setFrameOrigin(CCPointZero);
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
    
    CCPoint point = CCPointZero;
    
    if (m_bNavigationBarHidden)
    {
        switch (m_eNavigationBarVerticalAlignment)
        {
            case CABarVerticalAlignmentTop:
            {
                point.y = -m_pNavigationBar->getFrame().size.height;
            }
                break;
            case CABarVerticalAlignmentBottom:
            {
                point.y = this->getView()->getBounds().size.height;
            }
                break;
            default:
                break;
        }
    }
    else
    {
        switch (m_eNavigationBarVerticalAlignment)
        {
            case CABarVerticalAlignmentTop:
            {
                point.y = 0;
            }
                break;
            case CABarVerticalAlignmentBottom:
            {
                point.y = this->getView()->getBounds().size.height - m_pNavigationBar->getFrame().size.height;
            }
                break;
            default:
                break;
        }
    }
    
    if (animated)
    {
        CCArray* array = CCArray::create();
        array->addObject(CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, point)));
        array->addObject(CCCallFunc::create(this, callfunc_selector(CANavigationController::scheduleUpdate)));
        array->addObject(CCCallFunc::create(this, callfunc_selector(CANavigationController::unScheduleUpdate)));
        array->addObject(CCDelayTime::create(0.1f));
        m_pNavigationBar->runAction(CCSequence::create(array));
    }
    else
    {
        m_pNavigationBar->setFrameOrigin(point);
        if (this->getView()->getSuperview())
        {
            this->update(0);
        }
    }
    
}

void CANavigationController::update(float dt)
{
    CCRect rect = m_pContainer->getFrame();
    
    switch (m_eNavigationBarVerticalAlignment)
    {
        case CABarVerticalAlignmentTop:
        {
            rect.origin.y = m_pNavigationBar->getFrame().origin.y + m_pNavigationBar->getFrame().size.height;
            rect.size.height = this->getView()->getBounds().size.height - rect.origin.y;
        }
            break;
        case CABarVerticalAlignmentBottom:
        {
            rect.size.height = m_pNavigationBar->getFrame().origin.y;
        }
            break;
        default:
            break;
    }
    
    m_pContainer->setFrame(rect);
    
    CAViewController* viewController = m_pViewControllers.back();
    rect.origin = CCPointZero;
    viewController->getSuperViewRect(rect);
}

void CANavigationController::scheduleUpdate()
{
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    CAScheduler::unschedule(schedule_selector(CANavigationController::update), this);
    CAScheduler::schedule(schedule_selector(CANavigationController::update), this, 1/60.0f);
}

void CANavigationController::unScheduleUpdate()
{
    CAScheduler::unschedule(schedule_selector(CANavigationController::update), this);
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
    if (m_bNavigationBarHidden)
    {
        m_pNavigationBar->setVisible(false);
    }
    else
    {
        m_pNavigationBar->setVisible(true);
    }
}

#pragma CATabBarController

CATabBarController::CATabBarController()
:m_nSelectedIndex(0)
,m_nLastSelectedIndex(0xffffffff)
,m_pTabBar(NULL)
,m_pContainer(NULL)
,m_bTabBarHidden(false)
,m_bscrollEnabled(false)
{
    m_pView->setColor(CAColor_clear);
    m_pView->setDisplayRange(false);
}

CATabBarController::~CATabBarController()
{
    m_pViewControllers.clear();
    CC_SAFE_RELEASE_NULL(m_pTabBar);
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
        switch (m_eTabBarVerticalAlignment)
        {
            case CABarVerticalAlignmentBottom:
            {
                tab_bar_rectOrgin.y = container_rect.size.height;
            }
                break;
            case CABarVerticalAlignmentTop:
            {
                tab_bar_rectOrgin.y = -m_pTabBar->getFrame().size.height;
            }
                break;
            default:
                break;
        }
    }
    else
    {
        container_rect.size.height -= m_pTabBar->getFrame().size.height;
        switch (m_eTabBarVerticalAlignment)
        {
            case CABarVerticalAlignmentBottom:
            {
                tab_bar_rectOrgin.y = container_rect.size.height;
            }
                break;
            case CABarVerticalAlignmentTop:
            {
                container_rect.origin.y = m_pTabBar->getFrame().size.height;
            }
                break;
            default:
                break;
        }
    }
    
    m_pContainer = CAPageView::createWithFrame(container_rect, CAPageView::CAPageViewDirectionHorizontal);
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
    
    m_pTabBar->setFrameOrigin(tab_bar_rectOrgin);
    this->getView()->addSubview(m_pTabBar);
    
    m_pTabBar->setSelectedAtIndex(m_nSelectedIndex);
    this->renderingSelectedViewController();
}

void CATabBarController::viewDidUnload()
{
    m_pContainer = NULL;
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

void CATabBarController::setTabBarHidden(bool hidden, bool animated)
{
    CC_RETURN_IF(m_bTabBarHidden == hidden);
    m_bTabBarHidden = hidden;
    CC_RETURN_IF(this->getView()->getSuperview() == NULL);
    
    CCPoint point = CCPointZero;
    
    if (m_bTabBarHidden)
    {
        switch (m_eTabBarVerticalAlignment)
        {
            case CABarVerticalAlignmentTop:
            {
                point.y = -m_pTabBar->getFrame().size.height;
            }
                break;
            case CABarVerticalAlignmentBottom:
            {
                point.y = this->getView()->getBounds().size.height;
            }
                break;
            default:
                break;
        }
    }
    else
    {
        switch (m_eTabBarVerticalAlignment)
        {
            case CABarVerticalAlignmentTop:
            {
                point.y = 0;
            }
                break;
            case CABarVerticalAlignmentBottom:
            {
                point.y = this->getView()->getBounds().size.height - m_pTabBar->getFrame().size.height;
            }
                break;
            default:
                break;
        }
    }
    
    if (animated)
    {
        m_pTabBar->stopAllActions();
        CCFrameOrginTo* moveTo = CCFrameOrginTo::create(0.2f, point);
        CCEaseSineOut* easeBack = CCEaseSineOut::create(moveTo);
        CCCallFunc* begin = CCCallFunc::create(this, callfunc_selector(CATabBarController::scheduleUpdate));
        CCCallFunc* end = CCCallFunc::create(this, callfunc_selector(CATabBarController::unScheduleUpdate));
        CCDelayTime* delayTime = CCDelayTime::create(0.1f);
        CCSequence* actions = CCSequence::create(begin, easeBack, delayTime, end, NULL);
        m_pTabBar->runAction(actions);
    }
    else
    {
        m_pTabBar->setFrameOrigin(point);
        if (this->getView()->getSuperview())
        {
            this->update(0);
        }
    }
    
}

void CATabBarController::update(float dt)
{
    CCRect rect = m_pContainer->getFrame();
    
    switch (m_eTabBarVerticalAlignment)
    {
        case CABarVerticalAlignmentTop:
        {
            rect.origin.y = m_pTabBar->getFrame().origin.y + m_pTabBar->getFrame().size.height;
            rect.size.height = this->getView()->getBounds().size.height - rect.origin.y;
        }
            break;
        case CABarVerticalAlignmentBottom:
        {
            rect.size.height = m_pTabBar->getFrame().origin.y;
        }
            break;
        default:
            break;
    }
    
    m_pContainer->setFrame(rect);
    
    for (unsigned int i=0; i<m_pViewControllers.size(); i++)
    {
        CAViewController* viewController = m_pViewControllers.at(i);
        viewController->getSuperViewRect(m_pContainer->getBounds());
    }
}

void CATabBarController::scheduleUpdate()
{
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    CAScheduler::unschedule(schedule_selector(CATabBarController::update), this);
    CAScheduler::schedule(schedule_selector(CATabBarController::update), this, 1/60.0f);
}

void CATabBarController::unScheduleUpdate()
{
    CAScheduler::unschedule(schedule_selector(CATabBarController::update), this);
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
    if (m_bTabBarHidden)
    {
        m_pTabBar->setVisible(false);
    }
    else
    {
        m_pTabBar->setVisible(true);
    }
}

NS_CC_END;
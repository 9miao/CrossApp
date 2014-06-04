//
//  CAViewController.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-11.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#include "CAViewController.h"
#include "CCDirector.h"
#include "cocoa/CCString.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionEase.h"
#include "CAScheduler.h"
NS_CC_BEGIN

CAViewController::CAViewController()
:m_pView(NULL)
,m_pNavigationController(NULL)
,m_pNavigationBarItem(NULL)
,m_pTabBarController(NULL)
,m_pTabBarItem(NULL)
,m_sTitle("")
,m_bLifeLock(false)
,m_bKeypadEnabled(false)
{
    m_pView = CAView::createWithFrame(CCRectZero, ccc4(255, 255, 255, 255));
    m_pView->retain();
    m_pView->setViewDelegate(this);
}

CAViewController::~CAViewController()
{
    m_pView->setViewDelegate(NULL);
    CC_SAFE_RELEASE_NULL(m_pView);
    CC_SAFE_DELETE(m_pTabBarItem);
    CC_SAFE_DELETE(m_pNavigationBarItem);
    CCDirector* pDirector = CCDirector::sharedDirector();
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

/// isKeypadEnabled getter
bool CAViewController::isKeypadEnabled()
{
    return m_bKeypadEnabled;
}

/// isKeypadEnabled setter
void CAViewController::setKeypadEnabled(bool enabled)
{
    if (enabled != m_bKeypadEnabled)
    {
        m_bKeypadEnabled = enabled;
        
        CCDirector* pDirector = CCDirector::sharedDirector();
        if (enabled)
        {
            pDirector->getKeypadDispatcher()->addDelegate(this);
        }
        else
        {
            pDirector->getKeypadDispatcher()->removeDelegate(this);
        }
    }
}

#pragma CANavigationController

CANavigationController::CANavigationController()
:m_pViewControllers(NULL)
,m_pNavigationBar(NULL)
,m_pContainer(NULL)
,m_bNavigationBarHidden(false)
{
    m_pNavigationBar = CANavigationBar::create();
    m_pNavigationBar->retain();
}

CANavigationController::~CANavigationController()
{
    std::vector<CAViewController*>::iterator itr;
    for (itr=m_pViewControllers.begin(); itr!=m_pViewControllers.end(); itr++)
    {
        (*itr)->autorelease();
    }
    m_pViewControllers.clear();
    
    if (m_pNavigationBar)
    {
        CC_SAFE_RELEASE_NULL(m_pNavigationBar);
    }
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
        
    viewController->retain();
    viewController->m_pNavigationController = this;
    m_pViewControllers.push_back(viewController);
    if (viewController->getNavigationBarItem() == NULL && viewController->getTitle().compare("") != 0)
    {
        viewController->setNavigationBarItem(CANavigationBarItem::create(viewController->getTitle()));
    }
    m_pNavigationBar->pushItem(viewController->getNavigationBarItem());
    
    m_eNavigationBarVerticalAlignment = var;
    
    return true;
}

void CANavigationController::viewDidLoad()
{
    CCRect navigation_bar_rect = CCRectZero;
    
    CCRect container_rect = this->getView()->getBounds();
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
    
    m_pContainer = CAView::createWithFrame(container_rect);
    this->getView()->addSubview(m_pContainer);
    
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
    
    m_pContainer->removeFromSuperview();
    m_pContainer = NULL;
    m_pNavigationBar->removeFromSuperview();
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
    lastViewController->getView()->setFrame(CCRect(-x, 0, 0, 0));
    viewController->retain();
    viewController->m_pNavigationController = this;
    m_pViewControllers.push_back(viewController);
    if (viewController->getNavigationBarItem() == NULL && viewController->getTitle().compare("") != 0)
    {
        viewController->setNavigationBarItem(CANavigationBarItem::create(viewController->getTitle()));
    }
    m_pNavigationBar->pushItem(viewController->getNavigationBarItem());
    viewController->addViewFromSuperview(m_pContainer);
    
    if (animated)
    {
        m_pContainer->stopAllActions();
        m_pContainer->setFrame(CCRect(x, m_pContainer->getFrame().origin.y, 0, 0));
        
        CCDelayTime* delayTime = CCDelayTime::create(0.2f);
        CCMoveBy* moveBy = CCMoveBy::create(0.4f, CCPoint(-x, 0));
        CCEaseSineOut* easeBack = CCEaseSineOut::create(moveBy);
        CCCallFunc* finish = CCCallFunc::create(this, callfunc_selector(CANavigationController::pushViewControllerFinish));
        CCSequence* actions = CCSequence::create(delayTime, easeBack, finish, NULL);
        m_pContainer->runAction(actions);
        actions->setTag(0);
    }
    else
    {
        this->pushViewControllerFinish();
    }
}

void CANavigationController::pushViewControllerFinish()
{
    m_pContainer->setFrame(CCRect(0, m_pContainer->getFrame().origin.y, 0, 0));
    
    CAViewController* lastViewController = m_pViewControllers.at(m_pViewControllers.size() - 2);
    lastViewController->getView()->removeFromSuperview();
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
    
    unsigned int index = m_pViewControllers.size() - 2;
    CAViewController* showViewController = m_pViewControllers.at(index);
    showViewController->getView()->setFrame(CCRectZero);
    m_pContainer->addSubview(showViewController->getView());
    
    CAViewController* backViewController = m_pViewControllers.back();
    
    float x = m_pContainer->getFrame().size.width;
    backViewController->getView()->setFrame(CCRect(x, 0, 0, 0));
    
    if (animated)
    {
        m_pContainer->stopAllActions();
        m_pContainer->setFrame(CCRect(-x, m_pContainer->getFrame().origin.y, 0, 0));
        
        CCDelayTime* delayTime = CCDelayTime::create(0.2f);
        CCMoveBy* moveBy = CCMoveBy::create(0.4f, CCPoint(x, 0));
        CCEaseSineOut* easeBack = CCEaseSineOut::create(moveBy);
        CCCallFunc* finish = CCCallFunc::create(this, callfunc_selector(CANavigationController::popViewControllerFinish));
        CCSequence* actions = CCSequence::create(delayTime, easeBack, finish, NULL);
        m_pContainer->runAction(actions);
        actions->setTag(0);
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
    lastViewController->m_pNavigationController = NULL;
    lastViewController->removeViewFromSuperview();
    lastViewController->autorelease();
    m_pViewControllers.pop_back();
    m_pNavigationBar->popItem();
    m_pContainer->setFrame(CCRect(0, m_pContainer->getFrame().origin.y, 0, 0));
}

void CANavigationController::navigationPopViewController(CANavigationBar* navigationBar, bool animated)
{
    this->popViewControllerAnimated(animated);
}

void CANavigationController::setNavigationBarHidden(bool hidden, bool animated)
{
    switch (m_eNavigationBarVerticalAlignment)
    {
        case CABarVerticalAlignmentTop:
        {
        
        }
            break;
        case CABarVerticalAlignmentBottom:
        {
        
        }
            break;
        default:
            break;
    }
}

#pragma CATabBarController

CATabBarController::CATabBarController()
:m_nSelectedIndex(0)
,m_pTabBar(NULL)
,m_pContainer(NULL)
,m_bTabBarHidden(false)
{

}

CATabBarController::~CATabBarController()
{
    std::vector<CAViewController*>::iterator itr;
    for (itr=m_pViewControllers.begin(); itr!=m_pViewControllers.end(); itr++)
    {
        (*itr)->autorelease();
    }
    m_pViewControllers.clear();
    
    CC_SAFE_RELEASE_NULL(m_pTabBar);
    CC_SAFE_RELEASE_NULL(m_pContainer);
}

bool CATabBarController::initWithViewControllers(const std::vector<CAViewController*>& viewControllers, CABarVerticalAlignment var)
{
    CAViewController::init();
    
    do
    {
        CC_BREAK_IF(viewControllers.size() == 0);
        m_pViewControllers.insert(m_pViewControllers.begin(), viewControllers.begin(), viewControllers.end());
        
        std::vector<CATabBarItem*> items;
        
        for (unsigned int i=0; i<m_pViewControllers.size(); i++)
        {
            CAViewController* view = m_pViewControllers.at(i);
            view->retain();
            view->m_pTabBarController = this;
            if (view->getTabBarItem() == NULL)
            {
                const char* title = CCString::createWithFormat("item%d",i)->getCString();
                CATabBarItem* item = CATabBarItem::create(title, NULL, i);
                view->setTabBarItem(item);
            }
            items.push_back(view->getTabBarItem());
        }
        
        m_pTabBar = CATabBar::create(items);
        m_pTabBar->retain();
        m_pTabBar->setDelegate(this);
        
        m_pContainer = new CAScrollView();
        m_pContainer->initWithFrame(CCRectZero);
        m_pContainer->setScrollEnabled(false);
        m_pContainer->setBounces(false);
        m_pContainer->setShowsHorizontalScrollIndicator(false);
        m_pContainer->setShowsVerticalScrollIndicator(false);
        
    }
    while (0);
    
    m_eTabBarVerticalAlignment = var;
    
    return true;
    
}

void CATabBarController::viewDidLoad()
{
    CCRect tab_bar_rect = CCRectZero;
    
    CCRect container_rect = this->getView()->getBounds();
    container_rect.size.height -= m_pTabBar->getFrame().size.height;
    
    CCSize container_view_size = container_rect.size;
    container_view_size.width *= m_pViewControllers.size();
    
    switch (m_eTabBarVerticalAlignment)
    {
        case CABarVerticalAlignmentBottom:
        {
            tab_bar_rect.origin.y = container_rect.size.height;
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
    
    m_pContainer->setFrame(container_rect);
    m_pContainer->setViewSize(container_view_size);
    this->getView()->addSubview(m_pContainer);
    CCPoint point = CCPoint(m_pContainer->getFrame().size.width * m_nSelectedIndex, 0);
    m_pContainer->setContentOffset(point, false);
    
    m_pTabBar->setFrame(tab_bar_rect);
    this->getView()->addSubview(m_pTabBar);
    
    unsigned int index = 0;
    while (index < m_pViewControllers.size())
    {
        CCRect rect = container_rect;
        rect.origin.x = index * rect.size.width;
        rect.origin.y = 0;
        CAView* view = CAView::createWithFrame(rect);
        m_pContainer->addSubview(view);
        m_pViewControllers.at(index)->getView()->setFrame(view->getBounds());
        m_pViewControllers.at(index)->addViewFromSuperview(view);
        index++;
    }
}

void CATabBarController::viewDidUnload()
{
    std::vector<CAViewController*>::iterator itr;
    for (itr=m_pViewControllers.begin(); itr!=m_pViewControllers.end(); itr++)
    {
        (*itr)->removeViewFromSuperview();
    }
    
    m_pContainer->removeAllSubviews();
    m_pContainer->removeFromSuperview();
    m_pContainer = NULL;
    m_pTabBar->removeFromSuperview();
}

bool CATabBarController::showSelectedViewController(CAViewController* viewController, bool animated)
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
        
        return this->showSelectedViewControllerAtIndex(index, animated);
    }
    while (0);
    
    return false;
}

CAViewController* CATabBarController::getSelectedViewController()
{
    return m_pViewControllers.at(m_nSelectedIndex);
}

bool CATabBarController::showSelectedViewControllerAtIndex(unsigned int index, bool animated)
{
    do
    {
        CC_BREAK_IF(index >= m_pViewControllers.size());
        CC_BREAK_IF(index == m_nSelectedIndex);
        m_nSelectedIndex = index;
        
        m_pTabBar->setSelectedAtIndex(m_nSelectedIndex);
        
        if (animated)
        {
            this->renderingAllViewController();
        }
        
        CCPoint point = CCPoint(m_pContainer->getFrame().size.width * m_nSelectedIndex, 0);
        
        m_pContainer->setContentOffset(point, animated);
        
        CAScheduler::schedule(schedule_selector(CATabBarController::renderingSelectedViewController), this, 0, 1, 1.0f,false);
        
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
    do
    {
        CC_BREAK_IF(index == m_nSelectedIndex);
        this->showSelectedViewControllerAtIndex(index, true);
    }
    while (0);
}

void CATabBarController::renderingAllViewController(float dt)
{
    std::vector<CAViewController*>::iterator itr;
    for (itr=m_pViewControllers.begin(); itr!=m_pViewControllers.end(); itr++)
    {
        (*itr)->getView()->setVisible(true);
    }
}

void CATabBarController::renderingSelectedViewController(float dt)
{
    std::vector<CAViewController*>::iterator itr;
    for (itr=m_pViewControllers.begin(); itr!=m_pViewControllers.end(); itr++)
    {
        (*itr)->getView()->setVisible(false);
    }
    m_pViewControllers.at(m_nSelectedIndex)->getView()->setVisible(true);
}

void CATabBarController::setTabBarHidden(bool hidden, bool animated)
{
    do
    {
        CC_BREAK_IF(m_bTabBarHidden == hidden);
        m_bTabBarHidden = hidden;
        
        if (m_bTabBarHidden)
        {
            
            
            
            
        }
        else
        {
        
        }
        
        switch (m_eTabBarVerticalAlignment)
        {
            case CABarVerticalAlignmentTop:
            {
                
            }
                break;
            case CABarVerticalAlignmentBottom:
            {
                
            }
                break;
            default:
                break;
        }
    }
    while (0);
}

NS_CC_END;
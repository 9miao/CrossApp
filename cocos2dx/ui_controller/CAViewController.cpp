//
//  CAViewController.cpp
//  cocos2dx
//
//  Created by Li Yuanfeng on 14-4-11.
//  Copyright (c) 2014 www.9miao.com All rights reserved.
//

#include "CAViewController.h"
#include "CCDirector.h"
#include "cocoa/CCString.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionEase.h"
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
    m_pView = CAView::createWithFrame(CCRectZero);
    m_pView->retain();
    m_pView->setViewDelegate(this);
}

CAViewController::~CAViewController()
{
    m_pView->setViewDelegate(NULL);
    //CC_SAFE_RELEASE_NULL(m_pView);
    m_pView->release();
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
        
    viewController->retain();
    viewController->m_pNavigationController = this;
    m_pViewControllers.push_back(viewController);
    if (viewController->getNavigationBarItem() == NULL && viewController->getTitle().compare("") != 0)
    {
        viewController->setNavigationBarItem(CANavigationBarItem::create(viewController->getTitle()));
    }
    m_pNavigationBar->pushItem(viewController->getNavigationBarItem());
    
    return true;
}

void CANavigationController::viewDidLoad()
{
    CCRect rect = this->getView()->getBounds();
    rect.size.height -= m_pNavigationBar->getFrame().size.height;
    rect.origin.y = m_pNavigationBar->getFrame().size.height;
    
    m_pContainer = CAView::createWithFrame(rect);
    this->getView()->addSubview(m_pContainer);
    
    CAViewController* viewController = m_pViewControllers.front();
    viewController->addViewFromSuperview(m_pContainer);
    
    m_pNavigationBar->setFrame(CCRectZero);
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
    CCLog("view:%d",viewController->retainCount());
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
        
        CCMoveBy* moveBy = CCMoveBy::create(0.3f, CCPoint(-x, 0));
        CCEaseSineOut* easeBack = CCEaseSineOut::create(moveBy);
        CCCallFunc* finish = CCCallFunc::create(this, callfunc_selector(CANavigationController::pushViewControllerFinish));
        CCSequence* actions = CCSequence::create(easeBack, finish, NULL);
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
        
        CCMoveBy* moveBy = CCMoveBy::create(0.3f, CCPoint(x, 0));
        CCEaseSineOut* easeBack = CCEaseSineOut::create(moveBy);
        CCCallFunc* finish = CCCallFunc::create(this, callfunc_selector(CANavigationController::popViewControllerFinish));
        CCSequence* actions = CCSequence::create(easeBack, finish, NULL);
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
    //lastViewController->autorelease();
    m_pViewControllers.pop_back();
    m_pNavigationBar->popItem();
    m_pContainer->setFrame(CCRect(0, m_pContainer->getFrame().origin.y, 0, 0));
}

void CANavigationController::navigationPopViewController(CANavigationBar* navigationBar, bool animated)
{
    this->popViewControllerAnimated(animated);
}

#pragma CATabBarController

CATabBarController::CATabBarController()
:m_pSelectedViewController(NULL)
,m_nSelectedIndex(0)
,m_pTabBar(NULL)
,m_pContainer(NULL)
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
    
    if (m_pTabBar)
    {
        CC_SAFE_RELEASE_NULL(m_pTabBar);
    }
}

bool CATabBarController::initWithViewControllers(const std::vector<CAViewController*>& viewControllers)
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
    }
    while (0);
    
    return true;
    
}

void CATabBarController::viewDidLoad()
{
    CCRect rect = this->getView()->getBounds();
    rect.size.height -= m_pTabBar->getFrame().size.height;
    
    m_pContainer = CAView::createWithFrame(rect);
    this->getView()->addSubview(m_pContainer);
    
    m_pTabBar->setFrame(CCRect(0, rect.size.height, 0, 0));
    this->getView()->addSubview(m_pTabBar);
}

void CATabBarController::viewDidUnload()
{
    std::vector<CAViewController*>::iterator itr;
    for (itr=m_pViewControllers.begin(); itr!=m_pViewControllers.end(); itr++)
    {
        (*itr)->removeViewFromSuperview();
    }
    
    m_pContainer->removeFromSuperview();
    m_pContainer = NULL;
    m_pTabBar->removeFromSuperview();
}

bool CATabBarController::showSelectedViewController(CAViewController* viewController, bool animated)
{
    do
    {
        CC_BREAK_IF(viewController->isEqual(m_pSelectedViewController));
        
        std::vector<CAViewController*>::iterator itr;
        for (itr=m_pViewControllers.begin(); itr!=m_pViewControllers.end(); itr++)
        {
            CC_BREAK_IF(viewController->isEqual(*itr));
        }
        CC_BREAK_IF(itr == m_pViewControllers.end());
        CC_BREAK_IF((*itr)->isEqual(m_pSelectedViewController));
        
        if (m_pSelectedViewController)
        {
            m_pSelectedViewController->getView()->removeFromSuperview();
            m_pSelectedViewController = NULL;
        }
        
        m_pSelectedViewController = *itr;
        m_pContainer->addSubview(m_pSelectedViewController->getView());
        
        if (animated)
        {
            
        }
        else
        {
            
        }
        
        return true;
    }
    while (0);
    
    return false;
}

CAViewController* CATabBarController::getSelectedViewController()
{
    return m_pSelectedViewController;
}

bool CATabBarController::showSelectedViewControllerAtIndex(unsigned int index, bool animated)
{
    do
    {
        CC_BREAK_IF(index >= m_pViewControllers.size());
        CAViewController* viewController = m_pViewControllers.at(index);
        CC_BREAK_IF(viewController->isEqual(m_pSelectedViewController));
        
        if (m_pSelectedViewController)
        {
            m_pSelectedViewController->getView()->removeFromSuperview();
            m_pSelectedViewController = NULL;
        }
        
        m_pSelectedViewController = viewController;
        m_pContainer->addSubview(m_pSelectedViewController->getView());
        
        if (animated)
        {
            
        }
        else
        {
        
        }
        
        return true;
    }
    while (0);
    
    return false;
}

unsigned int CATabBarController::getSelectedViewControllerAtIndex()
{
    return m_nSelectedIndex;
}

void CATabBarController::tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, int index)
{
    this->showSelectedViewControllerAtIndex(index, true);
}

NS_CC_END;
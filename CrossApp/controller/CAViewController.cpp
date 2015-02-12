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
:m_bNavigationBarHidden(false)
,m_bPopViewController(false)
,m_pNavigationBarBackGroundImage(NULL)
,m_sNavigationBarBackGroundColor(CAColor_white)
,m_sNavigationBarTitleColor(CAColor_white)
,m_sNavigationBarButtonColor(CAColor_white)
{
    m_pView->setColor(CAColor_clear);
    m_pView->setDisplayRange(false);
    
    this->setTouchMovedListenVertical(false);
    this->setTouchMoved(true);
    m_bSlidingMaxX = true;
    m_bSlidingMinX = true;
    
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
}

CAImage* CANavigationController::getNavigationBarBackGroundImage()
{
    return m_pNavigationBarBackGroundImage;
}

void CANavigationController::setNavigationBarBackGroundColor(const CAColor4B &var)
{
    m_sNavigationBarBackGroundColor = var;
    CC_SAFE_RELEASE_NULL(m_pNavigationBarBackGroundImage);
}

const CAColor4B& CANavigationController::getNavigationBarBackGroundColor()
{
    return m_sNavigationBarBackGroundColor;
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
    for (std::vector<CAViewController*>::iterator itr=m_pViewControllers.begin();
         itr!=m_pViewControllers.end();
         itr++)
    {
        (*itr)->removeViewFromSuperview();
    }
    
    for (std::vector<CAView*>::iterator itr=m_pContainers.begin();
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
    container->release();
    m_pContainers.pushBack(container);
    
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
    CCRect navigation_bar_rect = CCRectZero;
    
    CCRect container_rect = this->getView()->getBounds();
    
    if (m_bNavigationBarHidden)
    {
        if (m_eNavigationBarVerticalAlignment == CABarVerticalAlignmentTop)
        {
            navigation_bar_rect.origin.y = -m_tNavigationBarSize.height;
        }
        else
        {
            navigation_bar_rect.origin.y = container_rect.size.height;
        }
    }
    else
    {
        container_rect.size.height -= m_tNavigationBarSize.height;
        if (m_eNavigationBarVerticalAlignment == CABarVerticalAlignmentTop)
        {
            container_rect.origin.y = m_tNavigationBarSize.height;
        }
        else
        {
            navigation_bar_rect.origin.y = container_rect.size.height;
        }
    }
    
    CAView* container = m_pContainers.back();
    container->setFrame(this->getView()->getBounds());
    
    CANavigationBar* navigationBar = m_pNavigationBars.back();
    navigationBar->setFrame(navigation_bar_rect);

    CAView* secondContainer = m_pSecondContainers.back();
    secondContainer->setFrame(container_rect);
    
    CAViewController* viewController = m_pViewControllers.back();
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
    
    if (animated)
    {
        CAViewAnimation::beginAnimations("navigation_lastContainer_replace", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveLinear);
        lastContainer->setFrameOrigin(CCPoint(-x/2.0f, 0));
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("navigation_newContainer_replace", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveLinear);
        newContainer->setFrameOrigin(CCPointZero);
        CAViewAnimation::commitAnimations();
        

        CCArray* newActions = CCArray::create();
        newActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(), callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)));
        newActions->addObject(CCDelayTime::create(0.27f));
        newActions->addObject(CCCallFunc::create(this, callfunc_selector(CANavigationController::replaceViewControllerFinish)));
        newActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)));
        newContainer->runAction(CCSequence::create(newActions));
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
    lastViewController->retain()->autorelease();
    m_pViewControllers.erase(index);
    
    CAView* backContainer = m_pContainers.at(index);
    backContainer->removeFromSuperview();
    m_pContainers.erase(index);
    
    m_pNavigationBars.erase(index);
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
    newContainer->setFrameOrigin(CCPoint(x, 0));
    
    if (animated)
    {
        CAViewAnimation::beginAnimations("navigation_lastContainer_replace", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        lastContainer->setFrameOrigin(CCPoint(-x/2.0f, 0));
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("navigation_newContainer_replace", NULL);
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        newContainer->setFrameOrigin(CCPointZero);
        CAViewAnimation::commitAnimations();
        
        
        CCArray* newActions = CCArray::create();
        newActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(), callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)));
        newActions->addObject(CCDelayTime::create(0.27f));
        newActions->addObject(CCCallFunc::create(this, callfunc_selector(CANavigationController::pushViewControllerFinish)));
        newActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)));
        newContainer->runAction(CCSequence::create(newActions));
        
    }
    else
    {
        this->pushViewControllerFinish();
    }
}

void CANavigationController::pushViewControllerFinish()
{
    float x = this->getView()->getBounds().size.width;
    
    CAView* lastContainer = m_pContainers.at(m_pContainers.size() - 2);
    lastContainer->setVisible(false);
    lastContainer->setFrameOrigin(CCPoint(-x/2.0f, 0));
    
    CAView* newContainer = m_pContainers.back();
    newContainer->setFrameOrigin(CCPointZero);
    
    CAViewController* lastViewController = m_pViewControllers.at(m_pViewControllers.size() - 2);
    lastViewController->viewDidDisappear();
    
    m_bSlidingMinX = m_pViewControllers.size() <= 1;
}

CAViewController* CANavigationController::popViewControllerAnimated(bool animated)
{
    if (m_pViewControllers.size() == 1)
    {
        return NULL;
    }

    float x = this->getView()->getBounds().size.width;
    
    size_t index = m_pViewControllers.size() - 2;
    CAViewController* showViewController = m_pViewControllers.at(index);
    showViewController->viewDidAppear();
    
    CAViewController* backViewController = m_pViewControllers.back();
    
    CAView* showContainer = m_pContainers.at(m_pContainers.size() - 2);
    showContainer->setVisible(true);
    showContainer->setFrameOrigin(CCPoint(-x/2.0f, 0));
    
    CAView* backContainer = m_pContainers.back();
    backContainer->setFrameOrigin(CCPointZero);
    
    if (animated)
    {
        CCArray* showActions = CCArray::create();
        showActions->addObject(CCDelayTime::create(0.02f));
        showActions->addObject(CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPointZero)));
        showContainer->runAction(CCSequence::create(showActions));
        
        CCArray* backActions = CCArray::create();
        backActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(), callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)));
        backActions->addObject(CCDelayTime::create(0.03f));
        backActions->addObject(CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPoint(x, 0))));
        backActions->addObject(CCCallFunc::create(this, callfunc_selector(CANavigationController::popViewControllerFinish)));
        backActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)));
        backContainer->runAction(CCSequence::create(backActions));
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
    
    CAView* backContainer = m_pContainers.back();
    backContainer->removeFromSuperview();
    m_pContainers.popBack();
    
    m_pNavigationBars.popBack();
    
    m_bSlidingMinX = m_pViewControllers.size() <= 1;
}

void CANavigationController::homingViewControllerFinish()
{
    unsigned int index = m_pViewControllers.size() - 2;
    CAViewController* lastViewController = m_pViewControllers.at(index);
    lastViewController->viewDidDisappear();
    
    CAView* lastContainer = m_pContainers.at(index);
    lastContainer->setVisible(false);
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
                point.y = -m_tNavigationBarSize.height;
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
                point.y = this->getView()->getBounds().size.height - m_tNavigationBarSize.height;
            }
                break;
            default:
                break;
        }
    }
    
    for (int i=0; i<m_pNavigationBars.size() - 1; i++)
    {
        m_pNavigationBars.at(i)->setFrameOrigin(point);
        this->updateNavigationBarHidden(i);
    }
    
    if (animated)
    {
        m_pNavigationBars.back()->stopAllActions();
        m_pNavigationBars.back()->setVisible(true);
        CCArray* array = CCArray::create();
        array->addObject(CCCallFunc::create(this, callfunc_selector(CANavigationController::scheduleUpdate)));
        array->addObject(CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, point)));
        array->addObject(CCDelayTime::create(0.1f));
        array->addObject(CCCallFunc::create(this, callfunc_selector(CANavigationController::unScheduleUpdate)));
        m_pNavigationBars.back()->runAction(CCSequence::create(array));
    }
    else
    {
        m_pNavigationBars.back()->setFrameOrigin(point);
        if (this->getView()->getSuperview())
        {
            this->update(0);
        }
    }
}

void CANavigationController::updateNavigationBarHidden(int index)
{
    CCRect containerRect = this->getView()->getBounds();
    containerRect.origin = m_pContainers.at(index)->getFrameOrigin();
    m_pContainers.at(index)->setFrame(containerRect);
    CCRect rect = m_pContainers.at(index)->getBounds();
    CCRect navigationBarFrame = m_pNavigationBars.at(index)->getFrame();
    
    switch (m_eNavigationBarVerticalAlignment)
    {
        case CABarVerticalAlignmentTop:
        {
            rect.origin.y = navigationBarFrame.origin.y + navigationBarFrame.size.height;
            rect.size.height = rect.size.height - rect.origin.y;
        }
            break;
        case CABarVerticalAlignmentBottom:
        {
            rect.size.height = navigationBarFrame.origin.y;
        }
            break;
        default:
            break;
    }
    CAView* secondContainer = m_pSecondContainers.at(index);
    CAViewController* viewController = m_pViewControllers.at(index);
    secondContainer->setFrame(rect);
    viewController->getSuperViewRect(secondContainer->getBounds());
}

void CANavigationController::update(float dt)
{
    this->updateNavigationBarHidden(m_pNavigationBars.size() - 1);
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
        m_pNavigationBars.back()->setVisible(false);
    }
    else
    {
        m_pNavigationBars.back()->setVisible(true);
    }
}

bool CANavigationController::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
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
    CC_RETURN_IF(m_pViewControllers.size() <= 1);
    
    float x = this->getView()->getBounds().size.width;
    unsigned int index = m_pViewControllers.size() - 2;
    CAViewController* lastViewController = m_pViewControllers.at(index);
    lastViewController->viewDidAppear();

    CAView* lastContainer = m_pContainers.at(index);
    lastContainer->setVisible(true);
    
    CAView* backContainer = m_pContainers.back();
    
    if (m_bPopViewController)
    {
        CCArray* laseActions = CCArray::create();
        laseActions->addObject(CCDelayTime::create(0.02f));
        laseActions->addObject(CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPointZero)));
        lastContainer->runAction(CCSequence::create(laseActions));
        
        CCArray* backActions = CCArray::create();
        backActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(), callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)));
        backActions->addObject(CCDelayTime::create(0.03f));
        backActions->addObject(CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPoint(x, 0))));
        backActions->addObject(CCCallFunc::create(this, callfunc_selector(CANavigationController::popViewControllerFinish)));
        backActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)));
        backContainer->runAction(CCSequence::create(backActions));
    }
    else
    {
        CCArray* lastActions = CCArray::create();
        lastActions->addObject(CCDelayTime::create(0.03f));
        lastActions->addObject(CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPoint(-x/2.0f, 0))));
        lastContainer->runAction(CCSequence::create(lastActions));
        
        CCArray* newActions = CCArray::create();
        newActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(), callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)));
        newActions->addObject(CCDelayTime::create(0.02f));
        newActions->addObject(CCEaseSineOut::create(CCFrameOrginTo::create(0.25f, CCPointZero)));
        newActions->addObject(CCCallFunc::create(this, callfunc_selector(CANavigationController::homingViewControllerFinish)));
        newActions->addObject(CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)));
        backContainer->runAction(CCSequence::create(newActions));
    }
}

void CANavigationController::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->ccTouchEnded(pTouch, pEvent);
}

void CANavigationController::setTouchMoved(bool var)
{
    m_bTouchMovedStopSubviews = m_bTouchMoved = var;
}

bool CANavigationController::isTouchMoved()
{
    return m_bTouchMoved;
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
}

void CATabBarController::setTabBarBackGroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pTabBarBackGroundImage);
    m_pTabBarBackGroundImage = var;
    m_sTabBarBackGroundColor = CAColor_white;
}

CAImage* CATabBarController::getTabBarBackGroundImage()
{
    return m_pTabBarBackGroundImage;
}

void CATabBarController::setTabBarBackGroundColor(const CAColor4B &var)
{
    m_sTabBarBackGroundColor = var;
    CC_SAFE_RELEASE_NULL(m_pTabBarBackGroundImage);
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
}

CAImage* CATabBarController::getTabBarSelectedBackGroundImage()
{
    return m_pTabBarSelectedBackGroundImage;
}

void CATabBarController::setTabBarSelectedBackGroundColor(const CAColor4B &var)
{
    m_sTabBarSelectedBackGroundColor = var;
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedBackGroundImage);
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
}

CAImage* CATabBarController::getTabBarSelectedIndicatorImage()
{
    return m_pTabBarSelectedIndicatorImage;
}

void CATabBarController::setTabBarSelectedIndicatorColor(const CAColor4B &var)
{
    m_sTabBarSelectedIndicatorColor = var;
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedIndicatorImage);
}

const CAColor4B& CATabBarController::getTabBarSelectedIndicatorColor()
{
    return m_sTabBarSelectedIndicatorColor;
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
        m_pTabBar->setSelectedIndicatorColor(m_sTabBarSelectedBackGroundColor);
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
        m_pTabBar->setVisible(true);
        CCCallFunc* begin = CCCallFunc::create(this, callfunc_selector(CATabBarController::scheduleUpdate));
        CCEaseSineOut* easeBack = CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, point));
        CCDelayTime* delayTime = CCDelayTime::create(0.1f);
        CCCallFunc* end = CCCallFunc::create(this, callfunc_selector(CATabBarController::unScheduleUpdate));
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
//
//  CAViewController.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-4-11.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAViewController.h"
#include "basics/CAApplication.h"
#include "basics/CAPointExtension.h"
#include "basics/CAScheduler.h"
#include "view/CAWindow.h"
#include "view/CAScale9ImageView.h"
#include "dispatcher/CATouchDispatcher.h"
#include "animation/CAViewAnimation.h"
#include "animation/CACustomAnimation.h"
#include "support/CAUIEditorParser.h"
#include "device/CADevice.h"
#include "script_support/CAScriptSupport.h"
#include "support/CAThemeManager.h"
#include "support/ccUtils.h"

NS_CC_BEGIN

CAViewController::CAViewController()
:m_pView(nullptr)
,m_pModalViewController(nullptr)
,m_pNavigationController(nullptr)
,m_pNavigationBarItem(nullptr)
,m_pTabBarController(nullptr)
,m_pTabBarItem(nullptr)
,m_sTitle("The Title")
,m_pParser(nullptr)
{
    this->setView(CAView::createWithColor(CAColor4B::WHITE));
}

CAViewController::~CAViewController()
{
    CC_SAFE_RELEASE_NULL(m_pParser);
    m_pView->setContentContainer(nullptr);
    CC_SAFE_RELEASE_NULL(m_pView);
    CC_SAFE_RELEASE_NULL(m_pTabBarItem);
    CC_SAFE_RELEASE_NULL(m_pNavigationBarItem);
}

bool CAViewController::init()
{
    this->parser();
    return true;
}

CAView* CAViewController::getViewWithID(const std::string &tag)
{
    return m_pParser->m_mViews.at(tag);
}

void CAViewController::parser()
{
    m_pParser = new CAUIEditorParser();
    
    std::string name = typeid(*this).name();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    name = name.substr(6, name.length() - 6);
#else
    
    if (name.length() <= 10)
    {
        name = name.substr(1, name.length() - 1);
    }
    else
    {
        name = name.substr(2, name.length() - 2);
    }

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
        if(CAScriptEngineManager::getScriptEngineManager())
        {
#if CC_ENABLE_SCRIPT_BINDING
            if (CAScriptEngineManager::getScriptEngineManager()->getScriptEngine()->getScriptType()== kScriptTypeJavascript)
            {
                CAScriptEngineManager::sendViewControllerEventToJS(this, script::viewDidLoad);
            }
#endif
        }
        else
        {
            this->viewDidLoad();
        }
    }
    while (0);

}

void CAViewController::viewOnExitTransitionDidStart()
{
    CAScheduler::getScheduler()->pauseTarget(this);   

    if(CAScriptEngineManager::getScriptEngineManager())
    {
#if CC_ENABLE_SCRIPT_BINDING
        if (CAScriptEngineManager::getScriptEngineManager()->getScriptEngine()->getScriptType()== kScriptTypeJavascript)
        {
            CAScriptEngineManager::sendViewControllerEventToJS(this, script::viewDidUnload);
        }
#endif
    }
    else
    {
        this->viewDidUnload();
    }
}

void CAViewController::viewOnSizeTransitionDidChanged()
{
    if(CAScriptEngineManager::getScriptEngineManager())
    {
#if CC_ENABLE_SCRIPT_BINDING
        if (CAScriptEngineManager::getScriptEngineManager()->getScriptEngine()->getScriptType()== kScriptTypeJavascript)
        {
            CAScriptEngineManager::sendViewControllerEventToJS(this, script::viewSizeDidChanged);
        }
#endif
    }
    else
    {
        this->viewSizeDidChanged();
    }
}

void CAViewController::setViewVisibleTrue()
{
    this->getView()->setVisible(true);
    if(CAScriptEngineManager::getScriptEngineManager())
    {
#if CC_ENABLE_SCRIPT_BINDING
        if (CAScriptEngineManager::getScriptEngineManager()->getScriptEngine()->getScriptType()== kScriptTypeJavascript)
        {
            if (CAScriptEngineManager::sendViewControllerEventToJS(this, script::viewDidAppear));
        }
#endif
    }
    else
    {
        this->viewDidAppear();
    }
}

void CAViewController::setViewVisibleFalse()
{
    this->getView()->setVisible(false);
    if(CAScriptEngineManager::getScriptEngineManager())
    {
#if CC_ENABLE_SCRIPT_BINDING
        if (CAScriptEngineManager::getScriptEngineManager()->getScriptEngine()->getScriptType()== kScriptTypeJavascript)
        {
            if (CAScriptEngineManager::sendViewControllerEventToJS(this, script::viewDidDisappear));
        }
#endif
    }
    else
    {
        this->viewDidDisappear();
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

void CAViewController::setView(CAView* view)
{
    if (m_pView)
    {
        m_pView->removeFromSuperview();
        m_pView->release();
        m_pView = nullptr;
    }
    m_pView = view;
    if (m_pView)
    {
        m_pView->retain();
        m_pView->setContentContainer(this);
        m_pView->setLayout(DLayoutFill);
        m_pView->setHaveNextResponder(false);
    }
}

void CAViewController::addViewFromSuperview(CAView* node)
{
    node->addSubview(m_pView);
}

void CAViewController::removeViewFromSuperview()
{
    m_pView->removeFromSuperview();
}

bool CAViewController::isViewRunning()
{
    return (bool)(m_pView->getSuperview() && m_pView->isVisible());
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

void CAViewController::presentModalViewController(CAViewController* controller, bool animated)
{
    CC_RETURN_IF(m_pModalViewController);
    CC_SAFE_RETAIN(controller);
    m_pModalViewController = controller;

    m_pModalViewController->addViewFromSuperview(this->getView());
    m_pModalViewController->getView()->setZOrder(CAWindowZOrderTop);
    m_pModalViewController->setViewVisibleTrue();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();
        DLayout layout = view->getLayout();
        float y = this->getView()->getBounds().size.height;
        layout.vertical = DVerticalLayout_T_B(y, -y);
        view->setLayout(layout);

        CAViewAnimation::beginAnimations("");
        CAViewAnimation::setAnimationDuration(0.3f);
        CAViewAnimation::setAnimationDelay(0.1f);
        CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
        CAViewAnimation::setAnimationDidStopSelector([&]()
        {
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
        });
        view->setLayout(DLayoutFill);
        CAViewAnimation::commitAnimations();
    }
    else
    {
        CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
    }
}


void CAViewController::dismissModalViewController(bool animated)
{
    CC_RETURN_IF(m_pModalViewController == nullptr);
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    if (animated)
    {
        CAView* view = m_pModalViewController->getView();

        CAViewAnimation::beginAnimations("");
        CAViewAnimation::setAnimationDuration(0.3f);
        CAViewAnimation::setAnimationDelay(0.1f);
        CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
        CAViewAnimation::setAnimationDidStopSelector([&]()
        {
            m_pModalViewController->setViewVisibleFalse();
            m_pModalViewController->removeViewFromSuperview();
            CC_SAFE_RELEASE_NULL(m_pModalViewController);
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
        });
        DLayout layout = view->getLayout();
        float y = m_pModalViewController->getView()->getBounds().size.height;
        layout.vertical = DVerticalLayout_T_B(y, -y);
        view->setLayout(layout);
        CAViewAnimation::commitAnimations();
    }
    else
    {
        m_pModalViewController->setViewVisibleFalse();
        m_pModalViewController->removeViewFromSuperview();
        CC_SAFE_RELEASE_NULL(m_pModalViewController);
        CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
    }
}

#pragma CANavigationController

CANavigationController::CANavigationController()
:m_bNavigationBarHidden(false)
,m_bPopViewController(false)
,m_bNavigationBarEnabledShadow(true)
,m_pNavigationBarBackgroundImage(nullptr)
,m_sNavigationBarTitleColor(CAColor4B::WHITE)
,m_sNavigationBarButtonColor(CAColor4B::WHITE)
,m_pNavigationBarGoBackBarButtonItem(nullptr)
,m_fProgress(1.0f)
,m_iClearance(0)
{
    this->setTouchMoved(true);
    
    m_pView->setVerticalScrollEnabled(false);
    m_pView->setColor(CAColor4B::CLEAR);
    m_pView->setDisplayRange(false);
    
    m_pView->onTouchBegan(CALLBACK_BIND_2(CANavigationController::ccTouchBegan, this));
    m_pView->onTouchMoved(CALLBACK_BIND_2(CANavigationController::ccTouchMoved, this));
    m_pView->onTouchEnded(CALLBACK_BIND_2(CANavigationController::ccTouchEnded, this));
    m_pView->onTouchCancelled(CALLBACK_BIND_2(CANavigationController::ccTouchCancelled, this));
    
    
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CANavigationBar");
    this->setNavigationBarBackgroundImage(CAImage::create(map.at("backgroundView")));
    m_sNavigationBarTitleColor = ccc4Int(CrossApp::hex2Int(map.at("titleColor")));
}

CANavigationController::~CANavigationController()
{
    m_pViewControllers.clear();
    m_pNavigationBars.clear();
    m_pContainers.clear();
    m_pSecondContainers.clear();
    CC_SAFE_RELEASE(m_pNavigationBarGoBackBarButtonItem);
    CC_SAFE_RELEASE_NULL(m_pNavigationBarBackgroundImage);
}

void CANavigationController::setNavigationBarBackgroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pNavigationBarBackgroundImage);
    m_pNavigationBarBackgroundImage = var;
    
    if (!m_pNavigationBars.empty())
    {
        CADeque<CANavigationBar*>::iterator itr;
        for (itr=m_pNavigationBars.begin(); itr!=m_pNavigationBars.end(); itr++)
        {
            (*itr)->setBackgroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackgroundImage));
        }
    }
}

CAImage* CANavigationController::getNavigationBarBackgroundImage()
{
    return m_pNavigationBarBackgroundImage;
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

void CANavigationController::setNavigationBarGoBackBarButtonItem(CrossApp::CABarButtonItem *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pNavigationBarGoBackBarButtonItem);
    m_pNavigationBarGoBackBarButtonItem = var;

    if (m_pNavigationBarGoBackBarButtonItem && !m_pNavigationBars.empty())
    {
        CADeque<CANavigationBar*>::iterator itr;
        for (itr=m_pNavigationBars.begin(); itr!=m_pNavigationBars.end(); itr++)
        {
            (*itr)->setGoBackBarButtonItem(m_pNavigationBarGoBackBarButtonItem);
        }
    }
}

CABarButtonItem* CANavigationController::getNavigationBarGoBackBarButtonItem()
{
    return m_pNavigationBarGoBackBarButtonItem;
}

void CANavigationController::setNavigationBarEnabledShadow(bool var)
{
    m_bNavigationBarEnabledShadow = var;
    
    for (auto itr=m_pNavigationBars.begin(); itr!=m_pNavigationBars.end(); itr++)
    {
        (*itr)->setEnabledShadow(m_bNavigationBarEnabledShadow);
    }
}

bool CANavigationController::isNavigationBarEnabledShadow()
{
    return m_bNavigationBarEnabledShadow;
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
    
    m_pNavigationBars.at(index)->setBackgroundView(CAScale9ImageView::createWithImage(m_pNavigationBarBackgroundImage));


    m_pNavigationBars.at(index)->setTitleColor(m_sNavigationBarTitleColor);
    m_pNavigationBars.at(index)->setButtonColor(m_sNavigationBarButtonColor);
    m_pNavigationBars.at(index)->setItem(viewController->getNavigationBarItem());
    
    DLayout navLayout;
    navLayout.horizontal = DHorizontalLayoutFill;
    navLayout.vertical.height = m_iNavigationBarHeight;
    
    if (viewController->getNavigationBarItem() && viewController->getNavigationBarItem()->isNagigationBarHidden())
    {
        navLayout.vertical.top = -m_iNavigationBarHeight;
    }
    else
    {
        navLayout.vertical.top = 0;
    }
    m_pNavigationBars.at(index)->setLayout(navLayout);
    
    DLayout secondLayout;
    secondLayout.horizontal = DHorizontalLayoutFill;
    secondLayout.vertical = DVerticalLayout_T_B(navLayout.vertical.top + navLayout.vertical.height, 0);

    m_pSecondContainers.at(index)->setLayout(secondLayout);
}

void CANavigationController::viewDidLoad()
{
    CAViewController* viewController = m_pViewControllers.front();
    viewController->retain()->autorelease();
    m_pViewControllers.popFront();
    
    this->layout();

    this->createWithContainer(viewController, DLayoutFill);
}

void CANavigationController::viewDidUnload()
{

}

void CANavigationController::viewDidAppear()
{
    CC_RETURN_IF(m_pViewControllers.empty());
    m_pViewControllers.back()->setViewVisibleTrue();
}

void CANavigationController::viewDidDisappear()
{
    CC_RETURN_IF(m_pViewControllers.empty());
    m_pViewControllers.back()->setViewVisibleFalse();
}

void CANavigationController::viewSizeDidChanged()
{
    this->layout();
}

void CANavigationController::layout()
{
    m_iClearance = CABar::get_top_clearance(this->getView());

    m_iNavigationBarHeight = m_iClearance + 88;
    
    for (int i=0; i < m_pNavigationBars.size(); i++)
    {
        auto navbar = m_pNavigationBars.at(i);
        navbar->updateClearance(m_iClearance);
        
        DLayout navLayout = navbar->getLayout();
        navLayout.vertical.height = m_iNavigationBarHeight;
    
        if (navLayout.vertical.top < 0)
        {
            navLayout.vertical.top = -m_iNavigationBarHeight;
        }
        navbar->setLayout(navLayout);
        
        DLayout secondLayout;
        secondLayout.horizontal = DHorizontalLayoutFill;
        secondLayout.vertical = DVerticalLayout_T_B(navLayout.vertical.top + navLayout.vertical.height, 0);
        
        auto secondContainer = m_pSecondContainers.at(i);
        secondContainer->setLayout(secondLayout);
    }
}

void CANavigationController::createWithContainer(CAViewController* viewController, const DLayout& layout)
{
    CAView* container = new CAView();
    container->setLayout(layout);
    container->enabledLeftShadow(true);
    container->setHaveNextResponder(false);
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
    
    CANavigationBar* navigationBar = CANavigationBar::createWithLayout(navLayout, m_iClearance);
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
    
    if (m_pNavigationBarGoBackBarButtonItem)
    {
        navigationBar->setGoBackBarButtonItem(m_pNavigationBarGoBackBarButtonItem);
    }
    

    navigationBar->setTitleColor(m_sNavigationBarTitleColor);
    navigationBar->setButtonColor(m_sNavigationBarButtonColor);
    navigationBar->setEnabledShadow(m_bNavigationBarEnabledShadow);
    container->insertSubview(navigationBar, 1);
    navigationBar->onPopViewController([&]
    {
        this->popViewControllerAnimated(true);
    });
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
        viewController->setViewVisibleTrue();
    }
}

/************************************/

void beginPushAnimation(float width, CAView* last, CAView* new_, const std::function<void()>& function)
{
    CAViewAnimation::beginAnimations("navgationController-push");
    CAViewAnimation::setAnimationDelay(0.1f);
    CAViewAnimation::setAnimationDuration(0.25f);
    CAViewAnimation::setAnimationDelay(1/30.0f);
    CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
    last->setFrameOrigin(DPoint(-width/3.0f, 0));
    CAViewAnimation::setAnimationDidStopSelector(function);
    new_->setFrameOrigin(DPointZero);
    CAViewAnimation::commitAnimations();
}

void beginPopAnimation(const DRect& bounds, CAView* show, CAView* back, const std::function<void()>& function)
{
    DRect rect = bounds;
    rect.origin = DPoint(-bounds.size.width/3.0f, 0);
    show->setFrame(rect);

    CAViewAnimation::beginAnimations("navgationController-pop");
    CAViewAnimation::setAnimationDelay(0.1f);
    CAViewAnimation::setAnimationDuration(0.25f);
    CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
    CAViewAnimation::setAnimationDidStopSelector(function);
    show->setFrameOrigin(DPointZero);
    back->setFrameOrigin(DPoint(bounds.size.width + 10, 0));
    CAViewAnimation::commitAnimations();
}

bool isAnimation()
{
    if (CAViewAnimation::areBeginAnimationsWithID("navgationController-push"))
    {
        return true;
    }
    
    if (CAViewAnimation::areBeginAnimationsWithID("navgationController-pop"))
    {
        return true;
    }
    return false;
}

/************************************/

void CANavigationController::replaceViewController(CrossApp::CAViewController *viewController, bool animated)
{
    CC_RETURN_IF(this->getView()->getSuperview() == nullptr);
    CC_RETURN_IF(isAnimation());
    
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
    this->createWithContainer(viewController, DLayout(DHorizontalLayout_L_R(x + 10, -x - 10), DVerticalLayoutFill));
    CAView* newContainer = m_pContainers.back();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        beginPushAnimation(x, lastContainer, newContainer,
                           std::bind(&CANavigationController::replaceViewControllerFinish, this));
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
    lastViewController->setViewVisibleFalse();
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
    CC_RETURN_IF(this->getView()->getSuperview() == nullptr);
    CC_RETURN_IF(isAnimation());

    float x = this->getView()->getBounds().size.width;
    
    CAView* lastContainer = m_pContainers.back();
    this->createWithContainer(viewController, DLayout(DHorizontalLayout_L_R(x + 10, -x - 10), DVerticalLayoutFill));
    CAView* newContainer = m_pContainers.back();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    if (animated)
    {
        beginPushAnimation(x, lastContainer, newContainer,
                           std::bind(&CANavigationController::pushViewControllerFinish, this));
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
    lastViewController->setViewVisibleFalse();
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

CAViewController* CANavigationController::popViewControllerAnimated(bool animated)
{
    if (m_pViewControllers.size() == 1)
    {
        return nullptr;
    }
    
    if(isAnimation())
    {
        return nullptr;
    }
    
    CAViewController* backViewController = m_pViewControllers.back();
    CAView* backContainer = m_pContainers.back();
    {
        backContainer->setFrameOrigin(DPointZero);
    }
    
    size_t index = m_pViewControllers.size() - 2;
    CAViewController* showViewController = m_pViewControllers.at(index);
    showViewController->setViewVisibleTrue();
    
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
        beginPopAnimation(this->getView()->getBounds(), showContainer, backContainer,
                          std::bind(&CANavigationController::popViewControllerFinish, this));
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
    backViewController->setViewVisibleFalse();
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
    CC_RETURN_IF(m_pViewControllers.size() == 1);
    CC_RETURN_IF(isAnimation());
    
    float x = this->getView()->getBounds().size.width;
    
    CAView* backContainer = m_pContainers.back();
    backContainer->setFrameOrigin(DPointZero);
    
    size_t index = 0;
    CAViewController* showViewController = m_pViewControllers.at(index);
    showViewController->setViewVisibleTrue();
    
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
        beginPopAnimation(this->getView()->getBounds(), showContainer, backContainer,
                          std::bind(&CANavigationController::popToRootViewControllerFinish, this));
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
        backViewController->setViewVisibleFalse();
        backViewController->m_pNavigationController = nullptr;
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
    
    CAViewController* lastViewController = m_pViewControllers.at(index);
    lastViewController->setViewVisibleFalse();
    
    m_pContainers.back()->setLayout(DLayoutFill);
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}

CAViewController* CANavigationController::popFirstViewController()
{
    if (m_pViewControllers.size() <= 1)
    {
        return nullptr;
    }
    
    CAViewController* frontViewController = m_pViewControllers.front();
    frontViewController->setViewVisibleFalse();
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
        return nullptr;
    }
    
    if (isAnimation())
    {
        return nullptr;
    }
    
    if (index == m_pViewControllers.size() - 1)
    {
        return this->popViewControllerAnimated(false);
    }
    
    CAViewController* viewController = m_pViewControllers.at(index);
    viewController->setViewVisibleFalse();
    viewController->m_pNavigationController = nullptr;
    viewController->removeViewFromSuperview();
    viewController->retain()->autorelease();
    m_pViewControllers.erase(index);
    
    m_pContainers.at(index)->removeFromSuperview();
    m_pContainers.erase(index);
    
    m_pSecondContainers.erase(index);
    
    m_pNavigationBars.erase(index);
    
    return viewController;
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
    
    return nullptr;
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
        CACustomAnimation::schedule([&](const CACustomAnimation::Model& model)
        {
            m_fProgress = model.now / model.total;
            
            if (this->getView()->getSuperview())
            {
                this->update(0);
            }
        }, "CANavigationController:" + m_s__StrID, 0.25f);
        
        CAViewAnimation::beginAnimations("");
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationWillStartSelector([]()
        {
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
        });
        CAViewAnimation::setAnimationDidStopSelector([]()
        {
            CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
        });
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
    if (m_pViewControllers.size() == 1)
        return false;

    m_vTouches.pushBack(pTouch);
    
    if (m_vTouches.size() == 1)
    {
        m_bTouchBeganRange = pTouch->getLocation().x <= 120 ? true : false;
        m_bPopViewController = false;
    }
    
    return true;
}

void CANavigationController::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_vTouches.size() > 1);
    CC_RETURN_IF(m_bTouchBeganRange == false);
    CC_RETURN_IF(m_bTouchMoved == false);
    float offDis = pTouch->getLocation().x - pTouch->getPreviousLocation().x;
    CAView* showContainer = m_pContainers.at(m_pContainers.size() - 2);
    if (!showContainer->getBounds().size.equals(this->getView()->getBounds().size))
    {
        showContainer->setFrame(this->getView()->getBounds());
    }
    if (showContainer->isVisible() == false)
    {
        showContainer->setVisible(true);
        showContainer->setTouchEnabled(false);
        
        size_t index = m_pViewControllers.size() - 2;
        CAViewController* showViewController = m_pViewControllers.at(index);
        showViewController->setViewVisibleTrue();
    }
    
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
    m_vTouches.eraseObject(pTouch);
    CC_RETURN_IF(!m_vTouches.empty());

    float x = this->getView()->getBounds().size.width;
    size_t index = m_pViewControllers.size() - 2;

    CAView* lastContainer = m_pContainers.at(index);
    lastContainer->setVisible(true);
    lastContainer->setTouchEnabled(true);
    
    CAView* backContainer = m_pContainers.back();
    backContainer->setTouchEnabled(true);
    
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
    
    CAViewAnimation::removeAnimations("navigation_animation");
    CAViewAnimation::removeAnimations("navigation_animation2");
    
    if (m_bPopViewController)
    {
        CAViewAnimation::beginAnimations("navigation_animation");
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
        lastContainer->setFrameOrigin(DPointZero);
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("navigation_animation2");
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
        CAViewAnimation::setAnimationDidStopSelector(std::bind(&CANavigationController::popViewControllerFinish, this));
        backContainer->setFrameOrigin(DPoint(x, 0));
        CAViewAnimation::commitAnimations();
    }
    else
    {
        CAViewAnimation::beginAnimations("navigation_animation");
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.03f);
        CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
        lastContainer->setFrameOrigin(DPoint(-x/3.0f, 0));
        CAViewAnimation::commitAnimations();
        
        CAViewAnimation::beginAnimations("navigation_animation2");
        CAViewAnimation::setAnimationDuration(0.25f);
        CAViewAnimation::setAnimationDelay(0.02f);
        CAViewAnimation::setAnimationCurve(CAViewAnimation::Curve::EaseOut);
        CAViewAnimation::setAnimationDidStopSelector(std::bind(&CANavigationController::homingViewControllerFinish, this));
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
    m_pView->setPriorityScroll(var);
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
,m_pTabBar(nullptr)
,m_bTabBarHidden(false)
,m_pTabBarBackgroundImage(nullptr)
,m_pTabBarSelectedBackgroundImage(nullptr)
,m_pTabBarSelectedIndicatorImage(nullptr)
,m_bTabBarSelectedTitleBold(false)
,m_sTabBarTitleColor(CAColor4B::WHITE)
,m_sTabBarSelectedTitleColor(CAColor4B(50, 193, 255, 255))
,m_bShowTabBarSelectedIndicator(false)
,m_obTabBarLayout(DLayoutZero)
,m_obViewLayout(DLayoutZero)
,m_fProgress(1.0f)
,m_iTabBarHeight(0)
{
    m_pView->setColor(CAColor4B::CLEAR);
    m_pView->setDisplayRange(false);
    m_pView->onTouchBegan(CALLBACK_BIND_2(CATabBarController::ccTouchBegan, this));
    m_pView->onTouchMoved(CALLBACK_BIND_2(CATabBarController::ccTouchMoved, this));
    m_pView->onTouchEnded(CALLBACK_BIND_2(CATabBarController::ccTouchEnded, this));
    m_pView->onTouchCancelled(CALLBACK_BIND_2(CATabBarController::ccTouchCancelled, this));
    
    const CAThemeManager::stringMap& map = GETINSTANCE_THEMEMAP("CATabBar");
    
    this->setTabBarBackgroundImage(CAImage::create(map.at("backgroundView_normal")));
    
    this->setTabBarSelectedBackgroundImage(CAImage::create(map.at("backgroundView_selected")));
    
    this->setTabBarSelectedIndicatorImage(CAImage::create(map.at("bottomLine")));
    
    m_sTabBarTitleColor = ccc4Int(CrossApp::hex2Int(map.at("titleColor_normal")));
    m_sTabBarSelectedTitleColor = ccc4Int(CrossApp::hex2Int(map.at("titleColor_selected")));
}

CATabBarController::~CATabBarController()
{
    m_pViewControllers.clear();
    CC_SAFE_RELEASE_NULL(m_pTabBarBackgroundImage);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedBackgroundImage);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedIndicatorImage);
}

CATabBar* CATabBarController::getTabBar()
{
    return m_pTabBar;
}

void CATabBarController::setTabBarBackgroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pTabBarBackgroundImage);
    m_pTabBarBackgroundImage = var;
    
    if (m_pTabBar)
    {
        m_pTabBar->setBackgroundImage(m_pTabBarBackgroundImage);
    }
}

CAImage* CATabBarController::getTabBarBackgroundImage()
{
    return m_pTabBarBackgroundImage;
}

void CATabBarController::setTabBarSelectedBackgroundImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedBackgroundImage);
    m_pTabBarSelectedBackgroundImage = var;
    
    if (m_pTabBar)
    {
        m_pTabBar->setSelectedBackgroundImage(m_pTabBarSelectedBackgroundImage);
    }
}

CAImage* CATabBarController::getTabBarSelectedBackgroundImage()
{
    return m_pTabBarSelectedBackgroundImage;
}

void CATabBarController::setTabBarSelectedIndicatorImage(CrossApp::CAImage *var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE_NULL(m_pTabBarSelectedIndicatorImage);
    m_pTabBarSelectedIndicatorImage = var;
    
    if (m_pTabBar)
    {
        m_pTabBar->setSelectedIndicatorImage(m_pTabBarSelectedIndicatorImage);
    }
}

CAImage* CATabBarController::getTabBarSelectedIndicatorImage()
{
    return m_pTabBarSelectedIndicatorImage;
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

void CATabBarController::setTabBarTitleBoldForSelected(bool var)
{
    m_bTabBarSelectedTitleBold = var;
    
    if (m_pTabBar)
    {
        m_pTabBar->setTitleBoldForSelected(m_bTabBarSelectedTitleBold);
    }
}

bool CATabBarController::getTabBarTitleBoldForSelected()
{
    return m_bTabBarSelectedTitleBold;
}

void CATabBarController::showTabBarSelectedIndicator()
{
    m_bShowTabBarSelectedIndicator = true;
    if (m_pTabBar)
    {
        m_pTabBar->showSelectedIndicator();
    }
}

bool CATabBarController::initWithViewControllers(const CAVector<CAViewController*>& viewControllers, CATabBar::VerticalAlignment var)
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
    this->layout();

    std::vector<CATabBarItem*> items;
    
    for (unsigned int i=0; i<m_pViewControllers.size(); i++)
    {
        CAViewController* view = m_pViewControllers.at(i);
        if (view->getTabBarItem() == nullptr)
        {
            char title[8];
            sprintf(title, "item%d", i);
            CATabBarItem* item = CATabBarItem::create(title, nullptr);
            item->setTag(i);
            view->setTabBarItem(item);
        }
        items.push_back(view->getTabBarItem());
        view->m_pTabBarController = this;
    }

    m_pTabBar = CATabBar::createWithLayout(m_obTabBarLayout, CABar::get_top_clearance(this->getView()), m_eTabBarVerticalAlignment);
    m_pTabBar->setItems(items);
    m_pTabBar->onSelectedItem(CALLBACK_BIND_2(CATabBarController::tabBarSelectedItem, this));
    this->getView()->insertSubview(m_pTabBar, 1);
    
    m_pTabBar->setBackgroundImage(m_pTabBarBackgroundImage);
    m_pTabBar->setSelectedBackgroundImage(m_pTabBarSelectedBackgroundImage);
    m_pTabBar->setSelectedIndicatorImage(m_pTabBarSelectedIndicatorImage);

    m_pTabBar->setTitleColorForNormal(m_sTabBarTitleColor);
    m_pTabBar->setTitleColorForSelected(m_sTabBarSelectedTitleColor);
    m_pTabBar->setTitleBoldForSelected(m_bTabBarSelectedTitleBold);
    
    if (m_bShowTabBarSelectedIndicator)
    {
        m_pTabBar->showSelectedIndicator();
    }

    m_pTabBar->setSelectedAtIndex(m_nSelectedIndex);
    this->renderingSelectedViewController();
    
}

void CATabBarController::viewDidUnload()
{

}

void CATabBarController::viewDidAppear()
{
    m_pViewControllers.at(m_nSelectedIndex)->setViewVisibleTrue();
}

void CATabBarController::viewDidDisappear()
{
    m_pViewControllers.at(m_nSelectedIndex)->setViewVisibleFalse();
}

void CATabBarController::viewSizeDidChanged()
{
    this->layout();
}

void CATabBarController::layout()
{
    DLayout tabBarLayout;
    tabBarLayout.horizontal = DHorizontalLayoutFill;
    
    int clearance = 0;
    
    if (m_iTabBarHeight == 0)
    {
        
        if (m_eTabBarVerticalAlignment == CATabBar::VerticalAlignment::Top)
        {
            clearance = CABar::get_top_clearance(this->getView());
            m_iTabBarHeight = clearance + 88;
        }
        else
        {
            clearance = CABar::get_bottom_clearance(this->getView());
            m_iTabBarHeight = clearance + 98;
        }
        
    }
    
    if (m_eTabBarVerticalAlignment == CATabBar::VerticalAlignment::Top)
    {
        tabBarLayout.vertical.top = m_bTabBarHidden ? -m_iTabBarHeight : 0;
        tabBarLayout.vertical.height = m_iTabBarHeight;
    }
    else
    {
        tabBarLayout.vertical.bottom = m_bTabBarHidden ? -m_iTabBarHeight : 0;
        tabBarLayout.vertical.height = m_iTabBarHeight;
    }
    m_obTabBarLayout = tabBarLayout;
    
    DLayout viewLayout;
    viewLayout.horizontal = DHorizontalLayoutFill;
    
    if (m_eTabBarVerticalAlignment == CATabBar::VerticalAlignment::Top)
    {
        viewLayout.vertical = DVerticalLayout_T_B(tabBarLayout.vertical.top + tabBarLayout.vertical.height, 0);
    }
    else
    {
        viewLayout.vertical = DVerticalLayout_T_B(0, tabBarLayout.vertical.bottom + tabBarLayout.vertical.height);
    }
    m_obViewLayout = viewLayout;
    
    if (m_pTabBar)
    {
        m_pTabBar->updateClearance(clearance);
    }
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

void CATabBarController::tabBarSelectedItem(CATabBarItem* item, unsigned int index)
{
    CC_RETURN_IF(m_nSelectedIndex == index);
    this->showSelectedViewControllerAtIndex(index);
}

void CATabBarController::renderingSelectedViewController()
{
    if (m_nLastSelectedIndex < m_pViewControllers.size())
    {
        m_pViewControllers.at(m_nLastSelectedIndex)->setViewVisibleFalse();
    }
    
    if (!m_pViewControllers.at(m_nSelectedIndex)->getView()->getSuperview())
    {
        m_pViewControllers.at(m_nSelectedIndex)->getView()->setLayout(m_obViewLayout);
        m_pViewControllers.at(m_nSelectedIndex)->addViewFromSuperview(this->getView());
    }
    
    m_pViewControllers.at(m_nSelectedIndex)->setViewVisibleTrue();
}

int CATabBarController::getTabBarNowY()

{    int y = 0;

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
    CC_RETURN_IF(this->getView()->getSuperview() == nullptr);
    
    if (animated)
    {
        CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
        CACustomAnimation::schedule([&](const CACustomAnimation::Model& model)
        {
            m_fProgress = model.now / model.total;
            
            if (this->getView()->getSuperview())
            {
                this->update(0);
            }
            
            if (model.end)
            {
                CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
            }
            
        }, "CATabBarController:" + m_s__StrID, 0.25f);
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

void CATabBarController::update(float dt)
{
    int y = this->getTabBarNowY();

    switch (m_eTabBarVerticalAlignment)
    {
        case CATabBar::VerticalAlignment::Top:
        {
            m_obTabBarLayout.vertical.top = y;
            m_obViewLayout.vertical.top = y + m_iTabBarHeight;
        }
            break;
        case CATabBar::VerticalAlignment::Bottom:
        {
            m_obTabBarLayout.vertical.bottom = y;
            m_obViewLayout.vertical.bottom = y + m_iTabBarHeight;
        }
            break;
        default:
            break;
    }
    
    m_pTabBar->setLayout(m_obTabBarLayout);
    
    for (auto &var : m_pViewControllers)
    {
        var->getView()->setLayout(m_obViewLayout);
    }
}

NS_CC_END;

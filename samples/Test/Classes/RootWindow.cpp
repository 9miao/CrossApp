

#include "RootWindow.h"
#include "MenuViewController.h"
#include "FirstViewController.h"
#include "MyWebViewController.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

static RootWindow* _window = NULL;

RootWindow* RootWindow::getInstance()
{
    if (_window == NULL)
    {
        _window = new RootWindow();
        _window->init();
        _window->autorelease();
    }
    return _window;
    
}

RootWindow::RootWindow()
:m_pRootNavigationController(NULL)
,m_pRootDrawerController(NULL)
{
    CAApplication::getApplication()->getKeypadDispatcher()->onBackClicked([&]
    {
        if (this->getModalViewController())
        {
            this->dismissModalViewController(true);
        }
        else if (this->getDrawerController()->isShowLeftViewController())
        {
            this->getDrawerController()->hideLeftViewController(true);
        }
        else if (this->getRootNavigationController()->getViewControllerCount() > 1)
        {
            this->getRootNavigationController()->popViewControllerAnimated(true);
        }
        else
        {
            CAApplication::getApplication()->end();
        }
    });
    
    CAApplication::getApplication()->setStatusBarStyle(CAStatusBarStyle::LightContent);
}

RootWindow::~RootWindow()
{
}

bool RootWindow::init()
{
    if (!CAWindow::init())
    {
        return false;
    }
    
    CAVector<CAViewController*> conttollers;
    for (int i=0; i<3; i++)
    {
        conttollers.pushBack(FirstViewController::create());
    }

    CANavigationBarItem* firstItem = CANavigationBarItem::create(UTF8("控件展示"));
    CABarButtonItem* barButtonItem = CABarButtonItem::create("", CAImage::create("image/ic_category_list.png"), NULL);
    barButtonItem->setCallbackFunction([=]()
    {
        this->getDrawerController()->showLeftViewController(true);
    });
    firstItem->addLeftButtonItem(barButtonItem);
    
    CATabBarController* tabBarConttoller = new CATabBarController();
    tabBarConttoller->initWithViewControllers(conttollers);
    tabBarConttoller->setNavigationBarItem(firstItem);
    tabBarConttoller->autorelease();

    m_pRootNavigationController = new CANavigationController();
    m_pRootNavigationController->initWithRootViewController(tabBarConttoller);
    m_pRootNavigationController->autorelease();

    CADrawerController* drawer = new CADrawerController();
    drawer->initWithController(MenuViewController::create(), m_pRootNavigationController);
    drawer->autorelease();
    drawer->setBackgroundImage(CAImage::create("image/bg.jpg"));
    drawer->setEffect3D(true);
    
    this->setRootViewController(drawer);
    
    m_pRootDrawerController = drawer;

    return true;
}


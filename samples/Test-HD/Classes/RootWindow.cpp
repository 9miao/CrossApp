

#include "RootWindow.h"
#include "MenuViewController.h"
#include "CDUIShowCollectionView.h"
#include "CDNewsViewController.h"
#include "CDNewsImageController.h"
#include "CDNewsAboutController.h"
#include "CDWebViewController.h"
#include "CDTempViewController.h"
#include "CDUIShowView.h"
#include "CDNewsMenuView.h"

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
,m_pRootNavigationController1(NULL)
,m_pSplitNavigationController(NULL)
{
    CAApplication::getApplication()->getKeypadDispatcher()->addDelegate(this);
}

RootWindow::~RootWindow()
{
    CAApplication::getApplication()->getKeypadDispatcher()->removeDelegate(this);
}

bool RootWindow::init()
{
    if (!CAWindow::init())
    {
        return false;
    }
    winSize = this->getBounds().size;
    
    CAApplication::getApplication()->setNotificationView(CAView::createWithFrame(this->getBounds(), CAColor_green));
    
    MenuViewController* _menuview = MenuViewController::create();

    
    CDUIShowCollectionView* tabBarController = new CDUIShowCollectionView();
    tabBarController->init();
    tabBarController->autorelease();
    
    CANavigationBarItem* temp_nav = CANavigationBarItem::create(UTF8("控件展示"));
    tabBarController->setNavigationBarItem(temp_nav);
    
    m_pRootNavigationController = new CANavigationController();
    m_pRootNavigationController->initWithRootViewController(tabBarController);
    m_pRootNavigationController->setNavigationBarBackGroundImage(CAImage::create("image/navbg.jpg"));
    m_pRootNavigationController->getView()->setDisplayRange(true);
    
    CDTempViewController* viewContrller = new CDTempViewController();
    viewContrller->init();
    viewContrller->autorelease();
    
    CANavigationBarItem* temp_nav1 = CANavigationBarItem::create(" ");
    temp_nav1->setNagigationBarHidden(true);
    viewContrller->setNavigationBarItem(temp_nav1);
    m_pRootNavigationController1 = new CANavigationController();
    m_pRootNavigationController1->initWithRootViewController(viewContrller);
    m_pRootNavigationController1->setNavigationBarBackGroundImage(CAImage::create("image/navbg.jpg"));

    _vec.pushBack(_menuview);
    _vec.pushBack(m_pRootNavigationController);
    _vec.pushBack(m_pRootNavigationController1);
    _splitViewController = new CASplitViewController();
    vector<unsigned int> widths;
    widths.push_back(_px(200));
    widths.push_back(_px(winSize.width-200)/2);
    widths.push_back(_px(winSize.width-200)/2);
    _splitViewController->initWithController(_vec, widths);
    _splitViewController->setBackgroundView(CAImageView::createWithImage(CAImage::create("image/bg1.jpg")));
    
    CANavigationBarItem* temp_nav2 = CANavigationBarItem::create(" ");
    temp_nav2->setNagigationBarHidden(true);
    _splitViewController->setNavigationBarItem(temp_nav2);
    m_pSplitNavigationController = new CANavigationController();
    m_pSplitNavigationController->initWithRootViewController(_splitViewController);
    m_pSplitNavigationController->setNavigationBarBackGroundImage(CAImage::create("image/navbg.jpg"));
    
    this->setRootViewController(m_pSplitNavigationController);
    _splitViewController->autorelease();

    CAApplication::getApplication()->setNotificationView(NULL);
    CAApplication::getApplication()->setStatusBarStyle(CAStatusBarStyleLightContent);
    return true;
}

void RootWindow::draw()
{

}

void RootWindow::keyBackClicked()
{
    CC_RETURN_IF(CAAlertView::hideWithDisplayed());
    
    if (this->getModalViewController())
    {
        this->dismissModalViewController(true);
    }
    else if (this->getRootNavigationController()->getViewControllerCount() > 1)
    {
        this->getRootNavigationController()->popViewControllerAnimated(true);
    }else{
        CAApplication::getApplication()->end();
    }
}

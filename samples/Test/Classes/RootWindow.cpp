

#include "RootWindow.h"
#include "MenuViewController.h"
#include "CDUIShowCollectionView.h"
#include "CDNewsViewController.h"
#include "CDNewsImageController.h"
#include "CDNewsAboutController.h"
#include "CDWebViewController.h"
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
    CCSize winSize = this->getBounds().size;
    
    CAApplication::getApplication()->setNotificationView(CAView::createWithFrame(this->getBounds(), CAColor_green));
    
    this->initUIView();
    
    MenuViewController* _menuview = MenuViewController::create();
    
    CADrawerController* drawer = new CADrawerController();
    drawer->initWithController(_menuview, m_pRootNavigationController, this->getBounds().size.width/6*5);
    drawer->setBackgroundView(CAImageView::createWithImage(CAImage::create("image/bg.jpg")));
    drawer->setEffect3D(true);
    
    this->setRootViewController(drawer);
    drawer->autorelease();
    
    m_pRootDrawerController = drawer;
    CAApplication::getApplication()->setNotificationView(NULL);

    return true;
}

void RootWindow::draw()
{

}

void RootWindow::initUIView()
{
    do
    {
        CAViewController* viewController =
        m_pRootNavigationController
        ? m_pRootNavigationController->getViewControllerAtIndex(0) : NULL;
        
        CC_BREAK_IF(dynamic_cast<CDUIShowCollectionView*>(viewController));

        CDUIShowCollectionView* tabBarController = new CDUIShowCollectionView();
        tabBarController->init();
        tabBarController->autorelease();
        
        CANavigationBarItem* temp_nav = CANavigationBarItem::create(UTF8("控件展示"));
        CABarButtonItem* item = CABarButtonItem::create("", CAImage::create("image/ic_category_list.png"), NULL);
        item->setTarget(this, CAControl_selector(RootWindow::buttonCallBack));
        temp_nav->addLeftButtonItem(item);
        tabBarController->setNavigationBarItem(temp_nav);
        
        if (m_pRootNavigationController)
        {
            m_pRootNavigationController->replaceViewController(tabBarController, false);
        }
        else
        {
            m_pRootNavigationController = new CANavigationController();
            m_pRootNavigationController->initWithRootViewController(tabBarController);
            m_pRootNavigationController->setNavigationBarBackGroundImage(CAImage::create("image/navbg.jpg"));
        }
        
    }
    while (0);
    
    if (m_pRootDrawerController)
    {
        m_pRootDrawerController->hideLeftViewController(true);
    }
    
    CAApplication::getApplication()->setStatusBarStyle(CAStatusBarStyleLightContent);
}
void RootWindow::buttonCallBack(CAControl* btn,CCPoint point)
{
    this->getDrawerController()->showLeftViewController(true);
}
void RootWindow::intNewsView()
{
    do
    {
        CAViewController* viewController = m_pRootNavigationController->getViewControllerAtIndex(0);
        CC_BREAK_IF(dynamic_cast<CATabBarController*>(viewController));
        
        
        CAVector<CAViewController*> vec_news;
        
        CATabBarItem* item = CATabBarItem::create(unicode_to_utf8(newsTitle[0]), CAImage::create(""), CAImage::create(""));
        item->setBadgeValue("new");
        CDNewsViewController* news_controller1 = new CDNewsViewController(0);
        news_controller1->autorelease();
        news_controller1->setTabBarItem(item);
        vec_news.pushBack(news_controller1);
        
        CDNewsViewController* news_controller2 = new CDNewsViewController(1);
        news_controller2->autorelease();
        news_controller2->setTabBarItem(CATabBarItem::create(unicode_to_utf8(newsTitle[1]), CAImage::create(""), CAImage::create("")));
        vec_news.pushBack(news_controller2);
        
        CDNewsViewController* news_controller3 = new CDNewsViewController(2);
        news_controller3->autorelease();
        news_controller3->setTabBarItem(CATabBarItem::create(unicode_to_utf8(newsTitle[2]), CAImage::create(""), CAImage::create("")));
        vec_news.pushBack(news_controller3);
        
        CDNewsViewController* news_controller4 = new CDNewsViewController(3);
        news_controller4->autorelease();
        news_controller4->setTabBarItem(CATabBarItem::create(unicode_to_utf8(newsTitle[3]), CAImage::create(""), CAImage::create("")));
        vec_news.pushBack(news_controller4);
        
        CDNewsViewController* news_controller5 = new CDNewsViewController(4);
        news_controller5->autorelease();
        news_controller5->setTabBarItem(CATabBarItem::create(unicode_to_utf8(newsTitle[4]), CAImage::create(""), CAImage::create("")));
        vec_news.pushBack(news_controller5);
        
        
        CATabBarController * newsTabBarController = new CATabBarController();
        newsTabBarController->initWithViewControllers(vec_news,CABarVerticalAlignmentTop);
        newsTabBarController->setScrollEnabled(true);
        newsTabBarController->setTabBarItem(CATabBarItem::create("", CAImage::create("image/tab_news_btn_up.png"),CAImage::create("image/tab_news_btn_down.png")));
        newsTabBarController->setTabBarBackGroundImage(CAImage::create("source_material/tabbar_background.png"));
        newsTabBarController->setTabBarSelectedBackGroundColor(CAColor_clear);
        newsTabBarController->setTabBarTitleColorForNormal(CAColor_black);
        newsTabBarController->setTabBarTitleColorForSelected(ccc4(42,117,201,255));
        newsTabBarController->setTitle(" ");
        newsTabBarController->showTabBarSelectedIndicator();
        newsTabBarController->setTabBarSelectedIndicatorColor(ccc4(42,117,201,255));
        newsTabBarController->autorelease();
        
        CAVector<CAViewController*> vec;
        vec.pushBack(newsTabBarController);
        /////////////////NewsTabBar///////////////////////
        
        /////////////////ImageTabBar///////////////////////
        CAVector<CAViewController*> vec_image;
        
        CDNewsImageController* controller1 = new CDNewsImageController(0);
        controller1->autorelease();
        controller1->setTabBarItem(CATabBarItem::create(unicode_to_utf8(imageTitle[0]), CAImage::create(""), CAImage::create("")));
        vec_image.pushBack(controller1);
        
        CDNewsImageController* controller2 = new CDNewsImageController(1);
        controller2->setTabBarItem(CATabBarItem::create(unicode_to_utf8(imageTitle[1]), CAImage::create(""), CAImage::create("")));
        controller2->autorelease();
        vec_image.pushBack(controller2);
        
        CDNewsImageController* controller3 = new CDNewsImageController(2);
        controller3->setTabBarItem(CATabBarItem::create(unicode_to_utf8(imageTitle[2]), CAImage::create(""), CAImage::create("")));
        controller3->autorelease();
        vec_image.pushBack(controller3);
        
        CDNewsImageController* controller4 = new CDNewsImageController(3);
        controller4->setTabBarItem(CATabBarItem::create(unicode_to_utf8(imageTitle[3]), CAImage::create(""), CAImage::create("")));
        controller4->autorelease();
        vec_image.pushBack(controller4);
        
        CATabBarController * imageTabBarController = new CATabBarController();
        imageTabBarController->initWithViewControllers(vec_image,CABarVerticalAlignmentTop);
        imageTabBarController->setScrollEnabled(true);
        imageTabBarController->setTabBarItem(CATabBarItem::create("", CAImage::create("image/tab_image_btn_up.png"),CAImage::create("image/tab_image_btn_down.png")));
        imageTabBarController->setTabBarBackGroundImage(CAImage::create("source_material/tabbar_background.png"));
        imageTabBarController->setTabBarSelectedBackGroundColor(CAColor_clear);
        imageTabBarController->setTabBarTitleColorForNormal(CAColor_black);
        imageTabBarController->setTabBarTitleColorForSelected(ccc4(42,117,201,255));
        imageTabBarController->setTitle(" ");
        imageTabBarController->showTabBarSelectedIndicator();
        imageTabBarController->setTabBarSelectedIndicatorColor(ccc4(42,117,201,255));
        imageTabBarController->autorelease();
        
        vec.pushBack(imageTabBarController);
        
        /////////////////ImageTabBar///////////////////////
        
        CDNewsAboutController* _aboutController = CDNewsAboutController::create();
        _aboutController->setTabBarItem(CATabBarItem::create("", CAImage::create("image/tab_mine_btn_up.png"),CAImage::create("image/tab_mine_btn_down.png")));
        vec.pushBack(_aboutController);
        

        CATabBarController* tabBarController = new CATabBarController();
        tabBarController->initWithViewControllers(vec);
        tabBarController->setTitle(" ");
        tabBarController->setTabBarSelectedBackGroundColor(CAColor_clear);
        tabBarController->setTabBarBackGroundImage(CAImage::create("image/tab_news_bg.png"));
        CANavigationBarItem* temp_nav = CANavigationBarItem::create(" ");
        temp_nav->setNagigationBarHidden(true);
        tabBarController->setNavigationBarItem(temp_nav);
        tabBarController->autorelease();
        
        m_pRootNavigationController->replaceViewController(tabBarController, false);

    }
    while (0);
    
    m_pRootDrawerController->hideLeftViewController(true);
    CAApplication::getApplication()->setStatusBarStyle(CAStatusBarStyleDefault);
}

void RootWindow::keyBackClicked()
{
    CC_RETURN_IF(CAAlertView::hideWithDisplayed());
    
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
    }else{
        CAApplication::getApplication()->end();
    }
}

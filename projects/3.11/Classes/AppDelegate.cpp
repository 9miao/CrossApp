#include "AppDelegate.h"
#include "LTableBarController.h"
#include "RootViewController.h"
#include "ViewController.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60.0f);
    
    //pEGLView->setDesignResolutionSize(640, 960, kResolutionShowAll);
    // create a scene. it's an autorelease object
    m_pWindow = CAWindow::create();

    // run
    pDirector->runWithScene(m_pWindow);

    RootViewController* v = new RootViewController();
    v->init();
    v->setTabBarItem(CATabBarItem::create("111", CAImage::create("a.png"), 100));
    
    RootViewController* viewController = new RootViewController();
    viewController->init();
    viewController->setTabBarItem(CATabBarItem::create("111", CAImage::create("h.png"), 100));
    viewController->setTitle("view1");
    
    ViewController* viewController2 = new ViewController();
    viewController2->init();
    viewController2->setTabBarItem(CATabBarItem::create("222", CAImage::create("e.png"), 101));
    viewController2->setNavigationBarItem(CANavigationBarItem::create("viewController2"));
    viewController2->setTitle("view2");
    
    ViewController* viewController3 = new ViewController();
    viewController3->init();
    viewController3->setTabBarItem(CATabBarItem::create("333", CAImage::create("a.png"), 102));
    viewController3->setTitle("view3");
    
    ViewController* viewController4 = new ViewController();
    viewController4->init();
    viewController4->setTabBarItem(CATabBarItem::create("444", CAImage::create("o.png"), 103));
    viewController4->setTitle("view4");
    
    ViewController* viewController5 = new ViewController();
    viewController5->init();
    viewController5->setTabBarItem(CATabBarItem::create("555", CAImage::create("s.png"), 104));
    viewController5->setTitle("view5");
    
    std::vector<CAViewController*> views;
    views.push_back(viewController);
    views.push_back(viewController2);
    views.push_back(viewController3);
    views.push_back(viewController4);
    views.push_back(viewController5);
    
    CATabBarController* tabBarController = new CATabBarController();
    tabBarController->initWithViewControllers(views, true);
    tabBarController->getTabBar()->showSelectedIndicator();
    
    CANavigationController* navigationController = new CANavigationController();
    navigationController->initWithRootViewController(tabBarController);
    
    m_pWindow->setRootViewController(navigationController);
    navigationController->release();
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

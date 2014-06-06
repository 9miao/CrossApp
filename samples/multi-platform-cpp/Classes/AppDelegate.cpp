#include "AppDelegate.h"
#include "RootViewController.h"
#include "ViewController.h"
#include "FirstViewController.h"
USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
    
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);

	pDirector->setDelegate(this);
    
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60.0f);
    
    //pEGLView->setDesignResolutionSize(800, 1200, kResolutionShowAll);

    // create a scene. it's an autorelease object
	m_pWindow = CAWindow::create();
    
    // run
    pDirector->runWindow(m_pWindow);
    
    RootViewController* viewController = new RootViewController();
    viewController->init();
    viewController->setTitle("view1");
    viewController->setNavigationBarItem(CANavigationBarItem::create("The Home Page"));
    
    CANavigationController* navigationController = new CANavigationController();
    navigationController->initWithRootViewController(viewController);
    navigationController->setTabBarItem(CATabBarItem::create("First", CAImage::create("h.png"), 100));
    //navigationController->setNavigationBarHidden(true, false);
    
    viewController->release();
    
    ViewController* viewController2 = new ViewController();
    viewController2->init();
    viewController2->setTabBarItem(CATabBarItem::create("Second", CAImage::create("e.png"), 101));
    viewController2->setNavigationBarItem(CANavigationBarItem::create("viewController2"));
    viewController2->setTitle("view2");
    
    ViewController* viewController3 = new ViewController();
    viewController3->init();
    viewController3->setTabBarItem(CATabBarItem::create("Third", CAImage::create("a.png"), 102));
    viewController3->setTitle("view3");
    
    ViewController* viewController4 = new ViewController();
    viewController4->init();
    viewController4->setTabBarItem(CATabBarItem::create("Fourth", CAImage::create("o.png"), 103));
    viewController4->setTitle("view4");
    
    FirstViewController* viewController5 = new FirstViewController();
    viewController5->init();
    viewController5->setTabBarItem(CATabBarItem::create("Fifth", CAImage::create("s.png"), 104));
    viewController5->setTitle("view5");
    
    std::vector<CAViewController*> views;
    views.push_back(navigationController);
    views.push_back(viewController2);
    views.push_back(viewController3);
    views.push_back(viewController4);
    views.push_back(viewController5);
    
    CATabBarController* tabBarController = new CATabBarController();
    tabBarController->initWithViewControllers(views);
    tabBarController->getTabBar()->showSelectedIndicator();
    //tabBarController->showSelectedViewControllerAtIndex(4, false);
    
    navigationController->release();
    viewController2->release();
    viewController3->release();
    viewController4->release();
    viewController5->release();
    

    m_pWindow->setRootViewController(tabBarController);
    tabBarController->release();
    
    return true;
}

void AppDelegate::updateProjection(void)
{
    
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

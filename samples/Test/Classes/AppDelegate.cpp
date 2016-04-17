#include "AppDelegate.h"
#include "RootWindow.h"

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
    CAApplication* application = CAApplication::getApplication();
    
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    application->setOpenGLView(pEGLView);

    // run
    application->runWindow(RootWindow::getInstance());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CAApplication::getApplication()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CAApplication::getApplication()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

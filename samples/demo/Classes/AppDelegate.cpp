#include "AppDelegate.h"
#include "MainMenu.h"

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
    CAApplication* pApplication = CAApplication::getApplication();

    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pApplication->setOpenGLView(pEGLView);

	pApplication->setDelegate(this);

    // create a scene. it's an autorelease object
	m_pWindow = MainMenu::createWindow();
    
    // run
    pApplication->runWindow(m_pWindow);
    
    return true;
}

void AppDelegate::updateProjection(void)
{
    
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

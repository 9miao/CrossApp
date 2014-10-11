#include "AppDelegate.h"
#include "RootWindow.h"
#include "ScriptingCore.h"
#include "generated/jsb_CrossApp_auto.hpp"
#include "CrossApp_specifics.hpp"
#include "CrossAppExt.h"
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
    CAApplication* pDirector = CAApplication::getApplication();
    
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // run
    ScriptingCore* sc = ScriptingCore::getInstance();
    sc->addRegisterCallback(register_all_CrossApp);
    sc->addRegisterCallback(register_CrossApp_js_special);
    sc->start();
    
    CCScriptEngineProtocol *pEngine = ScriptingCore::getInstance();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    ScriptingCore::getInstance()->runScript("js/main.js");
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

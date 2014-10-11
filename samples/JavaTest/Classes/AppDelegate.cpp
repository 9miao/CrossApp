#include "AppDelegate.h"
#include "ScriptingCore.h"
#include "generated/jsb_CrossApp_auto.hpp"
#include "CrossApp_specifics.hpp"
#include "CrossAppExt.h"
#include "XMLHTTPRequest.h"
#include "jsb_websocket.h"
USING_NS_CC;
USING_NS_CC_EXT;

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
    pDirector->setAnimationInterval(1.0/60.0f);
//    clock_t t = clock();
//    CCLog("%ld",t);
//    int result = 0;
//    for (int j=0 ; j<5; j++)
//    {
//        for (int i = 0 ; i< 10000;  i++) {
//            result=arc4random()%100;
//        }
//
//    }
//    clock_t t1= clock();
//    CCLog("%ld",t1);//39.8ms
    
//    CAWindow *window = CAWindow::create();
//    
//    CAHyperTextView *view= CAHyperTextView::create();
//    view->setFrame(CCRectMake(0, 0, 640, 960));
//    window->addSubview(view);
//    pDirector->runWindow(window);
    
    
    ScriptingCore* sc = ScriptingCore::getInstance();
    sc->addRegisterCallback(register_all_CrossApp);
    sc->addRegisterCallback(register_CrossApp_js_special);
    sc->addRegisterCallback(MinXmlHttpRequest::_js_register);
    sc->addRegisterCallback(register_jsb_websocket);
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

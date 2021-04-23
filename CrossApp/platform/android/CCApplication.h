#ifndef __CC_APPLICATION_ANDROID_H__
#define __CC_APPLICATION_ANDROID_H__

#include "platform/CACommon.h"
#include "platform/CCApplicationProtocol.h"
#include "ccTypes.h"

NS_CC_BEGIN

class DRect;

class CC_DLL CCApplication : public CCApplicationProtocol
{
public:
    CCApplication();
    virtual ~CCApplication();

    /**
    @brief    Callback by CCDirector to limit FPS.
    @interval       The time, expressed in seconds, between current frame and next. 
    */
    void setAnimationInterval(double interval);

    /**
    @brief    Run the message loop.
    */
    int run();

    /**
    @brief    Get current application instance.
    @return Current application instance pointer.
    */
    static CCApplication* sharedApplication();

    /**
    @brief Get current language config
    @return Current language config
    */
    virtual LanguageType getCurrentLanguage();
    
    /**
     @brief Get target platform
     */
    virtual TargetPlatform getTargetPlatform();
    
    void setStatusBarStyle(const CAStatusBarStyle& var);

protected:
    static CCApplication * sm_pSharedApplication;
    
    CAStatusBarStyle m_eStatusBarStyle;

};

NS_CC_END

#endif    // __CC_APPLICATION_ANDROID_H__

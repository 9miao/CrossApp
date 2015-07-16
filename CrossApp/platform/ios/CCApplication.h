

#ifndef __CC_APPLICATION_IOS_H__
#define __CC_APPLICATION_IOS_H__

#include "platform/CCCommon.h"
#include "platform/CCApplicationProtocol.h"

NS_CC_BEGIN

class CCRect;

class CC_DLL CCApplication : public CCApplicationProtocol
{
public:
    CCApplication();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCApplication();
        
    /**
     @brief    Run the message loop.
     @js NA
     @lua NA
    */
    int run();
        
    /**
    @brief    Get the current application instance.
    @return Current application instance pointer.
    @js getInstance
    */
    static CCApplication* sharedApplication();
    
    /**
     @brief    Callback by CCDirector for limit FPS.
     @interval       The time, expressed in seconds, between current frame and next. 
     @lua NA
     */
    virtual void setAnimationInterval(double interval);

    /**
    @brief Get current language config
    @return Current language config
    */
    virtual ccLanguageType getCurrentLanguage();
    
    /**
     @brief Get target platform
     */
    virtual TargetPlatform getTargetPlatform();

protected:
    static CCApplication * sm_pSharedApplication;
};

NS_CC_END

#endif    // end of __CC_APPLICATION_IOS_H__

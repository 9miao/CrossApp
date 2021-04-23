
#ifndef __CC_APPLICATION_PROTOCOL_H__
#define __CC_APPLICATION_PROTOCOL_H__

#include "basics/CASize.h"

NS_CC_BEGIN


enum TargetPlatform
{
    kTargetWindows,
    kTargetMacOS,
    kTargetAndroid,
    kTargetIphone,
    kTargetIpad
};

/**
 * @addtogroup platform
 * @{
 * @js NA
 * @lua NA
 */

class CC_DLL CCApplicationProtocol
{
public:

    virtual ~CCApplicationProtocol() {}

    virtual bool applicationDidFinishLaunching() = 0;

    virtual void applicationDidEnterBackground() = 0;

    virtual void applicationWillEnterForeground() = 0;
    
    virtual void applicationDidToggleFullScreen(){};
    
    virtual void applicationDidExitFullScreen(){};

    virtual DSize applicationScreenSize(const DSize& screenSize){return DSizeZero;};
    
    virtual void setAnimationInterval(double interval) = 0;

    virtual LanguageType getCurrentLanguage() = 0;

    virtual TargetPlatform getTargetPlatform() = 0;
};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_APPLICATION_PROTOCOL_H__

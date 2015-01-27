#ifndef __CCDEVICE_H__
#define __CCDEVICE_H__

#include "CCPlatformMacros.h"

NS_CC_BEGIN

typedef enum {
    CCDeviceIdiomUnknown = -1,
    
    CCDeviceIdiomPad = 0,
    CCDeviceIdiomPhone = 1
}CCDeviceIdiom;

/**
 @js NA
 @lua NA
 */
class CC_DLL CCDevice
{
private:
    CCDevice();
public:
    /**
     *  Gets the DPI of device
     *  @return The DPI of device.
     */
    static int getDPI();
    
    /**
     *  Gets the User Interface Idiom of device
     *  @return The User Interface Idiom of device.
     *  
     *  0 -- pad 
     *  1 -- phone
     */    
    static CCDeviceIdiom getIdiom();
};


NS_CC_END

#endif /* __CCDEVICE_H__ */

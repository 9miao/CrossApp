

#ifndef __PLATFORM_IPHONE_CCACCELEROMETER_H__
#define __PLATFORM_IPHONE_CCACCELEROMETER_H__

#include "platform/CAAccelerometerDelegate.h"

NS_CC_BEGIN

class CC_DLL CAAccelerometer
{
public:
    CAAccelerometer();
    
    static CAAccelerometer* sharedAccelerometer();
    
    void setDelegate(CAAccelerometerDelegate* pDelegate);
    
    void setAccelerometerInterval(float interval);
    
    void stopAccelerometer();
};

NS_CC_END

#endif

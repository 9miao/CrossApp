

#ifndef __PLATFORM_MAC_CAACCELEROMETER_H__
#define __PLATFORM_MAC_CAACCELEROMETER_H__

#include "platform/CAAccelerometerDelegate.h"

NS_CC_BEGIN

class CC_DLL CAAccelerometer
{
public:
    CAAccelerometer() {}
    ~CAAccelerometer() {}

    void setDelegate(CAAccelerometerDelegate* pDelegate) { CC_UNUSED_PARAM(pDelegate); }
    void setAccelerometerInterval(float interval) {CC_UNUSED_PARAM(interval);};
};

NS_CC_END

#endif

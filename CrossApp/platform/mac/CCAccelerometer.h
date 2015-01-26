

#ifndef __PLATFORM_MAC_CCACCELEROMETER_H__
#define __PLATFORM_MAC_CCACCELEROMETER_H__

#include "platform/CCAccelerometerDelegate.h"

NS_CC_BEGIN

class CC_DLL CCAccelerometer
{
public:
    CCAccelerometer() {}
    ~CCAccelerometer() {}

    void setDelegate(CCAccelerometerDelegate* pDelegate) { CC_UNUSED_PARAM(pDelegate); }
    void setAccelerometerInterval(float interval) {CC_UNUSED_PARAM(interval);};
};

NS_CC_END

#endif

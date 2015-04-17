

#ifndef __PLATFORM_IPHONE_CCACCELEROMETER_H__
#define __PLATFORM_IPHONE_CCACCELEROMETER_H__

#include "platform/CCAccelerometerDelegate.h"

NS_CC_BEGIN

class CC_DLL CCAccelerometer
{
public:
    CCAccelerometer();
    ~CCAccelerometer();

    void setDelegate(CCAccelerometerDelegate* pDelegate);
    void setAccelerometerInterval(float interval);
};

NS_CC_END

#endif

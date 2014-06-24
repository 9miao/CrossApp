

#ifndef __PLATFORM_ANDROID_CCACCELEROMETER_H__
#define __PLATFORM_ANDROID_CCACCELEROMETER_H__

#include "platform/CCCommon.h"
#include "platform/CCAccelerometerDelegate.h"

namespace   CrossApp {

class CC_DLL CCAccelerometer
{
public:
    CCAccelerometer();
    ~CCAccelerometer();

    void setDelegate(CCAccelerometerDelegate* pDelegate);
    void setAccelerometerInterval(float interval);
    void update(float x, float y, float z, long sensorTimeStamp);

private:
    CCAccelerometerDelegate* m_pAccelDelegate;
    CCAcceleration m_obAccelerationValue;
};

}//namespace cocos2d

#endif

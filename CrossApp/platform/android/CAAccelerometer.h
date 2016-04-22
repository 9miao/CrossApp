

#ifndef __PLATFORM_ANDROID_CCACCELEROMETER_H__
#define __PLATFORM_ANDROID_CCACCELEROMETER_H__

#include "platform/CCCommon.h"
#include "platform/CAAccelerometerDelegate.h"

namespace   CrossApp {

class CC_DLL CAAccelerometer
{
public:
	CAAccelerometer();
	~CAAccelerometer();

    void setDelegate(CAAccelerometerDelegate* pDelegate);
    void setAccelerometerInterval(float interval);
    void update(float x, float y, float z, long sensorTimeStamp);

private:
    CAAccelerometerDelegate* m_pAccelDelegate;
    CAAcceleration m_obAccelerationValue;
};

}//namespace cocos2d

#endif

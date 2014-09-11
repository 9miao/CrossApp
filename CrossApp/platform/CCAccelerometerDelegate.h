

#ifndef __CCACCELEROMETER_DELEGATE_H__
#define __CCACCELEROMETER_DELEGATE_H__

#include "CCCommon.h"

NS_CC_BEGIN
/**
@brief The device accelerometer reports values for each axis in units of g-force 
*/
class CCAcceleration
{
public:
    double x;
    double y;
    double z;

    double timestamp;
};

/**
@brief 
The CCAccelerometerDelegate defines a single method for
receiving acceleration-related data from the system.
@js NA
@lua NA
*/
class CC_DLL CCAccelerometerDelegate
{
public:
    virtual void didAccelerate(CCAcceleration* pAccelerationValue) {CC_UNUSED_PARAM(pAccelerationValue);}
};

NS_CC_END

#endif

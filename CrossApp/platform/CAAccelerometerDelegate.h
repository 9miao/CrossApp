

#ifndef __CAACCELEROMETER_DELEGATE_H__
#define __CAACCELEROMETER_DELEGATE_H__

#include "CCCommon.h"

NS_CC_BEGIN
/**
@brief The device accelerometer reports values for each axis in units of g-force 
*/
class CAAcceleration
{
public:
    double x;
    double y;
    double z;

    double timestamp;
};

/**
@brief 
The CAAccelerometerDelegate defines a single method for
receiving acceleration-related data from the system.
@js NA
@lua NA
*/
class CC_DLL CAAccelerometerDelegate
{
public:
    virtual void didAccelerate(CAAcceleration* pAccelerationValue) {CC_UNUSED_PARAM(pAccelerationValue);}
};

NS_CC_END

#endif

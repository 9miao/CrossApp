

#include "CAAccelerometer.h"
#include "AccelerometerDelegateWrapper.h"

NS_CC_BEGIN

static CAAccelerometer* s_pAccelerometer;

CAAccelerometer::CAAccelerometer()
{
}

CAAccelerometer* CAAccelerometer::sharedAccelerometer()
{
    if(s_pAccelerometer==nil)
    {
        s_pAccelerometer = new CAAccelerometer();
    }
    
    return s_pAccelerometer;
}

void CAAccelerometer::setDelegate(CAAccelerometerDelegate* pDelegate)
{
    [[AccelerometerDispatcher sharedAccelerometerDispather] addDelegate:pDelegate];
}

void CAAccelerometer::setAccelerometerInterval(float interval)
{
    [[AccelerometerDispatcher sharedAccelerometerDispather] setAccelerometerInterval:interval];
}

void CAAccelerometer::stopAccelerometer()
{
    [[AccelerometerDispatcher sharedAccelerometerDispather] stopAccelerometerInterval];
    [[AccelerometerDispatcher sharedAccelerometerDispather] release];
}

NS_CC_END


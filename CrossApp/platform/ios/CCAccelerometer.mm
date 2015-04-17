

#include "CCAccelerometer.h"
#include "AccelerometerDelegateWrapper.h"

NS_CC_BEGIN
    
CCAccelerometer::CCAccelerometer()
{
}

CCAccelerometer::~CCAccelerometer()
{
}

void CCAccelerometer::setDelegate(CCAccelerometerDelegate* pDelegate)
{
    [[AccelerometerDispatcher sharedAccelerometerDispather] addDelegate:pDelegate];
}

void CCAccelerometer::setAccelerometerInterval(float interval)
{
    [[AccelerometerDispatcher sharedAccelerometerDispather] setAccelerometerInterval:interval];
}

NS_CC_END


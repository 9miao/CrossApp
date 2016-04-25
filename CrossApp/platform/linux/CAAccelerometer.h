/*
 * CAAccelerometer.h
 *
 *  Created on: Aug 9, 2011
 *      Author: laschweinski
 */

#ifndef CAACCELEROMETER_H_
#define CAACCELEROMETER_H_

#include "platform/CAAccelerometerDelegate.h"

class CAAccelerometer
{
public:
    CAAccelerometer(){}
    ~CAAccelerometer(){}

    static CAAccelerometer* sharedAccelerometer() { return NULL; };

    void removeDelegate(CAAccelerometerDelegate* pDelegate) {CC_UNUSED_PARAM(pDelegate);};
    void addDelegate(CAAccelerometerDelegate* pDelegate) {CC_UNUSED_PARAM(pDelegate);};
    void setDelegate(CAAccelerometerDelegate* pDelegate) {CC_UNUSED_PARAM(pDelegate);};
    void setAccelerometerInterval(float interval) {CC_UNUSED_PARAM(interval);};
};


#endif /* CAACCELEROMETER_H_ */

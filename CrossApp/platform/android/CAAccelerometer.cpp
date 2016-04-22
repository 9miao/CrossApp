
#include "CAAccelerometer.h"
#include "jni/Java_org_CrossApp_lib_CrossAppHelper.h"
#include <stdio.h>
#include <android/log.h>

#define TG3_GRAVITY_EARTH                    (9.80665f)
#define  LOG_TAG    "CAAccelerometer_android"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

namespace CrossApp
{
	CAAccelerometer::CAAccelerometer() : m_pAccelDelegate(NULL)
    {
    }

	CAAccelerometer::~CAAccelerometer()
    {

    }

	void CAAccelerometer::setDelegate(CAAccelerometerDelegate* pDelegate)
    {
        m_pAccelDelegate = pDelegate;

        if (pDelegate)
        {        
            enableAccelerometerJNI();
        }
        else
        {
            disableAccelerometerJNI();
        }
    }

	void CAAccelerometer::setAccelerometerInterval(float interval)
    {
        setAccelerometerIntervalJNI(interval);
    }


	void CAAccelerometer::update(float x, float y, float z, long sensorTimeStamp)
    {
        if (m_pAccelDelegate)
        {
            m_obAccelerationValue.x = -((double)x / TG3_GRAVITY_EARTH);
            m_obAccelerationValue.y = -((double)y / TG3_GRAVITY_EARTH);
            m_obAccelerationValue.z = -((double)z / TG3_GRAVITY_EARTH);
            m_obAccelerationValue.timestamp = (double)sensorTimeStamp;

            m_pAccelDelegate->didAccelerate(&m_obAccelerationValue);
        }    
    }
} // end of namespace cococs2d


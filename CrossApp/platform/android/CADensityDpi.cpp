
#include "platform/CADensityDpi.h"
#include "jni/JniHelper.h"
#include <jni.h>

NS_CC_BEGIN
float CADensityDpi::getDensityDpi()
{
    static float dpi = -1;
    if (dpi == -1)
    {
        dpi = (int)getDPIJNI();
    }
}

CADeviceIdiom CADensityDpi::getIdiom()
{
    return CADeviceIdiomPhone;
}

NS_CC_END
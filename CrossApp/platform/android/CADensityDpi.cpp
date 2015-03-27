
#include "platform/CADensityDpi.h"
#include "jni/JniHelper.h"
#include "jni/DPIJni.h"
#include <jni.h>

NS_CC_BEGIN
float CADensityDpi::getDensityDpi()
{
    static float dpi = -1;
    if (dpi == -1)
    {
        dpi = getDPIJNI();
    }
    return 320;
}

CADeviceIdiom CADensityDpi::getIdiom()
{
    return CADeviceIdiomPhone;
}

NS_CC_END
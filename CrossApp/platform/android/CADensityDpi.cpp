
#include "platform/CADensityDpi.h"
#include "jni/JniHelper.h"
#include <jni.h>

NS_CC_BEGIN

static float s_fDPI = -1.f;

void CADensityDpi::setDensityDpi(float dpi)
{
    s_fDPI = dpi;
}

float CADensityDpi::getDensityDpi()
{
    if (s_fDPI != -1.f)
    {
        return s_fDPI;
    }
    
    JniMethodInfo jni;
    if(!JniHelper::getStaticMethodInfo(jni , "org/CrossApp/lib/CrossAppHelper" , "getDPI" , "()F"))
    {
        return DPI_STANDARD;
    }
    
    s_fDPI = (float)(jfloat)jni.env->CallStaticFloatMethod(jni.classID , jni.methodID);
    jni.env->DeleteLocalRef(jni.classID);

    return s_fDPI;
}

CADeviceIdiom CADensityDpi::getIdiom()
{
    return CADeviceIdiom::iPhone;
}

NS_CC_END

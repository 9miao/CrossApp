
#include "platform/CADensityDpi.h"
#include "jni/JniHelper.h"
#include <jni.h>

NS_CC_BEGIN
float CADensityDpi::getDensityDpi()
{
    float relust = 326;
    
	JniMethodInfo jni;
	if(JniHelper::getStaticMethodInfo(jni , "org/CrossApp/lib/Cocos2dxActivity" , "getDensitDpi" , "()F"))
	{
		jfloat relustJava = (jfloat)jni.env->CallStaticFloatMethod(jni.classID , jni.methodID);
        relust = relustJava;
	}

    return relust;
}
NS_CC_END
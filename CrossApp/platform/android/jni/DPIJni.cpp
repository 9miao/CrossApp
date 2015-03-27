#include "DPIJni.h"
#include "jni/JniHelper.h"

USING_NS_CC;

extern "C" {

float getDPIJNI()
{
    JniMethodInfo t;
    jint ret = -1;
    if (JniHelper::getStaticMethodInfo(t, "org/CrossApp/lib/Cocos2dxHelper", "getDPI", "()F")) {
        ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

} // extern "C"
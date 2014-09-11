
#include "IMEJni.h"
#include "dispatcher/CAIMEDispatcher.h"
#include "JniHelper.h"
#include <android/log.h>
#include <string.h>
#include <jni.h>

using namespace CrossApp;

extern "C" {
    void setKeyboardStateJNI(int bOpen) {
        if (bOpen) {
            openKeyboardJNI();
        } else {
            closeKeyboardJNI();
        }
    }
    void  openKeyboardJNI()
    {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "org/CrossApp/lib/Cocos2dxGLSurfaceView", "openIMEKeyboard", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }
    void setKeyboardType(int type)
    {
        JniMethodInfo t;
        jobject jobj;
        bool isHave;
        if (JniHelper::getStaticMethodInfo(t, "org/CrossApp/lib/Cocos2dxGLSurfaceView", "getInstance", "()Lorg/CrossApp/lib/Cocos2dxGLSurfaceView;")) {
            jobj=t.env->CallStaticObjectMethod(t.classID, t.methodID);
            isHave = JniHelper::getMethodInfo(t,
                                              "org/CrossApp/lib/Cocos2dxGLSurfaceView",
                                              "setKeyBoardType",
                                              "(I)V");
            if (isHave) {
                t.env->CallVoidMethod(jobj, t.methodID,type);
            }
            t.env->DeleteLocalRef(t.classID);
        }
    }
    
    
    void setKeyboardReturnType(int type)
    {
        JniMethodInfo t;
        jobject jobj;
        bool isHave;
        if (JniHelper::getStaticMethodInfo(t, "org/CrossApp/lib/Cocos2dxGLSurfaceView", "getInstance", "()Lorg/CrossApp/lib/Cocos2dxGLSurfaceView;")) {
            jobj=t.env->CallStaticObjectMethod(t.classID, t.methodID);
            isHave = JniHelper::getMethodInfo(t,
                                              "org/CrossApp/lib/Cocos2dxGLSurfaceView",
                                              "setKeyBoardRetrunType",
                                              "(I)V");
            if (isHave) {
                t.env->CallVoidMethod(jobj, t.methodID,type);
            }
            t.env->DeleteLocalRef(t.classID);
        }
    }
    
    
    void closeKeyboardJNI() {
        JniMethodInfo t;

        if (JniHelper::getStaticMethodInfo(t, "org/CrossApp/lib/Cocos2dxGLSurfaceView", "closeIMEKeyboard", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

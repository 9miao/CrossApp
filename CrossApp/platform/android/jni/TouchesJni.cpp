
#include "cocoa/CCSet.h"
#include "basics/CAApplication.h"
#include "dispatcher/CAKeypadDispatcher.h"
#include "dispatcher/CATouch.h"
#include "dispatcher/CATouchDispatcher.h"
#include <android/log.h>
#include <jni.h>
#include "platform/android/CCEGLView.h"

using namespace CrossApp;

extern "C" {
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeTouchesBegin(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y)
    {
        CAApplication::getApplication()->getOpenGLView()->handleTouchesBegin(1, &id, &x, &y);
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeTouchesEnd(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y)
    {
        CAApplication::getApplication()->getOpenGLView()->handleTouchesEnd(1, &id, &x, &y);
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeTouchesMove(JNIEnv * env, jobject thiz, jintArray ids, jfloatArray xs, jfloatArray ys) {
        int size = env->GetArrayLength(ids);
        jint id[size];
        jfloat x[size];
        jfloat y[size];

        env->GetIntArrayRegion(ids, 0, size, id);
        env->GetFloatArrayRegion(xs, 0, size, x);
        env->GetFloatArrayRegion(ys, 0, size, y);

        CAApplication::getApplication()->getOpenGLView()->handleTouchesMove(size, id, x, y);
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeTouchesCancel(JNIEnv * env, jobject thiz, jintArray ids, jfloatArray xs, jfloatArray ys) {
        int size = env->GetArrayLength(ids);
        jint id[size];
        jfloat x[size];
        jfloat y[size];

        env->GetIntArrayRegion(ids, 0, size, id);
        env->GetFloatArrayRegion(xs, 0, size, x);
        env->GetFloatArrayRegion(ys, 0, size, y);

        CAApplication::getApplication()->getOpenGLView()->handleTouchesCancel(size, id, x, y);
    }

    #define KEYCODE_BACK 0x04
    #define KEYCODE_MENU 0x52

    JNIEXPORT jboolean JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeKeyDown(JNIEnv * env, jobject thiz, jint keyCode) {
        CAApplication* pDirector = CAApplication::getApplication();
        switch (keyCode) {
            case KEYCODE_BACK:
                  if (pDirector->getKeypadDispatcher()->dispatchKeypadMSG(kTypeBackClicked))
                    return JNI_TRUE;
                break;
            case KEYCODE_MENU:
                if (pDirector->getKeypadDispatcher()->dispatchKeypadMSG(kTypeMenuClicked))
                    return JNI_TRUE;
                break;
            default:
                return JNI_FALSE;
        }
        return JNI_FALSE;
    }
}

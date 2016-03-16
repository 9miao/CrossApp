
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
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeTouchesBegin(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y)
    {
        CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
        theEvent->setEventType(CrossApp::EventType::androidEvent);
        CAApplication::getApplication()->getOpenGLView()->handleTouchesBegin(1, &id, &x, &y, theEvent);
        theEvent->release();
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeTouchesEnd(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y)
    {
        CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
        theEvent->setEventType(CrossApp::EventType::androidEvent);
        CAApplication::getApplication()->getOpenGLView()->handleTouchesEnd(1, &id, &x, &y, theEvent);
        theEvent->release();
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeTouchesMove(JNIEnv * env, jobject thiz, jintArray ids, jfloatArray xs, jfloatArray ys) {
        int size = env->GetArrayLength(ids);
        jint id[size];
        jfloat x[size];
        jfloat y[size];

        env->GetIntArrayRegion(ids, 0, size, id);
        env->GetFloatArrayRegion(xs, 0, size, x);
        env->GetFloatArrayRegion(ys, 0, size, y);

        CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
        theEvent->setEventType(CrossApp::EventType::androidEvent);
        CAApplication::getApplication()->getOpenGLView()->handleTouchesMove(size, id, x, y, theEvent);
        theEvent->release();
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeTouchesCancel(JNIEnv * env, jobject thiz, jintArray ids, jfloatArray xs, jfloatArray ys) {
        int size = env->GetArrayLength(ids);
        jint id[size];
        jfloat x[size];
        jfloat y[size];

        env->GetIntArrayRegion(ids, 0, size, id);
        env->GetFloatArrayRegion(xs, 0, size, x);
        env->GetFloatArrayRegion(ys, 0, size, y);

        CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
        theEvent->setEventType(CrossApp::EventType::androidEvent);
        CAApplication::getApplication()->getOpenGLView()->handleTouchesCancel(size, id, x, y, theEvent);
        theEvent->release();
    }

    #define KEYCODE_BACK 0x04
    #define KEYCODE_MENU 0x52

    JNIEXPORT jboolean JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeKeyDown(JNIEnv * env, jobject thiz, jint keyCode) {
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

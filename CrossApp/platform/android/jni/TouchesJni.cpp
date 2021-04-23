
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
        intptr_t ids[1] = {0};      static_cast<intptr_t>(id);
        float x_s[1] = {0};         x_s[0] = x;
        float y_s[1] = {0};        y_s[0] = y;

        CAApplication::getApplication()->getOpenGLView()->handleTouchesBegin(1, ids, x_s, y_s, theEvent);
        theEvent->release();
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeTouchesEnd(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y)
    {
        CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
        theEvent->setEventType(CrossApp::EventType::androidEvent);
        
        intptr_t ids[1] = {0};      ids[0] = static_cast<intptr_t>(id);
        float x_s[1] = {0};         x_s[0] = x;
        float y_s[1] = {0};        y_s[0] = y;
        
        CAApplication::getApplication()->getOpenGLView()->handleTouchesEnd(1, ids, x_s, y_s, theEvent);
        theEvent->release();
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeTouchesMove(JNIEnv * env, jobject thiz, jintArray id_array, jfloatArray xs, jfloatArray ys) {
        int size = env->GetArrayLength(id_array);
        jint id[size];
        jfloat x[size];
        jfloat y[size];

        env->GetIntArrayRegion(id_array, 0, size, id);
        env->GetFloatArrayRegion(xs, 0, size, x);
        env->GetFloatArrayRegion(ys, 0, size, y);

        intptr_t ids[size];
        float x_s[size];
        float y_s[size];
        for (int i=0; i<size; i++)
        {
            ids[i] = static_cast<intptr_t>(id[i]);
            x_s[i] = x[i];
            y_s[i] = y[i];
        }
        
        CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
        theEvent->setEventType(CrossApp::EventType::androidEvent);
        CAApplication::getApplication()->getOpenGLView()->handleTouchesMove(size, ids, x_s, y_s, theEvent);
        theEvent->release();
        
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeTouchesCancel(JNIEnv * env, jobject thiz, jintArray id_array, jfloatArray xs, jfloatArray ys) {
        int size = env->GetArrayLength(id_array);
        jint id[size];
        jfloat x[size];
        jfloat y[size];

        env->GetIntArrayRegion(id_array, 0, size, id);
        env->GetFloatArrayRegion(xs, 0, size, x);
        env->GetFloatArrayRegion(ys, 0, size, y);

        intptr_t ids[size];
        float x_s[size];
        float y_s[size];
        for (int i=0; i<size; i++)
        {
            ids[i] = static_cast<intptr_t>(id[i]);
            x_s[i] = x[i];
            y_s[i] = y[i];
        }
        
        CrossApp::CAEvent* theEvent = new CrossApp::CAEvent();
        theEvent->setEventType(CrossApp::EventType::androidEvent);
        CAApplication::getApplication()->getOpenGLView()->handleTouchesCancel(size, ids, x_s, y_s, theEvent);
        theEvent->release();
        
    }

    #define KEYCODE_BACK 0x04
    #define KEYCODE_MENU 0x52

    JNIEXPORT jboolean JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeKeyDown(JNIEnv * env, jobject thiz, jint keyCode) {
        CAApplication* application = CAApplication::getApplication();
        switch (keyCode) {
            case KEYCODE_BACK:
                if (application->getKeypadDispatcher()->dispatchKeypadMSG(CAKeypadDispatcher::KeypadMSGType::BackClicked))
                    return JNI_TRUE;
                break;
            case KEYCODE_MENU:
                if (application->getKeypadDispatcher()->dispatchKeypadMSG(CAKeypadDispatcher::KeypadMSGType::MenuClicked))
                    return JNI_TRUE;
                break;
            default:
                return JNI_FALSE;
        }
        return JNI_FALSE;
    }
}

#include "basics/CAApplication.h"
#include "platform/CAFileUtils.h"
#include "basics/CANotificationCenter.h"
#include "platform/android/CCApplication.h"
#include "platform/android/CCEGLView.h"
#include "JniHelper.h"
#include <jni.h>

using namespace CrossApp;

extern "C" {
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeRender(JNIEnv* env) {
        CAApplication::getApplication()->mainLoop();
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeOnPause() {
        CCApplication::sharedApplication()->applicationDidEnterBackground();

        CANotificationCenter::getInstance()->postNotification(EVENT_COME_TO_BACKGROUND);
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeOnResume() {
        if (CAApplication::getApplication()->getOpenGLView()) {
            CCApplication::sharedApplication()->applicationWillEnterForeground();
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeChanged(JNIEnv*  env, jobject thiz, jint w, jint h) {
        if (CAApplication::getApplication()->getOpenGLView())
        {
            CCEGLView::sharedOpenGLView()->setFrameSize((float)(int)w, (float)(int)h);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeChangedOrientation(JNIEnv*  env, jobject thiz, jint type) {
        CCEGLView::sharedOpenGLView()->setStatusBarOrientation((CAInterfaceOrientation)((int)type));
    }

}

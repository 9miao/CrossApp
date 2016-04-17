#include "basics/CAApplication.h"
#include "platform/CCFileUtils.h"
#include "ccTypes.h"
#include "support/CANotificationCenter.h"
#include "JniHelper.h"
#include "platform/android/CCApplication.h"
#include <jni.h>

using namespace CrossApp;

extern "C" {
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeRender(JNIEnv* env) {
        CrossApp::CAApplication::getApplication()->mainLoop();
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeOnPause() {
        CCApplication::sharedApplication()->applicationDidEnterBackground();

        CANotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_BACKGROUND, NULL);
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeOnResume() {
        if (CrossApp::CAApplication::getApplication()->getOpenGLView()) {
            CCApplication::sharedApplication()->applicationWillEnterForeground();
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeChanged(JNIEnv*  env, jobject thiz, jint w, jint h) {
        if (CrossApp::CAApplication::getApplication()->getOpenGLView())
        {
            CrossApp::CCEGLView::sharedOpenGLView()->setFrameSize((int)w, (int)h);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppRenderer_nativeChangedOrientation(JNIEnv*  env, jobject thiz, jint type) {
        CrossApp::CCEGLView::sharedOpenGLView()->setStatusBarOrientation((CAInterfaceOrientation)((int)type));
    }

}

#include "basics/CAApplication.h"
#include "platform/CCFileUtils.h"
#include "ccTypes.h"
#include "support/CANotificationCenter.h"
#include "JniHelper.h"
#include "platform/android/CCApplication.h"
#include <jni.h>

using namespace CrossApp;

extern "C" {
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeRender(JNIEnv* env) {
        CrossApp::CAApplication::getApplication()->mainLoop();
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeOnPause() {
        CCApplication::sharedApplication()->applicationDidEnterBackground();

        CANotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_BACKGROUND, NULL);
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeOnResume() {
        if (CrossApp::CAApplication::getApplication()->getOpenGLView()) {
            CCApplication::sharedApplication()->applicationWillEnterForeground();
        }
    }

}

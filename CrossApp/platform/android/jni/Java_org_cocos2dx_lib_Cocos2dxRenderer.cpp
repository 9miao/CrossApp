#include "dispatcher/CAIMEDispatcher.h"
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

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeInsertText(JNIEnv* env, jobject thiz, jstring text) {
        const char* pszText = env->GetStringUTFChars(text, NULL);
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchInsertText(pszText, strlen(pszText));
        env->ReleaseStringUTFChars(text, pszText);
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeAndroidWillInsertText(JNIEnv* env, jobject thiz, jint text, jstring str, jint text1, jint text2) {

        const char* pszText = env->GetStringUTFChars(str, NULL);
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchAndroidWillInsertText(text,pszText,text1,text2);
        env->ReleaseStringUTFChars(str, pszText);
       // env->ReleaseStringUTFChars(text, pszText);
    }

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeDeleteBackward(JNIEnv* env, jobject thiz) {
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
    }

    JNIEXPORT jstring JNICALL Java_org_CrossApp_lib_Cocos2dxRenderer_nativeGetContentText() {
        JNIEnv * env = 0;

        if (JniHelper::getJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK || ! env) {
            return 0;
        }
        const char * pszText = CrossApp::CAIMEDispatcher::sharedDispatcher()->getContentText();
        return env->NewStringUTF(pszText);
    }
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxActivity_KeyBoardHeightReturn(JNIEnv* env, jobject thiz, jint height) {
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchGetKeyBoardHeight(height);
    }
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxTextInputWraper_KeyBoardReturnCallBack(JNIEnv* env, jobject thiz) {
        CrossApp::CAIMEDispatcher::sharedDispatcher()->dispatchGetKeyBoradReturnCallBack();
    }
}

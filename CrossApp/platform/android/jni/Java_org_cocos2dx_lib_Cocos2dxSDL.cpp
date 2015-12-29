#include "basics/CAApplication.h"
#include "platform/CCFileUtils.h"
#include "ccTypes.h"
#include "support/CANotificationCenter.h"
#include "JniHelper.h"
#include "platform/android/CCApplication.h"
#include <jni.h>
#include "SDL_main.h"
	
using namespace CrossApp;

extern "C" {
	
	extern void SDL_Android_Init(JNIEnv* env, jclass cls);
	extern jint SDL_JNI_OnLoad(JavaVM* vm, void* reserved);
	extern int Android_JNI_SetupThread(void);
	
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxSDL_nativeInit(JNIEnv* env, jclass cls, jobject obj) {
        int ret = SDL_JNI_OnLoad(JniHelper::getJavaVM(), NULL);
        SDL_Android_Init(env, cls);
    }
}
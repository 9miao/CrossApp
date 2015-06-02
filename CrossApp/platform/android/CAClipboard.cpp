
#include "platform/CAClipboard.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "basics/CAApplication.h"


NS_CC_BEGIN

extern "C"
{

	char* jstringTostring(JNIEnv* env, jstring jstr)
	{
		char* rtn = NULL;
		jclass clsstring = env->FindClass("java/lang/String");
		jstring strencode = env->NewStringUTF("utf-8");
		jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
		jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
		jsize alen = env->GetArrayLength(barr);
		jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
		if (alen > 0)
		{
			rtn = (char*)malloc(alen + 1);
			memcpy(rtn, ba, alen);
			rtn[alen] = 0;
		}
		env->ReleaseByteArrayElements(barr, ba, 0);
		return rtn;
	}



	void JAVAsetPasteBoardString(const char* str)
	{

		JniMethodInfo jmi;
		if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/Cocos2dxActivity", "getContext", "()Lorg/CrossApp/lib/Cocos2dxActivity;"))
		{
			jobject obj = jmi.env->CallStaticObjectMethod(jmi.classID, jmi.methodID);
			bool isHave = JniHelper::getMethodInfo(jmi, "org/CrossApp/lib/Cocos2dxActivity", "setPasteBoardStr", "(Ljava/lang/String;)V");
			if (isHave)
			{
				jmi.env->CallVoidMethod(obj, jmi.methodID, jmi.env->NewStringUTF(str));
			}


		}
	}

	const char* JAVAgetPasteBoardString()
	{
		JniMethodInfo jmi;
		if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/Cocos2dxActivity", "getContext", "()Lorg/CrossApp/lib/Cocos2dxActivity;"))
		{
			jobject obj = jmi.env->CallStaticObjectMethod(jmi.classID, jmi.methodID);
			bool isHave = JniHelper::getMethodInfo(jmi, "org/CrossApp/lib/Cocos2dxActivity", "getPasteBoardStr", "()Ljava/lang/String;");
			if (isHave)
			{
				jstring a = (jstring)jmi.env->CallObjectMethod(obj, jmi.methodID);
				const char *str = jstringTostring(jmi.env, a);
				return str;
			}
		}
		return "";
	}
}

std::string CAClipboard::getText()
{
    const char *str = JAVAgetPasteBoardString();
    
    return str ? str : "";
}

void CAClipboard::setText(const std::string& cszStrText)
{
	JAVAsetPasteBoardString(cszStrText.c_str());
}

NS_CC_END
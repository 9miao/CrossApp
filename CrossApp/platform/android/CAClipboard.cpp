
#include "platform/CAClipboard.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "basics/CAApplication.h"


NS_CC_BEGIN

extern "C"
{
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

	std::string JAVAgetPasteBoardString()
	{
		JniMethodInfo jmi; std::string cszResult;
		if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/Cocos2dxActivity", "getContext", "()Lorg/CrossApp/lib/Cocos2dxActivity;"))
		{
			jobject obj = jmi.env->CallStaticObjectMethod(jmi.classID, jmi.methodID);
			bool isHave = JniHelper::getMethodInfo(jmi, "org/CrossApp/lib/Cocos2dxActivity", "getPasteBoardStr", "()Ljava/lang/String;");
			if (isHave)
			{
				jstring a = (jstring)jmi.env->CallObjectMethod(obj, jmi.methodID);
				cszResult = JniHelper::jstring2string(a);
			}
		}
		return cszResult;
	}
}

std::string CAClipboard::getText()
{
	return JAVAgetPasteBoardString();
}

void CAClipboard::setText(const std::string& cszStrText)
{
	JAVAsetPasteBoardString(cszStrText.c_str());
}

NS_CC_END
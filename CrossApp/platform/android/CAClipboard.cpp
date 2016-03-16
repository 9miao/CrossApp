
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
		if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/CrossAppActivity", "getContext", "()Lorg/CrossApp/lib/CrossAppActivity;"))
		{
			jobject obj = jmi.env->CallStaticObjectMethod(jmi.classID, jmi.methodID);
			bool isHave = JniHelper::getMethodInfo(jmi, "org/CrossApp/lib/CrossAppActivity", "setPasteBoardStr", "(Ljava/lang/String;)V");
			if (isHave)
			{
				jstring strArg = jmi.env->NewStringUTF(str);
				jmi.env->CallVoidMethod(obj, jmi.methodID, strArg);
				jmi.env->DeleteLocalRef(strArg);
			}
			jmi.env->DeleteLocalRef(obj);
			jmi.env->DeleteLocalRef(jmi.classID);
		}
	}

	std::string JAVAgetPasteBoardString()
	{
		std::string cszResult;
		JniMethodInfo jmi;
		if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/CrossAppActivity", "getContext", "()Lorg/CrossApp/lib/CrossAppActivity;"))
		{
			jobject obj = jmi.env->CallStaticObjectMethod(jmi.classID, jmi.methodID);
			bool isHave = JniHelper::getMethodInfo(jmi, "org/CrossApp/lib/CrossAppActivity", "getPasteBoardStr", "()Ljava/lang/String;");
			if (isHave)
			{
				jstring a = (jstring)jmi.env->CallObjectMethod(obj, jmi.methodID);
				cszResult = JniHelper::jstring2string(a);
				jmi.env->DeleteLocalRef(a);
			}
			jmi.env->DeleteLocalRef(obj);
			jmi.env->DeleteLocalRef(jmi.classID);
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

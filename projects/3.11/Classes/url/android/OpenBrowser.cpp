/*
 * OpenBrowser.cpp
 *
 *  Created on: 2013-8-20
 *      Author: yangpichen
 */

#include "OpenBrowser.h"
#include "platform/android/jni/JniHelper.h"

void OpenBrowser::openUrl(CCString* strUrl) {
	const char* class_name = "com/component/impl/WebViewCtrl";
	JniMethodInfo info;
	jobject dthero;
	if (JniHelper::getStaticMethodInfo(info, class_name,
			"getInstance", "()Lcom/component/impl/WebViewCtrl;")) {
		dthero = info.env->CallStaticObjectMethod(info.classID, info.methodID);
	}
	if (dthero == NULL) {
		CCLog("activity null!");
		return;
	}
	if (JniHelper::getMethodInfo(info, class_name,
			"browserOpenURL", "(Ljava/lang/String;)V")) {
		info.env->CallVoidMethod(dthero, info.methodID,
				info.env->NewStringUTF(strUrl->getCString()));
	}
}

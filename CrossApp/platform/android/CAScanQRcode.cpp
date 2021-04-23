//
//  CAScanQRcode.cpp
//  CrossApp
//
//  Created by mac on 2021/3/12.
//  Copyright © 2021 cocos2d-x. All rights reserved.
//

#include "platform/CAScanQRcode.h"
#include "basics/CAApplication.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

static std::function<void(const std::string&)> s_ResultScanning_callback = nullptr;

extern "C"
{
    // 扫描二维码结果
    void Java_org_CrossApp_lib_CameraView_resultScanning(JNIEnv *env, jobject thiz, jstring value)
    {
        const char* str = env->GetStringUTFChars(value, NULL);
        if (s_ResultScanning_callback) s_ResultScanning_callback(str);
        s_ResultScanning_callback = nullptr;
        env->ReleaseStringUTFChars(value, str);
    }
    // 摄像头关闭
    void Java_org_CrossApp_lib_CameraView_cameraClosed(JNIEnv *env, jobject thiz)
    {
        CrossApp::CAApplication::getApplication()->resume();
    }
}

NS_CC_BEGIN

void CAScanQRcode::showScanQRcode(const std::function<void(const std::string&)>& callback)
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info, "org/CrossApp/lib/CrossAppActivity", "showQRCodeView", "()V");
    if (ret)
    {
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        s_ResultScanning_callback = callback;
    }
    CrossApp::CAApplication::getApplication()->pause();
}
NS_CC_END




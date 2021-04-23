//
//  CADevice.cpp
//  CrossApp
//
//  Created by 秦乐 on 2017/2/21.
//  Copyright © 2017年 cocos2d-x. All rights reserved.
//

#include "../CADevice.h"
#include "basics/CAScheduler.h"
#include <map>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

NS_CC_BEGIN


static std::map<std::string , std::function<void(bool isSuccess)> > s_browserOpenURLCallBack_map;

extern "C"
{
    const char* JAVAGetSystemVersion()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getSystemVersion" , "()Ljava/lang/String;"))
        {
            jstring a = (jstring)jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
            const char* b = jmi.env->GetStringUTFChars( a , 0 );
            jmi.env->DeleteLocalRef(jmi.classID);
            return b;
        }
    }
    
    const char* JAVAGetAppVersion()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getAppVersion" , "()Ljava/lang/String;"))
        {
            jstring a = (jstring)jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
            const char* b = jmi.env->GetStringUTFChars( a , 0 );
            jmi.env->DeleteLocalRef(jmi.classID);
            return b;
        }
    }
    
    void JAVASetScreenBrightness(int sender)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "setScreenBrightness" , "(I)V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,sender);
            jmi.env->DeleteLocalRef(jmi.classID);
        }
    }
    
    float JAVAgetBrightness()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getScreenBrightness" , "()I"))
        {
            jint nBrightness = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            int brightness = nBrightness;
            jmi.env->DeleteLocalRef(jmi.classID);
            return (float)brightness / 255.0f;
        }
    }
    
    CADevice::NetWorkData JAVAgetNetWorkType()
    {
        CADevice::NetWorkData networkType = CADevice::NetWorkData::None;
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getAPNType" , "()I"))
        {
            jint a = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            switch (a)
            {
                case 0:
                    networkType = CADevice::NetWorkData::None;
                    break;
                case 1:
                    networkType = CADevice::NetWorkData::Wifi;
                    break;
                case 2:
                case 3:
                    networkType = CADevice::NetWorkData::ReachableViaWWAN;
                    break;
            }
        }
        jmi.env->DeleteLocalRef(jmi.classID);
        return networkType;
    }
    
    bool JAVAisNetWorkAvailble()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "isNetWorkAvailble" , "()I"))
        {
            jint a = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            int b = (int)a;
            jmi.env->DeleteLocalRef(jmi.classID);
            return (bool)b;
        }
    }
    
    void JAVAsetVolume(float sender,int type)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "setVolum" , "(FI)V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,sender,type);
            jmi.env->DeleteLocalRef(jmi.classID);
        }
    }
    
    float JAVAgetVolume(int type)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getVolum" , "(I)F"))
        {
            jfloat a = (jfloat)jmi.env->CallStaticFloatMethod(jmi.classID , jmi.methodID,type);
            float b = a;
            jmi.env->DeleteLocalRef(jmi.classID);
            return b;
        }
    }
    
    float JAVAgetBattery()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getBatteryLevel" , "()I"))
        {
            jint a = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            int b = a;
            jmi.env->DeleteLocalRef(jmi.classID);
            return (float)b/100.0f;
        }
    }
    
    void JAVAsendLocalNotification(const char* title, const char* content, int leftMessage)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "sendLocalNotification" , "(Ljava/lang/String;Ljava/lang/String;I)V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,jmi.env->NewStringUTF(title),jmi.env->NewStringUTF(content),time);
            jmi.env->DeleteLocalRef(jmi.classID);
        }
    }
    
    void JAVAOpenUrl(const std::string &url, const std::function<void(bool)>& callback)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "browserOpenURL" , "(Ljava/lang/String;)V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,jmi.env->NewStringUTF(url.c_str()));
            jmi.env->DeleteLocalRef(jmi.classID);
        }

        if (callback)
        {
            s_browserOpenURLCallBack_map[url]=  callback;
        }
    }
    
    void JAVASetIdleTimerDisabled(int type)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "setIdleTimerDisabled" , "(I)V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,type);
            jmi.env->DeleteLocalRef(jmi.classID);
        }
    }
}

#endif

const CAValueMap& CADevice::getSystemVersion()
{
    CAValueMap valueMap;
    valueMap["os"] = "Android";
    valueMap["version"] = JAVAGetSystemVersion();
    return valueMap;
}

const std::string& CADevice::getAppVersion()
{
    return JAVAGetAppVersion();
}

void CADevice::setScreenBrightness(float brightness)
{
    int sender = (int)(brightness*255.0f);
    
    JAVASetScreenBrightness(sender);
}

float CADevice::getScreenBrightness()
{
    return JAVAgetBrightness();
}

CADevice::NetWorkData CADevice::getNetWorkType()
{
    return JAVAgetNetWorkType();
}

bool CADevice::isNetWorkAvailble()
{
    return JAVAisNetWorkAvailble();
}

void CADevice::setVolume(float sender, CADevice::VolumeData type)
{
    int mType = (int)type;
    JAVAsetVolume(sender, mType);
}

float CADevice::getVolume(CADevice::VolumeData type)
{
    int mType = (int)type;
    JAVAgetVolume(mType);
}

float CADevice::getBatteryLevel()
{
    return JAVAgetBattery();
}

void CADevice::sendLocalNotification(const char* title, const char* content, int leftMessage)
{
    JAVAsendLocalNotification(title, content, leftMessage);
}

void CADevice::openUrl(const std::string &url, const std::function<void(bool)>& callback)
{
    JAVAOpenUrl(url, callback);
}

void CADevice::setIdleTimerDisabled(bool isIdleTimerDisabled)
{
    int type = isIdleTimerDisabled ? 1 : 0;
    JAVASetIdleTimerDisabled(type);
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppNativeTool_browserOpenURLCallBack
    ( JNIEnv* env,jclass thiz ,jstring url,jboolean isSuccess)
    {
        const char* str = env->GetStringUTFChars(url, NULL);
        
        CAScheduler::getScheduler()->performFunctionInUIThread( [=]()
                                                               {
                                                                   if (s_browserOpenURLCallBack_map.count(str))
                                                                   {
                                                                       s_browserOpenURLCallBack_map[str](isSuccess == JNI_TRUE);
                                                                       s_browserOpenURLCallBack_map.erase(str);
                                                                    
                                                                   }
                                                                
                                                               });
    }
}

NS_CC_END

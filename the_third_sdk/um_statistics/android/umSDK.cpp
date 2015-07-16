//
//  umSDK.cpp
//  HelloCpp
//
//  Created by 栗元峰 on 14-11-14.
//
//

#include "../umSDK.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

namespace umSDK
{
    void startWithAppkey(const char* appkey, const char* channel)
    {
        JniMethodInfo jni;
        if (JniHelper::getStaticMethodInfo(jni, "com/umeng/AnalyticsHome", "startWithAppkey", "(Ljava/lang/String;Ljava/lang/String;)V"))
        {
            jstring j_appkey = jni.env->NewStringUTF(appkey);
            jstring j_channel = jni.env->NewStringUTF(channel);
            jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, j_appkey, j_channel);
            jni.env->DeleteLocalRef(jni.classID);
            jni.env->DeleteLocalRef(j_appkey);
            jni.env->DeleteLocalRef(j_channel);
        }
    }
    
    void onPageStart(const char* viewID)
    {
        JniMethodInfo jni;
        if (JniHelper::getStaticMethodInfo(jni, "com/umeng/AnalyticsHome", "onPageStart", "(Ljava/lang/String;)V"))
        {
            jstring j_viewID = jni.env->NewStringUTF(viewID);
            jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, j_viewID);
            jni.env->DeleteLocalRef(jni.classID);
            jni.env->DeleteLocalRef(j_viewID);
        }
    }
    
    void onPageEnd(const char* viewID)
    {
        JniMethodInfo jni;
        if (JniHelper::getStaticMethodInfo(jni, "com/umeng/AnalyticsHome", "onPageEnd", "(Ljava/lang/String;)V"))
        {
            jstring j_viewID = jni.env->NewStringUTF(viewID);
            jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, j_viewID);
            jni.env->DeleteLocalRef(jni.classID);
            jni.env->DeleteLocalRef(j_viewID);
        }
    }
    
    void onEventBegin(const char* eventId)
    {
        JniMethodInfo jni;
        if (JniHelper::getStaticMethodInfo(jni, "com/umeng/AnalyticsHome", "onEventBegin", "(Ljava/lang/String;)V"))
        {
            jstring j_eventId = jni.env->NewStringUTF(eventId);
            jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, j_eventId);
            jni.env->DeleteLocalRef(jni.classID);
            jni.env->DeleteLocalRef(j_eventId);
        }
    }
    
    void onEventBeginValue(const char* eventId, const char* value)
    {
        JniMethodInfo jni;
        if (JniHelper::getStaticMethodInfo(jni, "com/umeng/AnalyticsHome", "onEventBegin", "(Ljava/lang/String;Ljava/lang/String;)V"))
        {
            jstring j_eventId = jni.env->NewStringUTF(eventId);
            jstring j_value = jni.env->NewStringUTF(value);
            jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, j_eventId, j_value);
            jni.env->DeleteLocalRef(jni.classID);
            jni.env->DeleteLocalRef(j_eventId);
            jni.env->DeleteLocalRef(j_value);
        }
    }
    
    void onEventEnd(const char* eventId)
    {
        JniMethodInfo jni;
        if (JniHelper::getStaticMethodInfo(jni, "com/umeng/AnalyticsHome", "onEventEnd", "(Ljava/lang/String;)V"))
        {
            jstring j_eventId = jni.env->NewStringUTF(eventId);
            jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, j_eventId);
            jni.env->DeleteLocalRef(jni.classID);
            jni.env->DeleteLocalRef(j_eventId);
        }
    }
    
    void onEventEndValue(const char* eventId, const char* value)
    {
        JniMethodInfo jni;
        if (JniHelper::getStaticMethodInfo(jni, "com/umeng/AnalyticsHome", "onEventEnd", "(Ljava/lang/String;Ljava/lang/String;)V"))
        {
            jstring j_eventId = jni.env->NewStringUTF(eventId);
            jstring j_value = jni.env->NewStringUTF(value);
            jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, j_eventId, j_value);
            jni.env->DeleteLocalRef(jni.classID);
            jni.env->DeleteLocalRef(j_eventId);
            jni.env->DeleteLocalRef(j_value);
        }
    }
    
    void onEvent(const char* eventId)
    {
        JniMethodInfo jni;
        if (JniHelper::getStaticMethodInfo(jni, "com/umeng/AnalyticsHome", "onEvent", "(Ljava/lang/String;)V"))
        {
            jstring j_eventId = jni.env->NewStringUTF(eventId);
            jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, j_eventId);
            jni.env->DeleteLocalRef(jni.classID);
            jni.env->DeleteLocalRef(j_eventId);
        }
    }
    
    void onEventValue(const char* eventId, const char* value)
    {
        JniMethodInfo jni;
        if (JniHelper::getStaticMethodInfo(jni, "com/umeng/AnalyticsHome", "onEvent", "(Ljava/lang/String;Ljava/lang/String;)V"))
        {
            jstring j_eventId = jni.env->NewStringUTF(eventId);
            jstring j_value = jni.env->NewStringUTF(value);
            jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, j_eventId, j_value);
            jni.env->DeleteLocalRef(jni.classID);
            jni.env->DeleteLocalRef(j_eventId);
            jni.env->DeleteLocalRef(j_value);
        }
    }
    
    void onEventMap(const char* eventId, const std::map<std::string,std::string>& map)
    {
        
    }
    
    void onEventMapValue(const char* eventId, const std::map<std::string,std::string>& map, int value)
    {
        
    }
};
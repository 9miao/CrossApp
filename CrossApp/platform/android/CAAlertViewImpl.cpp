

#include "../CAAlertViewImpl.h"
#include "basics/CAApplication.h"
#include "view/CAWindow.h"
#include <map>
#include <string>
#include <android/log.h>
#include <jni.h>
#include "jni/JniHelper.h"
//#include "jni/Java_org_CrossApp_lib_CrossAppHelper.h"

NS_CC_BEGIN

static std::map<int, std::function<void(int)> > s_map;
static std::map<int, CAView*> s_mask_map;

#define CLASS_NAME "org/CrossApp/lib/CrossAppAlertView"

void createAlertJNI(const char * pszTitle, const char * pszMsg, int key)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createAlert", "(Ljava/lang/String;Ljava/lang/String;I)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(pszTitle);
        jstring stringArg2 = t.env->NewStringUTF(pszMsg);

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2, key);
        
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(stringArg2);
        t.env->DeleteLocalRef(t.classID);
    }
}

void addButtonJNI(const char * pszTitle, int key)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "addButton", "(Ljava/lang/String;I)V"))
    {
        jstring stringArg = t.env->NewStringUTF(pszTitle);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg, key);
        
        t.env->DeleteLocalRef(stringArg);
        t.env->DeleteLocalRef(t.classID);
    }
}

void showJNI(int key)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "show", "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, key);
        t.env->DeleteLocalRef(t.classID);
    }
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppAlertView_onClick(JNIEnv *env, jclass cls, jint index, jint key)
    {
        if (s_map.find(key) != s_map.end())
        {
            const std::function<void(int)>& callback = s_map.at(key);
            if (callback)
            {
                callback(index);
            }
            s_map.erase(key);
            s_mask_map.at(key)->removeFromSuperview();
            s_mask_map.erase(key);
        }
        
    }
}




void __show_alertView(const std::string& title, const std::string& message, const std::vector<std::string>& buttonTitles, const std::function<void(int)>& callback)
{
    static int key = 0;
    
    auto mask = new CAView();
    CAApplication::getApplication()->getRootWindow()->insertSubview(mask, 0xffffffff);
    mask->release();
    s_mask_map[key] = mask;
    s_map[key] = callback;
    
    createAlertJNI(title.c_str(), message.c_str(), key);
    
    for (auto& var : buttonTitles)
    {
        addButtonJNI(var.c_str(), key);
    }
    
    showJNI(key);
    
    ++key;
}







//void showEditTextDialogJNI(const char* pszTitle, const char* pszMessage, int nInputMode, int nInputFlag, int nReturnType, int nMaxLength, EditTextCallback pfEditTextCallback, void* ctx) {
//    if (pszMessage == NULL) {
//        return;
//    }
//
//    s_pfEditTextCallback = pfEditTextCallback;
//    s_ctx = ctx;
//
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showEditTextDialog", "(Ljava/lang/String;Ljava/lang/String;IIII)V")) {
//        jstring stringArg1;
//
//        if (!pszTitle) {
//            stringArg1 = t.env->NewStringUTF("");
//        } else {
//            stringArg1 = t.env->NewStringUTF(pszTitle);
//        }
//
//        jstring stringArg2 = t.env->NewStringUTF(pszMessage);
//
//        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2, nInputMode, nInputFlag, nReturnType, nMaxLength);
//
//        t.env->DeleteLocalRef(stringArg1);
//        t.env->DeleteLocalRef(stringArg2);
//        t.env->DeleteLocalRef(t.classID);
//    }
//}

NS_CC_END

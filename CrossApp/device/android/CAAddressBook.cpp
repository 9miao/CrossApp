//
//  CAAddressBook.cpp
//  CrossApp
//
//  Created by 秦乐 on 2017/2/21.
//  Copyright © 2017年 cocos2d-x. All rights reserved.
//
#include "../CAAddressBook.h"
#include "support/Json/CSContentJsonDictionary.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

NS_CC_BEGIN

extern "C"
{
    void JAVAGetAddressBook()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/CrossAppDevice", "getPersonList", "()V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID);
            jmi.env->DeleteLocalRef(jmi.classID);
        }
    }
}

#endif

CAAddressBook::CAAddressBook()
{
}

CAAddressBook::~CAAddressBook()
{
}

std::function<void(const std::vector<CAAddressBook::Data>&)> _bookCallBack;

std::vector<CAAddressBook::Data> _addressBookVec;

void CAAddressBook::getAddressBook(const std::function<void(const std::vector<CAAddressBook::Data>&)>& callback)
{
    _bookCallBack = callback;
    
    JAVAGetAddressBook();
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppPersonList_getPersonList(JNIEnv *env,jobject obj,jstring arg1)
    {
        _addressBookVec.clear();
        
        const char *sPersonList = env->GetStringUTFChars(arg1,NULL);
        
        CSJson::Reader read;
        CSJson::Value root;
        
        if (read.parse(sPersonList, root))
        {
            CSJson::Value personlist;
            personlist = root["person"];
            
            for (int i=0; i<personlist.size(); i++)
            {
                CSJson::Value person;
                person = personlist[i];
                CAAddressBook::Data addrec;
                
                addrec.fullname = person["name"].asString();
                
                CSJson::Value phonelist = person["phone"];
                
                for (int i=0; i<1; i++)
                {
                    addrec.phoneNumber = phonelist[i].asString();
                }
                
                addrec.email = person["email"].asString();
                
                addrec.street = person["address_street"].asString();
                
                addrec.province = person["address_region"].asString();
                
                addrec.city = person["address_city"].asString();
                
                addrec.nickname = person["nickname"].asString();
                
                addrec.firstLetter = person["firstLetter"].asString();
                
                addrec.zip = person["address_postCode"].asString();
                
                _addressBookVec.push_back(addrec);
                
            }
        }
        
        if(_bookCallBack)
        {
            _bookCallBack(_addressBookVec);
        }
    }
}

NS_CC_END

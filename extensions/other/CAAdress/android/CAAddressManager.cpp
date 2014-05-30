//
//  CAAddressManager.cpp
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#include "../CAAddressManager.h"
static CAAddressManager* ss=NULL;
static vector<CAAddressBookRecord*> recArr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
extern "C"
const char* ShowAddress()
{
	JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/cocos2dx/lib/AndroidPersonList" , "CAGetPersonList" , "()Ljava/lang/String;"))
	{
        
		jstring a = (jstring)jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
        const char* b = jmi.env->GetStringUTFChars( a , 0 );
        return b;
        
	}
}
#else
const char* ShowAddress()
{
}
#endif
CAAddressManager::CAAddressManager()
:_addressBookArr(NULL)
{
    recArr.clear();

}
CAAddressManager::~CAAddressManager()
{
    recArr.clear();
}
extern "C"
CAAddressManager *CAAddressManager::shareAddressManager()
{
    
    if (!ss)
    {
        ss = new CAAddressManager();
        
    }
    
    return ss;
}
std::vector<CAAddressBookRecord*> CAAddressManager::getAddressBook()
{
    
    if (_addressBookArr.size()>0) {
        return _addressBookArr;
    }
    this->releaseJson(ShowAddress());
    
    return _addressBookArr;
    
    
}

extern "C"
void CAAddressManager::releaseJson(const char* json)
{
    CSJson::Reader read;
    CSJson::Value root;
    if (read.parse(json, root))
    {
        CSJson::Value personlist;
        
        personlist = root["person"];
        for (int i=0; i<personlist.size(); i++)
        {
            CSJson::Value person;
            person = personlist[i];
            CAAddressBookRecord *addrec = new CAAddressBookRecord();

            addrec->fullname = person["name"].asString();
            
            CSJson::Value phonelist = person["phone"];
            for (int i=0; i<1; i++) {
                

                    addrec->phoneNumber = phonelist[i].asString();
                    
                
            }

            addrec->email = person["email"].asString();

            addrec->street = person["address_street"].asString();

            addrec->province = person["address_region"].asString();

            addrec->city = person["address_city"].asString();

            addrec->nickname = person["nickname"].asString();
            
            addrec->zip = person["address_postCode"].asString();

            _addressBookArr.push_back(addrec);

        }
    }
}

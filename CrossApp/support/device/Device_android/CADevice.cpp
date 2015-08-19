//
//  CADevice.cpp
//  PublishPhoto
//
//  Created by lihui on 14-10-21.
//
//

#include "../CADevice.h"
#include "../../Json/CSContentJsonDictionary.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

NS_CC_BEGIN

namespace CADevice
{

extern "C"
const char* ShowAddress()
{
	JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidPersonList" , "CAGetPersonList" , "()Ljava/lang/String;"))
	{
        
		jstring a = (jstring)jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
        const char* b = jmi.env->GetStringUTFChars( a , 0 );
        return b;
        
	}
}
extern "C"
{
    float JAVAgetBrightness()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNativeTool" , "getScreenBrightness" , "()I"))
        {
            
            jint a = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            int b = a;
            return (float)b/255.0f;
            
        }
    }
    
    void JAVAgetWifiList()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/Cocos2dxActivity" , "getWifiList" , "()V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID);
        }
    }
    CAWifiInfo JAVAgetWifiConnection()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/Cocos2dxActivity" , "getWifiConnectionInfo" , "()Lorg/CrossApp/lib/CustomScanResult;"))
        {
            jobject obj = (jobject)jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
            jclass cls_arraylist = jmi.env->GetObjectClass(obj);
            jmethodID user_getssid = jmi.env->GetMethodID(cls_arraylist,"getssid","()Ljava/lang/String;");
            jmethodID user_getmac = jmi.env->GetMethodID(cls_arraylist,"getmac","()Ljava/lang/String;");
            
            jstring ssid = (jstring)jmi.env->CallObjectMethod(obj,user_getssid);
            jstring mac = (jstring)jmi.env->CallObjectMethod(obj,user_getmac);
            
            const char *ssidPtr = jmi.env->GetStringUTFChars(ssid,0);
            const char *macPtr = jmi.env->GetStringUTFChars(mac,0);
        
            CAWifiInfo wifiinfo;
            wifiinfo.mac = macPtr;
            wifiinfo.ssid = ssidPtr;
            wifiinfo.level = 0;
            return wifiinfo;
        }
    }
    
    void JAVABlueToothManager()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/Cocos2dxActivity" , "getContext" , "()Lorg/CrossApp/lib/Cocos2dxActivity;"))
        {
            jobject obj = jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
            
            bool isHave =JniHelper::getMethodInfo(jmi,"org/CrossApp/lib/Cocos2dxActivity","initBlueTooth","()V");
            if (isHave) {
                jmi.env->CallVoidMethod(obj, jmi.methodID);
            }
        }
    }
    
    void JAVABlueSetType(int type)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/Cocos2dxActivity" , "getContext" , "()Lorg/CrossApp/lib/Cocos2dxActivity;"))
        {
            jobject obj = jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
            bool isHave =JniHelper::getMethodInfo(jmi,"org/CrossApp/lib/Cocos2dxActivity","setBlueToothActionType","(I)V");
            if (isHave)
            {
                jmi.env->CallVoidMethod(obj, jmi.methodID,type);
            }
        }
    }
    
    void JAVAsendLocalNotification(const char* title,const char* content,unsigned long time)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNativeTool" , "sendLocalNotification" , "(Ljava/lang/String;Ljava/lang/String;J)V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,jmi.env->NewStringUTF(title),jmi.env->NewStringUTF(content),time);
            
        }
    }
    void setJAVABrightness(int sender)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNativeTool" , "setScreenBrightness" , "(I)V"))
        {
            
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,sender);
            
        }
        
    }

    float JAVAgetVolume(int type)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidVolumeControl" , "getCallVolum" , "(I)F"))
        {
                
            jfloat a = (jfloat)jmi.env->CallStaticFloatMethod(jmi.classID , jmi.methodID,type);
            float b = a;
            return b;
                
        }
    }
    
    void JAVAsetVolume(float sender,int type)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidVolumeControl" , "setCallVolum" , "(FI)V"))
        {
                
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,sender,type);
                
        }
    }
    
    float JAVAgetBattery()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/Cocos2dxActivity" , "getBatteryLevel" , "()I"))
        {
            jint a = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            int b = a;
            return (float)b/100.0f;
        }
    }
    void JAVAgetNewVersion(const std::string &url,unsigned int versionNumber
                           ,const std::string &appId)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/Cocos2dxActivity" , "getNewVersion" , "(Ljava/lang/String;I)V"))
        {
            
            jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID,jmi.env->NewStringUTF(url.c_str()),versionNumber);
            
            
        }
    }
    bool JAVAisNetWorkAvailble()
    {
        
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNetWorkManager" , "getAPNType" , "()I"))
        {
            
            jint a = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            int b = a;
            return (bool)b;
        }
    }
    CANetWorkType JAVAgetNetWorkType()
    {
        CANetWorkType networkType = CANetWorkTypeNone;
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNetWorkManager" , "getAPNType" , "()I"))
        {
            
            jint a = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            switch (a)
            {
                case 0:
                    networkType = CANetWorkTypeNone;
                    break;
                case 1:
                    networkType = CANetWorkTypeWifi;
                    break;
                case 2:
                case 3:
                    networkType = CANetWorkType3G;
                    break;
            }
        }
        return networkType;
    }
}
    
const char* getAppVersion()
{
    JniMethodInfo jmi;
    if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNativeTool" , "getAppVersion" , "()Ljava/lang/String;"))
    {
        jstring a = (jstring)jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
        const char* b = jmi.env->GetStringUTFChars( a , 0 );
        return b;
    }
}
    
double* getGPSLocation()
{
	JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidGPS" , "getLocation" , "()[D"))
	{
		jdoubleArray doublearr = (jdoubleArray)jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
        if (doublearr==NULL) {
            double arr[2]={0.0f,0.0f};
            return arr;
        }
        double *b = jmi.env->GetDoubleArrayElements( doublearr , NULL );
        return b;
	}
}

void start()//ex
{
    JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/Cocos2dxActivity" , "startGps" , "()V"))
	{
        jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
        
	}
}

void sendLocalNotification(const char* title,const char* content,unsigned long time)
{
    JAVAsendLocalNotification(title,content,time);
}

void setVolume(float sender,int type)
{
        
    JAVAsetVolume(sender,type);
}

CAWifiInfo getWifiConnectionInfo()
{
    return JAVAgetWifiConnection();
}
    
float getVolume(int type)
{
    JAVAgetVolume(type);
}
    
float getBatteryLevel()
{
    return JAVAgetBattery();
}
void openBlueTooth()
{
    JAVABlueToothManager();
}
static CABlueToothDelegate *bluetoothdelegate =NULL;
    
void initBlueTooth(CABlueToothDelegate *target)
{
    bluetoothdelegate = target;
    openBlueTooth();
}


    
void setBlueToothType(CABlueToothType type)
{
    int intType = (int)type;
    JAVABlueSetType(intType);
}
    
bool isNetWorkAvailble()
{
    return JAVAisNetWorkAvailble();
}
    
void updateVersion(const std::string &url
                       ,unsigned int versionNumber
                       ,const std::string &appId)
{
    JAVAgetNewVersion(url,versionNumber,appId);
    
}
    
void OpenAlbum(int type)
{
	JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNativeTool" , "CAImageAlbum" , "(I)V"))
	{
		jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,type);
	}
}
    
CANetWorkType getNetWorkType()
{
    return JAVAgetNetWorkType();
}
    
void OpenURL(const std::string &url)
{
    JniMethodInfo jmi;
    if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNativeTool" , "browserOpenURL" , "(Ljava/lang/String;)V"))
    {
        jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,jmi.env->NewStringUTF(url.c_str()));
    }
}
    
void OpenCamera(int type)
{
    JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNativeTool" , "CAImageCapture" , "(I)V"))
	{
		jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,type);
	}
}
    
class ToMainThread:public CAObject
{
public:
    
    ToMainThread();
    
    ~ToMainThread();
    
    static ToMainThread* sharedMain();
    
    void getPath(const char* path);
    
    void runDelegate();
};
    
#endif
    

static CAMediaDelegate *delegate = NULL;
static ToMainThread *main = NULL;
static const char *_path = NULL;
//static CALocationDelegate *locationDelegate = NULL;
static CAWifiDelegate *wifidelegate = NULL;
static std::vector<CAAddressBookRecord> _addressBookArr;

void openCamera(CAMediaDelegate* target)
{
    delegate = target;
//    if (allowEdit) {
//        OpenCamera(0);
//    }
//    else
    {
        OpenCamera(1);
    }
    
}
    
void getWifiList(CAWifiDelegate *target)
{
    wifidelegate = target;
    JAVAgetWifiList();
}
    
void openAlbum(CAMediaDelegate* target)
{
    delegate = target;
//    if (allowEdit) {
//        OpenAlbum(0);
//    }
//    else
    {
        OpenAlbum(1);
    }

}
    
vector<CAAddressBookRecord> getAddressBook()
{
    if (_addressBookArr.size() > 0 )
    {
        return _addressBookArr;
    }
    
    CSJson::Reader read;
    CSJson::Value root;
    
    if (read.parse(ShowAddress(), root))
    {
        CSJson::Value personlist;
        personlist = root["person"];
        
        for (int i=0; i<personlist.size(); i++)
        {
            CSJson::Value person;
            person = personlist[i];
            CAAddressBookRecord addrec;
            
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
            
            addrec.zip = person["address_postCode"].asString();
            
            _addressBookArr.push_back(addrec);
            
        }
    }

    return _addressBookArr;
}
    
float getScreenBrightness()
{
    float percent = JAVAgetBrightness();
    return percent;
}
    
void setScreenBrightness(float brightness)
{
    int sender = (int)(brightness*255.0f);
    
    setJAVABrightness(sender);
}

    
ToMainThread::ToMainThread()
{
    
}
    
ToMainThread::~ToMainThread()
{
    
}
    
ToMainThread * ToMainThread::sharedMain()
{
    if (main == NULL)
    {
        main = new ToMainThread();
        return main;
    }
    return main;
}
    
void ToMainThread::getPath(const char* path)
{
    _path = path;
    CAScheduler::schedule(schedule_selector(ToMainThread::runDelegate), this, 0,false);
}
    
void ToMainThread::runDelegate()
{
    if (delegate)
    {
        CAImage *image = new CAImage();
        if (image->initWithImageFile(_path))
        {
            CAScheduler::unschedule(schedule_selector(ToMainThread::runDelegate), this);
            delegate->getSelectedImage(CAImage::generateMipmapsWithImage(image));
            image->release();
        }
    }
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_AndroidNativeTool_NativeReturn
    ( JNIEnv* env,jobject thiz ,jstring arg1, jobject arg2)
    {
        const char* str = env->GetStringUTFChars(arg1, false);
        ToMainThread::sharedMain()->getPath(str);
    }
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxActivity_returnBlueToothState(JNIEnv *env,jobject obj,jint sender)
    {
        int type = sender;
        
        if(bluetoothdelegate)
        {
            bluetoothdelegate->getBlueToothState((CABlueToothState)type);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxActivity_returnStartedDiscoveryDevice(JNIEnv *env,jobject obj)
    {
        if(bluetoothdelegate)
        {
            bluetoothdelegate->startDiscoveryBlueToothDevice();
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxActivity_returnFinfishedDiscoveryDevice(JNIEnv *env,jobject obj)
    {
        if(bluetoothdelegate)
        {
            bluetoothdelegate->finishedDiscoveryBlueToothDevice();
        }
    }
    
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxActivity_returnDiscoveryDevice(JNIEnv *env,jobject obj,jobject sender)
    {
        jclass bluetoothtype = env->GetObjectClass(sender);
        jmethodID getaddress = env->GetMethodID(bluetoothtype,"getAddress","()Ljava/lang/String;");
        jmethodID getname = env->GetMethodID(bluetoothtype,"getName","()Ljava/lang/String;");
        
        jstring address = (jstring)env->CallObjectMethod(sender,getaddress);
        jstring name = (jstring)env->CallObjectMethod(sender,getname);
        
        const char *addressCStr = env->GetStringUTFChars(address,0);
        const char *nameCStr = env->GetStringUTFChars(name,0);
        
        CABlueToothUnit unit;
        unit.address = addressCStr;
        unit.name = nameCStr;
        
        if(bluetoothdelegate)
        {
            bluetoothdelegate->getSearchBlueToothDevice(unit);
        }
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_Cocos2dxActivity_getWifiList(JNIEnv *env,jobject obj,jobject obj_wifiArray)
    {
        jclass cls_arraylist = env->GetObjectClass(obj_wifiArray);
        jmethodID   mGetID      = env->GetMethodID(cls_arraylist,"get","(I)Ljava/lang/Object;");
        jmethodID   mSizeID         = env->GetMethodID(cls_arraylist,"size","()I");
        int len = env->CallIntMethod(obj_wifiArray,mSizeID);
        std::vector<CAWifiInfo> wfinfo;
        for (int i=0; i<len; i++)
        {
            jobject obj_user = env->CallObjectMethod(obj_wifiArray,mGetID,i);
            jclass cls_user = env->GetObjectClass(obj_user);
            jmethodID user_getlevel = env->GetMethodID(cls_user,"getlevel","()I");
            jmethodID user_getssid = env->GetMethodID(cls_user,"getssid","()Ljava/lang/String;");
            jmethodID user_getmac = env->GetMethodID(cls_user,"getmac","()Ljava/lang/String;");
            
            jstring ssid = (jstring)env->CallObjectMethod(obj_user,user_getssid);
            jstring mac = (jstring)env->CallObjectMethod(obj_user,user_getmac);
            const char *ssidPtr = env->GetStringUTFChars(ssid,0);
            const char *macPtr = env->GetStringUTFChars(mac,0);
            CAWifiInfo wifiInfo;
            wifiInfo.ssid = ssidPtr;
            wifiInfo.mac = macPtr;
            wifiInfo.level = 0;
            wfinfo.push_back(wifiInfo);
        }
        
        if (wifidelegate)
        {
            wifidelegate->getWifiListFunc(wfinfo);
        }
        
        
    }
}

}

NS_CC_END
//
//  CADevice.cpp
//  PublishPhoto
//
//  Created by lihui on 14-10-21.
//
//

#include "../CADevice.h"
#include "../../Json/CSContentJsonDictionary.h"
#include "../../Json/DictionaryHelper.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

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
void start()
{
    JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/Cocos2dxActivity" , "startGps" , "()V"))
	{
        jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
        
	}
}

void setVolume(float sender,int type)
{
        
    JAVAsetVolume(sender,type);
}
    
float getVolume(int type)
{
    JAVAgetVolume(type);
}
    
float getBatteryLevel()
{
    return JAVAgetBattery();
}
    
int getBlueToothState()
{
    return 0;
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
void OpenAlbum()
{
	JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNativeTool" , "CAImageAlbum" , "()V"))
	{
		jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID);
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
    
void OpenCamera()
{
    JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/AndroidNativeTool" , "CAImageCapture" , "()V"))
	{
		jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID);
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
static CALocationDelegate *locationDelegate = NULL;
static std::vector<CAAddressBookRecord> _addressBookArr;

void openCamera(CAMediaDelegate* target)
{
    delegate = target;
    OpenCamera();
}

void openAlbum(CAMediaDelegate* target)
{
    delegate = target;
    OpenAlbum();
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
    
void startLocation(CALocationDelegate* target)
{
    start();
    
    double *b = getGPSLocation();
    
    CCString *lt= CCString::createWithFormat("%f",b[0]);
    CCString *lata= CCString::createWithFormat("%f",b[1]);
    CCDictionary *dic = CCDictionary::create();
    dic->retain();
    dic->setObject(lata, "longitude");
    dic->setObject(lt, "latitude");
    target->getLocations(dic);
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
    CCImage *image = new CCImage();
    if (image->initWithImageFileThreadSafe(_path,CCImage::kFmtJpg))
    {
        if (delegate)
        {
            CAScheduler::unschedule(schedule_selector(ToMainThread::runDelegate), this);
            CAImage* img = new CAImage();
            img->initWithImage(image);
            delegate->getSelectedImage(img);
            image->release();
            img->release();
            
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
}

}
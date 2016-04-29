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
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getPersonList" , "()Ljava/lang/String;"))
	{        
		jstring sList = (jstring)jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
		const char* utfSList = jmi.env->GetStringUTFChars(sList, 0);
		return utfSList;
	}
}

extern "C"
{
    float JAVAgetBrightness()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getScreenBrightness" , "()I"))
        {
            jint nBrightness = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
			int brightness = nBrightness;
			return (float)brightness / 255.0f;
        }
    }
    
    void JAVAgetWifiList()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getWifiList" , "()V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID);
        }
    }

    CAWifiInfo JAVAgetWifiConnection()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getWifiConnectionInfo" , "()Lorg/CrossApp/lib/CrossAppCustomScanResult;"))
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
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getDevice" , "()Lorg/CrossApp/lib/CrossAppDevice;"))
        {
            jobject obj = jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
            
            bool isHave =JniHelper::getMethodInfo(jmi,"org/CrossApp/lib/CrossAppDevice","initBlueTooth","()V");
            if (isHave) 
			{
                jmi.env->CallVoidMethod(obj, jmi.methodID);
            }
        }
    }
    
    void JAVABlueSetType(int type)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getDevice" , "()Lorg/CrossApp/lib/CrossAppDevice;"))
        {
            jobject obj = jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
            jint isHave =JniHelper::getMethodInfo(jmi,"org/CrossApp/lib/CrossAppDevice","setBlueToothActionType","(I)V");
            if (isHave)
            {
                jmi.env->CallVoidMethod(obj, jmi.methodID,type);
            }
        }
    }
    
    void JAVAsendLocalNotification(const char* title,const char* content,unsigned long time)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "sendLocalNotification" , "(Ljava/lang/String;Ljava/lang/String;J)V"))
        {
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,jmi.env->NewStringUTF(title),jmi.env->NewStringUTF(content),time);
            
        }
    }

    void setJAVABrightness(int sender)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "setScreenBrightness" , "(I)V"))
        {            
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,sender);           
        }       
    }

    float JAVAgetVolume(int type)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getVolum" , "(I)F"))
        {   
            jfloat a = (jfloat)jmi.env->CallStaticFloatMethod(jmi.classID , jmi.methodID,type);
            float b = a;
            return b;
        }
    }
    
    void JAVAsetVolume(float sender,int type)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "setVolum" , "(FI)V"))
        {    
            jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,sender,type);   
        }
    }
    
    float JAVAgetBattery()
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getBatteryLevel" , "()I"))
        {
            jint a = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            int b = a;
            return (float)b/100.0f;
        }
    }

    void JAVAgetNewVersion(const std::string &url,unsigned int versionNumber,const std::string &appId)
    {
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppActivity" , "getNewVersion" , "(Ljava/lang/String;I)V"))
        {
            
            jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID,jmi.env->NewStringUTF(url.c_str()),versionNumber); 
        }
    }

    bool JAVAisNetWorkAvailble()
    {        
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getAPNType" , "()I"))
        {
            jint a = (jint)jmi.env->CallStaticIntMethod(jmi.classID , jmi.methodID);
            int b = a;
            return (bool)b;
        }
    }

	void JAVAStartAccelerometer()
	{
		JniMethodInfo jmi;
		if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/CrossAppDevice", "enableAccelerometer", "()V"))
		{
			jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID);
		}
	}

	void JAVASetAccelerometer(float interval)
	{
		JniMethodInfo jmi;
		if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/CrossAppDevice", "setAccelerometerInterval", "(F)V"))
		{
			jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID,interval);
		}
	}

	void JAVAStopAccelerometer()
	{
		JniMethodInfo jmi;
		if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/CrossAppDevice", "disableAccelerometer", "()V"))
		{
			jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID);
		}
	}

    CANetWorkType JAVAgetNetWorkType()
    {
        CANetWorkType networkType = CANetWorkTypeNone;
        JniMethodInfo jmi;
        if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getAPNType" , "()I"))
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
    
const char* getSystemVersionWithIOS()
{
    return "";
}
    
const char* getAppVersion()
{
    JniMethodInfo jmi;
    if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "getAppVersion" , "()Ljava/lang/String;"))
    {
        jstring a = (jstring)jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
        const char* b = jmi.env->GetStringUTFChars( a , 0 );
        return b;
    }
}

void writeToSavedPhotosAlbum(CAImage* image, const std::string &imageName)
{
	JniMethodInfo jmi;
	const char* path;
	std::string savePath;
	if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/CrossAppDevice", "getSaveImagePath", "()Ljava/lang/String;"))
	{
		jstring a = (jstring)jmi.env->CallStaticObjectMethod(jmi.classID, jmi.methodID);
		path = jmi.env->GetStringUTFChars(a, 0);
	}
	savePath = path;
	savePath = savePath + imageName + ".jpg";
	image->saveToFile(savePath);

	if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/CrossAppDevice", "UpdateCamera", "(Ljava/lang/String;)V"))
	{
		jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, jmi.env->NewStringUTF(savePath.c_str()));
	}	
}

void JAVAUpdatingLocation()
{
    JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "startUpdatingLocation" , "()V"))
	{
        jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
	}
}

void JAVAStopUpdateLocation()
{
	JniMethodInfo jmi;
	if (JniHelper::getStaticMethodInfo(jmi, "org/CrossApp/lib/CrossAppDevice", "stopUpdatingLocation", "()V"))
	{
		jmi.env->CallStaticObjectMethod(jmi.classID, jmi.methodID);
	}
}

void sendLocalNotification(const char* title,const char* content,unsigned long time)
{
    JAVAsendLocalNotification(title,content,time);
}

void setVolume(float sender, CAVolumeType type)
{
	int mType = (int)type;
	JAVAsetVolume(sender, mType);
}

CAWifiInfo getWifiConnectionInfo()
{
    return JAVAgetWifiConnection();
}
    
float getVolume(CAVolumeType type)
{
	int mType = (int)type;
    JAVAgetVolume(mType);
}
    
float getBatteryLevel()
{
    return JAVAgetBattery();
}

static CABlueToothDelegate *bluetoothdelegate =NULL;
    
void initBlueTooth(CABlueToothDelegate *target)
{
    bluetoothdelegate = target;
	JAVABlueToothManager();
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
    
void JAVAOpenAlbum(int type)
{
	JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "CAImageAlbum" , "(I)V"))
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
    if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "browserOpenURL" , "(Ljava/lang/String;)V"))
    {
        jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID,jmi.env->NewStringUTF(url.c_str()));
    }
}
    
void JAVAOpenCamera(int type)
{
    JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/CrossApp/lib/CrossAppDevice" , "CAImageCapture" , "(I)V"))
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
static CALocationDelegate *locationDelegate = NULL;
static CAWifiDelegate *wifidelegate = NULL;
static CAAccelerometerDelegate* accelerometerDelegate = NULL;
static std::vector<CAAddressBookRecord> _addressBookArr;

void openCamera(CAMediaDelegate* target)
{
    delegate = target;

    JAVAOpenCamera(1);
}
    
void getWifiListWithAndroid(CAWifiDelegate *target)
{
    wifidelegate = target;

    JAVAgetWifiList();
}
    
void openAlbum(CAMediaDelegate* target)
{
    delegate = target;

    JAVAOpenAlbum(1);
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

void startAccelerometer(CAAccelerometerDelegate* delegate)
{
	accelerometerDelegate = delegate;

	JAVAStartAccelerometer();
}

void setAccelerometerInterval(float interval)
{
	JAVASetAccelerometer(interval);
}

void stopAccelerometer()
{
	JAVAStopAccelerometer();
}

void startUpdateLocation(CALocationDelegate* gpsDelegate)
{
	locationDelegate = gpsDelegate;
	JAVAUpdatingLocation();
}

void stopUpdateLocation()
{
	JAVAStopUpdateLocation();
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

	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppNativeTool_NativeReturn
    ( JNIEnv* env,jobject thiz ,jstring arg1, jobject arg2)
    {
        const char* str = env->GetStringUTFChars(arg1, false);
        ToMainThread::sharedMain()->getPath(str);
    }

	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppBlueTooth_returnBlueToothState(JNIEnv *env, jobject obj, jint sender)
    {
        int type = sender;
        
        if(bluetoothdelegate)
        {
            bluetoothdelegate->getBlueToothState((CABlueToothState)type);
        }
    }
    
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppBlueTooth_returnStartedDiscoveryDevice(JNIEnv *env, jobject obj)
    {
        if(bluetoothdelegate)
        {
            bluetoothdelegate->startDiscoveryBlueToothDevice();
        }
    }
    
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppBlueTooth_returnFinfishedDiscoveryDevice(JNIEnv *env, jobject obj)
    {
        if(bluetoothdelegate)
        {
            bluetoothdelegate->finishedDiscoveryBlueToothDevice();
        }
    }
    
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppBlueTooth_returnDiscoveryDevice(JNIEnv *env,jobject obj,jobject sender)
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
    
	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppAccelerometer_onSensorChanged(
		JNIEnv *env, 
		jobject obj,
		jfloat px,
		jfloat py,
		jfloat pz,
		jlong pTime)
	{
		CAAcceleration* acceleration = new CAAcceleration();
		acceleration->x = px;
		acceleration->y = py;
		acceleration->z = pz;		
		acceleration->timestamp = pTime;

		accelerometerDelegate->didAccelerate(acceleration);
	}

	JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppGPS_returnLocationInfo(JNIEnv *env, jobject obj, jobject sender)
	{
		jclass gpsInfoType = env->GetObjectClass(sender);


		jmethodID getLongitude = env->GetMethodID(gpsInfoType, "getLongitude", "()Ljava/lang/String;");
		jmethodID getLatitude = env->GetMethodID(gpsInfoType, "getLatitude", "()Ljava/lang/String;");
		jmethodID getAltitude = env->GetMethodID(gpsInfoType, "getAltitude", "()Ljava/lang/String;");
		jmethodID getSpeed = env->GetMethodID(gpsInfoType, "getSpeed", "()Ljava/lang/String;");
		jmethodID getBearing = env->GetMethodID(gpsInfoType, "getBearing", "()Ljava/lang/String;");

		jstring sLongitude = (jstring)env->CallObjectMethod(sender, getLongitude);
		jstring sLatitude = (jstring)env->CallObjectMethod(sender, getLatitude);
		jstring sAltitude = (jstring)env->CallObjectMethod(sender, getAltitude);
		jstring sSpeed = (jstring)env->CallObjectMethod(sender, getSpeed);
		jstring sBearing = (jstring)env->CallObjectMethod(sender, getBearing);

		const char *sLongitudeCStr = env->GetStringUTFChars(sLongitude, 0);
		const char *sLatitudeCStr = env->GetStringUTFChars(sLatitude, 0);
		const char *sAltitudeCStr = env->GetStringUTFChars(sAltitude, 0);
		const char *sSpeedCStr = env->GetStringUTFChars(sSpeed, 0);
		const char *sBearingCStr = env->GetStringUTFChars(sBearing, 0);

		CALocationInfo info;
		info.sLongitude = sLongitudeCStr;
		info.sLatitude = sLatitudeCStr;
		info.sAltitude = sAltitudeCStr;
		info.sSpeed = sSpeedCStr;
		info.sBearing = sBearingCStr;


		if (locationDelegate)
		{
			locationDelegate->getLocationChanged(info);
		}
	}

    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppDevice_getWifiList(JNIEnv *env,jobject obj,jobject obj_wifiArray)
    {
        jclass cls_arraylist = env->GetObjectClass(obj_wifiArray);
        jmethodID   mGetID    = env->GetMethodID(cls_arraylist,"get","(I)Ljava/lang/Object;");
        jmethodID   mSizeID   = env->GetMethodID(cls_arraylist,"size","()I");

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
//
//  CALocationManager.cpp
//  iBook
//
//  Created by lh on 14-5-6.
//
//

#include "../CALocationManager.h"
static CALocationManager* manger=NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
extern "C"
double* ShowDlg()
{
	JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/cocos2dx/lib/AndroidGPS" , "getLocation" , "()[D"))
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
	if(JniHelper::getStaticMethodInfo(jmi , "org/cocos2dx/lib/Cocos2dxActivity" , "startGps" , "()V"))
	{
        jmi.env->CallStaticObjectMethod(jmi.classID , jmi.methodID);
        
	}
}
#else
double* ShowDlg()
{
}
void start()
{
    
}
#endif
CALocationManager::CALocationManager()
{
    start();
}
CALocationManager::~CALocationManager()
{
    
}

CALocationManager* CALocationManager::create()
{
    if (!manger) {
        manger = new CALocationManager();
    }

    return manger;
}
void CALocationManager::startLocation()
{
    double *b=ShowDlg();
    
    this->setLocation(b[0],b[1]);
}

extern "C"
void CALocationManager::setLocation(double longt,double lat)
{
    CCString *lt= CCString::createWithFormat("%f",longt);
    CCString *lata= CCString::createWithFormat("%f",lat);
    CCDictionary *dic = CCDictionary::create();
    dic->retain();
    dic->setObject(lata, "longitude");
    dic->setObject(lt, "latitude");
    m_pLocationDelegate->getLocations(dic);
    
}

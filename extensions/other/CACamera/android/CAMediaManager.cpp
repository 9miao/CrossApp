//
//  CAMediaManager.cpp
//  LocationTest
//
//  Created by lh on 14-5-29.
//
//

#include "../CAMediaManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

void OpenAlbum()
{
	JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/cocos2dx/lib/AndroidNativeTool" , "CAImageAlbum" , "()V"))
	{
		jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID);
	}
}
void OpenCamera()
{
    JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/cocos2dx/lib/AndroidNativeTool" , "CAImageCapture" , "()V"))
	{
		jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID);
	}
}

#else
void OpenAlbum()
{
    
}
void OpenCamera()
{
    
}
#endif
static CAMediaManager *pCamera = NULL;



CAMediaManager::CAMediaManager()
:m_pMediaDelegate(NULL)
{
    
}
CAMediaManager::~CAMediaManager()
{
    
}
CAMediaManager* CAMediaManager::shareCAMediaManager()
{
    if (!pCamera)
    {
        pCamera= new CAMediaManager();
    }
    return  pCamera;
    
}

void CAMediaManager::addCameraView()
{
    OpenCamera();
    
}
void CAMediaManager::addAlbumView()
{
    OpenAlbum();
    
    
}
void CAMediaManager::initAndroidImage()
{
    
    CCImage *iamge =new CCImage();
    if (iamge->initWithImageData((void*)m_pBuffer,n_fileSize,CCImage::kFmtJpg))
    {
        
        this->getMediaDelegate()->getSelectedImage(iamge);
        CAScheduler::schedule(schedule_selector(CAMediaManager::initAndroidImage), this, 0,false);
    }
    
    
    
    
}

extern "C"
void CAMediaManager::getSelectedImage(const char *path)
{
    unsigned long filesize;
    char *buffer =(char *) CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &filesize);
    m_pBuffer = buffer;
    n_fileSize=filesize;
    CAScheduler::schedule(schedule_selector(CAMediaManager::initAndroidImage), this, 0,false);
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_AndroidNativeTool_NativeReturn
    ( JNIEnv* env,jobject thiz ,jstring arg1, jobject arg2)
    {
        const char* str = env->GetStringUTFChars(arg1, false);
        
        pCamera->getSelectedImage(str);
        
        
    }
}

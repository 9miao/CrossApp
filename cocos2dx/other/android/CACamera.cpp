//
//  CACamera.cpp
//  iBook
//
//  Created by lh on 14-5-5.
//
//

#include "../CACamera.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

void ShowDlg()
{
	JniMethodInfo jmi;
	if(JniHelper::getStaticMethodInfo(jmi , "org/cocos2dx/lib/AndroidNativeTool" , "CAImageAlbum" , "()V"))
	{

		jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID);
	}
}
#else
void ShowDlg()
{
}
#endif


static CACamera *pCamera = NULL;

CACamera::CACamera()
:m_pCameraDelegate(NULL)
{
    
}
CACamera::~CACamera()
{
    
}
CACamera* CACamera::shareCamera()
{
    if (!pCamera)
    {
        pCamera= new CACamera();
    }
    return  pCamera;
    
}

void CACamera::addCameraView()
{
    
    
}
void CACamera::addAlbumView()
{
    ShowDlg();
    
    
}
void CACamera::initAndroidImage()
{
    
    CCImage *iamge =new CCImage();

    if (iamge->initWithImageData((void*)m_pBuffer,n_fileSize,CCImage::kFmtJpg)) {
        
        this->getCameraDelegate()->getSelectedImage(iamge);
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CACamera::initAndroidImage), this);
    }

    
    
    
}

extern "C"
void CACamera::getSelectedImage(const char *path)
{
    unsigned long filesize;
    char *buffer =(char *) CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &filesize);
    m_pBuffer = buffer;
    n_fileSize=filesize;
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CACamera::initAndroidImage), this, 0, false);
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

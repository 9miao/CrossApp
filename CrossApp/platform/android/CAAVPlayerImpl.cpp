#include "../CAAVPlayerImpl.h"
#include "view/CAAVPlayerView.h"
#include "images/CAImage.h"
#include "basics/CAApplication.h"
#include <map>
#include <android/log.h>
#include <stdio.h>
#include <jni.h>
#include "jni/JniHelper.h"
#include "platform/CAFileUtils.h"

NS_CC_BEGIN


static std::map<int , CAAVPlayer*> s_map;
static std::map<CAAVPlayer* , std::pair<bool, std::string> > s_mapUrl;
static std::map<int , std::function<void(CAImage*)> > s_ImageCallback_map;
static std::map<int , std::function<void(float current, float duratuon)> > s_PeriodicTime_map;
static std::map<int , std::function<void(float current, float duratuon)> > s_LoadedTime_map;
static std::map<int , std::function<void()> > s_DidPlayToEndTime_map;
static std::map<int , std::function<void()> > s_TimeJumped_map;

static std::map<int , std::function<void(const std::string&)> > s_PlayBufferLoadingState_map;
static std::map<int , std::function<void(const std::string&)> > s_PlayState_map;

void removePlayer(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "removeById", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID , key);
        jni.env->DeleteLocalRef(jni.classID);
    }
}

void addPlayer(int key)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "addVideoByKey", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID , key);
        jni.env->DeleteLocalRef(jni.classID);
    }
}

CAAVPlayerImpl::CAAVPlayerImpl(CAAVPlayer* Player)
: m_pPlayer(Player)
{
    s_map[m_pPlayer->m_u__ID] = m_pPlayer;
    
    s_PeriodicTime_map[m_pPlayer->m_u__ID] = [&](float current, float duratuon)
    {
        if(m_pPlayer->m_obPeriodicTime)
            m_pPlayer->m_obPeriodicTime(current,duratuon) ;
    };
    s_LoadedTime_map[m_pPlayer->m_u__ID] = [&](float current, float duratuon)
    {
        if(m_pPlayer->m_obLoadedTime)
            m_pPlayer->m_obLoadedTime(current,duratuon) ;
    };
    
    s_DidPlayToEndTime_map[m_pPlayer->m_u__ID] = [&]()
    {
        if(m_pPlayer->m_obDidPlayToEndTime)
            m_pPlayer->m_obDidPlayToEndTime() ;
    };
    s_TimeJumped_map[m_pPlayer->m_u__ID] = [&]()
    {
        if(m_pPlayer->m_obTimeJumped)
            m_pPlayer->m_obTimeJumped() ;
    };
    
    s_PlayBufferLoadingState_map[m_pPlayer->m_u__ID] = [&](const std::string& str)
    {
        if(m_pPlayer->m_obPlayBufferLoadingState)
            m_pPlayer->m_obPlayBufferLoadingState(str) ;
    };
    
    s_PlayState_map[m_pPlayer->m_u__ID] = [&](const std::string& str)
    {
        if(m_pPlayer->m_obPlayState)
            m_pPlayer->m_obPlayState(str) ;
    };
    addPlayer(m_pPlayer->m_u__ID) ;
    
}

CAAVPlayerImpl::~CAAVPlayerImpl()
{
    s_map.erase(m_pPlayer->m_u__ID);
    s_mapUrl.erase(m_pPlayer);
    s_ImageCallback_map.erase(m_pPlayer->m_u__ID);
    s_PeriodicTime_map.erase(m_pPlayer->m_u__ID);
    s_LoadedTime_map.erase(m_pPlayer->m_u__ID);
    s_DidPlayToEndTime_map.erase(m_pPlayer->m_u__ID);
    s_TimeJumped_map.erase(m_pPlayer->m_u__ID);
    s_PlayBufferLoadingState_map.erase(m_pPlayer->m_u__ID);
    s_PlayState_map.erase(m_pPlayer->m_u__ID);    
    removePlayer(m_pPlayer->m_u__ID) ;
}


void CAAVPlayerImpl::setUrl(const std::string& url)
{
    s_mapUrl[m_pPlayer] = std::pair<bool, std::string>(true, url);
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "setUrl", "(Ljava/lang/String;I)V"))
    {
        jstring jFilePath = jni.env->NewStringUTF(url.c_str());
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID , jFilePath , (jint)m_pPlayer->m_u__ID);
        jni.env->DeleteLocalRef(jni.classID);
    }
    
}

void CAAVPlayerImpl::setFilePath(const std::string& filePath)
{
    s_mapUrl[m_pPlayer] = std::pair<bool, std::string>(false, filePath);
    std::string path = CrossApp::FileUtils::getInstance()->fullPathForFilename(filePath);
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "setFilePath", "(Ljava/lang/String;I)V"))
    {
        jstring jFilePath = jni.env->NewStringUTF(path.c_str());
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID , jFilePath , (jint)m_pPlayer->m_u__ID);
        jni.env->DeleteLocalRef(jni.classID);
    }
}

void CAAVPlayerImpl::play()
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "play4native", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID , (jint)m_pPlayer->m_u__ID);
        jni.env->DeleteLocalRef(jni.classID);
    }
}

void CAAVPlayerImpl::playWithRate(float rate)
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "play4native", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID , (jint)m_pPlayer->m_u__ID);
        jni.env->DeleteLocalRef(jni.classID);
    }
}

void CAAVPlayerImpl::pause()
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "pause4native", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, (jint)m_pPlayer->m_u__ID );
        jni.env->DeleteLocalRef(jni.classID);
    }
}

float CAAVPlayerImpl::getDuration()
{
    jint duration = 0 ;
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "getDuration", "(I)I"))
    {
        duration = jni.env->CallStaticIntMethod(jni.classID, jni.methodID, (jint)m_pPlayer->m_u__ID );
        jni.env->DeleteLocalRef(jni.classID);
    }
    
    return (float)duration ;
}

float CAAVPlayerImpl::getCurrentTime()
{
    jint current = 0 ;
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "getCurrentTime", "(I)I"))
    {
        current = jni.env->CallStaticIntMethod(jni.classID, jni.methodID, (jint)m_pPlayer->m_u__ID );
        jni.env->DeleteLocalRef(jni.classID);
    }
    
    return (float)current ;
}

void CAAVPlayerImpl::stop()
{
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "stop4native", "(I)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, (jint)m_pPlayer->m_u__ID );
        jni.env->DeleteLocalRef(jni.classID);
    }
}

void CAAVPlayerImpl::setCurrentTime(float current)
{
    
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "setCurrentTime4native", "(FI)V"))
    {
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, (jfloat)current , (jint)m_pPlayer->m_u__ID );
        jni.env->DeleteLocalRef(jni.classID);
    }
}


const DSize& CAAVPlayerImpl::getPresentationSize()
{
    DSize size = DSize(0, 0) ;
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "getPresentationSize4native", "(I)[I"))
    {
        jintArray arr = (jintArray)jni.env->CallStaticObjectMethod(jni.classID, jni.methodID, (jint)m_pPlayer->m_u__ID );
        
        jint* elems =jni.env-> GetIntArrayElements(arr, 0);
        size.width = (int)elems[0] ;
        size.height = (int)elems[1] ;
        
        jni.env->DeleteLocalRef(jni.classID);
        jni.env->ReleaseIntArrayElements(arr, elems, 0);
    }
    return size ;
    
}

void CAAVPlayerImpl::onImage(const std::function<void(CAImage*)>& function)
{
    s_ImageCallback_map[m_pPlayer->m_u__ID] = function;
}



static CAImage* image_frame_getter =  nullptr ;
CAImage* CAAVPlayerImpl::getFirstFrameImageWithFilePath(const std::string& filePath)
{
    
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "getFirstFrameImageWithFilePath", "(Ljava/lang/String;)V"))
    {
        jstring jFilePath = jni.env->NewStringUTF(filePath.c_str());
        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, jFilePath );
        jni.env->DeleteLocalRef(jni.classID);
    }
    CAImage* image = image_frame_getter;
    image_frame_getter= nullptr;
    return image;
}


//void CAAVPlayerControllerImpl::showAVPlayerController(CAAVPlayer* player)
//{
//    JniMethodInfo jni;
//    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppVideoPlayer", "showAVPlayerController", "(Ljava/lang/String;I)V"))
//    {
//        int local = 0 ;
//        bool b = s_mapUrl[player].first;
//        std::string& url = s_mapUrl[player].second ;
//        
//        //本地
//        if(!b)
//        {
//            local = 1 ;
//        }
//        
//        jni.env->CallStaticVoidMethod(jni.classID, jni.methodID, url.c_str() , local);
//        jni.env->DeleteLocalRef(jni.classID);
//    }
//}


extern "C"
{
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppVideoPlayer_onBitmapFrameAttached(JNIEnv *env, jclass cls, jbyteArray buf, jint length)
    {
        
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * (ssize_t)length);
        env->GetByteArrayRegion(buf, 0, (ssize_t)length, (jbyte *)data);
        CrossApp::CAData* ca_data = CrossApp::CAData::create();
        ca_data->fastSet(data, (ssize_t)length);
        image_frame_getter = CAImage::createWithImageDataNoCache(ca_data);
        

        
    }
    
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppVideoPlayer_onFrameAttached(JNIEnv *env, jclass cls, jint key, jbyteArray buf, jint width, jint height)
    {
        
        ssize_t length = width * height * 4;
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * length);
        env->GetByteArrayRegion(buf, 0, width * height * 4, (jbyte *)data);
        CrossApp::CAData* ca_data = CrossApp::CAData::create();
        ca_data->fastSet(data, length);
        CAImage* image = CAImage::createWithRawDataNoCache(ca_data, CAImage::PixelFormat::RGBA8888, width, height);
        
        if (s_ImageCallback_map.count((int)key) > 0)
        {
            if (auto& callback = s_ImageCallback_map.at((int)key))
            {
                callback(image);
            }
        }
        
    }
    
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppVideoPlayer_onFrameImageAttached(JNIEnv *env, jclass cls, jint key, jbyteArray buf, jint length)
    {
        
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * (ssize_t)length);
        env->GetByteArrayRegion(buf, 0, (ssize_t)length, (jbyte *)data);
        CrossApp::CAData* ca_data = CrossApp::CAData::create();
        ca_data->fastSet(data, (ssize_t)length);
        CAImage* image = CAImage::createWithImageDataNoCache(ca_data);
        
        if (s_ImageCallback_map.count((int)key) > 0)
        {
            if (auto& callback = s_ImageCallback_map.at((int)key))
            {
                callback(image);
            }
        }
        
    }
    
    
    // 监听播放进度
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppVideoPlayer_onPeriodicTime(JNIEnv *env, jclass cls, jint key, jfloat current,jfloat duration)
    {
        
        if (s_PeriodicTime_map.count((int)key) > 0)
        {
            if (auto& callback = s_PeriodicTime_map.at((int)key))
            {
                callback((float) current, (float) duration);
            }
        }
        
    }
    
    // 监听缓冲进度
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppVideoPlayer_onLoadedTime(JNIEnv *env, jclass cls, jint key, jfloat current,jfloat duration)
    {
        if (s_LoadedTime_map.count((int)key) > 0)
        {
            if (auto& callback = s_LoadedTime_map.at((int)key))
            {
                callback((float)current, (float)duration);
            }
        }
        
    }
    
    // 监听播放完毕
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppVideoPlayer_onDidPlayToEndTime(JNIEnv *env, jclass cls, jint key)
    {
        
        if (s_DidPlayToEndTime_map.count((int)key) > 0)
        {
            if (auto& callback = s_DidPlayToEndTime_map.at((int)key))
            {
                callback();
            }
        }
        
    }
    
    // 监听快进或者慢进或者跳过某段播放
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppVideoPlayer_onTimeJumped(JNIEnv *env, jclass cls, jint key)
    {
        if (s_TimeJumped_map.count((int)key) > 0)
        {
            if (auto& callback = s_TimeJumped_map.at((int)key))
            {
                callback();
            }
        }
    }
    
    // 监听缓冲状态
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppVideoPlayer_onPlayBufferLoadingState(JNIEnv *env, jclass cls, jint key , jint state)
    {
        if (s_PlayBufferLoadingState_map.count((int)key) > 0)
        {
            if (auto& callback = s_PlayBufferLoadingState_map.at((int)key))
            {
                int s = (int)state ;
                callback(   s == 0 ?  CrossApp::CAAVPlayer::PlaybackBufferEmpty : CrossApp::CAAVPlayer::PlaybackLikelyToKeepUp  );
            }
        }
    }
    
    // 监听播放状态
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppVideoPlayer_onPlayState(JNIEnv *env, jclass cls, jint key , jint state)
    {
        if (s_PlayState_map.count((int)key) > 0)
        {
            if (auto& callback = s_PlayState_map.at((int)key))
            {
                int s = (int)state ;
                callback(  s == 0 ? CrossApp::CAAVPlayer::PlayStatePause: ( s == 1 ? CrossApp::CAAVPlayer::PlayStatePlaying  : CrossApp::CAAVPlayer::PlayStatePlayback )  );
            }
        }
    }
    
    
    
}


NS_CC_END



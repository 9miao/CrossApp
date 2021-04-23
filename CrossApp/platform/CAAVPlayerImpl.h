

#ifndef __CAAVPlayerViewImpl_H__
#define __CAAVPlayerViewImpl_H__

#include <stdio.h>
#include <string>
#include <vector>
#include <functional>
#include "ccMacros.h"
#include "basics/CASize.h"
NS_CC_BEGIN
class CC_DLL CAImage;
class CC_DLL CAAVPlayer;
class CC_DLL CAAVPlayerImpl
{
public:
    
    static CAImage* getFirstFrameImageWithFilePath(const std::string& filePath);
    
    CAAVPlayerImpl(CAAVPlayer* player);
    
    virtual ~CAAVPlayerImpl();
    
    void setUrl(const std::string& url);
    
    void setFilePath(const std::string& filePath);
    
    void play();
    
    void playWithRate(float rate);
    
    void pause();
    
    void stop();
    
    float getDuration();
    
    float getCurrentTime();
    
    void setCurrentTime(float current);
    
    float getRate();
    
    void setRate(float rate);
    
    const DSize& getPresentationSize();

private:
    
    void onImage(const std::function<void(CAImage*)>& function);
        
    void* m_pNativeImpl;
    
    CAAVPlayer* m_pPlayer;
    
    friend class CAAVPlayer;
};

/*
class CC_DLL CAAVPlayerControllerImpl
{
public:
    
    static void showAVPlayerController(CAAVPlayer* player);
};
*/
NS_CC_END

#endif    // __CAAVPlayerViewImpl_H__

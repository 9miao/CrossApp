

#ifndef __CAAVPlayerView_H__
#define __CAAVPlayerView_H__

#include "CAView.h"

NS_CC_BEGIN


class CC_DLL CAAVPlayerImpl;
class CC_DLL CAAVPlayerView;

class CC_DLL CAAVPlayer : public CAObject
{

public:

    CAAVPlayer();
    
    virtual ~CAAVPlayer();
    
    static CAAVPlayer *createWithUrl(const std::string& uri);
    
    static CAAVPlayer *createWithFilePath(const std::string& uri);
    
    bool initWithUrl(const std::string& uri);
    
    bool initWithFilePath(const std::string& uri);
    
    void play();

    void pause();
    
    void stop();
  
    float getDuration();
    
    float getCurrentTime();
    
    void setCurrentTime(float current);
    
    const DSize& getPresentationSize();
    
public:
    
    CC_LISTENING_FUNCTION(void(float current, float duratuon), PeriodicTime);
    
    CC_LISTENING_FUNCTION(void(float current, float duratuon), LoadedTime);
    
    CC_LISTENING_FUNCTION(void(), DidPlayToEndTime);
    
    CC_LISTENING_FUNCTION(void(), TimeJumped);
    
    typedef std::string PlayBufferLoadingState;
	static const PlayBufferLoadingState PlaybackBufferEmpty;
	static const PlayBufferLoadingState PlaybackLikelyToKeepUp;
    CC_LISTENING_FUNCTION(void(const PlayBufferLoadingState&), PlayBufferLoadingState);
    
    typedef std::string PlayState;
	static const PlayState PlayStatePause;
	static const PlayState PlayStatePlaying;
	static const PlayState PlayStatePlayback;
    CC_LISTENING_FUNCTION(void(const PlayState&), PlayState);

protected:
    
    void onImage(const std::function<void(CAImage*)>& function);
        
    CAAVPlayerImpl* m_pImpl;
    
    friend class CAAVPlayerImpl;
    
    friend class CAAVPlayerView;
};


class CC_DLL CAAVPlayerView : public CAView
{
    
public:
    
    CAAVPlayerView();
    
    virtual ~CAAVPlayerView();
    
    static CAAVPlayerView *create(void);
    
    static CAAVPlayerView* createWithFrame(const DRect& rect);
    
    static CAAVPlayerView* createWithCenter(const DRect& rect);
    
    static CAAVPlayerView* createWithLayout(const DLayout& layout);
    
    bool init() override;
    
    CAAVPlayer* getPlayer();
    
    void setPlayer(CAAVPlayer* player);
    
protected:
    
    void setImage(CAImage* image) override;
    
    void updateImageRect() override;
    
private:
    
    float m_fX;
    
    float m_fY;
    
    float m_fWidth;
    
    float m_fHeight;
    
    CAAVPlayer* m_pPlayer;
};


NS_CC_END

#endif // __CAAVPlayerView_H__

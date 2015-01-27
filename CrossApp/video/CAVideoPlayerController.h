//
//  CAVideoPlayerController.h
//  CrossApp
//
//  Created by dai xinping on 14-10-30.
//  Copyright (c) 2014年 cocos2d-x. All rights reserved.
//

#ifndef __CrossApp__CAVideoPlayerController__
#define __CrossApp__CAVideoPlayerController__

#include "CAVideoPlayerView.h"

#include "controller/CAViewController.h"
#include "basics/CAThread.h"

#include "control/CASlider.h"
#include "control/CAButton.h"
#include "view/CALabel.h"

NS_CC_BEGIN

using namespace std;

using namespace CAVideoPlayer;

class CAVideoPlayerControllerDelegate
{
public:
    virtual void onVideoPlayerButtonBack() = 0;
};

class CAVideoPlayerController : public CAViewController {
    
public:
    CAVideoPlayerController();
    virtual ~CAVideoPlayerController();
    
    static CAVideoPlayerController* create(const string& path, const string& title);
    
    virtual bool init(const string& path, const string& title);
    
    virtual void play();
    virtual void pause();
    virtual bool isPlaying();
    virtual void displayHUD(bool bDisp);
    
    virtual float getDuration();
    virtual float getPosition();
    virtual void setPosition(float pos);
    
    CC_SYNTHESIZE(CAVideoPlayerControllerDelegate*, _delegate, Delegate);
            
protected:
    enum {
        
        kMovieInfoSectionGeneral,
        kMovieInfoSectionVideo,
        kMovieInfoSectionAudio,
        kMovieInfoSectionSubtitles,
        kMovieInfoSectionMetadata,    
        kMovieInfoSectionCount,
    };
    
    enum {
        
        kMovieInfoGeneralFormat,
        kMovieInfoGeneralBitrate,
        kMovieInfoGeneralCount,
    };
    
protected:
    CAVideoPlayerView   *_glView;
    VPDecoder           *_decoder;
    CCArray             *_videoFrames;
    CCArray             *_audioFrames;
    CCArray             *_subtitles;
    VPAudioFrame        *_currentAudioFrame;
    unsigned int        _currentAudioFramePos;
    float               _moviePosition;
    float               _wantMoviePosition;
    float               _movieDuration;
    bool                _interrupted;    
    float               _bufferedDuration;
    float               _minBufferedDuration;
    float               _maxBufferedDuration;
    bool                _playing;
    bool                _disableUpdateHUD;
    struct timeval      _tickCorrectionTime;
    float               _tickCorrectionPosition;
    unsigned int        _tickCounter;
    bool                _decoding;
    VPArtworkFrame      *_artworkFrame;
    bool                _buffered;
    string              _path;
    string              _title;
    CAActivityIndicatorView *_activityView;
    CAButton            *_playButton;
    CASlider            *_playSlider;
    CALabel             *_playTime;
    
protected:
    virtual void viewDidLoad();
    virtual void viewDidUnload();
    virtual void prepare();
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    bool interruptDecoder();
    bool setMovieDecoder();
    void freeBufferedFrames();
    void enableAudio(bool on);
    void updatePlayButton();
    void asyncDecodeFrames();
    void tick(float dt);
    float tickCorrection();
    bool addFrames(const vector<VPFrame*>& frames);
    float presentFrame();
    float presentVideoFrame(VPVideoFrame* frame);
    void presentSubtitles();
    void audioCallback(unsigned char *stream, int len, int channels);
    void _decodeProcess();
    
    void setMoviePositionFromDecoder();
    void setDecoderPosition(float position);
    void setMoviePosition(float position);
    void updatePosition(float position, bool playing);
    void gotoWantedMoviePosition();
    
    void dispearHUDView();
    void showHUDView();
    
    virtual void didReceiveMemoryWarning();

    //    HUD
protected:
    CASlider            *_slider;
    CAView              *_HUDView;
        
protected:
    void onButtonBack(CAControl* control, CCPoint point);
    void onButtonPause(CAControl* control, CCPoint point);
    void onCheckExit(float dt);
    void buildHUD();
    void updateHUD();
    void onSlideTouched(CAControl* control, CCPoint point);
    void onSlideChanged(CAControl* control, CCPoint point);
    
private: // THREAD
    bool            m_inited;
    bool            m_bNeedExit;
    pthread_t       m_thread;
    pthread_mutex_t m_vp_data_mutex;
    pthread_mutex_t m_vp_cond_mutex;
    pthread_cond_t  m_vp_cond;
    
    void lazyInit();
    
public:
    void lazyProcess();
};

NS_CC_END

#endif /* defined(__CrossApp__CAVideoPlayerController__) */

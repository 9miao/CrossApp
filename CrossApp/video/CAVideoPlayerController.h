//
//  CAVideoPlayerController.h
//  CrossApp
//
//  Created by dai xinping on 14-11-6.
//  Modified by zhujian on 15-9-6
//
//  Copyright (c) 2015年 http://www.9miao.com. All rights reserved.
//

#ifndef __CrossApp__CAVideoPlayerController__
#define __CrossApp__CAVideoPlayerController__

#include "controller/CAViewController.h"
#include "basics/CAThread.h"
#include "control/CASlider.h"
#include "control/CAButton.h"
#include "view/CALabel.h"
#include "basics/CASTLContainer.h"
#include "CAVideoPlayerView.h"

NS_CC_BEGIN

class CAVideoPlayerView;
class VPAudioFrame;
class VPFrame;
class VPArtworkFrame;
class VPDecoder;



class CC_DLL CAVideoPlayerController : public CAViewController
{
public:
    CAVideoPlayerController();
    virtual ~CAVideoPlayerController();

    static CAVideoPlayerController* createWithPath(const string& path, const string& title);
    static CAVideoPlayerController* createWithUrl(const string& path, const string& title);
    
    virtual bool init(const string& path, const string& title,bool pathType);
    
    virtual void play();
    virtual void pause();
    virtual bool isPlaying();
    virtual void displayHUD(bool bDisp);
    
    virtual float getDuration();
    virtual float getPosition();
    virtual void setPosition(float pos);

            
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

    CAVideoPlayerView*_glView;

    VPDecoder*_decoder;
    CAVector<CAObject*>  _videoFrames;
    CAVector<CAObject*>  _audioFrames;

    VPAudioFrame        *_currentAudioFrame;
    unsigned int        _currentAudioFramePos;
    float               _moviePosition;
    float               _wantMoviePosition;
    float               _movieDuration;
 
    float               _bufferedDuration;
    float               _minBufferedDuration;
    float               _maxBufferedDuration;
    bool                _playing;
    bool                _disableUpdateHUD;
    struct timeval      _tickCorrectionTime;
    float               _tickCorrectionPosition;
    unsigned int        _tickCounter;
    bool                _decoding;

    bool                _buffered;
    string              _path;
    string              _title;
    CAActivityIndicatorView *_activityView;
    CAButton            *_playButton;
    CASlider            *_playSlider;
    CALabel             *_playTime;
    bool                _isPathByUrl;
    
protected:
    virtual void viewDidLoad();
    virtual void viewDidUnload();
    virtual void prepare(float);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    

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

    void audioCallback(unsigned char *stream, int len, int channels);
    void _decodeProcess();
    
    void setMoviePositionFromDecoder();
    void setDecoderPosition(float position);
    void setMoviePosition(float position);
    void updatePosition(float position, bool playing);
    void gotoWantedMoviePosition(float);
    
    void dispearHUDView();
    void showHUDView();
    
    //    HUD
protected:
    CASlider            *_slider;
    CAView              *_HUDView;
        
protected:
    void onButtonBack(CAControl* control, CCPoint point);
    void onButtonPause(CAControl* control, CCPoint point);
    void buildHUD();
    void updateHUD(float);
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

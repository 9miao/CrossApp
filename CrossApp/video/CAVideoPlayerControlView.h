//
//  CAVideoPlayerControlView.h
//  CrossApp
//
//  Created by zhujian on 15-9-16.
//
//  Copyright (c) 2015年 http://www.9miao.com. All rights reserved.
//

#ifndef __CrossApp__CAVideoPlayerControlView__
#define __CrossApp__CAVideoPlayerControlView__

#include "controller/CAViewController.h"
#include "control/CASlider.h"
#include "control/CAButton.h"
#include "CAVideoPlayerView.h"

NS_CC_BEGIN

class CAVideoPlayerView;
class VPAudioFrame;
class VPFrame;
class VPArtworkFrame;
class VPDecoder;



class CC_DLL CAVideoPlayerControlView : public CAView
{
public:
	CAVideoPlayerControlView();
	virtual ~CAVideoPlayerControlView();


protected:
	CAVideoPlayerView *m_glView;



//    bool                _buffered;
    string              _path;
    string              _title;
    CAActivityIndicatorView *_activityView;
    CAButton            *_playButton;
    CASlider            *_playSlider;
    CALabel             *_playTime;

    
protected:
    virtual void viewDidLoad();
    virtual void viewDidUnload();
//    virtual void prepare(float);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    

    void updatePlayButton();
 //   void tick(float dt);
  
 //   void setMoviePositionFromDecoder();
 //   void setDecoderPosition(float position);
 //   void setMoviePosition(float position);
 //   void updatePosition(float position, bool playing);
 //   void gotoWantedMoviePosition(float);
    
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

};

NS_CC_END

#endif /* defined(__CrossApp__CAVideoPlayerControlView__) */

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
#include "CAVideoPlayerControlView.h"

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

 
protected:
    virtual void viewDidLoad();
    virtual void viewDidUnload();
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
};

NS_CC_END

#endif /* defined(__CrossApp__CAVideoPlayerController__) */

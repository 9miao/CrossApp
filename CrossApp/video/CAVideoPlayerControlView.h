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
#include "video/CAVideoPlayerView.h"

NS_CC_BEGIN


class CC_DLL CAVideoPlayerControlView : public CAView
{
public:
	CAVideoPlayerControlView();
	virtual ~CAVideoPlayerControlView();

	static CAVideoPlayerControlView* createWithFrame(const DRect& rect);
	static CAVideoPlayerControlView* createWithCenter(const DRect& rect);

	CC_SYNTHESIZE(std::string, m_szTitle, Title);
	
	void initWithPath(const std::string& szPath);
	void initWithUrl(const std::string& szUrl);

protected:
	virtual bool init();
//	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	void onSlideTouched(CAControl* control, DPoint point);
	void onSlideChanged(CAControl* control, DPoint point);
	void onButtonPause(CAControl* control, DPoint point);
	void onButtonBack(CAControl* control, DPoint point);
	void updatePlayUI(float t);
	void buildCtrlViews();
	void updatePlayButton();
	std::string formatTimeInterval(float seconds, bool isLeft);

private:
	CAVideoPlayerView *m_glView;
	CAActivityIndicatorView *m_actView;
	CAButton *m_playButton;
	CASlider *m_playSlider;
	CALabel *m_playTimeLabel;
};

NS_CC_END

#endif /* defined(__CrossApp__CAVideoPlayerControlView__) */

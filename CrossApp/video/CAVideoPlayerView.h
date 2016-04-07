//
//  CAVideoPlayerView.h
//  CrossApp
//
//  Created by dai xinping on 14-11-6.
//  Modified by zhujian on 15-9-6
//
//  Copyright (c) 2015年 http://www.9miao.com. All rights reserved.
//

#ifndef __CrossApp__CAVideoPlayerView__
#define __CrossApp__CAVideoPlayerView__

#include "view/CAView.h"
#include "view/CAImageView.h"
#include "view/CARenderImage.h"
#include "view/CAActivityIndicatorView.h"
#include "basics/CAThread.h"
#include "basics/CASyncQueue.h"
#include "CAVideoPlayerRender.h"
#include "CAVideoPlayerDecoder.h"

NS_CC_BEGIN

class CAVideoPlayerView;
class CAVideoPlayerViewDelegate
{
public:
	virtual ~CAVideoPlayerViewDelegate(){};

	virtual void movieFinishedCallback(CAVideoPlayerView *playerView) {};
};

class CC_DLL CAVideoPlayerView : public CAView, public CAThread
{
public:
    CAVideoPlayerView();
	virtual ~CAVideoPlayerView();
    
	static CAVideoPlayerView* create();
	static CAVideoPlayerView* createWithFrame(const DRect& rect);
	static CAVideoPlayerView* createWithCenter(const DRect& rect);
    static CAVideoPlayerView* createWithLayout(const DRectLayout& layout);
    
	void setFullPath(const std::string& szPath, bool showFirstFrame = true);
	void setUrl(const std::string& szUrl, bool showFirstFrame = true);
	bool isDecoderInited() { return (m_iDecoderInited==1); }
	bool isWaitSetPos() { return m_isSetPosWaiting; }
	void play();
	void pause();
	bool isPlaying();
	void enableAudio(bool on);
	float getDuration();
	float getPosition();
	void setPosition(float position);
	void setFirstVideoFrame();
	

	CC_SYNTHESIZE(CAVideoPlayerViewDelegate*, m_pPlayerViewDelegate, PlayerViewDelegate);

private:
	virtual bool init();
	virtual void visit();
	virtual void draw();
	virtual void update(float fDelta);
	virtual void setContentSize(const DSize& size);
	virtual void setImageCoords(DRect rect);
	virtual void updateImageRect();

	void showLoadingView(bool on);
	void createDecoderSync();
	bool createDecoder();
	static bool decodeProcessThread(void* param);
	void setVPPosition(float p);
	void decodeProcess();
	void asyncDecodeFrames();
	void setDecodePosition(float pos);
	bool addFrames(const std::vector<VPFrame*>& frames);
	float presentFrame();
	void setCurrentFrame(VPVideoFrame* frame);
	void tick(float dt);
	float tickCorrection();
	void audioCallback(unsigned char *stream, int len, int channels);

private:
	VPDecoder *m_pDecoder;
	VPFrameRender *m_pRenderer;

	CASyncQueue<VPFrame*> m_vVideoFrames;
	CASyncQueue<VPFrame*> m_vAudioFrames;

	DRect m_viewRect;

	std::string m_cszPath;

	int m_iDecoderInited;
	bool m_isShowFirstFrame;
	bool m_isPlaying;
	bool m_isBuffered;
	bool m_isSetPosWaiting;

	float m_fMinBufferedDuration;
	float m_fMaxBufferedDuration;

	float m_fBufferedDuration;
	float m_fMoviePosition;

	float m_tickCorrectionPosition;
	struct timeval m_tickCorrectionTime;

	VPVideoFrame *m_pCurVideoFrame;
	VPAudioFrame *m_pCurAudioFrame;
	unsigned int m_uCurAudioFramePos;
	CALock m_aLock;

	CAActivityIndicatorView* m_pLoadingView;
};

NS_CC_END


#endif /* defined(__CrossApp__CAVideoPlayerView__) */

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


class CC_DLL CAVideoPlayerView : public CAView, public CAThread
{
public:
    CAVideoPlayerView();
	virtual ~CAVideoPlayerView();
    
	static CAVideoPlayerView* create();
	static CAVideoPlayerView* createWithFrame(const DRect& rect);
	static CAVideoPlayerView* createWithCenter(const DRect& rect);

	bool initWithPath(const std::string& szPath);
	bool initWithUrl(const std::string& szUrl);
	void play();
	void pause();
	bool isPlaying();
	void enableAudio(bool on);
	float getDuration();
	float getPosition();
	void setPosition(float position);

private:
	virtual bool init();
	virtual void visit();
	virtual void draw();
	virtual void setContentSize(const DSize& size);
	virtual void setImageCoords(DRect rect);
	virtual void updateImageRect();

	void showLoadingView(bool on);
	bool createDecoder(const std::string& cszPath);
	static bool decodeProcessThread(void* param);
	void setVPPosition(float p);
	void decodeProcess();
	void asyncDecodeFrames();
	void setDecodePosition(float pos);
	bool addFrames(const std::vector<VPFrame*>& frames);
	float presentFrame();
	void setCurrentFrame(VPVideoFrame* frame);
	void tick(float dt);
	void audioCallback(unsigned char *stream, int len, int channels);

private:
	VPDecoder *m_pDecoder;
	VPFrameRender *m_pRenderer;

	CASyncQueue<VPFrame*> m_vVideoFrames;
	CASyncQueue<VPFrame*> m_vAudioFrames;

	DRect m_viewRect;

	std::string m_cszPath;

	bool m_isPlaying;

	float m_fMinBufferedDuration;
	float m_fMaxBufferedDuration;

	float m_fBufferedDuration;
	float m_fMoviePosition;

	VPVideoFrame *m_pCurVideoFrame;
	VPAudioFrame *m_pCurAudioFrame;
	unsigned int m_uCurAudioFramePos;
	CALock m_aLock;

	CAActivityIndicatorView* m_pLoadingView;
};

NS_CC_END


#endif /* defined(__CrossApp__CAVideoPlayerView__) */

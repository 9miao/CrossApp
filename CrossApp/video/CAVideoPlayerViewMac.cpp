#include "CAVideoPlayerView.h"

NS_CC_BEGIN


CAVideoPlayerView::CAVideoPlayerView()
: m_pRenderer(NULL)
, m_pDecoder(NULL)
, m_isPlaying(false)
, m_fMinBufferedDuration(0)
, m_fMaxBufferedDuration(0)
, m_fBufferedDuration(0)
, m_fMoviePosition(0)
, m_pCurVideoFrame(NULL)
, m_pCurAudioFrame(NULL)
, m_uCurAudioFramePos(0)
{
}

CAVideoPlayerView::~CAVideoPlayerView()
{

}

CAVideoPlayerView* CAVideoPlayerView::create()
{
    CAVideoPlayerView* view = new CAVideoPlayerView();
    if (view && view->init()) 
	{
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

CAVideoPlayerView* CAVideoPlayerView::createWithCenter(const DRect &rect)
{
    CAVideoPlayerView* view = new CAVideoPlayerView;
    if (view && view->initWithCenter(rect))
	{
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    
    return NULL;
}

CAVideoPlayerView* CAVideoPlayerView::createWithFrame(const DRect &rect)
{
    CAVideoPlayerView* view = new CAVideoPlayerView;
    if (view && view->initWithFrame(rect)) 
	{
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    
    return NULL;
}

bool CAVideoPlayerView::init()	
{
    if (!CAView::init()) {
        return false;
    }
    
    setColor(ccc4(0, 0, 0, 255));
    return true;
}

bool CAVideoPlayerView::decodeProcessThread(void* param)
{
	return true;
}

void CAVideoPlayerView::initWithPath(const std::string& szPath)
{
//	return 0;
}

void CAVideoPlayerView::initWithUrl(const std::string& szUrl)
{
//	return 0;
}

void CAVideoPlayerView::setContentSize(const DSize& size)
{
    CAView::setContentSize(size);
}

void CAVideoPlayerView::setImageCoords(DRect rect)
{
    CAView::setImageCoords(rect);
}

void CAVideoPlayerView::updateImageRect()
{
    CAView::updateImageRect();
}

void CAVideoPlayerView::visit()
{
    CAView::visit();
}

void CAVideoPlayerView::draw()
{

}

void CAVideoPlayerView::setCurrentFrame(VPVideoFrame *frame)
{

}

void CAVideoPlayerView::play()
{

}

void CAVideoPlayerView::pause()
{

}

bool CAVideoPlayerView::isPlaying()
{
	return m_isPlaying;
}

void CAVideoPlayerView::enableAudio(bool on)
{
}

float CAVideoPlayerView::getDuration()
{
	return 0;
}

float CAVideoPlayerView::getPosition()
{
	return 0;
}

void CAVideoPlayerView::setPosition(float position)
{

}

bool CAVideoPlayerView::createDecoder()
{

	return true;
}

void CAVideoPlayerView::decodeProcess()
{

}


bool CAVideoPlayerView::addFrames(const std::vector<VPFrame*>& frames)
{
	return 0;
}

void CAVideoPlayerView::asyncDecodeFrames()
{

}

float CAVideoPlayerView::presentFrame()
{

	return 0;
}

void CAVideoPlayerView::tick(float dt)
{

}

void CAVideoPlayerView::audioCallback(unsigned char *stream, int len, int channels)
{
}

NS_CC_END
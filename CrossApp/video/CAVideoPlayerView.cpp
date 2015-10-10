#include "CAVideoPlayerView.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#include "view/CADrawingPrimitives.h"
//#include "SDL.h"

NS_CC_BEGIN

#define LOCAL_MIN_BUFFERED_DURATION   0.2
#define LOCAL_MAX_BUFFERED_DURATION   0.4
#define NETWORK_MIN_BUFFERED_DURATION 2.0
#define NETWORK_MAX_BUFFERED_DURATION 20.0


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
	CAScheduler::schedule(schedule_selector(CAVideoPlayerView::tick), this, 0);
}

CAVideoPlayerView::~CAVideoPlayerView()
{
	CAScheduler::unschedule(schedule_selector(CAVideoPlayerView::tick), this);

	CAThread::close();
	CC_SAFE_DELETE(m_pRenderer);
	CC_SAFE_DELETE(m_pDecoder);
	freeBufferedFrames();
	CC_SAFE_DELETE(m_pCurAudioFrame);
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
	CAVideoPlayerView* pVideoView = (CAVideoPlayerView*)param;

	pVideoView->retain();
	pVideoView->decodeProcess();
	pVideoView->release();

	return true;
}

bool CAVideoPlayerView::initWithPath(const std::string& szPath)
{
	m_fMinBufferedDuration = LOCAL_MIN_BUFFERED_DURATION;
	m_fMaxBufferedDuration = LOCAL_MAX_BUFFERED_DURATION;

	return createDecoder(szPath);
}

bool CAVideoPlayerView::initWithUrl(const std::string& szUrl)
{
	m_fMinBufferedDuration = NETWORK_MIN_BUFFERED_DURATION;
	m_fMaxBufferedDuration = NETWORK_MAX_BUFFERED_DURATION;

	return createDecoder(szUrl);
}

void CAVideoPlayerView::setContentSize(const DSize& size)
{
    CAView::setContentSize(size);
    
	if (m_pRenderer)
	{
		m_viewRect = m_pRenderer->updateVertices(
			m_pDecoder->getFrameWidth(), m_pDecoder->getFrameHeight(), getFrame().size.width, getFrame().size.height);
	}
	setImageRect(m_viewRect);
}

void CAVideoPlayerView::setImageCoords(DRect rect)
{
    float left = 0, right = 1, top = 0, bottom = 1;
    
    if (m_bRectRotated)
    {
        if (m_bFlipX)
        {
            CC_SWAP(top, bottom, float);
        }
        
        if (m_bFlipY)
        {
            CC_SWAP(left, right, float);
        }
        
        m_sQuad.bl.texCoords.u = left;
        m_sQuad.bl.texCoords.v = top;
        m_sQuad.br.texCoords.u = left;
        m_sQuad.br.texCoords.v = bottom;
        m_sQuad.tl.texCoords.u = right;
        m_sQuad.tl.texCoords.v = top;
        m_sQuad.tr.texCoords.u = right;
        m_sQuad.tr.texCoords.v = bottom;
    }
    else
    {
        if(m_bFlipX)
        {
            CC_SWAP(left,right,float);
        }
        
        if(m_bFlipY)
        {
            CC_SWAP(top,bottom,float);
        }
        
        m_sQuad.bl.texCoords.u = left;
        m_sQuad.bl.texCoords.v = bottom;
        m_sQuad.br.texCoords.u = right;
        m_sQuad.br.texCoords.v = bottom;
        m_sQuad.tl.texCoords.u = left;
        m_sQuad.tl.texCoords.v = top;
        m_sQuad.tr.texCoords.u = right;
        m_sQuad.tr.texCoords.v = top;
    }
}

void CAVideoPlayerView::updateImageRect()
{
    // Don't update Z.
    float m_fLeft, m_fTop, m_fRight, m_fBottom;
    
	m_fLeft = m_viewRect.origin.x;
	m_fRight = m_viewRect.origin.x + m_viewRect.size.width;
	m_fTop = m_viewRect.origin.y;
	m_fBottom = m_viewRect.origin.y + m_viewRect.size.height;
    
    m_sQuad.bl.vertices = vertex3(m_fLeft, m_fTop, 0);
    m_sQuad.br.vertices = vertex3(m_fRight, m_fTop, 0);
    m_sQuad.tl.vertices = vertex3(m_fLeft, m_fBottom, 0);
    m_sQuad.tr.vertices = vertex3(m_fRight, m_fBottom, 0);
}

void CAVideoPlayerView::visit()
{
    CAView::visit();
    
    updateDraw();
}


void CAVideoPlayerView::draw()
{
    long offset = (long)&m_sQuad;
    
	if (m_pCurVideoFrame && m_pRenderer) {
		m_pRenderer->draw(m_pCurVideoFrame, offset);
	}
}

void CAVideoPlayerView::setCurrentFrame(VPVideoFrame *frame)
{
	CC_SAFE_DELETE(m_pCurVideoFrame);
	m_pCurVideoFrame = frame;
}

void CAVideoPlayerView::play()
{
	if (isPlaying())
		return;

	m_isPlaying = true;
	this->enableAudio(true);
	asyncDecodeFrames();
}

void CAVideoPlayerView::pause()
{
	if (!this->isPlaying())
		return;
	
	m_isPlaying = false;
	this->enableAudio(false);
}

bool CAVideoPlayerView::isPlaying()
{
	return m_isPlaying;
}

void CAVideoPlayerView::enableAudio(bool on)
{
//	SDL_PauseAudio(!on);
}

float CAVideoPlayerView::getDuration()
{
	if (m_pDecoder)
	{
		return m_pDecoder->getDuration();
	}
	return 0;
}

float CAVideoPlayerView::getPosition()
{
	if (m_pDecoder)
	{
		return m_fMoviePosition - m_pDecoder->getStartTime();
	}
	return 0;
}

void CAVideoPlayerView::setPosition(float position)
{
	if (m_pDecoder == NULL)
		return;
	
	freeBufferedFrames();
	position = MIN(m_pDecoder->getDuration(), MAX(0, position));
	m_pDecoder->setPosition(position);
	m_fMoviePosition = position + m_pDecoder->getStartTime();
}

bool CAVideoPlayerView::createDecoder(const std::string& cszPath)
{
	m_pDecoder = new VPDecoder();
	if (m_pDecoder == NULL)
	{
		return false;
	}

	if (!m_pDecoder->openFile(cszPath))
	{
		CC_SAFE_DELETE(m_pDecoder);
		return false;
	}

	if (!m_pDecoder->isValidVideo())
		m_fMinBufferedDuration *= 10.0; // increase for audio

	m_pDecoder->setAudioCallback(this, decoder_audio_selector(CAVideoPlayerView::audioCallback));

	if (m_pDecoder->setupVideoFrameFormat(kVideoFrameFormatYUV))
	{
		m_pRenderer = new VPFrameRenderYUV();
	}
	else
	{
		m_pRenderer = new VPFrameRenderRGB();
	}

	if (!m_pRenderer->loadShaders())
	{
		CC_SAFE_DELETE(m_pRenderer);
		CC_SAFE_DELETE(m_pDecoder);
		return false;
	}
	setFrame(getFrame());

	CAThread::startAndWait(decodeProcessThread);
	return true;
}

void CAVideoPlayerView::decodeProcess()
{
	if (!m_isPlaying)
		return;
	
	bool good = true;

	while (good) {
		good = false;
		if (m_pDecoder && (m_pDecoder->isValidVideo() || m_pDecoder->isValidAudio())) {
			std::vector<VPFrame*> frames = m_pDecoder->decodeFrames(0.1f);
			if (!frames.empty()) 
			{
				good = addFrames(frames);
			}
		}
	}
}


bool CAVideoPlayerView::addFrames(const std::vector<VPFrame*>& frames)
{
	std::vector<VPFrame*>::iterator obj;

	if (m_pDecoder->isValidVideo()) {
		for (int i = 0; i < frames.size(); i++) {
			VPFrame* frame = frames.at(i);
			if (frame && frame->getType() == kFrameTypeVideo) {
				m_vVideoFrames.AddElement(frame);
				m_fBufferedDuration += frame->getDuration();
			}
		}
	}

	if (m_pDecoder->isValidAudio()) {
		for (int i = 0; i < frames.size(); i++) {
			VPFrame* frame = frames.at(i);
			if (frame && frame->getType() == kFrameTypeAudio) {
				m_vAudioFrames.AddElement(frame);
				if (!m_pDecoder->isValidVideo()) {
					m_fBufferedDuration += frame->getDuration();
				}
			}
		}
	}

	return m_isPlaying && m_fBufferedDuration < m_fMaxBufferedDuration;
}

void CAVideoPlayerView::asyncDecodeFrames()
{
	CAThread::notifyRun(this);
}

void CAVideoPlayerView::freeBufferedFrames()
{
	CAThread::clear();

	m_vVideoFrames.Clear();
	m_vAudioFrames.Clear();

	m_fBufferedDuration = 0;
	m_fMoviePosition = 0;

	CC_SAFE_DELETE(m_pCurVideoFrame);

	m_uCurAudioFramePos = 0;
}

float CAVideoPlayerView::presentFrame()
{
	float interval = 0;

	if (m_pDecoder->isValidVideo()) {

		VPFrame *frame = NULL;
		if (m_vVideoFrames.PopElement(frame))
		{
			interval = frame->getDuration();

			float fCurPos = frame->getPosition();

			if (fCurPos >= m_fMoviePosition)
			{
				m_fBufferedDuration -= frame->getDuration();
				m_fMoviePosition = frame->getPosition();
			}
			else
			{
				CC_SAFE_DELETE(frame);
			}
		}

		if (m_pDecoder->getDuration() - m_fMoviePosition < 0.1f) {

			pause();
			m_pDecoder->setPosition(0);
			freeBufferedFrames();
		}
		setCurrentFrame((VPVideoFrame*)frame);
	}
	return interval;
}

void CAVideoPlayerView::tick(float dt)
{
	if (m_pDecoder == NULL)
		return;
	
	if (!m_isPlaying)
		return;

	float interval = presentFrame();

	const unsigned int leftFrames =
		(m_pDecoder->isValidVideo() ? m_vVideoFrames.GetCount() : 0) +
		(m_pDecoder->isValidAudio() ? m_vAudioFrames.GetCount() : 0);

	if (0 == leftFrames)
	{
		if (m_pDecoder->isEOF()) {
			pause();
			return;
		}
	}

	if (m_fBufferedDuration < m_fMaxBufferedDuration)
	{
		asyncDecodeFrames();
	}

	float time = MAX(interval, 0.01);
	CAScheduler::schedule(schedule_selector(CAVideoPlayerView::tick), this, time);
}

void CAVideoPlayerView::audioCallback(unsigned char *stream, int len, int channels)
{
	while (len > 0) 
	{
		if (!isPlaying())
			return;

		if (m_pCurAudioFrame == NULL)
		{
			VPFrame* frame = NULL;
			if (m_vAudioFrames.PopElement(frame))
			{
				m_pCurAudioFrame = (VPAudioFrame*)frame;
			}
			m_uCurAudioFramePos = 0;
		}

		if (m_pCurAudioFrame)
		{
			if (m_pDecoder->isValidAudio())
			{
				const float delta = m_fMoviePosition - m_pCurAudioFrame->getPosition();
				if (delta < -0.1) 
				{
					continue;
				}
			}
			else
			{
				m_fMoviePosition = m_pCurAudioFrame->getPosition();
				m_fBufferedDuration -= m_pCurAudioFrame->getDuration();
			}

			unsigned char* bytes = (unsigned char*)(m_pCurAudioFrame->getData() + m_uCurAudioFramePos);
			const unsigned int bytesLeft = m_pCurAudioFrame->getDataLength() - m_uCurAudioFramePos;
			const unsigned int bytesToCopy = MIN(len, bytesLeft);

			memcpy(stream, bytes, bytesToCopy);
			stream += bytesToCopy;
			len -= bytesToCopy;

			if (bytesToCopy < bytesLeft) 
			{
				m_uCurAudioFramePos += bytesToCopy;
			}
			else 
			{
				CC_SAFE_DELETE(m_pCurAudioFrame);
			}
		}
	}
}


NS_CC_END
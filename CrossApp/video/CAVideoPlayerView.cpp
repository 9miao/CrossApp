#include "CAVideoPlayerView.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#include "view/CADrawingPrimitives.h"

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
}

CAVideoPlayerView::~CAVideoPlayerView()
{
	CAThread::close();
	CC_SAFE_DELETE(m_pRenderer);
	CC_SAFE_DELETE(m_pDecoder);
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

CAVideoPlayerView* CAVideoPlayerView::createWithCenter(const CCRect &rect)
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

CAVideoPlayerView* CAVideoPlayerView::createWithFrame(const CCRect &rect)
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

bool CAVideoPlayerView::initWithPath(const std::string& szPath, bool isPathByUrl)
{
	m_pDecoder = new VPDecoder();
	if (m_pDecoder == NULL)
	{
		return false;
	}

	if (!m_pDecoder->openFile(szPath))
	{
		CC_SAFE_DELETE(m_pDecoder);
		return false;
	}

	if (isPathByUrl)
	{
		m_fMinBufferedDuration = NETWORK_MIN_BUFFERED_DURATION;
		m_fMaxBufferedDuration = NETWORK_MAX_BUFFERED_DURATION;
	}
	else 
	{
		m_fMinBufferedDuration = LOCAL_MIN_BUFFERED_DURATION;
		m_fMaxBufferedDuration = LOCAL_MAX_BUFFERED_DURATION;
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

void CAVideoPlayerView::setContentSize(const CCSize& size)
{
    CAView::setContentSize(size);
    
	if (m_pRenderer)
	{
		m_viewRect = m_pRenderer->updateVertices(
			m_pDecoder->getFrameWidth(), m_pDecoder->getFrameHeight(), getFrame().size.width, getFrame().size.height);
	}
	setImageRect(m_viewRect);
}

void CAVideoPlayerView::setImageCoords(CCRect rect)
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
	asyncDecodeFrames();
	tick(0);
}

void CAVideoPlayerView::pause()
{

}

bool CAVideoPlayerView::isPlaying()
{
	return m_isPlaying;
}

float CAVideoPlayerView::getDuration()
{
	if (m_pDecoder)
	{
		return m_pDecoder->getDuration();
	}
	return 0;
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

float CAVideoPlayerView::presentFrame()
{
	float interval = 0;

	if (m_pDecoder->isValidVideo()) {

		VPFrame *frame = NULL;
		if (m_vVideoFrames.PopElement(frame))
		{
			m_fBufferedDuration -= frame->getDuration();
			m_fMoviePosition = frame->getPosition();
			interval = frame->getDuration();
		}

		if (m_pDecoder->getDuration() - m_fMoviePosition < 1) {

			pause();
			//_moviePosition = 0;
			//_currentAudioFramePos = 0;
			m_pDecoder->setPosition(0);
			//setDecoderPosition(0);
			//presentFrame();
			//freeBufferedFrames();
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
/*
void CAVideoPlayerView::audioCallback(unsigned char *stream, int len, int channels)
{
	if (_buffered) {
		memset(stream, 0, len * channels);
		return;
	}

	while (len > 0) {

		unsigned int count = _audioFrames.size();

		if (!_currentAudioFrame) {

			if (count > 0) {
				pthread_mutex_lock(&m_vp_data_mutex);
				VPAudioFrame* frame = (VPAudioFrame*)_audioFrames.at(0);
				pthread_mutex_unlock(&m_vp_data_mutex);


				//CCLog("Audio frame position: %f, %f", _moviePosition, frame->getPosition());

				if (_decoder->isValidVideo()) {
					const float delta = _moviePosition - frame->getPosition();

					if (delta < -0.1) {
						memset(stream, 0, len * channels);
						break;
					}

					pthread_mutex_lock(&m_vp_data_mutex);
					_audioFrames.erase(0);
					pthread_mutex_unlock(&m_vp_data_mutex);

					if (delta > 0.1 && count > 1)
					{
						continue;
					}

				}
				else {
					_audioFrames.erase(0);
					_moviePosition = frame->getPosition();
					_bufferedDuration -= frame->getDuration();
				}

				_currentAudioFramePos = 0;
				CC_SAFE_RELEASE_NULL(_currentAudioFrame);
				_currentAudioFrame = frame;
				//                CC_SAFE_RETAIN(_currentAudioFrame);
			}
		}

		if (_currentAudioFrame) {
			unsigned char* bytes = (unsigned char*)(_currentAudioFrame->getData() + _currentAudioFramePos);
			const unsigned int bytesLeft = _currentAudioFrame->getDataLength() - _currentAudioFramePos;
			const unsigned int bytesToCopy = MIN(len, bytesLeft);

			//            CCLog("%s, copyLen = %d, leftLen = %d, count = %d", __FUNCTION__, bytesToCopy, len - bytesToCopy, count);
			memcpy(stream, bytes, bytesToCopy);
			stream += bytesToCopy;
			len -= bytesToCopy;

			if (bytesToCopy < bytesLeft) {
				_currentAudioFramePos += bytesToCopy;
			}
			else {
				CC_SAFE_RELEASE_NULL(_currentAudioFrame);
			}
		}
		else {
			memset(stream, 0, len * channels);
			return;
		}
	}
}
*/

NS_CC_END
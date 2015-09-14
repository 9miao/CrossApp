#include "CAVideoPlayerView.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#include "view/CADrawingPrimitives.h"

NS_CC_BEGIN

CAVideoPlayerView::CAVideoPlayerView()
: m_pRenderer(NULL)
, m_pDecoder(NULL)
{
}

CAVideoPlayerView::~CAVideoPlayerView()
{
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

bool CAVideoPlayerView::initWithPath(const std::string& szPath)
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
    
//	if (m_pCurFrame && m_pRenderer) {
//		m_pRenderer->draw(m_pCurFrame, offset);
//    }
}

void CAVideoPlayerView::setCurrentFrame(VPVideoFrame *frame)
{
//	CC_SAFE_RELEASE_NULL(m_pCurFrame);
//	m_pCurFrame = frame;
//	CC_SAFE_RETAIN(m_pCurFrame);
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
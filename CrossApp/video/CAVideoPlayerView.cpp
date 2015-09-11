#include "CAVideoPlayerView.h"
#include "basics/CAScheduler.h"
#include "basics/CAApplication.h"
#include "view/CADrawingPrimitives.h"

NS_CC_BEGIN

CAVideoPlayerView::CAVideoPlayerView(VPDecoder* decoder)
: m_pRenderer(NULL)
, m_pCurFrame(NULL)
{
	m_pDecoder = decoder;
}

CAVideoPlayerView::~CAVideoPlayerView()
{
	CC_SAFE_DELETE(m_pRenderer);
	CC_SAFE_RELEASE(m_pCurFrame);
}

CAVideoPlayerView* CAVideoPlayerView::create(VPDecoder* decoder)
{
    CAVideoPlayerView* view = new CAVideoPlayerView(decoder);
    if (view && view->init()) 
	{
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

CAVideoPlayerView* CAVideoPlayerView::createWithCenter(const CCRect &rect, VPDecoder* decoder)
{
    CAVideoPlayerView* view = new CAVideoPlayerView(decoder);
    if (view && view->initWithCenter(rect))
	{
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    
    return NULL;
}

CAVideoPlayerView* CAVideoPlayerView::createWithFrame(const CCRect &rect, VPDecoder* decoder)
{
    CAVideoPlayerView* view = new CAVideoPlayerView(decoder);
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
		delete m_pRenderer;
        return false;
    }
    return true;
}

void CAVideoPlayerView::setContentSize(const CCSize& size)
{
    CAView::setContentSize(size);
    
	CCRect pictureRect = m_pRenderer->updateVertices(
		m_pDecoder->getFrameWidth(), m_pDecoder->getFrameHeight(), getFrame().size.width, getFrame().size.height);
    
	m_viewRect = pictureRect;
	setImageRect(pictureRect);
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
    
	if (m_pCurFrame && m_pRenderer) {
		m_pRenderer->draw(m_pCurFrame, offset);
    }
}

void CAVideoPlayerView::setCurrentFrame(VPVideoFrame *frame)
{
	CC_SAFE_RELEASE_NULL(m_pCurFrame);
	m_pCurFrame = frame;
	CC_SAFE_RETAIN(m_pCurFrame);
}


NS_CC_END
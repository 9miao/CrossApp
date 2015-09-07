

#include "CARenderImage.h"
#include "basics/CAApplication.h"
#include "platform/platform.h"
#include "shaders/CAGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "support/ccUtils.h"
#include "images/CAImageCache.h"
#include "platform/CCFileUtils.h"
#include "CCGL.h"
#include "support/CANotificationCenter.h"
#include "ccTypes.h"
// extern
#include "kazmath/GL/matrix.h"
#include "CCEGLView.h"

NS_CC_BEGIN

// implementation CARenderImage
CARenderImage::CARenderImage()
: m_pImageView(NULL)
, m_uFBO(0)
, m_uDepthRenderBufffer(0)
, m_nOldFBO(0)
, m_pImage(0)
, m_ePixelFormat(CAImage::PixelFormat_RGBA8888)
, m_uClearFlags(0)
, m_sClearColor(ccc4f(0,0,0,1))
, m_fClearDepth(0.0f)
, m_nClearStencil(0)
, m_bAutoDraw(false)
, m_uPixelsWide(0)
, m_uPixelsHigh(0)
, m_uName(0)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // Listen this event to save render Image before come to background.
    // Then it can be restored after coming to foreground on Android.
    CANotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(CARenderImage::listenToBackground),
                                                                  EVENT_COME_TO_BACKGROUND,
                                                                  NULL);
    
    CANotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(CARenderImage::listenToForeground),
                                                                  EVENT_COME_TO_FOREGROUND, // this is misspelt
                                                                  NULL);
#endif

}

CARenderImage::~CARenderImage()
{
    CC_SAFE_RELEASE(m_pImageView);
    CC_SAFE_RELEASE(m_pImage);
    
    glDeleteFramebuffers(1, &m_uFBO);
    if (m_uDepthRenderBufffer)
    {
        glDeleteRenderbuffers(1, &m_uDepthRenderBufffer);
    }
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CANotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_BACKGROUND);
    CANotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
#endif

}

void CARenderImage::listenToBackground(CrossApp::CAObject *obj)
{

}

void CARenderImage::listenToForeground(CrossApp::CAObject *obj)
{
// -- regenerate frame buffer object and attach the texture
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
    
    glGenFramebuffers(1, &m_uFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
    
    m_pImage->setAliasTexParameters();
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pImage->getName(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);

}

CAImageView * CARenderImage::getImageView()
{
    return m_pImageView;
}

void CARenderImage::setImageView(CAImageView* var)
{
    CC_SAFE_RELEASE(m_pImageView);
    m_pImageView = var;
    CC_SAFE_RETAIN(m_pImageView);
}

unsigned int CARenderImage::getClearFlags() const
{
    return m_uClearFlags;
}

void CARenderImage::setClearFlags(unsigned int uClearFlags)
{
    m_uClearFlags = uClearFlags;
}

const CAColor4F& CARenderImage::getClearColor() const
{
    return m_sClearColor;
}

void CARenderImage::setClearColor(const CAColor4F &clearColor)
{
    m_sClearColor = clearColor;
}

float CARenderImage::getClearDepth() const
{
    return m_fClearDepth;
}

void CARenderImage::setClearDepth(float fClearDepth)
{
    m_fClearDepth = fClearDepth;
}

int CARenderImage::getClearStencil() const
{
    return m_nClearStencil;
}

void CARenderImage::setClearStencil(float fClearStencil)
{
    m_nClearStencil = fClearStencil;
}

bool CARenderImage::isAutoDraw() const
{
    return m_bAutoDraw;
}

void CARenderImage::setAutoDraw(bool bAutoDraw)
{
    m_bAutoDraw = bAutoDraw;
}

CARenderImage * CARenderImage::create(int w, int h, CAImage::PixelFormat eFormat)
{
    CARenderImage *pRet = new CARenderImage();

    if(pRet && pRet->initWithWidthAndHeight(w, h, eFormat))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CARenderImage * CARenderImage::create(int w ,int h, CAImage::PixelFormat eFormat, GLuint uDepthStencilFormat)
{
    CARenderImage *pRet = new CARenderImage();

    if(pRet && pRet->initWithWidthAndHeight(w, h, eFormat, uDepthStencilFormat))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CARenderImage * CARenderImage::create(int w, int h)
{
    CARenderImage *pRet = new CARenderImage();

    if(pRet && pRet->initWithWidthAndHeight(w, h, CAImage::PixelFormat_RGBA8888, 0))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CARenderImage::initWithWidthAndHeight(int w, int h, CAImage::PixelFormat eFormat)
{
    return initWithWidthAndHeight(w, h, eFormat, 0);
}

bool CARenderImage::initWithWidthAndHeight(int w, int h, CAImage::PixelFormat eFormat, GLuint uDepthStencilFormat)
{
    CCAssert(eFormat != CAImage::PixelFormat_A8, "only RGB and RGBA formats are valid for a render texture");

    bool bRet = false;
    unsigned char *data = NULL;
    do 
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);

        // textures must be power of two squared
        unsigned int powW = (unsigned int)w;
        unsigned int powH = (unsigned int)h;

        data = (unsigned char *)malloc((int)(powW * powH * 4));
        CC_BREAK_IF(! data);

        memset(data, 0, (int)(powW * powH * 4));
        m_ePixelFormat = eFormat;
        m_uPixelsWide = powW;
        m_uPixelsHigh = powH;
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
        glGenTextures(1, &m_uName);
        ccGLBindTexture2D(m_uName);
        
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uPixelsWide, m_uPixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        GLint oldRBO;
        glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRBO);
        
        // generate FBO
        glGenFramebuffers(1, &m_uFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);

        // associate Image with FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uName, 0);

        if (uDepthStencilFormat != 0)
        {
            //create and attach depth buffer
            glGenRenderbuffers(1, &m_uDepthRenderBufffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_uDepthRenderBufffer);
            glRenderbufferStorage(GL_RENDERBUFFER, uDepthStencilFormat, (GLsizei)powW, (GLsizei)powH);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uDepthRenderBufffer);

            // if depth format is the one with stencil part, bind same render buffer as stencil attachment
            if (uDepthStencilFormat == GL_DEPTH24_STENCIL8)
            {
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uDepthRenderBufffer);
            }
        }
//        // check if it worked (probably worth doing :) )
        CCAssert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not attach Image to framebuffer");

        ccGLBindTexture2D( m_uName );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

        CAImageView* imageView = CAImageView::createWithFrame(CCRect(0, 0, m_uPixelsWide, m_uPixelsHigh));
        ccBlendFunc tBlendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
        imageView->setBlendFunc(tBlendFunc);
        this->addSubview(imageView);
        this->setImageView(imageView);
        
        glBindRenderbuffer(GL_RENDERBUFFER, oldRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
        
        // Diabled by default.
        m_bAutoDraw = false;

        bRet = true;
    } while (0);
    
    CC_SAFE_FREE(data);
    
    return bRet;
}

void CARenderImage::printscreenWithView(CAView* view)
{
    this->printscreenWithView(view, CCPointZero);
}

void CARenderImage::printscreenWithView(CAView* view, CCPoint offset)
{
    this->printscreenWithView(view, offset, CAColor_clear);
}

void CARenderImage::printscreenWithView(CAView* view, const CAColor4B& backgroundColor)
{
    this->printscreenWithView(view, CCPointZero, backgroundColor);
}

void CARenderImage::printscreenWithView(CAView* view, CCPoint offset, const CAColor4B& backgroundColor)
{
    CC_RETURN_IF(view == NULL);
    
    CCPoint point = CCPointZero;
    if (view->getSuperview())
    {
        point.y += view->getSuperview()->getFrame().size.height;
    }
    else
    {
        point.y += CAApplication::getApplication()->getWinSize().height;
    }
    point.y -= view->getFrame().size.height;
    point.y += offset.y;
    point.x -= offset.x;
    
    CCPoint originalFrameOrigin = view->getFrameOrigin();
    CCPoint originalAnchorPoint = view->getAnchorPoint();
    float originalRotationX = view->getRotationX();
    
    view->setAnchorPoint(CCPoint(0.5f, 0.5f));
    view->setRotationX(originalRotationX + 180);
    view->setFrameOrigin(point);
    
    this->beginWithClear(backgroundColor);
    view->visit();
    this->end();
    
    view->setRotationX(originalRotationX);
    view->setAnchorPoint(originalAnchorPoint);
    view->setFrameOrigin(originalFrameOrigin);
}

void CARenderImage::begin()
{
    kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLPushMatrix();
	kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLPushMatrix();

    // Calculate the adjustment ratios based on the old and new projections
    CCSize size = CAApplication::getApplication()->getWinSize();
    float widthRatio = size.width / m_uPixelsWide;
    float heightRatio = size.height / m_uPixelsHigh;

    // Adjust the orthographic projection and viewport
    glViewport(0, 0, (GLsizei)m_uPixelsWide, (GLsizei)m_uPixelsHigh);


    kmMat4 orthoMatrix;
    kmMat4OrthographicProjection(&orthoMatrix,
                                 (float)-1.0 / widthRatio,
                                 (float)1.0 / widthRatio,
                                 (float)-1.0 / heightRatio,
                                 (float)1.0 / heightRatio,
                                 -1,
                                 1);
    kmGLMultMatrix(&orthoMatrix);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
    
}

void CARenderImage::beginWithClear(const CAColor4B& backgroundColor)
{
    beginWithClear(backgroundColor, 0, 0, GL_COLOR_BUFFER_BIT);
}

void CARenderImage::beginWithClear(const CAColor4B& backgroundColor, float depthValue)
{
    beginWithClear(backgroundColor, depthValue, 0, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void CARenderImage::beginWithClear(const CAColor4B& backgroundColor, float depthValue, int stencilValue)
{
    beginWithClear(backgroundColor, depthValue, stencilValue, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}

void CARenderImage::beginWithClear(const CAColor4B& backgroundColor, float depthValue, int stencilValue, GLbitfield flags)
{
    this->begin();

    if (backgroundColor.a > 0)
    {
        // save clear color
        GLfloat	clearColor[4] = {0.0f};
        GLfloat depthClearValue = 0.0f;
        int stencilClearValue = 0;
        
        if (flags & GL_COLOR_BUFFER_BIT)
        {
            glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
            glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        }
        
        if (flags & GL_DEPTH_BUFFER_BIT)
        {
            glGetFloatv(GL_DEPTH_CLEAR_VALUE, &depthClearValue);
            glClearDepth(depthValue);
        }
        
        if (flags & GL_STENCIL_BUFFER_BIT)
        {
            glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &stencilClearValue);
            glClearStencil(stencilValue);
        }
        
        glClear(flags);
        
        // restore
        if (flags & GL_COLOR_BUFFER_BIT)
        {
            glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        }
        if (flags & GL_DEPTH_BUFFER_BIT)
        {
            glClearDepth(depthClearValue);
        }
        if (flags & GL_STENCIL_BUFFER_BIT)
        {
            glClearStencil(stencilClearValue);
        }
    }
}

void CARenderImage::end()
{
    GLubyte *pBuffer = pBuffer = new GLubyte[m_uPixelsWide * m_uPixelsHigh * 4];
    GLubyte *pTempData = new GLubyte[m_uPixelsWide * m_uPixelsHigh * 4];
    
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, m_uPixelsWide, m_uPixelsHigh, GL_RGBA, GL_UNSIGNED_BYTE, pTempData);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);

    CAApplication::getApplication()->setViewport();

    kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLPopMatrix();
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLPopMatrix();

    m_pImage = new CAImage();
    m_pImage->initWithRawData(pTempData, CAImage::PixelFormat_RGBA8888, m_uPixelsWide, m_uPixelsHigh);
    m_pImageView->setImage(m_pImage);
    
    CC_SAFE_DELETE_ARRAY(pBuffer);
    CC_SAFE_DELETE_ARRAY(pTempData);
}

void CARenderImage::clear(const CAColor4B& backgroundColor)
{
    this->beginWithClear(backgroundColor);
    this->end();
}

void CARenderImage::clearDepth(float depthValue)
{
    this->begin();
    //! save old depth value
    GLfloat depthClearValue;
    glGetFloatv(GL_DEPTH_CLEAR_VALUE, &depthClearValue);

    glClearDepth(depthValue);
    glClear(GL_DEPTH_BUFFER_BIT);

    // restore clear color
    glClearDepth(depthClearValue);
    this->end();
}

void CARenderImage::clearStencil(int stencilValue)
{
    // save old stencil value
    int stencilClearValue;
    glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &stencilClearValue);

    glClearStencil(stencilValue);
    glClear(GL_STENCIL_BUFFER_BIT);

    // restore clear color
    glClearStencil(stencilClearValue);
}

void CARenderImage::visit()
{
    // override visit.
	// Don't call visit on its children
    if (!m_bVisible)
    {
        return;
    }
	
	kmGLPushMatrix();
	
    transform();
    if (m_pImageView)
    {
        m_pImageView->visit();
    }
    draw();
	
	kmGLPopMatrix();

    m_uOrderOfArrival = 0;
}

void CARenderImage::draw()
{
    if( m_bAutoDraw)
    {
        begin();
		
        if (m_uClearFlags)
        {
            GLfloat oldClearColor[4] = {0.0f};
			GLfloat oldDepthClearValue = 0.0f;
			GLint oldStencilClearValue = 0;
			
			// backup and set
			if (m_uClearFlags & GL_COLOR_BUFFER_BIT)
            {
				glGetFloatv(GL_COLOR_CLEAR_VALUE, oldClearColor);
				glClearColor(m_sClearColor.r, m_sClearColor.g, m_sClearColor.b, m_sClearColor.a);
			}
			
			if (m_uClearFlags & GL_DEPTH_BUFFER_BIT)
            {
				glGetFloatv(GL_DEPTH_CLEAR_VALUE, &oldDepthClearValue);
				glClearDepth(m_fClearDepth);
			}
			
			if (m_uClearFlags & GL_STENCIL_BUFFER_BIT)
            {
				glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &oldStencilClearValue);
				glClearStencil(m_nClearStencil);
			}
			
			// clear
			glClear(m_uClearFlags);
			
			// restore
			if (m_uClearFlags & GL_COLOR_BUFFER_BIT)
            {
				glClearColor(oldClearColor[0], oldClearColor[1], oldClearColor[2], oldClearColor[3]);
            }
			if (m_uClearFlags & GL_DEPTH_BUFFER_BIT)
            {
				glClearDepth(oldDepthClearValue);
            }
			if (m_uClearFlags & GL_STENCIL_BUFFER_BIT)
            {
				glClearStencil(oldStencilClearValue);
            }
		}
		
		//! make sure all children are drawn
        sortAllSubviews();
		
        CAVector<CAView*>::const_iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
        {
            (*itr)->visit();
        }

        end();
	}
}

bool CARenderImage::saveToFile(const char *szFilePath)
{
    bool bRet = false;
    if (m_pImage)
    {
        bRet = m_pImage->saveToFile(szFilePath);
    }
    return bRet;
}


void CARenderImage::setContentSize(const CCSize& contentSize)
{
    CAView::setContentSize(CCSize(m_uPixelsWide, m_uPixelsHigh));
}
NS_CC_END

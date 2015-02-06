

#include "CARenderImage.h"
#include "basics/CAApplication.h"
#include "platform/platform.h"
#include "platform/CCImage.h"
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
: m_pSprite(NULL)
, m_uFBO(0)
, m_uDepthRenderBufffer(0)
, m_nOldFBO(0)
, m_pTexture(0)
, m_pTextureCopy(0)
, m_pUITextureImage(NULL)
, m_ePixelFormat(kCAImagePixelFormat_RGBA8888)
, m_uClearFlags(0)
, m_sClearColor(ccc4f(0,0,0,0))
, m_fClearDepth(0.0f)
, m_nClearStencil(0)
, m_bAutoDraw(false)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
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
    CC_SAFE_RELEASE(m_pSprite);
    CC_SAFE_RELEASE(m_pTextureCopy);
    
    glDeleteFramebuffers(1, &m_uFBO);
    if (m_uDepthRenderBufffer)
    {
        glDeleteRenderbuffers(1, &m_uDepthRenderBufffer);
    }
    CC_SAFE_DELETE(m_pUITextureImage);

#if CC_ENABLE_CACHE_TEXTURE_DATA
    CANotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_BACKGROUND);
    CANotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
#endif
}

void CARenderImage::listenToBackground(CrossApp::CAObject *obj)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    CC_SAFE_DELETE(m_pUITextureImage);
    
    // to get the rendered Image data
    m_pUITextureImage = newCCImage(false);

    if (m_pUITextureImage)
    {
        const CCSize& s = m_pTexture->getContentSizeInPixels();
        VolatileTexture::addDataTexture(m_pTexture, m_pUITextureImage->getData(), kImagePixelFormat_RGBA8888, s);
        
        if ( m_pTextureCopy )
        {
            VolatileTexture::addDataTexture(m_pTextureCopy, m_pUITextureImage->getData(), kImagePixelFormat_RGBA8888, s);
        }
    }
    else
    {
        CCLOG("Cache rendertexture failed!");
    }
    
    glDeleteFramebuffers(1, &m_uFBO);
    m_uFBO = 0;
#endif
}

void CARenderImage::listenToForeground(CrossApp::CAObject *obj)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    // -- regenerate frame buffer object and attach the texture
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
    
    glGenFramebuffers(1, &m_uFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
    
    m_pTexture->setAliasTexParameters();
    
    if ( m_pTextureCopy )
    {
        m_pTextureCopy->setAliasTexParameters();
    }
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getName(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
#endif
}

CAImageView * CARenderImage::getSprite()
{
    return m_pSprite;
}

void CARenderImage::setSprite(CAImageView* var)
{
    CC_SAFE_RELEASE(m_pSprite);
    m_pSprite = var;
    CC_SAFE_RETAIN(m_pSprite);
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

CARenderImage * CARenderImage::create(int w, int h, CAImagePixelFormat eFormat)
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

CARenderImage * CARenderImage::create(int w ,int h, CAImagePixelFormat eFormat, GLuint uDepthStencilFormat)
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

    if(pRet && pRet->initWithWidthAndHeight(w, h, kCAImagePixelFormat_RGBA8888, 0))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CARenderImage::initWithWidthAndHeight(int w, int h, CAImagePixelFormat eFormat)
{
    return initWithWidthAndHeight(w, h, eFormat, 0);
}

bool CARenderImage::initWithWidthAndHeight(int w, int h, CAImagePixelFormat eFormat, GLuint uDepthStencilFormat)
{
    CCAssert(eFormat != kCAImagePixelFormat_A8, "only RGB and RGBA formats are valid for a render texture");

    bool bRet = false;
    void *data = NULL;
    do 
    {
        w = (int)(w * CC_CONTENT_SCALE_FACTOR());
        h = (int)(h * CC_CONTENT_SCALE_FACTOR());

        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);

        // textures must be power of two squared
        unsigned int powW = 0;
        unsigned int powH = 0;

        {
            //2014.7.21
            powW = ccNextPOT(w);
            powH = ccNextPOT(h);
        }

        data = malloc((int)(powW * powH * 4));
        CC_BREAK_IF(! data);

        memset(data, 0, (int)(powW * powH * 4));
        m_ePixelFormat = eFormat;

        m_pTexture = new CAImage();
        if (m_pTexture)
        {
            m_pTexture->initWithData(data, (CAImagePixelFormat)m_ePixelFormat, powW, powH, CCSizeMake((float)w, (float)h));
        }
        else
        {
            break;
        }
        GLint oldRBO;
        glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRBO);
        
        // generate FBO
        glGenFramebuffers(1, &m_uFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);

        // associate Image with FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getName(), 0);

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

        // check if it worked (probably worth doing :) )
        CCAssert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not attach Image to framebuffer");

        m_pTexture->setAliasTexParameters();

        // retained
        setSprite(CAImageView::createWithImage(m_pTexture));

        m_pTexture->release();
        m_pSprite->setScaleY(-1);

        ccBlendFunc tBlendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
        m_pSprite->setBlendFunc(tBlendFunc);

        glBindRenderbuffer(GL_RENDERBUFFER, oldRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
        
        // Diabled by default.
        m_bAutoDraw = false;
        
        // add sprite for backward compatibility
        addSubview(m_pSprite);
        
        bRet = true;
    } while (0);
    
    CC_SAFE_FREE(data);
    
    return bRet;
}

void CARenderImage::begin()
{
    kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLPushMatrix();
	kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLPushMatrix();
    
    CAApplication *director = CAApplication::getApplication();
    director->setProjection(director->getProjection());

#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8
    kmMat4 modifiedProjection;
    kmGLGetMatrix(KM_GL_PROJECTION, &modifiedProjection);
    kmMat4Multiply(&modifiedProjection, CCEGLView::sharedOpenGLView()->getReverseOrientationMatrix(), &modifiedProjection);
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadMatrix(&modifiedProjection);
    kmGLMatrixMode(KM_GL_MODELVIEW);
#endif

    const CCSize& texSize = m_pTexture->getContentSizeInPixels();

    // Calculate the adjustment ratios based on the old and new projections
    CCSize size = director->getWinSizeInPixels();
    float widthRatio = size.width / texSize.width;
    float heightRatio = size.height / texSize.height;

    // Adjust the orthographic projection and viewport
    glViewport(0, 0, (GLsizei)texSize.width, (GLsizei)texSize.height);


    kmMat4 orthoMatrix;
    kmMat4OrthographicProjection(&orthoMatrix, (float)-1.0 / widthRatio,  (float)1.0 / widthRatio,
        (float)-1.0 / heightRatio, (float)1.0 / heightRatio, -1,1 );
    kmGLMultMatrix(&orthoMatrix);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
    
}

void CARenderImage::beginWithClear(float r, float g, float b, float a)
{
    beginWithClear(r, g, b, a, 0, 0, GL_COLOR_BUFFER_BIT);
}

void CARenderImage::beginWithClear(float r, float g, float b, float a, float depthValue)
{
    beginWithClear(r, g, b, a, depthValue, 0, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void CARenderImage::beginWithClear(float r, float g, float b, float a, float depthValue, int stencilValue)
{
    beginWithClear(r, g, b, a, depthValue, stencilValue, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}

void CARenderImage::beginWithClear(float r, float g, float b, float a, float depthValue, int stencilValue, GLbitfield flags)
{
    this->begin();

    // save clear color
    GLfloat	clearColor[4] = {0.0f};
    GLfloat depthClearValue = 0.0f;
    int stencilClearValue = 0;
    
    if (flags & GL_COLOR_BUFFER_BIT)
    {
        glGetFloatv(GL_COLOR_CLEAR_VALUE,clearColor);
        glClearColor(r, g, b, a);
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

void CARenderImage::end()
{
    CAApplication *director = CAApplication::getApplication();
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);

    // restore viewport
    director->setViewport();

    kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLPopMatrix();
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLPopMatrix();
}

void CARenderImage::clear(float r, float g, float b, float a)
{
    this->beginWithClear(r, g, b, a);
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
    m_pSprite->visit();
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

    CCImage *pImage = newCCImage(true);
    if (pImage)
    {
        bRet = pImage->saveToFile(szFilePath, kCCImageFormatJPEG);
    }

    CC_SAFE_DELETE(pImage);
    return bRet;
}
bool CARenderImage::saveToFile(const char *fileName, tCCImageFormat format)
{
    bool bRet = false;
    CCAssert(format == kCCImageFormatJPEG || format == kCCImageFormatPNG,
             "the image can only be saved as JPG or PNG format");

    CCImage *pImage = newCCImage(true);
    if (pImage)
    {
        std::string fullpath = CCFileUtils::sharedFileUtils()->getWritablePath() + fileName;
        
        bRet = pImage->saveToFile(fullpath.c_str(), true);
    }

    CC_SAFE_DELETE(pImage);

    return bRet;
}

/* get buffer as CCImage */
CCImage* CARenderImage::newCCImage(bool flipImage)
{
    CCAssert(m_ePixelFormat == kCAImagePixelFormat_RGBA8888, "only RGBA8888 can be saved as image");

    if (NULL == m_pTexture)
    {
        return NULL;
    }

    const CCSize& s = m_pTexture->getContentSizeInPixels();

    // to get the image size to save
    //        if the saving image domain exceeds the buffer Image domain,
    //        it should be cut
    int nSavedBufferWidth = (int)s.width;
    int nSavedBufferHeight = (int)s.height;

    GLubyte *pBuffer = NULL;
    GLubyte *pTempData = NULL;
    CCImage *pImage = new CCImage();

    do
    {
        CC_BREAK_IF(! (pBuffer = new GLubyte[nSavedBufferWidth * nSavedBufferHeight * 4]));

        if(! (pTempData = new GLubyte[nSavedBufferWidth * nSavedBufferHeight * 4]))
        {
            delete[] pBuffer;
            pBuffer = NULL;
            break;
        }

        this->begin();
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0,0,nSavedBufferWidth, nSavedBufferHeight,GL_RGBA,GL_UNSIGNED_BYTE, pTempData);
        this->end();

        if ( flipImage ) // -- flip is only required when saving image to file
        {
            // to get the actual Image data
            // #640 the image read from rendertexture is dirty
            for (int i = 0; i < nSavedBufferHeight; ++i)
            {
                memcpy(&pBuffer[i * nSavedBufferWidth * 4], 
                       &pTempData[(nSavedBufferHeight - i - 1) * nSavedBufferWidth * 4], 
                       nSavedBufferWidth * 4);
            }

            pImage->initWithImageData(pBuffer, nSavedBufferWidth * nSavedBufferHeight * 4, CCImage::kFmtRawData, nSavedBufferWidth, nSavedBufferHeight, 8);
        }
        else
        {
            pImage->initWithImageData(pTempData, nSavedBufferWidth * nSavedBufferHeight * 4, CCImage::kFmtRawData, nSavedBufferWidth, nSavedBufferHeight, 8);
        }
        
    } while (0);

    CC_SAFE_DELETE_ARRAY(pBuffer);
    CC_SAFE_DELETE_ARRAY(pTempData);

    return pImage;
}

NS_CC_END



#include "CARenderImage.h"
#include "CCGL.h"
#include "ccTypes.h"
#include <string>
#include "basics/CAApplication.h"
#include "renderer/CCRenderer.h"
#include "renderer/ccGLStateCache.h"
#include "support/ccUtils.h"
#include "images/CAImageCache.h"
#include "basics/CACamera.h"
#include "basics/CAConfiguration.h"
#include "basics/CANotificationCenter.h"
#include "platform/CAFileUtils.h"
#include "platform/CADensityDpi.h"
#include "basics/CANotificationCenter.h"

NS_CC_BEGIN
// implementation CARenderImage
CARenderImage::CARenderImage()
: m_pImageView(nullptr)
, m_uName(0)
, m_uNameCopy(0)
, m_uFBO(0)
, m_uOldFBO(0)
, m_uDepthRenderBufffer(0)
, m_ePixelFormat(CAImage::PixelFormat::RGBA8888)
, m_uClearFlags(0)
, m_sClearColor(CAColor4F(0,0,0,1))
, m_fClearDepth(0.0f)
, m_nClearStencil(0)
, m_bAutoDraw(false)
, m_bInTheScreenshot(false)
, m_uPixelsWide(0)
, m_uPixelsHigh(0)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    // Listen this event to save render Image before come to background.
    // Then it can be restored after coming to foreground on Android.
    CANotificationCenter::getInstance()->addObserver([this](CAObject* obj)
    {
        glDeleteFramebuffers(1, &m_uFBO);
        m_uFBO = 0;
        
    }, this, EVENT_COME_TO_BACKGROUND);
    
    CANotificationCenter::getInstance()->addObserver([this](CAObject* obj)
    {
        // -- regenerate frame buffer object and attach the texture
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_uOldFBO);
        
        glGenFramebuffers(1, &m_uFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
        
        //    m_pImageView->getImage()->setAliasTexParameters();
        if (m_pImageView->getImage())
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pImageView->getImage()->getName(), 0);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, m_uOldFBO);
    }, this, EVENT_COME_TO_FOREGROUND);
#endif

}

CARenderImage::~CARenderImage()
{
    CC_SAFE_RELEASE(m_pImageView);
    glDeleteFramebuffers(1, &m_uFBO);
    if (m_uDepthRenderBufffer)
    {
        glDeleteRenderbuffers(1, &m_uDepthRenderBufffer);
    }
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

    if(pRet && pRet->initWithWidthAndHeight(w, h, CAImage::PixelFormat::RGBA8888, 0))
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
    CCAssert(eFormat != CAImage::PixelFormat::A8, "only RGB and RGBA formats are valid for a render texture");

    bool bRet = false;
    unsigned char *data = NULL;
    do 
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_uOldFBO);

        // textures must be power of two squared
        unsigned long powW = 0;
        unsigned long powH = 0;
        
        float width = this->to_px(w);
        float height = this->to_px(h);
        
        if (CAConfiguration::getInstance()->supportsNPOT())
        {
            powW = width;
            powH = height;
        }
        else
        {
            powW = ccNextPOT(width);
            powH = ccNextPOT(height);
        }
        
        data = (unsigned char *)malloc((unsigned long)(powW * powH * 4));
        CC_BREAK_IF(! data);
        
        memset(data, 0, (unsigned long)(powW * powH * 4));
        
        m_ePixelFormat = eFormat;
        m_uPixelsWide = powW;
        m_uPixelsHigh = powH;
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
        glGenTextures(1, &m_uName);
        GL::bindTexture2D(m_uName);
        
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_uPixelsWide, (GLsizei)m_uPixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        GLint oldRBO;
        glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRBO);
        

        // generate FBO
        glGenFramebuffers(1, &m_uFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);

        // associate Image with FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uName, 0);

        if (uDepthStencilFormat != 0)
        {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if(CAConfiguration::getInstance()->supportsOESPackedDepthStencil())
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
            else
            {
                
                glGenRenderbuffers(1, &m_uDepthRenderBufffer);
                glGenRenderbuffers(1, &m_uDepthRenderBufffer);
                glBindRenderbuffer(GL_RENDERBUFFER, m_uDepthRenderBufffer);
                
                if(CAConfiguration::getInstance()->supportsOESDepth24())
                {
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, (GLsizei)powW, (GLsizei)powH);
                }
                else
                {
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, (GLsizei)powW, (GLsizei)powH);
                }
                
                glBindRenderbuffer(GL_RENDERBUFFER, m_uDepthRenderBufffer);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8,  (GLsizei)powW, (GLsizei)powH);
                
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uDepthRenderBufffer);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                          GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uDepthRenderBufffer);
            }
#else
            
            
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
#endif
        }
//        // check if it worked (probably worth doing :) )
        CCAssert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not attach Image to framebuffer");

        CAImageView* imageView = CAImageView::createWithLayout(DLayoutFill);
        imageView->setBlendFunc(BlendFunc_alpha_premultiplied);
        imageView->setOpacityModifyRGB(true);
        this->addSubview(imageView);
        this->setImageView(imageView);
        
        glBindRenderbuffer(GL_RENDERBUFFER, oldRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_uOldFBO);
        
        // Diabled by default.
        m_bAutoDraw = false;

        bRet = true;
    } while (0);
    
    CC_SAFE_FREE(data);
    
    return bRet;
}

CARenderImage* CARenderImage::printscreen()
{
    auto size = CAApplication::getApplication()->getWinSize();
    CARenderImage* render = CARenderImage::create(size.width, size.height , CAImage::PixelFormat::RGBA8888);
    render->printscreenWithView(CAApplication::getApplication()->getRootWindow());
    return render;
}

void CARenderImage::printscreenWithView(CAView* view)
{
    this->printscreenWithView(view, DPointZero);
}

void CARenderImage::printscreenWithView(CAView* view, DPoint offset)
{
    this->printscreenWithView(view, offset, CAColor4B(0, 0, 0, 0));
}

void CARenderImage::printscreenWithView(CAView* view, const CAColor4B& backgroundColor)
{
    this->printscreenWithView(view, DPointZero, backgroundColor);
}

void CARenderImage::printscreenWithView(CAView* view, DPoint offset, const CAColor4B& backgroundColor)
{
    CC_RETURN_IF(view == NULL);
    
    DPoint point = DPointZero;
    if (view->getSuperview())
    {
        point.y = view->getSuperview()->m_obContentSize.height;
    }
    else
    {
        point.y = CAApplication::getApplication()->getWinSize().height;
    }
    
    DPoint ori_p = view->m_obPoint;
    DPoint orig_anp = view->getAnchorPoint();
    float ori_ro_x = view->getRotationX();
    float ori_s = view->getScale();
    
    point.y -= view->m_obContentSize.height * ori_s;
    point.y += offset.y;
    point.x -= offset.x;
    
    
    view->setRotationX(ori_ro_x + 180);
    view->setAnchorPoint(DPoint(0.0f, 1.0f));
    view->setPoint(point);
    view->setScale(ori_s);
    
    auto renderer = m_pApplication->getRenderer();

    view->visitEve();

    renderer->clean();
    experimental::FrameBuffer::clearAllFBOs();
    
    renderer->clearDrawStats();
    
    this->beginWithClear(backgroundColor);
    m_bInTheScreenshot = true;
    view->visit();
    m_bInTheScreenshot = false;
    this->end();
    
    renderer->render();

    view->setRotationX(ori_ro_x);
    view->setAnchorPoint(orig_anp);
    view->setPoint(ori_p);
    view->setScale(ori_s);
}

void CARenderImage::begin()
{
    m_pApplication->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    m_tProjectionMatrix = m_pApplication->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    m_pApplication->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    m_tTransformMatrix = m_pApplication->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);


    Renderer *renderer =  m_pApplication->getRenderer();
    m_obGroupCommand.init(0);
    renderer->addCommand(&m_obGroupCommand);
    renderer->pushGroup(m_obGroupCommand.getRenderQueueID());

    m_obBeginCommand.init(0);
    m_obBeginCommand.func = std::bind(&CARenderImage::onBegin, this);
    
    m_pApplication->getRenderer()->addCommand(&m_obBeginCommand);

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
    setClearColor(CAColor4F(backgroundColor));
    
    setClearDepth(depthValue);
    
    setClearStencil(stencilValue);
    
    setClearFlags(flags);
    
    this->begin();
    
    //clear screen
    m_obBeginWithClearCommand.init(0);
    m_obBeginWithClearCommand.func = std::bind(&CARenderImage::onClear, this);
    CAApplication::getApplication()->getRenderer()->addCommand(&m_obBeginWithClearCommand);
}

void CARenderImage::end()
{
    m_obEndCommand.init(0);
    m_obEndCommand.func = std::bind(&CARenderImage::onEnd, this);
    
    CAApplication* application = CAApplication::getApplication();
    CCASSERT(nullptr != application, "CAApplication is null when setting matrix stack");
    
    Renderer *renderer = application->getRenderer();
    renderer->addCommand(&m_obEndCommand);
    renderer->popGroup();
    
    application->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    application->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

}

void CARenderImage::clear(const CAColor4B& backgroundColor)
{
    this->beginWithClear(backgroundColor);
    this->end();
}

void CARenderImage::clearDepth(float depthValue)
{
    setClearDepth(depthValue);
    
    this->begin();
    
    m_tClearDepthCommand.init(m_nZOrder);
    m_tClearDepthCommand.func = std::bind(&CARenderImage::onClearDepth, this);
    
    CAApplication::getApplication()->getRenderer()->addCommand(&m_tClearDepthCommand);
    
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

void CARenderImage::onBegin()
{

    m_tOldProjMatrix = m_pApplication->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    m_pApplication->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, m_tProjectionMatrix);
    
    m_tOldProjMatrix = m_pApplication->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    m_pApplication->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, m_tTransformMatrix);
    
    {
        m_pApplication->setProjection(m_pApplication->getProjection());
        const DSize texSize = DSize(m_uPixelsWide, m_uPixelsHigh);
        
        // Calculate the adjustment ratios based on the old and new projections
        DSize size = m_pApplication->getWinSize();
        float widthRatio = texSize.width / size.width;
        float heightRatio = texSize.height / size.height;
        
        Mat4 orthoMatrix;
        Mat4::createOrthographicOffCenter(-widthRatio, widthRatio, -heightRatio, heightRatio, -1, 1, &orthoMatrix);
        
        m_pApplication->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, orthoMatrix);
    }
    
    {
        glViewport(0, 0, this->to_px(m_uPixelsWide), this->to_px(m_uPixelsHigh));
    }
    
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_uOldFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);

    if (CAConfiguration::getInstance()->checkForGLExtension("GL_QCOM"))
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uNameCopy, 0);
        CHECK_GL_ERROR_DEBUG();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uName, 0);
    }
    
    CANotificationCenter::getInstance()->postNotification("RENDER_IMAGE_BEGIN");

}

void CARenderImage::onEnd()
{
    CAApplication *application = CAApplication::getApplication();
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_uOldFBO);
    
    // restore viewport
    application->setViewport();
    const auto& vp = CACamera::getDefaultViewport();
    glViewport(vp._left, vp._bottom, vp._width, vp._height);
    //
    application->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, m_tOldProjMatrix);
    application->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, m_tOldProjMatrix);
    
    
    
    GLubyte *buffer = nullptr;
    CAImage* image = new (std::nothrow) CAImage();
    
    do
    {
        CC_BREAK_IF(! (buffer = new (std::nothrow) GLubyte[m_uPixelsWide * m_uPixelsHigh * 4 + 1]));
        buffer[m_uPixelsWide * m_uPixelsHigh * 4] = '\0';
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_uOldFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
        
        if (CAConfiguration::getInstance()->checkForGLExtension("GL_QCOM"))
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uNameCopy, 0);
            CHECK_GL_ERROR_DEBUG();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uName, 0);
        }
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, (GLsizei)m_uPixelsWide, (GLsizei)m_uPixelsHigh, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_uOldFBO);
        
        CAData* data = CAData::create();
        data->fastSet(buffer, m_uPixelsWide * m_uPixelsHigh * 4);
        
        image->initWithRawData(data, m_ePixelFormat, (unsigned int)m_uPixelsWide, (unsigned int)m_uPixelsHigh);
        
    } while (0);
    
    m_pImageView->setImage(image);
    image->release();
    
    CANotificationCenter::getInstance()->postNotification("RENDER_IMAGE_END");

}

void CARenderImage::onClear()
{
    // save clear color
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

void CARenderImage::onClearDepth()
{
    //! save old depth value
    GLfloat depthClearValue;
    glGetFloatv(GL_DEPTH_CLEAR_VALUE, &depthClearValue);
    
    glClearDepth(m_uClearFlags);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // restore clear color
    glClearDepth(depthClearValue);
}

void CARenderImage::visitEve()
{
    CAView::visitEve();
    if (m_pImageView)
    {
        m_pImageView->visitEve();
    }
}

void CARenderImage::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    CC_RETURN_IF(m_bInTheScreenshot);
    CC_RETURN_IF(!m_bVisible);
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    CAApplication* application = CAApplication::getApplication();

    application->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    application->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, m_tModelViewTransform);
    
    m_pImageView->visit(renderer, m_tModelViewTransform, flags);
    if (isVisitableByVisitingCamera())
    {
        draw(renderer, m_tModelViewTransform, flags);
    }
    
    application->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void CARenderImage::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if( m_bAutoDraw)
    {
        //Begin will create a render group using new render target
        begin();
        
        //clear screen
        m_obClearCommand.init(0);
        m_obClearCommand.func = std::bind(&CARenderImage::onClear, this);
        renderer->addCommand(&m_obClearCommand);
        
        //! make sure all children are drawn
        sortAllSubviews();
        
        for (auto& subview : m_obSubviews)
        {
            CC_CONTINUE_IF(subview == m_pImageView);
            subview->visit(renderer, transform, flags);
        }

        //End will pop the current render group
        end();
	}
}

bool CARenderImage::saveToFile(const char *szFilePath)
{
    bool bRet = false;
    if (m_pImageView->getImage())
    {
        bRet = m_pImageView->getImage()->saveToFile(szFilePath);
    }
    return bRet;
}


void CARenderImage::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(DSize(m_uPixelsWide, m_uPixelsHigh));
}

float CARenderImage::to_px(float dip)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return s_dip_to_px(dip);
    #else
        return dip;
    #endif
}

NS_CC_END

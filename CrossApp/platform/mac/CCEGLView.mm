
#include "CCEGLView.h"
#include "EAGLView.h"
#include "CAApplicationCaller.h"
#include "CCSet.h"
#include "CATouch.h"
#include "CATouchDispatcher.h"
#include "basics/CAApplication.h"
#include "basics/CANotificationCenter.h"
#include "basics/CACamera.h"
#include "CrossApp-Prefix.h"
#import "EAGLView.h"

NS_CC_BEGIN

CCEGLView* CCEGLView::s_sharedView = NULL;

CCEGLView* CCEGLView::sharedOpenGLView(void)
{
    if (!s_sharedView)
    {
        s_sharedView = new CCEGLView();
    }
    return s_sharedView;
}

CCEGLView::CCEGLView(void)
:m_bIsRenderImage(false)
{
    bool * p_IsRenderImage = &m_bIsRenderImage;
    CANotificationCenter::getInstance()->addObserver([=](CAObject*){
        *p_IsRenderImage = true;
    }, CAApplication::getApplication(), "RENDER_IMAGE_BEGIN");
    
    CANotificationCenter::getInstance()->addObserver([=](CAObject*){
        *p_IsRenderImage = false;
    }, CAApplication::getApplication(), "RENDER_IMAGE_END");
}

CCEGLView::~CCEGLView(void)
{
    s_sharedView = NULL;
}

void CCEGLView::setFrameSize(float width, float height)
{
    CCEGLViewProtocol::setFrameSize(width, height);
    
    if (width / height > 1.25f)
    {
        this->setStatusBarOrientation(CAInterfaceOrientation::Landscape);
    }
    else
    {
        this->setStatusBarOrientation(CAInterfaceOrientation::Portrait);
    }
}

bool CCEGLView::isOpenGLReady(void)
{
    return [EAGLView sharedEGLView] != NULL;
}

bool CCEGLView::setContentScaleFactor(float contentScaleFactor)
{
    return false;
}

void CCEGLView::end(void)
{
    [[CAApplicationCaller sharedCAApplicationCaller] end];
    
    // destroy EAGLView
    [[EAGLView sharedEGLView] removeFromSuperview];
    
    delete this;
}

void CCEGLView::swapBuffers()
{
	[[EAGLView sharedEGLView] swapBuffers];
}

void CCEGLView::setIMEKeyboardState(bool bOpen)
{
    if (bOpen)
    {
        [[EAGLView sharedEGLView] becomeFirstResponder];
    }
    else
    {
        [[EAGLView sharedEGLView] resignFirstResponder];
    }
}

void CCEGLView::setViewPortInPoints(float x , float y , float w , float h)
{
    float frameZoomFactor = this->getFrameZoomFactor();
    
    experimental::Viewport vp((float)(x * m_fScale * frameZoomFactor + m_obViewPortRect.origin.x * frameZoomFactor),
                              (float)(y * m_fScale * frameZoomFactor + m_obViewPortRect.origin.y * frameZoomFactor),
                              (float)(w * m_fScale * frameZoomFactor),
                              (float)(h * m_fScale * frameZoomFactor));
    
    CACamera::setDefaultViewport(vp);
}

void CCEGLView::setScissorInPoints(float x , float y , float w , float h)
{
    float frameZoomFactor = this->getFrameZoomFactor();
    
    if (m_bIsRenderImage)
    {
        frameZoomFactor = 2;
    }

    glScissor((GLint)(x * m_fScale * frameZoomFactor + m_obViewPortRect.origin.x * frameZoomFactor),
              (GLint)(y * m_fScale * frameZoomFactor + m_obViewPortRect.origin.y * frameZoomFactor),
              (GLsizei)ceilf(w * m_fScale * frameZoomFactor),
              (GLsizei)ceilf(h * m_fScale * frameZoomFactor));
}

void CCEGLView::setMultiTouchMask(bool mask)
{
	//EAGLView *glView = [EAGLView sharedEGLView];
	//glView.multipleTouchEnabled = mask ? YES : NO;
}

DRect CCEGLView::getScissorRect()
{
    GLfloat params[4];
    
    float frameZoomFactor = this->getFrameZoomFactor();
    
    glGetFloatv(GL_SCISSOR_BOX, params);
    params[0] = (params[0] / frameZoomFactor / m_fScale - m_obViewPortRect.origin.x);
    params[1] = (params[1] / frameZoomFactor / m_fScale - m_obViewPortRect.origin.y);
    params[2] = params[2] / frameZoomFactor / m_fScale;
    params[3] = params[3] / frameZoomFactor / m_fScale;
    
    return DRect(params[0], params[1], params[2], params[3]);
}

float CCEGLView::getFrameZoomFactor() const
{
    NSScreen *screen = [[[EAGLView sharedEGLView] window] screen];;

    static float s_backingScaleFactor = 0;
    
    if (s_backingScaleFactor == 0)
    {
        s_backingScaleFactor = screen.backingScaleFactor;
        NSLog(@"screen factor %f", screen.backingScaleFactor);
    }
    
    if (s_backingScaleFactor != screen.backingScaleFactor)
    {
        s_backingScaleFactor = screen.backingScaleFactor;
        [[EAGLView sharedEGLView] reshape];
        NSLog(@"screen factor %f", screen.backingScaleFactor);
    }
    
    return s_backingScaleFactor;
}

NS_CC_END // end of namespace CossApp;

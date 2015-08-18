
#include "CCEGLView.h"
#include "EAGLView.h"
#include "CAApplicationCaller.h"
#include "CCSet.h"
#include "CATouch.h"
#include "CATouchDispatcher.h"

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
: m_bKeyboardOpen(false)
{
}

CCEGLView::~CCEGLView(void)
{
    s_sharedView = NULL;
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
	m_bKeyboardOpen = bOpen;
}

bool CCEGLView::getIMEKeyboardState()
{
	return m_bKeyboardOpen;
}

void CCEGLView::setViewPortInPoints(float x , float y , float w , float h)
{
    float frameZoomFactor = [[EAGLView sharedEGLView] frameZoomFactor];
    
    glViewport((GLint)(x * m_fScaleX * frameZoomFactor + m_obViewPortRect.origin.x * frameZoomFactor),
               (GLint)(y * m_fScaleY * frameZoomFactor + m_obViewPortRect.origin.y * frameZoomFactor),
               (GLsizei)(w * m_fScaleX * frameZoomFactor),
               (GLsizei)(h * m_fScaleY * frameZoomFactor));
}

void CCEGLView::setScissorInPoints(float x , float y , float w , float h)
{
    float frameZoomFactor = [[EAGLView sharedEGLView] frameZoomFactor];
    
    glScissor((GLint)(x * m_fScaleX * frameZoomFactor + m_obViewPortRect.origin.x * frameZoomFactor),
              (GLint)(y * m_fScaleY * frameZoomFactor + m_obViewPortRect.origin.y * frameZoomFactor),
              (GLsizei)(w * m_fScaleX * frameZoomFactor),
              (GLsizei)(h * m_fScaleY * frameZoomFactor));
}

void CCEGLView::setMultiTouchMask(bool mask)
{
	//EAGLView *glView = [EAGLView sharedEGLView];
	//glView.multipleTouchEnabled = mask ? YES : NO;
}

NS_CC_END // end of namespace cocos2d;

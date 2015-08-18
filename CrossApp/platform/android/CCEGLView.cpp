
#include "CCEGLView.h"
#include "cocoa/CCSet.h"
#include "basics/CAApplication.h"
#include "ccMacros.h"
#include "dispatcher/CATouchDispatcher.h"
#include "jni/IMEJni.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "CCGL.h"

#include <stdlib.h>
#include <android/log.h>



#if CC_TEXTURE_ATLAS_USE_VAO

// <EGL/egl.h> exists since android 2.3
#include <EGL/egl.h>
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT = 0;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT = 0;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT = 0;

#endif

void initExtensions() {
#if CC_TEXTURE_ATLAS_USE_VAO
     glGenVertexArraysOESEXT = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
     glBindVertexArrayOESEXT = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
     glDeleteVertexArraysOESEXT = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");
#endif
}

NS_CC_BEGIN

CCEGLView::CCEGLView()
: m_bKeyboardOpen(false)
{
    initExtensions();
}

CCEGLView::~CCEGLView()
{

}

bool CCEGLView::isOpenGLReady()
{
    return (m_obScreenSize.width != 0 && m_obScreenSize.height != 0);
}

void CCEGLView::end()
{
    terminateProcessJNI();
}

void CCEGLView::swapBuffers()
{
}

CCEGLView* CCEGLView::sharedOpenGLView()
{
    static CCEGLView instance;
    return &instance;
}

void CCEGLView::setIMEKeyboardState(bool bOpen)
{
    CCLog("setIMEKeyboardState %d",bOpen);
    setKeyboardStateJNI((int)bOpen);
	m_bKeyboardOpen = bOpen;
}

bool CCEGLView::getIMEKeyboardState()
{
	return m_bKeyboardOpen;
}

void CCEGLView::setIMEKeyboardNumber()
{
    setKeyboardType(11);
}

void CCEGLView::setIMEKeyboardDefault()
{
    setKeyboardType(10);
}

void CCEGLView::setIMEKeyboardAlphabet()
{
    setKeyboardType(12);
}
void CCEGLView::setIMEKeyboardReturnSend()
{
    setKeyboardReturnType(23);
}

void CCEGLView::setIMEKeyboardReturnSearch()
{
    setKeyboardReturnType(22);
}

void CCEGLView::setIMEKeyboardReturnDone()
{
    setKeyboardReturnType(21);
}

void CCEGLView::setIMEKeyboardReturnEnter()
{
    setKeyboardReturnType(25);
}

void CCEGLView::setIMECursorPos(int pos,const char* text)
{
    setCursorPos(pos,text);
}
NS_CC_END


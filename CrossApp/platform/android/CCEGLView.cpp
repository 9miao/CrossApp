
#include "CCEGLView.h"
#include "cocoa/CCSet.h"
#include "basics/CAApplication.h"
#include "ccMacros.h"
#include "dispatcher/CATouchDispatcher.h"
#include "jni/Java_org_CrossApp_lib_CrossAppHelper.h"
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

NS_CC_END


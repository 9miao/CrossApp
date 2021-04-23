

#ifndef __CC_EGLVIEW_IPHONE_H__
#define __CC_EGLVIEW_IPHONE_H__

#include "platform/CACommon.h"
#include "platform/CCEGLViewProtocol.h"

NS_CC_BEGIN



class CC_DLL CCEGLView : public CCEGLViewProtocol
{
public:
    CCEGLView();
   ~CCEGLView();

    virtual bool    isOpenGLReady();

    // keep compatible
    virtual void    end();
    virtual void    swapBuffers();

    static CCEGLView* sharedOpenGLView();

};

NS_CC_END

#endif    // end of __CC_EGLVIEW_IPHONE_H__



#ifndef __CC_EGLVIEW_ANDROID_H__
#define __CC_EGLVIEW_ANDROID_H__

#include "platform/CCEGLViewProtocol.h"

NS_CC_BEGIN

class CC_DLL CCEGLView : public CCEGLViewProtocol
{
public:
    CCEGLView();
    virtual ~CCEGLView();

    bool    isOpenGLReady();

    // keep compatible
    void    end();
    void    swapBuffers();
    // static function
    /**
    @brief    get the shared main open gl window
    */
    static CCEGLView* sharedOpenGLView();
};

NS_CC_END

#endif    // end of __CC_EGLVIEW_ANDROID_H__



#ifndef __CC_EGLVIEW_IPHONE_H__
#define __CC_EGLVIEW_IPHONE_H__

#include "platform/CCCommon.h"
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
    virtual void    checkContext();
    
    virtual void setIMEKeyboardState(bool bOpen);
    
    virtual void setIMEKeyboardNumber();
    
    virtual void setIMEKeyboardDefault();
    
    virtual void setIMEKeyboardAlphabet();
    
    virtual void setIMEKeyboardReturnSend();
    
    virtual void setIMEKeyboardReturnSearch();
    
    virtual void setIMEKeyboardReturnDone();
    
    virtual void setIMEKeyboardReturnEnter();
    
    static CCEGLView* sharedOpenGLView();

};

NS_CC_END

#endif    // end of __CC_EGLVIEW_IPHONE_H__

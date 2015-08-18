

#ifndef __CC_EGLVIEW_ANDROID_H__
#define __CC_EGLVIEW_ANDROID_H__

#include "basics/CAGeometry.h"
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
    void    setIMEKeyboardState(bool bOpen);
	bool	getIMEKeyboardState();
    
    virtual void setIMEKeyboardNumber();
    
    virtual void setIMEKeyboardDefault();
    
    virtual void setIMEKeyboardAlphabet();
    
    virtual void setIMEKeyboardReturnSend();
    
    virtual void setIMEKeyboardReturnSearch();
    
    virtual void setIMEKeyboardReturnDone();

    virtual void setIMEKeyboardReturnEnter();
    
    void setIMECursorPos(int pos,const char* text);
    // static function
    /**
    @brief    get the shared main open gl window
    */
    static CCEGLView* sharedOpenGLView();

private:
	bool m_bKeyboardOpen;
};

NS_CC_END

#endif    // end of __CC_EGLVIEW_ANDROID_H__

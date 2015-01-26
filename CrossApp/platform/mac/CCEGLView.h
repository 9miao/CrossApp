

#ifndef __CC_EGLVIEW_MAC_H__
#define __CC_EGLVIEW_MAC_H__

#include "platform/CCCommon.h"
#include "platform/CCEGLViewProtocol.h"

NS_CC_BEGIN

class CCSet;
class CCTouch;
class CCSize;

class CC_DLL CCEGLView : public CCEGLViewProtocol
{
public:
    static CCEGLView* sharedOpenGLView(void);
    
    virtual ~CCEGLView(void);
    
    virtual bool isOpenGLReady(void);
    virtual bool setContentScaleFactor(float contentScaleFactor);
    virtual void end();
    virtual void swapBuffers(void);
    /**
     * Set opengl view port rectangle with points.
     */
    virtual void setViewPortInPoints(float x , float y , float w , float h);
    virtual void setScissorInPoints(float x , float y , float w , float h);
    
    virtual void setIMEKeyboardState(bool bOpen);
	virtual void setMultiTouchMask(bool mask);
    
private:
    static CCEGLView* s_sharedView;
    
    CCEGLView(void);
};

NS_CC_END   // end of namespace   cocos2d

#endif	// end of __CC_EGLVIEW_MAC_H__

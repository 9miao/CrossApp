

#ifndef __CC_EGLVIEW_MAC_H__
#define __CC_EGLVIEW_MAC_H__

#include "platform/CACommon.h"
#include "platform/CCEGLViewProtocol.h"

NS_CC_BEGIN

class CCSet;
class CCTouch;
class DSize;

class CC_DLL CCEGLView : public CCEGLViewProtocol
{
public:
    static CCEGLView* sharedOpenGLView(void);
    
    virtual ~CCEGLView(void);
    
    virtual void setFrameSize(float width, float height);
    
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
    
    /**
     * Get the current scissor rectangle
     * @lua NA
     */
    virtual DRect getScissorRect();
    
    virtual float getFrameZoomFactor() const;
    
private:
    static CCEGLView* s_sharedView;
    
    CCEGLView(void);
    
    bool m_bIsRenderImage;
};

NS_CC_END 

#endif	// end of __CC_EGLVIEW_MAC_H__

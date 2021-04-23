

#ifndef __CC_EGLVIEW_WIN32_H__
#define __CC_EGLVIEW_WIN32_H__

#include "CCStdC.h"
#include "platform/CACommon.h"
#include "basics/CAPoint.h"
#include "basics/CASize.h"
#include "basics/CARect.h"
#include "platform/CCEGLViewProtocol.h"

NS_CC_BEGIN

typedef LRESULT (*CUSTOM_WND_PROC)(UINT message, WPARAM wParam, LPARAM lParam, BOOL* pProcessed);

class CCEGL;

class CC_DLL CCEGLView : public CCEGLViewProtocol
{
public:
    CCEGLView();
    virtual ~CCEGLView();

    /* override functions */
    virtual bool isOpenGLReady();
    virtual void end();
    virtual void swapBuffers();
    virtual void setFrameSize(float width, float height);
	virtual void setEditorFrameSize(float width, float height,HWND hWnd); 
    virtual void setIMEKeyboardState(bool bOpen);

    void setMenuResource(LPCWSTR menu);
    void setWndProc(CUSTOM_WND_PROC proc);

protected:
    virtual bool Create();
public:
    bool initGL();
    void destroyGL();

    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void setHWnd(HWND hWnd);
    // win32 platform function
    HWND getHWnd();
    virtual void resize(int width, int height);
	
    /* 
     * Set zoom factor for frame. This method is for debugging big resolution (e.g.new ipad) app on desktop.
     */
    void setFrameZoomFactor(float fZoomFactor);
	float getFrameZoomFactor();
    virtual void centerWindow();

    typedef void (*LPFN_ACCELEROMETER_KEYHOOK)( UINT message,WPARAM wParam, LPARAM lParam );
    void setAccelerometerKeyHook( LPFN_ACCELEROMETER_KEYHOOK lpfnAccelerometerKeyHook );

    virtual void setViewPortInPoints(float x , float y , float w , float h);
    virtual void setScissorInPoints(float x , float y , float w , float h);
    
    // static function
    /**
    @brief    get the shared main open gl window
    */
    static CCEGLView* sharedOpenGLView();

protected:
	static CCEGLView* s_pEglView;
    bool m_bCaptured;
    HWND m_hWnd;
    HDC  m_hDC;
    HGLRC m_hRC;
    LPFN_ACCELEROMETER_KEYHOOK m_lpfnAccelerometerKeyHook;
    bool m_bSupportTouch;

    LPCWSTR m_menu;
    CUSTOM_WND_PROC m_wndproc;

    float m_fFrameZoomFactor;
};

NS_CC_END

#endif    // end of __CC_EGLVIEW_WIN32_H__

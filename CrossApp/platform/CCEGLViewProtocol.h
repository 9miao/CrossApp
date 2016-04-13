#ifndef __CCEGLViewProtocol_H__
#define __CCEGLViewProtocol_H__

#include "ccTypes.h"

NS_CC_BEGIN

#define CC_MAX_TOUCHES  5

class CCEGLTouchDelegate;
class CCSet;

/**
 * @addtogroup platform
 * @{
 * @js NA
 */

class CC_DLL CCEGLViewProtocol
{
public:
    /**
     * @lua NA
     */
    CCEGLViewProtocol();
    /**
     * @lua NA
     */
    virtual ~CCEGLViewProtocol();

    /** Force destroying EGL view, subclass must implement this method. 
     * @lua NA
     */
    virtual void    end() = 0;

    /** Get whether opengl render system is ready, subclass must implement this method. 
     * @lua NA
     */
    virtual bool    isOpenGLReady() = 0;

    /** Exchanges the front and back buffers, subclass must implement this method. 
     * @lua NA
     */
    virtual void    swapBuffers() = 0;

    /**
     * Get the frame size of EGL view.
     * In general, it returns the screen size since the EGL view is a fullscreen view.
     */
    virtual const DSize& getFrameSize() const;

    /**
     * Set the frame size of EGL view.
     */
    virtual void setFrameSize(float width, float height);

    /**
     * Get the visible area size of opengl viewport.
     */
    virtual DSize getVisibleSize() const;

    /**
     * Get the visible origin point of opengl viewport.
     */
    virtual DPoint getVisibleOrigin() const;

    /**
     * Set the design resolution size.
     * @param width Design resolution width.
     * @param height Design resolution height.
     * @param resolutionPolicy The resolution policy desired, you may choose:
     *                         [1] kResolutionExactFit Fill screen by stretch-to-fit: if the design resolution ratio of width to height is different from the screen resolution ratio, your game view will be stretched.
     *                         [2] kResolutionNoBorder Full screen without black border: if the design resolution ratio of width to height is different from the screen resolution ratio, two areas of your game view will be cut.
     *                         [3] kResolutionShowAll  Full screen with black border: if the design resolution ratio of width to height is different from the screen resolution ratio, two black borders will be shown.
     */

    /** Get design resolution size.
     *  Default resolution size is the same as 'getFrameSize'.
     */
    virtual const DSize&  getDesignResolutionSize() const;

    /** Set touch delegate */
    virtual void setTouchDelegate(CCEGLTouchDelegate * pDelegate);

    /**
     * Set opengl view port rectangle with points.
     */
    virtual void setViewPortInPoints(float x , float y , float w , float h);

    /**
     * Set Scissor rectangle with points.
     */
    virtual void setScissorInPoints(float x , float y , float w , float h);

    /**
     * Get whether GL_SCISSOR_TEST is enable
     * @lua NA
     */
    virtual bool isScissorEnabled();

    /**
     * Get the current scissor rectangle
     * @lua NA
     */
    virtual DRect getScissorRect();
    /**
     * @lua NA
     */
    virtual void setViewName(const char* pszViewName);
    /**
     * @lua NA
     */
    const char* getViewName();

    /** Touch events are handled by default; if you want to customize your handlers, please override these functions: 
     * @lua NA
     */
    virtual void handleTouchesBegin(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);
    /**
     * @lua NA
     */
    virtual void handleTouchesMove(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);
    /**
     * @lua NA
     */
    virtual void handleTouchesEnd(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);
    /**
     * @lua NA
     */
    virtual void handleTouchesCancel(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);
    /**
     * @lua NA
     */
    virtual void handleMouseMoved(float x, float y, CAEvent* event);
    /**
     * @lua NA
     */
    virtual void handleScrollWheel(float x, float y, float offx, float offy, CAEvent* event);
    
    virtual void handleOtherMouseDown(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);
    virtual void handleOtherMouseDragged(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);
    virtual void handleOtherMouseUp(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);
    virtual void handleMouseEntered(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);
    virtual void handleMouseExited(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);
    /**
     * Get the opengl view port rectangle.
     */
    const DRect& getViewPortRect() const;

    /**
     * Get scale factor of the direction.
     */
    float getScale() const;

    virtual void setStatusBarOrientation(const CAInterfaceOrientation& var);
    
    virtual const CAInterfaceOrientation& getStatusBarOrientation();
    
private:
    
    void getSetOfTouchesEndOrCancel(CCSet& set, int num, intptr_t ids[], float xs[], float ys[], CAEvent* event);

protected:
    
    CCEGLTouchDelegate* m_pDelegate;

    CAInterfaceOrientation m_eOrientation;
    
    // real screen size
    DSize m_obScreenSize;
    // resolution size, it is the size appropriate for the app resources.
    DSize m_obDesignResolutionSize;
    // the view port size
    DRect m_obViewPortRect;
    // the view name
    char   m_szViewName[50];

    float  m_fScale;
};

// end of platform group
/// @}

NS_CC_END

#endif /* __CCEGLViewProtocol_H__ */

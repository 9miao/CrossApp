

#ifndef __CCDIRECTOR_H__
#define __CCDIRECTOR_H__

#include "platform/CCPlatformMacros.h"
#include "CAObject.h"
#include "ccTypes.h"
#include "CAGeometry.h"
#include "CCGL.h"
#include "kazmath/mat4.h"
#include "ccTypeInfo.h"
#include "view/CALabel.h"

#define CROSSAPP_ADPTATION_RATIO  CAApplication::getApplication()->getAdaptationRatio()

NS_CC_BEGIN

/**
 * @addtogroup base_nodes
 * @{
 */



/* Forward declarations. */
class CAWindow;
class CCEGLView;
class CAApplicationDelegate;
class CAScheduler;
class CCActionManager;
class CATouchDispatcher;
class CAKeypadDispatcher;
class CAAccelerometer;


class CC_DLL CAApplication : public CAObject, public TypeInfo
{
public:
    
    /** @typedef Projection
     Possible OpenGL projections used by director
     */
    typedef enum
    {
        /// sets a 2D projection (orthogonal projection)
        P2D,
        
        /// sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500.
        P3D,

        Default = P2D,
    } Projection;
    
    /**
     *  @js ctor
     */
    CAApplication(void);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CAApplication(void);
    virtual bool init(void);
    /**
     * @js NA
     * @lua NA
     */
    virtual long getClassTypeInfo() {
		static const long id = CrossApp::getHashCodeByString(typeid(CrossApp::CAApplication).name());
		return id;
    }

    // attribute

    /** Get current running Scene. Director can only run one Scene at the time */
    inline CAWindow* getRootWindow(void) { return m_pRootWindow; }

    /** Get the FPS value */
    inline double getAnimationInterval(void) { return m_dAnimationInterval; }
    /** Set the FPS value. */
    virtual void setAnimationInterval(double dValue) = 0;

    /** Whether or not to display the FPS on the bottom-left corner */
    inline bool isDisplayStats(void) { return m_bDisplayStats; }
    /** Display the FPS on the bottom-left corner */
    inline void setDisplayStats(bool bDisplayStats) { m_bDisplayStats = bDisplayStats; }
    
    /** seconds per frame */
    inline float getSecondsPerFrame() { return m_fSecondsPerFrame; }

    /** Get the CCEGLView, where everything is rendered
     * @js NA
     */
    inline CCEGLView* getOpenGLView(void) { return m_pobOpenGLView; }
    void setOpenGLView(CCEGLView *pobOpenGLView);

    inline bool isNextDeltaTimeZero(void) { return m_bNextDeltaTimeZero; }
    void setNextDeltaTimeZero(bool bNextDeltaTimeZero);

    /** Whether or not the Director is paused */
    inline bool isPaused(void) { return m_bPaused; }

    /** How many frames were called since the director started */
    inline unsigned int getTotalFrames(void) { return m_uTotalFrames; }
    
     /** reshape projection matrix when canvas has been change"*/
    void reshapeProjection(const DSize& newWindowSize);
    
    inline const CAStatusBarStyle& getStatusBarStyle() { return m_eStatusBarStyle; }
    void setStatusBarStyle(const CAStatusBarStyle& var);
    
    bool isStatusBarHidden();
    
    const CAInterfaceOrientation& getStatusBarOrientation();
    
    /** Sets the glViewport*/
    void setViewport();

    /** This object will be visited after the main scene is visited.
     This object MUST implement the "visit" selector.
     Useful to hook a notification object, like CCNotifications (http://github.com/manucorporat/CCNotifications)
     @since v0.99.5
     */
    CAView* getNotificationView();
    void setNotificationView(CAView *view);

    // window size

    /** returns the size of the OpenGL view in points.
    */
    DSize getWinSize(void);

    /** returns visible size of the OpenGL view in points.
     *  the value is equal to getWinSize if don't invoke
     *  CCEGLView::setDesignResolutionSize()
     */
    DSize getVisibleSize();
    
    /** returns visible origin of the OpenGL view in points.
     */
    DPoint getVisibleOrigin();

    /** converts a UIKit coordinate to an OpenGL coordinate
     Useful to convert (multi) touch coordinates to the current layout (portrait or landscape)
     */
    DPoint convertToGL(const DPoint& obPoint);

    /** converts an OpenGL coordinate to a UIKit coordinate
     Useful to convert node points to window points for calls such as glScissor
     */
    DPoint convertToUI(const DPoint& obPoint);

    /// XXX: missing description 
    float getZEye(void);

    // Scene Management

    /** Enters the Director's main loop with the given Scene.
     * Call it to run only your FIRST scene.
     * Don't call it if there is already a running scene.
     *
     * It will call pushScene: and then it will call startAnimation
     */
    void runWindow(CAWindow *pWindow);

    /** Ends the execution, releases the running scene.
     It doesn't remove the OpenGL view from its parent. You have to do it manually.
     */
    void end(void);

    /** Pauses the running scene.
     The running scene will be _drawed_ but all scheduled timers will be paused
     While paused, the draw rate will be 4 FPS to reduce CPU consumption
     */
    void pause(void);

    /** Resumes the paused scene
     The scheduled timers will be activated again.
     The "delta time" will be 0 (as if the game wasn't paused)
     */
    void resume(void);

    /** Stops the animation. Nothing will be drawn. The main loop won't be triggered anymore.
     If you don't want to pause your animation call [pause] instead.
     */
    virtual void stopAnimation(void) = 0;

    /** The main loop is triggered again.
     Call this function only if [stopAnimation] was called earlier
     @warning Don't call this function to start the main loop. To run the main loop call runWithScene
     */
    virtual void startAnimation(void) = 0;

    /** Draw the scene.
    This method is called every frame. Don't call it manually.
    */
    //void drawView(CAView* var);

    void updateDraw();
    
    void drawScene(float dt = 0);
    
    void run(float dt);
    
    // Memory Helper

    /** Removes cached all CrossApp cached data.
     It will purge the CAImageCache, CCSpriteFrameCache, CCLabelBMFont cache
     @since v0.99.3
     */
    void purgeCachedData(void);

	/** sets the default values based on the CCConfiguration info */
    void setDefaultValues(void);

    // OpenGL Helper

    /** sets the OpenGL default values */
    void setGLDefaultValues(void);

    /** enables/disables OpenGL alpha blending */
    void setAlphaBlending(bool bOn);

    /** enables/disables OpenGL depth test */
    void setDepthTest(bool bOn);

    virtual void mainLoop(void) = 0;

    bool isDrawing() {return (m_nDrawCount > 0);}
    
    unsigned long getCurrentNumberOfDraws();
    
    /** Sets an OpenGL projection
     @since v0.8.2
     @js NA
     */
    inline CAApplication::Projection getProjection(void) { return m_eProjection; }
    void setProjection(CAApplication::Projection kProjection);
    
public:

    /** CATouchDispatcher associated with this director
     @since v2.0
     */
    CC_PROPERTY(CATouchDispatcher*, m_pTouchDispatcher, TouchDispatcher);

    /** CAKeypadDispatcher associated with this director
     @since v2.0
     */
    CC_PROPERTY(CAKeypadDispatcher*, m_pKeypadDispatcher, KeypadDispatcher);

    /** CAAccelerometer associated with this director
     @since v2.0
     @js NA
     @lua NA
     */
    CC_PROPERTY(CAAccelerometer*, m_pAccelerometer, Accelerometer);

    /* delta time since last tick to main loop */
	CC_PROPERTY_READONLY(float, m_fDeltaTime, DeltaTime);
    
    CC_SYNTHESIZE_READONLY(float, m_fAdaptationRatio, AdaptationRatio);
    
    CC_SYNTHESIZE_READONLY(unsigned long, m_uNumberOfDraws, NumberOfDraws);

public:
    /** returns a shared instance of the director 
     *  @js getInstance
     */
    static CAApplication* getApplication(void);
	
protected:
    
    void purgeDirector();
    bool m_bPurgeDirecotorInNextLoop; // this flag will be set to true in end()
    
    void showStats();
    void createStatsLabel();
    void calculateMPF();
    void getFPSImageData(unsigned char** datapointer, unsigned int* length);
    
    /** calculates delta time since last time it was called */    
    void calculateDeltaTime();
protected:
    /* The CCEGLView, where everything is rendered */
    CCEGLView    *m_pobOpenGLView;

    double m_dAnimationInterval;
    double m_dOldAnimationInterval;

    /* landscape mode ? */
    bool m_bLandscape;
    
    bool m_bDisplayStats;
    float m_fAccumDt;
    float m_fFrameRate;
    
    CALabel *m_pFPSLabel;

    /** Whether or not the Director is paused */
    bool m_bPaused;

    /* How many frames were called since the director started */
    unsigned int m_uTotalFrames;
    unsigned int m_uFrames;
    float m_fSecondsPerFrame;
     
    /* The running scene */
    CAWindow *m_pRootWindow;
    
    /* last time the main loop was updated */
    struct cc_timeval *m_pLastUpdate;

    /* whether or not the next delta time will be zero */
    bool m_bNextDeltaTimeZero;
    
    /* projection used */
    Projection m_eProjection;

    /* window size in points */
    DSize    m_obWinSizeInPoints;

    /* store the fps string */
    char *m_pszFPS;

    /* This object will be visited after the scene. Useful to hook a notification node */
    CAView *m_pNotificationNode;
    
    CAStatusBarStyle m_eStatusBarStyle;
    
    int m_nDrawCount;
    
    // CCEGLViewProtocol will recreate stats labels to fit visible rect
    friend class CCEGLViewProtocol;
};


class CCDisplayLinkDirector : public CAApplication
{
public:
    CCDisplayLinkDirector(void) 
        : m_bInvalid(false)
    {}

    virtual void mainLoop(void);
    virtual void setAnimationInterval(double dValue);
    virtual void startAnimation(void);
    virtual void stopAnimation();

protected:
    bool m_bInvalid;
};

// end of base_node group
/// @}

CC_DEPRECATED_ATTRIBUTE static inline float _px(float dip) { return dip; }
CC_DEPRECATED_ATTRIBUTE static inline float _dip(float px) { return px; }

NS_CC_END

#endif // __CCDIRECTOR_H__

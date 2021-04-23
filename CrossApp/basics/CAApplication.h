

#ifndef __CCDIRECTOR_H__
#define __CCDIRECTOR_H__

#include "platform/CCPlatformMacros.h"
#include "CAObject.h"
#include "ccTypes.h"
#include "basics/CAPoint.h"
#include "basics/CASize.h"
#include "basics/CARect.h"
#include "basics/CAScheduler.h"
#include "ccTypeInfo.h"
#include "view/CALabel.h"
#include "CCGL.h"
#include <thread>
#include <chrono>

NS_CC_BEGIN

class CAWindow;
class CCEGLView;
class CAApplicationDelegate;
class CAImageCache;
class CAScheduler;
class ActionManager;
class CATouchDispatcher;
class CAKeypadDispatcher;
class CAThemeManager;
class Renderer;
class CANotificationCenter;
class CAMotionManager;
class CAConsole;
namespace experimental
{
    class FrameBuffer;
}

enum class MATRIX_STACK_TYPE
{
    /// Model view matrix stack
    MATRIX_STACK_MODELVIEW,
    
    /// projection matrix stack
    MATRIX_STACK_PROJECTION,
    
    /// texture matrix stack
    MATRIX_STACK_TEXTURE
};


class CC_DLL CAApplication : public CAObject, public TypeInfo
{
public:
    
    static const char* EVENT_PROJECTION_CHANGED;
    static const char* EVENT_BEFORE_UPDATE;
    static const char* EVENT_AFTER_UPDATE;
    static const char* EVENT_RESET;
    static const char* EVENT_AFTER_VISIT;
    static const char* EVENT_AFTER_DRAW;
    
    enum class Projection
    {
        P2D,
        P3D,
        Default = P3D,
    };

    CAApplication(void);

    virtual ~CAApplication(void);
    
    virtual bool init(void);

    virtual long getClassTypeInfo()
    {
		static const long id = CrossApp::getHashCodeByString(typeid(CrossApp::CAApplication).name());
		return id;
    }

    inline CAWindow* getRootWindow(void) { return m_pRootWindow; }

    inline double getAnimationInterval(void) { return m_dAnimationInterval; }

    virtual void setAnimationInterval(double dValue) = 0;

    inline bool isDisplayStats(void) { return m_bDisplayStats; }

    inline void setDisplayStats(bool bDisplayStats) { m_bDisplayStats = bDisplayStats; }
    
    inline float getSecondsPerFrame() { return m_fSecondsPerFrame; }

    inline CCEGLView* getOpenGLView(void) { return m_pobOpenGLView; }
    
    void setOpenGLView(CCEGLView *pobOpenGLView);

    inline bool isNextDeltaTimeZero(void) { return m_bNextDeltaTimeZero; }
    
    void setNextDeltaTimeZero(bool bNextDeltaTimeZero);

    inline bool isPaused(void) { return m_bPaused; }

    inline unsigned int getTotalFrames(void) { return m_uTotalFrames; }
    
    void reshapeProjection(const DSize& newWindowSize);
    
    inline const CAStatusBarStyle& getStatusBarStyle() { return m_eStatusBarStyle; }
    
    void setStatusBarStyle(const CAStatusBarStyle& var);
    
    inline CAThemeManager* getThemeManager() { return m_pThemeManager; }
    
    void setThemeManager(CAThemeManager* var);
    
    void setStatusBarHidden(bool isStatusBarHidden);
    
    bool isStatusBarHidden();
    
    const CAInterfaceOrientation& getStatusBarOrientation();
    
    void setViewport();

    DSize getWinSize(void);

    DSize getVisibleSize();

    DPoint getVisibleOrigin();

    float getZEye(void);

    void runWindow(CAWindow *pWindow);

    void end(void);

    void pause(void);

    void resume(void);

    void restart();

    virtual void stopAnimation(void) = 0;

    virtual void startAnimation(void) = 0;

    void updateDraw();
    
    void drawScene(float dt = 0);
    
    void run(float dt);

    void purgeCachedData(void);

    void setDefaultValues(void);

    void setGLDefaultValues(void);

    void setAlphaBlending(bool bOn);

    void setDepthTest(bool bOn);

    virtual void mainLoop(void) = 0;

    bool isDrawing() {return (m_nDrawCount > 0);}
    
    unsigned long getCurrentNumberOfDraws();

    void setClearColor(const CAColor4F& clearColor);
    
    float getDeltaTime() const;
    
    void pushMatrix(MATRIX_STACK_TYPE type);
    
    void popMatrix(MATRIX_STACK_TYPE type);
    
    void loadIdentityMatrix(MATRIX_STACK_TYPE type);
    
    void loadMatrix(MATRIX_STACK_TYPE type, const Mat4& mat);
    
    void multiplyMatrix(MATRIX_STACK_TYPE type, const Mat4& mat);
    
    const Mat4& getMatrix(MATRIX_STACK_TYPE type) const;
    
    void resetMatrixStack();
    
    inline CAApplication::Projection getProjection(void) { return m_eProjection; }
    
    void setProjection(CAApplication::Projection kProjection);
    
    CAScheduler* getScheduler() const { return m_pScheduler; }
    
    void setScheduler(CAScheduler* scheduler);
    
    ActionManager* getActionManager() const { return m_pActionManager; }
    
    void setActionManager(ActionManager* actionManager);
    
    Renderer* getRenderer() const { return m_pRenderer; }
    
    CAImageCache* getImageCache(){ return m_pImageCache; }
    
    void setImageCache(CAImageCache* imageCache);
    
    CANotificationCenter* getNotificationCenter() const { return m_pNotificationCenter; }
    
    CAMotionManager* getMotionManager() const { return m_pMotionManager; }
    
    LanguageType getCurrentLanguage();
    
    CAConsole* getConsole() const { return m_pConsole; }

    const std::thread::id& getCrossAppThreadId() const { return m_uCrossAppThreadID; }
    
    void setCrossAppCCLogNotification(bool var) { m_bCrossAppCCLogNotification = var; }
    
    bool isCrossAppCCLogNotification() const { return m_bCrossAppCCLogNotification; }
    
public:

    CC_PROPERTY(CATouchDispatcher*, m_pTouchDispatcher, TouchDispatcher);

    CC_PROPERTY(CAKeypadDispatcher*, m_pKeypadDispatcher, KeypadDispatcher);

	CC_PROPERTY_READONLY(float, m_fDeltaTime, DeltaTime);
    
    CC_SYNTHESIZE_READONLY(unsigned long, m_uNumberOfDraws, NumberOfDraws);

    CC_LISTENING_FUNCTION(void(), Reset);
    
public:

    static CAApplication* getApplication(void);
	
protected:
    
    void reset();
    
    void initMatrixStack();
    
    void purgeApplication();
    void restartApplication();
    
    bool m_bPurgeApplicationInNextLoop;
    bool m_bRestartApplicationInNextLoop;
    
    void destroyImageCache();
    
    void showStats();
    void createStatsLabel();
    void calculateMPF();
    void getFPSImageData(unsigned char** datapointer, unsigned int* length);
    void calculateDeltaTime();
    
protected:
    
    std::stack<Mat4> _modelViewMatrixStack;
    std::stack<Mat4> _projectionMatrixStack;
    std::stack<Mat4> _textureMatrixStack;
    
    std::thread::id m_uCrossAppThreadID;
    
    std::chrono::steady_clock::time_point m_fLastUpdate;
    
    Renderer *m_pRenderer;
    
    experimental::FrameBuffer* m_defaultFBO;
    
    CAConsole* m_pConsole;
    
    CAScheduler *m_pScheduler;
    
    ActionManager *m_pActionManager;
    
    CCEGLView    *m_pobOpenGLView;

    CAImageCache* m_pImageCache;
    
    CANotificationCenter* m_pNotificationCenter;
    
    CAMotionManager* m_pMotionManager;
    
    double m_dAnimationInterval;
    
    double m_dOldAnimationInterval;

    bool m_bLandscape;
    
    bool m_bDisplayStats;
    
    float m_fAccumDt;
    
    float m_fFrameRate;
    
    CALabel *m_pFPSLabel;

    bool m_bPaused;

    unsigned int m_uTotalFrames;
    
    unsigned int m_uFrames;
    
    float m_fSecondsPerFrame;
     
    CAWindow *m_pRootWindow;
    
    std::chrono::steady_clock::time_point m_lastUpdate;

    bool m_bNextDeltaTimeZero;
    
    Projection m_eProjection;

    DSize    m_obWinSizeInPoints;

    char *m_pszFPS;
    
    CAStatusBarStyle m_eStatusBarStyle;
    
    CAThemeManager* m_pThemeManager;
    
    int m_nDrawCount;
    
    bool m_bCrossAppCCLogNotification;
    
    friend class CCEGLViewProtocol;
};

#define GETINSTANCE_THEMEMAP(__KEY__) CAApplication::getApplication()->getThemeManager()->getThemeMap(__KEY__)

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

NS_CC_END

#endif // __CCDIRECTOR_H__

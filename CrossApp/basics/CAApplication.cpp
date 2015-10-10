#include <string>
#include "CAApplication.h"
#include "CAFPSImages.h"
#include "view/CADrawingPrimitives.h"
#include "cocoa/CCNS.h"
#include "view/CAWindow.h"
#include "CAScheduler.h"
#include "ccMacros.h"
#include "dispatcher/CATouchDispatcher.h"
#include "support/CAPointExtension.h"
#include "support/CANotificationCenter.h"
#include "images/CAImageCache.h"
#include "basics/CAAutoreleasePool.h"
#include "platform/platform.h"
#include "platform/CCFileUtils.h"
#include "CCApplication.h"
#include "dispatcher/CAKeypadDispatcher.h"
#include "CCAccelerometer.h"
#include "dispatcher/CATouch.h"
#include "support/user_default/CAUserDefault.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CAShaderCache.h"
#include "kazmath/kazmath.h"
#include "kazmath/GL/matrix.h"
#include "support/CAProfiling.h"
#include "CCEGLView.h"
#include "platform/CADensityDpi.h"
#include "view/CALabelStyle.h"

/**
 Position of the FPS
 
 Default: 0,0 (bottom-left corner)
 */
#ifndef CC_DIRECTOR_STATS_POSITION
#define CC_DIRECTOR_STATS_POSITION CAApplication::getApplication()->getVisibleOrigin()
#endif

using namespace std;

NS_CC_BEGIN

static CCDisplayLinkDirector *s_SharedApplication = NULL;

#define kDefaultFPS        60  // 60 frames per second
extern const char* CrossAppVersion(void);

CAApplication* CAApplication::getApplication()
{
    if (!s_SharedApplication)
    {
        s_SharedApplication = new CCDisplayLinkDirector();
        s_SharedApplication->init();
    }

    return s_SharedApplication;
}

CAApplication::CAApplication(void)
{

}

bool CAApplication::init(void)
{
	setDefaultValues();

    // scenes
    m_pRootWindow = NULL;

    m_pNotificationNode = NULL;

    // projection delegate if "Custom" projection is used
    m_pProjectionDelegate = NULL;

    // FPS
    m_fAccumDt = 0.0f;
    m_fFrameRate = 0.0f;
    m_pFPSLabel = NULL;
    m_uTotalFrames = m_uFrames = 0;
    m_pszFPS = new char[10];
    m_pLastUpdate = new struct cc_timeval();
    m_fSecondsPerFrame = 0.0f;

    m_nDrawCount = 60;
    m_dAnimationInterval = 1.0 / 100.0f;
    m_bDisplayStats = false;
    m_uNumberOfDraws = 0;
    // paused ?
    m_bPaused = false;
   
    // purge ?
    m_bPurgeDirecotorInNextLoop = false;

    m_obWinSizeInPoints = DSizeZero;    

    m_pobOpenGLView = NULL;

    // touchDispatcher
    m_pTouchDispatcher = new CATouchDispatcher();
    m_pTouchDispatcher->init();

    // KeypadDispatcher
    m_pKeypadDispatcher = new CAKeypadDispatcher();

    // Accelerometer
    m_pAccelerometer = new CCAccelerometer();

    // create autorelease pool
    CAPoolManager::sharedPoolManager()->push();

    m_fAdaptationRatio = CADensityDpi::getDensityDpi() / 320.0f;
    
    return true;
}
    
CAApplication::~CAApplication(void)
{
    CC_SAFE_RELEASE(m_pFPSLabel);
    
    CC_SAFE_RELEASE(m_pRootWindow);
    CC_SAFE_RELEASE(m_pNotificationNode);
    CC_SAFE_RELEASE(m_pTouchDispatcher);
    CC_SAFE_RELEASE(m_pKeypadDispatcher);
    CC_SAFE_DELETE(m_pAccelerometer);

    // pop the autorelease pool
    CAPoolManager::sharedPoolManager()->pop();
    CAPoolManager::purgePoolManager();

    // delete m_pLastUpdate
    CC_SAFE_DELETE(m_pLastUpdate);
    // delete fps string
    delete []m_pszFPS;

    s_SharedApplication = NULL;
}

void CAApplication::setDefaultValues(void)
{

	m_dOldAnimationInterval = m_dAnimationInterval = 1.0 / 100;


	const char *projection = "3d";
	if( strcmp(projection, "3d") == 0 )
        m_eProjection = CAApplication::P3D;
	else if (strcmp(projection, "2d") == 0)
		m_eProjection = CAApplication::P2D;
	else if (strcmp(projection, "custom") == 0)
		m_eProjection = CAApplication::PCustom;
	else
		CCAssert(false, "Invalid projection value");

}

void CAApplication::setGLDefaultValues(void)
{
    // This method SHOULD be called only after openGLView_ was initialized
    CCAssert(m_pobOpenGLView, "opengl view should not be null");

    setAlphaBlending(true);
    setDepthTest(false);
    setProjection(m_eProjection);

    // set other opengl default values
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void CAApplication::updateDraw()
{
    m_nDrawCount = 30;
}

void CAApplication::drawScene(float dt)
{
    //tick before glClear: issue #533
    
    if (m_nDrawCount > 0)
    {
        --m_nDrawCount;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        if (m_pobOpenGLView)
        {
            m_pobOpenGLView->checkContext();
        }
#endif
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        kmGLPushMatrix();
        
        kmGLTranslatef(-0.5f, 0, 0);
        m_uNumberOfDraws = 0;
        // draw the scene
        if (m_pRootWindow)
        {
            m_pRootWindow->visit();
        }
        
        // draw the notifications node
        if (m_pNotificationNode)
        {
            m_pNotificationNode->visit();
        }
        
        if (m_bDisplayStats)
        {
            showStats();
        }
        
        kmGLPopMatrix();
        
        m_uTotalFrames++;
        
        // swap buffers
        if (m_pobOpenGLView)
        {
            m_pobOpenGLView->swapBuffers();
        }
        
        if (m_bDisplayStats)
        {
            calculateMPF();
        }
    }
}

void CAApplication::calculateDeltaTime(void)
{
    struct cc_timeval now;

    if (CCTime::gettimeofdayCrossApp(&now, NULL) != 0)
    {
        CCLOG("error in gettimeofday");
        m_fDeltaTime = 0;
        return;
    }

    // new delta time. Re-fixed issue #1277
    if (m_bNextDeltaTimeZero)
    {
        m_fDeltaTime = 0;
        m_bNextDeltaTimeZero = false;
    }
    else
    {
        m_fDeltaTime = (now.tv_sec - m_pLastUpdate->tv_sec) + (now.tv_usec - m_pLastUpdate->tv_usec) / 1000000.0f;
        m_fDeltaTime = MAX(0, m_fDeltaTime);
    }

#ifdef DEBUG
    // If we are debugging our code, prevent big delta time
    if(m_fDeltaTime > 0.2f)
    {
        m_fDeltaTime = 1 / 60.0f;
    }
#endif

    *m_pLastUpdate = now;
}
float CAApplication::getDeltaTime()
{
	return m_fDeltaTime;
}
void CAApplication::setOpenGLView(CCEGLView *pobOpenGLView)
{
    CCAssert(pobOpenGLView, "opengl view should not be null");

    if (m_pobOpenGLView != pobOpenGLView)
    {

        // EAGLView is not a CAObject
        if(m_pobOpenGLView)
            delete m_pobOpenGLView; // [openGLView_ release]
        m_pobOpenGLView = pobOpenGLView;

        // set size
        m_obWinSizeInPoints = m_pobOpenGLView->getDesignResolutionSize();
        
        createStatsLabel();
        
        if (m_pobOpenGLView)
        {
            setGLDefaultValues();
        }  
        
        CHECK_GL_ERROR_DEBUG();

        m_pobOpenGLView->setTouchDelegate(m_pTouchDispatcher);
        m_pTouchDispatcher->setDispatchEvents(true);
    }
}

void CAApplication::setViewport()
{
    if (m_pobOpenGLView)
    {
        m_pobOpenGLView->setViewPortInPoints(0, 0, m_obWinSizeInPoints.width, m_obWinSizeInPoints.height);
    }
}

void CAApplication::setNextDeltaTimeZero(bool bNextDeltaTimeZero)
{
    m_bNextDeltaTimeZero = bNextDeltaTimeZero;
}

void CAApplication::setProjection(CAApplication::Projection kProjection)
{
    DSize size = m_obWinSizeInPoints;

    setViewport();

    switch (kProjection)
    {
    case CAApplication::P2D:
        {
            kmGLMatrixMode(KM_GL_PROJECTION);
            kmGLLoadIdentity();
            kmMat4 orthoMatrix;
            kmMat4OrthographicProjection(&orthoMatrix, 0, size.width, 0, size.height, -1024, 1024 );
            kmGLMultMatrix(&orthoMatrix);
            kmGLMatrixMode(KM_GL_MODELVIEW);
            kmGLLoadIdentity();
        }
        break;

    case CAApplication::P3D:
        {
            float zeye = this->getZEye();

            kmMat4 matrixPerspective, matrixLookup;

            kmGLMatrixMode(KM_GL_PROJECTION);
            kmGLLoadIdentity();
            
            // issue #1334
            kmMat4PerspectiveProjection( &matrixPerspective, 60, (GLfloat)size.width/size.height, 0.1f, zeye*2);
            // kmMat4PerspectiveProjection( &matrixPerspective, 60, (GLfloat)size.width/size.height, 0.1f, 1500);

            kmGLMultMatrix(&matrixPerspective);

            kmGLMatrixMode(KM_GL_MODELVIEW);
            kmGLLoadIdentity();
            kmVec3 eye, center, up;
            kmVec3Fill( &eye, size.width/2, size.height/2, zeye );
            kmVec3Fill( &center, size.width/2, size.height/2, 0.0f );
            kmVec3Fill( &up, 0.0f, 1.0f, 0.0f);
            kmMat4LookAt(&matrixLookup, &eye, &center, &up);
            kmGLMultMatrix(&matrixLookup);
        }
        break;
            
    case CAApplication::PCustom:
        if (m_pProjectionDelegate)
        {
            m_pProjectionDelegate->updateProjection();
        }
        break;
            
    default:
        CCLOG("CrossApp: CAApplication: unrecognized projection");
        break;
    }

    m_eProjection = kProjection;
    ccSetProjectionMatrixDirty();
}

void CAApplication::purgeCachedData(void)
{
    if (s_SharedApplication->getOpenGLView())
    {
        CAImageCache::sharedImageCache()->removeUnusedImages();
    }
    CCFileUtils::sharedFileUtils()->purgeCachedEntries();
}

float CAApplication::getZEye(void)
{
    return (m_obWinSizeInPoints.height / 1.1566f);
}

void CAApplication::setAlphaBlending(bool bOn)
{
    if (bOn)
    {
        ccGLBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    }
    else
    {
        ccGLBlendFunc(GL_ONE, GL_ZERO);
    }

    CHECK_GL_ERROR_DEBUG();
}

void CAApplication::reshapeProjection(const DSize& newWindowSize)
{
	CC_UNUSED_PARAM(newWindowSize);
	if (m_pobOpenGLView)
	{
		m_obWinSizeInPoints = DSize(newWindowSize.width, newWindowSize.height);
		setProjection(m_eProjection);       
	}

}

void CAApplication::setStatusBarStyle(const CAStatusBarStyle &var)
{
    m_eStatusBarStyle = var;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCApplication::sharedApplication()->setStatusBarStyle(var);
#endif
    
}

void CAApplication::setDepthTest(bool bOn)
{
    if (bOn)
    {
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    CHECK_GL_ERROR_DEBUG();
}

static void
GLToClipTransform(kmMat4 *transformOut)
{
	kmMat4 projection;
	kmGLGetMatrix(KM_GL_PROJECTION, &projection);

	kmMat4 modelview;
	kmGLGetMatrix(KM_GL_MODELVIEW, &modelview);
	
	kmMat4Multiply(transformOut, &projection, &modelview);
}

DPoint CAApplication::convertToGL(const DPoint& uiPoint)
{
    kmMat4 transform;
	GLToClipTransform(&transform);
	
	kmMat4 transformInv;
	kmMat4Inverse(&transformInv, &transform);
	
	// Calculate z=0 using -> transform*[0, 0, 0, 1]/w
	kmScalar zClip = transform.mat[14]/transform.mat[15];
	
    DSize glSize = m_pobOpenGLView->getDesignResolutionSize();
	kmVec3 clipCoord = {2.0f*uiPoint.x/glSize.width - 1.0f, 1.0f - 2.0f*uiPoint.y/glSize.height, zClip};
	
	kmVec3 glCoord;
	kmVec3TransformCoord(&glCoord, &clipCoord, &transformInv);
	
	return DPoint(glCoord.x, glCoord.y);
}

DPoint CAApplication::convertToUI(const DPoint& glPoint)
{
    kmMat4 transform;
	GLToClipTransform(&transform);
    
	kmVec3 clipCoord;
	// Need to calculate the zero depth from the transform.
	kmVec3 glCoord = {glPoint.x, glPoint.y, 0.0};
	kmVec3TransformCoord(&clipCoord, &glCoord, &transform);
	
	DSize glSize = m_pobOpenGLView->getDesignResolutionSize();
	return DPoint(glSize.width*(clipCoord.x*0.5 + 0.5), glSize.height*(-clipCoord.y*0.5 + 0.5));
}

DSize CAApplication::getWinSize(void)
{
    return m_obWinSizeInPoints;
}

DSize CAApplication::getVisibleSize()
{
    if (m_pobOpenGLView)
    {
        return m_pobOpenGLView->getVisibleSize();
    }
    else 
    {
        return DSizeZero;
    }
}

DPoint CAApplication::getVisibleOrigin()
{
    if (m_pobOpenGLView)
    {
        return m_pobOpenGLView->getVisibleOrigin();
    }
    else 
    {
        return DPointZero;
    }
}

// scene management

void CAApplication::runWindow(CAWindow *pWindow)
{
    CCAssert(pWindow != NULL, "This command can only be used to start the CAApplication. There is already a scene present.");
    CCAssert(m_pRootWindow == NULL, "m_pRootWindow should be null");

    m_bSendCleanupToScene = false;
    
    pWindow->retain();
    m_pRootWindow = pWindow;
    
    startAnimation();
 
    CAScheduler::schedule(schedule_selector(CAApplication::run), this, 0);
}

void CAApplication::run(float dt)
{
    if (m_pRootWindow)
    {
        m_pRootWindow->onEnter();
        m_pRootWindow->onEnterTransitionDidFinish();
    }
    CAScheduler::unschedule(schedule_selector(CAApplication::run), this);
}

void CAApplication::end()
{
    m_bPurgeDirecotorInNextLoop = true;
}

void CAApplication::purgeDirector()
{
    // cleanup scheduler
    CAScheduler::unscheduleAll();
    
    // don't release the event handlers
    // They are needed in case the director is run again

    if (m_pRootWindow)
    {
        m_pRootWindow->onExitTransitionDidStart();
        m_pRootWindow->onExit();
        m_pRootWindow->release();
    }
    
    m_pRootWindow = NULL;

    stopAnimation();

    CC_SAFE_RELEASE_NULL(m_pFPSLabel);

    // purge all managed caches
    ccDrawFree();
    CAImageCache::purgeSharedImageCache();
    CAShaderCache::purgeSharedShaderCache();
    CCFileUtils::purgeFileUtils();
	CALabelStyleCache::purgeSharedStyleCache();

    // CrossApp specific data structures
    CAUserDefault::purgeSharedUserDefault();
    CANotificationCenter::purgeNotificationCenter();

    ccGLInvalidateStateCache();
    
    CHECK_GL_ERROR_DEBUG();
    
    // OpenGL view
    m_pobOpenGLView->end();
    m_pobOpenGLView = NULL;

    // delete CAApplication
    release();
}

void CAApplication::pause(void)
{
    if (m_bPaused)
    {
        return;
    }

    m_dOldAnimationInterval = m_dAnimationInterval;

    // when paused, don't consume CPU
    setAnimationInterval(1 / 4.0);
    m_bPaused = true;
}

void CAApplication::resume(void)
{
    if (! m_bPaused)
    {
        return;
    }

    setAnimationInterval(m_dOldAnimationInterval);

    if (CCTime::gettimeofdayCrossApp(m_pLastUpdate, NULL) != 0)
    {
        CCLOG("CrossApp: CAApplication: Error in gettimeofday");
    }

    m_bPaused = false;
    m_fDeltaTime = 0;
}

// display the FPS using a LabelAtlas
// updates the FPS every frame
void CAApplication::showStats(void)
{
    m_uFrames++;
    m_fAccumDt += m_fDeltaTime;
    
    if (m_bDisplayStats && m_pFPSLabel)
    {
        if (m_fAccumDt > CC_DIRECTOR_STATS_INTERVAL)
        {
            m_fFrameRate = m_uFrames / m_fAccumDt;
            m_uFrames = 0;
            m_fAccumDt = 0;
            
            sprintf(m_pszFPS, "%.1f", m_fFrameRate);
            m_pFPSLabel->setText(m_pszFPS);
        }
        m_pFPSLabel->visit();
    }
}

void CAApplication::calculateMPF()
{
    struct cc_timeval now;
    CCTime::gettimeofdayCrossApp(&now, NULL);
    
    m_fSecondsPerFrame = (now.tv_sec - m_pLastUpdate->tv_sec) + (now.tv_usec - m_pLastUpdate->tv_usec) / 1000000.0f;
}

// returns the FPS image data pointer and len
void CAApplication::getFPSImageData(unsigned char** datapointer, unsigned int* length)
{
    // XXX fixed me if it should be used 
    *datapointer = cc_fps_images_png;
	*length = cc_fps_images_len();
}

void CAApplication::createStatsLabel()
{
    CAImage* image = NULL;
    CAImageCache *ImageCache = CAImageCache::sharedImageCache();

    if( m_pFPSLabel)
    {
        CC_SAFE_RELEASE_NULL(m_pFPSLabel);
        ImageCache->removeImageForKey("cc_fps_images");
        CCFileUtils::sharedFileUtils()->purgeCachedEntries();
    }

    unsigned char *data = NULL;
    unsigned int data_len = 0;
    getFPSImageData(&data, &data_len);

    image = CAImage::createWithImageData(data, data_len, "cc_fps_images");
    
    float factor = CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height / 640.0f;

    m_pFPSLabel = CALabel::createWithFrame(DRect(0, 0, 100, 32));
    m_pFPSLabel->retain();
    m_pFPSLabel->setScale(factor);
	m_pFPSLabel->setColor(CAColor_blue);
    m_pFPSLabel->setFrameOrigin(CC_DIRECTOR_STATS_POSITION);
}

CAView* CAApplication::getNotificationView()
{ 
    return m_pNotificationNode; 
}

void CAApplication::setNotificationView(CAView *view)
{
    if (m_pNotificationNode)
    {
        m_pNotificationNode->resignFirstResponder();
    }
    CC_SAFE_RELEASE(m_pNotificationNode);
    m_pNotificationNode = view;
    if (m_pNotificationNode)
    {
        m_pNotificationNode->becomeFirstResponder();
    }
    CC_SAFE_RETAIN(m_pNotificationNode);
    this->updateDraw();
}

CAApplicationDelegate* CAApplication::getDelegate() const
{
    return m_pProjectionDelegate;
}

void CAApplication::setDelegate(CAApplicationDelegate* pDelegate)
{
    m_pProjectionDelegate = pDelegate;
}

void CAApplication::setTouchDispatcher(CATouchDispatcher* pTouchDispatcher)
{
    if (m_pTouchDispatcher != pTouchDispatcher)
    {
        CC_SAFE_RETAIN(pTouchDispatcher);
        CC_SAFE_RELEASE(m_pTouchDispatcher);
        m_pTouchDispatcher = pTouchDispatcher;
    }    
}

CATouchDispatcher* CAApplication::getTouchDispatcher()
{
    return m_pTouchDispatcher;
}

void CAApplication::setKeypadDispatcher(CAKeypadDispatcher* pKeypadDispatcher)
{
    CC_SAFE_RETAIN(pKeypadDispatcher);
    CC_SAFE_RELEASE(m_pKeypadDispatcher);
    m_pKeypadDispatcher = pKeypadDispatcher;
}

CAKeypadDispatcher* CAApplication::getKeypadDispatcher()
{
    return m_pKeypadDispatcher;
}

void CAApplication::setAccelerometer(CCAccelerometer* pAccelerometer)
{
    if (m_pAccelerometer != pAccelerometer)
    {
        CC_SAFE_DELETE(m_pAccelerometer);
        m_pAccelerometer = pAccelerometer;
    }
}

CCAccelerometer* CAApplication::getAccelerometer()
{
    return m_pAccelerometer;
}

unsigned long CAApplication::getCurrentNumberOfDraws()
{
    return ++m_uNumberOfDraws;
}

/***************************************************
* implementation of DisplayLinkDirector
**************************************************/

// should we implement 4 types of director ??
// I think DisplayLinkDirector is enough
// so we now only support DisplayLinkDirector
void CCDisplayLinkDirector::startAnimation(void)
{
    if (CCTime::gettimeofdayCrossApp(m_pLastUpdate, NULL) != 0)
    {
        CCLOG("CrossApp: DisplayLinkDirector: Error on gettimeofday");
    }

    m_bInvalid = false;
#ifndef EMSCRIPTEN
    CCApplication::sharedApplication()->setAnimationInterval(m_dAnimationInterval);
#endif // EMSCRIPTEN
    CAApplication::getApplication()->updateDraw();
}

void CCDisplayLinkDirector::mainLoop(void)
{
    // calculate "global" dt
    calculateDeltaTime();
    
    if (m_bPurgeDirecotorInNextLoop)
    {
        m_bPurgeDirecotorInNextLoop = false;
        purgeDirector();
    }
    else if (! m_bInvalid)
     {
         if (! m_bPaused)
         {
             CAScheduler::getScheduler()->update(m_fDeltaTime);
             drawScene();
         }
         CAPoolManager::sharedPoolManager()->pop();        
     }
}

void CCDisplayLinkDirector::stopAnimation(void)
{
    m_bInvalid = true;
}

void CCDisplayLinkDirector::setAnimationInterval(double dValue)
{
    m_dAnimationInterval = dValue;
    if (! m_bInvalid)
    {
        stopAnimation();
        startAnimation();
    }    
}

NS_CC_END


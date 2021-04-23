#include "CCEGLViewProtocol.h"
#include "dispatcher/CATouchDispatcher.h"
#include "dispatcher/CATouch.h"
#include "cocoa/CCSet.h"
#include "platform/CADensityDpi.h"
#include "basics/CACamera.h"
#include "basics/CAApplication.h"
#include "basics/CANotificationCenter.h"
#include "CrossApp-Prefix.h"
#include "CCApplication.h"
NS_CC_BEGIN

static CATouch* s_pTouches[CC_MAX_TOUCHES] = { NULL };
static CATouch* s_pMouseMoved = NULL;
static unsigned int s_indexBitsUsed = 0;
static std::map<intptr_t, int> s_TouchesIntergerDict;

static int getUnUsedIndex()
{
    int i;
    int temp = s_indexBitsUsed;

    for (i = 0; i < CC_MAX_TOUCHES; i++) {
        if (! (temp & 0x00000001)) {
            s_indexBitsUsed |= (1 <<  i);
            return i;
        }

        temp >>= 1;
    }

    // all bits are used
    return -1;
}

static void removeUsedIndexBit(int index)
{
    if (index < 0 || index >= CC_MAX_TOUCHES) 
    {
        return;
    }

    unsigned int temp = 1 << index;
    temp = ~temp;
    s_indexBitsUsed &= temp;
}

CCEGLViewProtocol::CCEGLViewProtocol()
: m_pDelegate(nullptr)
, m_fScale(1.0f)
, m_eOrientation(CAInterfaceOrientation::Unknown)
{
}

CCEGLViewProtocol::~CCEGLViewProtocol()
{

}


const DSize& CCEGLViewProtocol::getDesignResolutionSize() const 
{
    return m_obDesignResolutionSize;
}

const DSize& CCEGLViewProtocol::getFrameSize() const
{
    return m_obScreenSize;
}

void CCEGLViewProtocol::setFrameSize(float width, float height)
{
    m_obScreenSize = DSize(width, height);
    
    CCApplication* nativeApp = CCApplication::sharedApplication();
    
    DSize customScreenSize = nativeApp->applicationScreenSize(m_obScreenSize);
    if (nativeApp && !customScreenSize.equals(DSizeZero))
    {
        float x = customScreenSize.width > 0 ? width / customScreenSize.width : 0;
        float y = customScreenSize.height > 0 ? height / customScreenSize.height : 0;
        m_fScale = MIN(x, y);
        CADensityDpi::setDensityDpi(m_fScale * DPI_STANDARD);
        m_obDesignResolutionSize.setSize(customScreenSize.width, customScreenSize.height);
        
        float viewPortX = (m_obScreenSize.width - m_obDesignResolutionSize.width * m_fScale) / 2;
        float viewPortY = (m_obScreenSize.height - m_obDesignResolutionSize.height * m_fScale) / 2;
        m_obViewPortRect.setRect(viewPortX, viewPortY, m_obScreenSize.width, m_obScreenSize.height);
    }
    else
    {
        m_obDesignResolutionSize.setSize(s_px_to_dip(width), s_px_to_dip(height));
        m_fScale = s_dip_to_px(1.0f);
        m_obViewPortRect.setRect(0, 0, m_obScreenSize.width, m_obScreenSize.height);
    }
    
    
    CAApplication::getApplication()->reshapeProjection(m_obDesignResolutionSize);
}

DSize  CCEGLViewProtocol::getVisibleSize() const
{
    return m_obDesignResolutionSize;
}

DPoint CCEGLViewProtocol::getVisibleOrigin() const
{
    return DPointZero;
}

void CCEGLViewProtocol::setTouchDelegate(CCEGLTouchDelegate * pDelegate)
{
    m_pDelegate = pDelegate;
}

void CCEGLViewProtocol::setViewPortInPoints(float x , float y , float w , float h)
{
    experimental::Viewport vp((float)(x * m_fScale + m_obViewPortRect.origin.x),
                              (float)(y * m_fScale + m_obViewPortRect.origin.y),
                              (float)(w * m_fScale),
                              (float)(h * m_fScale));
    
    CACamera::setDefaultViewport(vp);
}

void CCEGLViewProtocol::setScissorInPoints(float x , float y , float w , float h)
{
    glScissor((GLint)(x * m_fScale + m_obViewPortRect.origin.x),
              (GLint)(y * m_fScale + m_obViewPortRect.origin.y),
              (GLsizei)ceilf(w * m_fScale),
              (GLsizei)ceilf(h * m_fScale));
}

bool CCEGLViewProtocol::isScissorEnabled()
{
	return (GL_FALSE == glIsEnabled(GL_SCISSOR_TEST)) ? false : true;
}

DRect CCEGLViewProtocol::getScissorRect()
{
	GLfloat params[4];
    
    glGetFloatv(GL_SCISSOR_BOX, params);
    params[0] = (params[0] / m_fScale - m_obViewPortRect.origin.x);
    params[1] = (params[1] / m_fScale - m_obViewPortRect.origin.y);
    params[2] = params[2] / m_fScale;
    params[3] = params[3] / m_fScale;
    
	return DRect(params[0], params[1], params[2], params[3]);
}

void CCEGLViewProtocol::setViewName(const char* pszViewName)
{
    if (pszViewName != NULL && strlen(pszViewName) > 0)
    {
        strncpy(m_szViewName, pszViewName, sizeof(m_szViewName));
    }
}

const char* CCEGLViewProtocol::getViewName()
{
    return m_szViewName;
}

void CCEGLViewProtocol::handleTouchesBegin(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    CCSet set;
    for (int i = 0; i < num; ++i)
    {
        intptr_t id = ids[i];
        float x = xs[i];
        float y = ys[i];

        int nUnusedIndex = 0;

        // it is a new touch
        if (s_TouchesIntergerDict.find(id) == s_TouchesIntergerDict.end())
        {
            nUnusedIndex = getUnUsedIndex();

            // The touches is more than MAX_TOUCHES ?
            if (nUnusedIndex == -1)
            {
                CCLOG("The touches is more than MAX_TOUCHES, nUnusedIndex = %d", nUnusedIndex);
                continue;
            }

            CATouch* pTouch = s_pTouches[nUnusedIndex] = new CATouch();
			pTouch->setTouchInfo(nUnusedIndex,
                                 (x - m_obViewPortRect.origin.x) / m_fScale,
                                 (y - m_obViewPortRect.origin.y) / m_fScale);
            
            
            s_TouchesIntergerDict.insert(std::make_pair(id, nUnusedIndex));
            set.addObject(pTouch);
        }
    }

    if (set.count() == 0)
    {
        CCLOG("touchesBegan: count = 0");
        return;
    }

    if (m_pDelegate) m_pDelegate->touchesBegan(&set, event);
}

void CCEGLViewProtocol::handleTouchesMove(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    CCSet set;
    for (int i = 0; i < num; ++i)
    {
        intptr_t id = ids[i];
        float x = xs[i];
        float y = ys[i];

        CC_CONTINUE_IF(s_TouchesIntergerDict.find(id) == s_TouchesIntergerDict.end());
        int index = s_TouchesIntergerDict.at(id);
        
        CCLOG("Moving touches with id: %d, x=%f, y=%f", id, x, y);
        CATouch* pTouch = s_pTouches[index];
        if (pTouch)
        {
			pTouch->setTouchInfo(index,
                                 (x - m_obViewPortRect.origin.x) / m_fScale,
                                 (y - m_obViewPortRect.origin.y) / m_fScale);
            
            set.addObject(pTouch);
        }
        else
        {
            // It is error, should return.
            CCLOG("Moving touches with id: %ld error", id);
            return;
        }
    }

    if (set.count() == 0)
    {        
        CCLOG("touchesMoved: count = 0");
        return;
    }

    if (m_pDelegate) m_pDelegate->touchesMoved(&set, event);
}

void CCEGLViewProtocol::getSetOfTouchesEndOrCancel(CCSet& set, int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    for (int i = 0; i < num; ++i)
    {
        intptr_t id = ids[i];
        float x = xs[i];
        float y = ys[i];

        CC_CONTINUE_IF(s_TouchesIntergerDict.find(id) == s_TouchesIntergerDict.end());
        int index = s_TouchesIntergerDict.at(id);
        
        /* Add to the set to send to the director */
        CATouch* pTouch = s_pTouches[index];
        if (pTouch)
        {
            CCLOG("Ending touches with id: %d, x=%f, y=%f", id, x, y);
			pTouch->setTouchInfo(index,
                                 (x - m_obViewPortRect.origin.x) / m_fScale,
                                 (y - m_obViewPortRect.origin.y) / m_fScale);

            set.addObject(pTouch);

            // release the object
            pTouch->release();
            s_pTouches[index] = NULL;
            removeUsedIndexBit(index);

            s_TouchesIntergerDict.erase(id);

        } 
        else
        {
            CCLOG("Ending touches with id: %ld error", id);
            return;
        } 

    }

    if (set.count() == 0)
    {
        CCLOG("touchesEnded or touchesCancel: count = 0");
        return;
    }
}

void CCEGLViewProtocol::handleTouchesEnd(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    CCSet set;
    getSetOfTouchesEndOrCancel(set, num, ids, xs, ys, event);
    if (m_pDelegate) m_pDelegate->touchesEnded(&set, event);
}

void CCEGLViewProtocol::handleTouchesCancel(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    CCSet set;
    getSetOfTouchesEndOrCancel(set, num, ids, xs, ys, event);
    if (m_pDelegate) m_pDelegate->touchesCancelled(&set, event);
}

void CCEGLViewProtocol::handleMouseMoved(float x, float y, CAEvent* event)
{
    if (s_pMouseMoved == NULL)
    {
        s_pMouseMoved = new CATouch();
    }
    s_pMouseMoved->setTouchInfo(-1,
                         (x - m_obViewPortRect.origin.x) / m_fScale,
                         (y - m_obViewPortRect.origin.y) / m_fScale);
    
    if (m_pDelegate) m_pDelegate->mouseMoved(s_pMouseMoved, event);
}

void CCEGLViewProtocol::handleScrollWheel(float x, float y, float offx, float offy, CAEvent* event)
{
    if (s_pMouseMoved == NULL)
    {
        s_pMouseMoved = new CATouch();
    }
    s_pMouseMoved->setTouchInfo(-1,
                                (x - m_obViewPortRect.origin.x) / m_fScale,
                                (y - m_obViewPortRect.origin.y) / m_fScale);
    
    if (m_pDelegate) m_pDelegate->mouseScrollWheel(s_pMouseMoved, offx, offy, event);
}


void CCEGLViewProtocol::handleOtherMouseDown(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    
}

void CCEGLViewProtocol::handleOtherMouseDragged(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    
}

void CCEGLViewProtocol::handleOtherMouseUp(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    
}

void CCEGLViewProtocol::handleMouseEntered(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    
}

void CCEGLViewProtocol::handleMouseExited(int num, intptr_t ids[], float xs[], float ys[], CAEvent* event)
{
    
}

const DRect& CCEGLViewProtocol::getViewPortRect() const
{
    return m_obViewPortRect;
}

float CCEGLViewProtocol::getScale() const
{
    return m_fScale;
}

float CCEGLViewProtocol::getFrameZoomFactor() const
{
    return 1;
}

void CCEGLViewProtocol::setStatusBarOrientation(const CAInterfaceOrientation& var)
{
    CC_RETURN_IF(m_eOrientation == var);
    if (m_eOrientation == CAInterfaceOrientation::Unknown)
    {
        m_eOrientation = var;
    }
    else
    {
        m_eOrientation = var;
        CANotificationCenter::getInstance()->postNotification(CAApplicationDidChangeStatusBarOrientationNotification, NULL);
    }
}

const CAInterfaceOrientation& CCEGLViewProtocol::getStatusBarOrientation()
{
    return m_eOrientation;
}

NS_CC_END

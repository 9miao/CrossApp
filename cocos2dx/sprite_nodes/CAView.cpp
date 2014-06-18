//
//  CAView.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-12.
//  Copyright (c) 2014年 http://9miao.com All rights reserved.
//

#include "CAView.h"
#include "cocoa/CCString.h"
#include "support/CCPointExtension.h"
#include "support/TransformUtils.h"
#include "CCCamera.h"
#include "effects/CCGrid.h"
#include "CCDirector.h"
#include "CAScheduler.h"
#include "touch_dispatcher/CCTouch.h"
#include "actions/CCActionManager.h"
#include "shaders/CCGLProgram.h"
#include "CABatchView.h"
#include "kazmath/GL/matrix.h"
#include "support/component/CCComponent.h"
#include "support/component/CCComponentContainer.h"
#include <stdarg.h>
#include "touch_dispatcher/CATouchDispatcher.h"
#include "keypad_dispatcher/CCKeypadDispatcher.h"
#include "CCAccelerometer.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "CCEGLView.h"
#include "cocoa/CCSet.h"

#if CC_NODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

#define SET_DIRTY_RECURSIVELY() {                                       \
                        if (m_pobBatchView && ! m_bRecursiveDirty)      \
                        {                                               \
                            m_bRecursiveDirty = true;                   \
                            setDirty(true);                             \
                            if (m_bHasChildren)                         \
                                setDirtyRecursively(true);              \
                        }                                               \
                    }


NS_CC_BEGIN;



static int viewCount = 0;

// XXX: Yes, nodes might have a sort problem once every 15 days if the game runs at 60 FPS and each frame sprites are reordered.
static int s_globalOrderOfArrival = 1;

CAView::CAView(void)
: m_fRotationX(0.0f)
, m_fRotationY(0.0f)
, m_fScaleX(1.0f)
, m_fScaleY(1.0f)
, m_fVertexZ(0.0f)
, m_fSkewX(0.0f)
, m_fSkewY(0.0f)
, m_obAnchorPointInPoints(kCAViewPointInvalid)
, m_obAnchorPoint(kCAViewPointInvalid)
, m_obContentSize(kCAViewSizeInvalid)
, m_obFrameRect(kCAViewRectInvalid)
, m_obPoint(kCAViewPointInvalid)
, m_sAdditionalTransform(CCAffineTransformMakeIdentity())
, m_pCamera(NULL)
, m_pGrid(NULL)
, m_nZOrder(0)
, m_pSubviews(NULL)
, m_pSuperview(NULL)
, m_pUserData(NULL)
, m_pUserObject(NULL)
, m_pShaderProgram(NULL)
, m_eGLServerState(ccGLServerState(0))
, m_uOrderOfArrival(0)
, m_bRunning(false)
, m_bTransformDirty(true)
, m_bInverseDirty(true)
, m_bAdditionalTransformDirty(false)
, m_bVisible(true)
, m_bReorderChildDirty(false)
, m_pComponentContainer(NULL)
, _displayedAlpha(1.0f)
, _realAlpha(1.0f)
, _displayedColor(CAColor_white)
, _realColor(CAColor_white)
, m_bDisplayRange(true)
, m_pobImage(NULL)
, m_bShouldBeHidden(false)
, m_bFlipX(false)
, m_bFlipY(false)
, m_bRecursiveDirty(false)
, m_bDirty(false)
, m_bHasChildren(false)
, m_pViewDelegate(NULL)
, m_bFrame(true)
{
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;
    CCDirector *director = CCDirector::sharedDirector();
    m_pActionManager = director->getActionManager();
    m_pActionManager->retain();
    
    m_pComponentContainer = new CCComponentContainer(this);
    
    // clean the Quad
    memset(&m_sQuad, 0, sizeof(m_sQuad));
    
    // Atlas: Color
    CAColor4B tmpColor = { 255, 255, 255, 255 };
    m_sQuad.bl.colors = tmpColor;
    m_sQuad.br.colors = tmpColor;
    m_sQuad.tl.colors = tmpColor;
    m_sQuad.tr.colors = tmpColor;
    
    // shader program
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    
    this->setAnchorPoint(CCPoint(0.5f, 0.5f));
    
    m_pobBatchView = NULL;
    m_pobImageAtlas = NULL;
    
    ++viewCount;
//    CCLog("CAView = %d\n",viewCount);
}

CAView::~CAView(void)
{
    CC_SAFE_RELEASE(m_pActionManager);
    CC_SAFE_RELEASE(m_pCamera);
    CC_SAFE_RELEASE(m_pGrid);
    CC_SAFE_RELEASE(m_pShaderProgram);
    CC_SAFE_RELEASE(m_pUserObject);
    
    if(m_pSubviews && m_pSubviews->count() > 0)
    {
        CCObject* subview;
        CCARRAY_FOREACH(m_pSubviews, subview)
        {
            CAView* pChild = (CAView*) subview;
            if (pChild)
            {
                pChild->m_pSuperview = NULL;
            }
        }
    }
    CC_SAFE_RELEASE_NULL(m_pSubviews);

    m_pComponentContainer->removeAll();
    CC_SAFE_DELETE(m_pComponentContainer);
    CC_SAFE_RELEASE(m_pobImage);
    
    --viewCount;
//    CCLog("~CAView = %d\n",viewCount);
}

CAView * CAView::create(void)
{
	CAView * pRet = new CAView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

bool CAView::init()
{
    this->setImage(CAImage::CC_WHITE_IMAGE());
    return true;
}

CAView* CAView::createWithFrame(const CCRect& rect)
{
	CAView * pRet = new CAView();
    if (pRet && pRet->initWithFrame(rect))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

CAView* CAView::createWithFrame(const CCRect& rect, const CAColor4B& color4B)
{
	CAView * pRet = new CAView();
    if (pRet && pRet->initWithFrame(rect, color4B))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

CAView* CAView::createWithCenter(const CCRect& rect)
{
	CAView * pRet = new CAView();
    if (pRet && pRet->initWithCenter(rect))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

CAView* CAView::createWithCenter(const CCRect& rect, const CAColor4B& color4B)
{
    CAView * pRet = new CAView();
    if (pRet && pRet->initWithCenter(rect, color4B))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

CAView* CAView::createWithColor(const CAColor4B& color4B)
{
   	CAView * pRet = new CAView();
    if (pRet && pRet->initWithColor(color4B))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

bool CAView::initWithColor(const CAColor4B& color4B)
{
    if (!CAView::init())
    {
        return false;
    }
    this->setColor(color4B);
    
    return true;
}

bool CAView::initWithFrame(const CCRect& rect)
{
    if (!CAView::init())
    {
        return false;
    }
    this->setFrame(rect);
    
    return true;
}

bool CAView::initWithFrame(const CCRect& rect, const CAColor4B& color4B)
{
    if (!CAView::initWithColor(color4B))
    {
        return false;
    }
    this->setFrame(rect);

    return true;
}

bool CAView::initWithCenter(const CCRect& rect)
{
    if (!CAView::init())
    {
        return false;
    }
    this->setCenter(rect);
    
    return true;
}

bool CAView::initWithCenter(const CCRect& rect, const CAColor4B& color4B)
{
    if (!CAView::initWithColor(color4B))
    {
        return false;
    }
    this->setCenter(rect);
    
    return true;
}

float CAView::getSkewX()
{
    return m_fSkewX;
}

void CAView::setSkewX(float newSkewX)
{
    if (m_fSkewX != newSkewX)
    {
        m_fSkewX = newSkewX;
        this->updateDraw();
    }
}

float CAView::getSkewY()
{
    return m_fSkewY;
}

void CAView::setSkewY(float newSkewY)
{
    if (m_fSkewY != newSkewY)
    {
        m_fSkewY = newSkewY;
        this->updateDraw();
    }
}

/// zOrder getter
int CAView::getZOrder()
{
    return m_nZOrder;
}

/// zOrder setter : private method
/// used internally to alter the zOrder variable. DON'T call this method manually
void CAView::_setZOrder(int z)
{
    m_nZOrder = z;
}

void CAView::setZOrder(int z)
{
    _setZOrder(z);
    if (m_pSuperview)
    {
        m_pSuperview->reorderSubview(this, z);
    }
}

/// vertexZ getter
float CAView::getVertexZ()
{
    return m_fVertexZ;
}


/// vertexZ setter
void CAView::setVertexZ(float var)
{
    m_fVertexZ = var;
}


/// rotation getter
float CAView::getRotation()
{
    CCAssert(m_fRotationX == m_fRotationY, "CAView#rotation. RotationX != RotationY. Don't know which one to return");
    return m_fRotationX;
}

/// rotation setter
void CAView::setRotation(float newRotation)
{
    if (m_fRotationX != newRotation || m_fRotationY != newRotation)
    {
        m_fRotationX = m_fRotationY = newRotation;
        this->updateDraw();
    }
}

float CAView::getRotationX()
{
    return m_fRotationX;
}

void CAView::setRotationX(float fRotationX)
{
    if (m_fRotationX != fRotationX)
    {
        m_fRotationX = fRotationX;
        this->updateDraw();
    }
}

float CAView::getRotationY()
{
    return m_fRotationY;
}

void CAView::setRotationY(float fRotationY)
{
    if (m_fRotationY != fRotationY)
    {
        m_fRotationY = fRotationY;
        this->updateDraw();
    }
}

/// scale getter
float CAView::getScale(void)
{
    CCAssert( m_fScaleX == m_fScaleY, "CAView#scale. ScaleX != ScaleY. Don't know which one to return");
    return m_fScaleX;
}

/// scale setter
void CAView::setScale(float scale)
{
    this->setScale(scale, scale);
}

/// scale setter
void CAView::setScale(float fScaleX,float fScaleY)
{
    if (m_fScaleX != fScaleX || m_fScaleY != fScaleY)
    {
        m_fScaleX = fScaleX;
        m_fScaleY = fScaleY;
        m_obFrameRect.size.width = m_fScaleX * m_obContentSize.width;
        m_obFrameRect.size.height = m_fScaleY * m_obContentSize.height;
        CCPoint point = CCPoint(m_obAnchorPointInPoints.x * m_fScaleX, m_obAnchorPointInPoints.y * m_fScaleY);
        m_obFrameRect.origin = ccpSub(m_obPoint, point);
        this->updateDraw();
    }
}

/// scaleX getter
float CAView::getScaleX()
{
    return m_fScaleX;
}

/// scaleX setter
void CAView::setScaleX(float newScaleX)
{
    if (m_fScaleX != newScaleX)
    {
        m_fScaleX = newScaleX;
        m_obFrameRect.size.width = m_fScaleX * m_obContentSize.width;
        float x = m_obAnchorPointInPoints.x * m_fScaleX;
        m_obFrameRect.origin.x = m_obPoint.y - x;
        this->updateDraw();
    }
}

/// scaleY getter
float CAView::getScaleY()
{
    return m_fScaleY;
}

/// scaleY setter
void CAView::setScaleY(float newScaleY)
{
    if (m_fScaleY != newScaleY)
    {
        m_fScaleY = newScaleY;
        m_obFrameRect.size.height = m_fScaleY * m_obContentSize.height;
        float y = m_obAnchorPointInPoints.y * m_fScaleY;
        m_obFrameRect.origin.y = m_obPoint.y - y;
        this->updateDraw();
    }
}

/// position setter
void CAView::setPoint(const CCPoint& newPoint)
{
    m_obPoint = newPoint;
    CCPoint point = CCPoint(m_obAnchorPointInPoints.x * m_fScaleX,
                            m_obAnchorPointInPoints.y * m_fScaleY);
    m_obFrameRect.origin = ccpSub(m_obPoint, point);
    this->updateDraw();
}

/// children getter
CCArray* CAView::getSubviews()
{
    return m_pSubviews;
}

unsigned int CAView::getSubviewsCount(void) const
{
    return m_pSubviews ? m_pSubviews->count() : 0;
}

/// camera getter: lazy alloc
CCCamera* CAView::getCamera()
{
    if (!m_pCamera)
    {
        m_pCamera = new CCCamera();
    }
    
    return m_pCamera;
}


/// grid getter
CCGridBase* CAView::getGrid()
{
    return m_pGrid;
}

/// grid setter
void CAView::setGrid(CCGridBase* pGrid)
{
    CC_SAFE_RETAIN(pGrid);
    CC_SAFE_RELEASE(m_pGrid);
    m_pGrid = pGrid;
}


/// isVisible getter
bool CAView::isVisible()
{
    return m_bVisible;
}

/// isVisible setter
void CAView::setVisible(bool var)
{
    m_bVisible = var;
    this->updateDraw();
}

const CCPoint& CAView::getAnchorPointInPoints()
{
    return m_obAnchorPointInPoints;
}

/// anchorPoint getter
const CCPoint& CAView::getAnchorPoint()
{
    return m_obAnchorPoint;
}

void CAView::setAnchorPointInPoints(const CCPoint& anchorPointInPoints)
{
    if( ! anchorPointInPoints.equals(m_obAnchorPointInPoints))
    {
        CCPoint p;
        if (m_bFrame)
        {
            p = this->getFrameOrigin();
        }
        else
        {
            p = this->getCenterOrigin();
        }
        
        m_obAnchorPointInPoints = anchorPointInPoints;
        m_obAnchorPoint = ccp(m_obAnchorPointInPoints.x / m_obContentSize.width,
                              m_obAnchorPointInPoints.y / m_obContentSize.height);
        
        if (m_bFrame)
        {
            this->setFrameOrigin(p);
        }
        else
        {
            this->setCenterOrigin(p);
        }
        
        this->updateDraw();
    }
}

void CAView::setAnchorPoint(const CCPoint& point)
{
    if( ! point.equals(m_obAnchorPoint))
    {
        CCPoint p;
        if (m_bFrame)
        {
            p = this->getFrameOrigin();
        }
        else
        {
            p = this->getCenterOrigin();
        }
        
        
        m_obAnchorPoint = point;
        m_obAnchorPointInPoints = ccp(m_obContentSize.width * m_obAnchorPoint.x,
                                      m_obContentSize.height * m_obAnchorPoint.y );
        //m_obFrameRect.origin = ccpSub(m_obPoint, m_obAnchorPointInPoints);
        
        if (m_bFrame)
        {
            this->setFrameOrigin(p);
        }
        else
        {
            this->setCenterOrigin(p);
        }
        
        this->updateDraw();
    }
}

void CAView::setContentSize(const CCSize & size)
{
    if ( ! size.equals(m_obContentSize))
    {
        m_obContentSize = size;
        
        m_obAnchorPointInPoints = ccp(m_obContentSize.width * m_obAnchorPoint.x, m_obContentSize.height * m_obAnchorPoint.y );
        m_obFrameRect.size = CCSize(m_obContentSize.width * m_fScaleX, m_obContentSize.height * m_fScaleY);

        this->updateImageRect();
        
        arrayMakeObjectsPerformSelector(m_pSubviews, reViewlayout, CAView*);
        
        this->updateDraw();
    }
}

const CCRect& CAView::getFrame() const
{
    return m_obFrameRect;
}

void CAView::setFrame(const CCRect &rect)
{
    if ( ! rect.size.equals(CCSizeZero))
    {
        this->setContentSize(CCSize(rect.size.width / m_fScaleX, rect.size.height / m_fScaleY));
    }
    
    this->setFrameOrigin(rect.origin);
}

void CAView::setFrameOrigin(const CCPoint& point)
{
    CCPoint p = CCPoint(m_obAnchorPointInPoints.x * m_fScaleX,
                        m_obAnchorPointInPoints.y * m_fScaleY);
    p = ccpAdd(p, point);
    this->setPoint(p);
    
    m_bFrame = true;
}

const CCPoint& CAView::getFrameOrigin()
{
    return m_obFrameRect.origin;
}

CCRect CAView::getBounds() const
{
    CCRect rect = CCRectZero;
    rect.size = m_obContentSize;
    return rect;
}

void CAView::setBounds(const CCRect& rect)
{
    if ( ! rect.size.equals(CCSizeZero))
    {
        this->setContentSize(CCSize(rect.size.width, rect.size.height));
    }
}

CCRect CAView::getCenter()
{
    CCRect rect = this->getFrame();
    rect.origin = ccpAdd(rect.origin, ccpMult(rect.size, 0.5f));
    return rect;
}

void CAView::setCenter(CCRect rect)
{
    if ( ! rect.size.equals(CCSizeZero))
    {
        this->setContentSize(CCSize(rect.size.width / m_fScaleX, rect.size.height / m_fScaleY));
    }
    
    this->setCenterOrigin(rect.origin);
}

CCPoint CAView::getCenterOrigin()
{
    return this->getCenter().origin;
}

void CAView::setCenterOrigin(const CCPoint& point)
{
    CCPoint p = ccpSub(ccpMult(m_obContentSize, 0.5f), m_obAnchorPointInPoints);
    p = CCPoint(p.x * m_fScaleX, p.y * m_fScaleY);
    p = ccpSub(point, p);
    this->setPoint(p);
    
    m_bFrame = false;
}

// isRunning getter
bool CAView::isRunning()
{
    return m_bRunning;
}

/// parent getter
CAView * CAView::getSuperview()
{
    return m_pSuperview;
}
/// parent setter
void CAView::setSuperview(cocos2d::CAView *var)
{
    m_pSuperview = var;
}

/// userData getter
void * CAView::getUserData()
{
    return m_pUserData;
}

/// userData setter
void CAView::setUserData(void *var)
{
    m_pUserData = var;
}

unsigned int CAView::getOrderOfArrival()
{
    return m_uOrderOfArrival;
}

void CAView::setOrderOfArrival(unsigned int uOrderOfArrival)
{
    m_uOrderOfArrival = uOrderOfArrival;
}

CCGLProgram* CAView::getShaderProgram()
{
    return m_pShaderProgram;
}

CCObject* CAView::getUserObject()
{
    return m_pUserObject;
}

ccGLServerState CAView::getGLServerState()
{
    return m_eGLServerState;
}

void CAView::setGLServerState(ccGLServerState glServerState)
{
    m_eGLServerState = glServerState;
}

void CAView::setUserObject(CCObject *pUserObject)
{
    CC_SAFE_RETAIN(pUserObject);
    CC_SAFE_RELEASE(m_pUserObject);
    m_pUserObject = pUserObject;
}

void CAView::setShaderProgram(CCGLProgram *pShaderProgram)
{
    CC_SAFE_RETAIN(pShaderProgram);
    CC_SAFE_RELEASE(m_pShaderProgram);
    m_pShaderProgram = pShaderProgram;
}

void CAView::cleanup()
{
    // actions
    this->stopAllActions();
    
    // timers
    arrayMakeObjectsPerformSelector(m_pSubviews, cleanup, CAView*);
}


const char* CAView::description()
{
    return CCString::createWithFormat("<CAView | Tag = %d>", m_nTag)->getCString();
}

void CAView::reViewlayout()
{
    m_bTransformDirty = m_bInverseDirty = true;
}

void CAView::updateDraw()
{
    SET_DIRTY_RECURSIVELY();
    if (this->getSuperview())
    {
        this->reViewlayout();
        CCDirector::sharedDirector()->updateDraw();
    }
}

// lazy allocs
void CAView::childrenAlloc(void)
{
    m_pSubviews = new CCArray(4);
}

CAView* CAView::getSubviewByTag(int aTag)
{
    CCAssert( aTag != kCCObjectTagInvalid, "Invalid tag");
    
    if(m_pSubviews && m_pSubviews->count() > 0)
    {
        CCObject* subview;
        CCARRAY_FOREACH(m_pSubviews, subview)
        {
            CAView* pNode = (CAView*) subview;
            if(pNode && pNode->m_nTag == aTag)
                return pNode;
        }
    }
    return NULL;
}


void CAView::addSubview(CAView *subview)
{
    this->insertSubview(subview, subview->getZOrder());
}

// helper used by reorderChild & add
void CAView::insertSubview(CAView* subview, int z)
{
    CCAssert( subview != NULL, "Argument must be non-nil");
    CCAssert( subview->m_pSuperview == NULL, "child already added. It can't be added again");
    
    if (m_pobBatchView)
    {
        m_pobBatchView->appendChild(subview);
        
        if (!m_bReorderChildDirty)
        {
            setReorderChildDirtyRecursively();
        }
    }
    
    if(m_pSubviews == NULL)
    {
        this->childrenAlloc();
    }
    
    m_bReorderChildDirty = true;
    ccArrayAppendObjectWithResize(m_pSubviews->data, subview);
    subview->_setZOrder(z);
    
    subview->setSuperview(this);
    subview->setOrderOfArrival(s_globalOrderOfArrival++);
    
    this->updateDraw();
    
    if( m_bRunning )
    {
        subview->onEnter();
        subview->onEnterTransitionDidFinish();
    }
}

void CAView::removeFromSuperview()
{
    if (m_pSuperview != NULL)
    {
        m_pSuperview->removeSubview(this);
    }
}

void CAView::removeSubview(CAView* subview)
{
    // explicit nil handling
    if (m_pSubviews == NULL)
    {
        return;
    }
    
    if (m_pobBatchView)
    {
        m_pobBatchView->removeSpriteFromAtlas(subview);
    }
    
    if ( m_pSubviews->containsObject(subview) )
    {
        this->detachSubview(subview);
    }
}


void CAView::removeSubviewByTag(int tag)
{
    CCAssert( tag != kCCObjectTagInvalid, "Invalid tag");
    
    CAView *subview = this->getSubviewByTag(tag);
    
    if (subview == NULL)
    {
        CCLOG("cocos2d: removeChildByTag(tag = %d): child not found!", tag);
    }
    else
    {
        this->removeSubview(subview);
    }
}

void CAView::removeAllSubviews()
{
    // not using detachChild improves speed here
    if ( m_pSubviews && m_pSubviews->count() > 0 )
    {
        if (m_pobBatchView)
        {
            CCObject* obj = NULL;
            CCARRAY_FOREACH(m_pSubviews, obj)
            {
                CAView* subview = dynamic_cast<CAView*>(obj);
                if (subview)
                {
                    m_pobBatchView->removeSpriteFromAtlas(subview);
                }
            }
        }
        
        CCObject* obj;
        CCARRAY_FOREACH(m_pSubviews, obj)
        {
            CAView* subview = dynamic_cast<CAView*>(obj);
            if (subview)
            {
                // IMPORTANT:
                //  -1st do onExit
                //  -2nd cleanup
                if(m_bRunning)
                {
                    subview->onExitTransitionDidStart();
                    subview->onExit();
                }
                
                subview->cleanup();
                // set parent nil at the end
                subview->setSuperview(NULL);
            }
        }
        
        m_pSubviews->removeAllObjects();
    }
    
    m_bHasChildren = false;
}


void CAView::detachSubview(CAView *subview)
{
    // IMPORTANT:
    //  -1st do onExit
    //  -2nd cleanup
    if (m_bRunning)
    {
        subview->onExitTransitionDidStart();
        subview->onExit();
    }
    
    // If you don't do cleanup, the child's actions will not get removed and the
    // its scheduledSelectors_ dict will not get released!
    subview->cleanup();
    
    // set parent nil at the end
    subview->setSuperview(NULL);
    
    m_pSubviews->removeObject(subview);
}

void CAView::reorderSubview(CAView *subview, int zOrder)
{
    if (zOrder == subview->getZOrder())
    {
        return;
    }
    
    if( m_pobBatchView && ! m_bReorderChildDirty)
    {
        setReorderChildDirtyRecursively();
        m_pobBatchView->reorderBatch(true);
    }
    
    CCAssert( subview != NULL, "Child must be non-nil");
    m_bReorderChildDirty = true;
    subview->setOrderOfArrival(s_globalOrderOfArrival++);
    subview->_setZOrder(zOrder);
}

void CAView::sortAllSubviews()
{
    if (m_bReorderChildDirty)
    {
        int i,j,length = m_pSubviews->data->num;
        CAView ** x = (CAView**)m_pSubviews->data->arr;
        CAView *tempItem;
        
        // insertion sort
        for(i=1; i<length; i++)
        {
            tempItem = x[i];
            j = i-1;
            
            //continue moving element downwards while zOrder is smaller or when zOrder is the same but mutatedIndex is smaller
            while(j>=0 && ( tempItem->m_nZOrder < x[j]->m_nZOrder || ( tempItem->m_nZOrder== x[j]->m_nZOrder && tempItem->m_uOrderOfArrival < x[j]->m_uOrderOfArrival ) ) )
            {
                x[j+1] = x[j];
                j = j-1;
            }
            x[j+1] = tempItem;
        }
        
        //don't need to check children recursively, that's done in visit of each child
        
        if ( m_pobBatchView)
        {
            arrayMakeObjectsPerformSelector(m_pSubviews, sortAllSubviews, CAView*);
        }
        
        m_bReorderChildDirty = false;
    }
}

#include "CAImageView.h"

void CAView::draw()
{
    //CCAssert(0);
    // override me
    // Only use- this function to draw your stuff.
    // DON'T draw your stuff outside this method
    
    if (m_pobImage == NULL)
        return;
    
    CC_NODE_DRAW_SETUP();
    
    ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
    
    ccGLBindTexture2D( m_pobImage->getName() );
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
    
#define kQuadSize sizeof(m_sQuad.bl)
#ifdef EMSCRIPTEN
    long offset = 0;
    setGLBufferData(&m_sQuad, 4 * kQuadSize, 0);
#else
    long offset = (long)&m_sQuad;
#endif // EMSCRIPTEN
    
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    CHECK_GL_ERROR_DEBUG();
    
    
#if CC_SPRITE_DEBUG_DRAW == 1
    // draw bounding box
    CCPoint vertices[4]={
        ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
        ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
        ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
        ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
    };
    ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
    // draw Image box
    CCSize s = this->getImageRect().size;
    CCPoint offsetPix = this->getOffsetPoint();
    CCPoint vertices[4] = {
        ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
        ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CAImageView - draw");

    
}

void CAView::visit()
{
    // quick return if not visible. children won't be drawn.
    if (!m_bVisible)
    {
        return;
    }
    
    if (m_bDisplayRange)
    {
        this->rendering();
    }
    else
    {
        glEnable(GL_SCISSOR_TEST);// 开启显示指定区域
        
        CCPoint point = this->convertToWorldSpace(CCPoint(0, this->getBounds().size.height));
        point = CCDirector::sharedDirector()->convertToGL(point);
        CCSize size = this->getBounds().size;
        
        CCEGLView* pGLView = CCEGLView::sharedOpenGLView();
        float glScaleX = pGLView->getScaleX();
        float glScaleY = pGLView->getScaleY();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        
        float off_X = 0;
        float off_Y = 0;
        
#else
        
        float off_X = pGLView->getViewPortRect().origin.x;
        float off_Y = pGLView->getViewPortRect().origin.y;
        
#endif
        
        float scaleX = this->getScaleX();
        float scaleY = this->getScaleY();
        
        CAView* parent = this->getSuperview();
        while (parent)
        {
            scaleX *= parent->getScaleX();
            scaleY *= parent->getScaleY();
            parent = parent->getSuperview();
        }
        
        float x = point.x * glScaleX + off_X;
        float y = point.y * glScaleY + off_Y;
        float width = size.width * scaleX * glScaleX;
        float height = size.height * scaleY * glScaleY;
        
		glScissor(x, y, width, height);// 只显示当前窗口的区域
        
        this->rendering();
        
        glDisable(GL_SCISSOR_TEST);// 禁用
    }
}

void CAView::rendering()
{
    kmGLPushMatrix();
    
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
    }
    
    this->transform();
    
    CCRect winRect = CCRectZero;
    winRect.size = CCDirector::sharedDirector()->getWinSize();
    
//    CAView* pNode = NULL;
//    unsigned int i = 0;
//    if(m_pSubviews && m_pSubviews->count() > 0)
//    {
//        this->sortAllSubviews();
//        // draw children zOrder < 0
//        ccArray *arrayData = m_pSubviews->data;
//
//        for( ; i < arrayData->num; i++ )
//        {
//            pNode = (CAView*) arrayData->arr[i];
//            
//            if ( pNode && pNode->m_nZOrder < 0 )
//            {
//                pNode->visit();
//            }
//            else
//            {
//                break;
//            }
//        }
//        
//        this->draw();
//        
//        for( ; i < arrayData->num; i++ )
//        {
//            pNode = (CAView*) arrayData->arr[i];
//            if (pNode && pNode->m_nZOrder >= 0)
//            {
//                pNode->visit();
//            }
//        }
//    }
//    else
//    {
//        this->draw();
//    }
    
    this->draw();
    
    if(m_pSubviews && m_pSubviews->count() > 0)
    {
        this->sortAllSubviews();
        // draw children zOrder < 0
        ccArray *arrayData = m_pSubviews->data;

        for(unsigned int i = 0 ; i < arrayData->num; i++ )
        {
            CAView* pNode = (CAView*) arrayData->arr[i];
            if (pNode)
            {
                pNode->visit();
            }
        }
    }
    
    // reset for next frame
    m_uOrderOfArrival = 0;
    
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }
    
    kmGLPopMatrix();
}


void CAView::transformAncestors()
{
    if( m_pSuperview != NULL  )
    {
        m_pSuperview->transformAncestors();
        m_pSuperview->transform();
    }
}

void CAView::transform()
{
    kmMat4 transfrom4x4;
    
    // Convert 3x3 into 4x4 matrix
    CCAffineTransform tmpAffine = this->nodeToParentTransform();
    CGAffineToGL(&tmpAffine, transfrom4x4.mat);
    
    // Update Z vertex manually
    transfrom4x4.mat[14] = m_fVertexZ;
    
    kmGLMultMatrix( &transfrom4x4 );
    
    
    // XXX: Expensive calls. Camera should be integrated into the cached affine matrix
    if ( m_pCamera != NULL && !(m_pGrid != NULL && m_pGrid->isActive()) )
    {
        CCPoint anchorPointInPoints = CCPoint(m_obAnchorPointInPoints.x,
                                              m_obContentSize.height - m_obAnchorPointInPoints.y);
        
        bool translate = (anchorPointInPoints.x != 0.0f || anchorPointInPoints.y != 0.0f);

        if( translate )
            kmGLTranslatef(RENDER_IN_SUBPIXEL(anchorPointInPoints.x), RENDER_IN_SUBPIXEL(anchorPointInPoints.y), 0 );
        
        m_pCamera->locate();
        
        if( translate )
            kmGLTranslatef(RENDER_IN_SUBPIXEL(-anchorPointInPoints.x), RENDER_IN_SUBPIXEL(-anchorPointInPoints.y), 0 );
    }
    
}


void CAView::onEnter()
{
    arrayMakeObjectsPerformSelector(m_pSubviews, onEnter, CAView*);

    m_bRunning = true;
    
    CAScheduler::getScheduler()->resumeTarget(this);
    m_pActionManager->resumeTarget(this);
}

void CAView::onEnterTransitionDidFinish()
{
    arrayMakeObjectsPerformSelector(m_pSubviews, onEnterTransitionDidFinish, CAView*);
    
    if (m_pViewDelegate)
    {
        m_pViewDelegate->getSuperViewRect(this->getSuperview()->getBounds());
        m_pViewDelegate->viewOnEnterTransitionDidFinish();
    }
}

void CAView::onExitTransitionDidStart()
{
    arrayMakeObjectsPerformSelector(m_pSubviews, onExitTransitionDidStart, CAView*);
    
    if (m_pViewDelegate)
    {
        m_pViewDelegate->viewOnExitTransitionDidStart();
    }
}

void CAView::onExit()
{
    m_bRunning = false;
    
    arrayMakeObjectsPerformSelector(m_pSubviews, onExit, CAView*);
    
    CAScheduler::getScheduler()->pauseTarget(this);
    m_pActionManager->pauseTarget(this);
}

void CAView::setActionManager(CCActionManager* actionManager)
{
    if( actionManager != m_pActionManager ) {
        this->stopAllActions();
        CC_SAFE_RETAIN(actionManager);
        CC_SAFE_RELEASE(m_pActionManager);
        m_pActionManager = actionManager;
    }
}

CCActionManager* CAView::getActionManager()
{
    return m_pActionManager;
}

CCAction * CAView::runAction(CCAction* action)
{
    CCAssert( action != NULL, "Argument must be non-nil");
    m_pActionManager->addAction(action, this, !m_bRunning);
    return action;
}

void CAView::stopAllActions()
{
    m_pActionManager->removeAllActionsFromTarget(this);
}

void CAView::stopAction(CCAction* action)
{
    m_pActionManager->removeAction(action);
}

void CAView::stopActionByTag(int tag)
{
    CCAssert( tag != kCCActionTagInvalid, "Invalid tag");
    m_pActionManager->removeActionByTag(tag, this);
}

CCAction * CAView::getActionByTag(int tag)
{
    CCAssert( tag != kCCActionTagInvalid, "Invalid tag");
    return m_pActionManager->getActionByTag(tag, this);
}

unsigned int CAView::numberOfRunningActions()
{
    return m_pActionManager->numberOfRunningActionsInTarget(this);
}



// override me
void CAView::update(float fDelta)
{
    if (m_pComponentContainer && !m_pComponentContainer->isEmpty())
    {
        m_pComponentContainer->visit(fDelta);
    }
}

CCAffineTransform CAView::nodeToParentTransform(void)
{
    if (m_bTransformDirty)
    {
        
        // Translate values
        float height = 0;
        
        if (this->getSuperview())
        {
            height= this->getSuperview()->getBounds().size.height;
        }
        else
        {
            height= CCDirector::sharedDirector()->getWinSize().height;
        }
        
        
        float x = m_obPoint.x;
        float y = height - m_obPoint.y;
        
        // Rotation values
		// Change rotation code to handle X and Y
		// If we skew with the exact same value for both x and y then we're simply just rotating
        float cx = 1, sx = 0, cy = 1, sy = 0;
        if (m_fRotationX || m_fRotationY)
        {
            float radiansX = -CC_DEGREES_TO_RADIANS(m_fRotationX);
            float radiansY = -CC_DEGREES_TO_RADIANS(m_fRotationY);
            cx = cosf(radiansX);
            sx = sinf(radiansX);
            cy = cosf(radiansY);
            sy = sinf(radiansY);
        }
        
        bool needsSkewMatrix = ( m_fSkewX || m_fSkewY );
        
        
        // optimization:
        // inline anchor point calculation if skew is not needed
        // Adjusted transform calculation for rotational skew
        CCPoint anchorPointInPoints = CCPoint(m_obAnchorPointInPoints.x,
                                              m_obContentSize.height - m_obAnchorPointInPoints.y);
        
        if (! needsSkewMatrix && !anchorPointInPoints.equals(CCPointZero))
        {
            x += cy * -anchorPointInPoints.x * m_fScaleX + -sx * -anchorPointInPoints.y * m_fScaleY;
            y += sy * -anchorPointInPoints.x * m_fScaleX +  cx * -anchorPointInPoints.y * m_fScaleY;
        }
        
        
        // Build Transform Matrix
        // Adjusted transform calculation for rotational skew
        m_sTransform = CCAffineTransformMake( cy * m_fScaleX,  sy * m_fScaleX,
                                             -sx * m_fScaleY, cx * m_fScaleY,
                                             x, y );
        
        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needsSkewMatrix)
        {
            CCAffineTransform skewMatrix = CCAffineTransformMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(m_fSkewY)),
                                                                 tanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1.0f,
                                                                 0.0f, 0.0f );
            m_sTransform = CCAffineTransformConcat(skewMatrix, m_sTransform);
            
            // adjust anchor point
            CCPoint anchorPointInPoints = CCPoint(m_obAnchorPointInPoints.x,
                                                  m_obContentSize.height - m_obAnchorPointInPoints.y);
            
            if (!anchorPointInPoints.equals(CCPointZero))
            {
                m_sTransform = CCAffineTransformTranslate(m_sTransform, -anchorPointInPoints.x, -anchorPointInPoints.y);
            }
        }
        
        if (m_bAdditionalTransformDirty)
        {
            m_sTransform = CCAffineTransformConcat(m_sTransform, m_sAdditionalTransform);
            m_bAdditionalTransformDirty = false;
        }
        
        m_bTransformDirty = false;
    }
    
    return m_sTransform;
}

void CAView::setAdditionalTransform(const CCAffineTransform& additionalTransform)
{
    m_sAdditionalTransform = additionalTransform;
    m_bTransformDirty = true;
    m_bAdditionalTransformDirty = true;
}

CCAffineTransform CAView::parentToNodeTransform(void)
{
    if ( m_bInverseDirty ) {
        m_sInverse = CCAffineTransformInvert(this->nodeToParentTransform());
        m_bInverseDirty = false;
    }
    
    return m_sInverse;
}

CCAffineTransform CAView::nodeToWorldTransform()
{
    CCAffineTransform t = this->nodeToParentTransform();
    
    for (CAView *p = m_pSuperview; p != NULL; p = p->getSuperview())
        t = CCAffineTransformConcat(t, p->nodeToParentTransform());
    
    return t;
}

CCAffineTransform CAView::worldToNodeTransform(void)
{
    return CCAffineTransformInvert(this->nodeToWorldTransform());
}

CCRect CAView::convertRectToNodeSpace(const cocos2d::CCRect &worldRect)
{
    CCRect ret = worldRect;
    ret.origin.y += ret.size.height;
    ret.origin = CCDirector::sharedDirector()->convertToGL(ret.origin);
    ret.origin = this->convertToNodeSpace(ret.origin);
    ret.origin.y = this->getBounds().size.height - ret.size.height - ret.origin.y;
    
    CAView*  view = this;
    while (view)
    {
        ret.size = CCSize(ret.size.width / view->getScaleX(), ret.size.height / view->getScaleY());
        view = view->getSuperview();
    }
    return ret;
}

CCRect CAView::convertRectToWorldSpace(const cocos2d::CCRect &nodeRect)
{
    CCRect ret = nodeRect;
    ret.origin.y = this->getBounds().size.height - ret.size.height - ret.origin.y;
    ret.origin = this->convertToWorldSpace(ret.origin);
    ret.origin = CCDirector::sharedDirector()->convertToUI(ret.origin);
    ret.origin.y -= ret.size.height;
    CAView*  view = this;
    while (view)
    {
        ret.size = CCSize(ret.size.width * view->getScaleX(), ret.size.height * view->getScaleY());
        view = view->getSuperview();
    }
    return ret;
}

CCPoint CAView::convertToNodeSpace(const CCPoint& worldPoint)
{
    CCPoint p = CCDirector::sharedDirector()->convertToGL(worldPoint);
    CCPoint ret = CCPointApplyAffineTransform(p, worldToNodeTransform());
    ret.y = this->getBounds().size.height - ret.y;
    return ret;
}

CCPoint CAView::convertToWorldSpace(const CCPoint& nodePoint)
{
    CCPoint p = nodePoint;
    p.y = this->getBounds().size.height - p.y;
    CCPoint ret = CCPointApplyAffineTransform(p, nodeToWorldTransform());
    ret = CCDirector::sharedDirector()->convertToUI(ret);
    return ret;
}

CCPoint CAView::convertToNodeSpaceAR(const CCPoint& worldPoint)
{
    CCPoint nodePoint = convertToNodeSpace(worldPoint);
    return ccpSub(nodePoint, m_obAnchorPointInPoints);
}

CCPoint CAView::convertToWorldSpaceAR(const CCPoint& nodePoint)
{
    CCPoint pt = ccpAdd(nodePoint, m_obAnchorPointInPoints);
    return convertToWorldSpace(pt);
}

// convenience methods which take a CCTouch instead of CCPoint
CCPoint CAView::convertTouchToNodeSpace(CCTouch *touch)
{
    CCPoint point = touch->getLocation();
    return this->convertToNodeSpace(point);
}
CCPoint CAView::convertTouchToNodeSpaceAR(CCTouch *touch)
{
    CCPoint point = touch->getLocation();
    return this->convertToNodeSpaceAR(point);
}

void CAView::updateTransform()
{
    // Recursively iterate over children
    
    if(m_pobImage && isDirty() ) {
        
        // If it is not visible, or one of its ancestors is not visible, then do nothing:
        if( !m_bVisible || ( m_pSuperview && m_pSuperview != m_pobBatchView && m_pSuperview->m_bShouldBeHidden) )
        {
            m_sQuad.br.vertices = m_sQuad.tl.vertices = m_sQuad.tr.vertices = m_sQuad.bl.vertices = vertex3(0,0,0);
            m_bShouldBeHidden = true;
        }
        else
        {
            m_bShouldBeHidden = false;
            
            if( ! m_pSuperview || m_pSuperview == m_pobBatchView)
            {
                m_transformToBatch = nodeToParentTransform();
            }
            else
            {
                CCAssert( dynamic_cast<CAImageView*>(m_pSuperview), "Logic error in CAImageView. Parent must be a CAImageView");
                m_transformToBatch = CCAffineTransformConcat( nodeToParentTransform() , ((CAImageView*)m_pSuperview)->m_transformToBatch );
            }
            
            //
            // calculate the Quad based on the Affine Matrix
            //
            
            CCSize size = m_obRect.size;
            
            float x1 = 0;
            float y1 = 0;
            
            float x2 = x1 + size.width;
            float y2 = y1 + size.height;
            float x = m_transformToBatch.tx;
            float y = m_transformToBatch.ty;
            
            float cr = m_transformToBatch.a;
            float sr = m_transformToBatch.b;
            float cr2 = m_transformToBatch.d;
            float sr2 = -m_transformToBatch.c;
            float ax = x1 * cr - y1 * sr2 + x;
            float ay = x1 * sr + y1 * cr2 + y;
            
            float bx = x2 * cr - y1 * sr2 + x;
            float by = x2 * sr + y1 * cr2 + y;
            
            float cx = x2 * cr - y2 * sr2 + x;
            float cy = x2 * sr + y2 * cr2 + y;
            
            float dx = x1 * cr - y2 * sr2 + x;
            float dy = x1 * sr + y2 * cr2 + y;
            
            m_sQuad.bl.vertices = vertex3( RENDER_IN_SUBPIXEL(ax), RENDER_IN_SUBPIXEL(ay), m_fVertexZ );
            m_sQuad.br.vertices = vertex3( RENDER_IN_SUBPIXEL(bx), RENDER_IN_SUBPIXEL(by), m_fVertexZ );
            m_sQuad.tl.vertices = vertex3( RENDER_IN_SUBPIXEL(dx), RENDER_IN_SUBPIXEL(dy), m_fVertexZ );
            m_sQuad.tr.vertices = vertex3( RENDER_IN_SUBPIXEL(cx), RENDER_IN_SUBPIXEL(cy), m_fVertexZ );
        }
        
        // MARMALADE CHANGE: ADDED CHECK FOR NULL, TO PERMIT SPRITES WITH NO BATCH NODE / Image ATLAS
        if (m_pobImageAtlas)
		{
            m_pobImageAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
        }
        
        m_bRecursiveDirty = false;
        setDirty(false);
    }
    
    // MARMALADE CHANGED
    // recursively iterate over children
    /*    if( m_bHasChildren )
     {
     // MARMALADE: CHANGED TO USE CCNode*
     // NOTE THAT WE HAVE ALSO DEFINED virtual CAView::updateTransform()
     arrayMakeObjectsPerformSelector(m_pSubviews, updateTransform, CAImageView*);
     }*/
    arrayMakeObjectsPerformSelector(m_pSubviews, updateTransform, CAView*);
    
#if CC_SPRITE_DEBUG_DRAW
    // draw bounding box
    CCPoint vertices[4] = {
        ccp( m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y ),draw
        ccp( m_sQuad.br.vertices.x, m_sQuad.br.vertices.y ),
        ccp( m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y ),
        ccp( m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y ),
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW

}

CCComponent* CAView::getComponent(const char *pName) const
{
    return m_pComponentContainer->get(pName);
}

bool CAView::addComponent(CCComponent *pComponent)
{
    return m_pComponentContainer->add(pComponent);
}

bool CAView::removeComponent(const char *pName)
{
    return m_pComponentContainer->remove(pName);
}

void CAView::removeAllComponents()
{
    m_pComponentContainer->removeAll();
}

bool CAView::isDisplayRange()
{
    return m_bDisplayRange;
}

void CAView::setDisplayRange(bool value)
{
    m_bDisplayRange = value;
}

//Image...

void CAView::setImage(CAImage* image)
{
    if (!m_pobBatchView)
    {
        CC_SAFE_RETAIN(image);
        CC_SAFE_RELEASE(m_pobImage);
        m_pobImage = image;
        updateBlendFunc();
    }
}

CAImage* CAView::getImage(void)
{
    return m_pobImage;
}

void CAView::setImageRect(const CCRect& rect)
{
    setImageRect(rect, false, m_obContentSize);
}

void CAView::setImageRect(const CCRect& rect, bool rotated, const CCSize& untrimmedSize)
{
    m_bRectRotated = rotated;
    
    setVertexRect(rect);
    setImageCoords(rect);
    
    if (!m_obContentSize.equals(untrimmedSize))
    {
        setContentSize(untrimmedSize);
    }
    
    CCPoint relativeOffset = m_obUnflippedOffsetPositionFromCenter;
    
    // issue #732
    if (m_bFlipX)
    {
        relativeOffset.x = -relativeOffset.x;
    }
    if (m_bFlipY)
    {
        relativeOffset.y = -relativeOffset.y;
    }

    if (m_pobBatchView)
    {
        // update dirty_, don't update recursiveDirty_
        setDirty(true);
    }
    else
    {
        this->updateImageRect();
    }
}

void CAView::updateImageRect()
{
    // Don't update Z.
    m_sQuad.bl.vertices = vertex3(0, 0, 0);
    m_sQuad.br.vertices = vertex3(m_obContentSize.width, 0, 0);
    m_sQuad.tl.vertices = vertex3(0, m_obContentSize.height, 0);
    m_sQuad.tr.vertices = vertex3(m_obContentSize.width, m_obContentSize.height, 0);
}

// override this method to generate "double scale" sprites
void CAView::setVertexRect(const CCRect& rect)
{
    m_obRect = rect;
}

void CAView::setImageCoords(CCRect rect)
{
    rect = CC_RECT_POINTS_TO_PIXELS(rect);
    
    CAImage* tex = m_pobBatchView ? m_pobImageAtlas->getImage() : m_pobImage;
    if (! tex)
    {
        return;
    }
    
    float atlasWidth = (float)tex->getPixelsWide();
    float atlasHeight = (float)tex->getPixelsHigh();
    
    float left, right, top, bottom;
    
    if (m_bRectRotated)
    {
#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        left    = (2*rect.origin.x+1)/(2*atlasWidth);
        right    = left+(rect.size.height*2-2)/(2*atlasWidth);
        top        = (2*rect.origin.y+1)/(2*atlasHeight);
        bottom    = top+(rect.size.width*2-2)/(2*atlasHeight);
#else
        left    = rect.origin.x/atlasWidth;
        right    = (rect.origin.x+rect.size.height) / atlasWidth;
        top        = rect.origin.y/atlasHeight;
        bottom    = (rect.origin.y+rect.size.width) / atlasHeight;
#endif // CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        
        if (m_bFlipX)
        {
            CC_SWAP(top, bottom, float);
        }
        
        if (m_bFlipY)
        {
            CC_SWAP(left, right, float);
        }
        
        m_sQuad.bl.texCoords.u = left;
        m_sQuad.bl.texCoords.v = top;
        m_sQuad.br.texCoords.u = left;
        m_sQuad.br.texCoords.v = bottom;
        m_sQuad.tl.texCoords.u = right;
        m_sQuad.tl.texCoords.v = top;
        m_sQuad.tr.texCoords.u = right;
        m_sQuad.tr.texCoords.v = bottom;
    }
    else
    {
#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        left    = (2*rect.origin.x+1)/(2*atlasWidth);
        right    = left + (rect.size.width*2-2)/(2*atlasWidth);
        top        = (2*rect.origin.y+1)/(2*atlasHeight);
        bottom    = top + (rect.size.height*2-2)/(2*atlasHeight);
#else
        left    = rect.origin.x/atlasWidth;
        right    = (rect.origin.x + rect.size.width) / atlasWidth;
        top        = rect.origin.y/atlasHeight;
        bottom    = (rect.origin.y + rect.size.height) / atlasHeight;
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        
        if(m_bFlipX)
        {
            CC_SWAP(left,right,float);
        }
        
        if(m_bFlipY)
        {
            CC_SWAP(top,bottom,float);
        }
        
        m_sQuad.bl.texCoords.u = left;
        m_sQuad.bl.texCoords.v = bottom;
        m_sQuad.br.texCoords.u = right;
        m_sQuad.br.texCoords.v = bottom;
        m_sQuad.tl.texCoords.u = left;
        m_sQuad.tl.texCoords.v = top;
        m_sQuad.tr.texCoords.u = right;
        m_sQuad.tr.texCoords.v = top;
    }
}

float CAView::getAlpha(void)
{
	return _realAlpha;
}

float CAView::getDisplayedAlpha(void)
{
	return _displayedAlpha;
}

void CAView::setAlpha(float alpha)
{
    alpha = MIN(alpha, 1.0f);
    alpha = MAX(alpha, 0.0f);
    
    _displayedAlpha = _realAlpha = alpha;
    
    float superviewAlpha = m_pSuperview ? m_pSuperview->getDisplayedAlpha() : 1.0f;
    
    this->updateDisplayedAlpha(superviewAlpha);
    
    this->updateColor();
}

void CAView::updateDisplayedAlpha(float parentAlpha)
{
	_displayedAlpha = _realAlpha * parentAlpha;
	
    CCObject* pObj;
    CCARRAY_FOREACH(m_pSubviews, pObj)
    {
        CAView* item = dynamic_cast<CAView*>(pObj);
        if (item)
        {
            item->updateDisplayedAlpha(_displayedAlpha);
        }
    }
    
    this->updateColor();
}

const CAColor4B& CAView::getColor(void)
{
	return _realColor;
}

const CAColor4B& CAView::getDisplayedColor()
{
	return _displayedColor;
}

void CAView::setColor(const CAColor4B& color)
{
	_displayedColor = _realColor = color;
	
	this->updateColor();
}

void CAView::updateDisplayedColor(const CAColor4B& parentColor)
{
	_displayedColor.r = _realColor.r * parentColor.r/255.0;
	_displayedColor.g = _realColor.g * parentColor.g/255.0;
	_displayedColor.b = _realColor.b * parentColor.b/255.0;
    _displayedColor.a = _realColor.a * parentColor.a/255.0;
    
    updateColor();
}

void CAView::updateColor(void)
{
    if (m_pobImage == NULL)
    {
        return;
    }
    
    CAColor4B color4 = _displayedColor;
    
    color4.r *= _displayedAlpha;
    color4.g *= _displayedAlpha;
    color4.b *= _displayedAlpha;
    color4.a *= _displayedAlpha;
    
    m_sQuad.bl.colors = color4;
    m_sQuad.br.colors = color4;
    m_sQuad.tl.colors = color4;
    m_sQuad.tr.colors = color4;
    
    if (m_pobBatchView)
    {
        if (m_uAtlasIndex != 0xffffffff)
        {
            m_pobImageAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
        }
        else
        {
            // no need to set it recursively
            // update dirty_, don't update recursiveDirty_
            setDirty(true);
        }
    }
    
    this->updateDraw();
    // self render
    // do nothing
}

void CAView::updateBlendFunc(void)
{
    // it is possible to have an untextured sprite
    if (! m_pobImage || ! m_pobImage->hasPremultipliedAlpha())
    {
        m_sBlendFunc.src = GL_SRC_ALPHA;
        m_sBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    }
    else
    {
        m_sBlendFunc.src = CC_BLEND_SRC;
        m_sBlendFunc.dst = CC_BLEND_DST;
    }
}

void CAView::setReorderChildDirtyRecursively(void)
{
    //only set parents flag the first time
    if ( ! m_bReorderChildDirty )
    {
        m_bReorderChildDirty = true;
        CAView* pNode = (CAView*)m_pSuperview;
        while (pNode && pNode != m_pobBatchView)
        {
            pNode->setReorderChildDirtyRecursively();
            pNode=pNode->getSuperview();
        }
    }
}


void CAView::setDirtyRecursively(bool bValue)
{
    m_bRecursiveDirty = bValue;
    setDirty(bValue);
    // recursively set dirty
    if (m_bHasChildren)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pSubviews, pObject)
        {
            CAImageView* pChild = dynamic_cast<CAImageView*>(pObject);
            if (pChild)
            {
                pChild->setDirtyRecursively(true);
            }
        }
    }
}

void CAView::setFlipX(bool bFlipX)
{
    if (m_bFlipX != bFlipX)
    {
        m_bFlipX = bFlipX;
        if (m_pobImage)
        {
            setImageRect(m_obRect, m_bRectRotated, m_obContentSize);
        }
    }
}

bool CAView::isFlipX(void)
{
    return m_bFlipX;
}

void CAView::setFlipY(bool bFlipY)
{
    if (m_bFlipY != bFlipY)
    {
        m_bFlipY = bFlipY;
        if (m_pobImage)
        {
            setImageRect(m_obRect, m_bRectRotated, m_obContentSize);
        }
    }
}

bool CAView::isFlipY(void)
{
    return m_bFlipY;
}

bool CAView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pViewDelegate)
    {
        return m_pViewDelegate->ccTouchBegan(pTouch, pEvent);
    }
    
    return false;
}

void CAView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pViewDelegate)
    {
        m_pViewDelegate->ccTouchMoved(pTouch, pEvent);
    }
}

void CAView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pViewDelegate)
    {
        m_pViewDelegate->ccTouchEnded(pTouch, pEvent);
    }
}

void CAView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pViewDelegate)
    {
        m_pViewDelegate->ccTouchCancelled(pTouch, pEvent);
    }
}

void CAView::setBatch(CABatchView *batchView)
{
    m_pobBatchView = batchView; // weak reference
    
    // self render
    if( ! m_pobBatchView ) {
        m_uAtlasIndex = 0xffffffff;
        setImageAtlas(NULL);
        m_bRecursiveDirty = false;
        setDirty(false);
        
        float x1 = 0;
        float y1 = 0;
        float x2 = x1 + m_obRect.size.width;
        float y2 = y1 + m_obRect.size.height;
        m_sQuad.bl.vertices = vertex3( x1, y1, 0 );
        m_sQuad.br.vertices = vertex3( x2, y1, 0 );
        m_sQuad.tl.vertices = vertex3( x1, y2, 0 );
        m_sQuad.tr.vertices = vertex3( x2, y2, 0 );
        
    } else {
        
        // using batch
        m_transformToBatch = CCAffineTransformIdentity;
        setImageAtlas(m_pobBatchView->getImageAtlas()); // weak ref
    }
}

NS_CC_END;
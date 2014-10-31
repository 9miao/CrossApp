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
#include "basics/CACamera.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "dispatcher/CATouch.h"
#include "actions/CCActionManager.h"
#include "shaders/CAGLProgram.h"
#include "CABatchView.h"
#include "kazmath/GL/matrix.h"
#include <stdarg.h>
#include "dispatcher/CATouchDispatcher.h"
#include "dispatcher/CAKeypadDispatcher.h"
#include "CCAccelerometer.h"
#include "shaders/CAShaderCache.h"
#include "shaders/CAGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "CCEGLView.h"
#include "cocoa/CCSet.h"
#include "CAImageView.h"
#include "actions/CCActionInterval.h"

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
, m_sAdditionalTransform(CATransformationMakeIdentity())
, m_pCamera(NULL)
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
, m_bRestoreScissor(false)
, m_obRestoreScissorRect(CCRectZero)
, m_pobBatchView(NULL)
, m_pobImageAtlas(NULL)
{
    m_pActionManager = CAApplication::getApplication()->getActionManager();
    m_pActionManager->retain();
    
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;
    memset(&m_sQuad, 0, sizeof(m_sQuad));
    
    CAColor4B tmpColor = CAColor_white;
    m_sQuad.bl.colors = tmpColor;
    m_sQuad.br.colors = tmpColor;
    m_sQuad.tl.colors = tmpColor;
    m_sQuad.tr.colors = tmpColor;
    
    this->setAnchorPoint(CCPoint(0.5f, 0.5f));
    this->setHaveNextResponder(true);
    
    ++viewCount;
    //CCLog("CAView = %d\n",viewCount);
}

CAView::~CAView(void)
{
    CAScheduler::getScheduler()->pauseTarget(this);
    m_pActionManager->pauseTarget(this);
    
    CC_SAFE_RELEASE(m_pActionManager);
    CC_SAFE_RELEASE(m_pCamera);
    CC_SAFE_RELEASE(m_pShaderProgram);
    CC_SAFE_RELEASE(m_pUserObject);
    
    if(m_pSubviews && m_pSubviews->count() > 0)
    {
        CAObject* subview;
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
    CC_SAFE_RELEASE(m_pobImage);
    
    --viewCount;
    //CCLog("~CAView = %d\n",viewCount);
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
    // shader program
    this->setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureA8Color));
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
    if (!this->init())
    {
        return false;
    }
    this->setColor(color4B);
    
    return true;
}

bool CAView::initWithFrame(const CCRect& rect)
{
    if (!this->init())
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
    if (!this->init())
    {
        return false;
    }
    this->setCenter(rect);
    
    return true;
}

bool CAView::initWithCenter(const CCRect& rect, const CAColor4B& color4B)
{
    if (!this->initWithColor(color4B))
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
CACamera* CAView::getCamera()
{
    if (!m_pCamera)
    {
        m_pCamera = new CACamera();
    }
    
    return m_pCamera;
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
    rect.setCenter(true);
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
void CAView::setSuperview(CrossApp::CAView * superview)
{
    m_pSuperview = superview;
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

CAGLProgram* CAView::getShaderProgram()
{
    return m_pShaderProgram;
}

CAObject* CAView::getUserObject()
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

void CAView::setUserObject(CAObject *pUserObject)
{
    CC_SAFE_RETAIN(pUserObject);
    CC_SAFE_RELEASE(m_pUserObject);
    m_pUserObject = pUserObject;
}

void CAView::setShaderProgram(CAGLProgram *pShaderProgram)
{
    CC_SAFE_RETAIN(pShaderProgram);
    CC_SAFE_RELEASE(m_pShaderProgram);
    m_pShaderProgram = pShaderProgram;
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
        CAApplication::getApplication()->updateDraw();
    }
}

// lazy allocs
void CAView::childrenAlloc(void)
{
    m_pSubviews = new CCArray(4);
}

CAView* CAView::getSubviewByTag(int aTag)
{
    CCAssert( aTag != kCAObjectTagInvalid, "Invalid tag");
    
    if(m_pSubviews && m_pSubviews->count() > 0)
    {
        CAObject* subview;
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
    CCAssert( tag != kCAObjectTagInvalid, "Invalid tag");
    
    CAView *subview = this->getSubviewByTag(tag);
    
    if (subview == NULL)
    {
        CCLOG("CrossApp: removeChildByTag(tag = %d): child not found!", tag);
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
            CAObject* obj = NULL;
            CCARRAY_FOREACH(m_pSubviews, obj)
            {
                CAView* subview = dynamic_cast<CAView*>(obj);
                if (subview)
                {
                    m_pobBatchView->removeSpriteFromAtlas(subview);
                }
            }
        }
        
        CAObject* obj;
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
    
    // set parent nil at the end
    subview->setSuperview(NULL);
    
    m_pSubviews->removeObject(subview);
    
    this->updateDraw();
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
        
        for(i=1; i<length; i++)
        {
            tempItem = x[i];
            j = i-1;
            CC_CONTINUE_IF(tempItem == NULL);
            //continue moving element downwards while zOrder is smaller or when zOrder is the same but mutatedIndex is smaller
            while(j>=0 && ( tempItem->m_nZOrder < x[j]->m_nZOrder || ( tempItem->m_nZOrder== x[j]->m_nZOrder && tempItem->m_uOrderOfArrival < x[j]->m_uOrderOfArrival ) ) )
            {
                x[j+1] = x[j];
                j = j-1;
            }
            x[j+1] = tempItem;
        }
        
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

    CC_RETURN_IF(m_pobImage == NULL);
    CC_RETURN_IF(m_pShaderProgram == NULL);
    
    CC_NODE_DRAW_SETUP();
    
    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    
    ccGLBindTexture2D(m_pobImage->getName());
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    
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
    CC_RETURN_IF(!m_bVisible);
    
    kmGLPushMatrix();

    this->transform();
    
    if (!m_bDisplayRange)
    {
        // check parent scissor
        if (glIsEnabled(GL_SCISSOR_TEST)) {
            GLfloat params[4];
            glGetFloatv(GL_SCISSOR_BOX, params);
            m_bRestoreScissor = true;
            m_obRestoreScissorRect = CCRectMake(params[0] - 1.0f, params[1], params[2] + 1.0f, params[3] + 1.0f);
        }
        
        CCPoint point = CCPointZero;
        
        CCSize size = this->getBounds().size;
        
        {
            int rotation = this->getRotation();
            
            CAView* parent = this->getSuperview();
            while (parent)
            {
                rotation += parent->getRotation();
                parent = parent->getSuperview();
            }
            
            if (fabsf(rotation % 360 - 90) < FLT_EPSILON)
            {
                point = this->getBounds().size;
                size.width = size.width + size.height;
                size.height = size.width - size.height;
                size.width = size.width - size.height;
            }
            else if (fabsf(rotation % 360 - 180) < FLT_EPSILON)
            {
                point = CCPoint(this->getBounds().size.width, 0);
            }
            else if (fabsf(rotation % 360 - 270) < FLT_EPSILON)
            {
                point = CCPointZero;
                size.width = size.width + size.height;
                size.height = size.width - size.height;
                size.width = size.width - size.height;
            }
            else
            {
                point = CCPoint(0, this->getBounds().size.height);
            }
        }
        point = this->convertToWorldSpace(point);
        point = CAApplication::getApplication()->convertToGL(point);
        
        
        CCEGLView* pGLView = CCEGLView::sharedOpenGLView();
        float glScaleX = pGLView->getScaleX();
        float glScaleY = pGLView->getScaleY();
        
        float off_X = pGLView->getViewPortRect().origin.x;
        float off_Y = pGLView->getViewPortRect().origin.y;
        
        float scaleX = this->getScaleX();
        float scaleY = this->getScaleY();
        
        CAView* parent = this->getSuperview();
        while (parent)
        {
            scaleX *= parent->getScaleX();
            scaleY *= parent->getScaleY();
            parent = parent->getSuperview();
        }
        
        CCRect frame = CCRectMake((point.x - 0) * glScaleX + off_X,
                                  (point.y - 0) * glScaleY + off_Y,
                                  (size.width + 0) * scaleX * glScaleX,
                                  (size.height + 0) * scaleY * glScaleY);
        if (m_bRestoreScissor)
        {
            if (frame.intersectsRect(m_obRestoreScissorRect))
            {
                float x = MAX(frame.origin.x, m_obRestoreScissorRect.origin.x);
                float y = MAX(frame.origin.y, m_obRestoreScissorRect.origin.y);
                float xx = MIN(frame.origin.x+frame.size.width, m_obRestoreScissorRect.origin.x+m_obRestoreScissorRect.size.width);
                float yy = MIN(frame.origin.y+frame.size.height, m_obRestoreScissorRect.origin.y+m_obRestoreScissorRect.size.height);
                glScissor(x, y, xx-x, yy-y);
            }
        }
        else
        {
            glEnable(GL_SCISSOR_TEST);
            glScissor(frame.origin.x, frame.origin.y, frame.size.width + 1.0f, frame.size.height);
        }
    }

    CCRect winRect = CCRectZero;
    winRect.size = CAApplication::getApplication()->getWinSize();
    
    CAView* pNode = NULL;
    unsigned int i = 0;
    if(m_pSubviews && m_pSubviews->count() > 0)
    {
        this->sortAllSubviews();
        // draw children zOrder < 0
        ccArray *arrayData = m_pSubviews->data;

        for( ; i < arrayData->num; i++ )
        {
            pNode = (CAView*) arrayData->arr[i];

            if ( pNode && pNode->m_nZOrder < 0 )
            {
                pNode->visit();
            }
            else
            {
                break;
            }
        }

        this->draw();

        for( ; i < arrayData->num; i++ )
        {
            pNode = (CAView*) arrayData->arr[i];
            if (pNode && pNode->m_nZOrder >= 0)
            {
                pNode->visit();
            }
        }
    }
    else
    {
        this->draw();
    }
    
    m_uOrderOfArrival = 0;
    
    if (!m_bDisplayRange)
    {
        if (m_bRestoreScissor) {
            glScissor(m_obRestoreScissorRect.origin.x, 
                      m_obRestoreScissorRect.origin.y, 
                      m_obRestoreScissorRect.size.width, 
                      m_obRestoreScissorRect.size.height);
        } else {
            glDisable(GL_SCISSOR_TEST);
        }
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
    CATransformation tmpAffine = this->nodeToParentTransform();
    CGAffineToGL(&tmpAffine, transfrom4x4.mat);
    
    // Update Z vertex manually
    transfrom4x4.mat[14] = m_fVertexZ;
    
    kmGLMultMatrix( &transfrom4x4 );
    
    
    // XXX: Expensive calls. Camera should be integrated into the cached affine matrix
    if ( m_pCamera != NULL)
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

CAView* CAView::copy()
{
    CAView* view = NULL;
    if (m_bFrame)
        
    {
        view = CAView::createWithFrame(this->getFrame(), this->getColor());
    }
    else
    {
        view = CAView::createWithCenter(this->getCenter(), this->getColor());
    }
    view->setImage(this->getImage());
    
    return view;
}

CAResponder* CAView::nextResponder()
{
    if (m_bHaveNextResponder == false)
    {
        return NULL;
    }
    
    if (m_pViewDelegate)
    {
        return dynamic_cast<CAResponder*>(m_pViewDelegate);
    }
    return this->getSuperview();
}

void CAView::onEnter()
{
    arrayMakeObjectsPerformSelector(m_pSubviews, onEnter, CAView*);

    m_bRunning = true;
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
    action = CCSequence::createWithTwoActions(CCDelayTime::create(1/60.0f), (CCFiniteTimeAction*)action);
    m_pActionManager->addAction(action, this, false);
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

}

CATransformation CAView::nodeToParentTransform(void)
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
            height= CAApplication::getApplication()->getWinSize().height;
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
        m_sTransform = CATransformationMake( cy * m_fScaleX,  sy * m_fScaleX,
                                             -sx * m_fScaleY, cx * m_fScaleY,
                                             x, y );
        
        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needsSkewMatrix)
        {
            CATransformation skewMatrix = CATransformationMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(m_fSkewY)),
                                                                 tanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1.0f,
                                                                 0.0f, 0.0f );
            m_sTransform = CATransformationConcat(skewMatrix, m_sTransform);
            
            // adjust anchor point
            CCPoint anchorPointInPoints = CCPoint(m_obAnchorPointInPoints.x,
                                                  m_obContentSize.height - m_obAnchorPointInPoints.y);
            
            if (!anchorPointInPoints.equals(CCPointZero))
            {
                m_sTransform = CATransformationTranslate(m_sTransform, -anchorPointInPoints.x, -anchorPointInPoints.y);
            }
        }
        
        if (m_bAdditionalTransformDirty)
        {
            m_sTransform = CATransformationConcat(m_sTransform, m_sAdditionalTransform);
            m_bAdditionalTransformDirty = false;
        }
        
        m_bTransformDirty = false;
    }
    
    return m_sTransform;
}

void CAView::setAdditionalTransform(const CATransformation& additionalTransform)
{
    m_sAdditionalTransform = additionalTransform;
    m_bTransformDirty = true;
    m_bAdditionalTransformDirty = true;
}

CATransformation CAView::parentToNodeTransform(void)
{
    if ( m_bInverseDirty ) {
        m_sInverse = CATransformationInvert(this->nodeToParentTransform());
        m_bInverseDirty = false;
    }
    
    return m_sInverse;
}

CATransformation CAView::nodeToWorldTransform()
{
    CATransformation t = this->nodeToParentTransform();
    
    for (CAView *p = m_pSuperview; p != NULL; p = p->getSuperview())
        t = CATransformationConcat(t, p->nodeToParentTransform());
    
    return t;
}

CATransformation CAView::worldToNodeTransform(void)
{
    return CATransformationInvert(this->nodeToWorldTransform());
}

CCRect CAView::convertRectToNodeSpace(const CrossApp::CCRect &worldRect)
{
    CCRect ret = worldRect;
    ret.origin.y += ret.size.height;
    ret.origin = CAApplication::getApplication()->convertToGL(ret.origin);
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

CCRect CAView::convertRectToWorldSpace(const CrossApp::CCRect &nodeRect)
{
    CCRect ret = nodeRect;
    ret.origin.y = this->getBounds().size.height - ret.size.height - ret.origin.y;
    ret.origin = this->convertToWorldSpace(ret.origin);
    ret.origin = CAApplication::getApplication()->convertToUI(ret.origin);
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
    CCPoint p = CAApplication::getApplication()->convertToGL(worldPoint);
    CCPoint ret = CCPointApplyAffineTransform(p, worldToNodeTransform());
    ret.y = this->getBounds().size.height - ret.y;
    return ret;
}

CCPoint CAView::convertToWorldSpace(const CCPoint& nodePoint)
{
    CCPoint p = nodePoint;
    p.y = this->getBounds().size.height - p.y;
    CCPoint ret = CCPointApplyAffineTransform(p, nodeToWorldTransform());
    ret = CAApplication::getApplication()->convertToUI(ret);
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

// convenience methods which take a CATouch instead of CCPoint
CCPoint CAView::convertTouchToNodeSpace(CATouch *touch)
{
    CCPoint point = touch->getLocation();
    return this->convertToNodeSpace(point);
}
CCPoint CAView::convertTouchToNodeSpaceAR(CATouch *touch)
{
    CCPoint point = touch->getLocation();
    return this->convertToNodeSpaceAR(point);
}

void CAView::updateTransform()
{
    // Recursively iterate over children
    
    if(m_pobImage && isDirty() )
    {
        
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
                m_transformToBatch = CATransformationConcat( nodeToParentTransform() , m_pSuperview->m_transformToBatch );
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
        this->updateDraw();
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
        CCRect rect;
        if (m_bFrame)
        {
            rect = this->getFrame();
        }
        else
        {
            rect = this->getCenter();
        }
        rect.size = untrimmedSize;
        
        if (m_bFrame)
        {
            this->setFrame(rect);
        }
        else
        {
            this->setCenter(rect);
        }
        
        
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
	
    CAObject* pObj;
    CCARRAY_FOREACH(m_pSubviews, pObj)
    {
        CAView* item = (CAView*)pObj;
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
    CAColor4B color4 = _displayedColor;
    
    if (m_pobImage && m_pobBatchView)
    {
        color4.r *= _displayedAlpha;
        color4.g *= _displayedAlpha;
        color4.b *= _displayedAlpha;
        color4.a *= _displayedAlpha;
        
        m_sQuad.bl.colors = color4;
        m_sQuad.br.colors = color4;
        m_sQuad.tl.colors = color4;
        m_sQuad.tr.colors = color4;
        
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
    else
    {
        color4.a *= _displayedAlpha;
        
        m_sQuad.bl.colors = color4;
        m_sQuad.br.colors = color4;
        m_sQuad.tl.colors = color4;
        m_sQuad.tr.colors = color4;
    }
    
    this->updateDraw();
}

void CAView::updateBlendFunc(void)
{
    if (m_pobImage && ! m_pobImage->hasPremultipliedAlpha())
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
        CAObject* pObject = NULL;
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

bool CAView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    return false;
}

void CAView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{

}

void CAView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{

}

void CAView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{

}

void CAView::setBatch(CABatchView *batchView)
{
    m_pobBatchView = batchView; // weak reference
    
    // self render
    if( ! m_pobBatchView )
    {
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
        
    }
    else
    {
        
        // using batch
        m_transformToBatch = CATransformationIdentity;
        setImageAtlas(m_pobBatchView->getImageAtlas()); // weak ref
    }
}

NS_CC_END;
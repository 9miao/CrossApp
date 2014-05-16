//
//  CAView.cpp
//  cocos2dx
//
//  Created by 栗元峰 on 14-5-12.
//  Copyright (c) 2014年 厦门雅基软件有限公司. All rights reserved.
//

#include "CAView.h"
#include "cocoa/CCString.h"
#include "support/CCPointExtension.h"
#include "support/TransformUtils.h"
#include "CCCamera.h"
#include "effects/CCGrid.h"
#include "CCDirector.h"
#include "CCScheduler.h"
#include "touch_dispatcher/CCTouch.h"
#include "actions/CCActionManager.h"
#include "shaders/CCGLProgram.h"
// externals
#include "kazmath/GL/matrix.h"
#include "support/component/CCComponent.h"
#include "support/component/CCComponentContainer.h"

#include <stdarg.h>
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "keypad_dispatcher/CCKeypadDispatcher.h"
#include "CCAccelerometer.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "CCEGLView.h"

#if CC_NODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

NS_CC_BEGIN;

// XXX: Yes, nodes might have a sort problem once every 15 days if the game runs at 60 FPS and each frame sprites are reordered.
static int s_globalOrderOfArrival = 1;

static CCTexture2D* CC_2x2_WHITE_IMAGE(const CCSize& size)
{
    CCTexture2D* texture = NULL;
    
    if (1)
    {
        int pixels[1][1] = {0xffffffff};
        
        texture = new CCTexture2D();
        texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1, size);
        texture->setMonochrome(true);
    }
    return texture;
}

CAView::CAView(void)
: m_fRotationX(0.0f)
, m_fRotationY(0.0f)
, m_fScaleX(1.0f)
, m_fScaleY(1.0f)
, m_fVertexZ(0.0f)
, m_obPosition(CCPointZero)
, m_fSkewX(0.0f)
, m_fSkewY(0.0f)
, m_obAnchorPointInPoints(CCPointZero)
, m_obAnchorPoint(CCPointZero)
, m_obContentSize(CCSizeZero)
, m_obFrameRect(CCRectZero)
, m_sAdditionalTransform(CCAffineTransformMakeIdentity())
, m_pCamera(NULL)
, m_pGrid(NULL)
, m_nZOrder(0)
, m_pSubviews(NULL)
, m_pSuperview(NULL)
, m_nTag(kCAViewTagInvalid)
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
, m_bIgnoreAnchorPointForPosition(false)
, m_bReorderChildDirty(false)
, m_pComponentContainer(NULL)
, _displayedOpacity(255)
, _realOpacity(255)
, _displayedColor(ccWHITE)
, _realColor(ccWHITE)
, _cascadeColorEnabled(false)
, _cascadeOpacityEnabled(false)
, m_bTouchEnabled(false)
, m_bDisplayRange(true)
, m_nTouchPriority(0)
, m_pobTexture(NULL)
, m_bShouldBeHidden(false)
, m_bFlipX(false)
, m_bFlipY(false)
, m_bRecursiveDirty(false)
, m_bDirty(false)
, m_bOpacityModifyRGB(true)
, m_obOffsetPosition(CCPointZero)
, m_bHasChildren(false)
, m_pViewDelegate(NULL)
{
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;
    CCDirector *director = CCDirector::sharedDirector();
    m_pActionManager = director->getActionManager();
    m_pActionManager->retain();
    
    m_pComponentContainer = new CCComponentContainer(this);
}

CAView::~CAView(void)
{
    CCLOGINFO( "cocos2d: deallocing" );
    
    CC_SAFE_RELEASE(m_pActionManager);
    // attributes
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
    
    // children
    CC_SAFE_RELEASE(m_pSubviews);
    
    // m_pComsContainer
    m_pComponentContainer->removeAll();
    CC_SAFE_DELETE(m_pComponentContainer);
    
    CC_SAFE_RELEASE(m_pobTexture);
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
    _displayedOpacity = _realOpacity = 255;
    _displayedColor = _realColor = ccWHITE;
    _cascadeOpacityEnabled = _cascadeColorEnabled = false;
    
    this->setAnchorPoint(CCPoint(0.5f, 0.5f));

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

CAView* CAView::createWithFrame(const CCRect& rect, const ccColor4B& color4B)
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

bool CAView::initWithFrame(const CCRect& rect)
{
    return this->initWithFrame(rect, ccc4(255, 255, 255, 255));
}

bool CAView::initWithFrame(const CCRect& rect, const ccColor4B& color4B)
{
    if (!CAView::init())
    {
        return false;
    }
    
    this->setFrame(rect);

    // clean the Quad
    memset(&m_sQuad, 0, sizeof(m_sQuad));
    
    // Atlas: Color
    ccColor4B tmpColor = { 255, 255, 255, 255 };
    m_sQuad.bl.colors = tmpColor;
    m_sQuad.br.colors = tmpColor;
    m_sQuad.tl.colors = tmpColor;
    m_sQuad.tr.colors = tmpColor;
    
    // shader program
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    
    this->setTexture(CC_2x2_WHITE_IMAGE(rect.size));
    this->setTextureRect(rect, false, rect.size);
    this->setColor(ccc3(color4B.r, color4B.g, color4B.b));
    this->setOpacity(color4B.a);

    return true;
}

float CAView::getSkewX()
{
    return m_fSkewX;
}

void CAView::setSkewX(float newSkewX)
{
    m_fSkewX = newSkewX;
    m_bTransformDirty = m_bInverseDirty = true;
}

float CAView::getSkewY()
{
    return m_fSkewY;
}

void CAView::setSkewY(float newSkewY)
{
    m_fSkewY = newSkewY;
    
    m_bTransformDirty = m_bInverseDirty = true;
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
    m_fRotationX = m_fRotationY = newRotation;
    m_bTransformDirty = m_bInverseDirty = true;
}

float CAView::getRotationX()
{
    return m_fRotationX;
}

void CAView::setRotationX(float fRotationX)
{
    m_fRotationX = fRotationX;
    m_bTransformDirty = m_bInverseDirty = true;
}

float CAView::getRotationY()
{
    return m_fRotationY;
}

void CAView::setRotationY(float fRotationY)
{
    m_fRotationY = fRotationY;
    m_bTransformDirty = m_bInverseDirty = true;
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
    m_fScaleX = fScaleX;
    m_fScaleY = fScaleY;
    m_obFrameRect.size.width = m_fScaleX * m_obContentSize.width;
    m_obFrameRect.size.height = m_fScaleY * m_obContentSize.height;
    CCPoint point = CCPoint(m_obAnchorPointInPoints.x * m_fScaleX, m_obAnchorPointInPoints.y * m_fScaleY);
    m_obFrameRect.origin = ccpSub(m_obPosition, point);
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleX getter
float CAView::getScaleX()
{
    return m_fScaleX;
}

/// scaleX setter
void CAView::setScaleX(float newScaleX)
{
    m_fScaleX = newScaleX;
    m_obFrameRect.size.width = m_fScaleX * m_obContentSize.width;
    float x = m_obAnchorPointInPoints.x * m_fScaleX;
    m_obFrameRect.origin.x = m_obPosition.y - x;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleY getter
float CAView::getScaleY()
{
    return m_fScaleY;
}

/// scaleY setter
void CAView::setScaleY(float newScaleY)
{
    m_fScaleY = newScaleY;
    m_obFrameRect.size.height = m_fScaleY * m_obContentSize.height;
    float y = m_obAnchorPointInPoints.y * m_fScaleY;
    m_obFrameRect.origin.y = m_obPosition.y - y;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// position getter
const CCPoint& CAView::getPosition()
{
    return m_obPosition;
}

/// position setter
void CAView::setPosition(const CCPoint& newPosition)
{
    m_obPosition = newPosition;
    CCPoint point = CCPoint(m_obAnchorPointInPoints.x * m_fScaleX, m_obAnchorPointInPoints.y * m_fScaleY);
    m_obFrameRect.origin = ccpSub(m_obPosition, point);
    m_bTransformDirty = m_bInverseDirty = true;
}

void CAView::getPosition(float* x, float* y)
{
    *x = m_obPosition.x;
    *y = m_obPosition.y;
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
        CCPoint p = this->getFrameOrigin();
        
        m_obAnchorPointInPoints = anchorPointInPoints;
        m_obAnchorPoint = ccp(m_obAnchorPoint.x / m_obContentSize.width,
                              m_obAnchorPoint.y / m_obContentSize.height);
        
        this->setFrameOrigin(p);
        
        m_bTransformDirty = m_bInverseDirty = true;
    }
}

void CAView::setAnchorPoint(const CCPoint& point)
{
    if( ! point.equals(m_obAnchorPoint))
    {
        CCPoint p = this->getFrameOrigin();
        
        m_obAnchorPoint = point;
        m_obAnchorPointInPoints = ccp(m_obContentSize.width * m_obAnchorPoint.x,
                                      m_obContentSize.height * m_obAnchorPoint.y );
        //m_obFrameRect.origin = ccpSub(m_obPosition, m_obAnchorPointInPoints);
        this->setFrameOrigin(p);
        m_bTransformDirty = m_bInverseDirty = true;
    }
}

/// contentSize getter
const CCSize& CAView::getContentSize() const
{
    return m_obContentSize;
}

void CAView::setContentSize(const CCSize & size)
{
    if ( ! size.equals(m_obContentSize))
    {
        m_obContentSize = size;
        
        m_obAnchorPointInPoints = ccp(m_obContentSize.width * m_obAnchorPoint.x, m_obContentSize.height * m_obAnchorPoint.y );
        m_obFrameRect.size = CCSize(m_obContentSize.width * m_fScaleX, m_obContentSize.height * m_fScaleY);

        m_bTransformDirty = m_bInverseDirty = true;

        do
        {
            CC_BREAK_IF(m_pobTexture == NULL);
            CC_BREAK_IF(m_pobTexture->isMonochrome() == false);
            CC_BREAK_IF(m_pobTexture->getContentSize().equals(size));

            this->setTexture(CC_2x2_WHITE_IMAGE(size));
            this->setTextureRect(CCRect(0, 0, size.width, size.height));
        }
        while (0);
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
    this->setPosition(p);
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
    this->setPosition(p);
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

/// isRelativeAnchorPoint getter
bool CAView::isIgnoreAnchorPointForPosition()
{
    return m_bIgnoreAnchorPointForPosition;
}
/// isRelativeAnchorPoint setter
void CAView::ignoreAnchorPointForPosition(bool newValue)
{
    if (newValue != m_bIgnoreAnchorPointForPosition)
    {
		m_bIgnoreAnchorPointForPosition = newValue;
		m_bTransformDirty = m_bInverseDirty = true;
	}
}

/// tag getter
int CAView::getTag() const
{
    return m_nTag;
}

/// tag setter
void CAView::setTag(int var)
{
    m_nTag = var;
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

CCRect CAView::boundingBox()
{
    CCRect rect = CCRectMake(0, 0, m_obContentSize.width, m_obContentSize.height);
    return CCRectApplyAffineTransform(rect, nodeToParentTransform());
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

// lazy allocs
void CAView::childrenAlloc(void)
{
    m_pSubviews = new CCArray(4);
}

CAView* CAView::getSubviewByTag(int aTag)
{
    CCAssert( aTag != kCAViewTagInvalid, "Invalid tag");
    
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
    this->insertSubview(subview, 0);
}

// helper used by reorderChild & add
void CAView::insertSubview(CAView* subview, int z)
{
    CCAssert( subview != NULL, "Argument must be non-nil");
    CCAssert( subview->m_pSuperview == NULL, "child already added. It can't be added again");
    
    if(m_pSubviews == NULL)
    {
        this->childrenAlloc();
    }
    
    m_bReorderChildDirty = true;
    ccArrayAppendObjectWithResize(m_pSubviews->data, subview);
    subview->_setZOrder(z);
    
    subview->setSuperview(this);
    subview->setOrderOfArrival(s_globalOrderOfArrival++);
    
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
    
    if ( m_pSubviews->containsObject(subview) )
    {
        this->detachSubview(subview);
    }
}


void CAView::removeSubviewByTag(int tag)
{
    CCAssert( tag != kCAViewTagInvalid, "Invalid tag");
    
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
        CCObject* subview;
        CCARRAY_FOREACH(m_pSubviews, subview)
        {
            CAView* pNode = (CAView*) subview;
            if (pNode)
            {
                // IMPORTANT:
                //  -1st do onExit
                //  -2nd cleanup
                if(m_bRunning)
                {
                    pNode->onExitTransitionDidStart();
                    pNode->onExit();
                }
                
                pNode->cleanup();
                // set parent nil at the end
                pNode->setSuperview(NULL);
            }
        }
        
        m_pSubviews->removeAllObjects();
    }
    
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
        
        m_bReorderChildDirty = false;
    }
}

#include "CCSprite.h"
void CAView::draw()
{
    //CCAssert(0);
    // override me
    // Only use- this function to draw your stuff.
    // DON'T draw your stuff outside this method
    
    if (m_pobTexture == NULL)
        return;
    
    CC_NODE_DRAW_SETUP();
    
    ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
    
    ccGLBindTexture2D( m_pobTexture->getName() );
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
    // draw texture box
    CCSize s = this->getTextureRect().size;
    CCPoint offsetPix = this->getOffsetPosition();
    CCPoint vertices[4] = {
        ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
        ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");

    
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
        CAView::rendering();
    }
    else
    {
        glEnable(GL_SCISSOR_TEST);// 开启显示指定区域
        
        CCPoint point = this->convertToWorldSpace(CCPoint(0, this->getContentSize().height));
        point = CCDirector::sharedDirector()->convertToGL(point);
        CCSize size = this->getContentSize();
        
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
        
        //y = CCDirector::sharedDirector()->getWinSize().height - height - y;
        
		glScissor(x, y, width, height);// 只显示当前窗口的区域
        
        CAView::rendering();
        
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
        
        // self draw
        this->draw();
        
        for( ; i < arrayData->num; i++ )
        {
            pNode = (CAView*) arrayData->arr[i];
            if (pNode)
            {
                pNode->visit();
            }
        }
    }
    else
    {
        this->draw();
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
    
    if (m_bTouchEnabled)
    {
        this->registerWithTouchDispatcher();
    }
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
    
    CCDirector* pDirector = CCDirector::sharedDirector();
    if( m_bTouchEnabled )
    {
        pDirector->getTouchDispatcher()->removeDelegate(this);
    }
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
        float x = m_obPosition.x;
        float y = -m_obPosition.y;
        
        float height = 0;
        
        if (this->getSuperview())
        {
            height= this->getSuperview()->getContentSize().height;
        }
        else
        {
            height= CCDirector::sharedDirector()->getWinSize().height;
        }
        
        y += height;
        
        if (m_bIgnoreAnchorPointForPosition)
        {
            x += m_obAnchorPointInPoints.x;
            y -= m_obContentSize.height - m_obAnchorPointInPoints.y;
        }
        
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

CCPoint CAView::convertToWindowSpace(const CCPoint& nodePoint)
{
    CCPoint worldPoint = this->convertToWorldSpace(nodePoint);
    return CCDirector::sharedDirector()->convertToUI(worldPoint);
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
    arrayMakeObjectsPerformSelector(m_pSubviews, updateTransform, CAView*);
    
    if(m_pobTexture && isDirty() ) {
        
        // If it is not visible, or one of its ancestors is not visible, then do nothing:
        if( !m_bVisible || ( m_pSuperview && ((CCSprite*)m_pSuperview)->m_bShouldBeHidden) )
        {
            m_sQuad.br.vertices = m_sQuad.tl.vertices = m_sQuad.tr.vertices = m_sQuad.bl.vertices = vertex3(0,0,0);
            m_bShouldBeHidden = true;
        }
        else
        {
            m_bShouldBeHidden = false;
            
            if( ! m_pSuperview)
            {
                m_transformToBatch = nodeToParentTransform();
            }
            else
            {
                CCAssert( dynamic_cast<CCSprite*>(m_pSuperview), "Logic error in CCSprite. Parent must be a CCSprite");
                m_transformToBatch = CCAffineTransformConcat( nodeToParentTransform() , ((CCSprite*)m_pSuperview)->m_transformToBatch );
            }
            
            //
            // calculate the Quad based on the Affine Matrix
            //
            
            CCSize size = m_obRect.size;
            
            float x1 = m_obOffsetPosition.x;
            float y1 = m_obOffsetPosition.y;
            
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
        
        // MARMALADE CHANGE: ADDED CHECK FOR NULL, TO PERMIT SPRITES WITH NO BATCH NODE / TEXTURE ATLAS
        
        m_bRecursiveDirty = false;
        setDirty(false);
    }
    
    // MARMALADE CHANGED
    // recursively iterate over children
    /*    if( m_bHasChildren )
     {
     // MARMALADE: CHANGED TO USE CCNode*
     // NOTE THAT WE HAVE ALSO DEFINED virtual CAView::updateTransform()
     arrayMakeObjectsPerformSelector(m_pSubviews, updateTransform, CCSprite*);
     }*/
    CAView::updateTransform();
    
#if CC_SPRITE_DEBUG_DRAW
    // draw bounding box
    CCPoint vertices[4] = {
        ccp( m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y ),
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


/// Touch and Accelerometer related

void CAView::registerWithTouchDispatcher()
{
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    pDispatcher->addTargetedDelegate(this, m_nTouchPriority, true);
}

/// isTouchEnabled getter
bool CAView::isTouchEnabled()
{
    return m_bTouchEnabled;
}
/// isTouchEnabled setter
void CAView::setTouchEnabled(bool enabled)
{
    if (m_bTouchEnabled != enabled)
    {
        m_bTouchEnabled = enabled;
        if (m_bRunning)
        {
            if (enabled)
            {
                this->registerWithTouchDispatcher();
            }
            else
            {
                // have problems?
                CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
            }
        }
    }
}

void CAView::setTouchPriority(int priority)
{
    if (m_nTouchPriority != priority)
    {
        m_nTouchPriority = priority;
        
		if( m_bTouchEnabled)
        {
			setTouchEnabled(false);
			setTouchEnabled(true);
		}
    }
}

int CAView::getTouchPriority()
{
    return m_nTouchPriority;
}

bool CAView::isDisplayRange()
{
    return m_bDisplayRange;
}

void CAView::setDisplayRange(bool value)
{
    m_bDisplayRange = value;
}

bool CAView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    CCAssert(false, "Layer#ccTouchBegan override me");
    return true;
}

void CAView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CAView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

GLubyte CAView::getOpacity(void)
{
	return _realOpacity;
}

GLubyte CAView::getDisplayedOpacity(void)
{
	return _displayedOpacity;
}

void CAView::setOpacity(GLubyte opacity)
{
    _displayedOpacity = _realOpacity = opacity;
    
	if (_cascadeOpacityEnabled)
    {
		GLubyte parentOpacity = 255;
        CCRGBAProtocol* pParent = dynamic_cast<CCRGBAProtocol*>(m_pSuperview);
        if (pParent && pParent->isCascadeOpacityEnabled())
        {
            parentOpacity = pParent->getDisplayedOpacity();
        }
        this->updateDisplayedOpacity(parentOpacity);
	}
    
    this->updateColor();
}

void CAView::updateDisplayedOpacity(GLubyte parentOpacity)
{
	_displayedOpacity = _realOpacity * parentOpacity/255.0;
	
    if (_cascadeOpacityEnabled)
    {
        CCObject* pObj;
        CCARRAY_FOREACH(m_pSubviews, pObj)
        {
            CCRGBAProtocol* item = dynamic_cast<CCRGBAProtocol*>(pObj);
            if (item)
            {
                item->updateDisplayedOpacity(_displayedOpacity);
            }
        }
    }
    
    this->updateColor();
}

bool CAView::isCascadeOpacityEnabled(void)
{
    return _cascadeOpacityEnabled;
}

void CAView::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    _cascadeOpacityEnabled = cascadeOpacityEnabled;
}

const ccColor3B& CAView::getColor(void)
{
	return _realColor;
}

const ccColor3B& CAView::getDisplayedColor()
{
	return _displayedColor;
}

void CAView::setColor(const ccColor3B& color)
{
	_displayedColor = _realColor = color;
	
	if (_cascadeColorEnabled)
    {
		ccColor3B parentColor = ccWHITE;
        CCRGBAProtocol *parent = dynamic_cast<CCRGBAProtocol*>(m_pSuperview);
		if (parent && parent->isCascadeColorEnabled())
        {
            parentColor = parent->getDisplayedColor();
        }
        
        updateDisplayedColor(parentColor);
	}
    else
    {
        this->updateColor();
    }
}

void CAView::updateDisplayedColor(const ccColor3B& parentColor)
{
	_displayedColor.r = _realColor.r * parentColor.r/255.0;
	_displayedColor.g = _realColor.g * parentColor.g/255.0;
	_displayedColor.b = _realColor.b * parentColor.b/255.0;
    
    if (_cascadeColorEnabled)
    {
        CCObject *obj = NULL;
        CCARRAY_FOREACH(m_pSubviews, obj)
        {
            CCRGBAProtocol *item = dynamic_cast<CCRGBAProtocol*>(obj);
            if (item)
            {
                item->updateDisplayedColor(_displayedColor);
            }
        }
    }
    
    updateColor();
}


bool CAView::isCascadeColorEnabled(void)
{
    return _cascadeColorEnabled;
}

void CAView::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    _cascadeColorEnabled = cascadeColorEnabled;
}


//Image...

void CAView::setTexture(CCTexture2D *texture)
{
    CC_SAFE_RETAIN(texture);
    CC_SAFE_RELEASE(m_pobTexture);
    m_pobTexture = texture;
    updateBlendFunc();
}

CCTexture2D* CAView::getTexture(void)
{
    return m_pobTexture;
}

void CAView::setTextureRect(const CCRect& rect)
{
    setTextureRect(rect, false, rect.size);
}


void CAView::setTextureRect(const CCRect& rect, bool rotated, const CCSize& untrimmedSize)
{
    m_bRectRotated = rotated;
    
    setContentSize(untrimmedSize);
    setVertexRect(rect);
    setTextureCoords(rect);
    
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
    
    m_obOffsetPosition.x = relativeOffset.x + (m_obContentSize.width - m_obRect.size.width) / 2;
    m_obOffsetPosition.y = relativeOffset.y + (m_obContentSize.height - m_obRect.size.height) / 2;
    
    // self rendering
    
    // Atlas: Vertex
    float x1 = 0 + m_obOffsetPosition.x;
    float y1 = 0 + m_obOffsetPosition.y;
    float x2 = x1 + m_obRect.size.width;
    float y2 = y1 + m_obRect.size.height;
    
    // Don't update Z.
    m_sQuad.bl.vertices = vertex3(x1, y1, 0);
    m_sQuad.br.vertices = vertex3(x2, y1, 0);
    m_sQuad.tl.vertices = vertex3(x1, y2, 0);
    m_sQuad.tr.vertices = vertex3(x2, y2, 0);
}

// override this method to generate "double scale" sprites
void CAView::setVertexRect(const CCRect& rect)
{
    m_obRect = rect;
}

void CAView::setTextureCoords(CCRect rect)
{
    rect = CC_RECT_POINTS_TO_PIXELS(rect);
    
    CCTexture2D *tex = m_pobTexture;
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

void CAView::updateColor(void)
{
    if (m_pobTexture == NULL)
    {
        return;
    }
    
    ccColor4B color4 = { _displayedColor.r, _displayedColor.g, _displayedColor.b, _displayedOpacity };
    
    // special opacity for premultiplied textures
	if (m_bOpacityModifyRGB)
    {
		color4.r *= _displayedOpacity/255.0f;
		color4.g *= _displayedOpacity/255.0f;
		color4.b *= _displayedOpacity/255.0f;
    }
    
    m_sQuad.bl.colors = color4;
    m_sQuad.br.colors = color4;
    m_sQuad.tl.colors = color4;
    m_sQuad.tr.colors = color4;
    
    
    // self render
    // do nothing
}

void CAView::setOpacityModifyRGB(bool modify)
{
    if (m_bOpacityModifyRGB != modify)
    {
        m_bOpacityModifyRGB = modify;
        updateColor();
    }
}

bool CAView::isOpacityModifyRGB(void)
{
    return m_bOpacityModifyRGB;
}

void CAView::updateBlendFunc(void)
{
    // it is possible to have an untextured sprite
    if (! m_pobTexture || ! m_pobTexture->hasPremultipliedAlpha())
    {
        m_sBlendFunc.src = GL_SRC_ALPHA;
        m_sBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
        setOpacityModifyRGB(false);
    }
    else
    {
        m_sBlendFunc.src = CC_BLEND_SRC;
        m_sBlendFunc.dst = CC_BLEND_DST;
        setOpacityModifyRGB(true);
    }
}

void CAView::setReorderChildDirtyRecursively(void)
{
    //only set parents flag the first time
    if ( ! m_bReorderChildDirty )
    {
        m_bReorderChildDirty = true;
        CAView* pNode = (CAView*)m_pSuperview;
        while (pNode)
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
            CCSprite* pChild = dynamic_cast<CCSprite*>(pObject);
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
        if (m_pobTexture)
        {
            setTextureRect(m_obRect, m_bRectRotated, m_obContentSize);
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
        if (m_pobTexture)
        {
            setTextureRect(m_obRect, m_bRectRotated, m_obContentSize);
        }
    }
}

bool CAView::isFlipY(void)
{
    return m_bFlipY;
}

NS_CC_END;
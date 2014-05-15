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


CAView_::CAView_(void)
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
{
    CCDirector *director = CCDirector::sharedDirector();
    m_pActionManager = director->getActionManager();
    m_pActionManager->retain();
    
    m_pComponentContainer = new CCComponentContainer(this);
}

CAView_::~CAView_(void)
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
            CAView_* pChild = (CAView_*) subview;
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
}

bool CAView_::init()
{
    return true;
}

float CAView_::getSkewX()
{
    return m_fSkewX;
}

void CAView_::setSkewX(float newSkewX)
{
    m_fSkewX = newSkewX;
    m_bTransformDirty = m_bInverseDirty = true;
}

float CAView_::getSkewY()
{
    return m_fSkewY;
}

void CAView_::setSkewY(float newSkewY)
{
    m_fSkewY = newSkewY;
    
    m_bTransformDirty = m_bInverseDirty = true;
}

/// zOrder getter
int CAView_::getZOrder()
{
    return m_nZOrder;
}

/// zOrder setter : private method
/// used internally to alter the zOrder variable. DON'T call this method manually
void CAView_::_setZOrder(int z)
{
    m_nZOrder = z;
}

void CAView_::setZOrder(int z)
{
    _setZOrder(z);
    if (m_pSuperview)
    {
        m_pSuperview->reorderSubview(this, z);
    }
}

/// vertexZ getter
float CAView_::getVertexZ()
{
    return m_fVertexZ;
}


/// vertexZ setter
void CAView_::setVertexZ(float var)
{
    m_fVertexZ = var;
}


/// rotation getter
float CAView_::getRotation()
{
    CCAssert(m_fRotationX == m_fRotationY, "CAView_#rotation. RotationX != RotationY. Don't know which one to return");
    return m_fRotationX;
}

/// rotation setter
void CAView_::setRotation(float newRotation)
{
    m_fRotationX = m_fRotationY = newRotation;
    m_bTransformDirty = m_bInverseDirty = true;
}

float CAView_::getRotationX()
{
    return m_fRotationX;
}

void CAView_::setRotationX(float fRotationX)
{
    m_fRotationX = fRotationX;
    m_bTransformDirty = m_bInverseDirty = true;
}

float CAView_::getRotationY()
{
    return m_fRotationY;
}

void CAView_::setRotationY(float fRotationY)
{
    m_fRotationY = fRotationY;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scale getter
float CAView_::getScale(void)
{
    CCAssert( m_fScaleX == m_fScaleY, "CAView_#scale. ScaleX != ScaleY. Don't know which one to return");
    return m_fScaleX;
}

/// scale setter
void CAView_::setScale(float scale)
{
    this->setScale(scale, scale);
}

/// scale setter
void CAView_::setScale(float fScaleX,float fScaleY)
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
float CAView_::getScaleX()
{
    return m_fScaleX;
}

/// scaleX setter
void CAView_::setScaleX(float newScaleX)
{
    m_fScaleX = newScaleX;
    m_obFrameRect.size.width = m_fScaleX * m_obContentSize.width;
    float x = m_obAnchorPointInPoints.x * m_fScaleX;
    m_obFrameRect.origin.x = m_obPosition.y - x;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleY getter
float CAView_::getScaleY()
{
    return m_fScaleY;
}

/// scaleY setter
void CAView_::setScaleY(float newScaleY)
{
    m_fScaleY = newScaleY;
    m_obFrameRect.size.height = m_fScaleY * m_obContentSize.height;
    float y = m_obAnchorPointInPoints.y * m_fScaleY;
    m_obFrameRect.origin.y = m_obPosition.y - y;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// position getter
const CCPoint& CAView_::getPosition()
{
    return m_obPosition;
}

/// position setter
void CAView_::setPosition(const CCPoint& newPosition)
{
    m_obPosition = newPosition;
    CCPoint point = CCPoint(m_obAnchorPointInPoints.x * m_fScaleX, m_obAnchorPointInPoints.y * m_fScaleY);
    m_obFrameRect.origin = ccpSub(m_obPosition, point);
    m_bTransformDirty = m_bInverseDirty = true;
}

void CAView_::getPosition(float* x, float* y)
{
    *x = m_obPosition.x;
    *y = m_obPosition.y;
}

void CAView_::setPosition(float x, float y)
{
    setPosition(ccp(x, y));
}

float CAView_::getPositionX(void)
{
    return m_obPosition.x;
}

float CAView_::getPositionY(void)
{
    return  m_obPosition.y;
}

void CAView_::setPositionX(float x)
{
    setPosition(ccp(x, m_obPosition.y));
}

void CAView_::setPositionY(float y)
{
    setPosition(ccp(m_obPosition.x, y));
}

/// children getter
CCArray* CAView_::getSubviews()
{
    return m_pSubviews;
}

unsigned int CAView_::getSubviewsCount(void) const
{
    return m_pSubviews ? m_pSubviews->count() : 0;
}

/// camera getter: lazy alloc
CCCamera* CAView_::getCamera()
{
    if (!m_pCamera)
    {
        m_pCamera = new CCCamera();
    }
    
    return m_pCamera;
}


/// grid getter
CCGridBase* CAView_::getGrid()
{
    return m_pGrid;
}

/// grid setter
void CAView_::setGrid(CCGridBase* pGrid)
{
    CC_SAFE_RETAIN(pGrid);
    CC_SAFE_RELEASE(m_pGrid);
    m_pGrid = pGrid;
}


/// isVisible getter
bool CAView_::isVisible()
{
    return m_bVisible;
}

/// isVisible setter
void CAView_::setVisible(bool var)
{
    m_bVisible = var;
}

const CCPoint& CAView_::getAnchorPointInPoints()
{
    return m_obAnchorPointInPoints;
}

/// anchorPoint getter
const CCPoint& CAView_::getAnchorPoint()
{
    return m_obAnchorPoint;
}

void CAView_::setAnchorPoint(const CCPoint& point)
{
    if( ! point.equals(m_obAnchorPoint))
    {
        m_obAnchorPoint = point;
        m_obAnchorPointInPoints = ccp(m_obContentSize.width * m_obAnchorPoint.x,
                                      m_obContentSize.height * m_obAnchorPoint.y );
        m_obFrameRect.origin = ccpSub(m_obPosition, m_obAnchorPointInPoints);
        m_bTransformDirty = m_bInverseDirty = true;
    }
}

/// contentSize getter
const CCSize& CAView_::getContentSize() const
{
    return m_obContentSize;
}

void CAView_::setContentSize(const CCSize & size)
{
    if ( ! size.equals(m_obContentSize))
    {
        m_obContentSize = size;
        
        m_obAnchorPointInPoints = ccp(m_obContentSize.width * m_obAnchorPoint.x, m_obContentSize.height * m_obAnchorPoint.y );
        m_obFrameRect.size = CCSize(m_obContentSize.width * m_fScaleX, m_obContentSize.height * m_fScaleY);
        
        m_bTransformDirty = m_bInverseDirty = true;
    }
}

const CCRect& CAView_::getFrame() const
{
    return m_obFrameRect;
}

void CAView_::setFrame(const CCRect &rect)
{
    if ( ! rect.size.equals(CCSizeZero))
    {
        this->setContentSize(CCSize(rect.size.width / m_fScaleX, rect.size.height / m_fScaleY));
    }
    
    CCPoint point = CCPoint(m_obAnchorPointInPoints.x * m_fScaleX,
                            m_obAnchorPointInPoints.y * m_fScaleY);
    point = ccpAdd(point, rect.origin);
    this->setPosition(point);
}

CCRect CAView_::getBounds() const
{
    CCRect rect = CCRectZero;
    rect.size = m_obContentSize;
    return rect;
}

void CAView_::setBoundsSize(const CCSize& size)
{
    if ( ! size.equals(CCSizeZero))
    {
        this->setContentSize(CCSize(size.width, size.height));
    }
}


CCPoint CAView_::getCenter()
{
    CCPoint point = ccpSub(ccpMult(m_obContentSize, 0.5f), m_obAnchorPointInPoints);
    point = CCPoint(point.x * m_fScaleX, point.y * m_fScaleY);
    point = ccpAdd(point, m_obPosition);
    
    return point;
}


void CAView_::setCenter(CCPoint center)
{
    CCPoint point = ccpSub(ccpMult(m_obContentSize, 0.5f), m_obAnchorPointInPoints);
    point = CCPoint(point.x * m_fScaleX, point.y * m_fScaleY);
    point = ccpSub(center, point);
    
    this->setPosition(point);
}

// isRunning getter
bool CAView_::isRunning()
{
    return m_bRunning;
}

/// parent getter
CAView_ * CAView_::getSuperview()
{
    return m_pSuperview;
}
/// parent setter
void CAView_::setSuperview(cocos2d::CAView_ *var)
{
    m_pSuperview = var;
}

/// isRelativeAnchorPoint getter
bool CAView_::isIgnoreAnchorPointForPosition()
{
    return m_bIgnoreAnchorPointForPosition;
}
/// isRelativeAnchorPoint setter
void CAView_::ignoreAnchorPointForPosition(bool newValue)
{
    if (newValue != m_bIgnoreAnchorPointForPosition)
    {
		m_bIgnoreAnchorPointForPosition = newValue;
		m_bTransformDirty = m_bInverseDirty = true;
	}
}

/// tag getter
int CAView_::getTag() const
{
    return m_nTag;
}

/// tag setter
void CAView_::setTag(int var)
{
    m_nTag = var;
}

/// userData getter
void * CAView_::getUserData()
{
    return m_pUserData;
}

/// userData setter
void CAView_::setUserData(void *var)
{
    m_pUserData = var;
}

unsigned int CAView_::getOrderOfArrival()
{
    return m_uOrderOfArrival;
}

void CAView_::setOrderOfArrival(unsigned int uOrderOfArrival)
{
    m_uOrderOfArrival = uOrderOfArrival;
}

CCGLProgram* CAView_::getShaderProgram()
{
    return m_pShaderProgram;
}

CCObject* CAView_::getUserObject()
{
    return m_pUserObject;
}

ccGLServerState CAView_::getGLServerState()
{
    return m_eGLServerState;
}

void CAView_::setGLServerState(ccGLServerState glServerState)
{
    m_eGLServerState = glServerState;
}

void CAView_::setUserObject(CCObject *pUserObject)
{
    CC_SAFE_RETAIN(pUserObject);
    CC_SAFE_RELEASE(m_pUserObject);
    m_pUserObject = pUserObject;
}

void CAView_::setShaderProgram(CCGLProgram *pShaderProgram)
{
    CC_SAFE_RETAIN(pShaderProgram);
    CC_SAFE_RELEASE(m_pShaderProgram);
    m_pShaderProgram = pShaderProgram;
}

CCRect CAView_::boundingBox()
{
    CCRect rect = CCRectMake(0, 0, m_obContentSize.width, m_obContentSize.height);
    return CCRectApplyAffineTransform(rect, nodeToParentTransform());
}

CAView_ * CAView_::create(void)
{
	CAView_ * pRet = new CAView_();
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

void CAView_::cleanup()
{
    // actions
    this->stopAllActions();
    // timers
    arrayMakeObjectsPerformSelector(m_pSubviews, cleanup, CAView_*);
}


const char* CAView_::description()
{
    return CCString::createWithFormat("<CAView_ | Tag = %d>", m_nTag)->getCString();
}

// lazy allocs
void CAView_::childrenAlloc(void)
{
    m_pSubviews = new CCArray(4);
}

CAView_* CAView_::getSubviewByTag(int aTag)
{
    CCAssert( aTag != kCAViewTagInvalid, "Invalid tag");
    
    if(m_pSubviews && m_pSubviews->count() > 0)
    {
        CCObject* subview;
        CCARRAY_FOREACH(m_pSubviews, subview)
        {
            CAView_* pNode = (CAView_*) subview;
            if(pNode && pNode->m_nTag == aTag)
                return pNode;
        }
    }
    return NULL;
}


void CAView_::addSubview(CAView_ *subview)
{
    this->insertSubview(subview, 0);
}

// helper used by reorderChild & add
void CAView_::insertSubview(CAView_* subview, int z)
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

void CAView_::removeFromSuperview()
{
    if (m_pSuperview != NULL)
    {
        m_pSuperview->removeSubview(this);
    }
}

void CAView_::removeSubview(CAView_* subview)
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


void CAView_::removeSubviewByTag(int tag)
{
    CCAssert( tag != kCAViewTagInvalid, "Invalid tag");
    
    CAView_ *subview = this->getSubviewByTag(tag);
    
    if (subview == NULL)
    {
        CCLOG("cocos2d: removeChildByTag(tag = %d): child not found!", tag);
    }
    else
    {
        this->removeSubview(subview);
    }
}

void CAView_::removeAllSubviews()
{
    // not using detachChild improves speed here
    if ( m_pSubviews && m_pSubviews->count() > 0 )
    {
        CCObject* subview;
        CCARRAY_FOREACH(m_pSubviews, subview)
        {
            CAView_* pNode = (CAView_*) subview;
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


void CAView_::detachSubview(CAView_ *subview)
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

void CAView_::reorderSubview(CAView_ *subview, int zOrder)
{
    CCAssert( subview != NULL, "Child must be non-nil");
    m_bReorderChildDirty = true;
    subview->setOrderOfArrival(s_globalOrderOfArrival++);
    subview->_setZOrder(zOrder);
}

void CAView_::sortAllSubviews()
{
    if (m_bReorderChildDirty)
    {
        int i,j,length = m_pSubviews->data->num;
        CAView_ ** x = (CAView_**)m_pSubviews->data->arr;
        CAView_ *tempItem;
        
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


void CAView_::draw()
{
    //CCAssert(0);
    // override me
    // Only use- this function to draw your stuff.
    // DON'T draw your stuff outside this method
}

void CAView_::visit()
{
    // quick return if not visible. children won't be drawn.
    if (!m_bVisible)
    {
        return;
    }
    kmGLPushMatrix();
    
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
    }
    
    this->transform();
    
    CAView_* pNode = NULL;
    unsigned int i = 0;
    
    if(m_pSubviews && m_pSubviews->count() > 0)
    {
        this->sortAllSubviews();
        // draw children zOrder < 0
        ccArray *arrayData = m_pSubviews->data;
        for( ; i < arrayData->num; i++ )
        {
            pNode = (CAView_*) arrayData->arr[i];
            
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
            pNode = (CAView_*) arrayData->arr[i];
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

void CAView_::transformAncestors()
{
    if( m_pSuperview != NULL  )
    {
        m_pSuperview->transformAncestors();
        m_pSuperview->transform();
    }
}

void CAView_::transform()
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


void CAView_::onEnter()
{
    arrayMakeObjectsPerformSelector(m_pSubviews, onEnter, CAView_*);

    m_bRunning = true;
    
}

void CAView_::onEnterTransitionDidFinish()
{
    arrayMakeObjectsPerformSelector(m_pSubviews, onEnterTransitionDidFinish, CAView_*);
}

void CAView_::onExitTransitionDidStart()
{
    arrayMakeObjectsPerformSelector(m_pSubviews, onExitTransitionDidStart, CAView_*);
    
}

void CAView_::onExit()
{
    m_bRunning = false;
    
    arrayMakeObjectsPerformSelector(m_pSubviews, onExit, CAView_*);
}

void CAView_::setActionManager(CCActionManager* actionManager)
{
    if( actionManager != m_pActionManager ) {
        this->stopAllActions();
        CC_SAFE_RETAIN(actionManager);
        CC_SAFE_RELEASE(m_pActionManager);
        m_pActionManager = actionManager;
    }
}

CCActionManager* CAView_::getActionManager()
{
    return m_pActionManager;
}

CCAction * CAView_::runAction(CCAction* action)
{
    CCAssert( action != NULL, "Argument must be non-nil");
    m_pActionManager->addAction(action, this, !m_bRunning);
    return action;
}

void CAView_::stopAllActions()
{
    m_pActionManager->removeAllActionsFromTarget(this);
}

void CAView_::stopAction(CCAction* action)
{
    m_pActionManager->removeAction(action);
}

void CAView_::stopActionByTag(int tag)
{
    CCAssert( tag != kCCActionTagInvalid, "Invalid tag");
    m_pActionManager->removeActionByTag(tag, this);
}

CCAction * CAView_::getActionByTag(int tag)
{
    CCAssert( tag != kCCActionTagInvalid, "Invalid tag");
    return m_pActionManager->getActionByTag(tag, this);
}

unsigned int CAView_::numberOfRunningActions()
{
    return m_pActionManager->numberOfRunningActionsInTarget(this);
}



// override me
void CAView_::update(float fDelta)
{
    if (m_pComponentContainer && !m_pComponentContainer->isEmpty())
    {
        m_pComponentContainer->visit(fDelta);
    }
}

CCAffineTransform CAView_::nodeToParentTransform(void)
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

void CAView_::setAdditionalTransform(const CCAffineTransform& additionalTransform)
{
    m_sAdditionalTransform = additionalTransform;
    m_bTransformDirty = true;
    m_bAdditionalTransformDirty = true;
}

CCAffineTransform CAView_::parentToNodeTransform(void)
{
    if ( m_bInverseDirty ) {
        m_sInverse = CCAffineTransformInvert(this->nodeToParentTransform());
        m_bInverseDirty = false;
    }
    
    return m_sInverse;
}

CCAffineTransform CAView_::nodeToWorldTransform()
{
    CCAffineTransform t = this->nodeToParentTransform();
    
    for (CAView_ *p = m_pSuperview; p != NULL; p = p->getSuperview())
        t = CCAffineTransformConcat(t, p->nodeToParentTransform());
    
    return t;
}

CCAffineTransform CAView_::worldToNodeTransform(void)
{
    return CCAffineTransformInvert(this->nodeToWorldTransform());
}

CCPoint CAView_::convertToNodeSpace(const CCPoint& worldPoint)
{
    CCPoint p = CCDirector::sharedDirector()->convertToGL(worldPoint);
    CCPoint ret = CCPointApplyAffineTransform(p, worldToNodeTransform());
    ret.y = this->getBounds().size.height - ret.y;
    return ret;
}

CCPoint CAView_::convertToWorldSpace(const CCPoint& nodePoint)
{
    CCPoint p = nodePoint;
    p.y = this->getBounds().size.height - p.y;
    CCPoint ret = CCPointApplyAffineTransform(p, nodeToWorldTransform());
    ret = CCDirector::sharedDirector()->convertToUI(ret);
    return ret;
}

CCPoint CAView_::convertToNodeSpaceAR(const CCPoint& worldPoint)
{
    CCPoint nodePoint = convertToNodeSpace(worldPoint);
    return ccpSub(nodePoint, m_obAnchorPointInPoints);
}

CCPoint CAView_::convertToWorldSpaceAR(const CCPoint& nodePoint)
{
    CCPoint pt = ccpAdd(nodePoint, m_obAnchorPointInPoints);
    return convertToWorldSpace(pt);
}

CCPoint CAView_::convertToWindowSpace(const CCPoint& nodePoint)
{
    CCPoint worldPoint = this->convertToWorldSpace(nodePoint);
    return CCDirector::sharedDirector()->convertToUI(worldPoint);
}

// convenience methods which take a CCTouch instead of CCPoint
CCPoint CAView_::convertTouchToNodeSpace(CCTouch *touch)
{
    CCPoint point = touch->getLocation();
    return this->convertToNodeSpace(point);
}
CCPoint CAView_::convertTouchToNodeSpaceAR(CCTouch *touch)
{
    CCPoint point = touch->getLocation();
    return this->convertToNodeSpaceAR(point);
}

void CAView_::updateTransform()
{
    // Recursively iterate over children
    arrayMakeObjectsPerformSelector(m_pSubviews, updateTransform, CAView_*);
}

CCComponent* CAView_::getComponent(const char *pName) const
{
    return m_pComponentContainer->get(pName);
}

bool CAView_::addComponent(CCComponent *pComponent)
{
    return m_pComponentContainer->add(pComponent);
}

bool CAView_::removeComponent(const char *pName)
{
    return m_pComponentContainer->remove(pName);
}

void CAView_::removeAllComponents()
{
    m_pComponentContainer->removeAll();
}







// CCNodeRGBA
CCNodeRGBA::CCNodeRGBA()
: _displayedOpacity(255)
, _realOpacity(255)
, _displayedColor(ccWHITE)
, _realColor(ccWHITE)
, _cascadeColorEnabled(false)
, _cascadeOpacityEnabled(false)
, m_bTouchEnabled(false)
, m_bKeypadEnabled(false)
, m_bDisplayRange(true)
, m_nTouchPriority(0)
, m_eTouchMode(kCCTouchesOneByOne)
{}

CCNodeRGBA::~CCNodeRGBA() {}

bool CCNodeRGBA::init()
{
    if (CAView_::init())
    {
        _displayedOpacity = _realOpacity = 255;
        _displayedColor = _realColor = ccWHITE;
        _cascadeOpacityEnabled = _cascadeColorEnabled = false;
        
        this->setAnchorPoint(CCPoint(0.5f, 0.5f));
        
        return true;
    }
    return false;
}

/// Touch and Accelerometer related

void CCNodeRGBA::registerWithTouchDispatcher()
{
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    
    if( m_eTouchMode == kCCTouchesAllAtOnce )
    {
        pDispatcher->addStandardDelegate(this, 0);
    }
    else
    {
        pDispatcher->addTargetedDelegate(this, m_nTouchPriority, true);
    }
}

/// isTouchEnabled getter
bool CCNodeRGBA::isTouchEnabled()
{
    return m_bTouchEnabled;
}
/// isTouchEnabled setter
void CCNodeRGBA::setTouchEnabled(bool enabled)
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

void CCNodeRGBA::setTouchMode(ccTouchesMode mode)
{
    if(m_eTouchMode != mode)
    {
        m_eTouchMode = mode;
        
		if( m_bTouchEnabled)
        {
			setTouchEnabled(false);
			setTouchEnabled(true);
		}
    }
}

void CCNodeRGBA::setTouchPriority(int priority)
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

int CCNodeRGBA::getTouchPriority()
{
    return m_nTouchPriority;
}

int CCNodeRGBA::getTouchMode()
{
    return m_eTouchMode;
}

/// isKeypadEnabled getter
bool CCNodeRGBA::isKeypadEnabled()
{
    return m_bKeypadEnabled;
}
/// isKeypadEnabled setter
void CCNodeRGBA::setKeypadEnabled(bool enabled)
{
    if (enabled != m_bKeypadEnabled)
    {
        m_bKeypadEnabled = enabled;
        
        if (m_bRunning)
        {
            CCDirector* pDirector = CCDirector::sharedDirector();
            if (enabled)
            {
                pDirector->getKeypadDispatcher()->addDelegate(this);
            }
            else
            {
                pDirector->getKeypadDispatcher()->removeDelegate(this);
            }
        }
    }
}

void CCNodeRGBA::keyBackClicked(void)
{
    
}

void CCNodeRGBA::keyMenuClicked(void)
{
    
}

/// Callbacks
void CCNodeRGBA::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    // register 'parent' nodes first
    // since events are propagated in reverse order
    if (m_bTouchEnabled)
    {
        this->registerWithTouchDispatcher();
    }
    
    // then iterate over all the children
    CAView_::onEnter();
    
    // add this layer to concern the keypad msg
    if (m_bKeypadEnabled)
    {
        pDirector->getKeypadDispatcher()->addDelegate(this);
    }
}

void CCNodeRGBA::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    if( m_bTouchEnabled )
    {
        pDirector->getTouchDispatcher()->removeDelegate(this);
    }
    
    // remove this layer from the delegates who concern the keypad msg
    if (m_bKeypadEnabled)
    {
        pDirector->getKeypadDispatcher()->removeDelegate(this);
    }
    
    CAView_::onExit();
}

void CCNodeRGBA::visit()
{
    if (m_bDisplayRange)
    {
        CAView_::visit();
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
        
        CAView_* parent = this->getSuperview();
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
        
        CAView_::visit();
        
        glDisable(GL_SCISSOR_TEST);// 禁用
    }
}

bool CCNodeRGBA::isDisplayRange()
{
    return m_bDisplayRange;
}

void CCNodeRGBA::setDisplayRange(bool value)
{
    m_bDisplayRange = value;
}

bool CCNodeRGBA::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    CCAssert(false, "Layer#ccTouchBegan override me");
    return true;
}

void CCNodeRGBA::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CCNodeRGBA::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CCNodeRGBA::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CCNodeRGBA::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

void CCNodeRGBA::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

void CCNodeRGBA::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

void CCNodeRGBA::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

CCNodeRGBA * CCNodeRGBA::create(void)
{
	CCNodeRGBA * pRet = new CCNodeRGBA();
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

GLubyte CCNodeRGBA::getOpacity(void)
{
	return _realOpacity;
}

GLubyte CCNodeRGBA::getDisplayedOpacity(void)
{
	return _displayedOpacity;
}

void CCNodeRGBA::setOpacity(GLubyte opacity)
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
}

void CCNodeRGBA::updateDisplayedOpacity(GLubyte parentOpacity)
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
}

bool CCNodeRGBA::isCascadeOpacityEnabled(void)
{
    return _cascadeOpacityEnabled;
}

void CCNodeRGBA::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    _cascadeOpacityEnabled = cascadeOpacityEnabled;
}

const ccColor3B& CCNodeRGBA::getColor(void)
{
	return _realColor;
}

const ccColor3B& CCNodeRGBA::getDisplayedColor()
{
	return _displayedColor;
}

void CCNodeRGBA::setColor(const ccColor3B& color)
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
}

void CCNodeRGBA::updateDisplayedColor(const ccColor3B& parentColor)
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
}

bool CCNodeRGBA::isCascadeColorEnabled(void)
{
    return _cascadeColorEnabled;
}

void CCNodeRGBA::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    _cascadeColorEnabled = cascadeColorEnabled;
}





NS_CC_END;
//
//  CAView.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-12.
//  Copyright (c) 2014Âπ?http://9miao.com All rights reserved.
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
#include "animation/CAViewAnimation.h"
#include "CADrawingPrimitives.h"

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
, m_obRect(CCRectZero)
, m_sAdditionalTransform(CATransformationMakeIdentity())
, m_pCamera(NULL)
, m_nZOrder(0)
, m_pSuperview(NULL)
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
, m_bIsAnimation(false)
, m_pobBatchView(NULL)
, m_pobImageAtlas(NULL)
{
    m_pActionManager = CAApplication::getApplication()->getActionManager();
    CC_SAFE_RETAIN(m_pActionManager);
    
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;
    memset(&m_sQuad, 0, sizeof(m_sQuad));
    
    CAColor4B tmpColor = CAColor_white;
    m_sQuad.bl.colors = tmpColor;
    m_sQuad.br.colors = tmpColor;
    m_sQuad.tl.colors = tmpColor;
    m_sQuad.tr.colors = tmpColor;
    
    this->setAnchorPoint(CCPoint(0.5f, 0.5f));
    
    //CCLog("CAView = %d\n", ++viewCount);
}

CAView::~CAView(void)
{
    CAScheduler::getScheduler()->pauseTarget(this);
    m_pActionManager->pauseTarget(this);
    
    CC_SAFE_RELEASE(m_pActionManager);
    CC_SAFE_RELEASE(m_pCamera);
    CC_SAFE_RELEASE(m_pShaderProgram);
    
    if(!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
        {
            (*itr)->setSuperview(NULL);
        }
    }
    m_obSubviews.clear();
    CC_SAFE_RELEASE(m_pobImage);
    
    //CCLog("~CAView = %d\n", --viewCount);
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
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setSkewX(newSkewX, this);
    }
    else if (m_fSkewX != newSkewX)
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
    if (CAViewAnimation::areAnimationsEnabled()
         && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setSkewY(newSkewY, this);
    }
    else if (m_fSkewY != newSkewY)
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
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setZOrder(z, this);
    }
    else
    {
        _setZOrder(z);
        if (m_pSuperview)
        {
            m_pSuperview->reorderSubview(this, z);
        }
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
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setVertexZ(var, this);
    }
    else if(m_fVertexZ != var)
    {
        m_fVertexZ = var;
        this->updateDraw();
    }
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
    this->setRotationX(newRotation);
    this->setRotationY(newRotation);
}

float CAView::getRotationX()
{
    return m_fRotationX;
}

void CAView::setRotationX(float fRotationX)
{
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setRotationX(fRotationX, this);
    }
    else if (m_fRotationX != fRotationX)
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
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setRotationY(fRotationY, this);
    }
    else if (m_fRotationY != fRotationY)
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
    this->setScaleX(fScaleX);
    this->setScaleY(fScaleY);
}

/// scaleX getter
float CAView::getScaleX()
{
    return m_fScaleX;
}

/// scaleX setter
void CAView::setScaleX(float newScaleX)
{
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setScaleX(newScaleX, this);
    }
    else if (m_fScaleX != newScaleX)
    {
        m_fScaleX = newScaleX;
        m_obFrameRect.size.width = m_fScaleX * m_obContentSize.width;
        float x = m_obAnchorPointInPoints.x * m_fScaleX;
        m_obFrameRect.origin.x = m_obPoint.x - x;
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
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setScaleY(newScaleY, this);
    }
    else if (m_fScaleY != newScaleY)
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
const CAVector<CAView*>& CAView::getSubviews()
{
    return m_obSubviews;
}

unsigned int CAView::getSubviewsCount(void) const
{
    return (unsigned int)m_obSubviews.size();
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

void CAView::setContentSize(const CCSize & contentSize)
{
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setContentSize(contentSize, this);
    }
    else if (!contentSize.equals(m_obContentSize))
    {
        m_obContentSize = contentSize;
        
        float anchorPointInPointsX = m_obContentSize.width * m_obAnchorPoint.x;
        float anchorPointInPointsY = m_obContentSize.height * m_obAnchorPoint.y;
        m_obAnchorPointInPoints = CCPoint(anchorPointInPointsX, anchorPointInPointsY);
        
        float frameRectWidth = m_obContentSize.width * m_fScaleX;
        float frameRectHeight = m_obContentSize.height * m_fScaleY;
        m_obFrameRect.size = CCSize(frameRectWidth, frameRectHeight);
        
        this->updateImageRect();
        
        if(!m_obSubviews.empty())
        {
            CAVector<CAView*>::iterator itr;
            for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            {
                (*itr)->reViewlayout();
            }
        }
        
        this->updateDraw();
    }
}

const CCRect& CAView::getFrame() const
{
    return m_obFrameRect;
}

void CAView::setFrame(const CCRect &rect)
{
    float width = rect.size.width / m_fScaleX;
    float height = rect.size.height / m_fScaleY;
    CCSize contentSize = CCSize(width, height);
    CCSize originalSize = m_obContentSize;
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setContentSize(contentSize, this);
        
        m_obContentSize = contentSize;
        m_obAnchorPointInPoints = m_obContentSize;
        m_obAnchorPointInPoints.x *= m_obAnchorPoint.x;
        m_obAnchorPointInPoints.y *= m_obAnchorPoint.y;
    }
    else
    {
        this->setContentSize(contentSize);
    }
    
    this->setFrameOrigin(rect.origin);
    
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        m_obContentSize = originalSize;
        m_obAnchorPointInPoints = m_obContentSize;
        m_obAnchorPointInPoints.x *= m_obAnchorPoint.x;
        m_obAnchorPointInPoints.y *= m_obAnchorPoint.y;
    }
}

void CAView::setFrameOrigin(const CCPoint& point)
{
    float x = m_obAnchorPointInPoints.x * m_fScaleX;
    float y = m_obAnchorPointInPoints.y * m_fScaleY;
    
    CCPoint p = CCPoint(x, y);
    p = ccpAdd(p, point);
    
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setPoint(p, this);
        m_bFrame = true;
    }
    else
    {
        this->setPoint(p);
        m_bFrame = true;
    }
}

const CCPoint& CAView::getFrameOrigin()
{
    return m_obFrameRect.origin;
}

CCRect CAView::getCenter()
{
    CCRect rect = this->getFrame();
    rect.origin = ccpAdd(rect.origin, ccpMult(rect.size, 0.5f));
    rect.setCenter(true);
    return rect;
}

void CAView::setCenter(const CCRect& rect)
{
    float width = rect.size.width / m_fScaleX;
    float height = rect.size.height / m_fScaleY;
    CCSize contentSize = CCSize(width, height);
    CCSize originalSize = m_obContentSize;
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setContentSize(contentSize, this);
        
        m_obContentSize = contentSize;
        m_obAnchorPointInPoints = m_obContentSize;
        m_obAnchorPointInPoints.x *= m_obAnchorPoint.x;
        m_obAnchorPointInPoints.y *= m_obAnchorPoint.y;
    }
    else
    {
        this->setContentSize(contentSize);
    }
    
    this->setCenterOrigin(rect.origin);
    
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        m_obContentSize = originalSize;
        m_obAnchorPointInPoints = m_obContentSize;
        m_obAnchorPointInPoints.x *= m_obAnchorPoint.x;
        m_obAnchorPointInPoints.y *= m_obAnchorPoint.y;
    }
}

CCPoint CAView::getCenterOrigin()
{
    return this->getCenter().origin;
}

void CAView::setCenterOrigin(const CCPoint& point)
{
    CCPoint p = ccpMult(m_obContentSize, 0.5f);
    p = ccpSub(p, m_obAnchorPointInPoints);
    p = CCPoint(p.x * m_fScaleX, p.y * m_fScaleY);
    p = ccpSub(point, p);
    
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setPoint(p, this);
        m_bFrame = false;
    }
    else
    {
        this->setPoint(p);
        m_bFrame = false;
    }
}

CCRect CAView::getBounds() const
{
    CCRect rect = CCRectZero;
    rect.size = m_obContentSize;
    return rect;
}

void CAView::setBounds(const CCRect& rect)
{
    if (!rect.size.equals(CCSizeZero))
    {
        this->setContentSize(rect.size);
    }
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

ccGLServerState CAView::getGLServerState()
{
    return m_eGLServerState;
}

void CAView::setGLServerState(ccGLServerState glServerState)
{
    m_eGLServerState = glServerState;
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
    CAView* v = this->getSuperview();
    CC_RETURN_IF(v == NULL);
    while (v == v->getSuperview())
    {
        CC_BREAK_IF(v == NULL);
        CC_RETURN_IF(v->isVisible());
    }
    
    SET_DIRTY_RECURSIVELY();
    this->reViewlayout();
    CAApplication::getApplication()->updateDraw();
}

CAView* CAView::getSubviewByTag(int aTag)
{
    CCAssert( aTag != kCAObjectTagInvalid, "Invalid tag");
    
    if(!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
        {
            if ((*itr)->m_nTag == aTag)
            {
                return *itr;
            }
        }
    }
    return NULL;
}

CAView* CAView::getSubviewByTextTag(const std::string& textTag)
{
    CCAssert( !textTag.empty(), "Invalid tag");
    
    if(!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
        {
            if ((*itr)->m_sTextTag.compare(textTag) == 0)
            {
                return *itr;
            }
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
        m_pobBatchView->appendSubview(subview);
        
        if (!m_bReorderChildDirty)
        {
            setReorderChildDirtyRecursively();
        }
    }
    
    m_bReorderChildDirty = true;
    m_obSubviews.pushBack(subview);
    subview->_setZOrder(z);
    
    subview->setSuperview(this);
    subview->setOrderOfArrival(s_globalOrderOfArrival++);
    
    if( m_bRunning )
    {
        subview->onEnter();
        subview->onEnterTransitionDidFinish();
    }
    
    this->updateDraw();
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
    if (m_pobBatchView)
    {
        m_pobBatchView->removeViewFromAtlas(subview);
    }
    
    if (m_obSubviews.contains(subview))
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
        CCLOG("CrossApp: removeSubviewByTag(tag = %d): child not found!", tag);
    }
    else
    {
        this->removeSubview(subview);
    }
}

void CAView::removeSubviewByTextTag(const std::string& textTag)
{
    CCAssert( !textTag.empty(), "Invalid tag");
    
    CAView *subview = this->getSubviewByTextTag(textTag);
    
    if (subview == NULL)
    {
        CCLOG("CrossApp: removeSubviewByTextTag(textTag = %s): child not found!", textTag.c_str());
    }
    else
    {
        this->removeSubview(subview);
    }
}

void CAView::removeAllSubviews()
{
    // not using detachSubview improves speed here
    if (!m_obSubviews.empty())
    {
        if (m_pobBatchView)
        {
            CAVector<CAView*>::iterator itr;
            for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            {
                m_pobBatchView->removeViewFromAtlas(*itr);
            }
        }

        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
        {
            if(m_bRunning)
            {
                (*itr)->onExitTransitionDidStart();
                (*itr)->onExit();
            }
            
            (*itr)->setSuperview(NULL);
        }
        
        m_obSubviews.clear();
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
    
    m_obSubviews.eraseObject(subview);
    
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
    
    CCAssert( subview != NULL, "Subview must be non-nil");
    m_bReorderChildDirty = true;
    subview->setOrderOfArrival(s_globalOrderOfArrival++);
    subview->_setZOrder(zOrder);
}

void CAView::sortAllSubviews()
{
    if (m_bReorderChildDirty && !m_obSubviews.empty())
    {
        std::sort(m_obSubviews.begin(), m_obSubviews.end(), compareSubviewZOrder);
    
        if (m_pobBatchView)
        {
            CAVector<CAView*>::iterator itr;
            for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
                if(m_bRunning) (*itr)->sortAllSubviews();
        }
        
        m_bReorderChildDirty = false;
    }
}

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
    
#endif
    
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          kQuadSize,
                          (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          kQuadSize,
                          (void*) (offset + diff));
    
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color,
                          4,
                          GL_UNSIGNED_BYTE,
                          GL_TRUE,
                          kQuadSize,
                          (void*)(offset + diff));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    CHECK_GL_ERROR_DEBUG();
    
#if CC_SPRITE_DEBUG_DRAW
    // draw bounding box
    CCPoint vertices[4]=
    {
        CCPoint(m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y),
        CCPoint(m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y),
        CCPoint(m_sQuad.br.vertices.x, m_sQuad.br.vertices.y),
        CCPoint(m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y),
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CAView - draw");
}

void CAView::visit()
{
    CC_RETURN_IF(!m_bVisible);
    
    kmGLPushMatrix();

    this->transform();
    
    bool isScissor = (bool)glIsEnabled(GL_SCISSOR_TEST);
    CCRect restoreScissorRect = CCRectZero;
    if (isScissor)
    {
        GLfloat params[4];
        glGetFloatv(GL_SCISSOR_BOX, params);
        restoreScissorRect = CCRect(params[0], params[1], params[2], params[3]);
    }

    
    if (!m_bDisplayRange)
    {
        kmMat4 modelview;
        kmGLGetMatrix(KM_GL_MODELVIEW, &modelview);
        kmMat4 tm;
        kmMat4Identity(&tm);
        tm.mat[12] = m_obContentSize.width;
        tm.mat[13] = m_obContentSize.height;
        kmMat4 tm2;
        kmMat4Multiply(&tm2, &modelview, &tm);

        CCSize winSize = CAApplication::getApplication()->getWinSize();
        CCPoint point = CCPoint(modelview.mat[12], modelview.mat[13]) + winSize/2;
        CCSize size = CCSize(tm2.mat[12] - modelview.mat[12], tm2.mat[13] - modelview.mat[13]);

        CCRect frame = CCRect(point.x + 0.5f, point.y, size.width + 1.0f, size.height + 1.0f);
        
        if (isScissor)
        {
            //bug fix by alexviolent ,support designresolutionsize.
            CCEGLView* eglView = CAApplication::getApplication()->getOpenGLView();
            frame.origin.x*=eglView->getScaleX();
            frame.origin.y*=eglView->getScaleY();
            frame.size.width*=eglView->getScaleX();
            frame.size.height*=eglView->getScaleY();
            float x1 = MAX(frame.getMinX(), restoreScissorRect.getMinX());
            float y1 = MAX(frame.getMinY(), restoreScissorRect.getMinY());
            float x2 = MIN(frame.getMaxX(), restoreScissorRect.getMaxX());
            float y2 = MIN(frame.getMaxY(), restoreScissorRect.getMaxY());
            float width = MAX(x2-x1, 0);
            float height = MAX(y2-y1, 0);
            
            glScissor(x1, y1, width, height);
        }
        else
        {
            glEnable(GL_SCISSOR_TEST);
            glScissor(frame.origin.x,
                      frame.origin.y,
                      frame.size.width,
                      frame.size.height);
        }
    }

    this->sortAllSubviews();
    
    CAVector<CAView*>::iterator itr=m_obSubviews.begin();
    while (itr!=m_obSubviews.end())
    {
        CC_BREAK_IF((*itr)->m_nZOrder >= 0);
        (*itr)->visit();
        itr++;
    }
    
    this->draw();
    
    while (itr!=m_obSubviews.end())
    {
        (*itr)->visit();
        itr++;
    }
    
    //m_uOrderOfArrival = 0;
    
    if (!m_bDisplayRange)
    {
        if (isScissor)
        {
            glScissor(restoreScissorRect.origin.x,
                      restoreScissorRect.origin.y,
                      restoreScissorRect.size.width,
                      restoreScissorRect.size.height);
        }
        else
        {
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
            kmGLTranslatef(RENDER_IN_SUBPIXEL(anchorPointInPoints.x),
                           RENDER_IN_SUBPIXEL(anchorPointInPoints.y),
                           0 );
        
        m_pCamera->locate();
        
        if( translate )
            kmGLTranslatef(RENDER_IN_SUBPIXEL(-anchorPointInPoints.x),
                           RENDER_IN_SUBPIXEL(-anchorPointInPoints.y),
                           0 );
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
    if (!m_bHaveNextResponder)
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
    if (!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            (*itr)->onEnter();
    }
    
    m_bRunning = true;
}

void CAView::onEnterTransitionDidFinish()
{
    if (!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            (*itr)->onEnterTransitionDidFinish();
    }
    
    if (m_pViewDelegate)
    {
        m_pViewDelegate->getSuperViewRect(this->getSuperview()->getBounds());
        m_pViewDelegate->viewOnEnterTransitionDidFinish();
    }
}

void CAView::onExitTransitionDidStart()
{
    CAViewAnimation::removeAnimationsWithView(this);
    if (!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            (*itr)->onExitTransitionDidStart();
    }
    
    if (m_pViewDelegate)
    {
        m_pViewDelegate->viewOnExitTransitionDidStart();
    }
}

void CAView::onExit()
{
    m_bRunning = false;
    
    if (!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            (*itr)->onExit();
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
        m_sTransform = CATransformationMake(cy * m_fScaleX,
                                            sy * m_fScaleX,
                                            -sx * m_fScaleY,
                                            cx * m_fScaleY,
                                            x,
                                            y );
        
        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needsSkewMatrix)
        {
            CATransformation skewMatrix = CATransformationMake(1.0f,
                                                               tanf(CC_DEGREES_TO_RADIANS(m_fSkewY)),
                                                               tanf(CC_DEGREES_TO_RADIANS(m_fSkewX)),
                                                               1.0f,
                                                               0.0f,
                                                               0.0f );
            
            m_sTransform = CATransformationConcat(skewMatrix, m_sTransform);
            
            // adjust anchor point
            CCPoint anchorPointInPoints = CCPoint(m_obAnchorPointInPoints.x,
                                                  m_obContentSize.height - m_obAnchorPointInPoints.y);
            
            if (!anchorPointInPoints.equals(CCPointZero))
            {
                m_sTransform = CATransformationTranslate(m_sTransform,
                                                         -anchorPointInPoints.x,
                                                         -anchorPointInPoints.y);
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
    if ( m_bInverseDirty )
    {
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
    ret.origin = this->convertToNodeSpace(ret.origin);
    ret.size = CCSizeApplyAffineTransform(ret.size, nodeToParentTransform());
    return ret;
}

CCRect CAView::convertRectToWorldSpace(const CrossApp::CCRect &nodeRect)
{
    CCRect ret = nodeRect;
    ret.origin = this->convertToWorldSpace(ret.origin);
    ret.size = CCSizeApplyAffineTransform(ret.size, worldToNodeTransform());
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
            
            if( !m_pSuperview || m_pSuperview == m_pobBatchView)
            {
                m_transformToBatch = nodeToParentTransform();
            }
            else
            {
                m_transformToBatch = CATransformationConcat( nodeToParentTransform() , m_pSuperview->m_transformToBatch );
            }

            CCSize size = m_obContentSize;
            
            float x1 = 0;
            float y1 = 0;
            
            float x = m_transformToBatch.tx;
            float y = m_transformToBatch.ty;
            
            float cr = m_transformToBatch.a;
            float sr = m_transformToBatch.b;
            float cr2 = m_transformToBatch.d;
            float sr2 = -m_transformToBatch.c;
            
            x1 = x1 * cr - y1 * sr2 + x;
            y1 = x1 * sr + y1 * cr2 + y;
            
            x1 = RENDER_IN_SUBPIXEL(x1);
            y1 = RENDER_IN_SUBPIXEL(y1);
            
            float x2 = x1 + size.width;
            float y2 = y1 + size.height;

            m_sQuad.bl.vertices = vertex3( x1, y1, m_fVertexZ );
            m_sQuad.br.vertices = vertex3( x2, y1, m_fVertexZ );
            m_sQuad.tl.vertices = vertex3( x1, y2, m_fVertexZ );
            m_sQuad.tr.vertices = vertex3( x2, y2, m_fVertexZ );
        }
        
        if (m_pobImageAtlas)
		{
            m_pobImageAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
        }
        
        m_bRecursiveDirty = false;
        setDirty(false);
    }
    
    if (!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            (*itr)->updateTransform();
    }
    
#if CC_SPRITE_DEBUG_DRAW
    // draw bounding box
    CCPoint vertices[4] =
    {
        CCPoint( m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y ),
        CCPoint( m_sQuad.br.vertices.x, m_sQuad.br.vertices.y ),
        CCPoint( m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y ),
        CCPoint( m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y ),
    };
    ccDrawPoly(vertices, 4, true);
#endif

}

bool CAView::isDisplayRange()
{
    return m_bDisplayRange;
}

void CAView::setDisplayRange(bool value)
{
    m_bDisplayRange = value;
}


void CAView::setImage(CAImage* image)
{
    if (!m_pobBatchView)
    {
        CC_SAFE_RETAIN(image);
        CC_SAFE_RELEASE(m_pobImage);
        m_pobImage = image;
        if (image)
        {
            if (image->getPixelFormat() == CAImage::PixelFormat_A8)
            {
                this->setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureA8Color));
            }
            else
            {
                this->setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
            }
        }
        
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
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setImageRect(rect, this);
    }
    else
    {
        m_bRectRotated = false;
        
        this->setVertexRect(rect);
        this->setImageCoords(rect);

        if (m_pobBatchView)
        {
            setDirty(true);
        }
        else
        {
            this->updateImageRect();
        }
    }
}

void CAView::updateImageRect()
{
    // Don't update Z.
    
    GLfloat x1,x2,y1,y2;
    x1 = 0;
    y1 = 0;
    x2 = m_obContentSize.width;
    y2 = m_obContentSize.height;
    
    m_sQuad.bl.vertices = vertex3(x1, y1, m_fVertexZ);
    m_sQuad.br.vertices = vertex3(x2, y1, m_fVertexZ);
    m_sQuad.tl.vertices = vertex3(x1, y2, m_fVertexZ);
    m_sQuad.tr.vertices = vertex3(x2, y2, m_fVertexZ);
}

// override this method to generate "double scale" sprites
void CAView::setVertexRect(const CCRect& rect)
{
    m_obRect = rect;
}

void CAView::setImageCoords(CCRect rect)
{
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
        
        left   = (2 * rect.origin.x + 1) / (2 * atlasWidth);
        right  = left + (rect.size.height * 2 - 2) / (2 * atlasWidth);
        top    = (2 * rect.origin.y + 1) / (2 * atlasHeight);
        bottom = top + (rect.size.width * 2 - 2) / (2 * atlasHeight);
        
#else
        
        left   = rect.origin.x / atlasWidth;
        right  = (rect.origin.x + rect.size.height) / atlasWidth;
        top    = rect.origin.y / atlasHeight;
        bottom = (rect.origin.y + rect.size.width) / atlasHeight;
        
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
        left   = (2 * rect.origin.x + 1) / (2 * atlasWidth);
        right  = left + (rect.size.width * 2 - 2) / (2 * atlasWidth);
        top    = (2 * rect.origin.y + 1) / (2 * atlasHeight);
        bottom = top + (rect.size.height * 2 - 2) / (2 * atlasHeight);
#else
        left   = rect.origin.x / atlasWidth;
        right  = (rect.origin.x + rect.size.width) / atlasWidth;
        top    = rect.origin.y / atlasHeight;
        bottom = (rect.origin.y + rect.size.height) / atlasHeight;
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        
        if(m_bFlipX)
        {
            CC_SWAP(left, right, float);
        }
        
        if(m_bFlipY)
        {
            CC_SWAP(top, bottom, float);
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
    
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setAlpha(alpha, this);
    }
    else if (_displayedAlpha != alpha)
    {
        _realAlpha = alpha;
        
        float superviewAlpha = m_pSuperview ? m_pSuperview->getDisplayedAlpha() : 1.0f;
        
        this->updateDisplayedAlpha(superviewAlpha);
    }
}

void CAView::updateDisplayedAlpha(float parentAlpha)
{
	_displayedAlpha = _realAlpha * parentAlpha;
	
    if (!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            (*itr)->updateDisplayedAlpha(_displayedAlpha);
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
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations())
    {
        CAViewAnimation::getInstance()->setColor(color, this);
    }
    else
    {
        _displayedColor = _realColor = color;
        this->updateColor();
    }
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
    color4.a = color4.a * _displayedAlpha;
    
    if (m_pobImage)
    {
        if (m_pobImage->getPixelFormat() == CAImage::PixelFormat_RGBA8888
            ||
            m_pobImage->getPixelFormat() == CAImage::PixelFormat_RGBA4444)
        {
           color4.r = color4.r * _displayedAlpha;
           color4.g = color4.g * _displayedAlpha;
           color4.b = color4.b * _displayedAlpha;
        }
    }
   
    m_sQuad.bl.colors = color4;
    m_sQuad.br.colors = color4;
    m_sQuad.tl.colors = color4;
    m_sQuad.tr.colors = color4;
    
    if (m_pobBatchView && m_pobImage)
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
        if (!m_obSubviews.empty())
        {
            CAVector<CAView*>::iterator itr;
            for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
                (*itr)->setDirtyRecursively(bValue);
        }
    }
}

void CAView::setFlipX(bool bFlipX)
{
    if (m_bFlipX != bFlipX)
    {
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setFlipX(bFlipX, this);
        }
        else
        {
            m_bFlipX = bFlipX;
            if (m_pobImage)
            {
                setImageRect(m_obRect);
            }
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
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setFlipY(bFlipY, this);
        }
        else
        {
            m_bFlipY = bFlipY;
            if (m_pobImage)
            {
                setImageRect(m_obRect);
            }
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
        
        this->updateImageRect();
    }
    else
    {
        // using batch
        m_transformToBatch = CATransformationIdentity;
        setImageAtlas(m_pobBatchView->getImageAtlas()); // weak ref
    }
}

NS_CC_END;

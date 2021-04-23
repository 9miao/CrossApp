//
//  CAView.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-12.
//  Copyright (c) 2014Âπ?http://9miao.com All rights reserved.
//

#include "CAView.h"
#include <stdarg.h>
#include "basics/CAPointExtension.h"
#include "math/TransformUtils.h"
#include "basics/CACamera.h"
#include "basics/CAApplication.h"
#include "basics/CAScheduler.h"
#include "dispatcher/CATouch.h"
#include "dispatcher/CATouchDispatcher.h"
#include "dispatcher/CAKeypadDispatcher.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCRenderState.h"
#include "CCEGLView.h"
#include "cocoa/CCSet.h"
#include "CAImageView.h"
#include "animation/CAViewAnimation.h"
#include "CADrawingPrimitives.h"
#include "platform/CADensityDpi.h"
#include "ccMacros.h"
#include "script_support/CAScriptSupport.h"

NS_CC_BEGIN;


//static int viewCount = 0;

static int s_globalOrderOfArrival = 1;

//static std::map<int, CAView*> s_gViews;

CAView::CAView(void)
: m_fRotationX(0.0f)
, m_fRotationY(0.0f)
, m_fRotationZ(0.0f)
, m_fScaleX(1.0f)
, m_fScaleY(1.0f)
, m_fScaleZ(1.0f)
, m_fSkewX(0.0f)
, m_fSkewY(0.0f)
, m_obAnchorPointInPoints(DPOINT_FLT_MIN)
, m_obAnchorPoint(DPOINT_FLT_MIN)
, m_obContentSize(DSizeZero)
, m_obPoint(DPOINT_FLT_MIN)
, m_fPointZ(0.0f)
, m_obRect(DRectZero)
, m_bTransformDirty(true)
, m_bInverseDirty(true)
, m_pAdditionalTransform(nullptr)
, m_bAdditionalTransformDirty(false)
, m_bTransformUpdated(true)
, m_nZOrder(0)
, m_pSuperview(nullptr)
, m_pGlProgramState(nullptr)
, m_uOrderOfArrival(0)
, m_bRunning(false)
, m_bVisible(true)
, m_bReorderSubviewDirty(false)
, _displayedAlpha(1.0f)
, _realAlpha(1.0f)
, _displayedColor(CAColor4B::WHITE)
, _realColor(CAColor4B::WHITE)
, m_bOpacityModifyRGB(true)
, m_bDisplayRange(true)
, m_bScissorRestored(false)
, m_pobImage(nullptr)
, m_bRectRotated(false)
, m_bFlipX(false)
, m_bFlipY(false)
, m_pContentContainer(nullptr)
, m_bIsAnimation(false)
, m_pLeftShadow(nullptr)
, m_pRightShadow(nullptr)
, m_pTopShadow(nullptr)
, m_pBottomShadow(nullptr)
, m_pLeftTopShadow(nullptr)
, m_pRightTopShadow(nullptr)
, m_pLeftBottomShadow(nullptr)
, m_pRightBottomShadow(nullptr)
, m_obLayout(DLayoutZero)
, m_eLayoutType(0)
, m_iCameraMask(1)
, m_pApplication(CAApplication::getApplication())
, m_obOnEnterCallback(nullptr)
, m_obOnEnterTransitionDidFinishCallback(nullptr)
, m_obOnExitCallback(nullptr)
, m_obOnExitTransitionDidStartCallback(nullptr)
{
    CAViewAnimation::removeAnimationsWithView(this);
    
    m_sBlendFunc = BlendFunc_alpha_non_premultiplied;
    memset(&m_sQuad, 0, sizeof(m_sQuad));
    
    CAColor4B tmpColor = CAColor4B::WHITE;
    m_sQuad.bl.colors = tmpColor;
    m_sQuad.br.colors = tmpColor;
    m_sQuad.tl.colors = tmpColor;
    m_sQuad.tr.colors = tmpColor;
    
    m_tTransform = m_tInverse = Mat4::IDENTITY;

    this->updateRotationQuat();
    this->setAnchorPoint(DPoint(0.5f, 0.5f));
    
    //s_gViews[m_u__ID] = this;
    
    //CCLog("CAView = %lu\n", s_gViews.size());
}

CAView::~CAView(void)
{
    CC_SAFE_DELETE(m_pLeftShadow);
    CC_SAFE_DELETE(m_pRightShadow);
    CC_SAFE_DELETE(m_pTopShadow);
    CC_SAFE_DELETE(m_pBottomShadow);
    CC_SAFE_DELETE(m_pLeftTopShadow);
    CC_SAFE_DELETE(m_pRightTopShadow);
    CC_SAFE_DELETE(m_pLeftBottomShadow);
    CC_SAFE_DELETE(m_pRightBottomShadow);
    
    CC_SAFE_RELEASE_NULL(m_pGlProgramState);
    CAScheduler::getScheduler()->pauseTarget(this);

    if (!m_obSubviews.empty())
    {
        for (auto& subview : m_obSubviews)
        {
            subview->setSuperview(nullptr);
        }
        m_obSubviews.clear();
    }
    
    if (m_pAdditionalTransform)
    {
        delete [] m_pAdditionalTransform;
    }
    
    CC_SAFE_RELEASE(m_pobImage);

    CAViewAnimation::removeAnimationsWithView(this);
    //s_gViews.erase(m_u__ID);
    
    //CCLog("~CAView = %lu\n", s_gViews.size());
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
    this->setImage(CAImage::WHITE_IMAGE());
    return true;
}

CAView* CAView::createWithFrame(const DRect& rect)
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

CAView* CAView::createWithFrame(const DRect& rect, const CAColor4B& color4B)
{
	CAView * pRet = new CAView();
    if (pRet && pRet->initWithFrame(rect))
    {
        pRet->setColor(color4B);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

CAView* CAView::createWithCenter(const DRect& rect)
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

CAView* CAView::createWithCenter(const DRect& rect, const CAColor4B& color4B)
{
    CAView * pRet = new CAView();
    if (pRet && pRet->initWithCenter(rect))
    {
        pRet->setColor(color4B);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

CAView* CAView::createWithLayout(const DLayout& layout)
{
    CAView * pRet = new CAView();
    if (pRet && pRet->initWithLayout(layout))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CAView* CAView::createWithLayout(const DLayout& layout, const CAColor4B& color4B)
{
    CAView * pRet = new CAView();
    if (pRet && pRet->initWithLayout(layout))
    {
        pRet->setColor(color4B);
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

bool CAView::initWithFrame(const DRect& rect)
{
    if (!this->init())
    {
        return false;
    }
    this->setFrame(rect);
    
    return true;
}

bool CAView::initWithCenter(const DRect& rect)
{
    if (!this->init())
    {
        return false;
    }
    this->setCenter(rect);
    
    return true;
}

bool CAView::initWithLayout(const CrossApp::DLayout &layout)
{
    if (!this->init())
    {
        return false;
    }
    this->setLayout(layout);
    
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
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setSkewX(newSkewX, this);
        }
        else
        {
            m_fSkewX = newSkewX;
            this->updateDraw();
        }
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
        && CAViewAnimation::areBeginAnimations()
        && m_nZOrder != z)
    {
        CAViewAnimation::getInstance()->setZOrder(z, this);
    }
    else
    {
        if (m_pSuperview)
        {
            m_pSuperview->reorderSubview(this, z);
        }
        else
        {
            this->_setZOrder(z);
        }
    }
}

/// PointZ getter
float CAView::getPointZ()
{
    return m_fPointZ;
}


/// PointZ setter
void CAView::setPointZ(float var)
{
    if(m_fPointZ != var)
    {
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setPointZ(var, this);
        }
        else
        {
            m_fPointZ = var;
            this->updateDraw();
        }
    }
}


/// rotation getter
int CAView::getRotation()
{
    return m_fRotationZ;
}

/// rotation setter
void CAView::setRotation(int newRotation)
{
    if (m_fRotationZ != newRotation)
    {
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setRotation(newRotation, this);
        }
        else
        {
            m_fRotationZ = newRotation;
            this->updateRotationQuat();
            this->updateDraw();
        }
    }
}

int CAView::getRotationX()
{
    return m_fRotationX;
}

void CAView::setRotationX(int fRotationX)
{
    if (m_fRotationX != fRotationX)
    {
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setRotationX(fRotationX, this);
        }
        else
        {
            m_fRotationX = fRotationX;
            this->updateRotationQuat();
            this->updateDraw();
        }
    }
}

int CAView::getRotationY()
{
    return m_fRotationY;
}

void CAView::setRotationY(int fRotationY)
{
    if (m_fRotationY != fRotationY)
    {
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setRotationY(fRotationY, this);
        }
        else
        {
            m_fRotationY = fRotationY;
            this->updateRotationQuat();
            this->updateDraw();
        }
    }
}

void CAView::updateRotationQuat()
{
    // convert Euler angle to quaternion
    // when m_fRotationZ_X == m_fRotationZ_Y, m_fRotationQuat = RotationZ_X * RotationY * RotationX
    // when m_fRotationZ_X != m_fRotationZ_Y, m_fRotationQuat = RotationY * RotationX
    float halfRadx = CC_DEGREES_TO_RADIANS(m_fRotationX / 2.f);
    float halfRady = CC_DEGREES_TO_RADIANS(m_fRotationY / 2.f);
    float halfRadz = -CC_DEGREES_TO_RADIANS(m_fRotationZ / 2.f);
    
    float coshalfRadx = cosf(halfRadx), sinhalfRadx = sinf(halfRadx), coshalfRady = cosf(halfRady), sinhalfRady = sinf(halfRady), coshalfRadz = cosf(halfRadz), sinhalfRadz = sinf(halfRadz);
    
    m_obRotationQuat.x = sinhalfRadx * coshalfRady * coshalfRadz - coshalfRadx * sinhalfRady * sinhalfRadz;
    m_obRotationQuat.y = coshalfRadx * sinhalfRady * coshalfRadz + sinhalfRadx * coshalfRady * sinhalfRadz;
    m_obRotationQuat.z = coshalfRadx * coshalfRady * sinhalfRadz - sinhalfRadx * sinhalfRady * coshalfRadz;
    m_obRotationQuat.w = coshalfRadx * coshalfRady * coshalfRadz + sinhalfRadx * sinhalfRady * sinhalfRadz;
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
void CAView::setScale(float fScaleX, float fScaleY)
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
    if (m_fScaleX != newScaleX)
    {
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setScaleX(newScaleX, this);
        }
        else
        {
            m_fScaleX = newScaleX;
            this->updateDraw();
        }
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
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setScaleY(newScaleY, this);
        }
        else
        {
            m_fScaleY = newScaleY;
            this->updateDraw();
        }
    }
}

/// position setter
void CAView::setPoint(const DPoint& newPoint)
{
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations()
        && !m_obPoint.equals(newPoint))
    {
        CAViewAnimation::getInstance()->setPoint(newPoint, this);
    }
    else
    {
        m_obPoint = newPoint;
        this->updateDraw();
    }
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

const DPoint& CAView::getAnchorPointInPoints()
{
    return m_obAnchorPointInPoints;
}

/// anchorPoint getter
const DPoint& CAView::getAnchorPoint()
{
    return m_obAnchorPoint;
}

void CAView::setAnchorPointInPoints(const DPoint& anchorPointInPoints)
{
    if (!m_obContentSize.equals(DPointZero))
    {
        DPoint anchorPoint = DPoint(anchorPointInPoints.x / m_obContentSize.width,
                                    anchorPointInPoints.y / m_obContentSize.height);

        if (m_bRunning)
        {
            this->getViewToSuperviewTransform();
        }
        
        m_obAnchorPoint = anchorPoint;
        m_obAnchorPointInPoints = anchorPointInPoints;
        
        if (m_bRunning)
        {
            DPoint point = anchorPointInPoints;
            point.y = m_obContentSize.height - point.y;
            point = PointApplyAffineTransform(point, this->getViewToSuperviewAffineTransform());
            point.y = this->m_pSuperview->m_obContentSize.height - point.y;
            this->setPoint(point);
            this->updateDraw();
        }
    }
}

void CAView::setAnchorPoint(const DPoint& anchorPoint)
{
    if( ! anchorPoint.equals(m_obAnchorPoint))
    {
        DPoint anchorPointInPoints = ccpCompMult(m_obContentSize, anchorPoint);
        
        if (m_bRunning)
        {
            this->getViewToSuperviewTransform();
        }
        
        m_obAnchorPoint = anchorPoint;
        m_obAnchorPointInPoints = anchorPointInPoints;
        
        if (m_bRunning)
        {
            DPoint point = anchorPointInPoints;
            point.y = m_obContentSize.height - point.y;
            point = PointApplyAffineTransform(point, this->getViewToSuperviewAffineTransform());
            if (m_pSuperview)
            {
                point.y = m_pSuperview->m_obContentSize.height - point.y;
            }
            else
            {
                point.y = m_pApplication->getWinSize().height - point.y;
            }
            this->setPoint(point);
            this->updateDraw();
        }
    }
}

void CAView::setContentSize(const DSize & contentSize)
{
    if (!contentSize.equals(m_obContentSize))
    {
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setContentSize(contentSize, this);
        }
        else
        {
            m_obContentSize = contentSize;
            m_obAnchorPointInPoints.x = m_obContentSize.width * m_obAnchorPoint.x;
            m_obAnchorPointInPoints.y = m_obContentSize.height * m_obAnchorPoint.y;
            
            this->updateImageRect();
            if (m_pContentContainer)
            {
                m_pContentContainer->viewOnSizeTransitionDidChanged();
            }
            
            CAVector<CAView*>::iterator itr;
            for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            {
                (*itr)->reViewlayout(m_obContentSize);
            }
            
            this->updateDraw();
        }
    }
}

const DRect& CAView::getFrame()
{
    m_obReturn.setType(DRect::Type::Frame);
    m_obReturn.origin = ccpSub(m_obPoint, m_obAnchorPointInPoints);
    m_obReturn.size = m_obContentSize;
    return m_obReturn;
}

void CAView::setFrame(const DRect &rect)
{
    m_obLayout = DLayoutZero;
    DSize originalSize = m_obContentSize;
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations()
        && !m_obContentSize.equals(rect.size))
    {
        CAViewAnimation::getInstance()->setContentSize(rect.size, this);
        
        m_obContentSize = rect.size;
        m_obAnchorPointInPoints = m_obContentSize;
        m_obAnchorPointInPoints.x *= m_obAnchorPoint.x;
        m_obAnchorPointInPoints.y *= m_obAnchorPoint.y;
    }
    else
    {
        this->setContentSize(rect.size);
    }
    
    this->setFrameOrigin(rect.origin);
    
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations()
        && !m_obContentSize.equals(rect.size))
    {
        m_obContentSize = originalSize;
        m_obAnchorPointInPoints = m_obContentSize;
        m_obAnchorPointInPoints.x *= m_obAnchorPoint.x;
        m_obAnchorPointInPoints.y *= m_obAnchorPoint.y;
    }
}

const DPoint& CAView::getFrameOrigin()
{
    m_obReturn.origin = ccpSub(m_obPoint, m_obAnchorPointInPoints);
    return m_obReturn.origin;
}

void CAView::setFrameOrigin(const DPoint& point)
{
    m_obLayout = DLayoutZero;
    DPoint p = ccpAdd(point, m_obAnchorPointInPoints);
    this->setPoint(p);
    m_eLayoutType = 0;
}

const DRect& CAView::getCenter()
{
    m_obReturn.setType(DRect::Type::Center);
    m_obReturn.origin = ccpAdd(ccpSub(m_obPoint, m_obAnchorPointInPoints),
                           ccpMult(m_obContentSize, 0.5f));
    m_obReturn.size = m_obContentSize;
    return m_obReturn;
}

void CAView::setCenter(const DRect& rect)
{
    m_obLayout = DLayoutZero;
    DSize originalSize = m_obContentSize;
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations()
        && !m_obContentSize.equals(rect.size))
    {
        CAViewAnimation::getInstance()->setContentSize(rect.size, this);
        
        m_obContentSize = rect.size;
        m_obAnchorPointInPoints = m_obContentSize;
        m_obAnchorPointInPoints.x *= m_obAnchorPoint.x;
        m_obAnchorPointInPoints.y *= m_obAnchorPoint.y;
    }
    else
    {
        this->setContentSize(rect.size);
    }
    
    this->setCenterOrigin(rect.origin);
    
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations()
        && !m_obContentSize.equals(rect.size))
    {
        m_obContentSize = originalSize;
        m_obAnchorPointInPoints = m_obContentSize;
        m_obAnchorPointInPoints.x *= m_obAnchorPoint.x;
        m_obAnchorPointInPoints.y *= m_obAnchorPoint.y;
    }
}

const DPoint& CAView::getCenterOrigin()
{
    m_obReturn.origin = ccpAdd(ccpSub(m_obPoint, m_obAnchorPointInPoints),
                  ccpMult(m_obContentSize, 0.5f));
    return m_obReturn.origin;
}

void CAView::setCenterOrigin(const DPoint& point)
{
    m_obLayout = DLayoutZero;
    DPoint p = ccpSub(point, ccpMult(m_obContentSize, 0.5f));
    p = ccpAdd(p, m_obAnchorPointInPoints);

    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations()
        && !m_obPoint.equals(p))
    {
        CAViewAnimation::getInstance()->setPoint(p, this);
    }
    else
    {
        this->setPoint(p);
    }
    m_eLayoutType = 1;
}

const DRect& CAView::getBounds()
{
    m_obReturn.setType(DRect::Type::Frame);
    m_obReturn.origin = DPointZero;
    m_obReturn.size = m_obContentSize;
    return m_obReturn;
}

void CAView::setBounds(const DRect& rect)
{
    if (!rect.size.equals(DSizeZero))
    {
        this->setContentSize(rect.size);
    }
}

void CAView::setLayout(const CrossApp::DLayout &layout)
{
    m_obLayout = layout;
    m_eLayoutType = 2;
    
    if (m_bRunning)
    {
        if (m_pSuperview)
        {
            this->reViewlayout(m_pSuperview->m_obContentSize, true);
        }
    }
}

const DLayout& CAView::getLayout()
{
    return m_obLayout;
}

// isRunning getter
bool CAView::isRunning()
{
    return m_bRunning;
}

/// Superview getter
CAView * CAView::getSuperview()
{
    return m_pSuperview;
}
/// Superview setter
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

GLProgram* CAView::getShaderProgram()
{
    return m_pGlProgramState ? m_pGlProgramState->getGLProgram() : nullptr;
}

void CAView::setShaderProgram(GLProgram *pShaderProgram)
{
    if (m_pGlProgramState == nullptr || (m_pGlProgramState && m_pGlProgramState->getGLProgram() != pShaderProgram))
    {
        this->setGLProgramState(GLProgramState::getOrCreateWithGLProgram(pShaderProgram));
    }
}

GLProgramState* CAView::getGLProgramState()
{
    return m_pGlProgramState;
}

void CAView::setGLProgramState(GLProgramState* glProgramState)
{
    if (glProgramState != m_pGlProgramState)
    {
        CC_SAFE_RETAIN(glProgramState);
        CC_SAFE_RELEASE(m_pGlProgramState);
        m_pGlProgramState = glProgramState;
        
        
        if (m_pGlProgramState)
            m_pGlProgramState->setBinding(this);
    }
}

const char* CAView::description()
{
    return crossapp_format_string("<CAView | TextTag = %s | Tag = %d >", m_sTextTag.c_str(), m_nTag).c_str();
}

void CAView::reViewlayout(const DSize& contentSize, bool allowAnimation)
{
    if (m_eLayoutType == 2)
    {
        DPoint point;
        DSize size;
        
        {
            const DHorizontalLayout& horizontalLayout = m_obLayout.horizontal;
            
            if (horizontalLayout.left < FLOAT_NONE && horizontalLayout.right < FLOAT_NONE)
            {
                size.width = contentSize.width - horizontalLayout.left - horizontalLayout.right;
                point.x = horizontalLayout.left;
            }
            else if (horizontalLayout.left < FLOAT_NONE && horizontalLayout.width < FLOAT_NONE)
            {
                size.width = horizontalLayout.width;
                point.x = horizontalLayout.left;
            }
            else if (horizontalLayout.right < FLOAT_NONE && horizontalLayout.width < FLOAT_NONE)
            {
                size.width = horizontalLayout.width;
                point.x = contentSize.width - horizontalLayout.right - size.width;
            }
            else if (horizontalLayout.width < FLOAT_NONE && horizontalLayout.center < FLOAT_NONE)
            {
                size.width = horizontalLayout.width;
                point.x = contentSize.width * horizontalLayout.center - size.width / 2;
            }
            else if (horizontalLayout.normalizedWidth < FLOAT_NONE && horizontalLayout.center < FLOAT_NONE)
            {
                size.width = contentSize.width * horizontalLayout.normalizedWidth;
                point.x = contentSize.width * horizontalLayout.center - size.width / 2;
            }
        }
        
        {
            const DVerticalLayout& verticalLayout = m_obLayout.vertical;
            
            if (verticalLayout.top < FLOAT_NONE && verticalLayout.bottom < FLOAT_NONE)
            {
                size.height = contentSize.height - verticalLayout.top - verticalLayout.bottom;
                point.y = verticalLayout.top;
            }
            else if (verticalLayout.top < FLOAT_NONE && verticalLayout.height < FLOAT_NONE)
            {
                size.height = verticalLayout.height;
                point.y = verticalLayout.top;
            }
            else if (verticalLayout.bottom < FLOAT_NONE && verticalLayout.height < FLOAT_NONE)
            {
                size.height = verticalLayout.height;
                point.y = contentSize.height - verticalLayout.bottom - size.height;
            }
            else if (verticalLayout.height < FLOAT_NONE && verticalLayout.center < FLOAT_NONE)
            {
                size.height = verticalLayout.height;
                point.y = contentSize.height * verticalLayout.center - size.height / 2;
            }
            else if (verticalLayout.normalizedHeight < FLOAT_NONE && verticalLayout.center < FLOAT_NONE)
            {
                size.height = contentSize.height * verticalLayout.normalizedHeight;
                point.y = contentSize.height * verticalLayout.center - size.height / 2;
            }
        }
        
        if (allowAnimation
            && CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setContentSize(size, this);
        }
        else
        {
            this->setContentSize(size);
        }
        
        DPoint p = ccpCompMult(size, m_obAnchorPoint);;
        p = ccpAdd(p, point);
        
        if (allowAnimation
            && CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setPoint(p, this);
        }
        else
        {
            this->setPoint(p);
        }
    }
    else
    {
        this->updateDraw();
    }
}

void CAView::updateDraw()
{
    m_bContentSizeDirty = m_bTransformUpdated = m_bTransformDirty = m_bInverseDirty = true;
    
    CAView* v = m_pSuperview;
    if (v)
    {
        while (v == v->getSuperview())
        {
            CC_BREAK_IF(v == nullptr);
            CC_RETURN_IF(!v->isVisible());
        }
    }
    CAApplication::getApplication()->updateDraw();
}

CAView* CAView::getSubviewByTag(int aTag)
{
    CCAssert( aTag != TagInvalid, "Invalid tag");
    
    for (auto& subview : m_obSubviews)
    {
        if (subview->m_nTag == aTag)
        {
            return subview;
        }
    }
    return NULL;
}

CAView* CAView::getSubviewByTextTag(const std::string& textTag)
{
    CCAssert( !textTag.empty(), "Invalid tag");
    
    for (auto& subview : m_obSubviews)
    {
        if (subview->m_sTextTag.compare(textTag) == 0)
        {
            return subview;
        }
    }
    return NULL;
}

void CAView::setTag(int tag)
{
    this->m_nTag = tag;
}

int CAView::getTag()
{
    return this->m_nTag;
}

void CAView::addSubview(CAView *subview)
{
    if (!subview) return;
    this->insertSubview(subview, subview->getZOrder());
}

// helper used by reorderChild & add
void CAView::insertSubview(CAView* subview, int z)
{
    CCAssert( subview != NULL, "Argument must be non-nil");
    CCAssert( subview->m_pSuperview == NULL, "child already added. It can't be added again");
    
    m_bReorderSubviewDirty = true;
    m_obSubviews.pushBack(subview);
    subview->_setZOrder(z);
    
    subview->setSuperview(this);
    subview->setOrderOfArrival(s_globalOrderOfArrival++);
    
    if(m_bRunning)
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
    if (m_obSubviews.contains(subview))
    {
        this->detachSubview(subview);
    }
}

void CAView::removeSubviewByTag(int tag)
{
    CCAssert( tag != TagInvalid, "Invalid tag");
    
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
        for (auto& subview : m_obSubviews)
        {
            if(m_bRunning)
            {
                subview->onExitTransitionDidStart();
                subview->onExit();
            }
            
            subview->setSuperview(nullptr);
        }
        m_obSubviews.clear();
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
    
    // set Superview nil at the end
    subview->setSuperview(nullptr);
    
    m_obSubviews.eraseObject(subview);
    
    this->updateDraw();
}

void CAView::reorderSubview(CAView *subview, int zOrder)
{
    if (zOrder == subview->getZOrder())
    {
        return;
    }

    CCAssert( subview != NULL, "Subview must be non-nil");
    m_bReorderSubviewDirty = true;
    subview->setOrderOfArrival(s_globalOrderOfArrival++);
    subview->_setZOrder(zOrder);
    this->updateDraw();
}

void CAView::sortAllSubviews()
{
    if (m_bReorderSubviewDirty && !m_obSubviews.empty())
    {
        std::sort(m_obSubviews.begin(), m_obSubviews.end(), compareSubviewZOrder);

        m_bReorderSubviewDirty = false;
    }
}

void CAView::visitEve(void)
{
    for (auto& subview : m_obSubviews)
    {
        subview->visitEve();
    }
}

void CAView::draw()
{
    auto renderer = m_pApplication->getRenderer();
    draw(renderer, m_tModelViewTransform, true);
}

void CAView::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
    CC_RETURN_IF(m_pobImage == nullptr);
    
    if(m_bInsideBounds)
    {
        static unsigned short quadIndices[] = {0,1,2, 3,2,1};
        
        TrianglesCommand::Triangles triangles;
        triangles.indices = quadIndices;
        triangles.vertCount = 4;
        triangles.indexCount = 6;
        triangles.verts = (ccV3F_C4B_T2F*)&m_sQuad;

        m_obTrianglesCommand.init(0,
                               m_pobImage->getName(),
                               this->getGLProgramState(),
                               m_sBlendFunc,
                               triangles,
                               transform,
                               flags);
        renderer->addCommand(&m_obTrianglesCommand);
    }
}

void CAView::checkInsideBounds(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
    // Don't calculate the culling if the transform was not updated
    auto visitingCamera = CACamera::getVisitingCamera();
    auto defaultCamera = CACamera::getDefaultCamera();
    if (visitingCamera == defaultCamera)
    {
        m_bInsideBounds = ((flags & FLAGS_TRANSFORM_DIRTY) || visitingCamera->isViewProjectionUpdated()) ? renderer->checkVisibility(transform, m_obContentSize) : m_bInsideBounds;
    }
    else
    {
        // XXX: this always return true since
        m_bInsideBounds = renderer->checkVisibility(transform, m_obContentSize);
    }
}

void CAView::visit()
{
    auto renderer = m_pApplication->getRenderer();
    auto& parentTransform = m_pApplication->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    visit(renderer, parentTransform, true);
}

uint32_t CAView::processParentFlags(const Mat4& parentTransform, uint32_t parentFlags)
{
    // Fixes Github issue #16100. Basically when having two cameras, one camera might set as dirty the
    // node that is not visited by it, and might affect certain calculations. Besides, it is faster to do this.
    if (!isVisitableByVisitingCamera())
        return parentFlags;
    
    uint32_t flags = parentFlags;
    flags |= (m_bTransformUpdated ? FLAGS_TRANSFORM_DIRTY : 0);
    flags |= (m_bContentSizeDirty ? FLAGS_CONTENT_SIZE_DIRTY : 0);
    
    
    if(flags & FLAGS_DIRTY_MASK)
        m_tModelViewTransform = this->transform(parentTransform);
    
    m_bTransformUpdated = false;
    m_bContentSizeDirty = false;
    
    return flags;
}

bool CAView::isVisitableByVisitingCamera() const
{
    auto camera = CACamera::getVisitingCamera();
    bool visibleByCamera = camera ? ((unsigned short)camera->getCameraFlag() & m_iCameraMask) != 0 : true;
    return visibleByCamera;
}

void CAView::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    CC_RETURN_IF(!m_bVisible);
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    m_pApplication->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    m_pApplication->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, m_tModelViewTransform);
    
    m_bInsideBounds = false;
    bool visibleByCamera = isVisitableByVisitingCamera();

    if (visibleByCamera)
    {
        this->checkInsideBounds(renderer, m_tModelViewTransform, flags);
        this->drawAllShadow(renderer, m_tModelViewTransform, flags);
    }

    m_bScissorRestored = false;

    if (m_bDisplayRange == false)
    {
        m_obBeforeDrawCommand.init(0);
        m_obBeforeDrawCommand.func = [&, parentTransform](){

            float minX = 0, maxX = 0, minY = 0, maxY = 0;
        
            Mat4 tm = Mat4::IDENTITY;
            tm.m[12] += m_obContentSize.width;
            tm.m[13] += m_obContentSize.height;

            Mat4 max;
            Mat4::multiply(m_tModelViewTransform, tm, &max);

            minX =  MIN(max.m[12], m_tModelViewTransform.m[12]);
            maxX =  MAX(max.m[12], m_tModelViewTransform.m[12]);
            minY =  MIN(max.m[13], m_tModelViewTransform.m[13]);
            maxY =  MAX(max.m[13], m_tModelViewTransform.m[13]);
            
            auto glview = m_pApplication->getOpenGLView();
            
			m_bScissorRestored = glview->isScissorEnabled();
            if (m_bScissorRestored)
            {
                m_obSupviewScissorRect = glview->getScissorRect();

                minX = MAX(minX, m_obSupviewScissorRect.getMinX());
                maxX = MIN(maxX, m_obSupviewScissorRect.getMaxX());
                minY = MAX(minY, m_obSupviewScissorRect.getMinY());
                maxY = MIN(maxY, m_obSupviewScissorRect.getMaxY());
            }
            else
            {
                glEnable(GL_SCISSOR_TEST);
            }
            
            const DSize& winSize = CAApplication::getApplication()->getWinSize();
            
            if (minX > maxX)
            {
                minX = winSize.width - minX;
                maxX = winSize.width - maxX;
            }
            
            if (minY > maxY)
            {
                minY = winSize.height - minY;
                maxY = winSize.height - maxY;
            }
            
            glview->setScissorInPoints(minX, minY, maxX-minX, maxY-minY);

        };
        m_pApplication->getRenderer()->addCommand(&m_obBeforeDrawCommand);
    }
    
    if(!m_obSubviews.empty())
    {
        
        this->sortAllSubviews();
        int i = 0;
        for( ; i < m_obSubviews.size(); i++ )
        {
            auto view = m_obSubviews.at(i);
            
            if (view && view->m_nZOrder < 0)
                view->visit(renderer, m_tModelViewTransform, flags);
            else
                break;
        }
        // self draw
        if (visibleByCamera)
            this->draw(renderer, m_tModelViewTransform, flags);
        
        for(auto it=m_obSubviews.begin()+i; it != m_obSubviews.end(); ++it)
            (*it)->visit(renderer, m_tModelViewTransform, flags);
    }
    else if (visibleByCamera)
    {
        this->draw(renderer, m_tModelViewTransform, flags);
    }

    if (m_bDisplayRange == false)
    {
        m_obAfterDrawCommand.init(0);
        m_obAfterDrawCommand.func = [&](){
            
            if (m_bScissorRestored)
            {
                auto glview = m_pApplication->getOpenGLView();
                glview->setScissorInPoints(m_obSupviewScissorRect.origin.x,
                                           m_obSupviewScissorRect.origin.y,
                                           m_obSupviewScissorRect.size.width,
                                           m_obSupviewScissorRect.size.height);
            }
            else
            {
                glDisable(GL_SCISSOR_TEST);
            }
        };
        m_pApplication->getRenderer()->addCommand(&m_obAfterDrawCommand);
    }
    
    m_pApplication->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

}


CAView* CAView::copy()
{
    CAView* view = NULL;
    if (m_eLayoutType == 0)
    {
        view = CAView::createWithFrame(this->getFrame(), this->getColor());
    }
    else if (m_eLayoutType == 1)
    {
        view = CAView::createWithCenter(this->getCenter(), this->getColor());
    }
    else
    {
        view = CAView::createWithLayout(this->getLayout(), this->getColor());
    }
    view->setImage(this->getImage());
    
    return view;
}

CAResponder* CAView::nextResponder()
{
    return m_bHaveNextResponder ? m_pSuperview : nullptr;
}

void CAView::onEnter()
{
    if (m_pSuperview)
    {
        this->reViewlayout(m_pSuperview->m_obContentSize);
    }
    
    m_bRunning = true;
    this->updateDraw();
    
    if (m_obOnEnterCallback)
    {
        m_obOnEnterCallback();
    }
    
    for (auto& subview : m_obSubviews)
    {
        subview->onEnter();
    }
}

void CAView::onEnterTransitionDidFinish()
{
    if (!m_obSubviews.empty())
    {
        CAVector<CAView*>::iterator itr;
        for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
            (*itr)->onEnterTransitionDidFinish();
    }

    if (m_pContentContainer)
    {
        m_pContentContainer->viewOnEnterTransitionDidFinish();
    }
    
    if (m_obOnEnterTransitionDidFinishCallback)
    {
        m_obOnEnterTransitionDidFinishCallback();
    }
}

void CAView::onExitTransitionDidStart()
{
    for (auto& subview : m_obSubviews)
    {
        subview->onExitTransitionDidStart();
    }

    if (m_pContentContainer)
    {
        m_pContentContainer->viewOnExitTransitionDidStart();
    }
    
    if (m_obOnExitTransitionDidStartCallback)
    {
        m_obOnExitTransitionDidStartCallback();
    }
}

void CAView::onExit()
{
    m_bRunning = false;
    
    if (m_obOnExitCallback)
    {
        m_obOnExitCallback();
    }
    
    for (auto& subview : m_obSubviews)
    {
        subview->onExit();
    }
}

// override me
void CAView::update(float fDelta)
{

}

/////
AffineTransform CAView::getViewToSuperviewAffineTransform() const
{
    AffineTransform ret;
    GLToCGAffine(getViewToSuperviewTransform().m, &ret);
    
    return ret;
}


Mat4 CAView::getViewToSuperviewTransform(CAView* ancestor) const
{
    Mat4 t(this->getViewToSuperviewTransform());

    CAView *s = m_pSuperview;
    while (s)
    {
        t = s->getViewToSuperviewTransform() * t;
        CC_BREAK_IF(!s->getSuperview());
        s = s->getSuperview();
        CC_BREAK_IF(s == ancestor);
    }

    return t;
}

AffineTransform CAView::getViewToSuperviewAffineTransform(CAView* ancestor) const
{
    AffineTransform t(this->getViewToSuperviewAffineTransform());
    
    CAView *s = m_pSuperview;
    while (s)
    {
        t = AffineTransformConcat(t, s->getViewToSuperviewAffineTransform());
        CC_BREAK_IF(!s->getSuperview());
        s = s->getSuperview();
        CC_BREAK_IF(s == ancestor);
    }
    
    return t;
}

const Mat4& CAView::getViewToSuperviewTransform() const
{
    if (m_bTransformDirty)
    {
        // Translate values
        float height = 0;
        
        if (this->m_pSuperview)
        {
            height = this->m_pSuperview->m_obContentSize.height;
        }
        else
        {
            height = m_pApplication->getWinSize().height;
        }
        
        float x = m_obPoint.x;
        float y = height - m_obPoint.y;
        float z = m_fPointZ;

        bool needsSkewMatrix = ( m_fSkewX || m_fSkewY );
        
        
        DPoint anchorPoint(m_obAnchorPointInPoints.x * m_fScaleX, (m_obContentSize.height - m_obAnchorPointInPoints.y) * m_fScaleY);
        
        // calculate real position
        if (! needsSkewMatrix && !anchorPoint.equals(DSizeZero))
        {
            x += -anchorPoint.x;
            y += -anchorPoint.y;
        }
        
        // Build Transform Matrix = translation * rotation * scale
        Mat4 translation;
        //move to anchor point first, then rotate
        Mat4::createTranslation(x + anchorPoint.x, y + anchorPoint.y, z, &translation);
        
        Mat4::createRotation(m_obRotationQuat, &m_tTransform);
        
        m_tTransform = translation * m_tTransform;
        //move by (-anchorPoint.x, -anchorPoint.y, 0) after rotation
        m_tTransform.translate(-anchorPoint.x, -anchorPoint.y, 0);
        
        
        if (m_fScaleX != 1.f)
        {
            m_tTransform.m[0] *= m_fScaleX, m_tTransform.m[1] *= m_fScaleX, m_tTransform.m[2] *= m_fScaleX;
        }
        if (m_fScaleY != 1.f)
        {
            m_tTransform.m[4] *= m_fScaleY, m_tTransform.m[5] *= m_fScaleY, m_tTransform.m[6] *= m_fScaleY;
        }
        if (m_fScaleZ != 1.f)
        {
            m_tTransform.m[8] *= m_fScaleZ, m_tTransform.m[9] *= m_fScaleZ, m_tTransform.m[10] *= m_fScaleZ;
        }
        
        // FIXME:: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needsSkewMatrix)
        {
            float skewMatArray[16] =
            {
                1, (float)tanf(CC_DEGREES_TO_RADIANS(m_fSkewY)), 0, 0,
                (float)tanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1, 0, 0,
                0,  0,  1, 0,
                0,  0,  0, 1
            };
            Mat4 skewMatrix(skewMatArray);
            
            m_tTransform = m_tTransform * skewMatrix;
            
            // adjust anchor point
            if (!m_obAnchorPointInPoints.equals(DSizeZero))
            {
                // FIXME:: Argh, Mat4 needs a "translate" method.
                // FIXME:: Although this is faster than multiplying a vec4 * mat4
                DPoint anchorPoint(m_obAnchorPointInPoints.x * m_fScaleX, (m_obContentSize.height - m_obAnchorPointInPoints.y) * m_fScaleY);
                m_tTransform.m[12] += m_tTransform.m[0] * -anchorPoint.x + m_tTransform.m[4] * -anchorPoint.y;
                m_tTransform.m[13] += m_tTransform.m[1] * -anchorPoint.x + m_tTransform.m[5] * -anchorPoint.y;
            }
        }
    }
    
    if (m_pAdditionalTransform)
    {
        if (m_bTransformDirty)
        {
            m_pAdditionalTransform[1] = m_tTransform;
        }
        
        if (m_bTransformUpdated)
        {
            m_tTransform = m_pAdditionalTransform[1] * m_pAdditionalTransform[0];
        }
    }
    m_bTransformDirty = m_bAdditionalTransformDirty = false;
    
    return m_tTransform;
}

void CAView::setViewToSuperviewTransform(const Mat4& transform)
{
    m_tTransform = transform;
    m_bTransformDirty = false;
    m_bTransformUpdated = true;
    
    if (m_pAdditionalTransform)
    {
        m_pAdditionalTransform[1] = transform;
    }
}

void CAView::setAdditionalTransform(const AffineTransform& additionalTransform)
{
    Mat4 tmp;
    CGAffineToGL(additionalTransform, tmp.m);
    setAdditionalTransform(&tmp);
}

void CAView::setAdditionalTransform(const Mat4* additionalTransform)
{
    if (additionalTransform == nullptr)
    {
        delete [] m_pAdditionalTransform;
        m_pAdditionalTransform = nullptr;
    }
    else
    {
        if (!m_pAdditionalTransform)
        {
            m_pAdditionalTransform = new Mat4[2];
            
            m_pAdditionalTransform[1] = m_tTransform;
        }
        
        m_pAdditionalTransform[0] = *additionalTransform;
    }
    m_bTransformUpdated = m_bAdditionalTransformDirty = m_bInverseDirty = true;
}


AffineTransform CAView::getSuperviewToViewAffineTransform() const
{
    AffineTransform ret;
    
    GLToCGAffine(getSuperviewToViewTransform().m,&ret);
    return ret;
}

const Mat4& CAView::getSuperviewToViewTransform() const
{
    if ( m_bInverseDirty )
    {
        m_tInverse = getViewToSuperviewTransform().getInversed();
        m_bInverseDirty = false;
    }
    
    return m_tInverse;
}


AffineTransform CAView::getViewToWorldAffineTransform() const
{
    return this->getViewToSuperviewAffineTransform(nullptr);
}

Mat4 CAView::getViewToWorldTransform() const
{
    return this->getViewToSuperviewTransform(nullptr);
}

AffineTransform CAView::getWorldToViewAffineTransform() const
{
    return AffineTransformInvert(this->getViewToWorldAffineTransform());
}

Mat4 CAView::getWorldToViewTransform() const
{
    return getViewToWorldTransform().getInversed();
}

Mat4 CAView::transform(const Mat4& parentTransform)
{
    return parentTransform * this->getViewToSuperviewTransform();
}

void CAView::updateTransform()
{
    for (auto& subview : m_obSubviews)
    {
        subview->updateTransform();
    }
    
#if CC_SPRITE_DEBUG_DRAW
    // draw bounding box
    DPoint vertices[4] =
    {
        DPoint( m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y ),
        DPoint( m_sQuad.br.vertices.x, m_sQuad.br.vertices.y ),
        DPoint( m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y ),
        DPoint( m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y ),
    };
    ccDrawPoly(vertices, 4, true);
#endif
}
////

DRect CAView::convertRectToNodeSpace(const CrossApp::DRect &worldRect)
{
    DRect ret = worldRect;
    ret.origin = this->convertToNodeSpace(ret.origin);
    ret.size = this->convertToNodeSize(ret.size);
    return ret;
}

DRect CAView::convertRectToWorldSpace(const CrossApp::DRect &nodeRect)
{
    DRect ret = nodeRect;
    ret.origin = this->convertToWorldSpace(ret.origin);
    ret.size = this->convertToWorldSize(ret.size);
    return ret;
}

DPoint CAView::convertToNodeSpace(const DPoint& worldPoint)
{
    Mat4 tmp = getWorldToViewTransform();
    DPoint3D vec3(worldPoint.x, CAApplication::getApplication()->getWinSize().height - worldPoint.y, 0);
    DPoint3D ret;
    tmp.transformPoint(vec3,&ret);
    return DPoint(ret.x, m_obContentSize.height - ret.y);
}

DPoint CAView::convertToWorldSpace(const DPoint& nodePoint)
{
    Mat4 tmp = getViewToWorldTransform();
    DPoint3D vec3(nodePoint.x, m_obContentSize.height - nodePoint.y, 0);
    DPoint3D ret;
    tmp.transformPoint(vec3,&ret);
    return DPoint(ret.x, CAApplication::getApplication()->getWinSize().height - ret.y);
}

DSize CAView::convertToNodeSize(const DSize& worldSize)
{
    DSize ret = worldSize;
    for (CAView* v = this; v; v = v->m_pSuperview)
    {
        ret.width /= v->m_fScaleX;
        ret.height /= v->m_fScaleY;
    }
    return ret;
}

DSize CAView::convertToWorldSize(const DSize& nodeSize)
{
    DSize ret = nodeSize;
    for (CAView* v = this; v; v = v->m_pSuperview)
    {
        ret.width *= v->m_fScaleX;
        ret.height *= v->m_fScaleY;
    }
    return ret;
}

DPoint CAView::convertTouchToNodeSpace(CATouch *touch)
{
    DPoint point = touch->getLocation();
    return this->convertToNodeSpace(point);
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
    CC_SAFE_RETAIN(image);
    CC_SAFE_RELEASE(m_pobImage);
    m_pobImage = image;
    this->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    this->updateBlendFunc();
    this->updateDraw();
}

CAImage* CAView::getImage(void)
{
    return m_pobImage;
}

void CAView::setImageRect(const DRect& rect)
{
    if (CAViewAnimation::areAnimationsEnabled()
        && CAViewAnimation::areBeginAnimations()
        && !m_obRect.equals(rect))
    {
        CAViewAnimation::getInstance()->setImageRect(rect, this);
    }
    else
    {
        this->setVertexRect(rect);
        this->setImageCoords(rect);
        this->updateImageRect();
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
    
    m_sQuad.bl.vertices = DPoint3D(x1, y1, m_fPointZ);
    m_sQuad.br.vertices = DPoint3D(x2, y1, m_fPointZ);
    m_sQuad.tl.vertices = DPoint3D(x1, y2, m_fPointZ);
    m_sQuad.tr.vertices = DPoint3D(x2, y2, m_fPointZ);
}

// override this method to generate "double scale" sprites
void CAView::setVertexRect(const DRect& rect)
{
    m_obRect = rect;
}

void CAView::setImageCoords(DRect rect)
{
    CAImage* image = m_pobImage;
    CC_RETURN_IF(! image);
    
    float atlasWidth = (float)image->getPixelsWide();
    float atlasHeight = (float)image->getPixelsHigh();
 
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
    if (_displayedAlpha != alpha)
    {
        if (CAViewAnimation::areAnimationsEnabled()
            && CAViewAnimation::areBeginAnimations())
        {
            CAViewAnimation::getInstance()->setAlpha(alpha, this);
        }
        else
        {
            _realAlpha = alpha;
            
            float superviewAlpha = m_pSuperview ? m_pSuperview->getDisplayedAlpha() : 1.0f;
            
            this->updateDisplayedAlpha(superviewAlpha);
        }
    }
}

void CAView::updateDisplayedAlpha(float superviewAlpha)
{
	_displayedAlpha = _realAlpha * superviewAlpha;
	
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
        && CAViewAnimation::areBeginAnimations()
        && _realColor != color)
    {
        CAViewAnimation::getInstance()->setColor(color, this);
    }
    else
    {
        _realColor = color;
        _displayedColor = color;
        this->updateColor();
    }
}

void CAView::updateDisplayedColor(const CAColor4B& superviewColor)
{
	_displayedColor.r = _realColor.r * superviewColor.r/255.0;
	_displayedColor.g = _realColor.g * superviewColor.g/255.0;
	_displayedColor.b = _realColor.b * superviewColor.b/255.0;
    _displayedColor.a = _realColor.a * superviewColor.a/255.0;
    
    updateColor();
}

void CAView::updateColor(void)
{
    CAColor4B color4 = _displayedColor;
    color4.a = color4.a * _displayedAlpha;
    
    if (m_bOpacityModifyRGB)
    {
        color4.r = color4.r * color4.a / 255.0f;
        color4.g = color4.g * color4.a / 255.0f;
        color4.b = color4.b * color4.a / 255.0f;
    }
   
    m_sQuad.bl.colors = color4;
    m_sQuad.br.colors = color4;
    m_sQuad.tl.colors = color4;
    m_sQuad.tr.colors = color4;

    this->updateDraw();
}

void CAView::updateBlendFunc(void)
{
    if (!m_pobImage || !m_pobImage->hasPremultipliedAlpha())
    {
        m_sBlendFunc = BlendFunc_alpha_non_premultiplied;
        setOpacityModifyRGB(false);
    }
    else
    {
        m_sBlendFunc = BlendFunc_alpha_premultiplied;
        setOpacityModifyRGB(true);
    }
}

void CAView::setOpacityModifyRGB(bool modify)
{
    if (m_bOpacityModifyRGB != modify)
    {
        m_bOpacityModifyRGB = modify;
        this->updateColor();
    }
}

bool CAView::isOpacityModifyRGB(void)
{
    return m_bOpacityModifyRGB;
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

void CAView::setCameraMask(unsigned short mask, bool applyChildren)
{
    m_iCameraMask = mask;
    if (applyChildren)
    {
        for (const auto& var : m_obSubviews)
        {
            var->setCameraMask(mask, applyChildren);
        }
    }
}

CAView::Shadow::Shadow()
: image(CAImage::CC_SHADOW_IMAGE())
, glProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP))
, blendFunc(BlendFunc_alpha_non_premultiplied)
{
    CC_SAFE_RETAIN(glProgramState);
}
CAView::Shadow::~Shadow()
{
    CC_SAFE_RELEASE(glProgramState);
}

void CAView::Shadow::updateQuad(CAView* view, GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat left, GLfloat right, GLfloat top, GLfloat bottom)
{
    this->quad = view->m_sQuad;

    this->quad.bl.vertices = DPoint3D(x1, y1, view->m_fPointZ);
    this->quad.br.vertices = DPoint3D(x2, y1, view->m_fPointZ);
    this->quad.tl.vertices = DPoint3D(x1, y2, view->m_fPointZ);
    this->quad.tr.vertices = DPoint3D(x2, y2, view->m_fPointZ);
    
    this->quad.bl.texCoords.u = left;
    this->quad.bl.texCoords.v = bottom;
    this->quad.br.texCoords.u = right;
    this->quad.br.texCoords.v = bottom;
    this->quad.tl.texCoords.u = left;
    this->quad.tl.texCoords.v = top;
    this->quad.tr.texCoords.u = right;
    this->quad.tr.texCoords.v = top;
    
    this->quad.bl.colors = this->quad.br.colors = this->quad.tl.colors = this->quad.tr.colors = CAColor4B::WHITE;
}

void CAView::enabledLeftShadow(bool var)
{
    if (var)
    {
        if (m_pLeftShadow == nullptr)
        {
            m_pLeftShadow = new CAView::Shadow();
        }
    }
    else
    {
        CC_SAFE_DELETE(m_pLeftShadow);
    }
    
    this->autoEnabledCornerShadow();
}

void CAView::enabledRightShadow(bool var)
{
    if (var)
    {
        if (m_pRightShadow == nullptr)
        {
            m_pRightShadow = new CAView::Shadow();
        }
    }
    else
    {
        CC_SAFE_DELETE(m_pRightShadow);
    }
    
    this->autoEnabledCornerShadow();
}

void CAView::enabledTopShadow(bool var)
{
    if (var)
    {
        if (m_pTopShadow == nullptr)
        {
            m_pTopShadow = new CAView::Shadow();
        }
    }
    else
    {
        CC_SAFE_DELETE(m_pTopShadow);
    }
    
    this->autoEnabledCornerShadow();
}

void CAView::enabledBottomShadow(bool var)
{
    if (var)
    {
        if (m_pBottomShadow == nullptr)
        {
            m_pBottomShadow = new CAView::Shadow();
        }
    }
    else
    {
        CC_SAFE_DELETE(m_pBottomShadow);
    }
    
    this->autoEnabledCornerShadow();
}

void CAView::autoEnabledCornerShadow()
{
    if (!m_pLeftTopShadow && m_pLeftShadow && m_pTopShadow)
    {
        m_pLeftTopShadow = new CAView::Shadow();
    }
    
    if (m_pLeftTopShadow && (!m_pLeftShadow || !m_pTopShadow))
    {
        CC_SAFE_DELETE(m_pLeftTopShadow);
    }
    
    if (!m_pRightTopShadow && m_pRightShadow && m_pTopShadow)
    {
        m_pRightTopShadow = new CAView::Shadow();
    }
    
    if (m_pRightTopShadow && (!m_pRightShadow || !m_pTopShadow))
    {
        CC_SAFE_DELETE(m_pRightTopShadow);
    }
    
    if (!m_pLeftBottomShadow && m_pLeftShadow && m_pBottomShadow)
    {
        m_pLeftBottomShadow = new CAView::Shadow();
    }
    
    if (m_pLeftBottomShadow && (!m_pLeftShadow || !m_pBottomShadow))
    {
        CC_SAFE_DELETE(m_pLeftBottomShadow);
    }
    
    if (!m_pRightBottomShadow && m_pRightShadow && m_pBottomShadow)
    {
        m_pRightBottomShadow = new CAView::Shadow();
    }
    
    if (m_pRightBottomShadow && (!m_pRightShadow || !m_pBottomShadow))
    {
        CC_SAFE_DELETE(m_pRightBottomShadow);
    }
}

void CAView::drawShadow(Renderer* renderer, const Mat4 &transform, uint32_t flags, CAView::Shadow* shadow)
{
    static unsigned short quadIndices[] = {0,1,2, 3,2,1};
    
    TrianglesCommand::Triangles triangles;
    triangles.indices = quadIndices;
    triangles.vertCount = 4;
    triangles.indexCount = 6;
    triangles.verts = (ccV3F_C4B_T2F*)&shadow->quad;
    
    TrianglesCommand* trianglesCommand = &shadow->trianglesCommand;
    trianglesCommand->init(0,
                           shadow->image->getName(),
                           shadow->glProgramState,
                           shadow->blendFunc,
                           triangles,
                           transform,
                           flags);
    renderer->addCommand(trianglesCommand);
}

void CAView::drawAllShadow(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
    CC_RETURN_IF(m_bInsideBounds == false);
    
    GLfloat x0 = -10;
    GLfloat x1 = 0;
    GLfloat x2 = m_obContentSize.width;
    GLfloat x3 = m_obContentSize.width+10;

    GLfloat y0 = -10;
    GLfloat y1 = 0;
    GLfloat y2 = m_obContentSize.height;
    GLfloat y3 = m_obContentSize.height+10;

    GLfloat n0 = 0;
    GLfloat n1 = 0.2;
    GLfloat n2 = 0.8;
    GLfloat n3 = 1.0;

    if (m_pLeftShadow)
    {
        m_pLeftShadow->updateQuad(this, x0, x1, y1, y2, n0, n1, 1-n2, 1-n1);
        this->drawShadow(renderer, transform, flags, m_pLeftShadow);
    }
    
    if (m_pLeftBottomShadow)
    {
        m_pLeftBottomShadow->updateQuad(this, x0, x1, y0, y1, n0, n1, 1-n1, 1-n0);
        this->drawShadow(renderer, transform, flags, m_pLeftBottomShadow);
    }
    
    if (m_pBottomShadow)
    {
        m_pBottomShadow->updateQuad(this, x1, x2, y0, y1, n1, n2, 1-n1, 1-n0);
        this->drawShadow(renderer, transform, flags, m_pBottomShadow);
    }

    if (m_pRightBottomShadow)
    {
        m_pRightBottomShadow->updateQuad(this, x2, x3, y0, y1, n2, n3, 1-n1, 1-n0);
        this->drawShadow(renderer, transform, flags, m_pRightBottomShadow);
    }

    if (m_pRightShadow)
    {
        m_pRightShadow->updateQuad(this, x2, x3, y1, y2, n2, n3, 1-n2, 1-n1);
        this->drawShadow(renderer, transform, flags, m_pRightShadow);
    }

    if (m_pRightTopShadow)
    {
        m_pRightTopShadow->updateQuad(this, x2, x3, y2, y3, n2, n3, 1-n3, 1-n2);
        this->drawShadow(renderer, transform, flags, m_pRightTopShadow);
    }

    if (m_pTopShadow)
    {
        m_pTopShadow->updateQuad(this, x1, x2, y2, y3, n1, n2, 1-n3, 1-n2);
        this->drawShadow(renderer, transform, flags, m_pTopShadow);
    }

    if (m_pLeftTopShadow)
    {
        m_pLeftTopShadow->updateQuad(this, x0, x1, y2, y3, n0, n1, 1-n3, 1-n2);
        this->drawShadow(renderer, transform, flags, m_pLeftTopShadow);
    }

}

NS_CC_END;

//
//  CGSprite.cpp
//  CrossApp
//
//  Created by Li Yuanfeng on 16-7-19.
//  Copyright (c) 2014Âπ?http://9miao.com All rights reserved.
//

#include "game/actions/CGAnimation.h"
#include "game/actions/CGAnimationCache.h"
#include "ccConfig.h"
#include "CGSprite.h"
#include "CGSpriteFrame.h"
#include "CGSpriteFrameCache.h"
#include "images/CAImageCache.h"
#include "view/CADrawingPrimitives.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramState.h"
#include "basics/CAPointExtension.h"
#include "math/CAAffineTransform.h"
#include "math/TransformUtils.h"
#include "math/CAAffineTransform.h"
#include "basics/CACamera.h"
#include "ccMacros.h"
#include <string.h>
#include "animation/CAViewAnimation.h"
#include "game/actions/CGActionManager.h"
NS_CC_BEGIN

//static int spriteCount = 0;

CGSprite::CGSprite(void)
: m_obAutoContentSize(true)
, m_obNormalizedPosition(DPointZero)
, m_bUsingNormalizedPosition(false)
, m_obOffsetPosition(DPointZero)
, m_obUnflippedOffsetPositionFromCenter(DPointZero)
{
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;

    //CCLog("CGSprite = %d\n", ++spriteCount);
}

CGSprite::~CGSprite(void)
{
    this->stopAllActions();
    //CCLog("~CGSprite = %d\n", --spriteCount);
}

void CGSprite::onEnter()
{
    CAView::onEnter();
    CAApplication::getApplication()->getActionManager()->resumeTarget(this);
}

void CGSprite::onExit()
{
    CAView::onExit();
    CAApplication::getApplication()->getActionManager()->pauseTarget(this);
}

CGSprite* CGSprite::createWithImage(CAImage *image)
{
    CGSprite *pobSprite = new CGSprite();
    if (pobSprite && pobSprite->initWithImage(image))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CGSprite* CGSprite::createWithImage(CAImage *image, const DRect& rect)
{
    CGSprite *pobSprite = new CGSprite();
    if (pobSprite && pobSprite->initWithImage(image, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CGSprite* CGSprite::create(const std::string& pszFileName)
{
    CGSprite *pobSprite = new CGSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CGSprite* CGSprite::create(const std::string& pszFileName, const DRect& rect)
{
    CGSprite *pobSprite = new CGSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CGSprite* CGSprite::createWithSpriteFrame(CGSpriteFrame *pSpriteFrame)
{
    CGSprite *pobSprite = new CGSprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CGSprite* CGSprite::createWithSpriteFrameName(const std::string& pszSpriteFrameName)
{
    CGSpriteFrame *pFrame = CGSpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName);
    
#if CROSSAPP_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName.c_str());
    CCAssert(pFrame != NULL, msg);
#endif
    
    return createWithSpriteFrame(pFrame);
}

CGSprite* CGSprite::create()
{
    CGSprite *pSprite = new CGSprite();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool CGSprite::init(void)
{
    return initWithImage(NULL, DRectZero);
}

// designated initializer
bool CGSprite::initWithImage(CAImage *image, const DRect& rect, bool rotated)
{
    if (CAView::init())
    {
        this->setImage(image);
        this->setImageRect(rect, rotated, rect.size);
        
        return true;
    }
    else
    {
        return false;
    }
}

bool CGSprite::initWithImage(CAImage *image, const DRect& rect)
{
    return initWithImage(image, rect, false);
}

bool CGSprite::initWithImage(CAImage *image)
{
    CCAssert(image != NULL, "Invalid texture for sprite");
    
    DRect rect = DRectZero;
    rect.size = image->getContentSize();
    
    return initWithImage(image, rect);
}

bool CGSprite::initWithFile(const std::string& pszFilename)
{
    CAImage *image = CAImage::create(pszFilename);
    if (image)
    {
        DRect rect = DRectZero;
        rect.size = image->getContentSize();
        return initWithImage(image, rect);
    }

    return false;
}

bool CGSprite::initWithFile(const std::string& pszFilename, const DRect& rect)
{
    CAImage *image = CAImage::create(pszFilename);
    if (image)
    {
        return initWithImage(image, rect);
    }

    return false;
}

bool CGSprite::initWithSpriteFrame(CGSpriteFrame *pSpriteFrame)
{
    CCAssert(pSpriteFrame != NULL, "");
    
    bool bRet = this->initWithImage(pSpriteFrame->getImage(), pSpriteFrame->getRect());
    this->setSpriteFrame(pSpriteFrame);
    
    return bRet;
}

bool CGSprite::initWithSpriteFrameName(const std::string& pszSpriteFrameName)
{

    CGSpriteFrame *pFrame = CGSpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName);
    return initWithSpriteFrame(pFrame);
}

void CGSprite::setImageRect(const DRect& rect, bool rotated, const DSize& untrimmedSize)
{
    m_bRectRotated = rotated;
    
    if (m_obAutoContentSize == true)
    {
        this->setContentSize(untrimmedSize);
        m_obAutoContentSize = true;
    }
    this->setVertexRect(rect);
    this->setImageCoords(rect);
    
    if (m_obAutoContentSize == true)
    {
        DPoint relativeOffset = m_obUnflippedOffsetPositionFromCenter;
        
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
        
        float x1 = 0 + m_obOffsetPosition.x;
        float y1 = 0 + m_obOffsetPosition.y;
        float x2 = x1 + m_obRect.size.width;
        float y2 = y1 + m_obRect.size.height;
        
        // Don't update Z.
        m_sQuad.bl.vertices = DPoint3D(x1, y1, 0);
        m_sQuad.br.vertices = DPoint3D(x2, y1, 0);
        m_sQuad.tl.vertices = DPoint3D(x1, y2, 0);
        m_sQuad.tr.vertices = DPoint3D(x2, y2, 0);
    }
    else
    {
        this->updateImageRect();
    }
    this->updateDraw();
}

void CGSprite::setSpriteFrame(CGSpriteFrame *pNewFrame)
{
    if (pNewFrame)
    {
        m_obUnflippedOffsetPositionFromCenter = pNewFrame->getOffset();
        
        this->setImage(pNewFrame->getImage());
        
        this->setImageRect(pNewFrame->getRect(), pNewFrame->isRotated(), pNewFrame->getOriginalSize());
    }
}

void CGSprite::setDisplayFrameWithAnimationName(const std::string& animationName, int frameIndex)
{
    CCAssert(!animationName.empty(), "CGSprite#setDisplayFrameWithAnimationName. animationName must not be NULL");
    
    Animation *a = AnimationCache::getInstance()->getAnimation(animationName);
    
    CCAssert(a, "CGSprite#setDisplayFrameWithAnimationName: Frame not found");
    
    AnimationFrame* frame = a->getFrames().at(frameIndex);
    
    CCAssert(frame, "CGSprite#setDisplayFrame. Invalid frame");
    
    this->setSpriteFrame(frame->getSpriteFrame());
}

bool CGSprite::isFrameDisplayed(CGSpriteFrame *pFrame)
{
    DRect r = pFrame->getRect();
    
    return (r.equals(m_obRect) &&
            pFrame->getImage()->getName() == m_pobImage->getName() &&
            pFrame->getOffset().equals(m_obUnflippedOffsetPositionFromCenter));
}

CGSpriteFrame* CGSprite::getSpriteFrame(void)
{
    return CGSpriteFrame::createWithImage(m_pobImage,
                                            m_obRect,
                                            m_bRectRotated,
                                            m_obUnflippedOffsetPositionFromCenter,
                                            m_obContentSize);
}

void CGSprite::setImage(CAImage *image)
{
    CAView::setImage(image);
    if (image)
    {
        DRect rect = DRectZero;
        rect.size = image->getContentSize();
        CAView::setImageRect(rect);
        
        if (m_obAutoContentSize == true)
        {
            this->setContentSize(rect.size);
            m_obAutoContentSize = true;
        }
    }
}

CAImage* CGSprite::getImage()
{
    return m_pobImage;
}

DPoint3D CGSprite::getRotation3D() const
{
    CCAssert(m_fRotationX == m_fRotationY, "CGSprite#rotation. m_fRotationX != m_fRotationY. Don't know which one to return");
    return DPoint3D(m_fRotationX, m_fRotationY, m_fRotationZ);
}

void CGSprite::setRotation3D(const DPoint3D& rotation)
{
    if (rotation.x != m_fRotationX || rotation.y != m_fRotationY || rotation.z != m_fRotationZ)
    {
        m_fRotationX = rotation.x;
        m_fRotationY = rotation.y;
        m_fRotationZ = rotation.z;
        this->updateRotationQuat();
        this->updateDraw();
    }
}

void CGSprite::updateRotation3D()
{
    //convert quaternion to Euler angle
    float x = m_obRotationQuat.x, y = m_obRotationQuat.y, z = m_obRotationQuat.z, w = m_obRotationQuat.w;
    m_fRotationX = atan2f(2.f * (w * x + y * z), 1.f - 2.f * (x * x + y * y));
    float sy = 2.f * (w * y - z * x);
    sy = clampf(sy, -1, 1);
    m_fRotationY = asinf(sy);
    m_fRotationZ = atan2f(2.f * (w * z + x * y), 1.f - 2.f * (y * y + z * z));
    
    m_fRotationX = CC_RADIANS_TO_DEGREES(m_fRotationX);
    m_fRotationY = CC_RADIANS_TO_DEGREES(m_fRotationY);
    m_fRotationZ = -CC_RADIANS_TO_DEGREES(m_fRotationZ);
}

Quaternion CGSprite::getRotationQuat() const
{
    return m_obRotationQuat;
}

void CGSprite::setRotationQuat(const Quaternion& quat)
{
    m_obRotationQuat = quat;
    updateRotation3D();
    this->updateDraw();
}

const DPoint& CGSprite::getNormalizedPosition() const
{
    return m_obNormalizedPosition;
}

void CGSprite::setNormalizedPosition(const DPoint &position)
{
    if ( ! position.equals(m_obNormalizedPosition))
    {
        m_obNormalizedPosition = position;
        m_bUsingNormalizedPosition = true;
        this->updateDraw();
    }
}

const DPoint& CGSprite::getPosition()
{
    return m_obPoint;
}

void CGSprite::setPosition(const DPoint& position)
{
    m_obLayout = DLayoutZero;
    if( ! position.equals(m_obPoint))
    {
        this->setPoint(position);
        this->updateDraw();
        m_bUsingNormalizedPosition = false;
    }
    m_eLayoutType = -1;
    
}

float CGSprite::getPositionX() const
{
    return m_obPoint.x;
}

void CGSprite::setPositionX(float x)
{
    setPosition(DPoint(x, m_obPoint.y));
}

float CGSprite::getPositionY() const
{
    return  m_obPoint.y;
}

void CGSprite::setPositionY(float y)
{
    setPosition(DPoint(m_obPoint.x, y));
}

float CGSprite::getPositionZ() const
{
    return m_fPointZ;
}

void CGSprite::setPositionZ(float positionZ)
{
    if (positionZ != m_fPointZ)
    {
        m_fPointZ = positionZ;
        this->updateDraw();
    }
}

void CGSprite::setPosition3D(const DPoint3D& position)
{
    setPositionZ(position.z);
    setPosition(DPoint(position.x, position.y));
}

DPoint3D CGSprite::getPosition3D() const
{
    return DPoint3D(m_obPoint.x, m_obPoint.y, m_fPointZ);
}

const DSize& CGSprite::getContentSize()
{
    return m_obContentSize;
}

void CGSprite::setContentSize(const DSize& contentSize)
{
    if (!contentSize.equals(m_obContentSize))
    {
        CAView::setContentSize(contentSize);
    }
    m_obAutoContentSize = false;
}

void CGSprite::setScaleZ(float scaleZ)
{
    m_fScaleZ = scaleZ;
}

float CGSprite::getScaleZ() const
{
    return m_fScaleZ;
}

Action * CGSprite::runAction(Action* action)
{
    CCAssert( action != nullptr, "Argument must be non-nil");
    CAApplication::getApplication()->getActionManager()->addAction(action, this, !m_bRunning);
    return action;
}

void CGSprite::stopAllActions()
{
    CAApplication::getApplication()->getActionManager()->removeAllActionsFromTarget(this);
}

void CGSprite::stopAction(Action* action)
{
    CAApplication::getApplication()->getActionManager()->removeAction(action);
}

void CGSprite::stopActionByTag(int tag)
{
    CCAssert( tag != Action::INVALID_TAG, "Invalid tag");
    CAApplication::getApplication()->getActionManager()->removeActionByTag(tag, this);
}

void CGSprite::stopAllActionsByTag(int tag)
{
    CCAssert( tag != Action::INVALID_TAG, "Invalid tag");
    CAApplication::getApplication()->getActionManager()->removeAllActionsByTag(tag, this);
}

void CGSprite::stopActionsByFlags(unsigned int flags)
{
    if (flags > 0)
    {
        CAApplication::getApplication()->getActionManager()->removeActionsByFlags(flags, this);
    }
}

Action * CGSprite::getActionByTag(int tag)
{
    CCAssert( tag != Action::INVALID_TAG, "Invalid tag");
    return CAApplication::getApplication()->getActionManager()->getActionByTag(tag, this);
}

size_t CGSprite::getNumberOfRunningActions() const
{
    return CAApplication::getApplication()->getActionManager()->getNumberOfRunningActionsInTarget(this);
}

DRect CGSprite::boundingBox()
{
    DRect rect = DRect(0, 0, m_obContentSize.width, m_obContentSize.height);
    rect = RectApplyAffineTransform(rect, getViewToSuperviewAffineTransform());
    rect.origin.y += rect.size.height;
    if (this->getSuperview())
    {
        rect.origin.y = this->getSuperview()->m_obContentSize.height - rect.origin.y;
    }
    else
    {
        rect.origin.y = CAApplication::getApplication()->getWinSize().height - rect.origin.y;
    }
    return rect;
}

const Mat4& CGSprite::getViewToSuperviewTransform() const
{
    if (m_bTransformDirty)
    {
        // Translate values
        float width = 0;
        float height = 0;
        
        if (this->m_pSuperview)
        {
            width = this->m_pSuperview->m_obContentSize.width;
            height = this->m_pSuperview->m_obContentSize.height;
        }
        else
        {
            width = m_pApplication->getWinSize().width;
            height = m_pApplication->getWinSize().height;
        }
        
        float x = m_obPoint.x;
        if (m_bUsingNormalizedPosition) x = m_obNormalizedPosition.x * width;
        float y = (height - m_obPoint.y);
        if (m_bUsingNormalizedPosition) y = (1 - m_obNormalizedPosition.y) * height;
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


NS_CC_END

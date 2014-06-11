//
//  CAImageView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-18.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CCAnimation.h"
#include "CCAnimationCache.h"
#include "ccConfig.h"
#include "CAImageView.h"
#include "CCSpriteFrame.h"
#include "CCSpriteFrameCache.h"
#include "images/CAImageCache.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCGLProgram.h"
#include "CCDirector.h"
#include "support/CCPointExtension.h"
#include "cocoa/CCGeometry.h"
#include "images/CAImage.h"
#include "cocoa/CCAffineTransform.h"
#include "support/TransformUtils.h"
#include "support/CCProfiling.h"
#include "platform/CCImage.h"
// external
#include "kazmath/GL/matrix.h"
#include <string.h>

using namespace std;

NS_CC_BEGIN

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

CAImageView* CAImageView::createWithImage(CAImage* image)
{
    CAImageView *pobSprite = new CAImageView();
    if (pobSprite && pobSprite->initWithImage(image))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CAImageView* CAImageView::createWithImage(CAImage* image, const CCRect& rect)
{
    CAImageView *pobSprite = new CAImageView();
    if (pobSprite && pobSprite->initWithImage(image, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CAImageView* CAImageView::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    CAImageView *pobSprite = new CAImageView();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CAImageView* CAImageView::create()
{
    CAImageView *pSprite = new CAImageView();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool CAImageView::init(void)
{
    return initWithImage(NULL, CCRectZero);
}

bool CAImageView::initWithImage(CAImage* image, const CCRect& rect, bool rotated)
{
    if (!CAView::init())
        return false;

    this->setImage(image);
    setImageRect(rect, rotated, rect.size);
    
    return true;
}

bool CAImageView::initWithImage(CAImage* image, const CCRect& rect)
{
    return initWithImage(image, rect, false);
}

bool CAImageView::initWithImage(CAImage* image)
{
    CCRect rect = CCRectZero;
    
    if (image)
    {
        rect.size = image->getContentSize();
    }
    
    return initWithImage(image, rect);
}

bool CAImageView::initWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    CCAssert(pSpriteFrame != NULL, "");

    bool bRet = initWithImage(pSpriteFrame->getImage(), pSpriteFrame->getRect());
    setDisplayFrame(pSpriteFrame);

    return bRet;
}

CAImageView::CAImageView(void)
{
    
}

CAImageView::~CAImageView(void)
{
    
}

void CAImageView::setFrame(const CCRect &rect)
{
	this->setBounds(rect);

	this->setFrameOrigin(rect.origin);
}

void CAImageView::setFrameOrigin(const CCPoint& point)
{
    CCPoint p = CCPoint(m_obAnchorPointInPoints.x * m_fScaleX, m_obAnchorPointInPoints.y * m_fScaleY);
	p = ccpAdd(point, p);
	this->setPosition(p);
    
    m_bFrame = true;
}

void CAImageView::setCenter(const CCRect& rect)
{
    this->setBounds(rect);
    
    this->setCenterOrigin(rect.origin);
}

CCRect CAImageView::getCenter()
{
    CCRect rect = m_obFrameRect;
    rect.origin = ccpAdd(rect.origin, CCPoint(m_obAnchorPointInPoints.x * m_fScaleX, m_obAnchorPointInPoints.y * m_fScaleY));
    return rect;
}

void CAImageView::setCenterOrigin(const CCPoint& point)
{
    CCPoint p = CCPoint(m_obContentSize.width * m_fScaleX, m_obContentSize.height * m_fScaleY);
    p = ccpSub(point, p/2);
    p = ccpAdd(CCPoint(m_obAnchorPointInPoints.x * m_fScaleX, m_obAnchorPointInPoints.y * m_fScaleY), p);
    this->setPosition(p);
    
    m_bFrame = false;
}

CCPoint CAImageView::getCenterOrigin()
{
    return this->getCenter().origin;
    
}
void CAImageView::setBounds(const CCRect& rect)
{
	if (!rect.size.equals(CCSizeZero))
	{
		CCPoint scale = CCSize(rect.size.width / m_obContentSize.width, rect.size.height / m_obContentSize.height);
		this->setScale(scale.x, scale.y);
	}
}

CCRect CAImageView::getBounds() const
{
    CCRect rect = this->getFrame();
    rect.origin = CCPointZero;
    return rect;
}

void CAImageView::setDisplayFrame(CCSpriteFrame *pNewFrame)
{
    m_obUnflippedOffsetPositionFromCenter = pNewFrame->getOffset();

    CAImage* pNewimage = pNewFrame->getImage();

    if (pNewimage != m_pobImage)
    {
        setImage(pNewimage);
    }

    m_bRectRotated = pNewFrame->isRotated();
    setImageRect(pNewFrame->getRect(), m_bRectRotated, pNewFrame->getOriginalSize());
}

void CAImageView::setDisplayFrameWithAnimationName(const char *animationName, int frameIndex)
{
    CCAssert(animationName, "CCSprite#setDisplayFrameWithAnimationName. animationName must not be NULL");

    CCAnimation *a = CCAnimationCache::sharedAnimationCache()->animationByName(animationName);

    CCAssert(a, "CCSprite#setDisplayFrameWithAnimationName: Frame not found");

    CCAnimationFrame* frame = (CCAnimationFrame*)a->getFrames()->objectAtIndex(frameIndex);

    CCAssert(frame, "CCSprite#setDisplayFrame. Invalid frame");

    setDisplayFrame(frame->getSpriteFrame());
}

bool CAImageView::isFrameDisplayed(CCSpriteFrame *pFrame)
{
    CCRect r = pFrame->getRect();

    return (r.equals(m_obRect) &&
            pFrame->getImage()->getName() == m_pobImage->getName() &&
            pFrame->getOffset().equals(m_obUnflippedOffsetPositionFromCenter));
}

CCSpriteFrame* CAImageView::displayFrame(void)
{
    return CCSpriteFrame::createWithImage(m_pobImage,
                                           CC_RECT_POINTS_TO_PIXELS(m_obRect),
                                           m_bRectRotated,
                                           CC_POINT_POINTS_TO_PIXELS(m_obUnflippedOffsetPositionFromCenter),
                                           CC_SIZE_POINTS_TO_PIXELS(m_obContentSize));
}

void CAImageView::setImage(CAImage* image)
{
    if (NULL == image)
    {
        image = CAImage::CC_WHITE_IMAGE();
    }

    CCRect rect;
    if (this->isFrame())
    {
        rect = this->getFrame();
    }
    else
    {
        rect = this->getCenter();
    }
    
    CAView::setImage(image);
    
    if (this->isFrame())
    {
        this->setFrame(rect);
    }
    else
    {
        this->CAView::setCenter(rect);
    }
}

NS_CC_END

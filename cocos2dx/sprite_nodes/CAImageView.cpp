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
// external
#include <string.h>

NS_CC_BEGIN

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

CAImageView* CAImageView::createWithFrame(const CCRect& rect)
{
	CAImageView * pRet = new CAImageView();
    if (pRet && pRet->init())
    {
        pRet->setFrame(rect);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
	return NULL;
}

CAImageView* CAImageView::createWithCenter(const CCRect& rect)
{
	CAImageView * pRet = new CAImageView();
    if (pRet && pRet->init())
    {
        pRet->setCenter(rect);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
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
    return CAImageView::initWithImage(NULL);
}

bool CAImageView::initWithImage(CAImage* image)
{
    if (!CAView::init())
        return false;
    
    CCRect rect = CCRectZero;
    if (image)
    {
        rect.size = image->getContentSize();
    }
    this->setImage(image);
    setImageRect(rect, false, rect.size);
    
    return true;
}

bool CAImageView::initWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    if (!CAView::init())
        return false;
    
    setDisplayFrame(pSpriteFrame);
    return true;
}

CAImageView::CAImageView(void)
{
    
}

CAImageView::~CAImageView(void)
{
    
}

void CAImageView::setImage(CAImage* image)
{
    if (NULL == image)
    {
        image = CAImage::CC_WHITE_IMAGE();
    }
    CAView::setImage(image);
    
    if (image->isMonochrome() == false)
    {
         CCRect rect = CCRectZero;
        rect.size = image->getContentSize();
        this->setImageRect(rect, false, m_obContentSize);
    }
    else
    {
        this->setImageRect(CCRectZero);
    }
    
}

void CAImageView::updateImageRect()
{
    CCSize size = CCSizeZero;
    if (m_pobImage && !m_pobImage->isMonochrome())
    {
        size = m_obContentSize;
    }
    
    // Don't update Z.
    m_sQuad.bl.vertices = vertex3(0, 0, 0);
    m_sQuad.br.vertices = vertex3(size.width, 0, 0);
    m_sQuad.tl.vertices = vertex3(0, size.height, 0);
    m_sQuad.tr.vertices = vertex3(size.width, size.height, 0);
}

void CAImageView::setDisplayFrame(CCSpriteFrame *pNewFrame)
{
    CC_RETURN_IF(!pNewFrame);
    m_obUnflippedOffsetPositionFromCenter = pNewFrame->getOffset();

    CAImage* pNewimage = pNewFrame->getImage();

    if (pNewimage != m_pobImage)
    {
        setImage(pNewimage);
    }

    m_bRectRotated = pNewFrame->isRotated();
    
    CCSize size = m_obContentSize;
    if (size.equals(CCSizeZero))
    {
        size = pNewFrame->getOriginalSize();
    }
    setImageRect(pNewFrame->getRect(), m_bRectRotated, size);
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

NS_CC_END

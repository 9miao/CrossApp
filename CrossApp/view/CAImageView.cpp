//
//  CAImageView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-18.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#include "CAImageView.h"
#include "ccConfig.h"
#include "images/CAImage.h"
#include "images/CAImageFrame.h"
#include "images/CAImageFrameCache.h"
#include "images/CAImageCache.h"
#include "images/CAAnimation.h"
#include "images/CAAnimationCache.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "shaders/CAShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CAGLProgram.h"
#include "basics/CAApplication.h"
#include "support/CCPointExtension.h"
#include "basics/CAGeometry.h"
#include "cocoa/CCAffineTransform.h"
#include "support/TransformUtils.h"
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

CAImageView* CAImageView::createWithSpriteFrame(CAImageFrame *pSpriteFrame)
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

bool CAImageView::initWithSpriteFrame(CAImageFrame *pSpriteFrame)
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

CAImage* CAImageView::getImage()
{
    return CAView::getImage();
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

void CAImageView::setDisplayFrame(CAImageFrame *pNewFrame)
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

    CAAnimation *a = CAAnimationCache::sharedAnimationCache()->animationByName(animationName);

    CCAssert(a, "CCSprite#setDisplayFrameWithAnimationName: Frame not found");

    CAAnimationFrame* frame = (CAAnimationFrame*)a->getFrames()->objectAtIndex(frameIndex);

    CCAssert(frame, "CCSprite#setDisplayFrame. Invalid frame");

    setDisplayFrame(frame->getSpriteFrame());
}

bool CAImageView::isFrameDisplayed(CAImageFrame *pFrame)
{
    CCRect r = pFrame->getRect();

    return (r.equals(m_obRect) &&
            pFrame->getImage()->getName() == m_pobImage->getName() &&
            pFrame->getOffset().equals(m_obUnflippedOffsetPositionFromCenter));
}

CAImageFrame* CAImageView::displayFrame(void)
{
    return CAImageFrame::createWithImage(m_pobImage,
                                           CC_RECT_POINTS_TO_PIXELS(m_obRect),
                                           m_bRectRotated,
                                           CC_POINT_POINTS_TO_PIXELS(m_obUnflippedOffsetPositionFromCenter),
                                           CC_SIZE_POINTS_TO_PIXELS(m_obContentSize));
}

bool CAImageView::initWithFrame(const CCRect& rect, const CAColor4B& color4B)
{
    return CAView::initWithFrame(rect);
}

bool CAImageView::initWithCenter(const CCRect& rect, const CAColor4B& color4B)
{
    return CAView::initWithCenter(rect);
}

void CAImageView::setImageAsyncWithFile(const char* fileName)
{
    CAImageCache::sharedImageCache()->addImageAsync(fileName, this, callfuncO_selector(CAImageView::asyncFinish));
}

void CAImageView::asyncFinish(CrossApp::CAObject *var)
{
    CAImage* image = dynamic_cast<CAImage*>(var);
    this->setImage(image);
}

NS_CC_END

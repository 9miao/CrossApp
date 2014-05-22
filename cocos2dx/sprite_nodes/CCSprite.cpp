/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCAnimation.h"
#include "CCAnimationCache.h"
#include "ccConfig.h"
#include "CCSprite.h"
#include "CCSpriteFrame.h"
#include "CCSpriteFrameCache.h"
#include "textures/CCTextureCache.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCGLProgram.h"
#include "CCDirector.h"
#include "support/CCPointExtension.h"
#include "cocoa/CCGeometry.h"
#include "textures/CCTexture2D.h"
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

CAImageView* CAImageView::createWithImage(CAImage* Image)
{
    CAImageView *pobSprite = new CAImageView();
    if (pobSprite && pobSprite->initWithImage(Image))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CAImageView* CAImageView::createWithImage(CAImage* Image, const CCRect& rect)
{
    CAImageView *pobSprite = new CAImageView();
    if (pobSprite && pobSprite->initWithImage(Image, rect))
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

// designated initializer
bool CAImageView::initWithImage(CAImage* Image, const CCRect& rect, bool rotated)
{
    if (CAView::init())
    {
        
        m_bRecursiveDirty = false;
        setDirty(false);
        
        m_bOpacityModifyRGB = true;
        
        m_sBlendFunc.src = CC_BLEND_SRC;
        m_sBlendFunc.dst = CC_BLEND_DST;
        
        m_bFlipX = m_bFlipY = false;
        
        // default transform anchor: center
        setAnchorPoint(ccp(0.5f, 0.5f));
        
        // zwoptex default values
        m_obOffsetPosition = CCPointZero;
        
        m_bHasChildren = false;
        
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
        
        this->setImage(Image);
        setImageRect(rect, rotated, rect.size);
        // by default use "Self Render".

        return true;
    }
    else
    {
        return false;
    }
}

bool CAImageView::initWithImage(CAImage* Image, const CCRect& rect)
{
    return initWithImage(Image, rect, false);
}

bool CAImageView::initWithImage(CAImage* Image)
{
    CCAssert(Image != NULL, "Invalid image for sprite");

    CCRect rect = CCRectZero;
    rect.size = Image->getContentSize();
    
    return initWithImage(Image, rect);
}

bool CAImageView::initWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    CCAssert(pSpriteFrame != NULL, "");

    bool bRet = initWithImage(pSpriteFrame->getImage(), pSpriteFrame->getRect());
    setDisplayFrame(pSpriteFrame);

    return bRet;
}

// XXX: deprecated
/*
CAImageView* CAImageView::initWithCGImage(CGImageRef pImage)
{
    // todo
    // because it is deprecated, so we do not implement it

    return NULL;
}
*/

/*
CAImageView* CAImageView::initWithCGImage(CGImageRef pImage, const char *pszKey)
{
    CCAssert(pImage != NULL);

    // XXX: possible bug. See issue #349. New API should be added
    CAImage* Image = CCTextureCache::sharedTextureCache()->addCGImage(pImage, pszKey);

    const CCSize& size = Image->getContentSize();
    CCRect rect = CCRectMake(0 ,0, size.width, size.height);

    return initWithImage(image, rect);
}
*/

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

// Frames

void CAImageView::setDisplayFrame(CCSpriteFrame *pNewFrame)
{
    m_obUnflippedOffsetPositionFromCenter = pNewFrame->getOffset();

    CAImage* pNewimage = pNewFrame->getImage();
    // update image before updating image rect
    if (pNewimage != m_pobImage)
    {
        setImage(pNewimage);
    }

    // update rect
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

    CAView::setImage(image);
}

NS_CC_END

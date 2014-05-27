/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2011 Ricardo Quesada
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
#include "images/CAImageCache.h"
#include "CCSpriteFrame.h"
#include "CCDirector.h"

NS_CC_BEGIN

// implementation of CCSpriteFrame

CCSpriteFrame* CCSpriteFrame::create(const char* filename, const CCRect& rect)
{
    CCSpriteFrame *pSpriteFrame = new CCSpriteFrame();;
    pSpriteFrame->initWithimageFilename(filename, rect);
    pSpriteFrame->autorelease();

    return pSpriteFrame;
}

CCSpriteFrame* CCSpriteFrame::createWithImage(CAImage* pobImage, const CCRect& rect)
{
    CCSpriteFrame *pSpriteFrame = new CCSpriteFrame();;
    pSpriteFrame->initWithImage(pobImage, rect);
    pSpriteFrame->autorelease();
    
    return pSpriteFrame;
}

CCSpriteFrame* CCSpriteFrame::createWithImage(CAImage* pobImage, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize)
{
    CCSpriteFrame *pSpriteFrame = new CCSpriteFrame();;
    pSpriteFrame->initWithImage(pobImage, rect, rotated, offset, originalSize);
    pSpriteFrame->autorelease();

    return pSpriteFrame;
}

CCSpriteFrame* CCSpriteFrame::create(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize)
{
    CCSpriteFrame *pSpriteFrame = new CCSpriteFrame();;
    pSpriteFrame->initWithimageFilename(filename, rect, rotated, offset, originalSize);
    pSpriteFrame->autorelease();

    return pSpriteFrame;
}

bool CCSpriteFrame::initWithImage(CAImage* pobImage, const CCRect& rect)
{
    CCRect rectInPixels = CC_RECT_POINTS_TO_PIXELS(rect);
    return initWithImage(pobImage, rectInPixels, false, CCPointZero, rectInPixels.size);
}

bool CCSpriteFrame::initWithimageFilename(const char* filename, const CCRect& rect)
{
    CCRect rectInPixels = CC_RECT_POINTS_TO_PIXELS( rect );
    return initWithimageFilename(filename, rectInPixels, false, CCPointZero, rectInPixels.size);
}

bool CCSpriteFrame::initWithImage(CAImage* pobImage, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize)
{
    m_pobImage = pobImage;

    if (pobImage)
    {
        pobImage->retain();
    }

    m_obRectInPixels = rect;
    m_obRect = CC_RECT_PIXELS_TO_POINTS(rect);
    m_obOffsetInPixels = offset;
    m_obOffset = CC_POINT_PIXELS_TO_POINTS( m_obOffsetInPixels );
    m_obOriginalSizeInPixels = originalSize;
    m_obOriginalSize = CC_SIZE_PIXELS_TO_POINTS( m_obOriginalSizeInPixels );
    m_bRotated = rotated;

    return true;
}

bool CCSpriteFrame::initWithimageFilename(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize)
{
    m_pobImage = NULL;
    m_strimageFilename = filename;
    m_obRectInPixels = rect;
    m_obRect = CC_RECT_PIXELS_TO_POINTS( rect );
    m_obOffsetInPixels = offset;
    m_obOffset = CC_POINT_PIXELS_TO_POINTS( m_obOffsetInPixels );
    m_obOriginalSizeInPixels = originalSize;
    m_obOriginalSize = CC_SIZE_PIXELS_TO_POINTS( m_obOriginalSizeInPixels );
    m_bRotated = rotated;

    return true;
}

CCSpriteFrame::~CCSpriteFrame(void)
{
    CCLOGINFO("cocos2d: deallocing %p", this);
    CC_SAFE_RELEASE(m_pobImage);
}

CCObject* CCSpriteFrame::copyWithZone(CCZone *pZone)
{
    CC_UNUSED_PARAM(pZone);
    CCSpriteFrame *pCopy = new CCSpriteFrame();
    
    pCopy->initWithimageFilename(m_strimageFilename.c_str(), m_obRectInPixels, m_bRotated, m_obOffsetInPixels, m_obOriginalSizeInPixels);
    pCopy->setImage(m_pobImage);
    return pCopy;
}

void CCSpriteFrame::setRect(const CCRect& rect)
{
    m_obRect = rect;
    m_obRectInPixels = CC_RECT_POINTS_TO_PIXELS(m_obRect);
}

void CCSpriteFrame::setRectInPixels(const CCRect& rectInPixels)
{
    m_obRectInPixels = rectInPixels;
    m_obRect = CC_RECT_PIXELS_TO_POINTS(rectInPixels);
}

const CCPoint& CCSpriteFrame::getOffset(void)
{
    return m_obOffset;
}

void CCSpriteFrame::setOffset(const CCPoint& offsets)
{
    m_obOffset = offsets;
    m_obOffsetInPixels = CC_POINT_POINTS_TO_PIXELS( m_obOffset );
}

const CCPoint& CCSpriteFrame::getOffsetInPixels(void)
{
    return m_obOffsetInPixels;
}

void CCSpriteFrame::setOffsetInPixels(const CCPoint& offsetInPixels)
{
    m_obOffsetInPixels = offsetInPixels;
    m_obOffset = CC_POINT_PIXELS_TO_POINTS( m_obOffsetInPixels );
}

void CCSpriteFrame::setImage(CAImage*  image)
{
    if( m_pobImage != image ) {
        CC_SAFE_RELEASE(m_pobImage);
        CC_SAFE_RETAIN(image);
        m_pobImage = image;
    }
}

CAImage* CCSpriteFrame::getImage(void)
{
    if( m_pobImage ) {
        return m_pobImage;
    }

    if( m_strimageFilename.length() > 0 ) {
        return CAImageCache::sharedImageCache()->addImage(m_strimageFilename.c_str());
    }
    // no image or image filename
    return NULL;
}

NS_CC_END


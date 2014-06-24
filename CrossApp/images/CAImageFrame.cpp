
#include "CAImageCache.h"
#include "CAImageFrame.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

// implementation of CAImageFrame

CAImageFrame* CAImageFrame::create(const char* filename, const CCRect& rect)
{
    CAImageFrame *pSpriteFrame = new CAImageFrame();;
    pSpriteFrame->initWithimageFilename(filename, rect);
    pSpriteFrame->autorelease();

    return pSpriteFrame;
}

CAImageFrame* CAImageFrame::createWithImage(CAImage* pobImage, const CCRect& rect)
{
    CAImageFrame *pSpriteFrame = new CAImageFrame();;
    pSpriteFrame->initWithImage(pobImage, rect);
    pSpriteFrame->autorelease();
    
    return pSpriteFrame;
}

CAImageFrame* CAImageFrame::createWithImage(CAImage* pobImage, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize)
{
    CAImageFrame *pSpriteFrame = new CAImageFrame();;
    pSpriteFrame->initWithImage(pobImage, rect, rotated, offset, originalSize);
    pSpriteFrame->autorelease();

    return pSpriteFrame;
}

CAImageFrame* CAImageFrame::create(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize)
{
    CAImageFrame *pSpriteFrame = new CAImageFrame();;
    pSpriteFrame->initWithimageFilename(filename, rect, rotated, offset, originalSize);
    pSpriteFrame->autorelease();

    return pSpriteFrame;
}

bool CAImageFrame::initWithImage(CAImage* pobImage, const CCRect& rect)
{
    CCRect rectInPixels = CC_RECT_POINTS_TO_PIXELS(rect);
    return initWithImage(pobImage, rectInPixels, false, CCPointZero, rectInPixels.size);
}

bool CAImageFrame::initWithimageFilename(const char* filename, const CCRect& rect)
{
    CCRect rectInPixels = CC_RECT_POINTS_TO_PIXELS( rect );
    return initWithimageFilename(filename, rectInPixels, false, CCPointZero, rectInPixels.size);
}

bool CAImageFrame::initWithImage(CAImage* pobImage, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize)
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

bool CAImageFrame::initWithimageFilename(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize)
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

CAImageFrame::~CAImageFrame(void)
{
    CCLOGINFO("CrossApp: deallocing %p", this);
    CC_SAFE_RELEASE(m_pobImage);
}

CAObject* CAImageFrame::copyWithZone(CAZone *pZone)
{
    CC_UNUSED_PARAM(pZone);
    CAImageFrame *pCopy = new CAImageFrame();
    
    pCopy->initWithimageFilename(m_strimageFilename.c_str(), m_obRectInPixels, m_bRotated, m_obOffsetInPixels, m_obOriginalSizeInPixels);
    pCopy->setImage(m_pobImage);
    return pCopy;
}

void CAImageFrame::setRect(const CCRect& rect)
{
    m_obRect = rect;
    m_obRectInPixels = CC_RECT_POINTS_TO_PIXELS(m_obRect);
}

void CAImageFrame::setRectInPixels(const CCRect& rectInPixels)
{
    m_obRectInPixels = rectInPixels;
    m_obRect = CC_RECT_PIXELS_TO_POINTS(rectInPixels);
}

const CCPoint& CAImageFrame::getOffset(void)
{
    return m_obOffset;
}

void CAImageFrame::setOffset(const CCPoint& offsets)
{
    m_obOffset = offsets;
    m_obOffsetInPixels = CC_POINT_POINTS_TO_PIXELS( m_obOffset );
}

const CCPoint& CAImageFrame::getOffsetInPixels(void)
{
    return m_obOffsetInPixels;
}

void CAImageFrame::setOffsetInPixels(const CCPoint& offsetInPixels)
{
    m_obOffsetInPixels = offsetInPixels;
    m_obOffset = CC_POINT_PIXELS_TO_POINTS( m_obOffsetInPixels );
}

void CAImageFrame::setImage(CAImage*  image)
{
    if( m_pobImage != image ) {
        CC_SAFE_RELEASE(m_pobImage);
        CC_SAFE_RETAIN(image);
        m_pobImage = image;
    }
}

CAImage* CAImageFrame::getImage(void)
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


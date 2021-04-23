

#include "images/CAImageCache.h"
#include "CGSpriteFrame.h"

NS_CC_BEGIN

// implementation of CGSpriteFrame

CGSpriteFrame::CGSpriteFrame()
: _rotated(false)
, _image(nullptr)
{
    
}

CGSpriteFrame::~CGSpriteFrame()
{
    CC_SAFE_RELEASE(_image);
}

CGSpriteFrame* CGSpriteFrame::create(const std::string& filename, const DRect& rect)
{
    CGSpriteFrame *spriteFrame = new (std::nothrow) CGSpriteFrame();
    spriteFrame->initWithImageFilename(filename, rect);
    spriteFrame->autorelease();

    return spriteFrame;
}

CGSpriteFrame* CGSpriteFrame::createWithImage(CAImage* image, const DRect& rect)
{
    CGSpriteFrame *spriteFrame = new (std::nothrow) CGSpriteFrame();
    spriteFrame->initWithImage(image, rect);
    spriteFrame->autorelease();
    
    return spriteFrame;
}

CGSpriteFrame* CGSpriteFrame::createWithImage(CAImage* image, const DRect& rect, bool rotated, const DPoint& offset, const DSize& originalSize)
{
    CGSpriteFrame *spriteFrame = new (std::nothrow) CGSpriteFrame();
    spriteFrame->initWithImage(image, rect, rotated, offset, originalSize);
    spriteFrame->autorelease();

    return spriteFrame;
}

CGSpriteFrame* CGSpriteFrame::create(const std::string& filename, const DRect& rect, bool rotated, const DPoint& offset, const DSize& originalSize)
{
    CGSpriteFrame *spriteFrame = new (std::nothrow) CGSpriteFrame();
    spriteFrame->initWithImageFilename(filename, rect, rotated, offset, originalSize);
    spriteFrame->autorelease();

    return spriteFrame;
}

bool CGSpriteFrame::initWithImage(CAImage* image, const DRect& rect)
{
    return initWithImage(image, rect, false, DPointZero, rect.size);
}

bool CGSpriteFrame::initWithImageFilename(const std::string& filename, const DRect& rect)
{
    return initWithImageFilename(filename, rect, false, DPointZero, rect.size);
}

bool CGSpriteFrame::initWithImage(CAImage* image, const DRect& rect, bool rotated, const DPoint& offset, const DSize& originalSize)
{
    this->setImage(image);

    _rect = rect;
    _offset = offset;
    _originalSize = originalSize;
    _rotated = rotated;
    _anchorPoint = DPoint(NAN, NAN);

    return true;
}

bool CGSpriteFrame::initWithImageFilename(const std::string& filename, const DRect& rect, bool rotated, const DPoint& offset, const DSize& originalSize)
{
    _image = nullptr;
    _imageFilename = filename;
    _rect = rect;
    _offset = offset;
    _originalSize = originalSize;
    _rotated = rotated;
    _rotated = rotated;
    _anchorPoint = DPoint(NAN, NAN);

    return true;
}

CGSpriteFrame* CGSpriteFrame::clone() const
{
    CGSpriteFrame *copy = new (std::nothrow) CGSpriteFrame();
    copy->initWithImageFilename(_imageFilename, _rect, _rotated, _offset, _originalSize);
    copy->setImage(_image);
    copy->autorelease();
    return copy;
}

void CGSpriteFrame::setRect(const DRect& rect)
{
    _rect = rect;
}

const DPoint& CGSpriteFrame::getOffset() const
{
    return _offset;
}

void CGSpriteFrame::setOffset(const DPoint& offsets)
{
    _offset = offsets;
}

const DPoint& CGSpriteFrame::getAnchorPoint() const
{
    return _anchorPoint;
}

void CGSpriteFrame::setAnchorPoint(const DPoint& anchorPoint)
{
    _anchorPoint = anchorPoint;
}

bool CGSpriteFrame::hasAnchorPoint() const
{
    return !isnan(_anchorPoint.x);
}

void CGSpriteFrame::setImage(CAImage * image)
{
    CC_SAFE_RETAIN(image);
    CC_SAFE_RELEASE(_image);
    _image = image;
}

CAImage* CGSpriteFrame::getImage()
{
    if( _image )
    {
        return _image;
    }

    if( !_imageFilename.empty())
    {
        this->setImage(CAImage::create(_imageFilename));
        return _image;
    }
    
    return nullptr;
}

NS_CC_END

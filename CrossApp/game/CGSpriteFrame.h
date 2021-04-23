

#ifndef __SPRITE_CCSPRITE_FRAME_H__
#define __SPRITE_CCSPRITE_FRAME_H__

#include "game/CGSprite.h"
#include "basics/CAObject.h"
#include "ccTypes.h"

NS_CC_BEGIN

class CAImage;

class CC_DLL CGSpriteFrame : public CAObject, public CAClonable
{
public:

    CGSpriteFrame();
    
    virtual ~CGSpriteFrame();
    
    static CGSpriteFrame* create(const std::string& filename, const DRect& rect);

    static CGSpriteFrame* create(const std::string& filename, const DRect& rect, bool rotated, const DPoint& offset, const DSize& originalSize);

    static CGSpriteFrame* createWithImage(CAImage* image, const DRect& rect);

    static CGSpriteFrame* createWithImage(CAImage* image, const DRect& rect, bool rotated, const DPoint& offset, const DSize& originalSize);

    bool initWithImage(CAImage* image, const DRect& rect);
    
    bool initWithImageFilename(const std::string& filename, const DRect& rect);
    
    bool initWithImage(CAImage* image, const DRect& rect, bool rotated, const DPoint& offset, const DSize& originalSize);
    
    bool initWithImageFilename(const std::string& filename, const DRect& rect, bool rotated, const DPoint& offset, const DSize& originalSize);

    inline bool isRotated() const { return _rotated; }

    inline void setRotated(bool rotated) { _rotated = rotated; }

    inline const DRect& getRect() const { return _rect; }

    void setRect(const DRect& rect);

    inline const DSize& getOriginalSize() const { return _originalSize; }

    inline void setOriginalSize(const DSize& sizeInPixels) { _originalSize = sizeInPixels; }

    CAImage* getImage();

    void setImage(CAImage* image);

    const DPoint& getOffset() const;

    void setOffset(const DPoint& offsets);

    const DPoint& getAnchorPoint() const;

    void setAnchorPoint(const DPoint& anchorPoint);

    bool hasAnchorPoint() const;

	virtual CGSpriteFrame *clone() const override;

protected:
    
    DPoint      _offset;
    DPoint      _anchorPoint;
    DSize       _originalSize;
    bool        _rotated;
    DRect       _rect;
    CAImage*    _image;
    std::string  _imageFilename;
};

NS_CC_END

#endif //__SPRITE_CCSPRITE_FRAME_H__

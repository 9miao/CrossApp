

#ifndef __SPRITE_CCSPRITE_FRAME_H__
#define __SPRITE_CCSPRITE_FRAME_H__

#include "view/CAView.h"
#include "basics/CAObject.h"
#include "basics/CAGeometry.h"

NS_CC_BEGIN

class CAImage;
class CAZone;

/**
 * @addtogroup sprite_nodes
 * @{
 */

/** @brief A CAImageFrame has:
    - image: A CCimage2D that will be used by the CCSprite
    - rectangle: A rectangle of the image


 You can modify the frame of a CAImageView by doing:
 
    CAImageFrame *frame = CAImageFrame::frameWithImage(image, rect, offset);
    sprite->setDisplayFrame(frame);
 */
class CC_DLL CAImageFrame : public CAObject
{
public:
    // attributes

    inline const CCRect& getRectInPixels(void) { return m_obRectInPixels; }
    void setRectInPixels(const CCRect& rectInPixels);

    inline bool isRotated(void) { return m_bRotated; }
    inline void setRotated(bool bRotated) { m_bRotated = bRotated; }

    /** get rect of the frame */
    inline const CCRect& getRect(void) { return m_obRect; }
    /** set rect of the frame */
    void setRect(const CCRect& rect);

    /** get offset of the frame */
    const CCPoint& getOffsetInPixels(void);
    /** set offset of the frame */
    void setOffsetInPixels(const CCPoint& offsetInPixels);

    /** get original size of the trimmed image */
    inline const CCSize& getOriginalSizeInPixels(void) { return m_obOriginalSizeInPixels; }
    /** set original size of the trimmed image */
    inline void setOriginalSizeInPixels(const CCSize& sizeInPixels) { m_obOriginalSizeInPixels = sizeInPixels; }

    /** get original size of the trimmed image */
    inline const CCSize& getOriginalSize(void) { return m_obOriginalSize; }
    /** set original size of the trimmed image */
    inline void setOriginalSize(const CCSize& sizeInPixels) { m_obOriginalSize = sizeInPixels; }

    /** get image of the frame */
    CAImage* getImage(void);
    /** set image of the frame, the image is retained */
    void setImage(CAImage* pobImage);

    const CCPoint& getOffset(void);
    void setOffset(const CCPoint& offsets);

public:
    /**
     *  @js NA
     *  @lua NA
     */
    ~CAImageFrame(void);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);

    /** Create a CAImageFrame with a image filename, rect in points.
     It is assumed that the frame was not trimmed.
     */
    static CAImageFrame* create(const char* filename, const CCRect& rect);
    
    /** Create a CAImageFrame with a image filename, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in pixels of the frame before being trimmed.
     */
    static CAImageFrame* create(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize);
    
    /** Create a CAImageFrame with a image, rect in points.
     It is assumed that the frame was not trimmed.
     */
    static CAImageFrame* createWithImage(CAImage* pobImage, const CCRect& rect);

    /** Create a CAImageFrame with a image, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in points of the frame before being trimmed.
     */
    static CAImageFrame* createWithImage(CAImage* pobImage, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize);

public:
    /** Initializes a CAImageFrame with a image, rect in points.
     It is assumed that the frame was not trimmed.
     */
    bool initWithImage(CAImage* pobImage, const CCRect& rect);

    /** Initializes a CAImageFrame with a image filename, rect in points;
     It is assumed that the frame was not trimmed.
     */
    bool initWithimageFilename(const char* filename, const CCRect& rect);

    /** Initializes a CAImageFrame with a image, rect, rotated, offset and originalSize in pixels.
    The originalSize is the size in points of the frame before being trimmed.
    */
    bool initWithImage(CAImage* pobImage, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize);

    /** Initializes a CAImageFrame with a image, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in pixels of the frame before being trimmed.

     @since v1.1
     */
    bool initWithimageFilename(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize);


protected:
    CCPoint m_obOffset;
    CCSize m_obOriginalSize;
    CCRect m_obRectInPixels;
    bool   m_bRotated;
    CCRect m_obRect;
    CCPoint m_obOffsetInPixels;
    CCSize m_obOriginalSizeInPixels;
    CAImage* m_pobImage;
    std::string  m_strimageFilename;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif //__SPRITE_CCSPRITE_FRAME_H__

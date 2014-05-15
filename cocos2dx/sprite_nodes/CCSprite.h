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

#ifndef __SPITE_NODE_CCSPRITE_H__
#define __SPITE_NODE_CCSPRITE_H__

#include "sprite_nodes/CAView.h"
#include "CCProtocols.h"
#include "ccTypes.h"
#include "cocoa/CCDictionary.h"
#include <string>
#ifdef EMSCRIPTEN
#include "base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

class CCSpriteBatchNode;
class CCSpriteFrame;
class CCAnimation;
class CCRect;
class CCPoint;
class CCSize;
class CCTexture2D;
struct transformValues_;

/**
 * @addtogroup sprite_nodes
 * @{
 */

#define CCSpriteIndexNotInitialized 0xffffffff     /// CCSprite invalid index on the CCSpriteBatchNode


/** 
 * CCSprite is a 2d image ( http://en.wikipedia.org/wiki/Sprite_(computer_graphics) )
 *
 * CCSprite can be created with an image, or with a sub-rectangle of an image.
 *
 * If the parent or any of its ancestors is a CCSpriteBatchNode then the following features/limitations are valid
 *    - Features when the parent is a CCBatchNode:
 *        - MUCH faster rendering, specially if the CCSpriteBatchNode has many children. All the children will be drawn in a single batch.
 *
 *    - Limitations
 *        - Camera is not supported yet (eg: CCOrbitCamera action doesn't work)
 *        - GridBase actions are not supported (eg: CCLens, CCRipple, CCTwirl)
 *        - The Alias/Antialias property belongs to CCSpriteBatchNode, so you can't individually set the aliased property.
 *        - The Blending function property belongs to CCSpriteBatchNode, so you can't individually set the blending function property.
 *        - Parallax scroller is not supported, but can be simulated with a "proxy" sprite.
 *
 *  If the parent is an standard CCNode, then CCSprite behaves like any other CCNode:
 *    - It supports blending functions
 *    - It supports aliasing / antialiasing
 *    - But the rendering will be slower: 1 draw per children.
 *
 * The default anchorPoint in CCSprite is (0.5, 0.5).
 */
class CCSprite;
typedef CCSprite CAImageView;
class CC_DLL CCSprite : public CCNodeRGBA, public CCTextureProtocol
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
public:
    /// @{
    /// @name Creators
    
    /**
     * Creates an empty sprite without texture. You can call setTexture method subsequently.
     *
     * @return An empty sprite object that is marked as autoreleased.
     */
    static CCSprite* create();
    
    /**
     * Creates a sprite with an exsiting texture contained in a CCTexture2D object
     * After creation, the rect will be the size of the texture, and the offset will be (0,0).
     *
     * @param   pTexture    A pointer to a CCTexture2D object.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static CCSprite* createWithTexture(CCTexture2D *pTexture);
    
    /**
     * Creates a sprite with a texture and a rect.
     *
     * After creation, the offset will be (0,0).
     *
     * @param   pTexture    A pointer to an existing CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @param   rect        Only the contents inside the rect of this texture will be applied for this sprite.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static CCSprite* createWithTexture(CCTexture2D *pTexture, const CCRect& rect);
    
    /**
     * Creates a sprite with an sprite frame.
     *
     * @param   pSpriteFrame    A sprite frame which involves a texture and a rect
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static CCSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    
    /// @}  end of creators group
    
    
    
    /// @{
    /// @name Initializers
    
    /**
     * Default constructor
     * @js ctor
     */
    CCSprite(void);
    
    /**
     * Default destructor
     * @js NA
     * @lua NA
     */
    virtual ~CCSprite(void);
    
    /**
     * Initializes an empty sprite with nothing init.
     */
    virtual bool init(void);
    
    /**
     * Initializes a sprite with a texture.
     *
     * After initialization, the rect used will be the size of the texture, and the offset will be (0,0).
     *
     * @param   pTexture    A pointer to an existing CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(CCTexture2D *pTexture);
    
    /**
     * Initializes a sprite with a texture and a rect.
     *
     * After initialization, the offset will be (0,0).
     *
     * @param   pTexture    A pointer to an exisiting CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @param   rect        Only the contents inside rect of this texture will be applied for this sprite.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect);
    
    /**
     * Initializes a sprite with a texture and a rect in points, optionally rotated.
     *
     * After initialization, the offset will be (0,0).
     * @note    This is the designated initializer.
     *
     * @param   pTexture    A CCTexture2D object whose texture will be applied to this sprite.
     * @param   rect        A rectangle assigned the contents of texture.
     * @param   rotated     Whether or not the texture rectangle is rotated.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated);
    
    /**
     * Initializes a sprite with an SpriteFrame. The texture and rect in SpriteFrame will be applied on this sprite
     *
     * @param   pSpriteFrame  A CCSpriteFrame object. It should includes a valid texture and a rect
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    
    /// @} end of initializers
    
    /// @{
    /// @name Functions inherited from CCTextureProtocol
    virtual void setTexture(CCTexture2D *texture);
    virtual CCTexture2D* getTexture(void);
    inline void setBlendFunc(ccBlendFunc blendFunc) { m_sBlendFunc = blendFunc; }
    /**
     * @js NA
     */
    inline ccBlendFunc getBlendFunc(void) { return m_sBlendFunc; }
    /// @}

    /// @{
    /// @name Functions inherited from CCNode

    virtual void draw(void);
    /// @}
    
    /// @{
    /// @name Functions inherited from CCNodeRGBA
    virtual void setColor(const ccColor3B& color3);
    virtual void updateDisplayedColor(const ccColor3B& parentColor);
    virtual void setOpacity(GLubyte opacity);
    virtual void setOpacityModifyRGB(bool modify);
    virtual bool isOpacityModifyRGB(void);
    virtual void updateDisplayedOpacity(GLubyte parentOpacity);
    /// @}

    
    /// @{
    /// @name BatchNode methods
    
    /**
     * Updates the quad according the rotation, position, scale values. 
     */
    virtual void updateTransform(void);
    
    /// @{
    /// @name Texture methods
    
    /**
     * Updates the texture rect of the CCSprite in points.
     * It will call setTextureRect:rotated:untrimmedSize with rotated = NO, and utrimmedSize = rect.size.
     */
    virtual void setTextureRect(const CCRect& rect);
    
    /**
     * Sets the texture rect, rectRotated and untrimmed size of the CCSprite in points.
     * It will update the texture coordinates and the vertex rectangle.
     */
    virtual void setTextureRect(const CCRect& rect, bool rotated, const CCSize& untrimmedSize);
    
    /**
     * Sets the vertex rect.
     * It will be called internally by setTextureRect.
     * Useful if you want to create 2x images from SD images in Retina Display.
     * Do not call it manually. Use setTextureRect instead.
     */
    virtual void setVertexRect(const CCRect& rect);
    
    /// @} end of texture methods
    

    
    /// @{
    /// @name Frames methods
    
    /**
     * Sets a new display frame to the CCSprite.
     */
    virtual void setDisplayFrame(CCSpriteFrame *pNewFrame);
    
    /**
     * Returns whether or not a CCSpriteFrame is being displayed
     */
    virtual bool isFrameDisplayed(CCSpriteFrame *pFrame);
    
    /**
     * Returns the current displayed frame.
     * @js NA
     */
    virtual CCSpriteFrame* displayFrame(void);
    
    /// @} End of frames methods
    

    /// @{
    /// @name Animation methods
    /**
     * Changes the display frame with animation name and index.
     * The animation name will be get from the CCAnimationCache
     */
    virtual void setDisplayFrameWithAnimationName(const char *animationName, int frameIndex);
    /// @}
    
    
    /// @{
    /// @name Sprite Properties' setter/getters
    
    /** 
     * Whether or not the Sprite needs to be updated in the Atlas.
     *
     * @return true if the sprite needs to be updated in the Atlas, false otherwise.
     */
    inline virtual bool isDirty(void) { return m_bDirty; }
    
    /** 
     * Makes the Sprite to be updated in the Atlas.
     */
    inline virtual void setDirty(bool bDirty) { m_bDirty = bDirty; }
    
    /**
     * Returns the quad (tex coords, vertex coords and color) information.
     * @js NA
     */
    inline ccV3F_C4B_T2F_Quad getQuad(void) { return m_sQuad; }

    /** 
     * Returns whether or not the texture rectangle is rotated.
     */
    inline bool isTextureRectRotated(void) { return m_bRectRotated; }
    
    /** 
     * Returns the index used on the TextureAtlas. 
     */
    inline unsigned int getAtlasIndex(void) { return m_uAtlasIndex; }
    
    /** 
     * Sets the index used on the TextureAtlas.
     * @warning Don't modify this value unless you know what you are doing
     */
    inline void setAtlasIndex(unsigned int uAtlasIndex) { m_uAtlasIndex = uAtlasIndex; }

    /** 
     * Returns the rect of the CCSprite in points 
     */
    inline const CCRect& getTextureRect(void) { return m_obRect; }

    /** 
     * Gets the offset position of the sprite. Calculated automatically by editors like Zwoptex.
     */
    inline const CCPoint& getOffsetPosition(void) { return m_obOffsetPosition; }


    /** 
     * Returns the flag which indicates whether the sprite is flipped horizontally or not.
     *
     * It only flips the texture of the sprite, and not the texture of the sprite's children.
     * Also, flipping the texture doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleX(sprite->getScaleX() * -1);
     *
     * @return true if the sprite is flipped horizaontally, false otherwise.
     * @js isFlippedX
     */
    bool isFlipX(void);
    /**
     * Sets whether the sprite should be flipped horizontally or not.
     *
     * @param bFlipX true if the sprite should be flipped horizaontally, false otherwise.
     */
    void setFlipX(bool bFlipX);
    
    /** 
     * Return the flag which indicates whether the sprite is flipped vertically or not.
     * 
     * It only flips the texture of the sprite, and not the texture of the sprite's children.
     * Also, flipping the texture doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleY(sprite->getScaleY() * -1);
     * 
     * @return true if the sprite is flipped vertically, flase otherwise.
     * @js isFlippedY
     */
    bool isFlipY(void);
    /**
     * Sets whether the sprite should be flipped vertically or not.
     *
     * @param bFlipY true if the sprite should be flipped vertically, flase otherwise.
     */
    void setFlipY(bool bFlipY);
    
    /// @} End of Sprite properties getter/setters

	void setFrame(const CCRect& rect);

	void setBoundsSize(const CCSize& size);

	void setContentSize(const CCSize &size);

protected:
    void updateColor(void);
    virtual void setTextureCoords(CCRect rect);
    virtual void updateBlendFunc(void);
    virtual void setReorderChildDirtyRecursively(void);
    virtual void setDirtyRecursively(bool bValue);

    unsigned int        m_uAtlasIndex;          /// Absolute (real) Index on the SpriteSheet
    
    bool                m_bDirty;               /// Whether the sprite needs to be updated
    bool                m_bRecursiveDirty;      /// Whether all of the sprite's children needs to be updated
    bool                m_bHasChildren;         /// Whether the sprite contains children
    bool                m_bShouldBeHidden;      /// should not be drawn because one of the ancestors is not visible
    CCAffineTransform   m_transformToBatch;
    
    //
    // Data used when the sprite is self-rendered
    //
    ccBlendFunc        m_sBlendFunc;            /// It's required for CCTextureProtocol inheritance
    CCTexture2D*       m_pobTexture;            /// CCTexture2D object that is used to render the sprite

    //
    // Shared data
    //

    // texture
    CCRect m_obRect;                            /// Retangle of CCTexture2D
    bool   m_bRectRotated;                      /// Whether the texture is rotated

    // Offset Position (used by Zwoptex)
    CCPoint m_obOffsetPosition;
    CCPoint m_obUnflippedOffsetPositionFromCenter;

    // vertex coords, texture coords and color info
    ccV3F_C4B_T2F_Quad m_sQuad;

    // opacity and RGB protocol
    bool m_bOpacityModifyRGB;

    // image is flipped
    bool m_bFlipX;                              /// Whether the sprite is flipped horizaontally or not.
    bool m_bFlipY;                              /// Whether the sprite is flipped vertically or not.
};


// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __SPITE_NODE_CCSPRITE_H__

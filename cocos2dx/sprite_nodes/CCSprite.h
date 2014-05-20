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

NS_CC_BEGIN

class CCSpriteFrame;
class CCAnimation;
class CCScale9Sprite;
/**
 * @addtogroup sprite_nodes
 * @{
 */

class CC_DLL CAImageView : public CAView
{
public:
    /// @{
    /// @name Creators
    
    /**
     * Creates an empty sprite without texture. You can call setTexture method subsequently.
     *
     * @return An empty sprite object that is marked as autoreleased.
     */
    static CAImageView* create();
    
    /**
     * Creates a sprite with an exsiting texture contained in a CCTexture2D object
     * After creation, the rect will be the size of the texture, and the offset will be (0,0).
     *
     * @param   Image    A pointer to a CCTexture2D object.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static CAImageView* createWithImage(CAImage* Image);
    
    /**
     * Creates a sprite with a texture and a rect.
     *
     * After creation, the offset will be (0,0).
     *
     * @param   Image    A pointer to an existing CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @param   rect        Only the contents inside the rect of this texture will be applied for this sprite.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static CAImageView* createWithImage(CAImage* Image, const CCRect& rect);
    
    /**
     * Creates a sprite with an sprite frame.
     *
     * @param   pSpriteFrame    A sprite frame which involves a texture and a rect
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static CAImageView* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    
    /// @}  end of creators group
    
    
    
    /// @{
    /// @name Initializers
    
    /**
     * Default constructor
     * @js ctor
     */
    CAImageView(void);
    
    /**
     * Default destructor
     * @js NA
     * @lua NA
     */
    virtual ~CAImageView(void);
    
    /**
     * Initializes an empty sprite with nothing init.
     */
    virtual bool init(void);
    
    /**
     * Initializes a sprite with a texture.
     *
     * After initialization, the rect used will be the size of the texture, and the offset will be (0,0).
     *
     * @param   Image    A pointer to an existing CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithImage(CAImage* Image);
    
    /**
     * Initializes a sprite with a texture and a rect.
     *
     * After initialization, the offset will be (0,0).
     *
     * @param   Image    A pointer to an exisiting CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @param   rect        Only the contents inside rect of this texture will be applied for this sprite.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithImage(CAImage* Image, const CCRect& rect);
    
    /**
     * Initializes a sprite with a texture and a rect in points, optionally rotated.
     *
     * After initialization, the offset will be (0,0).
     * @note    This is the designated initializer.
     *
     * @param   Image    A CCTexture2D object whose texture will be applied to this sprite.
     * @param   rect        A rectangle assigned the contents of texture.
     * @param   rotated     Whether or not the texture rectangle is rotated.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithImage(CAImage* Image, const CCRect& rect, bool rotated);
    
    /**
     * Initializes a sprite with an SpriteFrame. The texture and rect in SpriteFrame will be applied on this sprite
     *
     * @param   pSpriteFrame  A CCSpriteFrame object. It should includes a valid texture and a rect
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    
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

    
    /// @} End of Sprite properties getter/setters

	void setFrame(const CCRect& rect);
    
    void setFrameOrigin(const CCPoint& point);

    void setCenter(const CCRect& rect);
    
    CCRect getCenter();
    
    void setCenterOrigin(const CCPoint& point);
    
    CCPoint getCenterOrigin();
    
	void setBounds(const CCRect& rect);

    CCRect getBounds() const;
    
	void setContentSize(const CCSize &size);
    
    
    
    /// @} end of texture methods
    
    /// @{
    /// @name Animation methods
    /**
     * Changes the display frame with animation name and index.
     * The animation name will be get from the CCAnimationCache
     */
    virtual void setDisplayFrameWithAnimationName(const char *animationName, int frameIndex);
    /// @}
    
    virtual void setImage(CAImage* texture);
    
    friend class CCScale9Sprite;
};


// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __SPITE_NODE_CCSPRITE_H__

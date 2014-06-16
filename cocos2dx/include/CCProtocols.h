/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada

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

#ifndef __CCPROTOCOLS_H__
#define __CCPROTOCOLS_H__

#include "ccTypes.h"
#include "images/CAImage.h"
#include <string>

NS_CC_BEGIN

/**
 * RGBA protocol that affects CCNode's color and opacity
 * @js NA
 */
class CC_DLL CCRGBAProtocol
{
public:
    /** 
     * Changes the color with R,G,B,A bytes
     *
     * @param color Example: ccc4(255,100,0) means R=255, G=100, B=0
     */
    virtual void setColor(const CAColor4B& color) = 0;

    /**
     * Returns color that is currently used.
     *
     * @return The CAColor4B contains R,G,B,A bytes.
     */
    virtual const CAColor4B& getColor(void) = 0;
    
    /**
     * Returns the displayed color.
     *
     * @return The CAColor4B contains R,G,B,A bytes.
     */
    virtual const CAColor4B& getDisplayedColor(void) = 0;
    
    /**
     * Returns the displayed alpha.
     *
     * @return  The alpha of image, from 0 ~ 1.0f
     */
    virtual float getDisplayedAlpha(void) = 0;
    /**
     * Returns the alpha.
     *
     * The opacity which indicates how transparent or opaque this node is.
     * 0 indicates fully transparent and 1.0f is fully opaque.
     *
     * @return  The opacity of image, from 0 ~ 1.0f
     */
    virtual float getAlpha(void) = 0;

    /**
     * Changes the alpha.
     *
     * @param   value   Goes from 0 to 1.0f, where 1.0f means fully opaque and 0 means fully transparent.
     */
    virtual void setAlpha(float alpha) = 0;

    /** 
     *  recursive method that updates display color 
     */
    virtual void updateDisplayedColor(const CAColor4B& color) = 0;
    
    /**
     *  recursive method that updates the displayed alpha.
     */
    virtual void updateDisplayedAlpha(float alpha) = 0;
};

/**
 * Specify the blending function according glBlendFunc
 * Please refer to glBlendFunc in OpenGL ES Manual
 * http://www.khronos.org/opengles/sdk/docs/man/xhtml/glBlendFunc.xml for more details.
 * @js NA
 * @lua NA
 */
class CC_DLL CCBlendProtocol
{
public:
    /**
     * Sets the source blending function.
     *
     * @param blendFunc A structure with source and destination factor to specify pixel arithmetic, 
     *                  e.g. {GL_ONE, GL_ONE}, {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}.
     *
     */
    virtual void setBlendFunc(ccBlendFunc blendFunc) = 0;

    /**
     * Returns the blending function that is currently being used.
     * 
     * @return A ccBlendFunc structure with source and destination factor which specified pixel arithmetic.
     */
    virtual ccBlendFunc getBlendFunc(void) = 0;
};

/** 
 * CAView objects that uses a CAImage to render the images.
 * The Image can have a blending function.
 * If the Image has alpha premultiplied the default blending function is:
 *   src=GL_ONE dst= GL_ONE_MINUS_SRC_ALPHA
 * else
 *   src=GL_SRC_ALPHA dst= GL_ONE_MINUS_SRC_ALPHA
 * But you can change the blending function at any time.
 * @js NA
 */
class CC_DLL CCTextureProtocol : public CCBlendProtocol
{
public:
    /**
     * Returns the currently used texture
     *
     * @return  The Image that is currenlty being used.
     * @lua NA
     */
    virtual CAImage* getImage(void) = 0;

    /**
     * Sets a new texuture. It will be retained.
     *
     * @param   Image A valid CAImage object, which will be applied to this sprite object.
     * @lua NA
     */
    virtual void setImage(CAImage* texture) = 0;
};

/**
 * Common interface for Labels
 * @js NA
 * @lua NA
 */
class CC_DLL CCLabelProtocol
{
public:
    /**
     * Sets a new label using an string
     *
     * @param A null terminated string 
     */
    virtual void setString(std::string label) = 0;

    /** 
     * Returns the string that is currently being used in this label 
     *
     * @return The string that is currently being used in this label
     */
    virtual const std::string& getString(void) const = 0;
};

/** 
 * OpenGL projection protocol 
 * @js NA
 * @lua NA
 */
class CC_DLL CCDirectorDelegate
{
public:
    /** 
     * Will be called by CCDirector when the projection is updated, and "custom" projection is used
     */
    virtual void updateProjection(void) = 0;
};

NS_CC_END

#endif // __CCPROTOCOLS_H__

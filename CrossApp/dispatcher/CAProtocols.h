

#ifndef __CAProtocols_H__
#define __CAProtocols_H__

#include "ccTypes.h"
#include "images/CAImage.h"
#include <string>

NS_CC_BEGIN

/**
 * RGBA protocol that affects CCNode's color and opacity
 * @js NA
 */
class CC_DLL CARGBAProtocol
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


class CC_DLL CAImageProtocol
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
class CC_DLL CAApplicationDelegate
{
public:
    /** 
     * Will be called by CAApplication when the projection is updated, and "custom" projection is used
     */
    virtual void updateProjection(void) = 0;
};

NS_CC_END

#endif // __CAProtocols_H__

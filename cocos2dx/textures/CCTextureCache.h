/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.
Copyright (c) Microsoft Open Technologies, Inc.

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

#ifndef __CCTEXTURE_CACHE_H__
#define __CCTEXTURE_CACHE_H__

#include "cocoa/CCObject.h"
#include "cocoa/CCDictionary.h"
#include "textures/CCTexture2D.h"
#include <string>


#if CC_ENABLE_CACHE_TEXTURE_DATA
    #include "platform/CCImage.h"
    #include <list>
#endif

NS_CC_BEGIN

class CCLock;
class CCImage;

/**
 * @addtogroup textures
 * @{
 */

/** @brief Singleton that handles the loading of textures
* Once the texture is loaded, the next time it will return
* a reference of the previously loaded texture reducing GPU & CPU memory
*/
class CC_DLL CCTextureCache : public CCObject
{
protected:
    CCDictionary* m_pTextures;
    //pthread_mutex_t                *m_pDictLock;


private:
    /// todo: void addImageWithAsyncObject(CCAsyncObject* async);
    void addImageAsyncCallBack(float dt);
public:
    /**
     *  @js ctor
     *  @lua NA
     */
    CCTextureCache();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCTextureCache();
    /**
     *  @js NA
     *  @lua NA
     */
    const char* description(void);
    /**
     *  @js NA
     */
    CCDictionary* snapshotTextures();

    /** Returns the shared instance of the cache 
     *  @js getInstance
     */
    static CCTextureCache * sharedTextureCache();

    /** purges the cache. It releases the retained instance.
    @since v0.99.0
    */
    static void purgeSharedTextureCache();

    /** Returns a Texture2D object given an file image
    * If the file image was not previously loaded, it will create a new CCTexture2D
    *  object and it will return it. It will use the filename as a key.
    * Otherwise it will return a reference of a previously loaded image.
    * Supported image extensions: .png, .bmp, .tiff, .jpeg, .pvr, .gif
    */
    CAImage* addImage(const char* fileimage);

    /* Returns a Texture2D object given a file image
    * If the file image was not previously loaded, it will create a new CCTexture2D object and it will return it.
    * Otherwise it will load a texture in a new thread, and when the image is loaded, the callback will be called with the Texture2D as a parameter.
    * The callback will be called from the main thread, so it is safe to create any cocos2d object from the callback.
    * Supported image extensions: .png, .jpg
    * @since v0.8
    * @lua NA
    */
    
    void addImageAsync(const char *path, CCObject *target, SEL_CallFuncO selector);

    void addStringImageAsync(const char *text, const char *fontName, float fontSize, cocos2d::CCSize& dimensions, CCTextAlignment textAlignment, CCVerticalTextAlignment verticalAlignment, CCObject *target, SEL_CallFuncO selector);
    
    /* Returns a Texture2D object given an CGImageRef image
    * If the image was not previously loaded, it will create a new CCTexture2D object and it will return it.
    * Otherwise it will return a reference of a previously loaded image
    * The "key" parameter will be used as the "key" for the cache.
    * If "key" is nil, then a new texture will be created each time.
    * @since v0.8
    */
    // todo: CGImageRef CAImage* addCGImage(CGImageRef image, string &  key);
    /** Returns a Texture2D object given an UIImage image
    * If the image was not previously loaded, it will create a new CCTexture2D object and it will return it.
    * Otherwise it will return a reference of a previously loaded image
    * The "key" parameter will be used as the "key" for the cache.
    * If "key" is nil, then a new texture will be created each time.
    */
    CAImage* addUIImage(CCImage *image, const char *key);

    /** Returns an already created texture. Returns nil if the texture doesn't exist.
    @since v0.99.5
    */
    CAImage* textureForKey(const char* key);
    
    /** Reload texture from the image file
     * If the file image hasn't loaded before, load it.
     * Otherwise the texture will be reloaded from the file image.
     * The "filenName" parameter is the related/absolute path of the file image.
     * Return true if the reloading is succeed, otherwise return false.
     */
    bool reloadTexture(const char* fileName);

    /** Purges the dictionary of loaded textures.
    * Call this method if you receive the "Memory Warning"
    * In the short term: it will free some resources preventing your app from being killed
    * In the medium term: it will allocate more resources
    * In the long term: it will be the same
    */
    void removeAllTextures();

    /** Removes unused textures
    * Textures that have a retain count of 1 will be deleted
    * It is convenient to call this method after when starting a new Scene
    * @since v0.8
    */
    void removeUnusedTextures();

    /** Deletes a texture from the cache given a texture
    */
    void removeTexture(CAImage* texture);

    /** Deletes a texture from the cache given a its key name
    @since v0.99.4
    */
    void removeTextureForKey(const char *textureKeyName);

    /** Output to CCLOG the current contents of this CCTextureCache
    * This will attempt to calculate the size of each texture, and the total texture memory in use
    *
    * @since v1.0
    */
    void dumpCachedTextureInfo();
    
    /** Returns a Texture2D object given an PVR filename
    * If the file image was not previously loaded, it will create a new CCTexture2D
    *  object and it will return it. Otherwise it will return a reference of a previously loaded image
    */
    CAImage* addPVRImage(const char* filename);
    
    /** Returns a Texture2D object given an ETC filename
     * If the file image was not previously loaded, it will create a new CCTexture2D
     *  object and it will return it. Otherwise it will return a reference of a previously loaded image
     *  @lua NA
     */
    CAImage* addETCImage(const char* filename);

    /** Reload all textures
    It's only useful when the value of CC_ENABLE_CACHE_TEXTURE_DATA is 1
    */
    static void reloadAllTextures();
};

#if CC_ENABLE_CACHE_TEXTURE_DATA

class VolatileTexture
{
typedef enum {
    kInvalid = 0,
    kImageFile,
    kImageData,
    kString,
    kImage,
}ccCachedImageType;

public:
    VolatileTexture(CAImage* t);
    ~VolatileTexture();

    static void addImageTexture(CAImage* tt, const char* imageFileName, CCImage::EImageFormat format);
    static void addStringTexture(CAImage* tt, const char* text, const CCSize& dimensions, CCTextAlignment alignment, 
                                 CCVerticalTextAlignment vAlignment, const char *fontName, float fontSize);
    static void addDataTexture(CAImage* tt, void* data, CCTexture2DPixelFormat pixelFormat, const CCSize& contentSize);
    static void addCCImage(CAImage* tt, CCImage *image);

    static void setTexParameters(CAImage* t, ccTexParams *texParams);
    static void removeTexture(CAImage* t);
    static void reloadAllTextures();

public:
    static std::list<VolatileTexture*> textures;
    static bool isReloading;
    
private:
    // find VolatileTexture by CAImage*
    // if not found, create a new one
    static VolatileTexture* findVolotileTexture(CAImage* tt);

protected:
    CAImage* texture;
    
    CCImage *uiImage;

    ccCachedImageType m_eCashedImageType;

    void *m_pTextureData;
    CCSize m_TextureSize;
    CCTexture2DPixelFormat m_PixelFormat;

    std::string m_strFileName;
    CCImage::EImageFormat m_FmtImage;

    ccTexParams     m_texParams;
    CCSize          m_size;
    CCTextAlignment m_alignment;
    CCVerticalTextAlignment m_vAlignment;
    std::string     m_strFontName;
    std::string     m_strText;
    float           m_fFontSize;
};

#endif

// end of textures group
/// @}

NS_CC_END

#endif //__CCTEXTURE_CACHE_H__


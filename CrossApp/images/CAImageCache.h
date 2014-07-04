

#ifndef __CCTEXTURE_CACHE_H__
#define __CCTEXTURE_CACHE_H__

#include "basics/CAObject.h"
#include "cocoa/CCDictionary.h"
#include "CAImage.h"
#include <string>


#if CC_ENABLE_CACHE_TEXTURE_DATA
    #include "platform/CCImage.h"
    #include <list>
#endif

NS_CC_BEGIN

class CCLock;
class CCImage;

class CC_DLL CAImageCache : public CAObject
{

public:

    CAImageCache();

    virtual ~CAImageCache();

    const char* description(void);

    CCDictionary* snapshotTextures();

    static CAImageCache * sharedImageCache();

    static void purgeSharedImageCache();

    CAImage* addImage(const char* fileimage);

    void addImageAsync(const char *path, CAObject *target, SEL_CallFuncO selector);

    void addStringImageAsync(const char *text,
                             const char *fontName,
                             float fontSize,
                             CrossApp::CCSize& dimensions,
                             CATextAlignment textAlignment,
                             CAVerticalTextAlignment verticalAlignment,
                             CAObject *target,
                             SEL_CallFuncO selector);

    CAImage* addUIImage(CCImage *image, const char *key);

    CAImage* imageForKey(const char* key);

    bool reloadImage(const char* fileName);

    void removeAllImages();

    void removeUnusedImages();

    void removeImage(CAImage* texture);

    void removeImageForKey(const char *textureKeyName);

    void dumpCachedImageInfo();
 
    CAImage* addPVRImage(const char* filename);

    CAImage* addETCImage(const char* filename);

    static void reloadAllImages();
    
private:
    
    void addImageAsyncCallBack(float dt);
    
protected:
    
    CCDictionary* m_pTextures;
    //pthread_mutex_t                *m_pDictLock;
};



class CC_DLL CAImageAtlas : public CAObject
{

public:

    CAImageAtlas();

    virtual ~CAImageAtlas();

    const char* description();

    static CAImageAtlas* createWithImage(CAImage *image, unsigned int capacity);

    bool initWithImage(CAImage *image, unsigned int capacity);

    void updateQuad(ccV3F_C4B_T2F_Quad* quad, unsigned int index);

    void insertQuad(ccV3F_C4B_T2F_Quad* quad, unsigned int index);

    void insertQuads(ccV3F_C4B_T2F_Quad* quads, unsigned int index, unsigned int amount);
 
    void insertQuadFromIndex(unsigned int fromIndex, unsigned int newIndex);

    void removeQuadAtIndex(unsigned int index);

    void removeQuadsAtIndex(unsigned int index, unsigned int amount);

    void removeAllQuads();

    bool resizeCapacity(unsigned int n);

    void increaseTotalQuadsWith(unsigned int amount);

    void moveQuadsFromIndex(unsigned int oldIndex, unsigned int amount, unsigned int newIndex);

    void moveQuadsFromIndex(unsigned int index, unsigned int newIndex);

    void fillWithEmptyQuadsFromIndex(unsigned int index, unsigned int amount);

    void drawNumberOfQuads(unsigned int n);

    void drawNumberOfQuads(unsigned int n, unsigned int start);

    void drawQuads();

    void listenBackToForeground(CAObject *obj);

    inline bool isDirty(void) { return m_bDirty; }

    inline void setDirty(bool bDirty) { m_bDirty = bDirty; }
   
protected:
    
    CC_PROPERTY_READONLY(unsigned int, m_uTotalQuads, TotalQuads)
    
    CC_PROPERTY_READONLY(unsigned int, m_uCapacity, Capacity)
    
    CC_PROPERTY(CAImage *, m_pImage, Image)
    
    CC_PROPERTY(ccV3F_C4B_T2F_Quad *, m_pQuads, Quads)
    
private:
    
    void setupIndices();
    
    void mapBuffers();
#if CC_TEXTURE_ATLAS_USE_VAO
    void setupVBOandVAO();
#else
    void setupVBO();
#endif
    
protected:
    
    GLushort*           m_pIndices;
#if CC_TEXTURE_ATLAS_USE_VAO
    GLuint              m_uVAOname;
#endif
    GLuint              m_pBuffersVBO[2];
    bool                m_bDirty;
};




#if CC_ENABLE_CACHE_TEXTURE_DATA

class VolatileTexture
{
    typedef enum
    {
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
    
    static void addStringTexture(CAImage* tt, const char* text, const CCSize& dimensions, CATextAlignment alignment,
                                 CAVerticalTextAlignment vAlignment, const char *fontName, float fontSize);
    
    static void addDataTexture(CAImage* tt, void* data, CAImagePixelFormat pixelFormat, const CCSize& contentSize);
    
    static void addCCImage(CAImage* tt, CCImage *image);
    
    static void setTexParameters(CAImage* t, ccTexParams *texParams);
    
    static void removeImage(CAImage* t);
    
    static void reloadAllImages();
    
public:
    
    static std::list<VolatileTexture*> textures;
    
    static bool isReloading;
    
private:

    static VolatileTexture* findVolotileTexture(CAImage* tt);
    
protected:
    
    CAImage* texture;
    
    CCImage *uiImage;
    
    ccCachedImageType m_eCashedImageType;
    
    void *m_pTextureData;
    
    CCSize m_TextureSize;
    
    CAImagePixelFormat m_PixelFormat;
    
    std::string m_strFileName;
    
    CCImage::EImageFormat m_FmtImage;
    
    ccTexParams     m_texParams;
    
    CCSize          m_size;
    
    CATextAlignment m_alignment;
    
    CAVerticalTextAlignment m_vAlignment;
    
    std::string     m_strFontName;
    
    std::string     m_strText;
    
    float           m_fFontSize;
};

#endif



NS_CC_END

#endif //__CCTEXTURE_CACHE_H__


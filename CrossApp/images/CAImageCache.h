

#ifndef __CCTEXTURE_CACHE_H__
#define __CCTEXTURE_CACHE_H__

#include "basics/CAObject.h"
#include "cocoa/CCDictionary.h"
#include "CAImage.h"
#include <string>


NS_CC_BEGIN


class CC_DLL CAImageCache : public CAObject
{

public:

    CAImageCache();

    virtual ~CAImageCache();

    const char* description(void);

    CCDictionary* snapshotTextures();

    static CAImageCache * sharedImageCache();

    static void purgeSharedImageCache();

    CAImage* addImage(const std::string& fileimage);

    void addImageAsync(const std::string& path, CAObject *target, SEL_CallFuncO selector);

    void addImageFullPathAsync(const std::string& path, CAObject *target, SEL_CallFuncO selector);

    CAImage* addETCImage(const std::string& filename);

    CAImage* imageForKey(const std::string& key);
    
    void setImageForKey(CAImage* image, const std::string& key);
    
    void removeImage(CAImage* image);

    void removeImageForKey(const std::string& textureKeyName);

    void dumpCachedImageInfo();
    
    bool reloadImage(const std::string& fileName);
    
    void removeAllImages();
    
    void removeUnusedImages();
    
    static void reloadAllImages();
    
private:
    
    void addImageAsyncCallBack(float dt);
    
protected:
    
    CCDictionary* m_pImages;
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


NS_CC_END

#endif //__CCTEXTURE_CACHE_H__


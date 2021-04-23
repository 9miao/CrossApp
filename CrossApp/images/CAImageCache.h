

#ifndef __CCTEXTURE_CACHE_H__
#define __CCTEXTURE_CACHE_H__

#include "basics/CAObject.h"
#include "basics/CASTLContainer.h"
#include "CAImage.h"
#include <string>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>

NS_CC_BEGIN

class CC_DLL CAImageCache : public CAObject
{

public:

    CAImageCache();

    virtual ~CAImageCache();

    static CAImageCache* getInstance();
    
    const char* description(void);

    CAImage* addImage(const std::string& fileimage);

    void addImageAsync(const std::string& path, const std::function<void(CAImage*)>& function);

    void addImageFullPathAsync(const std::string& path, const std::function<void(CAImage*)>& function);

    CAImage* imageForKey(const std::string& key);
    
    const std::string& getImageFilePath(CAImage* image);
    
    void setImageForKey(CAImage* image, const std::string& key);
    
    void removeImage(CAImage* image);

    void removeImageForKey(const std::string& textureKeyName);

    void dumpCachedImageInfo();
    
    bool reloadImage(const std::string& fileName);
    
    void removeAllImages();
    
    void removeUnusedImages();
    
    void waitForQuit();
    
    static void reloadAllImages();
    
private:
    
    void addImageAsyncCallBack(float dt);
    
    void loadImage();
    
protected:
    
    CAMap<std::string, CAImage*> m_mImages;
    
    struct AsyncStruct;
    
    ssize_t m_iAsyncRefCount = 0;
    
    std::thread* m_pLoadingThread;
    
    std::deque<AsyncStruct*> m_pRequestQueue;
    std::deque<AsyncStruct*> m_pResponseQueue;
    
    std::mutex m_obRequestMutex;
    std::mutex m_obResponseMutex;
    
    std::condition_variable m_obSleepCondition;
    
    bool m_bNeedQuit;
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
    
    CC_SYNTHESIZE_READONLY(unsigned long, m_uSerialNumberOfDraws, SerialNumberOfDraws);
    
    CC_PROPERTY_READONLY(unsigned int, m_uTotalQuads, TotalQuads)
    
    CC_PROPERTY_READONLY(unsigned int, m_uCapacity, Capacity)
    
    CC_PROPERTY(CAImage*, m_pImage, Image)
    
    CC_PROPERTY(ccV3F_C4B_T2F_Quad *, m_pQuads, Quads)
    
private:
    
    void renderCommand();
    
    void setupIndices();
    
    void mapBuffers();
    
    void setupVBOandVAO();
    
    void setupVBO();
    
protected:
    
    GLushort*           m_pIndices;
    GLuint              m_uVAOname;
    GLuint              m_pBuffersVBO[2];
    bool                m_bDirty;
};


NS_CC_END

#endif //__CCTEXTURE_CACHE_H__


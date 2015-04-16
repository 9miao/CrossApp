

#include "CAImageCache.h"
#include "CAImage.h"
#include "ccMacros.h"
#include "basics/CAApplication.h"
#include "platform/platform.h"
#include "platform/CCFileUtils.h"
#include "support/ccUtils.h"
#include "basics/CAScheduler.h"
#include "cocoa/CCString.h"
#include "shaders/CAGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "support/CANotificationCenter.h"
#include "ccTypes.h"
#include "CCGL.h"
#include <errno.h>
#include <stack>
#include <string>
#include <cctype>
#include <queue>
#include <list>
#include <stdlib.h>
#include <pthread.h>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#include <pthread.h>
#else
#include "CCPThreadWinRT.h"
#include <ppl.h>
#include <ppltasks.h>
using namespace concurrency;
#endif

using namespace std;

NS_CC_BEGIN

#pragma CAImageCache

typedef enum _AsyncType
{
    AsyncImageType=0,
    AsyncStringType
}AsyncType;

typedef struct _AsyncStruct
{
    std::string            filename;
    CAObject    *target;
    SEL_CallFuncO        selector;
} AsyncStruct;

typedef struct _ImageInfo
{
    AsyncStruct *asyncStruct;
    CAImage        *image;
} ImageInfo;

static pthread_t s_loadingThread;

static pthread_mutex_t		s_SleepMutex;
static pthread_cond_t		s_SleepCondition;

static pthread_mutex_t      s_asyncStructQueueMutex;
static pthread_mutex_t      s_ImageInfoMutex;

#ifdef EMSCRIPTEN
// Hack to get ASM.JS validation (no undefined symbols allowed).
#define pthread_cond_signal(_)
#endif // EMSCRIPTEN

static unsigned long s_nAsyncRefCount = 0;

static bool need_quit = false;

static std::queue<AsyncStruct*>* s_pAsyncStructQueue = NULL;

static std::queue<ImageInfo*>*   s_pImageQueue = NULL;

static CAImage::Format computeImageFormatType(string& filename)
{
    CAImage::Format ret = CAImage::UNKOWN;

    if ((std::string::npos != filename.find(".jpg")) || (std::string::npos != filename.find(".jpeg")))
    {
        ret = CAImage::JPG;
    }
    else if ((std::string::npos != filename.find(".png")) || (std::string::npos != filename.find(".PNG")))
    {
        ret = CAImage::PNG;
    }
    else if ((std::string::npos != filename.find(".tiff")) || (std::string::npos != filename.find(".TIFF")))
    {
        ret = CAImage::TIFF;
    }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
    else if ((std::string::npos != filename.find(".webp")) || (std::string::npos != filename.find(".WEBP")))
    {
        ret = CAImage::WEBP;
    }
#endif
   
    return ret;
}

static void loadImageData(AsyncStruct *pAsyncStruct)
{
    const char *filename = pAsyncStruct->filename.c_str();

    // compute image type
    CAImage::Format imageType = computeImageFormatType(pAsyncStruct->filename);
    if (imageType == CAImage::UNKOWN)
    {
        //CCLOG("unsupported format %s",filename);
        //delete pAsyncStruct;
    }
    
    CAImage* image = new CAImage();
    pthread_mutex_lock(&s_ImageInfoMutex);
    if (image && !image->initWithImageFileThreadSafe(filename))
    {
        CC_SAFE_RELEASE(image);
        return;
    }
    pthread_mutex_unlock(&s_ImageInfoMutex);
    // generate image info
    ImageInfo *pImageInfo = new ImageInfo();
    pImageInfo->asyncStruct = pAsyncStruct;
    pImageInfo->image = image;

    // put the image info into the queue
    pthread_mutex_lock(&s_ImageInfoMutex);
    s_pImageQueue->push(pImageInfo);
    pthread_mutex_unlock(&s_ImageInfoMutex);   
}

static void* loadImage(void* data)
{
    AsyncStruct *pAsyncStruct = NULL;

    while (true)
    {
        std::queue<AsyncStruct*> *pQueue = s_pAsyncStructQueue;
        pthread_mutex_lock(&s_asyncStructQueueMutex);// get async struct from queue
        if (pQueue->empty())
        {
            pthread_mutex_unlock(&s_asyncStructQueueMutex);
            if (need_quit) {
                break;
            }
            else {
                pthread_cond_wait(&s_SleepCondition, &s_SleepMutex);
                continue;
            }
        }
        else
        {
            pAsyncStruct = pQueue->front();
            pQueue->pop();
            pthread_mutex_unlock(&s_asyncStructQueueMutex);
            loadImageData(pAsyncStruct);
        }        
    }
    
    if( s_pAsyncStructQueue != NULL )
    {
        delete s_pAsyncStructQueue;
        s_pAsyncStructQueue = NULL;
        delete s_pImageQueue;
        s_pImageQueue = NULL;

        pthread_mutex_destroy(&s_asyncStructQueueMutex);
        pthread_mutex_destroy(&s_ImageInfoMutex);
        pthread_mutex_destroy(&s_SleepMutex);
        pthread_cond_destroy(&s_SleepCondition);
    }
    
    return 0;
}

// implementation CAImageCache

// ImageCache - Alloc, Init & Dealloc
static CAImageCache *g_sharedImageCache = NULL;

CAImageCache * CAImageCache::sharedImageCache()
{
    if (!g_sharedImageCache)
    {
        g_sharedImageCache = new CAImageCache();
    }
    return g_sharedImageCache;
}

CAImageCache::CAImageCache()
{
    CCAssert(g_sharedImageCache == NULL, "Attempted to allocate a second instance of a singleton.");
    
    m_pImages = new CCDictionary();
}

CAImageCache::~CAImageCache()
{
    CCLOGINFO("CrossApp: deallocing CAImageCache.");
    need_quit = true;
    pthread_cond_signal(&s_SleepCondition);
    CC_SAFE_RELEASE(m_pImages);
}

void CAImageCache::purgeSharedImageCache()
{
    CC_SAFE_RELEASE_NULL(g_sharedImageCache);
}

const char* CAImageCache::description()
{
    return CCString::createWithFormat("<CAImageCache | Number of textures = %u>", m_pImages->count())->getCString();
}

CCDictionary* CAImageCache::snapshotTextures()
{ 
    CCDictionary* pRet = new CCDictionary();
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pImages, pElement)
    {
        pRet->setObject(pElement->getObject(), pElement->getStrKey());
    }
    pRet->autorelease();
    return pRet;
}

void CAImageCache::addImageAsync(const std::string& path, CAObject *target, SEL_CallFuncO selector)
{
    std::string pathKey = path;

    this->addImageFullPathAsync(pathKey.c_str(), target, selector);
}

void CAImageCache::addImageFullPathAsync(const std::string& path, CAObject *target, SEL_CallFuncO selector)
{
    CAImage* image = NULL;
    
    // optimization
    
    image = (CAImage*)m_pImages->objectForKey(path);
    
    std::string fullpath = path;
    
    if (image != NULL)
    {
        if (target && selector)
        {
            (target->*selector)(image);
        }
        
        return;
    }
    
    // lazy init
    if (s_pAsyncStructQueue == NULL)
    {
        s_pAsyncStructQueue = new queue<AsyncStruct*>();
        s_pImageQueue = new queue<ImageInfo*>();
        
        pthread_mutex_init(&s_asyncStructQueueMutex, NULL);
        pthread_mutex_init(&s_ImageInfoMutex, NULL);
        pthread_mutex_init(&s_SleepMutex, NULL);
        pthread_cond_init(&s_SleepCondition, NULL);
        pthread_create(&s_loadingThread, NULL, loadImage, NULL);
        need_quit = false;
    }
    
    if (0 == s_nAsyncRefCount)
    {
        CAScheduler::schedule(schedule_selector(CAImageCache::addImageAsyncCallBack), this, 0);
    }
    
    ++s_nAsyncRefCount;

    CC_SAFE_RETAIN(target);
    
    // generate async struct
    AsyncStruct *data = new AsyncStruct();
    data->filename = fullpath.c_str();
    data->target = target;
    data->selector = selector;
    
    // add async struct into queue
    pthread_mutex_lock(&s_asyncStructQueueMutex);
    s_pAsyncStructQueue->push(data);
    pthread_mutex_unlock(&s_asyncStructQueueMutex);
    pthread_cond_signal(&s_SleepCondition);
}


void CAImageCache::addImageAsyncCallBack(float dt)
{
    // the image is generated in loading thread
    std::queue<ImageInfo*> *imagesQueue = s_pImageQueue;

    pthread_mutex_lock(&s_ImageInfoMutex);
    if (imagesQueue->empty())
    {
        pthread_mutex_unlock(&s_ImageInfoMutex);
    }
    else
    {
        ImageInfo *pImageInfo = imagesQueue->front();
        imagesQueue->pop();
        pthread_mutex_unlock(&s_ImageInfoMutex);

        AsyncStruct *pAsyncStruct = pImageInfo->asyncStruct;
        CAImage *image = pImageInfo->image;
        image->premultipliedAImageData();
        
        CAObject *target = pAsyncStruct->target;
        SEL_CallFuncO selector = pAsyncStruct->selector;
        const char* filename = pAsyncStruct->filename.c_str();

        // cache the image
        m_pImages->setObject(image, filename);
        image->release();

        if (target && selector)
        {
            (target->*selector)(image);
            target->release();
        }
        
        delete pAsyncStruct;
        delete pImageInfo;

        --s_nAsyncRefCount;
        if (0 == s_nAsyncRefCount)
        {
            CAScheduler::unschedule(schedule_selector(CAImageCache::addImageAsyncCallBack), this);
        }
    }
}

CAImage* CAImageCache::addImage(const std::string& path)
{
    CAImage* image = NULL;
    
    //pthread_mutex_lock(m_pDictLock);
    
    image = (CAImage*)m_pImages->objectForKey(path);

    if (!image)
    {
        std::string lowerCase(path);
        for (unsigned int i = 0; i < lowerCase.length(); ++i)
        {
            lowerCase[i] = tolower(lowerCase[i]);
        }
        
        do
        {
            if (std::string::npos != lowerCase.find(".pkm"))
            {
                // ETC1 file format, only supportted on Android
                image = this->addETCImage(path.c_str());
            }
            else
            {
                image = new CAImage();
                if(image != NULL && image->initWithImageFile(path.c_str()))
                {
                    m_pImages->setObject(image, path);
                    image->release();
                }
                else
                {
                    CC_SAFE_DELETE(image);
                }
            }
        } while (0);
    }
    
    //pthread_mutex_unlock(m_pDictLock);
    return image;
}

CAImage* CAImageCache::addETCImage(const std::string& path)
{
    CAImage* image = NULL;
    std::string key(path);
    
    if( (image = (CAImage*)m_pImages->objectForKey(key)))
    {
        return image;
    }
    

    image = new CAImage();
    if(image != NULL && image->initWithETCFile(path.c_str()))
    {
        m_pImages->setObject(image, key.c_str());
        image->release();
    }
    else
    {
        CC_SAFE_DELETE(image);
    }
    
    return image;
}

bool CAImageCache::reloadImage(const std::string& fileName)
{
    if (fileName.size() == 0)
    {
        return false;
    }
    
    CAImage*  image = (CAImage*) m_pImages->objectForKey(fileName);
    
    bool ret = false;
    if (! image)
    {
        image = this->addImage(fileName.c_str());
        ret = (image != NULL);
    }

    return ret;
}

// ImageCache - Remove

void CAImageCache::removeAllImages()
{
    m_pImages->removeAllObjects();
}

void CAImageCache::removeUnusedImages()
{
    /*
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pImages, pElement)
    {
        CCLOG("CrossApp: CAImageCache: texture: %s", pElement->getStrKey());
        CAImage* value = (CAImage*)pElement->getObject();
        if (value->retainCount() == 1)
        {
            CCLOG("CrossApp: CAImageCache: removing unused texture: %s", pElement->getStrKey());
            m_pImages->removeObjectForElememt(pElement);
        }
    }
     */
    
    /** Inter engineer zhuoshi sun finds that this way will get better performance
     */    
    if (m_pImages->count())
    {   
        // find elements to be removed
        CCDictElement* pElement = NULL;
        list<CCDictElement*> elementToRemove;
        CCDICT_FOREACH(m_pImages, pElement)
        {
            CCLOG("CrossApp: CAImageCache: texture: %s", pElement->getStrKey());
            CAImage* value = (CAImage*)pElement->getObject();
            if (value->retainCount() == 1)
            {
                elementToRemove.push_back(pElement);
            }
        }
        
        // remove elements
        for (list<CCDictElement*>::iterator iter = elementToRemove.begin(); iter != elementToRemove.end(); ++iter)
        {
            CCLOG("CrossApp: CAImageCache: removing unused texture: %s", (*iter)->getStrKey());
            m_pImages->removeObjectForElememt(*iter);
        }
    }
}

void CAImageCache::setImageForKey(CAImage* image, const std::string& key)
{
    if( ! image )
    {
        return;
    }
    m_pImages->setObject(image, key.c_str());
}

void CAImageCache::removeImage(CAImage* image)
{
    if( ! image )
    {
        return;
    }

    CCArray* keys = m_pImages->allKeysForObject(image);
    m_pImages->removeObjectsForKeys(keys);
}

void CAImageCache::removeImageForKey(const std::string& imageKeyName)
{
    m_pImages->removeObjectForKey(imageKeyName.c_str());
}

CAImage* CAImageCache::imageForKey(const std::string& key)
{
    return (CAImage*)m_pImages->objectForKey(key);
}

void CAImageCache::reloadAllImages()
{

}

void CAImageCache::dumpCachedImageInfo()
{
    unsigned int count = 0;
    unsigned int totalBytes = 0;

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pImages, pElement)
    {
        CAImage* tex = (CAImage*)pElement->getObject();
        unsigned int bpp = tex->bitsPerPixelForFormat();
        // Each texture takes up width * height * bytesPerPixel bytes.
        unsigned int bytes = tex->getPixelsWide() * tex->getPixelsHigh() * bpp / 8;
        totalBytes += bytes;
        count++;
        CCLOG("CrossApp: \"%s\" rc=%lu id=%lu %lu x %lu @ %ld bpp => %lu KB",
               pElement->getStrKey(),
               (long)tex->retainCount(),
               (long)tex->getName(),
               (long)tex->getPixelsWide(),
               (long)tex->getPixelsHigh(),
               (long)bpp,
               (long)bytes / 1024);
    }

    CCLOG("CrossApp: CAImageCache dumpDebugInfo: %ld textures, for %lu KB (%.2f MB)", (long)count, (long)totalBytes / 1024, totalBytes / (1024.0f*1024.0f));
}



#pragma CAImageAtlas

CAImageAtlas::CAImageAtlas()
:m_pIndices(NULL)
,m_bDirty(false)
,m_pImage(NULL)
,m_pQuads(NULL)
{}

CAImageAtlas::~CAImageAtlas()
{
    CCLOGINFO("CrossApp: CAImageAtlas deallocing %p.", this);
    
    CC_SAFE_FREE(m_pQuads);
    CC_SAFE_FREE(m_pIndices);
    
    glDeleteBuffers(2, m_pBuffersVBO);
    
#if CC_TEXTURE_ATLAS_USE_VAO
    glDeleteVertexArrays(1, &m_uVAOname);
    ccGLBindVAO(0);
#endif
    CC_SAFE_RELEASE(m_pImage);
}

unsigned int CAImageAtlas::getTotalQuads()
{
    return m_uTotalQuads;
}

unsigned int CAImageAtlas::getCapacity()
{
    return m_uCapacity;
}

CAImage* CAImageAtlas::getImage()
{
    return m_pImage;
}

void CAImageAtlas::setImage(CAImage * var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pImage);
    m_pImage = var;
}

ccV3F_C4B_T2F_Quad* CAImageAtlas::getQuads()
{
    //if someone accesses the quads directly, presume that changes will be made
    m_bDirty = true;
    return m_pQuads;
}

void CAImageAtlas::setQuads(ccV3F_C4B_T2F_Quad *var)
{
    m_pQuads = var;
}

CAImageAtlas * CAImageAtlas::createWithImage(CAImage *image, unsigned int capacity)
{
    CAImageAtlas * pImageAtlas = new CAImageAtlas();
    if (pImageAtlas && pImageAtlas->initWithImage(image, capacity))
    {
        pImageAtlas->autorelease();
        return pImageAtlas;
    }
    CC_SAFE_DELETE(pImageAtlas);
    return NULL;
}

bool CAImageAtlas::initWithImage(CAImage *image, unsigned int capacity)
{
    //    CCAssert(image != NULL, "image should not be null");
    m_uCapacity = capacity;
    m_uTotalQuads = 0;
    
    // retained in property
    this->m_pImage = image;
    CC_SAFE_RETAIN(m_pImage);
    
    // Re-initialization is not allowed
    CCAssert(m_pQuads == NULL && m_pIndices == NULL, "");
    
    m_pQuads = (ccV3F_C4B_T2F_Quad*)malloc( m_uCapacity * sizeof(ccV3F_C4B_T2F_Quad) );
    m_pIndices = (GLushort *)malloc( m_uCapacity * 6 * sizeof(GLushort) );
    
    if( ! ( m_pQuads && m_pIndices) && m_uCapacity > 0)
    {
        //CCLOG("CrossApp: CAImageAtlas: not enough memory");
        CC_SAFE_FREE(m_pQuads);
        CC_SAFE_FREE(m_pIndices);
        
        // release image, should set it to null, because the destruction will
        // release it too. see CrossApp issue #484
        CC_SAFE_RELEASE_NULL(m_pImage);
        return false;
    }
    
    memset( m_pQuads, 0, m_uCapacity * sizeof(ccV3F_C4B_T2F_Quad) );
    memset( m_pIndices, 0, m_uCapacity * 6 * sizeof(GLushort) );
    
    this->setupIndices();
    
#if CC_TEXTURE_ATLAS_USE_VAO
    setupVBOandVAO();
#else
    setupVBO();
#endif
    
    m_bDirty = true;
    
    return true;
}

void CAImageAtlas::listenBackToForeground(CAObject *obj)
{
#if CC_TEXTURE_ATLAS_USE_VAO
    setupVBOandVAO();
#else
    setupVBO();
#endif
    
    // set m_bDirty to true to force it rebinding buffer
    m_bDirty = true;
}

const char* CAImageAtlas::description()
{
    return CCString::createWithFormat("<CAImageAtlas | totalQuads = %u>", m_uTotalQuads)->getCString();
}


void CAImageAtlas::setupIndices()
{
    if (m_uCapacity == 0)
        return;
    
    for( unsigned int i=0; i < m_uCapacity; i++)
    {
#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
        m_pIndices[i*6+0] = i*4+0;
        m_pIndices[i*6+1] = i*4+0;
        m_pIndices[i*6+2] = i*4+2;
        m_pIndices[i*6+3] = i*4+1;
        m_pIndices[i*6+4] = i*4+3;
        m_pIndices[i*6+5] = i*4+3;
#else
        m_pIndices[i*6+0] = i*4+0;
        m_pIndices[i*6+1] = i*4+1;
        m_pIndices[i*6+2] = i*4+2;
        
        // inverted index. issue #179
        m_pIndices[i*6+3] = i*4+3;
        m_pIndices[i*6+4] = i*4+2;
        m_pIndices[i*6+5] = i*4+1;
#endif
    }
}

//ImageAtlas - VAO / VBO specific

#if CC_TEXTURE_ATLAS_USE_VAO
void CAImageAtlas::setupVBOandVAO()
{
    glGenVertexArrays(1, &m_uVAOname);
    ccGLBindVAO(m_uVAOname);
    
#define kQuadSize sizeof(m_pQuads[0].bl)
    
    glGenBuffers(2, &m_pBuffersVBO[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * m_uCapacity, m_pQuads, GL_DYNAMIC_DRAW);
    
    // vertices
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, vertices));
    
    // colors
    glEnableVertexAttribArray(kCCVertexAttrib_Color);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, colors));
    
    // tex coords
    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, texCoords));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_pIndices[0]) * m_uCapacity * 6, m_pIndices, GL_STATIC_DRAW);
    
    // Must unbind the VAO before changing the element buffer.
    ccGLBindVAO(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    CHECK_GL_ERROR_DEBUG();
}
#else // CC_TEXTURE_ATLAS_USE_VAO
void CAImageAtlas::setupVBO()
{
    glGenBuffers(2, &m_pBuffersVBO[0]);
    
    mapBuffers();
}
#endif // ! // CC_TEXTURE_ATLAS_USE_VAO

void CAImageAtlas::mapBuffers()
{
    // Avoid changing the element buffer for whatever VAO might be bound.
	ccGLBindVAO(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * m_uCapacity, m_pQuads, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_pIndices[0]) * m_uCapacity * 6, m_pIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    CHECK_GL_ERROR_DEBUG();
}

// ImageAtlas - Update, Insert, Move & Remove

void CAImageAtlas::updateQuad(ccV3F_C4B_T2F_Quad *quad, unsigned int index)
{
    CCAssert( index >= 0 && index < m_uCapacity, "updateQuadWithImage: Invalid index");
    
    m_uTotalQuads = MAX( index+1, m_uTotalQuads);
    
    m_pQuads[index] = *quad;
    
    
    m_bDirty = true;
    
}

void CAImageAtlas::insertQuad(ccV3F_C4B_T2F_Quad *quad, unsigned int index)
{
    CCAssert( index < m_uCapacity, "insertQuadWithImage: Invalid index");
    
    m_uTotalQuads++;
    CCAssert( m_uTotalQuads <= m_uCapacity, "invalid totalQuads");
    
    // issue #575. index can be > totalQuads
    unsigned int remaining = (m_uTotalQuads-1) - index;
    
    // last object doesn't need to be moved
    if( remaining > 0)
    {
        // image coordinates
        memmove( &m_pQuads[index+1],&m_pQuads[index], sizeof(m_pQuads[0]) * remaining );
    }
    
    m_pQuads[index] = *quad;
    
    
    m_bDirty = true;
    
}

void CAImageAtlas::insertQuads(ccV3F_C4B_T2F_Quad* quads, unsigned int index, unsigned int amount)
{
    CCAssert(index + amount <= m_uCapacity, "insertQuadWithImage: Invalid index + amount");
    
    m_uTotalQuads += amount;
    
    CCAssert( m_uTotalQuads <= m_uCapacity, "invalid totalQuads");
    
    // issue #575. index can be > totalQuads
    int remaining = (m_uTotalQuads-1) - index - amount;
    
    // last object doesn't need to be moved
    if( remaining > 0)
    {
        // tex coordinates
        memmove( &m_pQuads[index+amount],&m_pQuads[index], sizeof(m_pQuads[0]) * remaining );
    }
    
    
    unsigned int max = index + amount;
    unsigned int j = 0;
    for (unsigned int i = index; i < max ; i++)
    {
        m_pQuads[index] = quads[j];
        index++;
        j++;
    }
    
    m_bDirty = true;
}

void CAImageAtlas::insertQuadFromIndex(unsigned int oldIndex, unsigned int newIndex)
{
    CCAssert( newIndex >= 0 && newIndex < m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");
    CCAssert( oldIndex >= 0 && oldIndex < m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");
    
    if( oldIndex == newIndex )
    {
        return;
    }
    // because it is ambiguous in iphone, so we implement abs ourselves
    // unsigned int howMany = abs( oldIndex - newIndex);
    unsigned int howMany = (oldIndex - newIndex) > 0 ? (oldIndex - newIndex) :  (newIndex - oldIndex);
    unsigned int dst = oldIndex;
    unsigned int src = oldIndex + 1;
    if( oldIndex > newIndex)
    {
        dst = newIndex+1;
        src = newIndex;
    }
    
    // image coordinates
    ccV3F_C4B_T2F_Quad quadsBackup = m_pQuads[oldIndex];
    memmove( &m_pQuads[dst],&m_pQuads[src], sizeof(m_pQuads[0]) * howMany );
    m_pQuads[newIndex] = quadsBackup;
    
    
    m_bDirty = true;
    
}

void CAImageAtlas::removeQuadAtIndex(unsigned int index)
{
    CCAssert( index < m_uTotalQuads, "removeQuadAtIndex: Invalid index");
    
    unsigned int remaining = (m_uTotalQuads-1) - index;
    
    
    // last object doesn't need to be moved
    if( remaining )
    {
        // image coordinates
        memmove( &m_pQuads[index],&m_pQuads[index+1], sizeof(m_pQuads[0]) * remaining );
    }
    
    m_uTotalQuads--;
    
    
    m_bDirty = true;
    
}

void CAImageAtlas::removeQuadsAtIndex(unsigned int index, unsigned int amount)
{
    CCAssert(index + amount <= m_uTotalQuads, "removeQuadAtIndex: index + amount out of bounds");
    
    unsigned int remaining = (m_uTotalQuads) - (index + amount);
    
    m_uTotalQuads -= amount;
    
    if ( remaining )
    {
        memmove( &m_pQuads[index], &m_pQuads[index+amount], sizeof(m_pQuads[0]) * remaining );
    }
    
    m_bDirty = true;
}

void CAImageAtlas::removeAllQuads()
{
    m_uTotalQuads = 0;
}

// ImageAtlas - Resize
bool CAImageAtlas::resizeCapacity(unsigned int newCapacity)
{
    if( newCapacity == m_uCapacity )
    {
        return true;
    }
    unsigned int uOldCapactiy = m_uCapacity;
    // update capacity and totolQuads
    m_uTotalQuads = MIN(m_uTotalQuads, newCapacity);
    m_uCapacity = newCapacity;
    
    ccV3F_C4B_T2F_Quad* tmpQuads = NULL;
    GLushort* tmpIndices = NULL;
    
    // when calling initWithImage(fileName, 0) on bada device, calloc(0, 1) will fail and return NULL,
    // so here must judge whether m_pQuads and m_pIndices is NULL.
    if (m_pQuads == NULL)
    {
        tmpQuads = (ccV3F_C4B_T2F_Quad*)malloc( m_uCapacity * sizeof(m_pQuads[0]) );
        if (tmpQuads != NULL)
        {
            memset(tmpQuads, 0, m_uCapacity * sizeof(m_pQuads[0]) );
        }
    }
    else
    {
        tmpQuads = (ccV3F_C4B_T2F_Quad*)realloc( m_pQuads, sizeof(m_pQuads[0]) * m_uCapacity );
        if (tmpQuads != NULL && m_uCapacity > uOldCapactiy)
        {
            memset(tmpQuads+uOldCapactiy, 0, (m_uCapacity - uOldCapactiy)*sizeof(m_pQuads[0]) );
        }
    }
    
    if (m_pIndices == NULL)
    {
        tmpIndices = (GLushort*)malloc( m_uCapacity * 6 * sizeof(m_pIndices[0]) );
        if (tmpIndices != NULL)
        {
            memset( tmpIndices, 0, m_uCapacity * 6 * sizeof(m_pIndices[0]) );
        }
        
    }
    else
    {
        tmpIndices = (GLushort*)realloc( m_pIndices, sizeof(m_pIndices[0]) * m_uCapacity * 6 );
        if (tmpIndices != NULL && m_uCapacity > uOldCapactiy)
        {
            memset( tmpIndices+uOldCapactiy, 0, (m_uCapacity-uOldCapactiy) * 6 * sizeof(m_pIndices[0]) );
        }
    }
    
    if( ! ( tmpQuads && tmpIndices) ) {
        CCLOG("CrossApp: CAImageAtlas: not enough memory");
        CC_SAFE_FREE(tmpQuads);
        CC_SAFE_FREE(tmpIndices);
        CC_SAFE_FREE(m_pQuads);
        CC_SAFE_FREE(m_pIndices);
        m_uCapacity = m_uTotalQuads = 0;
        return false;
    }
    
    m_pQuads = tmpQuads;
    m_pIndices = tmpIndices;
    
    
    setupIndices();
    mapBuffers();
    
    m_bDirty = true;
    
    return true;
}

void CAImageAtlas::increaseTotalQuadsWith(unsigned int amount)
{
    m_uTotalQuads += amount;
}

void CAImageAtlas::moveQuadsFromIndex(unsigned int oldIndex, unsigned int amount, unsigned int newIndex)
{
    CCAssert(newIndex + amount <= m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");
    CCAssert(oldIndex < m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");
    
    if( oldIndex == newIndex )
    {
        return;
    }
    //create buffer
    size_t quadSize = sizeof(ccV3F_C4B_T2F_Quad);
    ccV3F_C4B_T2F_Quad* tempQuads = (ccV3F_C4B_T2F_Quad*)malloc( quadSize * amount);
    memcpy( tempQuads, &m_pQuads[oldIndex], quadSize * amount );
    
    if (newIndex < oldIndex)
    {
        // move quads from newIndex to newIndex + amount to make room for buffer
        memmove( &m_pQuads[newIndex], &m_pQuads[newIndex+amount], (oldIndex-newIndex)*quadSize);
    }
    else
    {
        // move quads above back
        memmove( &m_pQuads[oldIndex], &m_pQuads[oldIndex+amount], (newIndex-oldIndex)*quadSize);
    }
    memcpy( &m_pQuads[newIndex], tempQuads, amount*quadSize);
    
    free(tempQuads);
    
    m_bDirty = true;
}

void CAImageAtlas::moveQuadsFromIndex(unsigned int index, unsigned int newIndex)
{
    CCAssert(newIndex + (m_uTotalQuads - index) <= m_uCapacity, "moveQuadsFromIndex move is out of bounds");
    
    memmove(m_pQuads + newIndex,m_pQuads + index, (m_uTotalQuads - index) * sizeof(m_pQuads[0]));
}

void CAImageAtlas::fillWithEmptyQuadsFromIndex(unsigned int index, unsigned int amount)
{
    ccV3F_C4B_T2F_Quad quad;
    memset(&quad, 0, sizeof(quad));
    
    unsigned int to = index + amount;
    for (unsigned int i = index ; i < to ; i++)
    {
        m_pQuads[i] = quad;
    }
}

// ImageAtlas - Drawing

void CAImageAtlas::drawQuads()
{
    this->drawNumberOfQuads(m_uTotalQuads, 0);
}

void CAImageAtlas::drawNumberOfQuads(unsigned int n)
{
    this->drawNumberOfQuads(n, 0);
}

void CAImageAtlas::drawNumberOfQuads(unsigned int n, unsigned int start)
{
    if (0 == n)
    {
        return;
    }
    ccGLBindTexture2D(m_pImage->getName());
    
#if CC_TEXTURE_ATLAS_USE_VAO
    
    //
    // Using VBO and VAO
    //
    
    // XXX: update is done in draw... perhaps it should be done in a timer
    if (m_bDirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_pBuffersVBO[0]);
        // option 1: subdata
        //glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0])*start, sizeof(m_pQuads[0]) * n , &m_pQuads[start] );
		
		// option 2: data
        //		glBufferData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * (n-start), &quads_[start], GL_DYNAMIC_DRAW);
		
		// option 3: orphaning + glMapBuffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * (n-start), NULL, GL_DYNAMIC_DRAW);
		void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(buf, m_pQuads, sizeof(m_pQuads[0])* (n-start));
		glUnmapBuffer(GL_ARRAY_BUFFER);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        m_bDirty = false;
    }
    
    ccGLBindVAO(m_uVAOname);
    
#if CC_REBIND_INDICES_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1]);
#endif
    
#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])) );
#else
    glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])) );
#endif // CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
    
#if CC_REBIND_INDICES_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
    
    //    glBindVertexArray(0);
    
#else // ! CC_TEXTURE_ATLAS_USE_VAO
    
    //
    // Using VBO without VAO
    //
    
#define kQuadSize sizeof(m_pQuads[0].bl)
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffersVBO[0]);
    
    // XXX: update is done in draw... perhaps it should be done in a timer
    if (m_bDirty)
    {
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0])*start, sizeof(m_pQuads[0]) * n , &m_pQuads[start] );
        m_bDirty = false;
    }
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    
    // vertices
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(ccV3F_C4B_T2F, vertices));
    
    // colors
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof(ccV3F_C4B_T2F, colors));
    
    // tex coords
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(ccV3F_C4B_T2F, texCoords));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1]);
    
#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])));
#else
    glDrawElements(GL_TRIANGLES, (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])));
#endif // CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
#endif // CC_TEXTURE_ATLAS_USE_VAO
    
    CC_INCREMENT_GL_DRAWS(1);
    CHECK_GL_ERROR_DEBUG();
}

NS_CC_END


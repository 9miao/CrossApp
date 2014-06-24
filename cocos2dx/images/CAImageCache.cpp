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

#include "CAImageCache.h"
#include "CAImage.h"
#include "ccMacros.h"
#include "CCDirector.h"
#include "platform/platform.h"
#include "platform/CCFileUtils.h"
#include "platform/CCThread.h"
#include "platform/CCImage.h"
#include "support/ccUtils.h"
#include "CAScheduler.h"
#include "cocoa/CCString.h"
#include <errno.h>
#include <stack>
#include <string>
#include <cctype>
#include <queue>
#include <list>

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

typedef enum _AsyncType
{
    AsyncImageType=0,
    AsyncStringType
}AsyncType;

typedef struct _AsyncStruct
{
    std::string            filename;
    CCObject    *target;
    SEL_CallFuncO        selector;
} AsyncStruct;

typedef struct _ImageInfo
{
    AsyncStruct *asyncStruct;
    CCImage        *image;
    CCImage::EImageFormat imageType;
} ImageInfo;

typedef struct _AsyncStringStruct
{
    CCObject             *target;
    
    SEL_CallFuncO        selector;
    
    std::string           text;
    
    std::string           fontName;
    
    float                 fontSize;
    
    CCSize                dimensions;
    
    CATextAlignment       textAlignment;
    
    CAVerticalTextAlignment verticalAlignment;
    
    AsyncType             type;
    
    _AsyncStringStruct()
    :target(NULL)
    ,selector(NULL)
    ,text("")
    ,fontName("")
    ,fontSize(0.0f)
    ,dimensions(CCSizeZero)
    ,textAlignment(CATextAlignmentLeft)
    ,verticalAlignment(CAVerticalTextAlignmentTop)
    {
        
    }
}AsyncStringStruct;

typedef struct _StringInfo
{
    AsyncStringStruct *asyncStruct;
    CCImage        *image;
    CCImage::ETextAlign stringType;
} StringInfo;

static int stringKey = 0;

static pthread_t s_loadingThread;

static pthread_mutex_t		s_SleepMutex;
static pthread_cond_t		s_SleepCondition;

static pthread_mutex_t      s_asyncStructQueueMutex;
static pthread_mutex_t      s_ImageInfoMutex;

//string
static pthread_t      s_loadingStringThread;

static pthread_mutex_t      s_SleepStringMutex;
static pthread_cond_t       s_SleepStringCondition;

static pthread_mutex_t      s_asyncStructStringQueueMutex;
static pthread_mutex_t      s_StringInfoMutex;

#ifdef EMSCRIPTEN
// Hack to get ASM.JS validation (no undefined symbols allowed).
#define pthread_cond_signal(_)
#endif // EMSCRIPTEN

static unsigned long s_nAsyncRefCount = 0;

static unsigned long s_nAsyncStringRefCount = 0;

static bool need_quit = false;

static bool need_quit1 = false;

static std::queue<AsyncStruct*>* s_pAsyncStructQueue = NULL;

static std::queue<ImageInfo*>*   s_pImageQueue = NULL;

static std::queue<AsyncStringStruct*>* s_pAsyncStringStructQueue = NULL;

static std::queue<StringInfo*>*  s_pStringQueue = NULL;

static CCImage::EImageFormat computeImageFormatType(string& filename)
{
    CCImage::EImageFormat ret = CCImage::kFmtUnKnown;

    if ((std::string::npos != filename.find(".jpg")) || (std::string::npos != filename.find(".jpeg")))
    {
        ret = CCImage::kFmtJpg;
    }
    else if ((std::string::npos != filename.find(".png")) || (std::string::npos != filename.find(".PNG")))
    {
        ret = CCImage::kFmtPng;
    }
    else if ((std::string::npos != filename.find(".tiff")) || (std::string::npos != filename.find(".TIFF")))
    {
        ret = CCImage::kFmtTiff;
    }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
    else if ((std::string::npos != filename.find(".webp")) || (std::string::npos != filename.find(".WEBP")))
    {
        ret = CCImage::kFmtWebp;
    }
#endif
   
    return ret;
}

static void loadImageData(AsyncStruct *pAsyncStruct)
{
    const char *filename = pAsyncStruct->filename.c_str();

    // compute image type
    CCImage::EImageFormat imageType = computeImageFormatType(pAsyncStruct->filename);
    if (imageType == CCImage::kFmtUnKnown)
    {
        CCLOG("unsupported format %s",filename);
        delete pAsyncStruct;
        return;
    }
        
    // generate image            
    CCImage *pImage = new CCImage();
    if (pImage && !pImage->initWithImageFileThreadSafe(filename, imageType))
    {
        CC_SAFE_RELEASE(pImage);
        CCLOG("can not load %s", filename);
        return;
    }

    // generate image info
    ImageInfo *pImageInfo = new ImageInfo();
    pImageInfo->asyncStruct = pAsyncStruct;
    pImageInfo->image = pImage;
    pImageInfo->imageType = imageType;
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
        // create autorelease pool for iOS
        CCThread thread;
        thread.createAutoreleasePool();

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

static void loadStringData(AsyncStringStruct *pAsyncStruct)
{
    CCImage::ETextAlign eAlign;
    
    if (CAVerticalTextAlignmentTop == pAsyncStruct->verticalAlignment)
    {
        eAlign = (CATextAlignmentCenter == pAsyncStruct->textAlignment) ? CCImage::kAlignTop
        : (CATextAlignmentLeft == pAsyncStruct->textAlignment) ? CCImage::kAlignTopLeft : CCImage::kAlignTopRight;
    }
    else if (CAVerticalTextAlignmentCenter == pAsyncStruct->verticalAlignment)
    {
        eAlign = (CATextAlignmentCenter == pAsyncStruct->textAlignment) ? CCImage::kAlignCenter
        : (CATextAlignmentLeft == pAsyncStruct->textAlignment) ? CCImage::kAlignLeft : CCImage::kAlignRight;
    }
    else if (CAVerticalTextAlignmentBottom == pAsyncStruct->verticalAlignment)
    {
        eAlign = (CATextAlignmentCenter == pAsyncStruct->textAlignment) ? CCImage::kAlignBottom
        : (CATextAlignmentLeft == pAsyncStruct->textAlignment) ? CCImage::kAlignBottomLeft : CCImage::kAlignBottomRight;
    }
    else
    {
        CCAssert(false, "Not supported alignment format!");
        return ;
    }
    CCImage *pImage = new CCImage();
    if (pImage && !pImage->initWithString(pAsyncStruct->text.c_str(),(int)pAsyncStruct->dimensions.width,(int)pAsyncStruct->dimensions.height,eAlign,pAsyncStruct->fontName.c_str(),(int)pAsyncStruct->fontSize))
    {
        CCLog("can not load");
        CC_SAFE_RELEASE(pImage);
        return;
    }
    // generate image info
    CCLog("generate image info");
    StringInfo *pImageInfo = new StringInfo();
    pImageInfo->asyncStruct = pAsyncStruct;
    pImageInfo->image = pImage;
    pImageInfo->stringType = eAlign;
    // put the image info into the queue
    pthread_mutex_lock(&s_StringInfoMutex);
    s_pStringQueue->push(pImageInfo);
    pthread_mutex_unlock(&s_StringInfoMutex);
}

static void* loadString(void* data)
{
    
    AsyncStringStruct *pAsyncStruct = NULL;
    
    while (true)
    {
        // create autorelease pool for iOS
        CCThread thread;
        thread.createAutoreleasePool();
        
        std::queue<AsyncStringStruct*> *pQueue = s_pAsyncStringStructQueue;
        pthread_mutex_lock(&s_asyncStructStringQueueMutex);// get async struct from queue
        CCLog("*-*-*-*-*-%d",pQueue->empty());
        if (pQueue->empty())
        {
            pthread_mutex_unlock(&s_asyncStructStringQueueMutex);
            CCLog("need_quit1    %d",need_quit1);
            if (need_quit1) {
                break;
            }
            else {
                pthread_cond_wait(&s_SleepStringCondition, &s_SleepStringMutex);
                continue;
            }
        }
        else
        {
            pAsyncStruct = pQueue->front();
            pQueue->pop();
            pthread_mutex_unlock(&s_asyncStructStringQueueMutex);
            loadStringData(pAsyncStruct);
        }
    }
    
    if( s_pAsyncStringStructQueue != NULL )
    {
        
        delete s_pAsyncStringStructQueue;
        s_pAsyncStringStructQueue = NULL;
        delete s_pStringQueue;
        s_pStringQueue = NULL;
        
        pthread_mutex_destroy(&s_asyncStructStringQueueMutex);
        pthread_mutex_destroy(&s_StringInfoMutex);
        pthread_mutex_destroy(&s_SleepStringMutex);
        pthread_cond_destroy(&s_SleepStringCondition);
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
    
    m_pTextures = new CCDictionary();
}

CAImageCache::~CAImageCache()
{
    CCLOGINFO("cocos2d: deallocing CAImageCache.");
    need_quit = true;
    pthread_cond_signal(&s_SleepCondition);
    CC_SAFE_RELEASE(m_pTextures);
}

void CAImageCache::purgeSharedImageCache()
{
    CC_SAFE_RELEASE_NULL(g_sharedImageCache);
}

const char* CAImageCache::description()
{
    return CCString::createWithFormat("<CAImageCache | Number of textures = %u>", m_pTextures->count())->getCString();
}

CCDictionary* CAImageCache::snapshotTextures()
{ 
    CCDictionary* pRet = new CCDictionary();
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pTextures, pElement)
    {
        pRet->setObject(pElement->getObject(), pElement->getStrKey());
    }
    pRet->autorelease();
    return pRet;
}

void CAImageCache::addImageAsync(const char *path, CCObject *target, SEL_CallFuncO selector)
{
#ifdef EMSCRIPTEN
    CCLOGWARN("Cannot load image %s asynchronously in Emscripten builds.", path);
    return;
#endif // EMSCRIPTEN

    CCAssert(path != NULL, "ImageCache: fileimage MUST not be NULL");    

    CAImage* texture = NULL;

    // optimization

    std::string pathKey = path;

    pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());

    texture = (CAImage*)m_pTextures->objectForKey(pathKey.c_str());

    std::string fullpath = pathKey;


    if (texture != NULL)
    {
        if (target && selector)
        {
            (target->*selector)(texture);
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
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
        pthread_create(&s_loadingThread, NULL, loadImage, NULL);
#endif
        need_quit = false;
    }

    if (0 == s_nAsyncRefCount)
    {
        CAScheduler::schedule(schedule_selector(CAImageCache::addImageAsyncCallBack), this, 0, false);
    }

    ++s_nAsyncRefCount;

    if (target)
    {
        target->retain();
    }

    // generate async struct
    AsyncStruct *data = new AsyncStruct();
    data->filename = fullpath.c_str();
    data->target = target;
    data->selector = selector;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
    // add async struct into queue
    pthread_mutex_lock(&s_asyncStructQueueMutex);
    s_pAsyncStructQueue->push(data);
    pthread_mutex_unlock(&s_asyncStructQueueMutex);
    pthread_cond_signal(&s_SleepCondition);
#else
    // WinRT uses an Async Task to load the image since the ThreadPool has a limited number of threads
    //std::replace( data->filename.begin(), data->filename.end(), '/', '\\'); 
    create_task([this, data] {
        loadImageData(data);
    });
#endif
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
        CCImage *pImage = pImageInfo->image;

        CCObject *target = pAsyncStruct->target;
        SEL_CallFuncO selector = pAsyncStruct->selector;
        const char* filename = pAsyncStruct->filename.c_str();

        // generate texture in render thread
        CAImage* texture = new CAImage();
#if 0 //TODO: (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        texture->initWithImage(pImage, kCCResolutioniPhone);
#else
        texture->initWithImage(pImage);
#endif

#if CC_ENABLE_CACHE_TEXTURE_DATA
       // cache the texture file name
       VolatileTexture::addImageTexture(texture, filename, pImageInfo->imageType);
#endif

        // cache the texture
        m_pTextures->setObject(texture, filename);
        texture->autorelease();

        if (target && selector)
        {
            (target->*selector)(texture);
            target->release();
        }        

        pImage->release();
        delete pAsyncStruct;
        delete pImageInfo;

        --s_nAsyncRefCount;
        if (0 == s_nAsyncRefCount)
        {
            CAScheduler::unschedule(schedule_selector(CAImageCache::addImageAsyncCallBack), this);
        }
    }
}

CAImage*  CAImageCache::addImage(const char * path)
{
    CCAssert(path != NULL, "ImageCache: fileimage MUST not be NULL");

    CAImage*  texture = NULL;
    CCImage* pImage = NULL;
    // Split up directory and filename
    // MUTEX:
    // Needed since addImageAsync calls this method from a different thread
    
    //pthread_mutex_lock(m_pDictLock);

    std::string pathKey = path;

    pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
    if (pathKey.size() == 0)
    {
        return NULL;
    }
    texture = (CAImage*)m_pTextures->objectForKey(pathKey.c_str());

    std::string fullpath = pathKey; // (CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(path));
    if (! texture) 
    {
        std::string lowerCase(pathKey);
        for (unsigned int i = 0; i < lowerCase.length(); ++i)
        {
            lowerCase[i] = tolower(lowerCase[i]);
        }
        // all images are handled by UIImage except PVR extension that is handled by our own handler
        do 
        {
            if (std::string::npos != lowerCase.find(".pvr"))
            {
                texture = this->addPVRImage(fullpath.c_str());
            }
            else if (std::string::npos != lowerCase.find(".pkm"))
            {
                // ETC1 file format, only supportted on Android
                texture = this->addETCImage(fullpath.c_str());
            }
            else
            {
                CCImage::EImageFormat eImageFormat = CCImage::kFmtUnKnown;
                if (std::string::npos != lowerCase.find(".png"))
                {
                    eImageFormat = CCImage::kFmtPng;
                }
                else if (std::string::npos != lowerCase.find(".jpg") || std::string::npos != lowerCase.find(".jpeg"))
                {
                    eImageFormat = CCImage::kFmtJpg;
                }
                else if (std::string::npos != lowerCase.find(".tif") || std::string::npos != lowerCase.find(".tiff"))
                {
                    eImageFormat = CCImage::kFmtTiff;
                }
                else if (std::string::npos != lowerCase.find(".webp"))
                {
                    eImageFormat = CCImage::kFmtWebp;
                }
                
                pImage = new CCImage();
                CC_BREAK_IF(NULL == pImage);

                bool bRet = pImage->initWithImageFile(fullpath.c_str(), eImageFormat);
                CC_BREAK_IF(!bRet);

                texture = new CAImage();
                
                if( texture &&
                    texture->initWithImage(pImage) )
                {
#if CC_ENABLE_CACHE_TEXTURE_DATA
                    // cache the texture file name
                    VolatileTexture::addImageTexture(texture, fullpath.c_str(), eImageFormat);
#endif
                    m_pTextures->setObject(texture, pathKey.c_str());
                    texture->release();
                }
                else
                {
                    CCLOG("cocos2d: Couldn't create texture for file:%s in CAImageCache", path);
                }
            }
        } while (0);
    }

    CC_SAFE_RELEASE(pImage);

    //pthread_mutex_unlock(m_pDictLock);
    return texture;
}

CAImage*  CAImageCache::addPVRImage(const char* path)
{
    CCAssert(path != NULL, "ImageCache: fileimage MUST not be nil");

    CAImage* texture = NULL;
    std::string key(path);
    
    if( (texture = (CAImage*)m_pTextures->objectForKey(key.c_str())) ) 
    {
        return texture;
    }

    // Split up directory and filename
    std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(key.c_str());
    texture = new CAImage();
    if(texture != NULL && texture->initWithPVRFile(fullpath.c_str()) )
    {
#if CC_ENABLE_CACHE_TEXTURE_DATA
        // cache the texture file name
        VolatileTexture::addImageTexture(texture, fullpath.c_str(), CCImage::kFmtRawData);
#endif
        m_pTextures->setObject(texture, key.c_str());
        texture->autorelease();
    }
    else
    {
        CCLOG("cocos2d: Couldn't add PVRImage:%s in CAImageCache",key.c_str());
        CC_SAFE_DELETE(texture);
    }

    return texture;
}

CAImage* CAImageCache::addETCImage(const char* path)
{
    CCAssert(path != NULL, "ImageCache: fileimage MUST not be nil");
    
    CAImage* texture = NULL;
    std::string key(path);
    
    if( (texture = (CAImage*)m_pTextures->objectForKey(key.c_str())) )
    {
        return texture;
    }
    
    // Split up directory and filename
    std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(key.c_str());
    texture = new CAImage();
    if(texture != NULL && texture->initWithETCFile(fullpath.c_str()))
    {
        m_pTextures->setObject(texture, key.c_str());
        texture->autorelease();
    }
    else
    {
        CCLOG("cocos2d: Couldn't add ETCImage:%s in CAImageCache",key.c_str());
        CC_SAFE_DELETE(texture);
    }
    
    return texture;
}

CAImage* CAImageCache::addUIImage(CCImage *image, const char *key)
{
    CCAssert(image != NULL, "ImageCache: image MUST not be nil");

    CAImage*  texture = NULL;
    // textureForKey() use full path,so the key should be full path
    std::string forKey;
    if (key)
    {
        forKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(key);
    }

    // Don't have to lock here, because addImageAsync() will not 
    // invoke opengl function in loading thread.

    do 
    {
        // If key is nil, then create a new texture each time
        if(key && (texture = (CAImage* )m_pTextures->objectForKey(forKey.c_str())))
        {
            break;
        }

        // prevents overloading the autorelease pool
        texture = new CAImage();
        texture->initWithImage(image);

        if(key && texture)
        {
            m_pTextures->setObject(texture, forKey.c_str());
            texture->autorelease();
        }
        else
        {
            CCLOG("cocos2d: Couldn't add UIImage in CAImageCache");
        }

    } while (0);

#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTexture::addCCImage(texture, image);
#endif
    
    return texture;
}

bool CAImageCache::reloadTexture(const char* fileName)
{
    std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    if (fullpath.size() == 0)
    {
        return false;
    }
    
    CAImage*  texture = (CAImage*) m_pTextures->objectForKey(fullpath);
    
    bool ret = false;
    if (! texture) {
        texture = this->addImage(fullpath.c_str());
        ret = (texture != NULL);
    }
    else
    {
        do {
            CCImage* image = new CCImage();
            CC_BREAK_IF(NULL == image);
            
            bool bRet = image->initWithImageFile(fullpath.c_str());
            CC_BREAK_IF(!bRet);
            
            ret = texture->initWithImage(image);
        } while (0);
    }
    
    return ret;
}

// ImageCache - Remove

void CAImageCache::removeAllTextures()
{
    m_pTextures->removeAllObjects();
}

void CAImageCache::removeUnusedTextures()
{
    /*
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pTextures, pElement)
    {
        CCLOG("cocos2d: CAImageCache: texture: %s", pElement->getStrKey());
        CAImage* value = (CAImage*)pElement->getObject();
        if (value->retainCount() == 1)
        {
            CCLOG("cocos2d: CAImageCache: removing unused texture: %s", pElement->getStrKey());
            m_pTextures->removeObjectForElememt(pElement);
        }
    }
     */
    
    /** Inter engineer zhuoshi sun finds that this way will get better performance
     */    
    if (m_pTextures->count())
    {   
        // find elements to be removed
        CCDictElement* pElement = NULL;
        list<CCDictElement*> elementToRemove;
        CCDICT_FOREACH(m_pTextures, pElement)
        {
            CCLOG("cocos2d: CAImageCache: texture: %s", pElement->getStrKey());
            CAImage* value = (CAImage*)pElement->getObject();
            if (value->retainCount() == 1)
            {
                elementToRemove.push_back(pElement);
            }
        }
        
        // remove elements
        for (list<CCDictElement*>::iterator iter = elementToRemove.begin(); iter != elementToRemove.end(); ++iter)
        {
            CCLOG("cocos2d: CAImageCache: removing unused texture: %s", (*iter)->getStrKey());
            m_pTextures->removeObjectForElememt(*iter);
        }
    }
}

void CAImageCache::removeTexture(CAImage* texture)
{
    if( ! texture )
    {
        return;
    }

    CCArray* keys = m_pTextures->allKeysForObject(texture);
    m_pTextures->removeObjectsForKeys(keys);
}

void CAImageCache::removeTextureForKey(const char *textureKeyName)
{
    if (textureKeyName == NULL)
    {
        return;
    }

    string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(textureKeyName);
    m_pTextures->removeObjectForKey(fullPath);
}

CAImage* CAImageCache::textureForKey(const char* key)
{
    return (CAImage*)m_pTextures->objectForKey(CCFileUtils::sharedFileUtils()->fullPathForFilename(key));
}

void CAImageCache::reloadAllTextures()
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTexture::reloadAllTextures();
#endif
}

void CAImageCache::dumpCachedTextureInfo()
{
    unsigned int count = 0;
    unsigned int totalBytes = 0;

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pTextures, pElement)
    {
        CAImage* tex = (CAImage*)pElement->getObject();
        unsigned int bpp = tex->bitsPerPixelForFormat();
        // Each texture takes up width * height * bytesPerPixel bytes.
        unsigned int bytes = tex->getPixelsWide() * tex->getPixelsHigh() * bpp / 8;
        totalBytes += bytes;
        count++;
        CCLOG("cocos2d: \"%s\" rc=%lu id=%lu %lu x %lu @ %ld bpp => %lu KB",
               pElement->getStrKey(),
               (long)tex->retainCount(),
               (long)tex->getName(),
               (long)tex->getPixelsWide(),
               (long)tex->getPixelsHigh(),
               (long)bpp,
               (long)bytes / 1024);
    }

    CCLOG("cocos2d: CAImageCache dumpDebugInfo: %ld textures, for %lu KB (%.2f MB)", (long)count, (long)totalBytes / 1024, totalBytes / (1024.0f*1024.0f));
}

#if CC_ENABLE_CACHE_TEXTURE_DATA

std::list<VolatileTexture*> VolatileTexture::textures;
bool VolatileTexture::isReloading = false;

VolatileTexture::VolatileTexture(CAImage* t)
: texture(t)
, m_eCashedImageType(kInvalid)
, m_pTextureData(NULL)
, m_PixelFormat(kImagePixelFormat_RGBA8888)
, m_strFileName("")
, m_FmtImage(CCImage::kFmtPng)
, m_alignment(CATextAlignmentCenter)
, m_vAlignment(CAVerticalTextAlignmentCenter)
, m_strFontName("")
, m_strText("")
, uiImage(NULL)
, m_fFontSize(0.0f)
{
    m_size = CCSizeMake(0, 0);
    m_texParams.minFilter = GL_LINEAR;
    m_texParams.magFilter = GL_LINEAR;
    m_texParams.wrapS = GL_CLAMP_TO_EDGE;
    m_texParams.wrapT = GL_CLAMP_TO_EDGE;
    textures.push_back(this);
}

VolatileTexture::~VolatileTexture()
{
    textures.remove(this);
    CC_SAFE_RELEASE(uiImage);
}

void VolatileTexture::addImageTexture(CAImage* tt, const char* imageFileName, CCImage::EImageFormat format)
{
    if (isReloading)
    {
        return;
    }

    VolatileTexture *vt = findVolotileTexture(tt);

    vt->m_eCashedImageType = kImageFile;
    vt->m_strFileName = imageFileName;
    vt->m_FmtImage    = format;
    vt->m_PixelFormat = tt->getPixelFormat();
}

void VolatileTexture::addCCImage(CAImage* tt, CCImage *image)
{
    VolatileTexture *vt = findVolotileTexture(tt);
    image->retain();
    vt->uiImage = image;
    vt->m_eCashedImageType = kImage;
}

VolatileTexture* VolatileTexture::findVolotileTexture(CAImage* tt)
{
    VolatileTexture *vt = 0;
    std::list<VolatileTexture *>::iterator i = textures.begin();
    while (i != textures.end())
    {
        VolatileTexture *v = *i++;
        if (v->texture == tt) 
        {
            vt = v;
            break;
        }
    }
    
    if (! vt)
    {
        vt = new VolatileTexture(tt);
    }
    
    return vt;
}

void VolatileTexture::addDataTexture(CAImage* tt, void* data, CAImagePixelFormat pixelFormat, const CCSize& contentSize)
{
    if (isReloading)
    {
        return;
    }

    VolatileTexture *vt = findVolotileTexture(tt);

    vt->m_eCashedImageType = kImageData;
    vt->m_pTextureData = data;
    vt->m_PixelFormat = pixelFormat;
    vt->m_TextureSize = contentSize;
}

void VolatileTexture::addStringTexture(CAImage* tt, const char* text, const CCSize& dimensions, CATextAlignment alignment, 
                                       CAVerticalTextAlignment vAlignment, const char *fontName, float fontSize)
{
    if (isReloading)
    {
        return;
    }

    VolatileTexture *vt = findVolotileTexture(tt);

    vt->m_eCashedImageType = kString;
    vt->m_size        = dimensions;
    vt->m_strFontName = fontName;
    vt->m_alignment   = alignment;
    vt->m_vAlignment  = vAlignment;
    vt->m_fFontSize   = fontSize;
    vt->m_strText     = text;
}

void VolatileTexture::setTexParameters(CAImage* t, ccTexParams *texParams) 
{
    VolatileTexture *vt = findVolotileTexture(t);

    if (texParams->minFilter != GL_NONE)
        vt->m_texParams.minFilter = texParams->minFilter;
    if (texParams->magFilter != GL_NONE)
        vt->m_texParams.magFilter = texParams->magFilter;
    if (texParams->wrapS != GL_NONE)
        vt->m_texParams.wrapS = texParams->wrapS;
    if (texParams->wrapT != GL_NONE)
        vt->m_texParams.wrapT = texParams->wrapT;
}

void VolatileTexture::removeTexture(CAImage* t) 
{

    std::list<VolatileTexture *>::iterator i = textures.begin();
    while (i != textures.end())
    {
        VolatileTexture *vt = *i++;
        if (vt->texture == t) 
        {
            delete vt;
            break;
        }
    }
}

void VolatileTexture::reloadAllTextures()
{
    isReloading = true;

    CCLOG("reload all texture");
    std::list<VolatileTexture *>::iterator iter = textures.begin();

    while (iter != textures.end())
    {
        VolatileTexture *vt = *iter++;

        switch (vt->m_eCashedImageType)
        {
        case kImageFile:
            {
                std::string lowerCase(vt->m_strFileName.c_str());
                for (unsigned int i = 0; i < lowerCase.length(); ++i)
                {
                    lowerCase[i] = tolower(lowerCase[i]);
                }

                if (std::string::npos != lowerCase.find(".pvr")) 
                {
                    CAImagePixelFormat oldPixelFormat = CAImage::defaultAlphaPixelFormat();
                    CAImage::setDefaultAlphaPixelFormat(vt->m_PixelFormat);

                    vt->texture->initWithPVRFile(vt->m_strFileName.c_str());
                    CAImage::setDefaultAlphaPixelFormat(oldPixelFormat);
                } 
                else 
                {
                    CCImage* pImage = new CCImage();
                    unsigned long nSize = 0;
                    unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(vt->m_strFileName.c_str(), "rb", &nSize);

                    if (pImage && pImage->initWithImageData((void*)pBuffer, nSize, vt->m_FmtImage))
                    {
                        CAImagePixelFormat oldPixelFormat = CAImage::defaultAlphaPixelFormat();
                        CAImage::setDefaultAlphaPixelFormat(vt->m_PixelFormat);
                        vt->texture->initWithImage(pImage);
                        CAImage::setDefaultAlphaPixelFormat(oldPixelFormat);
                    }

                    CC_SAFE_DELETE_ARRAY(pBuffer);
                    CC_SAFE_RELEASE(pImage);
                }
            }
            break;
        case kImageData:
            {
                vt->texture->initWithData(vt->m_pTextureData, 
                                          vt->m_PixelFormat, 
                                          vt->m_TextureSize.width, 
                                          vt->m_TextureSize.height, 
                                          vt->m_TextureSize);
            }
            break;
        case kString:
            {
                vt->texture->initWithString(vt->m_strText.c_str(),
                                            vt->m_strFontName.c_str(),
                                            vt->m_fFontSize,
                                            vt->m_size,
                                            vt->m_alignment,
                                            vt->m_vAlignment
                                            );
            }
            break;
        case kImage:
            {
                vt->texture->initWithImage(vt->uiImage);
            }
            break;
        default:
            break;
        }
        vt->texture->setTexParameters(&vt->m_texParams);
    }

    isReloading = false;
}

#endif // CC_ENABLE_CACHE_TEXTURE_DATA

NS_CC_END


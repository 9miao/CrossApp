

#include "cocoa/CCNS.h"
#include "ccMacros.h"
#include "CAImageCache.h"
#include "CAImageFrameCache.h"
#include "CAImageFrame.h"
#include "view/CAImageView.h"
#include "support/TransformUtils.h"
#include "platform/CCFileUtils.h"
#include "cocoa/CCString.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCDictionary.h"
#include <vector>

using namespace std;

NS_CC_BEGIN

static CAImageFrameCache *pSharedSpriteFrameCache = NULL;

CAImageFrameCache* CAImageFrameCache::sharedSpriteFrameCache(void)
{
    if (! pSharedSpriteFrameCache)
    {
        pSharedSpriteFrameCache = new CAImageFrameCache();
        pSharedSpriteFrameCache->init();
    }

    return pSharedSpriteFrameCache;
}

void CAImageFrameCache::purgeSharedSpriteFrameCache(void)
{
    CC_SAFE_RELEASE_NULL(pSharedSpriteFrameCache);
}

bool CAImageFrameCache::init(void)
{
    m_pSpriteFrames= new CCDictionary();
    m_pSpriteFramesAliases = new CCDictionary();
    m_pLoadedFileNames = new std::set<std::string>();
    return true;
}

CAImageFrameCache::~CAImageFrameCache(void)
{
    CC_SAFE_RELEASE(m_pSpriteFrames);
    CC_SAFE_RELEASE(m_pSpriteFramesAliases);
    CC_SAFE_DELETE(m_pLoadedFileNames);
}

void CAImageFrameCache::addSpriteFramesWithDictionary(CCDictionary* dictionary, CAImage* pobImage)
{
    /*
    Supported Zwoptex Formats:

    ZWTCoordinatesFormatOptionXMLLegacy = 0, // Flash Version
    ZWTCoordinatesFormatOptionXML1_0 = 1, // Desktop Version 0.0 - 0.4b
    ZWTCoordinatesFormatOptionXML1_1 = 2, // Desktop Version 1.0.0 - 1.0.1
    ZWTCoordinatesFormatOptionXML1_2 = 3, // Desktop Version 1.0.2+
    */

    CCDictionary *metadataDict = (CCDictionary*)dictionary->objectForKey("metadata");
    CCDictionary *framesDict = (CCDictionary*)dictionary->objectForKey("frames");
    int format = 0;

    // get the format
    if(metadataDict != NULL) 
    {
        format = metadataDict->valueForKey("format")->intValue();
    }

    // check the format
    CCAssert(format >=0 && format <= 3, "format is not supported for CAImageFrameCache addSpriteFramesWithDictionary:imageFilename:");

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(framesDict, pElement)
    {
        CCDictionary* frameDict = (CCDictionary*)pElement->getObject();
        const std::string& spriteFrameName = pElement->getStrKey();
        CAImageFrame* spriteFrame = (CAImageFrame*)m_pSpriteFrames->objectForKey(spriteFrameName);
        if (spriteFrame)
        {
            continue;
        }
        
        if(format == 0) 
        {
            float x = frameDict->valueForKey("x")->floatValue();
            float y = frameDict->valueForKey("y")->floatValue();
            float w = frameDict->valueForKey("width")->floatValue();
            float h = frameDict->valueForKey("height")->floatValue();
            float ox = frameDict->valueForKey("offsetX")->floatValue();
            float oy = frameDict->valueForKey("offsetY")->floatValue();
            int ow = frameDict->valueForKey("originalWidth")->intValue();
            int oh = frameDict->valueForKey("originalHeight")->intValue();
            // check ow/oh
            if(!ow || !oh)
            {
                CCLOGWARN("CrossApp: WARNING: originalWidth/Height not found on the CAImageFrame. AnchorPoint won't work as expected. Regenrate the .plist");
            }
            // abs ow/oh
            ow = abs(ow);
            oh = abs(oh);
            // create frame
            spriteFrame = new CAImageFrame();
            spriteFrame->initWithImage(pobImage, 
                                        CCRectMake(x, y, w, h), 
                                        false,
                                        CCPointMake(ox, oy),
                                        CCSizeMake((float)ow, (float)oh)
                                        );
        } 
        else if(format == 1 || format == 2) 
        {
            CCRect frame = CCRectFromString(frameDict->valueForKey("frame")->getCString());
            bool rotated = false;

            // rotation
            if (format == 2)
            {
                rotated = frameDict->valueForKey("rotated")->boolValue();
            }

            CCPoint offset = CCPointFromString(frameDict->valueForKey("offset")->getCString());
            CCSize sourceSize = CCSizeFromString(frameDict->valueForKey("sourceSize")->getCString());

            // create frame
            spriteFrame = new CAImageFrame();
            spriteFrame->initWithImage(pobImage, 
                frame,
                rotated,
                offset,
                sourceSize
                );
        } 
        else if (format == 3)
        {
            // get values
            CCSize spriteSize = CCSizeFromString(frameDict->valueForKey("spriteSize")->getCString());
            CCPoint spriteOffset = CCPointFromString(frameDict->valueForKey("spriteOffset")->getCString());
            CCSize spriteSourceSize = CCSizeFromString(frameDict->valueForKey("spriteSourceSize")->getCString());
            CCRect ImageRect = CCRectFromString(frameDict->valueForKey("ImageRect")->getCString());
            bool imageRotated = frameDict->valueForKey("imageRotated")->boolValue();

            // get aliases
            CCArray* aliases = (CCArray*) (frameDict->objectForKey("aliases"));
            CCString * frameKey = new CCString(spriteFrameName);

            CAObject* pObj = NULL;
            CCARRAY_FOREACH(aliases, pObj)
            {
                std::string oneAlias = ((CCString*)pObj)->getCString();
                if (m_pSpriteFramesAliases->objectForKey(oneAlias.c_str()))
                {
                    CCLOGWARN("CrossApp: WARNING: an alias with name %s already exists", oneAlias.c_str());
                }

                m_pSpriteFramesAliases->setObject(frameKey, oneAlias.c_str());
            }
            frameKey->release();
            // create frame
            spriteFrame = new CAImageFrame();
            spriteFrame->initWithImage(pobImage,
                            CCRectMake(ImageRect.origin.x, ImageRect.origin.y, spriteSize.width, spriteSize.height),
                            imageRotated,
                            spriteOffset,
                            spriteSourceSize);
        }

        // add sprite frame
        m_pSpriteFrames->setObject(spriteFrame, spriteFrameName);
        spriteFrame->release();
    }
}

void CAImageFrameCache::addSpriteFramesWithFile(const char *pszPlist, CAImage* pobImage)
{
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszPlist);
    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());

    addSpriteFramesWithDictionary(dict, pobImage);

    dict->release();
}

void CAImageFrameCache::addSpriteFramesWithFile(const char* plist, const char* imageFileName)
{
    CCAssert(imageFileName, "image name should not be null");
    CAImage* image = CAImageCache::sharedImageCache()->addImage(imageFileName);

    if (image)
    {
        addSpriteFramesWithFile(plist, image);
    }
    else
    {
        CCLOG("CrossApp: CAImageFrameCache: couldn't load image file. File not found %s", imageFileName);
    }
}

void CAImageFrameCache::addSpriteFramesWithFile(const char *pszPlist)
{
    CCAssert(pszPlist, "plist filename should not be NULL");

    if (m_pLoadedFileNames->find(pszPlist) == m_pLoadedFileNames->end())
    {
        std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszPlist);
        CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());

        string imagePath("");

        CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
        if (metadataDict)
        {
            // try to read  image file name from meta data
            imagePath = metadataDict->valueForKey("imageFileName")->getCString();
        }

        if (! imagePath.empty())
        {
            // build image path relative to plist file
            imagePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(imagePath.c_str(), pszPlist);
        }
        else
        {
            // build image path by replacing file extension
            imagePath = pszPlist;

            // remove .xxx
            size_t startPos = imagePath.find_last_of("."); 
            imagePath = imagePath.erase(startPos);

            // append .png
            imagePath = imagePath.append(".png");

            CCLOG("CrossApp: CAImageFrameCache: Trying to use file %s as image", imagePath.c_str());
        }

        CAImage* pimage = CAImageCache::sharedImageCache()->addImage(imagePath.c_str());

        if (pimage)
        {
            addSpriteFramesWithDictionary(dict, pimage);
            m_pLoadedFileNames->insert(pszPlist);
        }
        else
        {
            CCLOG("CrossApp: CAImageFrameCache: Couldn't load image");
        }

        dict->release();
    }

}

void CAImageFrameCache::addSpriteFrame(CAImageFrame *pobFrame, const char *pszFrameName)
{
    m_pSpriteFrames->setObject(pobFrame, pszFrameName);
}

void CAImageFrameCache::removeSpriteFrames(void)
{
    m_pSpriteFrames->removeAllObjects();
    m_pSpriteFramesAliases->removeAllObjects();
    m_pLoadedFileNames->clear();
}

void CAImageFrameCache::removeUnusedSpriteFrames(void)
{
    bool bRemoved = false;
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pSpriteFrames, pElement)
    {
        CAImageFrame* spriteFrame = (CAImageFrame*)pElement->getObject();
        if( spriteFrame->retainCount() == 1 ) 
        {
            CCLOG("CrossApp: CAImageFrameCache: removing unused frame: %s", pElement->getStrKey());
            m_pSpriteFrames->removeObjectForElememt(pElement);
            bRemoved = true;
        }
    }

    // XXX. Since we don't know the .plist file that originated the frame, we must remove all .plist from the cache
    if( bRemoved )
    {
        m_pLoadedFileNames->clear();
    }
}


void CAImageFrameCache::removeSpriteFrameByName(const char *pszName)
{
    // explicit nil handling
    if( ! pszName )
    {
        return;
    }

    // Is this an alias ?
    CCString* key = (CCString*)m_pSpriteFramesAliases->objectForKey(pszName);

    if (key)
    {
        m_pSpriteFrames->removeObjectForKey(key->getCString());
        m_pSpriteFramesAliases->removeObjectForKey(key->getCString());
    }
    else
    {
        m_pSpriteFrames->removeObjectForKey(pszName);
    }

    // XXX. Since we don't know the .plist file that originated the frame, we must remove all .plist from the cache
    m_pLoadedFileNames->clear();
}

void CAImageFrameCache::removeSpriteFramesFromFile(const char* plist)
{
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(plist);
    CCDictionary* dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());

    removeSpriteFramesFromDictionary((CCDictionary*)dict);

    // remove it from the cache
    set<string>::iterator ret = m_pLoadedFileNames->find(plist);
    if (ret != m_pLoadedFileNames->end())
    {
        m_pLoadedFileNames->erase(ret);
    }

    dict->release();
}

void CAImageFrameCache::removeSpriteFramesFromDictionary(CCDictionary* dictionary)
{
    CCDictionary* framesDict = (CCDictionary*)dictionary->objectForKey("frames");
    CCArray* keysToRemove = CCArray::create();

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(framesDict, pElement)
    {
        if (m_pSpriteFrames->objectForKey(pElement->getStrKey()))
        {
            keysToRemove->addObject(CCString::create(pElement->getStrKey()));
        }
    }

    m_pSpriteFrames->removeObjectsForKeys(keysToRemove);
}

void CAImageFrameCache::removeSpriteFramesFromImage(CAImage* image)
{
    CCArray* keysToRemove = CCArray::create();

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pSpriteFrames, pElement)
    {
        string key = pElement->getStrKey();
        CAImageFrame* frame = (CAImageFrame*)m_pSpriteFrames->objectForKey(key.c_str());
        if (frame && (frame->getImage() == image))
        {
            keysToRemove->addObject(CCString::create(pElement->getStrKey()));
        }
    }

    m_pSpriteFrames->removeObjectsForKeys(keysToRemove);
}

CAImageFrame* CAImageFrameCache::spriteFrameByName(const char *pszName)
{
    CAImageFrame* frame = (CAImageFrame*)m_pSpriteFrames->objectForKey(pszName);
    if (!frame)
    {
        // try alias dictionary
        CCString *key = (CCString*)m_pSpriteFramesAliases->objectForKey(pszName);  
        if (key)
        {
            frame = (CAImageFrame*)m_pSpriteFrames->objectForKey(key->getCString());
            if (! frame)
            {
                CCLOG("CrossApp: CAImageFrameCache: Frame '%s' not found", pszName);
            }
        }
    }
    return frame;
}

NS_CC_END


#include "CGSpriteFrameCache.h"
#include "cocoa/CCNS.h"
#include "ccMacros.h"
#include "images/CAImage.h"
#include "images/CAImageCache.h"
#include "CGSpriteFrame.h"
#include "CGSprite.h"
#include "math/TransformUtils.h"
#include "platform/CAFileUtils.h"




NS_CC_BEGIN

static CGSpriteFrameCache *pSharedSpriteFrameCache = nullptr;

CGSpriteFrameCache* CGSpriteFrameCache::getInstance(void)
{
    if (! pSharedSpriteFrameCache)
    {
        pSharedSpriteFrameCache = new CGSpriteFrameCache();
        pSharedSpriteFrameCache->init();
    }
    
    return pSharedSpriteFrameCache;
}

void CGSpriteFrameCache::destroyInstance(void)
{
    CC_SAFE_RELEASE_NULL(pSharedSpriteFrameCache);
}

bool CGSpriteFrameCache::init(void)
{
    m_pLoadedFileNames = new std::set<std::string>();
    return true;
}

CGSpriteFrameCache::~CGSpriteFrameCache(void)
{
    m_obSpriteFrames.clear();
    m_obSpriteFramesAliases.clear();
    CC_SAFE_DELETE(m_pLoadedFileNames);
}

void CGSpriteFrameCache::addSpriteFramesWithDictionary(CAValueMap& dictionary, CAImage* image)
{
    if (dictionary["frames"].getType() != CrossApp::CAValue::Type::MAP)
        return;
    
    CAValueMap& metadataDict = dictionary["metadata"].asValueMap();
    CAValueMap& framesDict = dictionary["frames"].asValueMap();
    int format = 0;

    // get the format
    if(!metadataDict.empty())
    {
        format = metadataDict["format"].asInt();
    }
    
    // check the format
    CCAssert(format >=0 && format <= 3, "format is not supported for CGSpriteFrameCache addSpriteFramesWithDictionary:textureFilename:");
    
    for (auto iter = framesDict.begin(); iter != framesDict.end(); ++iter)
    {
        CAValueMap& frameDict = iter->second.asValueMap();
        std::string spriteFrameName = iter->first;
        CGSpriteFrame* spriteFrame = m_obSpriteFrames.at(spriteFrameName);
        if (spriteFrame)
        {
            continue;
        }
        
        if(format == 0)
        {
            float x = frameDict["x"].asFloat();
            float y = frameDict["y"].asFloat();
            float w = frameDict["width"].asFloat();
            float h = frameDict["height"].asFloat();
            float ox = frameDict["offsetX"].asFloat();
            float oy = frameDict["offsetY"].asFloat();
            int ow = frameDict["originalWidth"].asInt();
            int oh = frameDict["originalHeight"].asInt();
            // check ow/oh
            if(!ow || !oh)
            {
                CCLOG("CrossApp WARNING: originalWidth/Height not found on the CCSpriteFrame. AnchorPoint won't work as expected. Regenrate the .plist");
            }
            // abs ow/oh
            ow = abs(ow);
            oh = abs(oh);
            // create frame
            spriteFrame = CGSpriteFrame::createWithImage(image,
                                                         DRect(x, y, w, h),
                                                         false,
                                                         DPoint(ox, oy),
                                                         DSize(ow, oh)
                                                         );
        }
        else if(format == 1 || format == 2)
        {
            DRect frame = DRectFromString(frameDict["frame"].asString().c_str());
            bool rotated = false;
            
            // rotation
            if (format == 2)
            {
                rotated = frameDict["rotated"].asBool();
            }
            
            DPoint offset = DPointFromString(frameDict["offset"].asString().c_str());
            DSize sourceSize = DSizeFromString(frameDict["sourceSize"].asString().c_str());
            
            // create frame
            spriteFrame = CGSpriteFrame::createWithImage(image,
                                                         frame,
                                                         rotated,
                                                         offset,
                                                         sourceSize
                                                         );
        }
        else if (format == 3)
        {
            // get values
            DSize spriteSize = DSizeFromString(frameDict["spriteSize"].asString().c_str());
            DPoint spriteOffset = DPointFromString(frameDict["spriteOffset"].asString().c_str());
            DSize spriteSourceSize = DSizeFromString(frameDict["spriteSourceSize"].asString().c_str());
            DRect imageRect = DRectFromString(frameDict["textureRect"].asString().c_str());
            bool imageRotated = frameDict["textureRotated"].asBool();
            
            // get aliases
            CAValueVector& aliases = frameDict["aliases"].asValueVector();
            
            for(const auto &value : aliases) {
                std::string oneAlias = value.asString();
                if (m_obSpriteFramesAliases.find(oneAlias) != m_obSpriteFramesAliases.end())
                {
                    CCLOG("CrossApp:WARNING: an alias with name %s already exists", oneAlias.c_str());
                }
                
                m_obSpriteFramesAliases[oneAlias] = CAValue(spriteFrameName);
            }

            spriteFrame = CGSpriteFrame::createWithImage(image,
                                                         DRect(imageRect.origin, spriteSize),
                                                         imageRotated,
                                                         spriteOffset,
                                                         spriteSourceSize);

        }
        
        // add sprite frame
        m_obSpriteFrames.insert(spriteFrameName, spriteFrame);
    }
}

void CGSpriteFrameCache::addSpriteFramesWithFile(const std::string& pszPlist, CAImage *image)
{
    if (m_pLoadedFileNames->find(pszPlist) != m_pLoadedFileNames->end())
    {
        return;//We already added it
    }
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(pszPlist);
    CAValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
    
    addSpriteFramesWithDictionary(dict, image);
    m_pLoadedFileNames->insert(pszPlist);
}

void CGSpriteFrameCache::addSpriteFramesWithFile(const std::string& plist, const std::string& imageFileName)
{
    this->addSpriteFramesWithFile(plist, CAImage::create(imageFileName));
}

void CGSpriteFrameCache::addSpriteFramesWithFile(const std::string& pszPlist)
{
    CCAssert(!pszPlist.empty(), "plist filename should not be NULL");
    
    std::string imageFileName("");
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(pszPlist);
    if (fullPath.size() == 0)
    {
        // return if plist file doesn't exist
        CCLOG("CrossApp:CGSpriteFrameCache: can not find %s", pszPlist.c_str());
        return;
    }
    
    if (m_pLoadedFileNames->find(pszPlist) == m_pLoadedFileNames->end())
    {
        
        CAValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
        
        std::string imagePath("");
        
        if (dict.find("metadata") != dict.end())
        {
            CAValueMap& metadataDict = dict["metadata"].asValueMap();
            // try to read  texture file name from meta data
            imagePath = metadataDict["textureFileName"].asString();
        }
        
        if (!imagePath.empty())
        {
            // build texture path relative to plist file
            imagePath = FileUtils::getInstance()->fullPathFromRelativeFile(imagePath, pszPlist);
        }
        else
        {
            // build texture path by replacing file extension
            imagePath = pszPlist;
            
            // remove .xxx
            size_t startPos = imagePath.find_last_of(".");
            imagePath = imagePath.erase(startPos);
            
            // append .png
            imagePath = imagePath.append(".png");
            
            CCLOG("CrossApp:CGSpriteFrameCache: Trying to use file %s as texture", imagePath.c_str());
        }
        addSpriteFramesWithDictionary(dict, CAImage::create(imagePath));
        m_pLoadedFileNames->insert(pszPlist);
    }
}

void CGSpriteFrameCache::addSpriteFrame(CGSpriteFrame *pobFrame, const std::string& pszFrameName)
{
    m_obSpriteFrames.insert(pszFrameName, pobFrame);
}

void CGSpriteFrameCache::removeSpriteFrames(void)
{
    m_obSpriteFrames.clear();
    m_obSpriteFramesAliases.clear();
    m_pLoadedFileNames->clear();
}

void CGSpriteFrameCache::removeUnusedSpriteFrames(void)
{
    bool removed = false;
    std::vector<std::string> toRemoveFrames;
    
    for (auto iter = m_obSpriteFrames.begin(); iter != m_obSpriteFrames.end(); ++iter)
    {
        CGSpriteFrame* spriteFrame = iter->second;
        if( spriteFrame->retainCount() == 1 )
        {
            toRemoveFrames.push_back(iter->first);
            CCLOG("CrossApp:CGSpriteFrameCache: removing unused frame: %s", iter->first.c_str());
            removed = true;
        }
    }
    m_obSpriteFrames.erase(toRemoveFrames);
    
    // XXX. Since we don't know the .plist file that originated the frame, we must remove all .plist from the cache
    if( removed )
    {
        m_pLoadedFileNames->clear();
    }
}


void CGSpriteFrameCache::removeSpriteFrameByName(const std::string& pszName)
{
    if(pszName.empty() )
    {
        return;
    }
    std::string key = m_obSpriteFramesAliases[pszName].asString();
    if (!key.empty())
    {
        m_obSpriteFrames.erase(key);
        m_obSpriteFramesAliases.erase(key);
    }
    else
    {
        m_obSpriteFrames.erase(pszName);
    }
    m_pLoadedFileNames->clear();
}

void CGSpriteFrameCache::removeSpriteFramesFromFile(const std::string& plist)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(plist);
    CAValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
    if (dict.empty())
    {
        CCLOG("CrossAppCGSpriteFrameCache:removeSpriteFramesFromFile: create dict by %s fail.",plist.c_str());
        return;
    }
    removeSpriteFramesFromDictionary(dict);
    
    // remove it from the cache
    std::set<std::string>::iterator ret = m_pLoadedFileNames->find(plist);
    if (ret != m_pLoadedFileNames->end())
    {
        m_pLoadedFileNames->erase(ret);
    }
}

void CGSpriteFrameCache::removeSpriteFramesFromDictionary(CAValueMap& dictionary)
{
    if (dictionary["frames"].getType() != CrossApp::CAValue::Type::MAP)
        return;
    
    CAValueMap framesDict = dictionary["frames"].asValueMap();
    std::vector<std::string> keysToRemove;
    
    for (auto iter = framesDict.cbegin(); iter != framesDict.cend(); ++iter)
    {
        if (m_obSpriteFrames.at(iter->first))
        {
            keysToRemove.push_back(iter->first);
        }
    }
    
    m_obSpriteFrames.erase(keysToRemove);
}

void CGSpriteFrameCache::removeSpriteFramesFromImage(CAImage* image)
{
    std::vector<std::string> keysToRemove;
    
    for (auto iter = m_obSpriteFrames.begin(); iter != m_obSpriteFrames.end(); ++iter)
    {
        std::string key = iter->first;
        CGSpriteFrame* frame = m_obSpriteFrames.at(key);
        if (frame && (frame->getImage() == image))
        {
            keysToRemove.push_back(key);
        }
    }
    
    m_obSpriteFrames.erase(keysToRemove);
}

CGSpriteFrame* CGSpriteFrameCache::getSpriteFrameByName(const std::string& pszName)
{
    CGSpriteFrame* frame = m_obSpriteFrames.at(pszName);
    if (!frame)
    {
        std::string key = m_obSpriteFramesAliases[pszName].asString();
        if (!key.empty())
        {
            frame = m_obSpriteFrames.at(key);
        }
    }
    return frame;
}

std::vector<std::string> CGSpriteFrameCache::allImageFileNames()
{
    std::vector<std::string> vec;
    for (auto it=m_obSpriteFrames.begin(); it!=m_obSpriteFrames.end(); it++)
    {
        vec.push_back(it->first);
    }
    return vec;
}

NS_CC_END

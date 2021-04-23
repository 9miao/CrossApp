
#ifndef __CGSpriteFrameCache_H__
#define __CGSpriteFrameCache_H__

#include <set>
#include <string>
#include "CGSpriteFrame.h"
#include "basics/CAValue.h"
#include "basics/CASTLContainer.h"

NS_CC_BEGIN

class CGSprite;
class CAImage;


class CCSprite;

class CC_DLL CGSpriteFrameCache : public CAObject
{

public:
    
    static CGSpriteFrameCache* getInstance();
    
    static void destroyInstance();
    
    bool init(void);

    CGSpriteFrameCache(void){}
    
    ~CGSpriteFrameCache(void);
    
public:

    void addSpriteFramesWithFile(const std::string& pszPlist);

    void addSpriteFramesWithFile(const std::string& plist, const std::string& imageFileName);

    void addSpriteFramesWithFile(const std::string& pszPlist, CAImage *image);

    void addSpriteFrame(CGSpriteFrame *pobFrame, const std::string& pszFrameName);

    void removeSpriteFrames(void);

    void removeUnusedSpriteFrames(void);

    void removeSpriteFrameByName(const std::string& pszName);

    void removeSpriteFramesFromFile(const std::string& plist);
    
    std::vector<std::string> allImageFileNames();
    
private:

    void addSpriteFramesWithDictionary(CAValueMap& dictionary, CAImage* image);
    
    void removeSpriteFramesFromDictionary(CAValueMap& dictionary);
    
public:

    void removeSpriteFramesFromImage(CAImage* image);

    CGSpriteFrame* getSpriteFrameByName(const std::string& pszName);
    
protected:
    
    CAMap<std::string, CGSpriteFrame*> m_obSpriteFrames;
    CAValueMap m_obSpriteFramesAliases;
    std::set<std::string>*  m_pLoadedFileNames;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __SPRITE_CCSPRITE_FRAME_CACHE_H__

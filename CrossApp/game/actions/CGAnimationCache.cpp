
#include "game/actions/CGAnimationCache.h"
#include "game/CGSpriteFrameCache.h"
#include "platform/CAFileUtils.h"



NS_CC_BEGIN

AnimationCache* AnimationCache::s_sharedAnimationCache = nullptr;

AnimationCache* AnimationCache::getInstance()
{
    if (! s_sharedAnimationCache)
    {
        s_sharedAnimationCache = new (std::nothrow) AnimationCache();
        s_sharedAnimationCache->init();
    }

    return s_sharedAnimationCache;
}

void AnimationCache::destroyInstance()
{
    CC_SAFE_RELEASE_NULL(s_sharedAnimationCache);
}

bool AnimationCache::init()
{
    return true;
}

AnimationCache::AnimationCache()
{
}

AnimationCache::~AnimationCache()
{
    CCLOG("deallocing AnimationCache: %p", this);
}

void AnimationCache::addAnimation(Animation *animation, const std::string& name)
{
    _animations.insert(name, animation);
}

void AnimationCache::removeAnimation(const std::string& name)
{
    if (name.size()==0)
        return;

    _animations.erase(name);
}

Animation* AnimationCache::getAnimation(const std::string& name)
{
    return _animations.at(name);
}

void AnimationCache::parseVersion1(const CAValueMap& animations)
{
    CGSpriteFrameCache *frameCache = CGSpriteFrameCache::getInstance();

    for (auto iter = animations.cbegin(); iter != animations.cend(); ++iter)
    {
        const CAValueMap& animationDict = iter->second.asValueMap();
        const CAValueVector& frameNames = animationDict.at("frames").asValueVector();
        float delay = animationDict.at("delay").asFloat();
        Animation* animation = nullptr;

        if ( frameNames.empty() )
        {
            CCLOG("CrossApp:AnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.", iter->first.c_str());
            continue;
        }

        size_t frameNameSize = frameNames.size();
        CAVector<AnimationFrame*> frames(frameNameSize);

        for (auto& frameName : frameNames)
        {
            CGSpriteFrame* spriteFrame = frameCache->getSpriteFrameByName(frameName.asString());

            if ( ! spriteFrame ) {
                CCLOG("CrossApp:AnimationCache: Animation '%s' refers to frame '%s' which is not currently in the SpriteFrameCache. This frame will not be added to the animation.", iter->first.c_str(), frameName.asString().c_str());

                continue;
            }

            AnimationFrame* animFrame = AnimationFrame::create(spriteFrame, 1, CAValueMap());
            frames.pushBack(animFrame);
        }

        if ( frames.empty() )
        {
            CCLOG("CrossApp:AnimationCache: None of the frames for animation '%s' were found in the SpriteFrameCache. Animation is not being added to the Animation Cache.", iter->first.c_str());
            continue;
        }
        else if ( frames.size() != frameNameSize )
        {
            CCLOG("CrossApp:AnimationCache: An animation in your dictionary refers to a frame which is not in the SpriteFrameCache. Some or all of the frames for the animation '%s' may be missing.", iter->first.c_str());
        }

        animation = Animation::create(frames, delay, 1);

        AnimationCache::getInstance()->addAnimation(animation, iter->first);
    }
}

void AnimationCache::parseVersion2(const CAValueMap& animations)
{
    CGSpriteFrameCache *frameCache = CGSpriteFrameCache::getInstance();

    for (auto iter = animations.cbegin(); iter != animations.cend(); ++iter)
    {
        std::string name = iter->first;
        CAValueMap& animationDict = const_cast<CAValueMap&>(iter->second.asValueMap());

        const CAValue& loops = animationDict["loops"];
        bool restoreOriginalFrame = animationDict["restoreOriginalFrame"].asBool();

        CAValueVector& frameArray = animationDict["frames"].asValueVector();

        if ( frameArray.empty() )
        {
            CCLOG("CrossApp:AnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.", name.c_str());
            continue;
        }

        // Array of AnimationFrames
        CAVector<AnimationFrame*> array(static_cast<int>(frameArray.size()));

        for (auto& obj : frameArray)
        {
            CAValueMap& entry = obj.asValueMap();
            std::string spriteFrameName = entry["spriteframe"].asString();
            CGSpriteFrame *spriteFrame = frameCache->getSpriteFrameByName(spriteFrameName);

            if( ! spriteFrame ) {
                CCLOG("CrossApp:AnimationCache: Animation '%s' refers to frame '%s' which is not currently in the SpriteFrameCache. This frame will not be added to the animation.", name.c_str(), spriteFrameName.c_str());

                continue;
            }

            float delayUnits = entry["delayUnits"].asFloat();
            CAValue& userInfo = entry["notification"];

            AnimationFrame *animFrame = AnimationFrame::create(spriteFrame, delayUnits, userInfo.getType() == CAValue::Type::MAP ? userInfo.asValueMap() : CAValueMapNull);

            array.pushBack(animFrame);
        }

        float delayPerUnit = animationDict["delayPerUnit"].asFloat();
        Animation *animation = Animation::create(array, delayPerUnit, loops.getType() != CAValue::Type::NONE ? loops.asInt() : 1);

        animation->setRestoreOriginalFrame(restoreOriginalFrame);

        AnimationCache::getInstance()->addAnimation(animation, name);
    }
}

void AnimationCache::addAnimationsWithDictionary(const CAValueMap& dictionary,const std::string& plist)
{
    if ( dictionary.find("animations") == dictionary.end() )
    {
        CCLOG("CrossApp:AnimationCache: No animations were found in provided dictionary.");
        return;
    }
    
    const CAValue& animations = dictionary.at("animations");
    unsigned int version = 1;

    if( dictionary.find("properties") != dictionary.end() )
    {
        const CAValueMap& properties = dictionary.at("properties").asValueMap();
        version = properties.at("format").asInt();
        const CAValueVector& spritesheets = properties.at("spritesheets").asValueVector();

        for(const auto &value : spritesheets) {
            std::string path = FileUtils::getInstance()->fullPathFromRelativeFile(value.asString().c_str(),plist.c_str());
            CGSpriteFrameCache::getInstance()->addSpriteFramesWithFile(path);
        }
    }

    switch (version) {
        case 1:
            parseVersion1(animations.asValueMap());
            break;
        case 2:
            parseVersion2(animations.asValueMap());
            break;
        default:
            CCASSERT(false, "Invalid animation format");
    }
}

/** Read an NSDictionary from a plist file and parse it automatically for animations */
void AnimationCache::addAnimationsWithFile(const std::string& plist)
{
    CCASSERT( plist.size()>0, "Invalid texture file name");

    std::string path = FileUtils::getInstance()->fullPathForFilename(plist);
    CAValueMap dict =  FileUtils::getInstance()->getValueMapFromFile(path);

    CCASSERT( !dict.empty(), "CCAnimationCache: File could not be found");

    addAnimationsWithDictionary(dict, plist);
}


NS_CC_END

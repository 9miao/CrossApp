
#include "CAAnimationCache.h"
#include "ccMacros.h"
#include "CAAnimation.h"
#include "CAImageFrame.h"
#include "CAImageFrameCache.h"
#include "cocoa/CCString.h"
#include "platform/CCFileUtils.h"

using namespace std;

NS_CC_BEGIN

CAAnimationCache* CAAnimationCache::s_pSharedAnimationCache = NULL;

CAAnimationCache* CAAnimationCache::sharedAnimationCache(void)
{
    if (! s_pSharedAnimationCache)
    {
        s_pSharedAnimationCache = new CAAnimationCache();
        s_pSharedAnimationCache->init();
    }

    return s_pSharedAnimationCache;
}

void CAAnimationCache::purgeSharedAnimationCache(void)
{
    CC_SAFE_RELEASE_NULL(s_pSharedAnimationCache);
}

bool CAAnimationCache::init()
{
    m_pAnimations = new CCDictionary();
    return true;
}

CAAnimationCache::CAAnimationCache()
: m_pAnimations(NULL)
{
}

CAAnimationCache::~CAAnimationCache()
{
    CCLOGINFO("CrossApp: deallocing %p", this);
    CC_SAFE_RELEASE(m_pAnimations);
}

void CAAnimationCache::addAnimation(CAAnimation *animation, const char * name)
{
    m_pAnimations->setObject(animation, name);
}

void CAAnimationCache::removeAnimationByName(const char* name)
{
    if (! name)
    {
        return;
    }

    m_pAnimations->removeObjectForKey(name);
}

CAAnimation* CAAnimationCache::animationByName(const char* name)
{
    return (CAAnimation*)m_pAnimations->objectForKey(name);
}

void CAAnimationCache::parseVersion1(CCDictionary* animations)
{
    CAImageFrameCache *frameCache = CAImageFrameCache::sharedSpriteFrameCache();

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(animations, pElement)
    {
        CCDictionary* animationDict = (CCDictionary*)pElement->getObject();
        CCArray* frameNames = (CCArray*)animationDict->objectForKey("frames");
        float delay = animationDict->valueForKey("delay")->floatValue();
        CAAnimation* animation = NULL;

        if ( frameNames == NULL ) 
        {
            CCLOG("CrossApp: CAAnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.", pElement->getStrKey());
            continue;
        }

        CCArray* frames = CCArray::createWithCapacity(frameNames->count());
        frames->retain();

        CAObject* pObj = NULL;
        CCARRAY_FOREACH(frameNames, pObj)
        {
            const char* frameName = ((CCString*)pObj)->getCString();
            CAImageFrame* spriteFrame = frameCache->spriteFrameByName(frameName);

            if ( ! spriteFrame ) {
                CCLOG("CrossApp: CAAnimationCache: Animation '%s' refers to frame '%s' which is not currently in the CAImageFrameCache. This frame will not be added to the animation.", pElement->getStrKey(), frameName);

                continue;
            }

            CAAnimationFrame* animFrame = new CAAnimationFrame();
            animFrame->initWithSpriteFrame(spriteFrame, 1, NULL);
            frames->addObject(animFrame);
            animFrame->release();
        }

        if ( frames->count() == 0 ) {
            CCLOG("CrossApp: CAAnimationCache: None of the frames for animation '%s' were found in the CAImageFrameCache. Animation is not being added to the Animation Cache.", pElement->getStrKey());
            continue;
        } else if ( frames->count() != frameNames->count() ) {
            CCLOG("CrossApp: CAAnimationCache: An animation in your dictionary refers to a frame which is not in the CAImageFrameCache. Some or all of the frames for the animation '%s' may be missing.", pElement->getStrKey());
        }

        animation = CAAnimation::create(frames, delay, 1);

        CAAnimationCache::sharedAnimationCache()->addAnimation(animation, pElement->getStrKey());
        frames->release();
    }    
}

void CAAnimationCache::parseVersion2(CCDictionary* animations)
{
    CAImageFrameCache *frameCache = CAImageFrameCache::sharedSpriteFrameCache();

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(animations, pElement)
    {
        const char* name = pElement->getStrKey();
        CCDictionary* animationDict = (CCDictionary*)pElement->getObject();

        const CCString* loops = animationDict->valueForKey("loops");
        bool restoreOriginalFrame = animationDict->valueForKey("restoreOriginalFrame")->boolValue();

        CCArray* frameArray = (CCArray*)animationDict->objectForKey("frames");

        if ( frameArray == NULL ) {
            CCLOG("CrossApp: CAAnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.", name);
            continue;
        }

        // Array of AnimationFrames
        CCArray* array = CCArray::createWithCapacity(frameArray->count());
        array->retain();

        CAObject* pObj = NULL;
        CCARRAY_FOREACH(frameArray, pObj)
        {
            CCDictionary* entry = (CCDictionary*)(pObj);

            const char* spriteFrameName = entry->valueForKey("spriteframe")->getCString();
            CAImageFrame *spriteFrame = frameCache->spriteFrameByName(spriteFrameName);

            if( ! spriteFrame ) {
                CCLOG("CrossApp: CAAnimationCache: Animation '%s' refers to frame '%s' which is not currently in the CAImageFrameCache. This frame will not be added to the animation.", name, spriteFrameName);

                continue;
            }

            float delayUnits = entry->valueForKey("delayUnits")->floatValue();
            CCDictionary* userInfo = (CCDictionary*)entry->objectForKey("notification");

            CAAnimationFrame *animFrame = new CAAnimationFrame();
            animFrame->initWithSpriteFrame(spriteFrame, delayUnits, userInfo);

            array->addObject(animFrame);
            animFrame->release();
        }

        float delayPerUnit = animationDict->valueForKey("delayPerUnit")->floatValue();
        CAAnimation *animation = new CAAnimation();
        animation->initWithAnimationFrames(array, delayPerUnit, 0 != loops->length() ? loops->intValue() : 1);
        array->release();

        animation->setRestoreOriginalFrame(restoreOriginalFrame);

        CAAnimationCache::sharedAnimationCache()->addAnimation(animation, name);
        animation->release();
    }
}

void CAAnimationCache::addAnimationsWithDictionary(CCDictionary* dictionary,const char* plist)
{
    CCDictionary* animations = (CCDictionary*)dictionary->objectForKey("animations");

    if ( animations == NULL ) {
        CCLOG("CrossApp: CAAnimationCache: No animations were found in provided dictionary.");
        return;
    }

    unsigned int version = 1;
    CCDictionary* properties = (CCDictionary*)dictionary->objectForKey("properties");
    if( properties )
    {
        version = properties->valueForKey("format")->intValue();
        CCArray* spritesheets = (CCArray*)properties->objectForKey("spritesheets");

        CAObject* pObj = NULL;
        CCARRAY_FOREACH(spritesheets, pObj)
        {
            CCString* name = (CCString*)(pObj);
            if (plist)
            {
                const char* path = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(name->getCString(),plist);
                CAImageFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path);
            } 
            else
            {
                CAImageFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(name->getCString());
            }            
        }
    }

    switch (version) {
        case 1:
            parseVersion1(animations);
            break;
        case 2:
            parseVersion2(animations);
            break;
        default:
            CCAssert(false, "Invalid animation format");
    }
}

/** Read an NSDictionary from a plist file and parse it automatically for animations */
void CAAnimationCache::addAnimationsWithFile(const char* plist)
{
    CCAssert( plist, "Invalid Image file name");

    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(plist);
    CCDictionary* dict = CCDictionary::createWithContentsOfFile(path.c_str());

    CCAssert( dict, "CAAnimationCache: File could not be found");

    addAnimationsWithDictionary(dict,plist);
}


NS_CC_END

#ifndef __CC_ANIMATION_CACHE_H__
#define __CC_ANIMATION_CACHE_H__

#include "basics/CAObject.h"
#include "cocoa/CCDictionary.h"

#include <string>

NS_CC_BEGIN

class CAAnimation;

/**
 * @addtogroup sprite_nodes
 * @{
 */

/** Singleton that manages the Animations.
It saves in a cache the animations. You should use this class if you want to save your animations in a cache.

Before v0.99.5, the recommend way was to save them on the CCSprite. Since v0.99.5, you should use this class instead.

@since v0.99.5
*/
class CC_DLL CAAnimationCache : public CAObject
{
public:
    /**
     * @js ctor
     */
    CAAnimationCache();
    /**
     * @js NA
     * @lua NA
     */
    ~CAAnimationCache();
    /** Returns the shared instance of the Animation cache 
     *  @js getInstance
     */
    static CAAnimationCache* sharedAnimationCache(void);

    /** Purges the cache. It releases all the CAAnimation objects and the shared instance.
    */
    static void purgeSharedAnimationCache(void);

    /** Adds a CAAnimation with a name.
    */
    void addAnimation(CAAnimation *animation, const char * name);

    /** Deletes a CAAnimation from the cache.
     *@js removeAnimation
     */
    void removeAnimationByName(const char* name);

    /** Returns a CAAnimation that was previously added.
    If the name is not found it will return nil.
    You should retain the returned copy if you are going to use it.
    @js getAnimation
    */
    CAAnimation* animationByName(const char* name);

    /** Adds an animation from an NSDictionary
     Make sure that the frames were previously loaded in the CAImageFrameCache.
     @param plist The path of the relative file,it use to find the plist path for load SpriteFrames.
     @since v1.1
     */
    void addAnimationsWithDictionary(CCDictionary* dictionary,const char* plist = NULL);

    /** Adds an animation from a plist file.
     Make sure that the frames were previously loaded in the CAImageFrameCache.
     @since v1.1
     @js addAnimations
     */
    void addAnimationsWithFile(const char* plist);

    bool init(void);

private:
    void parseVersion1(CCDictionary* animations);
    void parseVersion2(CCDictionary* animations);
private:
    CCDictionary* m_pAnimations;
    static CAAnimationCache* s_pSharedAnimationCache;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CC_ANIMATION_CACHE_H__
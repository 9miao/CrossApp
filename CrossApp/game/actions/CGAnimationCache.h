
#ifndef __CC_ANIMATION_CACHE_H__
#define __CC_ANIMATION_CACHE_H__

#include "basics/CAObject.h"
#include "basics/CASTLContainer.h"
#include "basics/CAValue.h"
#include "game/actions/CGAnimation.h"

#include <string>

NS_CC_BEGIN


class Animation;

/**
 * @addtogroup _2d
 * @{
 */

/** Singleton that manages the Animations.
It saves in a cache the animations. You should use this class if you want to save your animations in a cache.

Before v0.99.5, the recommend way was to save them on the Sprite. Since v0.99.5, you should use this class instead.

@since v0.99.5
@js cc.animationCache
*/
class CC_DLL AnimationCache : public CAObject
{
public:
    /**
     * @js ctor
     */
    AnimationCache();
    /**
     * @js NA
     * @lua NA
     */
    ~AnimationCache();
    /** Returns the shared instance of the Animation cache 
	 @js NA
	*/
    static AnimationCache* getInstance();

    /** Purges the cache. It releases all the Animation objects and the shared instance.
		@js NA
     */
    static void destroyInstance();

    bool init(void);

    /** Adds a Animation with a name.
     *
     * @param animation An animation.
     * @param name The name of animation.
     */
    void addAnimation(Animation *animation, const std::string& name);

    /** Deletes a Animation from the cache.
     *
     * @param name The name of animation.
     */
    void removeAnimation(const std::string& name);

    /** Returns a Animation that was previously added.
     * If the name is not found it will return nil.
     * You should retain the returned copy if you are going to use it.
     *
     * @return A Animation that was previously added. If the name is not found it will return nil.
     */
    Animation* getAnimation(const std::string& name);

    /** Adds an animation from an NSDictionary.
     * Make sure that the frames were previously loaded in the SpriteFrameCache.
     * @param dictionary An NSDictionary.
     * @param plist The path of the relative file,it use to find the plist path for load SpriteFrames.
     * @since v1.1
	 @js NA
     */
    void addAnimationsWithDictionary(const CAValueMap& dictionary,const std::string& plist);

    /** Adds an animation from a plist file.
     * Make sure that the frames were previously loaded in the SpriteFrameCache.
     * @since v1.1
     * @js addAnimations
     * @lua addAnimations
     * @param plist An animation from a plist file.
     */
    void addAnimationsWithFile(const std::string& plist);

private:
    void parseVersion1(const CAValueMap& animations);
    void parseVersion2(const CAValueMap& animations);

private:
    CAMap<std::string, Animation*> _animations;
    static AnimationCache* s_sharedAnimationCache;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CC_ANIMATION_CACHE_H__

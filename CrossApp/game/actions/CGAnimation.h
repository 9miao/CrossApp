
#ifndef __CC_ANIMATION_H__
#define __CC_ANIMATION_H__

#include "platform/CCPlatformConfig.h"
#include "basics/CAObject.h"
#include "basics/CASTLContainer.h"
#include "basics/CAValue.h"
#include "game/CGSpriteFrame.h"

#include <string>

NS_CC_BEGIN

class CAImage;
class CGSpriteFrame;

/**
 * @addtogroup _2d
 * @{
 */

/** @class AnimationFrame
 *
 * A frame of the animation. It contains information like:
 * - sprite frame name.
 * - # of delay units.
 * - offset
 
 @since v2.0
 */
class CC_DLL AnimationFrame : public CAObject, public CAClonable
{
public:
    /** @struct DisplayedEventInfo
     * When the animation display,Dispatches the event of UserData.
     */
    struct DisplayedEventInfo
    {
        CGSprite* target;
        const CAValueMap* userInfo;
    };
    
    /**
     * Creates the animation frame with a spriteframe, number of delay units and a notification user info.
     *
     * @param spriteFrame The animation frame with a spriteframe.
     * @param delayUnits Number of delay units.
     * @param userInfo A notification user info.
     * @since 3.0
     */
    static AnimationFrame* create(CGSpriteFrame* spriteFrame, float delayUnits, const CAValueMap& userInfo);
    /** Return a SpriteFrameName to be used.
     *
     * @return a SpriteFrameName to be used.
     */
    CGSpriteFrame* getSpriteFrame() const { return _spriteFrame; };
    /** Set the SpriteFrame.
     *
     * @param frame A SpriteFrame will be used.
     */
    void setSpriteFrame(CGSpriteFrame* frame)
    {
        CC_SAFE_RETAIN(frame);
        CC_SAFE_RELEASE(_spriteFrame);
        _spriteFrame = frame;
    }

    /** Gets the units of time the frame takes.
     *
     * @return The units of time the frame takes.
     */
    float getDelayUnits() const { return _delayUnits; };
    
    /** Sets the units of time the frame takes.
     *
     * @param delayUnits The units of time the frame takes.
     */
    void setDelayUnits(float delayUnits) { _delayUnits = delayUnits; };
    
    /** @brief Gets user information
     * A AnimationFrameDisplayedNotification notification will be broadcast when the frame is displayed with this dictionary as UserInfo. 
     * If UserInfo is nil, then no notification will be broadcast.
     *
     * @return A dictionary as UserInfo
     */
    const CAValueMap& getUserInfo() const { return _userInfo; };
    CAValueMap& getUserInfo() { return _userInfo; };
    
    /** Sets user information.
     * @param userInfo A dictionary as UserInfo.
     */
    void setUserInfo(const CAValueMap& userInfo)
    {
        _userInfo = userInfo;
    }
    
    // Overrides
	virtual AnimationFrame *clone() const override;
    
public:
    /**
     * @js ctor
     */
    AnimationFrame();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~AnimationFrame();
    
    /** initializes the animation frame with a spriteframe, number of delay units and a notification user info */
    bool initWithSpriteFrame(CGSpriteFrame* spriteFrame, float delayUnits, const CAValueMap& userInfo);

protected:
    
    /** SpriteFrameName to be used */
    CGSpriteFrame* _spriteFrame;

    /**  how many units of time the frame takes */
    float _delayUnits;

    /**  A AnimationFrameDisplayedNotification notification will be broadcast when the frame is displayed with this dictionary as UserInfo. If UserInfo is nil, then no notification will be broadcast. */
    CAValueMap _userInfo;
    
private:
    DISALLOW_COPY_AND_ASSIGN(AnimationFrame);
};




/** @class Animation
 * A Animation object is used to perform animations on the Sprite objects.
 * The Animation object contains AnimationFrame objects, and a possible delay between the frames.
 * You can animate a Animation object by using the Animate action. Example:
 * @code
 * sprite->runAction(Animate::create(animation));
 * @endcode
*/
class CC_DLL Animation : public CAObject, public CAClonable
{
public:
    /** Creates an animation.
     * @since v0.99.5
     */
    static Animation* create(void);

    /* Creates an animation with an array of SpriteFrame and a delay between frames in seconds.
     * The frames will be added with one "delay unit".
     * @since v0.99.5
     * @param arrayOfSpriteFrameNames An array of SpriteFrame.
     * @param delay A delay between frames in seconds.
     * @param loops The times the animation is going to loop.
     */
    static Animation* createWithSpriteFrames(const CAVector<CGSpriteFrame*>& arrayOfSpriteFrameNames, float delay = 0.0f, unsigned int loops = 1);

    /* Creates an animation with an array of AnimationFrame, the delay per units in seconds and and how many times it should be executed.
     * @since v2.0
     * @param arrayOfAnimationFrameNames An animation with an array of AnimationFrame.
     * @param delayPerUnit The delay per units in seconds and how many times it should be executed.
     * @param loops The times the animation is going to loop.
     */
    static Animation* create(const CAVector<AnimationFrame*>& arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops = 1);

    /** Adds a SpriteFrame to a Animation.
     *
     * @param frame The frame will be added with one "delay unit".
     */
    void addSpriteFrame(CGSpriteFrame *frame);

    /** Adds a frame with an image filename. Internally it will create a SpriteFrame and it will add it.
     * The frame will be added with one "delay unit".
     * Added to facilitate the migration from v0.8 to v0.9.
     * @param filename The path of SpriteFrame.
     */
    void addSpriteFrameWithFile(const std::string& filename);
    /**
     * @deprecated. Use addSpriteFrameWithFile() instead.
	 @js NA
     */
    CC_DEPRECATED_ATTRIBUTE void addSpriteFrameWithFileName(const std::string& filename){ addSpriteFrameWithFile(filename);}

    /** Adds a frame with a texture and a rect. Internally it will create a SpriteFrame and it will add it.
     * The frame will be added with one "delay unit".
     * Added to facilitate the migration from v0.8 to v0.9.
     * @param pobTexture A frame with a texture.
     * @param rect The Texture of rect.
     */
    void addSpriteFrameWithImage(CAImage* image, const DRect& rect);

    /** Gets the total Delay units of the Animation. 
     *
     * @return The total Delay units of the Animation.
     */
    float getTotalDelayUnits() const { return _totalDelayUnits; };
    
    /** Sets the delay in seconds of the "delay unit".
     *
     * @param delayPerUnit The delay in seconds of the "delay unit".
     */
    void setDelayPerUnit(float delayPerUnit) { _delayPerUnit = delayPerUnit; };
    
    /** Gets the delay in seconds of the "delay unit".
     * 
     * @return The delay in seconds of the "delay unit".
     */
    float getDelayPerUnit() const { return _delayPerUnit; };

    
    /** Gets the duration in seconds of the whole animation. It is the result of totalDelayUnits * delayPerUnit.
     *
     * @return Result of totalDelayUnits * delayPerUnit.
     */
    float getDuration() const;
    
    /** Gets the array of AnimationFrames.
     * 
     * @return The array of AnimationFrames.
     */
    const CAVector<AnimationFrame*>& getFrames() const { return _frames; };
    
    /** Sets the array of AnimationFrames. 
     *
     * @param frames The array of AnimationFrames.
     */
    void setFrames(const CAVector<AnimationFrame*>& frames)
    {
        _frames = frames;
    }
    
    /** Checks whether to restore the original frame when animation finishes. 
     *
     * @return Restore the original frame when animation finishes.
     */
    bool getRestoreOriginalFrame() const { return _restoreOriginalFrame; };
    
    /** Sets whether to restore the original frame when animation finishes. 
     *
     * @param restoreOriginalFrame Whether to restore the original frame when animation finishes.
     */
    void setRestoreOriginalFrame(bool restoreOriginalFrame) { _restoreOriginalFrame = restoreOriginalFrame; };
    
    /** Gets the times the animation is going to loop. 0 means animation is not animated. 1, animation is executed one time, ... 
     *
     * @return The times the animation is going to loop.
     */
    unsigned int getLoops() const { return _loops; };
    
    /** Sets the times the animation is going to loop. 0 means animation is not animated. 1, animation is executed one time, ... 
     *
     * @param loops The times the animation is going to loop.
     */
    void setLoops(unsigned int loops) { _loops = loops; };
    
    // overrides
	virtual Animation *clone() const override;
    
public:
    Animation();
    virtual ~Animation(void);
    
    /** Initializes a Animation. */
    bool init();
    
    /** Initializes a Animation with frames and a delay between frames.
     * @since v0.99.5
     */
    bool initWithSpriteFrames(const CAVector<CGSpriteFrame*>& arrayOfSpriteFrameNames, float delay = 0.0f, unsigned int loops = 1);
    
    /** Initializes a Animation with AnimationFrame.
     * @since v2.0
     */
    bool initWithAnimationFrames(const CAVector<AnimationFrame*>& arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops);

protected:
    /** total Delay units of the Animation. */
    float _totalDelayUnits;

    /** Delay in seconds of the "delay unit". */
    float _delayPerUnit;

    /** duration in seconds of the whole animation. It is the result of totalDelayUnits * delayPerUnit. */
    float _duration;

    /** array of AnimationFrames. */
    CAVector<AnimationFrame*> _frames;

    /** whether or not it shall restore the original frame when the animation finishes. */
    bool _restoreOriginalFrame;

    /** how many times the animation is going to loop. 0 means animation is not animated. 1, animation is executed one time, ... */
    unsigned int _loops;
    
private:
    DISALLOW_COPY_AND_ASSIGN(Animation);
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CC_ANIMATION_H__

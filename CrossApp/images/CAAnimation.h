
#ifndef __CC_ANIMATION_H__
#define __CC_ANIMATION_H__

#include "platform/CCPlatformConfig.h"
#include "basics/CAObject.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCDictionary.h"
#include "basics/CAGeometry.h"
#include "CAImageFrame.h"
#include <string>

NS_CC_BEGIN

class CAImage;
class CAImageFrame;

/**
 * @addtogroup sprite_nodes
 * @{
 */

/** CAAnimationFrame
 A frame of the animation. It contains information like:
    - sprite frame name
    - # of delay units.
    - offset
 
 @since v2.0
 */
class CC_DLL CAAnimationFrame : public CAObject
{
public:
    /**
     * @js ctor
     */
    CAAnimationFrame();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CAAnimationFrame();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone* pZone);
    /** initializes the animation frame with a spriteframe, number of delay units and a notification user info */
    bool initWithSpriteFrame(CAImageFrame* spriteFrame, float delayUnits, CCDictionary* userInfo);
    
    /** CAImageFrameName to be used */
    CC_SYNTHESIZE_RETAIN(CAImageFrame*, m_pSpriteFrame, SpriteFrame)

    /**  how many units of time the frame takes */
    CC_SYNTHESIZE(float, m_fDelayUnits, DelayUnits)

    /**  A CAAnimationFrameDisplayedNotification notification will be broadcast when the frame is displayed with this dictionary as UserInfo. If UserInfo is nil, then no notification will be broadcast. */
    CC_SYNTHESIZE_RETAIN(CCDictionary*, m_pUserInfo, UserInfo)
};




/** A CAAnimation object is used to perform animations on the CAImageView objects.

The CAAnimation object contains CAAnimationFrame objects, and a possible delay between the frames.
You can animate a CAAnimation object by using the CCAnimate action. Example:

[sprite runAction:[CCAnimate actionWithAnimation:animation]];

*/
class CC_DLL CAAnimation : public CAObject
{
public:
    /**
     * @js ctor
     */
    CAAnimation();
    /**
     * @js NA
     * @lua NA
     */
    ~CAAnimation(void);
public:
    /** Creates an animation
    @since v0.99.5
    */
    static CAAnimation* create(void);

    /* Creates an animation with an array of CAImageFrame and a delay between frames in seconds.
     The frames will be added with one "delay unit".
     @since v0.99.5
     @js create
    */
    static CAAnimation* createWithSpriteFrames(CCArray* arrayOfSpriteFrameNames, float delay = 0.0f);

    /* Creates an animation with an array of CAAnimationFrame, the delay per units in seconds and and how many times it should be executed.
     @since v2.0
     */
    static CAAnimation* create(CCArray *arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops);
    static CAAnimation* create(CCArray *arrayOfAnimationFrameNames, float delayPerUnit) {
        return CAAnimation::create(arrayOfAnimationFrameNames, delayPerUnit, 1);
    }

    /** Adds a CAImageFrame to a CAAnimation.
     The frame will be added with one "delay unit".
    */
    void addSpriteFrame(CAImageFrame *pFrame);

    /** Adds a frame with an image filename. Internally it will create a CAImageFrame and it will add it.
     The frame will be added with one "delay unit".
     Added to facilitate the migration from v0.8 to v0.9.
     * @js addSpriteFrameWithFile
     */  
    void addSpriteFrameWithFileName(const char *pszFileName);

    /** Adds a frame with a Image and a rect. Internally it will create a CAImageFrame and it will add it.
     The frame will be added with one "delay unit".
     Added to facilitate the migration from v0.8 to v0.9.
     */
    void addSpriteFrameWithImage(CAImage* pobImage, const CCRect& rect);
    /**
     * @lua NA
     */
    bool init();

    /** Initializes a CAAnimation with frames and a delay between frames
     @since v0.99.5
     @lua NA
    */
    bool initWithSpriteFrames(CCArray *pFrames, float delay = 0.0f);

    /** Initializes a CAAnimation with CAAnimationFrame
     @since v2.0
     @lua NA
    */
    bool initWithAnimationFrames(CCArray* arrayOfAnimationFrames, float delayPerUnit, unsigned int loops);
    /**
     * @js NA
     * @lua NA
     */
    virtual CAObject* copyWithZone(CAZone* pZone);

    /** total Delay units of the CAAnimation. */
    CC_SYNTHESIZE_READONLY(float, m_fTotalDelayUnits, TotalDelayUnits)

    /** Delay in seconds of the "delay unit" */
    CC_SYNTHESIZE(float, m_fDelayPerUnit, DelayPerUnit)

    /** duration in seconds of the whole animation. It is the result of totalDelayUnits * delayPerUnit */
    CC_PROPERTY_READONLY(float, m_fDuration, Duration)

    /** array of CAAnimationFrames */
    CC_SYNTHESIZE_RETAIN(CCArray*, m_pFrames, Frames)

    /** whether or not it shall restore the original frame when the animation finishes */
    CC_SYNTHESIZE(bool, m_bRestoreOriginalFrame, RestoreOriginalFrame)

    /** how many times the animation is going to loop. 0 means animation is not animated. 1, animation is executed one time, ... */
    CC_SYNTHESIZE(unsigned int, m_uLoops, Loops)
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CC_ANIMATION_H__

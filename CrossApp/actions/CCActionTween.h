
#ifndef __CCACTIONTWEEN_H__
#define __CCACTIONTWEEN_H__

#include "CCActionInterval.h"

NS_CC_BEGIN

/**
 * @addtogroup actions
 * @{
 */

class CC_DLL CCActionTweenDelegate
{
public:
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCActionTweenDelegate() {}
    virtual void updateTweenAction(float value, const char* key) = 0;
};

/** CCActionTween

 CCActionTween is an action that lets you update any property of an object.
 For example, if you want to modify the "width" property of a target from 200 to 300 in 2 seconds, then:

    id modifyWidth = [CCActionTween actionWithDuration:2 key:@"width" from:200 to:300];
    [target runAction:modifyWidth];


 Another example: CCScaleTo action could be rewritten using CCPropertyAction:

    // scaleA and scaleB are equivalents
    id scaleA = [CCScaleTo actionWithDuration:2 scale:3];
    id scaleB = [CCActionTween actionWithDuration:2 key:@"scale" from:1 to:3];


 @since v0.99.2
 */
class CC_DLL CCActionTween : public CCActionInterval
{
public:
    /** creates an initializes the action with the property name (key), and the from and to parameters. */
    static CCActionTween* create(float aDuration, const char* key, float from, float to);
    /** initializes the action with the property name (key), and the from and to parameters. */
    bool initWithDuration(float aDuration, const char* key, float from, float to);

    void startWithTarget(CAView *pTarget);
    void update(float dt);
    CCActionInterval* reverse();

    std::string        m_strKey;
    float            m_fFrom, m_fTo;
    float            m_fDelta;
};

// end of actions group
/// @}

NS_CC_END

#endif /* __CCACTIONTWEEN_H__ */



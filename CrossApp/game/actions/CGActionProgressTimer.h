
#ifndef __ACTION_CCPROGRESS_TIMER_H__
#define __ACTION_CCPROGRESS_TIMER_H__

#include "game/actions/CGActionInterval.h"

NS_CC_BEGIN

/**
 * @addtogroup actions
 * @{
 */

/**
@brief Progress to percentage.
@details This action show the target node from current percentage to the specified percentage.
        You should specify the destination percentage when creating the action.
@since v0.99.1
*/
class CC_DLL ProgressTo : public ActionInterval
{
public:
    /** 
     * @brief Create and initializes with a duration and a destination percentage.
     * @param duration Specify the duration of the ProgressTo action. It's a value in seconds.
     * @param percent Specify the destination percentage.
     * @return If the creation success, return a pointer of ProgressTo action; otherwise, return nil.
     */
    static ProgressTo* create(float duration, float percent);

    //
    // Overrides
    //
	virtual ProgressTo* clone() const override;
	virtual ProgressTo* reverse() const override;
    virtual void startWithTarget(CGSprite *target) override;
    virtual void update(float time) override;
    
public:
    ProgressTo() {}
    virtual ~ProgressTo() {}

    /** 
     * @brief Initializes with a duration and destination percentage. 
     * @param duration Specify the duration of the ProgressTo action. It's a value in seconds.
     * @param percent Specify the destination percentage.
     * @return If the creation success, return true; otherwise, return false.
     */
    bool initWithDuration(float duration, float percent);

protected:
    float _to;
    float _from;

private:
    M_DISALLOW_COPY_AND_ASSIGN(ProgressTo);
};

/**
@brief Progress from a percentage to another percentage.
@since v0.99.1
*/
class CC_DLL ProgressFromTo : public ActionInterval
{
public:
    /** 
     * @brief Create and initializes the action with a duration, a "from" percentage and a "to" percentage.
     * @param duration Specify the duration of the ProgressFromTo action. It's a value in seconds.
     * @param fromPercentage Specify the source percentage.
     * @param toPercentage Specify the destination percentage.
     * @return If the creation success, return a pointer of ProgressFromTo action; otherwise, return nil.
     */
    static ProgressFromTo* create(float duration, float fromPercentage, float toPercentage);

    //
    // Overrides
    //
	virtual ProgressFromTo* clone() const override;
	virtual ProgressFromTo* reverse() const override;
    virtual void startWithTarget(CGSprite *target) override;
    virtual void update(float time) override;
    
public:
    ProgressFromTo() {}
    virtual ~ProgressFromTo() {}

    /**
     * @brief Initializes the action with a duration, a "from" percentage and a "to" percentage.
     * @param duration Specify the duration of the ProgressFromTo action. It's a value in seconds.
     * @param fromPercentage Specify the source percentage.
     * @param toPercentage Specify the destination percentage.
     * @return If the creation success, return true; otherwise, return false.
     */
    bool initWithDuration(float duration, float fromPercentage, float toPercentage);

protected:
    float _to;
    float _from;

private:
    M_DISALLOW_COPY_AND_ASSIGN(ProgressFromTo);
};

// end of actions group
/// @}

NS_CC_END

#endif // __ACTION_CCPROGRESS_TIMER_H__

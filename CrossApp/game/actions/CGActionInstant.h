

#ifndef __CCINSTANT_ACTION_H__
#define __CCINSTANT_ACTION_H__

#include <functional>
#include "game/actions/CGAction.h"

NS_CC_BEGIN

/**
 * @addtogroup actions
 * @{
 */

/** @class ActionInstant
* @brief Instant actions are immediate actions. They don't have a duration like the IntervalAction actions.
**/
class CC_DLL ActionInstant : public FiniteTimeAction //<NSCopying>
{
public:
    //
    // Overrides
    //
    virtual ActionInstant* clone() const override
    {
        CC_ASSERT(0);
        return nullptr;
    }
    
    virtual ActionInstant * reverse() const override
    {
        CC_ASSERT(0);
        return nullptr;
    }

    virtual bool isDone() const override;
    /**
     * @param dt In seconds.
     */
    virtual void step(float dt) override;
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;
};

/** @class Show
* @brief Show the node.
**/
class CC_DLL Show : public ActionInstant
{
public:
    /** Allocates and initializes the action.
     *
     * @return  An autoreleased Show object.
     */
    static Show * create();

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;
    virtual ActionInstant* reverse() const override;
    virtual Show* clone() const override;

public:
    Show(){}
    virtual ~Show(){}

private:
    M_DISALLOW_COPY_AND_ASSIGN(Show);
};

/** @class Hide
* @brief Hide the node.
*/
class CC_DLL Hide : public ActionInstant
{
public:
    /** Allocates and initializes the action.
     *
     * @return An autoreleased Hide object.
     */
    static Hide * create();

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;
    virtual ActionInstant* reverse() const override;
    virtual Hide* clone() const override;

public:
    Hide(){}
    virtual ~Hide(){}

private:
    M_DISALLOW_COPY_AND_ASSIGN(Hide);
};

/** @class ToggleVisibility
* @brief Toggles the visibility of a node.
*/
class CC_DLL ToggleVisibility : public ActionInstant
{
public:
    /** Allocates and initializes the action.
     *
     * @return An autoreleased ToggleVisibility object.
     */
    static ToggleVisibility * create();

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;
    virtual ToggleVisibility* reverse() const override;
    virtual ToggleVisibility* clone() const override;

public:
    ToggleVisibility(){}
    virtual ~ToggleVisibility(){}

private:
    M_DISALLOW_COPY_AND_ASSIGN(ToggleVisibility);
};

/** @class RemoveSelf
* @brief Remove the node.
*/
class CC_DLL RemoveSelf : public ActionInstant
{
public:

    static RemoveSelf * create();

    virtual void update(float time) override;
    virtual RemoveSelf* clone() const override;
    virtual RemoveSelf* reverse() const override;
    
public:
    RemoveSelf(){}
    virtual ~RemoveSelf(){}
    bool init();

private:
    M_DISALLOW_COPY_AND_ASSIGN(RemoveSelf);
};

/** @class FlipX
* @brief Flips the sprite horizontally.
* @since v0.99.0
*/
class CC_DLL FlipX : public ActionInstant
{
public:
    /** Create the action.
     *
     * @param x Flips the sprite horizontally if true.
     * @return  An autoreleased FlipX object.
     */
    static FlipX * create(bool x);

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;
    virtual FlipX* reverse() const override;
    virtual FlipX* clone() const override;
    
public:
    FlipX() :_flipX(false) {}
    virtual ~FlipX() {}

    /** init the action */
    bool initWithFlipX(bool x);

protected:
    bool    _flipX;

private:
    M_DISALLOW_COPY_AND_ASSIGN(FlipX);
};

/** @class FlipY
* @brief Flips the sprite vertically.
* @since v0.99.0
*/
class CC_DLL FlipY : public ActionInstant
{
public:
    /** Create the action.
     *
     * @param y Flips the sprite vertically if true.
     * @return An autoreleased FlipY object.
     */
    static FlipY * create(bool y);

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;
    virtual FlipY* reverse() const override;
    virtual FlipY* clone() const override;
    
public:
    FlipY() :_flipY(false) {}
    virtual ~FlipY() {}

    /** init the action */
    bool initWithFlipY(bool y);

protected:
    bool    _flipY;

private:
    M_DISALLOW_COPY_AND_ASSIGN(FlipY);
};

/** @class Place
* @brief Places the node in a certain position.
*/
class CC_DLL Place : public ActionInstant //<NSCopying>
{
public:

    /** Creates a Place action with a position.
     *
     * @param pos  A certain position.
     * @return  An autoreleased Place object.
     */
    static Place * create(const DPoint& pos);

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;
    virtual Place* reverse() const override;
    virtual Place* clone() const override;
    
public:
    Place(){}
    virtual ~Place(){}

    /** Initializes a Place action with a position */
    bool initWithPosition(const DPoint& pos);

protected:
    DPoint _position;

private:
    M_DISALLOW_COPY_AND_ASSIGN(Place);
};


/** @class CallFunc
* @brief Calls a 'callback'.
*/
class CC_DLL CallFunc : public ActionInstant //<NSCopying>
{
public:
    /** Creates the action with the callback of type std::function<void()>.
     This is the preferred way to create the callback.
     * When this function bound in js or lua ,the input param will be changed.
     * In js: var create(var func, var this, var [data]) or var create(var func).
     * In lua:local create(local funcID).
     *
     * @param func  A callback function need to be executed.
     * @return  An autoreleased CallFunc object.
     */
    static CallFunc * create(const std::function<void()>& func);

public:
    /** Executes the callback.
     */
    virtual void execute();

    virtual void update(float time) override;
    virtual CallFunc* reverse() const override;
    virtual CallFunc* clone() const override;
    
public:
    CallFunc()
    :_function(nullptr)
    {
    }
    virtual ~CallFunc();

    /** initializes the action with the std::function<void()>
     * @lua NA
     */
    bool initWithFunction(const std::function<void()>& func);

protected:

    std::function<void()> _function;

private:
    M_DISALLOW_COPY_AND_ASSIGN(CallFunc);
};

/** @class CallFuncN
* @brief Calls a 'callback' with the node as the first argument. N means Node.
* @js NA
*/
class CC_DLL CallFuncN : public CallFunc
{
public:
    /** Creates the action with the callback of type std::function<void()>.
     This is the preferred way to create the callback.
     *
     * @param func  A callback function need to be executed.
     * @return  An autoreleased CallFuncN object.
     */
    static CallFuncN * create(const std::function<void(CGSprite*)>& func);
    //
    // Overrides
    //
    virtual CallFuncN* clone() const override;
    virtual void execute() override;
    
public:
    CallFuncN():_functionN(nullptr){}
    virtual ~CallFuncN(){}

    /** initializes the action with the std::function<void(CGSprite*)> */
    bool initWithFunction(const std::function<void(CGSprite*)>& func);

protected:
    /** function that will be called with the "sender" as the 1st argument */
    std::function<void(CGSprite*)> _functionN;

private:
    M_DISALLOW_COPY_AND_ASSIGN(CallFuncN);
};

// end of actions group
/// @}

NS_CC_END

#endif //__CCINSTANT_ACTION_H__

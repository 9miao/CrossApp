

#include "game/actions/CGActionInstant.h"
#include "game/CGSprite.h"
#include "game/CGSprite.h"

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (push)
#pragma warning (disable: 4996)
#endif

NS_CC_BEGIN
//
// InstantAction
//
bool ActionInstant::isDone() const
{
    return true;
}

void ActionInstant::step(float dt) {
    CC_UNUSED_PARAM(dt);
    float updateDt = 1;
#if CC_ENABLE_SCRIPT_BINDING
//    if (_scriptType == kScriptTypeJavascript)
//    {
//        if (ScriptEngineManager::sendActionEventToJS(this, kActionUpdate, (void *)&updateDt))
//            return;
//    }
#endif
    update(updateDt);
}

void ActionInstant::update(float time) {
    CC_UNUSED_PARAM(time);
    // nothing
}

//
// Show
//

Show* Show::create() 
{
    Show* ret = new (std::nothrow) Show();

    if (ret) {
        ret->autorelease();
    }

    return ret;
}

void Show::update(float time) {
    CC_UNUSED_PARAM(time);
    _target->setVisible(true);
}

ActionInstant* Show::reverse() const
{
    return Hide::create();
}

Show * Show::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) Show();
    a->autorelease();
    return a;
}

//
// Hide
//
Hide * Hide::create() 
{
    Hide *ret = new (std::nothrow) Hide();

    if (ret) {
        ret->autorelease();
    }

    return ret;
}

void Hide::update(float time) {
    CC_UNUSED_PARAM(time);
    _target->setVisible(false);
}

ActionInstant *Hide::reverse() const
{
    return Show::create();
}

Hide * Hide::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) Hide();
    a->autorelease();
    return a;
}

//
// ToggleVisibility
//
ToggleVisibility * ToggleVisibility::create()
{
    ToggleVisibility *ret = new (std::nothrow) ToggleVisibility();

    if (ret)
    {
        ret->autorelease();
    }

    return ret;
}

void ToggleVisibility::update(float time) 
{
    CC_UNUSED_PARAM(time);
    _target->setVisible(!_target->isVisible());
}

ToggleVisibility * ToggleVisibility::reverse() const
{
    return ToggleVisibility::create();
}

ToggleVisibility * ToggleVisibility::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) ToggleVisibility();
    a->autorelease();
    return a;
}

//
// Remove Self
//
RemoveSelf * RemoveSelf::create()
{
    RemoveSelf *ret = new (std::nothrow) RemoveSelf();

    if (ret && ret->init()) {
        ret->autorelease();
    }

    return ret;
}

bool RemoveSelf::init()
{
    return true;
}

void RemoveSelf::update(float time)
{
    CC_UNUSED_PARAM(time);
    _target->removeFromSuperview();
}

RemoveSelf *RemoveSelf::reverse() const
{
    return RemoveSelf::create();
}

RemoveSelf * RemoveSelf::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) RemoveSelf();
    a->init();
    a->autorelease();
    return a;
}

//
// FlipX
//

FlipX *FlipX::create(bool x)
{
    FlipX *ret = new (std::nothrow) FlipX();

    if (ret && ret->initWithFlipX(x)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FlipX::initWithFlipX(bool x) {
    _flipX = x;
    return true;
}

void FlipX::update(float time) {
    CC_UNUSED_PARAM(time);
    static_cast<CGSprite*>(_target)->setFlipX(_flipX);
}

FlipX* FlipX::reverse() const
{
    return FlipX::create(!_flipX);
}

FlipX * FlipX::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) FlipX();
    a->initWithFlipX(_flipX);
    a->autorelease();
    return a;
}
//
// FlipY
//

FlipY * FlipY::create(bool y)
{
    FlipY *ret = new (std::nothrow) FlipY();

    if (ret && ret->initWithFlipY(y)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FlipY::initWithFlipY(bool y) {
    _flipY = y;
    return true;
}

void FlipY::update(float time) {
    CC_UNUSED_PARAM(time);
    static_cast<CGSprite*>(_target)->setFlipY(_flipY);
}

FlipY* FlipY::reverse() const
{
    return FlipY::create(!_flipY);
}

FlipY * FlipY::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) FlipY();
    a->initWithFlipY(_flipY);
    a->autorelease();
    return a;
}

//
// Place
//

Place* Place::create(const DPoint& pos)
{
    Place *ret = new (std::nothrow) Place();

    if (ret && ret->initWithPosition(pos)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Place::initWithPosition(const DPoint& pos) {
    _position = pos;
    return true;
}

Place * Place::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) Place();
    a->initWithPosition(_position);
    a->autorelease();
    return a;
}

Place * Place::reverse() const
{
    // no reverse, just clone
    auto a = new (std::nothrow) Place();
    a->initWithPosition(_position);
    a->autorelease();
    return a;
}

void Place::update(float time) {
    CC_UNUSED_PARAM(time);
    _target->setPosition(_position);
}

//
// CallFunc
//

CallFunc * CallFunc::create(const std::function<void()> &func)
{
    CallFunc *ret = new (std::nothrow) CallFunc();

    if (ret && ret->initWithFunction(func) ) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CallFunc::initWithFunction(const std::function<void()> &func)
{
    _function = func;
    return true;
}

CallFunc::~CallFunc()
{
    
}

CallFunc * CallFunc::clone() const
    {
    // no copy constructor
    auto a = new (std::nothrow) CallFunc();
    if( _function )
    {
        a->initWithFunction(_function);
    }

    a->autorelease();
    return a;
}

CallFunc * CallFunc::reverse() const
{
    // no reverse here, just return a clone
    // no copy constructor
    auto a = new (std::nothrow) CallFunc();
    if( _function )
    {
        a->initWithFunction(_function);
    }
    
    a->autorelease();
    return a;
}

void CallFunc::update(float time)
{
    CC_UNUSED_PARAM(time);
    this->execute();
}

void CallFunc::execute()
{
    if( _function )
    {
        _function();
    }
}

//
// CallFuncN
//

CallFuncN * CallFuncN::create(const std::function<void(CGSprite*)> &func)
{
    auto ret = new (std::nothrow) CallFuncN();

    if (ret && ret->initWithFunction(func) ) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

void CallFuncN::execute()
{
    if (_functionN)
    {
        _functionN(_target);
    }
}

bool CallFuncN::initWithFunction(const std::function<void(CGSprite *)> &func)
{
    _functionN = func;
    return true;
}

CallFuncN * CallFuncN::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) CallFuncN();

    if( _functionN )
    {
        a->initWithFunction(_functionN);
    }

    a->autorelease();
    return a;
}

NS_CC_END

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (pop)
#endif

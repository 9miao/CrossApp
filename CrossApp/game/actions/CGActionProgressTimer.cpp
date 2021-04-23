
#include "game/actions/CGActionProgressTimer.h"
#include "game/CGProgressTimer.h"

NS_CC_BEGIN

#define kProgressTimerCast CGProgressTimer*

// implementation of ProgressTo

ProgressTo* ProgressTo::create(float duration, float percent)
{
    ProgressTo *progressTo = new (std::nothrow) ProgressTo();
    progressTo->initWithDuration(duration, percent);
    progressTo->autorelease();

    return progressTo;
}

bool ProgressTo::initWithDuration(float duration, float percent)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _to = percent;

        return true;
    }

    return false;
}

ProgressTo* ProgressTo::clone() const
{
    // no copy constructor    
    auto a = new (std::nothrow) ProgressTo();
    a->initWithDuration(_duration, _to);
    a->autorelease();
    return a;
}

ProgressTo* ProgressTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in ProgressTo");
    return nullptr;
}

void ProgressTo::startWithTarget(CGSprite *target)
{
    ActionInterval::startWithTarget(target);
    _from = ((kProgressTimerCast)(target))->getPercentage();
}

void ProgressTo::update(float time)
{
    ((kProgressTimerCast)(_target))->setPercentage(_from + (_to - _from) * time);
}

// implementation of ProgressFromTo

ProgressFromTo* ProgressFromTo::create(float duration, float fromPercentage, float toPercentage)
{
    ProgressFromTo *progressFromTo = new (std::nothrow) ProgressFromTo();
    progressFromTo->initWithDuration(duration, fromPercentage, toPercentage);
    progressFromTo->autorelease();

    return progressFromTo;
}

bool ProgressFromTo::initWithDuration(float duration, float fromPercentage, float toPercentage)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _to = toPercentage;
        _from = fromPercentage;

        return true;
    }

    return false;
}

ProgressFromTo* ProgressFromTo::clone() const
{
    // no copy constructor    
    auto a = new (std::nothrow) ProgressFromTo();
    a->initWithDuration(_duration, _from, _to);
    a->autorelease();
    return a;
}


ProgressFromTo* ProgressFromTo::reverse() const
{
    return ProgressFromTo::create(_duration, _to, _from);
}

void ProgressFromTo::startWithTarget(CGSprite *target)
{
    ActionInterval::startWithTarget(target);
}

void ProgressFromTo::update(float time)
{
    ((kProgressTimerCast)(_target))->setPercentage(_from + (_to - _from) * time);
}

NS_CC_END

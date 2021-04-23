
#include "CGAction.h"
#include "game/actions/CGActionInterval.h"
#include "game/CGSprite.h"
#include "basics/CAApplication.h"
#include "ccTypeInfo.h"
#include "basics/CAPointExtension.h"

NS_CC_BEGIN
//
// Action Base Class
//

Action::Action()
:_originalTarget(nullptr)
,_target(nullptr)
,_tag(Action::INVALID_TAG)
,_flags(0)
{

}

Action::~Action()
{
    CCLOG("deallocing Action: %p - tag: %i", this, _tag);
}

std::string Action::description() const
{
    return crossapp_format_string("<Action | Tag = %d", _tag);
}

void Action::startWithTarget(CGSprite *aTarget)
{
    _originalTarget = _target = aTarget;
}

void Action::stop()
{
    _target = nullptr;
}

bool Action::isDone() const
{
    return true;
}

void Action::step(float dt)
{
    CC_UNUSED_PARAM(dt);
    CCLOG("[Action step]. override me");
}

void Action::update(float time)
{
    CC_UNUSED_PARAM(time);
    CCLOG("[Action update]. override me");
}

//
// Speed
//
Speed::Speed()
: _speed(0.0)
, _innerAction(nullptr)
{
}

Speed::~Speed()
{
    CC_SAFE_RELEASE(_innerAction);
}

Speed* Speed::create(ActionInterval* action, float speed)
{
    Speed *ret = new (std::nothrow) Speed();
    if (ret && ret->initWithAction(action, speed))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Speed::initWithAction(ActionInterval *action, float speed)
{
    CCASSERT(action != nullptr, "action must not be NULL");
    action->retain();
    _innerAction = action;
    _speed = speed;
    return true;
}

Speed *Speed::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) Speed();
    a->initWithAction(_innerAction->clone(), _speed);
    a->autorelease();
    return a;
}

void Speed::startWithTarget(CGSprite* target)
{
    Action::startWithTarget(target);
    _innerAction->startWithTarget(target);
}

void Speed::stop()
{
    _innerAction->stop();
    Action::stop();
}

void Speed::step(float dt)
{
    _innerAction->step(dt * _speed);
}

bool Speed::isDone() const
{
    return _innerAction->isDone();
}

Speed *Speed::reverse() const
{
    return Speed::create(_innerAction->reverse(), _speed);
}

void Speed::setInnerAction(ActionInterval *action)
{
    if (_innerAction != action)
    {
        CC_SAFE_RELEASE(_innerAction);
        _innerAction = action;
        CC_SAFE_RETAIN(_innerAction);
    }
}

//
// Follow
//
Follow::~Follow()
{
    CC_SAFE_RELEASE(_followedNode);
}

Follow* Follow::create(CGSprite *followedNode, const DRect& rect/* = DRectZero*/)
{
    Follow *follow = new (std::nothrow) Follow();
    if (follow && follow->initWithTarget(followedNode, rect))
    {
        follow->autorelease();
        return follow;
    }
    CC_SAFE_DELETE(follow);
    return nullptr;
}

Follow* Follow::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) Follow();
    a->initWithTarget(_followedNode, _worldRect);
    a->autorelease();
    return a;
}

Follow* Follow::reverse() const
{
    auto a = new (std::nothrow) Follow();
    a->initWithTarget(_followedNode, _worldRect);
    a->autorelease();
    return a;
}

bool Follow::initWithTarget(CGSprite *followedNode, const DRect& rect/* = DRectZero*/)
{
    CCASSERT(followedNode != nullptr, "FollowedCGSprite can't be NULL");
 
    followedNode->retain();
    _followedNode = followedNode;
    _worldRect = rect;
    _boundarySet = !rect.equals(DRectZero);
    _boundaryFullyCovered = false;

    DSize winSize = CAApplication::getApplication()->getWinSize();
    _fullScreenSize.setPoint(winSize.width, winSize.height);
    _halfScreenSize = _fullScreenSize * 0.5f;

    if (_boundarySet)
    {
        _leftBoundary = -((rect.origin.x+rect.size.width) - _fullScreenSize.x);
        _rightBoundary = -rect.origin.x ;
        _topBoundary = -rect.origin.y;
        _bottomBoundary = -((rect.origin.y+rect.size.height) - _fullScreenSize.y);

        if(_rightBoundary < _leftBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            _rightBoundary = _leftBoundary = (_leftBoundary + _rightBoundary) / 2;
        }
        if(_topBoundary < _bottomBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            _topBoundary = _bottomBoundary = (_topBoundary + _bottomBoundary) / 2;
        }

        if( (_topBoundary == _bottomBoundary) && (_leftBoundary == _rightBoundary) )
        {
            _boundaryFullyCovered = true;
        }
    }
    
    return true;
}

void Follow::step(float dt)
{
    CC_UNUSED_PARAM(dt);

    if(_boundarySet)
    {
        // whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
        if(_boundaryFullyCovered)
        {
            return;
        }

        DPoint tempPos = _halfScreenSize - _followedNode->getPosition();
        tempPos.x = clampf(tempPos.x, _leftBoundary, _rightBoundary);
        tempPos.y = clampf(tempPos.y, _bottomBoundary, _topBoundary);
        _target->setPosition(tempPos);
    }
    else
    {
        _target->setPosition(_halfScreenSize - _followedNode->getPosition());
    }
}

bool Follow::isDone() const
{
    return ( !_followedNode->isRunning() );
}

void Follow::stop()
{
    _target = nullptr;
    Action::stop();
}

NS_CC_END



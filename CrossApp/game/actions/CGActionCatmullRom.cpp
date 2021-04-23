
#include "ccMacros.h"
#include "game/actions/CGActionCatmullRom.h"
#include "game/CGSprite.h"
#include "view/CADrawingPrimitives.h"


NS_CC_BEGIN;

/*
 *  Implementation of PointArray
 */

PointArray* PointArray::create(ssize_t capacity)
{
    PointArray* pointArray = new (std::nothrow) PointArray();
    if (pointArray)
    {
        if (pointArray->initWithCapacity(capacity))
        {
            pointArray->autorelease();
        }
        else 
        {
            delete pointArray;
            pointArray = nullptr;
        }
    }

    return pointArray;
}


bool PointArray::initWithCapacity(ssize_t capacity)
{
    _controlPoints = new (std::nothrow) std::vector<DPoint*>();
    
    return true;
}

PointArray* PointArray::clone() const
{
    std::vector<DPoint*> *newArray = new std::vector<DPoint*>();
    std::vector<DPoint*>::iterator iter;
    for (iter = _controlPoints->begin(); iter != _controlPoints->end(); ++iter)
    {
        newArray->push_back(new DPoint((*iter)->x, (*iter)->y));
    }
    
    PointArray *points = new (std::nothrow) PointArray();
    points->initWithCapacity(10);
    points->setControlPoints(newArray);

    points->autorelease();
    return points;
}

PointArray::~PointArray()
{
    CCLOG("deallocing PointArray: %p", this);

    CAVector<DPoint*>::iterator iter;
    for (iter = _controlPoints->begin(); iter != _controlPoints->end(); ++iter)
    {
        delete *iter;
    }
    delete _controlPoints;
}

PointArray::PointArray() :_controlPoints(nullptr){}

const std::vector<DPoint*>* PointArray::getControlPoints() const
{
    return _controlPoints;
}

void PointArray::setControlPoints(std::vector<DPoint*> *controlPoints)
{
    CCASSERT(controlPoints != nullptr, "control points should not be nullptr");
    
    // delete old points
    CAVector<DPoint*>::iterator iter;
    for (iter = _controlPoints->begin(); iter != _controlPoints->end(); ++iter)
    {
        delete *iter;
    }
    delete _controlPoints;
    
    _controlPoints = controlPoints;
}

void PointArray::addControlPoint(DPoint controlPoint)
{    
    _controlPoints->push_back(new DPoint(controlPoint.x, controlPoint.y));
}

void PointArray::insertControlPoint(DPoint &controlPoint, ssize_t index)
{
    DPoint *temp = new (std::nothrow) DPoint(controlPoint.x, controlPoint.y);
    _controlPoints->insert(_controlPoints->begin() + index, temp);
}

DPoint PointArray::getControlPointAtIndex(ssize_t index)
{
    index = MIN(static_cast<ssize_t>(_controlPoints->size())-1, MAX(index, 0));
    return *(_controlPoints->at(index));
}

void PointArray::replaceControlPoint(CrossApp::DPoint &controlPoint, ssize_t index)
{
    DPoint *temp = _controlPoints->at(index);
    temp->x = controlPoint.x;
    temp->y = controlPoint.y;
}

void PointArray::removeControlPointAtIndex(ssize_t index)
{
    CAVector<DPoint*>::iterator iter = _controlPoints->begin() + index;
    DPoint* removeDPoint = *iter;
    _controlPoints->erase(iter);
    delete removeDPoint;
}

ssize_t PointArray::count() const
{
    return _controlPoints->size();
}

PointArray* PointArray::reverse() const
{
    std::vector<DPoint*> *newArray = new (std::nothrow) std::vector<DPoint*>();
    std::vector<DPoint*>::reverse_iterator iter;
    DPoint *point = nullptr;
    for (iter = _controlPoints->rbegin(); iter != _controlPoints->rend(); ++iter)
    {
        point = *iter;
        newArray->push_back(new DPoint(point->x, point->y));
    }
    PointArray *config = PointArray::create(0);
    config->setControlPoints(newArray);
    
    return config;
}

void PointArray::reverseInline()
{
    size_t l = _controlPoints->size();
    DPoint *p1 = nullptr;
    DPoint *p2 = nullptr;
    float x, y;
    for (size_t i = 0; i < l/2; ++i)
    {
        p1 = _controlPoints->at(i);
        p2 = _controlPoints->at(l-i-1);
        
        x = p1->x;
        y = p1->y;
        
        p1->x = p2->x;
        p1->y = p2->y;
        
        p2->x = x;
        p2->y = y;
    }
}

/* Implementation of CardinalSplineTo
 */

CardinalSplineTo* CardinalSplineTo::create(float duration, CrossApp::PointArray *points, float tension)
{
    CardinalSplineTo *ret = new (std::nothrow) CardinalSplineTo();
    if (ret)
    {
        if (ret->initWithDuration(duration, points, tension))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

bool CardinalSplineTo::initWithDuration(float duration, CrossApp::PointArray *points, float tension)
{
    CCASSERT(points->count() > 0, "Invalid configuration. It must at least have one control point");

    if (ActionInterval::initWithDuration(duration))
    {
        this->setPoints(points);
        this->_tension = tension;
        
        return true;
    }
    
    return false;
}

CardinalSplineTo::~CardinalSplineTo()
{
    CC_SAFE_RELEASE_NULL(_points);
}

CardinalSplineTo::CardinalSplineTo()
: _points(nullptr)
, _deltaT(0.f)
, _tension(0.f)
{
}

void CardinalSplineTo::startWithTarget(CrossApp::CGSprite *target)
{
    ActionInterval::startWithTarget(target);
	
//    _deltaT = (float) 1 / _points->count();
    
    // Issue #1441
    _deltaT = (float) 1 / (_points->count() - 1);

    _previousPosition = target->getPosition();
    _accumulatedDiff = DPointZero;
}

CardinalSplineTo* CardinalSplineTo::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) CardinalSplineTo();
    a->initWithDuration(this->_duration, this->_points->clone(), this->_tension);
    a->autorelease();
    return a;
}

void CardinalSplineTo::update(float time)
{
    ssize_t p;
    float lt;
	
    // eg.
    // p..p..p..p..p..p..p
    // 1..2..3..4..5..6..7
    // want p to be 1, 2, 3, 4, 5, 6
    if (time == 1)
    {
        p = _points->count() - 1;
        lt = 1;
    }
    else 
    {
        p = time / _deltaT;
        lt = (time - _deltaT * (float)p) / _deltaT;
    }
    
    // Interpolate
    DPoint pp0 = _points->getControlPointAtIndex(p-1);
    DPoint pp1 = _points->getControlPointAtIndex(p+0);
    DPoint pp2 = _points->getControlPointAtIndex(p+1);
    DPoint pp3 = _points->getControlPointAtIndex(p+2);
	
    DPoint newPos = ccCardinalSplineAt(pp0, pp1, pp2, pp3, _tension, lt);
	
#if CC_ENABLE_STACKABLE_ACTIONS
    // Support for stacked actions
    CGSprite *node = _target;
    DPoint diff = node->getPosition() - _previousPosition;
    if( diff.x !=0 || diff.y != 0 ) {
        _accumulatedDiff = _accumulatedDiff + diff;
        newPos = newPos + _accumulatedDiff;
    }
#endif
    
    this->updatePosition(newPos);
}

void CardinalSplineTo::updatePosition(CrossApp::DPoint &newPos)
{
    _target->setPosition(newPos);
    _previousPosition = newPos;
}

CardinalSplineTo* CardinalSplineTo::reverse() const
{
    PointArray *pReverse = _points->reverse();
    
    return CardinalSplineTo::create(_duration, pReverse, _tension);
}

/* CardinalSplineBy
 */

CardinalSplineBy* CardinalSplineBy::create(float duration, CrossApp::PointArray *points, float tension)
{
    CardinalSplineBy *ret = new (std::nothrow) CardinalSplineBy();
    if (ret)
    {
        if (ret->initWithDuration(duration, points, tension))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

CardinalSplineBy::CardinalSplineBy() : _startPosition(0,0)
{
}

void CardinalSplineBy::updatePosition(CrossApp::DPoint &newPos)
{
    DPoint p = newPos + _startPosition;
    _target->setPosition(p);
    _previousPosition = p;
}

CardinalSplineBy* CardinalSplineBy::reverse() const
{
    PointArray *copyConfig = _points->clone();
	
    //
    // convert "absolutes" to "diffs"
    //
    DPoint p = copyConfig->getControlPointAtIndex(0);
    for (ssize_t i = 1; i < copyConfig->count(); ++i)
    {
        DPoint current = copyConfig->getControlPointAtIndex(i);
        DPoint diff = current - p;
        copyConfig->replaceControlPoint(diff, i);
        
        p = current;
    }
	
    // convert to "diffs" to "reverse absolute"
	
    PointArray *pReverse = copyConfig->reverse();
	
    // 1st element (which should be 0,0) should be here too
    
    p = pReverse->getControlPointAtIndex(pReverse->count()-1);
    pReverse->removeControlPointAtIndex(pReverse->count()-1);
    
    p = -p;
    pReverse->insertControlPoint(p, 0);
    
    for (ssize_t i = 1; i < pReverse->count(); ++i)
    {
        DPoint current = pReverse->getControlPointAtIndex(i);
        current = -current;
        DPoint abs = current + p;
        pReverse->replaceControlPoint(abs, i);
        
        p = abs;
    }
	
    return CardinalSplineBy::create(_duration, pReverse, _tension);
}

void CardinalSplineBy::startWithTarget(CrossApp::CGSprite *target)
{    
    CardinalSplineTo::startWithTarget(target);
    _startPosition = target->getPosition();
}

CardinalSplineBy* CardinalSplineBy::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) CardinalSplineBy();
    a->initWithDuration(this->_duration, this->_points->clone(), this->_tension);
    a->autorelease();
    return a;
}

/* CatmullRomTo
 */

CatmullRomTo* CatmullRomTo::create(float dt, CrossApp::PointArray *points)
{
    CatmullRomTo *ret = new (std::nothrow) CatmullRomTo();
    if (ret)
    {
        if (ret->initWithDuration(dt, points))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

bool CatmullRomTo::initWithDuration(float dt, CrossApp::PointArray *points)
{
    if (CardinalSplineTo::initWithDuration(dt, points, 0.5f))
    {
        return true;
    }
    
    return false;
}

CatmullRomTo* CatmullRomTo::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) CatmullRomTo();
    a->initWithDuration(this->_duration, this->_points->clone());
    a->autorelease();
    return a;
}

CatmullRomTo* CatmullRomTo::reverse() const
{
    PointArray *reverse = _points->reverse();
    return CatmullRomTo::create(_duration, reverse);
}


/* CatmullRomBy
 */

CatmullRomBy* CatmullRomBy::create(float dt, CrossApp::PointArray *points)
{
    CatmullRomBy *ret = new (std::nothrow) CatmullRomBy();
    if (ret)
    {
        if (ret->initWithDuration(dt, points))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

bool CatmullRomBy::initWithDuration(float dt, CrossApp::PointArray *points)
{
    if (CardinalSplineTo::initWithDuration(dt, points, 0.5f))
    {
        return true;
    }
    
    return false;
}

CatmullRomBy* CatmullRomBy::clone() const
{
    // no copy constructor	
    auto a = new (std::nothrow) CatmullRomBy();
    a->initWithDuration(this->_duration, this->_points->clone());
    a->autorelease();
    return a;
}

CatmullRomBy* CatmullRomBy::reverse() const
{
    PointArray *copyConfig = _points->clone();

    //
    // convert "absolutes" to "diffs"
    //
    DPoint p = copyConfig->getControlPointAtIndex(0);
    for (ssize_t i = 1; i < copyConfig->count(); ++i)
    {
        DPoint current = copyConfig->getControlPointAtIndex(i);
        DPoint diff = current - p;
        copyConfig->replaceControlPoint(diff, i);

        p = current;
    }

	// convert to "diffs" to "reverse absolute"

    PointArray *reverse = copyConfig->reverse();

	// 1st element (which should be 0,0) should be here too

    p = reverse->getControlPointAtIndex(reverse->count()-1);
    reverse->removeControlPointAtIndex(reverse->count()-1);

    p = -p;
    reverse->insertControlPoint(p, 0);

    for (ssize_t i = 1; i < reverse->count(); ++i)
    {
        DPoint current = reverse->getControlPointAtIndex(i);
        current = -current;
        DPoint abs = current + p;
        reverse->replaceControlPoint(abs, i);

        p = abs;
    }

    return CatmullRomBy::create(_duration, reverse);
}

NS_CC_END;

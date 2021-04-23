

#include "game/actions/CGActionCamera.h"
#include "game/CGSprite.h"
#include "CCStdC.h"

NS_CC_BEGIN
//
// CameraAction
//
ActionCamera::ActionCamera()
: _center(0, 0, 0)
, _eye(0, 0, FLT_EPSILON)
, _up(0, 1, 0)
{
}
void ActionCamera::startWithTarget(CGSprite *target)
{
    ActionInterval::startWithTarget(target);
}

ActionCamera* ActionCamera::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) ActionCamera();
	a->autorelease();
	return a;
}

ActionCamera * ActionCamera::reverse() const
{
    // FIXME: This conversion isn't safe.
    return (ActionCamera*)ReverseTime::create(const_cast<ActionCamera*>(this));
}

void ActionCamera::restore()
{
    _center = DPoint3DZero;
    _eye.setPoint(0.0f, 0.0f, FLT_EPSILON);
    _up.setPoint(0.0f, 1.0f, 0.0f);
}

void ActionCamera::setEye(const DPoint3D& eye)
{
    _eye = eye;
    updateTransform();
}

void ActionCamera::setEye(float x, float y, float z)
{
    _eye.setPoint(x, y, z);
    updateTransform();
}

void ActionCamera::setCenter(const DPoint3D& center)
{
    _center = center;
    updateTransform();
}

void ActionCamera::setUp(const DPoint3D& up)
{
    _up = up;
    updateTransform();
}

void ActionCamera::updateTransform()
{
    Mat4 lookupMatrix;
    Mat4::createLookAt(_eye.x, _eye.y, _eye.z, _center.x, _center.y, _center.z, _up.x, _up.y, _up.z, &lookupMatrix);

    DPoint anchorPoint = _target->getAnchorPointInPoints();

    bool needsTranslation = !anchorPoint.equals(DPointZero);

    Mat4 mv = Mat4::IDENTITY;

    if(needsTranslation) {
        Mat4 t;
        Mat4::createTranslation(anchorPoint.x, anchorPoint.y, 0, &t);
        mv = mv * t;
    }
    
    mv = mv * lookupMatrix;

    if(needsTranslation) {
        
        Mat4 t;
        Mat4::createTranslation(-anchorPoint.x, -anchorPoint.y, 0, &t);
        mv = mv * t;
    }

    // FIXME: Using the AdditionalTransform is a complete hack.
    // This should be done by multiplying the lookup-Matrix with the Node's MV matrix
    // And then setting the result as the new MV matrix
    // But that operation needs to be done after all the 'updates'.
    // So the Director should emit an 'director_after_update' event.
    // And this object should listen to it
    _target->setAdditionalTransform(&mv);
}

//
// OrbitCamera
//

OrbitCamera::OrbitCamera()
: _radius(0.0)
, _deltaRadius(0.0)
, _angleZ(0.0)
, _deltaAngleZ(0.0)
, _angleX(0.0)
, _deltaAngleX(0.0)
, _radZ(0.0)
, _radDeltaZ(0.0)
, _radX(0.0)
, _radDeltaX(0.0)
{
}
OrbitCamera::~OrbitCamera()
{
}

OrbitCamera * OrbitCamera::create(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX)
{
    OrbitCamera * obitCamera = new (std::nothrow) OrbitCamera();
    if(obitCamera->initWithDuration(t, radius, deltaRadius, angleZ, deltaAngleZ, angleX, deltaAngleX))
    {
        obitCamera->autorelease();
        return obitCamera;
    }
    CC_SAFE_DELETE(obitCamera);
    return nullptr;
}

OrbitCamera* OrbitCamera::clone() const
{
    // no copy constructor	
    auto a = new (std::nothrow) OrbitCamera();
    a->initWithDuration(_duration, _radius, _deltaRadius, _angleZ, _deltaAngleZ, _angleX, _deltaAngleX);
    a->autorelease();
    return a;
}

bool OrbitCamera::initWithDuration(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX)
{
    if ( ActionInterval::initWithDuration(t) )
    {
        _radius = radius;
        _deltaRadius = deltaRadius;
        _angleZ = angleZ;
        _deltaAngleZ = deltaAngleZ;
        _angleX = angleX;
        _deltaAngleX = deltaAngleX;

        _radDeltaZ = (float)CC_DEGREES_TO_RADIANS(deltaAngleZ);
        _radDeltaX = (float)CC_DEGREES_TO_RADIANS(deltaAngleX);
        return true;
    }
    return false;
}

void OrbitCamera::startWithTarget(CGSprite *target)
{
    ActionCamera::startWithTarget(target);

    float r, zenith, azimuth;
    this->sphericalRadius(&r, &zenith, &azimuth);
    if( isnan(_radius) )
        _radius = r;
    if( isnan(_angleZ) )
        _angleZ = (float)CC_RADIANS_TO_DEGREES(zenith);
    if( isnan(_angleX) )
        _angleX = (float)CC_RADIANS_TO_DEGREES(azimuth);

    _radZ = (float)CC_DEGREES_TO_RADIANS(_angleZ);
    _radX = (float)CC_DEGREES_TO_RADIANS(_angleX);
}

void OrbitCamera::update(float dt)
{
    float r = (_radius + _deltaRadius * dt) * FLT_EPSILON;
    float za = _radZ + _radDeltaZ * dt;
    float xa = _radX + _radDeltaX * dt;

    float i = sinf(za) * cosf(xa) * r + _center.x;
    float j = sinf(za) * sinf(xa) * r + _center.y;
    float k = cosf(za) * r + _center.z;

    setEye(i,j,k);
}

void OrbitCamera::sphericalRadius(float *newRadius, float *zenith, float *azimuth)
{
    float r; // radius
    float s;

    float x = _eye.x - _center.x;
    float y = _eye.y - _center.y;
    float z = _eye.z - _center.z;

    r = sqrtf( powf(x,2) + powf(y,2) + powf(z,2));
    s = sqrtf( powf(x,2) + powf(y,2));
    if( s == 0.0f )
        s = FLT_EPSILON;
    if(r==0.0f)
        r = FLT_EPSILON;

    *zenith = acosf( z/r);
    if( x < 0 )
        *azimuth= (float)M_PI - asinf(y/s);
    else
        *azimuth = asinf(y/s);

    *newRadius = r / FLT_EPSILON;
}

NS_CC_END

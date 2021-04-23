

#ifndef __CCCAMERA_ACTION_H__
#define __CCCAMERA_ACTION_H__

#include "game/actions/CGActionInterval.h"
#include "math/CAMath.h"

NS_CC_BEGIN

class Camera;

/**
 * @addtogroup actions
 * @{
 */

/**
 *@brief Base class for Camera actions.
 *@ingroup Actions
 */
class CC_DLL ActionCamera : public ActionInterval //<NSCopying> 
{
public:
    /**
     * @js ctor
     * @lua new
     */
    ActionCamera();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~ActionCamera(){};

    // Overrides
    virtual void startWithTarget(CGSprite *target) override;
    virtual ActionCamera * reverse() const override;
	virtual ActionCamera *clone() const override;

    /* Sets the Eye value of the Camera. 
     * 
     * @param eye The Eye value of the Camera.
     * @js NA
     */
    void setEye(const DPoint3D &eye);
    void setEye(float x, float y, float z);
    /* Returns the Eye value of the Camera. 
     *
     * @return The Eye value of the Camera.
     * @js NA
     */
    const DPoint3D& getEye() const { return _eye; }
    /* Sets the Center value of the Camera. 
     *
     * @param center The Center value of the Camera.
     * @js NA
     */
    void setCenter(const DPoint3D &center);
    /* Returns the Center value of the Camera. 
     *
     * @return The Center value of the Camera.
     * @js NA
     */
    const DPoint3D& getCenter() const { return _center; }
    /* Sets the Up value of the Camera. 
     *
     * @param up The Up value of the Camera.
     * @js NA
     */
    void setUp(const DPoint3D &up);
    /* Returns the Up value of the Camera. 
     *
     * @return The Up value of the Camera.
     * @js NA
     */
    const DPoint3D& getUp() const { return _up; }

protected:

    void restore();
    void updateTransform();

    DPoint3D _center;
    DPoint3D _eye;
    DPoint3D _up;
};

/** @class OrbitCamera
 *
 * @brief OrbitCamera action.
 * Orbits the camera around the center of the screen using spherical coordinates.
 * @ingroup Actions
 */
class CC_DLL OrbitCamera : public ActionCamera //<NSCopying> 
{
public:
    /** Creates a OrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX. 
     *
     * @param t Duration in seconds.
     * @param radius The start radius.
     * @param deltaRadius The delta radius.
     * @param angleZ The start angle in Z.
     * @param deltaAngleZ The delta angle in Z.
     * @param angleX The start angle in X.
     * @param deltaAngleX The delta angle in X.
     * @return An OrbitCamera.
     */
    static OrbitCamera* create(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX);
    
    /** Positions the camera according to spherical coordinates. 
     *
     * @param r The spherical radius.
     * @param zenith The spherical zenith.
     * @param azimuth The spherical azimuth.
     */
    void sphericalRadius(float *r, float *zenith, float *azimuth);

    // Overrides
	OrbitCamera *clone() const override;
    virtual void startWithTarget(CGSprite *target) override;
    virtual void update(float time) override;
    
public:
    /**
     * @js ctor
     */
    OrbitCamera();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~OrbitCamera();
    
    /** Initializes a OrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX. */
    bool initWithDuration(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX);

protected:
    float _radius;
    float _deltaRadius;
    float _angleZ;
    float _deltaAngleZ;
    float _angleX;
    float _deltaAngleX;

    float _radZ;
    float _radDeltaZ;
    float _radX;
    float _radDeltaX;
};

// end of actions group
/// @}

NS_CC_END

#endif //__CCCAMERA_ACTION_H__

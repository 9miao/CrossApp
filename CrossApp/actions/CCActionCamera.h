

#ifndef __CCCAMERA_ACTION_H__
#define __CCCAMERA_ACTION_H__

#include "CCActionInterval.h"

NS_CC_BEGIN

class CACamera;

/**
 * @addtogroup actions
 * @{
 */

/** 
@brief Base class for CACamera actions
@ingroup Actions
*/
class CC_DLL CCActionCamera : public CCActionInterval //<NSCopying> 
{
public:
    /**
     *  @js ctor
     */
    CCActionCamera()
        :m_fCenterXOrig(0)
        ,m_fCenterYOrig(0)
        ,m_fCenterZOrig(0)
        ,m_fEyeXOrig(0)
        ,m_fEyeYOrig(0)
        ,m_fEyeZOrig(0)
        ,m_fUpXOrig(0)
        ,m_fUpYOrig(0)
        ,m_fUpZOrig(0)
    {}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCActionCamera(){}
    // super methods
    virtual void startWithTarget(CAView *pTarget);
    virtual CCActionInterval * reverse();
protected:
    float m_fCenterXOrig;
    float m_fCenterYOrig;
    float m_fCenterZOrig;

    float m_fEyeXOrig;
    float m_fEyeYOrig;
    float m_fEyeZOrig;

    float m_fUpXOrig;
    float m_fUpYOrig;
    float m_fUpZOrig;
};

/** 
@brief CCOrbitCamera action
Orbits the camera around the center of the screen using spherical coordinates
@ingroup Actions
*/
class CC_DLL CCOrbitCamera : public CCActionCamera //<NSCopying> 
{
public:
    /**
     *  @js ctor
     */
    CCOrbitCamera()
        : m_fRadius(0.0)
        , m_fDeltaRadius(0.0)
        , m_fAngleZ(0.0)
        , m_fDeltaAngleZ(0.0)
        , m_fAngleX(0.0)            
        , m_fDeltaAngleX(0.0)
        , m_fRadZ(0.0)
        , m_fRadDeltaZ(0.0)
        , m_fRadX(0.0)                        
        , m_fRadDeltaX(0.0)        
    {}
    /**
     *  @js NA
     *  @lua NA
     */
    ~CCOrbitCamera(){}
    
    /** creates a CCOrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX */
    static CCOrbitCamera* create(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX);
    
    /** initializes a CCOrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX */
    bool initWithDuration(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX);
    /** positions the camera according to spherical coordinates */
    void sphericalRadius(float *r, float *zenith, float *azimuth);
    // super methods
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone* pZone);
    virtual void startWithTarget(CAView *pTarget);
    virtual void update(float time);

protected:
    float m_fRadius;
    float m_fDeltaRadius;
    float m_fAngleZ;
    float m_fDeltaAngleZ;
    float m_fAngleX;
    float m_fDeltaAngleX;

    float m_fRadZ;
    float m_fRadDeltaZ;
    float m_fRadX;
    float m_fRadDeltaX;
};

// end of actions group
/// @}

NS_CC_END

#endif //__CCCAMERA_ACTION_H__
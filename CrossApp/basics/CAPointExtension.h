
#ifndef __SUPPORT_CGPOINTEXTENSION_H__
#define __SUPPORT_CGPOINTEXTENSION_H__

#include "basics/CAPoint.h"
#include "basics/CASize.h"
#include "basics/CARect.h"
#include <math.h>

NS_CC_BEGIN

/** Returns opposite of point.
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpNeg(const DPoint& v)
{
    return -v;
}

/** Calculates sum of two points.
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpAdd(const DPoint& v1, const DPoint& v2)
{
    return v1 + v2;
}

/** Calculates difference of two points.
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpSub(const DPoint& v1, const DPoint& v2)
{
    return v1 - v2;
}

/** Returns point multiplied by given factor.
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpMult(const DPoint& v, const float s)
{
    return v * s;
}

/** Calculates midpoint between two points.
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpMidpoint(const DPoint& v1, const DPoint& v2)
{
    return (v1 + v2) / 2.f;
}

/** Calculates dot product of two points.
 @return float
 @since v0.7.2
 */
static inline float
ccpDot(const DPoint& v1, const DPoint& v2)
{
    return v1.dot(v2);
}

/** Calculates cross product of two points.
 @return float
 @since v0.7.2
 */
static inline float
ccpCross(const DPoint& v1, const DPoint& v2)
{
    return v1.cross(v2);
}

/** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpPerp(const DPoint& v)
{
    return v.getPerp();
}

/** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpRPerp(const DPoint& v)
{
    return v.getRPerp();
}

/** Calculates the projection of v1 over v2.
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpProject(const DPoint& v1, const DPoint& v2)
{
    return v1.project(v2);
}

/** Rotates two points.
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpRotate(const DPoint& v1, const DPoint& v2)
{
    return v1.rotate(v2);
}

/** Unrotates two points.
 @return DPoint
 @since v0.7.2
 */
static inline DPoint
ccpUnrotate(const DPoint& v1, const DPoint& v2)
{
    return v1.unrotate(v2);
}

/** Calculates the square length of a DPoint (not calling sqrt() )
 @return float
 @since v0.7.2
 */
static inline float
ccpLengthSQ(const DPoint& v)
{
    return v.getLengthSq();
}


/** Calculates the square distance between two points (not calling sqrt() )
 @return float
 @since v1.1
*/
static inline float
ccpDistanceSQ(const DPoint p1, const DPoint p2)
{
    return (p1 - p2).getLengthSq();
}


/** Calculates distance between point an origin
 @return float
 @since v0.7.2
 */
float CC_DLL ccpLength(const DPoint& v);

/** Calculates the distance between two points
 @return float
 @since v0.7.2
 */
float CC_DLL ccpDistance(const DPoint& v1, const DPoint& v2);

/** Returns point multiplied to a length of 1.
 @return DPoint
 @since v0.7.2
 */
DPoint CC_DLL ccpNormalize(const DPoint& v);

/** Converts radians to a normalized vector.
 @return DPoint
 @since v0.7.2
 */
DPoint CC_DLL ccpForAngle(const float a);

/** Converts a vector to radians.
 @return float
 @since v0.7.2
 */
float CC_DLL ccpToAngle(const DPoint& v);


/** Clamp a value between from and to.
 @since v0.99.1
 */
float CC_DLL clampf(float value, float min_inclusive, float max_inclusive);

/** Clamp a point between from and to.
 @since v0.99.1
 */
DPoint CC_DLL ccpClamp(const DPoint& p, const DPoint& from, const DPoint& to);

/** Quickly convert DSize to a DPoint
 @since v0.99.1
 */
DPoint CC_DLL ccpFromSize(const DSize& s);

/** Run a math operation function on each point component
 * absf, fllorf, ceilf, roundf
 * any function that has the signature: float func(float);
 * For example: let's try to take the floor of x,y
 * ccpCompOp(p,floorf);
 @since v0.99.1
 */
DPoint CC_DLL ccpCompOp(const DPoint& p, float (*opFunc)(float));

/** Linear Interpolation between two points a and b
 @returns
    alpha == 0 ? a
    alpha == 1 ? b
    otherwise a value between a..b
 @since v0.99.1
 */
DPoint CC_DLL ccpLerp(const DPoint& a, const DPoint& b, float alpha);


/** @returns if points have fuzzy equality which means equal with some degree of variance.
 @since v0.99.1
 */
bool CC_DLL ccpFuzzyEqual(const DPoint& a, const DPoint& b, float variance);


/** Multiplies a and b components, a.x*b.x, a.y*b.y
 @returns a component-wise multiplication
 @since v0.99.1
 */
DPoint CC_DLL ccpCompMult(const DPoint& a, const DPoint& b);

/** @returns the signed angle in radians between two vector directions
 @since v0.99.1
 */
float CC_DLL ccpAngleSigned(const DPoint& a, const DPoint& b);

/** @returns the angle in radians between two vector directions
 @since v0.99.1
*/
float CC_DLL ccpAngle(const DPoint& a, const DPoint& b);

/** Rotates a point counter clockwise by the angle around a pivot
 @param v is the point to rotate
 @param pivot is the pivot, naturally
 @param angle is the angle of rotation cw in radians
 @returns the rotated point
 @since v0.99.1
 */
DPoint CC_DLL ccpRotateByAngle(const DPoint& v, const DPoint& pivot, float angle);

/** A general line-line intersection test
 @param p1 
    is the startpoint for the first line P1 = (p1 - p2)
 @param p2 
    is the endpoint for the first line P1 = (p1 - p2)
 @param p3 
    is the startpoint for the second line P2 = (p3 - p4)
 @param p4 
    is the endpoint for the second line P2 = (p3 - p4)
 @param s 
    is the range for a hitpoint in P1 (pa = p1 + s*(p2 - p1))
 @param t
    is the range for a hitpoint in P3 (pa = p2 + t*(p4 - p3))
 @return bool 
    indicating successful intersection of a line
    note that to truly test intersection for segments we have to make 
    sure that s & t lie within [0..1] and for rays, make sure s & t > 0
    the hit point is        p3 + t * (p4 - p3);
    the hit point also is    p1 + s * (p2 - p1);
 @since v0.99.1
 */
bool CC_DLL ccpLineIntersect(const DPoint& p1, const DPoint& p2, 
                      const DPoint& p3, const DPoint& p4,
                      float *s, float *t);

/*
ccpSegmentIntersect returns YES if Segment A-B intersects with segment C-D
@since v1.0.0
*/
bool CC_DLL ccpSegmentIntersect(const DPoint& A, const DPoint& B, const DPoint& C, const DPoint& D);

/*
ccpIntersectPoint returns the intersection point of line A-B, C-D
@since v1.0.0
*/
DPoint CC_DLL ccpIntersectPoint(const DPoint& A, const DPoint& B, const DPoint& C, const DPoint& D);

// end of data_structures group
/// @}

NS_CC_END

#endif // __SUPPORT_CGPOINTEXTENSION_H__


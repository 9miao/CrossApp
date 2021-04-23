

#ifndef __MATH_CCAFFINETRANSFORM_H__
#define __MATH_CCAFFINETRANSFORM_H__

#include "platform/CCPlatformMacros.h"
#include "basics/CAPoint.h"
#include "basics/CASize.h"
#include "basics/CARect.h"
#include "math/CAMath.h"

/**
 * @addtogroup base
 * @{
 */

NS_CC_BEGIN

/**@{
 Affine transform
 a   b    0
 c   d    0
 tx  ty   1
 
 Identity
 1   0    0
 0   1    0
 0   0    1
 */
struct CC_DLL AffineTransform {
    float a, b, c, d;
    float tx, ty;

    static const AffineTransform IDENTITY;
};

/**@}*/

/**Make affine transform.*/
CC_DLL AffineTransform __CCAffineTransformMake(float a, float b, float c, float d, float tx, float ty);
#define AffineTransformMake __CCAffineTransformMake

/**Multiply point (x,y,1) by a  affine transform.*/
CC_DLL DPoint __CCPointApplyAffineTransform(const DPoint& point, const AffineTransform& t);
#define PointApplyAffineTransform __CCPointApplyAffineTransform

/**Multiply size (width,height,0) by a  affine transform.*/
CC_DLL DSize __CCSizeApplyAffineTransform(const DSize& size, const AffineTransform& t);
#define SizeApplyAffineTransform __CCSizeApplyAffineTransform
/**Make identity affine transform.*/
CC_DLL AffineTransform AffineTransformMakeIdentity();
/**Transform Rect, which will transform the four vertices of the point.*/
CC_DLL DRect RectApplyAffineTransform(const DRect& rect, const AffineTransform& anAffineTransform);
/**@{
 Transform vec2 and Rect by Mat4.
 */
CC_DLL DRect RectApplyTransform(const DRect& rect, const Mat4& transform);
CC_DLL DPoint PointApplyTransform(const DPoint& point, const Mat4& transform);
/**@}*/
/**
 Translation, equals
 1  0  1
 0  1  0   * affine transform
 tx ty 1
 */
CC_DLL AffineTransform AffineTransformTranslate(const AffineTransform& t, float tx, float ty);
/**
 Rotation, equals
 cos(angle)   sin(angle)   0
 -sin(angle)  cos(angle)   0  * AffineTransform
 0            0            1
 */
CC_DLL AffineTransform AffineTransformRotate(const AffineTransform& aTransform, float anAngle);
/**
 Scale, equals
 sx   0   0
 0    sy  0  * affineTransform
 0    0   1
 */
CC_DLL AffineTransform AffineTransformScale(const AffineTransform& t, float sx, float sy);
/**Concat two affine transform, t1 * t2*/
CC_DLL AffineTransform AffineTransformConcat(const AffineTransform& t1, const AffineTransform& t2);
/**Compare affine transform.*/
CC_DLL bool AffineTransformEqualToTransform(const AffineTransform& t1, const AffineTransform& t2);
/**Get the inverse of affine transform.*/
CC_DLL AffineTransform AffineTransformInvert(const AffineTransform& t);
/**Concat Mat4, return t1 * t2.*/
CC_DLL Mat4 TransformConcat(const Mat4& t1, const Mat4& t2);

extern CC_DLL const AffineTransform AffineTransformIdentity;

NS_CC_END

// end of base transform
/// @}

#endif // __MATH_CCAFFINETRANSFORM_H__

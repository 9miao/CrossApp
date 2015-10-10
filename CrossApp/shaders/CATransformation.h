

#ifndef __COCOA_CGAFFINETRANSFORM_H__
#define __COCOA_CGAFFINETRANSFORM_H__

#include "basics/CAGeometry.h"
#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN

struct CATransformation {
  double a, b, c, d;
  double tx, ty;
};

CC_DLL CATransformation __CATransformationMake(double a, double b, double c, double d, double tx, double ty);
#define CATransformationMake __CATransformationMake

CC_DLL DPoint __DPointApplyAffineTransform(const DPoint& point, const CATransformation& t);
#define DPointApplyAffineTransform __DPointApplyAffineTransform

CC_DLL DSize __DSizeApplyAffineTransform(const DSize& size, const CATransformation& t);
#define DSizeApplyAffineTransform __DSizeApplyAffineTransform

CC_DLL CATransformation CATransformationMakeIdentity();
CC_DLL DRect DRectApplyAffineTransform(const DRect& rect, const CATransformation& anAffineTransform);

CC_DLL CATransformation CATransformationTranslate(const CATransformation& t, float tx, float ty);
CC_DLL CATransformation CATransformationRotate(const CATransformation& aTransform, float anAngle);
CC_DLL CATransformation CATransformationScale(const CATransformation& t, float sx, float sy);
CC_DLL CATransformation CATransformationConcat(const CATransformation& t1, const CATransformation& t2);
CC_DLL bool CATransformationEqualToTransform(const CATransformation& t1, const CATransformation& t2);
CC_DLL CATransformation CATransformationInvert(const CATransformation& t);

extern CC_DLL const CATransformation CATransformationIdentity;

NS_CC_END

#endif // __COCOA_CGAFFINETRANSFORM_H__

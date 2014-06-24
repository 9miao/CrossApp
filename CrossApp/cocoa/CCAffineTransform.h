

#ifndef __COCOA_CGAFFINETRANSFORM_H__
#define __COCOA_CGAFFINETRANSFORM_H__

#include "basics/CAGeometry.h"
#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN

struct CCAffineTransform {
  float a, b, c, d;
  float tx, ty;
};

CC_DLL CCAffineTransform __CCAffineTransformMake(float a, float b, float c, float d, float tx, float ty);
#define CCAffineTransformMake __CCAffineTransformMake

CC_DLL CCPoint __CCPointApplyAffineTransform(const CCPoint& point, const CCAffineTransform& t);
#define CCPointApplyAffineTransform __CCPointApplyAffineTransform

CC_DLL CCSize __CCSizeApplyAffineTransform(const CCSize& size, const CCAffineTransform& t);
#define CCSizeApplyAffineTransform __CCSizeApplyAffineTransform

CC_DLL CCAffineTransform CCAffineTransformMakeIdentity();
CC_DLL CCRect CCRectApplyAffineTransform(const CCRect& rect, const CCAffineTransform& anAffineTransform);

CC_DLL CCAffineTransform CCAffineTransformTranslate(const CCAffineTransform& t, float tx, float ty);
CC_DLL CCAffineTransform CCAffineTransformRotate(const CCAffineTransform& aTransform, float anAngle);
CC_DLL CCAffineTransform CCAffineTransformScale(const CCAffineTransform& t, float sx, float sy);
CC_DLL CCAffineTransform CCAffineTransformConcat(const CCAffineTransform& t1, const CCAffineTransform& t2);
CC_DLL bool CCAffineTransformEqualToTransform(const CCAffineTransform& t1, const CCAffineTransform& t2);
CC_DLL CCAffineTransform CCAffineTransformInvert(const CCAffineTransform& t);

extern CC_DLL const CCAffineTransform CCAffineTransformIdentity;

NS_CC_END

#endif // __COCOA_CGAFFINETRANSFORM_H__

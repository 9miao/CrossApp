
#include "math/CAAffineTransform.h"

#include <algorithm>
#include <math.h>

using namespace std;


NS_CC_BEGIN

AffineTransform __CCAffineTransformMake(float a, float b, float c, float d, float tx, float ty)
{
  AffineTransform t;
  t.a = a; t.b = b; t.c = c; t.d = d; t.tx = tx; t.ty = ty;
  return t;
}

DPoint __CCPointApplyAffineTransform(const DPoint& point, const AffineTransform& t)
{
  DPoint p;
  p.x = (float)((double)t.a * point.x + (double)t.c * point.y + t.tx);
  p.y = (float)((double)t.b * point.x + (double)t.d * point.y + t.ty);
  return p;
}

DPoint PointApplyTransform(const DPoint& point, const Mat4& transform)
{
    DPoint3D vec(point.x, point.y, 0);
    transform.transformPoint(&vec);
    return DPoint(vec.x, vec.y);
}


DSize __CCSizeApplyAffineTransform(const DSize& size, const AffineTransform& t)
{
  DSize s;
  s.width = (float)((double)t.a * size.width + (double)t.c * size.height);
  s.height = (float)((double)t.b * size.width + (double)t.d * size.height);
  return s;
}


AffineTransform AffineTransformMakeIdentity()
{
    return __CCAffineTransformMake(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

extern const AffineTransform AffineTransformIdentity = AffineTransformMakeIdentity();
const AffineTransform AffineTransform::IDENTITY = AffineTransformMakeIdentity();

DRect RectApplyAffineTransform(const DRect& rect, const AffineTransform& anAffineTransform)
{
    float top    = rect.getMinY();
    float left   = rect.getMinX();
    float right  = rect.getMaxX();
    float bottom = rect.getMaxY();
    
    DPoint topLeft = PointApplyAffineTransform(DPoint(left, top), anAffineTransform);
    DPoint topRight = PointApplyAffineTransform(DPoint(right, top), anAffineTransform);
    DPoint bottomLeft = PointApplyAffineTransform(DPoint(left, bottom), anAffineTransform);
    DPoint bottomRight = PointApplyAffineTransform(DPoint(right, bottom), anAffineTransform);

    float minX = min(min(topLeft.x, topRight.x), min(bottomLeft.x, bottomRight.x));
    float maxX = max(max(topLeft.x, topRight.x), max(bottomLeft.x, bottomRight.x));
    float minY = min(min(topLeft.y, topRight.y), min(bottomLeft.y, bottomRight.y));
    float maxY = max(max(topLeft.y, topRight.y), max(bottomLeft.y, bottomRight.y));
        
    return DRect(minX, minY, (maxX - minX), (maxY - minY));
}

DRect RectApplyTransform(const DRect& rect, const Mat4& transform)
{
    float top    = rect.getMinY();
    float left   = rect.getMinX();
    float right  = rect.getMaxX();
    float bottom = rect.getMaxY();
    
    DPoint3D topLeft(left, top, 0);
    DPoint3D topRight(right, top, 0);
    DPoint3D bottomLeft(left, bottom, 0);
    DPoint3D bottomRight(right, bottom, 0);
    transform.transformPoint(&topLeft);
    transform.transformPoint(&topRight);
    transform.transformPoint(&bottomLeft);
    transform.transformPoint(&bottomRight);

    float minX = min(min(topLeft.x, topRight.x), min(bottomLeft.x, bottomRight.x));
    float maxX = max(max(topLeft.x, topRight.x), max(bottomLeft.x, bottomRight.x));
    float minY = min(min(topLeft.y, topRight.y), min(bottomLeft.y, bottomRight.y));
    float maxY = max(max(topLeft.y, topRight.y), max(bottomLeft.y, bottomRight.y));

    return DRect(minX, minY, (maxX - minX), (maxY - minY));
}


AffineTransform AffineTransformTranslate(const AffineTransform& t, float tx, float ty)
{
    return __CCAffineTransformMake(t.a, t.b, t.c, t.d, t.tx + t.a * tx + t.c * ty, t.ty + t.b * tx + t.d * ty);
}

AffineTransform AffineTransformScale(const AffineTransform& t, float sx, float sy)
{
    return __CCAffineTransformMake(t.a * sx, t.b * sx, t.c * sy, t.d * sy, t.tx, t.ty);
}

AffineTransform AffineTransformRotate(const AffineTransform& t, float anAngle)
{
    float sine = sinf(anAngle);
    float cosine = cosf(anAngle);

    return __CCAffineTransformMake(    t.a * cosine + t.c * sine,
                                    t.b * cosine + t.d * sine,
                                    t.c * cosine - t.a * sine,
                                    t.d * cosine - t.b * sine,
                                    t.tx,
                                    t.ty);
}

/* Concatenate `t2' to `t1' and return the result:
     t' = t1 * t2 */
AffineTransform AffineTransformConcat(const AffineTransform& t1, const AffineTransform& t2)
{
    return __CCAffineTransformMake(    t1.a * t2.a + t1.b * t2.c, t1.a * t2.b + t1.b * t2.d, //a,b
                                    t1.c * t2.a + t1.d * t2.c, t1.c * t2.b + t1.d * t2.d, //c,d
                                    t1.tx * t2.a + t1.ty * t2.c + t2.tx,                  //tx
                                    t1.tx * t2.b + t1.ty * t2.d + t2.ty);                  //ty
}

Mat4 TransformConcat(const Mat4& t1, const Mat4& t2)
{
    return t1 * t2;
}


/* Return true if `t1' and `t2' are equal, false otherwise. */
bool AffineTransformEqualToTransform(const AffineTransform& t1, const AffineTransform& t2)
{
    return (t1.a == t2.a && t1.b == t2.b && t1.c == t2.c && t1.d == t2.d && t1.tx == t2.tx && t1.ty == t2.ty);
}

AffineTransform AffineTransformInvert(const AffineTransform& t)
{
    float determinant = 1 / (t.a * t.d - t.b * t.c);

    return __CCAffineTransformMake(determinant * t.d, -determinant * t.b, -determinant * t.c, determinant * t.a,
                            determinant * (t.c * t.ty - t.d * t.tx), determinant * (t.b * t.tx - t.a * t.ty) );
}

NS_CC_END

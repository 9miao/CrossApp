

#include "CAPointExtension.h"
#include "ccMacros.h" // FLT_EPSILON
#include <stdio.h>

NS_CC_BEGIN

#define kDPointEpsilon FLT_EPSILON

float
ccpLength(const DPoint& v)
{
    return v.getLength();
}

float
ccpDistance(const DPoint& v1, const DPoint& v2)
{
    return (v1 - v2).getLength();
}

DPoint
ccpNormalize(const DPoint& v)
{
    return v.normalize();
}

DPoint
ccpForAngle(const float a)
{
    return DPoint::forAngle(a);
}

float
ccpToAngle(const DPoint& v)
{
    return v.getAngle();
}

DPoint ccpLerp(const DPoint& a, const DPoint& b, float alpha)
{
    return a.lerp(b, alpha);
}

float clampf(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive) {
        CC_SWAP(min_inclusive, max_inclusive, float);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}

DPoint ccpClamp(const DPoint& p, const DPoint& min_inclusive, const DPoint& max_inclusive)
{
    return DPoint(clampf(p.x,min_inclusive.x,max_inclusive.x), clampf(p.y, min_inclusive.y, max_inclusive.y));
}

DPoint ccpFromSize(const DSize& s)
{
    return DPoint(s);
}

DPoint ccpCompOp(const DPoint& p, float (*opFunc)(float))
{
    return DPoint(opFunc(p.x), opFunc(p.y));
}

bool ccpFuzzyEqual(const DPoint& a, const DPoint& b, float var)
{
	return a.fuzzyEquals(b, var);
}

DPoint ccpCompMult(const DPoint& a, const DPoint& b)
{
    return DPoint(a.x * b.x, a.y * b.y);
}

float ccpAngleSigned(const DPoint& a, const DPoint& b)
{
	return a.getAngle(b);
}

DPoint ccpRotateByAngle(const DPoint& v, const DPoint& pivot, float angle)
{
	return v.rotateByAngle(pivot, angle);
}


bool ccpSegmentIntersect(const DPoint& A, const DPoint& B, const DPoint& C, const DPoint& D)
{
    float S, T;

    if( ccpLineIntersect(A, B, C, D, &S, &T )
        && (S >= 0.0f && S <= 1.0f && T >= 0.0f && T <= 1.0f) )
        return true;

    return false;
}

DPoint ccpIntersectPoint(const DPoint& A, const DPoint& B, const DPoint& C, const DPoint& D)
{
    float S, T;

    if( ccpLineIntersect(A, B, C, D, &S, &T) )
    {
        // Point of intersection
        DPoint P;
        P.x = A.x + S * (B.x - A.x);
        P.y = A.y + S * (B.y - A.y);
        return P;
    }

    return DPointZero;
}

bool ccpLineIntersect(const DPoint& A, const DPoint& B, 
                      const DPoint& C, const DPoint& D,
                      float *S, float *T)
{
    // FAIL: Line undefined
    if ( (A.x==B.x && A.y==B.y) || (C.x==D.x && C.y==D.y) )
    {
        return false;
    }
    const float BAx = B.x - A.x;
    const float BAy = B.y - A.y;
    const float DCx = D.x - C.x;
    const float DCy = D.y - C.y;
    const float ACx = A.x - C.x;
    const float ACy = A.y - C.y;

    const float denom = DCy*BAx - DCx*BAy;

    *S = DCx*ACy - DCy*ACx;
    *T = BAx*ACy - BAy*ACx;

    if (denom == 0)
    {
        if (*S == 0 || *T == 0)
        { 
            // Lines incident
            return true;   
        }
        // Lines parallel and not incident
        return false;
    }

    *S = *S / denom;
    *T = *T / denom;

    // Point of intersection
    // CGPoint P;
    // P.x = A.x + *S * (B.x - A.x);
    // P.y = A.y + *S * (B.y - A.y);

    return true;
}

float ccpAngle(const DPoint& a, const DPoint& b)
{
    float angle = acosf(ccpDot(ccpNormalize(a), ccpNormalize(b)));
    if( fabs(angle) < kDPointEpsilon ) return 0.f;
    return angle;
}

NS_CC_END

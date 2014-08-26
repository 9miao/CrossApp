

#include "CCPointExtension.h"
#include "ccMacros.h" // FLT_EPSILON
#include <stdio.h>

NS_CC_BEGIN

#define kCCPointEpsilon FLT_EPSILON

float
ccpLength(const CCPoint& v)
{
    return v.getLength();
}

float
ccpDistance(const CCPoint& v1, const CCPoint& v2)
{
    return (v1 - v2).getLength();
}

CCPoint
ccpNormalize(const CCPoint& v)
{
    return v.normalize();
}

CCPoint
ccpForAngle(const float a)
{
    return CCPoint::forAngle(a);
}

float
ccpToAngle(const CCPoint& v)
{
    return v.getAngle();
}

CCPoint ccpLerp(const CCPoint& a, const CCPoint& b, float alpha)
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

CCPoint ccpClamp(const CCPoint& p, const CCPoint& min_inclusive, const CCPoint& max_inclusive)
{
    return ccp(clampf(p.x,min_inclusive.x,max_inclusive.x), clampf(p.y, min_inclusive.y, max_inclusive.y));
}

CCPoint ccpFromSize(const CCSize& s)
{
    return CCPoint(s);
}

CCPoint ccpCompOp(const CCPoint& p, float (*opFunc)(float))
{
    return ccp(opFunc(p.x), opFunc(p.y));
}

bool ccpFuzzyEqual(const CCPoint& a, const CCPoint& b, float var)
{
	return a.fuzzyEquals(b, var);
}

CCPoint ccpCompMult(const CCPoint& a, const CCPoint& b)
{
    return ccp(a.x * b.x, a.y * b.y);
}

float ccpAngleSigned(const CCPoint& a, const CCPoint& b)
{
	return a.getAngle(b);
}

CCPoint ccpRotateByAngle(const CCPoint& v, const CCPoint& pivot, float angle)
{
	return v.rotateByAngle(pivot, angle);
}

/*
bool ccpSegmentIntersect(const CCPoint& A, const CCPoint& B, const CCPoint& C, const CCPoint& D)
{
    float S, T;

    if( ccpLineIntersect(A, B, C, D, &S, &T )
        && (S >= 0.0f && S <= 1.0f && T >= 0.0f && T <= 1.0f) )
        return true;

    return false;
}
*/

// This algorithm is from the book Introduction to Algorithms
static inline bool _onSegment(const CCPoint &p1, const CCPoint &p2, const CCPoint &a)
{
    if (p1.x < p2.x) { if (a.x < p1.x || a.x > p2.x) return false; }
    else             { if (a.x < p2.x || a.x > p1.x) return false; }

    if (p1.y < p2.y) { if (a.y < p1.y || a.y > p2.y) return false; }
    else             { if (a.y < p2.y || a.y > p1.y) return false; }

    return true;
}

bool ccpSegmentIntersect(const CCPoint &p1, const CCPoint &p2, const CCPoint &p3, const CCPoint &p4)
{
    if ((p1.x == p2.x && p1.y == p2.y) || (p3.x == p4.x && p3.y == p4.y))
    {
        return false;
    }

    const float x13 = p3.x - p1.x;
    const float y13 = p3.y - p1.y;
    const float x14 = p4.x - p1.x;
    const float y14 = p4.y - p1.y;

    const float x23 = p3.x - p2.x;
    const float y23 = p3.y - p2.y;
    const float x43 = p3.x - p4.x;
    const float y43 = p3.y - p4.y;

    const float x24 = p4.x - p2.x;
    const float y24 = p4.y - p2.y;

    const float x31 = -x13;
    const float y31 = -x13;
    const float x21 = p1.x - p2.x;
    const float y21 = p1.y - p2.y;

    const float x41 = -x14;
    const float y41 = -y14;

    const float d1 = x13 * y14 - y13 * x14;
    const float d2 = x23 * y43 - y23 * x43;
    const float d3 = x31 * y21 - y31 * x21;
    const float d4 = x41 * y21 - y41 * x21;

    if (((d1 > 0.0F && d2 < 0.0F) || (d1 < 0.0F && d2 > 0.0F)) && ((d3 > 0.0F && d4 < 0.0F) || (d3 < 0.0F && d4 > 0.0F)))
    {
        return true;
    }
    else if (d1 == 0.0F && _onSegment(p3, p4, p1))
    {
        return true;
    }
    else if (d2 == 0.0F && _onSegment(p3, p4, p1))
    {
        return true;
    }
    else if (d3 == 0.0F && _onSegment(p1, p2, p3))
    {
        return true;
    }
    else if (d4 == 0.0F && _onSegment(p1, p2, p4))
    {
        return true;
    }

    return false;
}

CCPoint ccpIntersectPoint(const CCPoint& A, const CCPoint& B, const CCPoint& C, const CCPoint& D)
{
    float S, T;

    if( ccpLineIntersect(A, B, C, D, &S, &T) )
    {
        // Point of intersection
        CCPoint P;
        P.x = A.x + S * (B.x - A.x);
        P.y = A.y + S * (B.y - A.y);
        return P;
    }

    return CCPointZero;
}

bool ccpLineIntersect(const CCPoint& A, const CCPoint& B, 
                      const CCPoint& C, const CCPoint& D,
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

float ccpAngle(const CCPoint& a, const CCPoint& b)
{
    float angle = acosf(ccpDot(ccpNormalize(a), ccpNormalize(b)));
    if( fabs(angle) < kCCPointEpsilon ) return 0.f;
    return angle;
}

NS_CC_END

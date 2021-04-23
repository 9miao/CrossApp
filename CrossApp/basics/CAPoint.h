

#ifndef __DPoint_H__
#define __DPoint_H__

#include "platform/CCPlatformMacros.h"
#include "CAObject.h"
#include <math.h>
#include <float.h>

NS_CC_BEGIN

class CC_DLL DSize;

class CC_DLL DPoint
{
public:
    float x;
    float y;

public:
    
    DPoint();
    
    DPoint(float x, float y);

    DPoint(const DPoint& other);

    DPoint(const DSize& size);

    DPoint& operator= (const DPoint& other);

    DPoint& operator= (const DSize& size);

    DPoint operator+(const DPoint& right) const;

    DPoint operator-(const DPoint& right) const;

    DPoint operator-() const;

    DPoint operator*(float a) const;

    DPoint operator/(float a) const;

    void setPoint(float x, float y);
    
    bool equals(const DPoint& target) const;

    bool fuzzyEquals(const DPoint& target, float variance) const;

    float getLength() const;

    float getLengthSq() const;

    float getDistanceSq(const DPoint& other) const;

    float getDistance(const DPoint& other) const;

    float getAngle() const;

    float getAngle(const DPoint& other) const;

    float dot(const DPoint& other) const;

    float cross(const DPoint& other) const;

    DPoint getPerp() const;

    DPoint getRPerp() const;

    DPoint project(const DPoint& other) const;

    DPoint rotate(const DPoint& other) const;

    DPoint unrotate(const DPoint& other) const;

    DPoint normalize() const;

    DPoint lerp(const DPoint& other, float alpha) const;

    DPoint rotateByAngle(const DPoint& pivot, float angle) const;

    static DPoint forAngle(const float a);
};

const DPoint DPointZero = DPoint();

typedef DPoint Vec2;

#define DPOINT_FLT_MIN DPoint(FLT_MIN, FLT_MIN)

NS_CC_END

#endif // __DPoint_H__

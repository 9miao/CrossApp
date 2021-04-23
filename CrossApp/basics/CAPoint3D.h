

#ifndef __DPoint3D_H__
#define __DPoint3D_H__

#include "platform/CCPlatformMacros.h"
#include "CAObject.h"
#include "math/CAMathBase.h"
NS_CC_BEGIN

class Mat4;
class Quaternion;
class CC_DLL DPoint3D
{
public:
    float x;
    float y;
    float z;
    
public:
    
    DPoint3D();
    
    DPoint3D(float x, float y, float z);

    DPoint3D(const DPoint3D& other);
    
    DPoint3D& operator= (const DPoint3D& other);

    DPoint3D operator+(const DPoint3D& right) const;

    DPoint3D operator-(const DPoint3D& right) const;

    DPoint3D operator-() const;

    DPoint3D operator*(float a) const;

    DPoint3D operator/(float a) const;

    void setPoint(float x, float y, float z);
    
    bool equals(const DPoint3D& target);
    
    bool fuzzyEquals(const DPoint3D& target, float variance) const;
    
    float getLength() const;
    
    float getLengthSq() const;
    
    float getDistanceSq(const DPoint3D& other) const;
    
    float getDistance(const DPoint3D& other) const;
    
    float getAngle(const DPoint3D& other) const;
    
    float getAngle() const;
    
    float dot(const DPoint3D& other) const;
    
    static void cross(const DPoint3D& v1, const DPoint3D& v2, DPoint3D* dst);
    
    DPoint3D normalize() const;
    
    DPoint3D lerp(const DPoint3D& other, float alpha) const;
    
    inline void subtract(const DPoint3D& v);

    static void subtract(const DPoint3D& v1, const DPoint3D& v2, DPoint3D* dst);

    void smooth(const DPoint3D& target, float elapsedTime, float responseTime);
};

const DPoint3D DPoint3DZero     = DPoint3D();
const DPoint3D DPoint3DOne      = DPoint3D(1.0f, 1.0f, 1.0f);
const DPoint3D DPoint3DUNIT_X   = DPoint3D(1.0f, 0.0f, 0.0f);
const DPoint3D DPoint3DUNIT_Y   = DPoint3D(0.0f, 1.0f, 0.0f);
const DPoint3D DPoint3DUNIT_Z   = DPoint3D(0.0f, 0.0f, 1.0f);

typedef DPoint3D Vec3;

NS_CC_END

#endif // __CCGEMETRY_H__

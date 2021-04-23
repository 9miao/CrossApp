

#ifndef __Vec4_H__
#define __Vec4_H__

#include "platform/CCPlatformMacros.h"
#include "CAObject.h"
#include "math/CAMathBase.h"

#ifdef __SSE__
#include <xmmintrin.h>
#endif

NS_CC_BEGIN

class Mat4;
class Quaternion;
class CC_DLL Vec4
{
public:
#ifdef __SSE__
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        __m128 v;
    };
#else

    float x;
    float y;
    float z;
    float w;
#endif
public:
    
    Vec4();
    
    Vec4(float x, float y, float z, float w);

    Vec4(const Vec4& other);

    Vec4& operator= (const Vec4& other);

    Vec4 operator+(const Vec4& right) const;

    Vec4 operator-(const Vec4& right) const;

    Vec4 operator-() const;

    Vec4 operator*(float a) const;

    Vec4 operator/(float a) const;

    static Vec4 fromColor(unsigned int color);
    
    void setVec4(float x, float y, float z, float w);
    
    bool equals(const Vec4& target);
    
    bool fuzzyEquals(const Vec4& target, float variance) const;
    
    float getLength() const;
    
    float getLengthSq() const;
    
    float getLengthSquared() const;
    
    float getDistanceSq(const Vec4& other) const;
    
    float getDistance(const Vec4& other) const;
    
    float getAngle(const Vec4& other) const;
    
    float getAngle() const;
    
    float dot(const Vec4& other) const;
    
    Vec4 getNormalized() const;

    void subtract(const Vec4& v);

    static void subtract(const Vec4& v1, const Vec4& v2, Vec4* dst);
};

const Vec4 Vec4Zero = Vec4();

NS_CC_END

#endif // __CCGEMETRY_H__

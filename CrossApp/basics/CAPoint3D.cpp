

#include "CAPoint3D.h"
#include "ccMacros.h"
#include "math/CAMathUtil.h"

// implementation of DPoint
NS_CC_BEGIN
DPoint3D::DPoint3D() : x(0), y(0), z(0)
{
    
}

DPoint3D::DPoint3D(float x, float y, float z)
{
    setPoint(x, y, z);
}

DPoint3D::DPoint3D(const DPoint3D& other)
{
    setPoint(other.x, other.y, other.z);
}

DPoint3D& DPoint3D::operator= (const DPoint3D& other)
{
    setPoint(other.x, other.y, other.z);
    return *this;
}

DPoint3D DPoint3D::operator+(const DPoint3D& right) const
{
    return DPoint3D(this->x + right.x, this->y + right.y, this->z + right.z);
}

DPoint3D DPoint3D::operator-(const DPoint3D& right) const
{
    return DPoint3D(this->x - right.x, this->y - right.y, this->z - right.z);
}

DPoint3D DPoint3D::operator-() const
{
    return DPoint3D(-this->x, -this->y, -this->z);
}

DPoint3D DPoint3D::operator*(float a) const
{
    return DPoint3D(this->x * a, this->y * a, this->z * a);
}

DPoint3D DPoint3D::operator/(float a) const
{
    return DPoint3D(this->x / a, this->y / a, this->z / a);
}

void DPoint3D::setPoint(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

bool DPoint3D::equals(const DPoint3D& target)
{
    return (fabs(this->x - target.x) < FLT_EPSILON)
        && (fabs(this->y - target.y) < FLT_EPSILON)
        && (fabs(this->z - target.z) < FLT_EPSILON);
}

bool DPoint3D::fuzzyEquals(const DPoint3D& b, float var) const
{
    return (x - var <= b.x && b.x <= x + var)
        && (y - var <= b.y && b.y <= y + var)
        && (z - var <= b.z && b.z <= z + var);
}

float DPoint3D::getLength() const
{
    return sqrt(x * x + y * y + z * z);
};

float DPoint3D::getLengthSq() const
{
    return dot(*this); //x*x + y*y;
};

float DPoint3D::getDistanceSq(const DPoint3D& other) const
{
    return (*this - other).getLengthSq();
};

float DPoint3D::getDistance(const DPoint3D& other) const
{
    return (*this - other).getLength();
};

float DPoint3D::getAngle(const DPoint3D& other) const
{
    float dx = this->y * other.z - this->z * other.y;
    float dy = this->z * other.x - this->x * other.z;
    float dz = this->x * other.y - this->y * other.x;
    
    return atan2f(sqrt(dx * dx + dy * dy + dz * dz) + MATH_FLOAT_SMALL, dot(other));
}

float DPoint3D::getAngle() const
{
    return getAngle(DPoint3DZero);
};

float DPoint3D::dot(const DPoint3D& other) const
{
    return x*other.x + y*other.y + z*other.z;
};

void DPoint3D::cross(const DPoint3D& v1, const DPoint3D& v2, DPoint3D* dst)
{
    GP_ASSERT(dst);
    
    // NOTE: This code assumes Vec3 struct members are contiguous floats in memory.
    // We might want to revisit this (and other areas of code that make this assumption)
    // later to guarantee 100% safety/compatibility.
    MathUtil::crossVec3(&v1.x, &v2.x, &dst->x);
}

DPoint3D DPoint3D::normalize() const
{
    DPoint3D v(*this);
    float n = x * x + y * y + z * z;
    // Already normalized.
    if (n == 1.0f)
        return v;
    
    n = sqrt(n);
    // Too close to zero.
    if (n < MATH_TOLERANCE)
        return v;
    
    n = 1.0f / n;
    v.x *= n;
    v.y *= n;
    v.z *= n;
    return v;
}

DPoint3D DPoint3D::lerp(const DPoint3D& other, float alpha) const
{
    return *this * (1.f - alpha) + other * alpha;
}

void DPoint3D::subtract(const Vec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void DPoint3D::subtract(const DPoint3D& v1, const DPoint3D& v2, DPoint3D* dst)
{
    GP_ASSERT(dst);
    
    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
    dst->z = v1.z - v2.z;
}

void DPoint3D::smooth(const DPoint3D& target, float elapsedTime, float responseTime)
{
    if (elapsedTime > 0)
    {
        *this = *this + (target - *this) * (elapsedTime / (elapsedTime + responseTime));
    }
}

NS_CC_END

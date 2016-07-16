

#include "CAVec4.h"
#include "ccMacros.h"
#include "math/MathUtil.h"
#include <math.h>
// implementation of DPoint
NS_CC_BEGIN
Vec4::Vec4() : x(0), y(0), z(0), w(0)
{
};

Vec4::Vec4(float x, float y, float z, float w)
{
    setVec4(x, y, z, w);
}

Vec4::Vec4(const Vec4& other)
{
    setVec4(other.x, other.y, other.z, other.w);
}

Vec4& Vec4::operator= (const Vec4& other)
{
    setVec4(other.x, other.y, other.z, other.w);
    return *this;
}

Vec4 Vec4::operator+(const Vec4& right) const
{
    return Vec4(this->x + right.x, this->y + right.y, this->z + right.z, this->w + right.w);
}

Vec4 Vec4::operator-(const Vec4& right) const
{
    return Vec4(this->x - right.x, this->y - right.y, this->z - right.z, this->w - right.w);
}

Vec4 Vec4::operator-() const
{
    return Vec4(-this->x, -this->y, -this->z, -this->w);
}

Vec4 Vec4::operator*(float a) const
{
    return Vec4(this->x * a, this->y * a, this->z * a, this->w * a);
}

Vec4 Vec4::operator/(float a) const
{
    return Vec4(this->x / a, this->y / a, this->z / a, this->w / a);
}

void Vec4::setVec4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

bool Vec4::equals(const Vec4& target)
{
    return (fabs(this->x - target.x) < FLT_EPSILON)
        && (fabs(this->y - target.y) < FLT_EPSILON)
        && (fabs(this->z - target.z) < FLT_EPSILON)
        && (fabs(this->w - target.w) < FLT_EPSILON);
}

bool Vec4::fuzzyEquals(const Vec4& b, float var) const
{
    return (x - var <= b.x && b.x <= x + var)
        && (y - var <= b.y && b.y <= y + var)
        && (z - var <= b.z && b.z <= z + var)
        && (w - var <= b.w && b.w <= w + var);
}

float Vec4::getLength() const
{
    return sqrt(x * x + y * y + z * z + w * w);
};

float Vec4::getLengthSq() const
{
    return dot(*this); //x*x + y*y;
};

float Vec4::getDistanceSq(const Vec4& other) const
{
    return (*this - other).getLengthSq();
};

float Vec4::getDistance(const Vec4& other) const
{
    return (*this - other).getLength();
};

float Vec4::getAngle(const Vec4& other) const
{
    float dx = this->w * other.x - this->x * other.w - this->y * other.z + this->z * other.y;
    float dy = this->w * other.y - this->y * other.w - this->z * other.x + this->x * other.z;
    float dz = this->w * other.z - this->z * other.w - this->x * other.y + this->y * other.x;
    
    return atan2f(sqrt(dx * dx + dy * dy + dz * dz) + MATH_FLOAT_SMALL, this->dot(other));
}

float Vec4::getAngle() const
{
    return getAngle(Vec4Zero);
};

float Vec4::dot(const Vec4& other) const
{
    return (x * other.x + y * other.y + z * other.z + w * other.w);
};



NS_CC_END

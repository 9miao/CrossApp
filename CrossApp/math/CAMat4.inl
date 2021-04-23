

#include "math/CAMat4.h"

NS_CC_BEGIN

inline const Mat4 Mat4::operator+(const Mat4& mat) const
{
    Mat4 result(*this);
    result.add(mat);
    return result;
}

inline Mat4& Mat4::operator+=(const Mat4& mat)
{
    add(mat);
    return *this;
}

inline const Mat4 Mat4::operator-(const Mat4& mat) const
{
    Mat4 result(*this);
    result.subtract(mat);
    return result;
}

inline Mat4& Mat4::operator-=(const Mat4& mat)
{
    subtract(mat);
    return *this;
}

inline const Mat4 Mat4::operator-() const
{
    Mat4 mat(*this);
    mat.negate();
    return mat;
}

inline const Mat4 Mat4::operator*(const Mat4& mat) const
{
    Mat4 result(*this);
    result.multiply(mat);
    return result;
}

inline Mat4& Mat4::operator*=(const Mat4& mat)
{
    multiply(mat);
    return *this;
}

inline DPoint3D& operator*=(DPoint3D& v, const Mat4& m)
{
    m.transformVector(&v);
    return v;
}

inline const DPoint3D operator*(const Mat4& m, const DPoint3D& v)
{
    DPoint3D x;
    m.transformVector(v, &x);
    return x;
}

inline Vec4& operator*=(Vec4& v, const Mat4& m)
{
    m.transformVector(&v);
    return v;
}

inline const Vec4 operator*(const Mat4& m, const Vec4& v)
{
    Vec4 x;
    m.transformVector(v, &x);
    return x;
}

NS_CC_END

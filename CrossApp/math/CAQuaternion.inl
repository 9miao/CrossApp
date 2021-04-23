
#include "math/CAQuaternion.h"

NS_CC_BEGIN

inline const Quaternion Quaternion::operator*(const Quaternion& q) const
{
    Quaternion result(*this);
    result.multiply(q);
    return result;
}

inline Quaternion& Quaternion::operator*=(const Quaternion& q)
{
    multiply(q);
    return *this;
}

inline DPoint3D Quaternion::operator*(const DPoint3D& v) const
{
    DPoint3D uv, uuv;
    DPoint3D qvec(x, y, z);
    DPoint3D::cross(qvec, v, &uv);
    DPoint3D::cross(qvec, uv, &uuv);

    uv = uv * (2.0f * w);
    uuv = uuv * 2.0f;

    return v + uv + uuv;
}

NS_CC_END

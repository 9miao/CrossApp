

#include "CAPoint.h"
#include "CASize.h"
#include "ccMacros.h"
#include "basics/CAApplication.h"

// implementation of DPoint
NS_CC_BEGIN

DPoint::DPoint(void) : x(0), y(0)
{
}

DPoint::DPoint(float x, float y)
{
    setPoint(x, y);
}

DPoint::DPoint(const DPoint& other)
{
    setPoint(other.x, other.y);
}

DPoint::DPoint(const DSize& size)
{
    setPoint(size.width, size.height);
}

DPoint& DPoint::operator= (const DPoint& other)
{
    setPoint(other.x, other.y);
    return *this;
}

DPoint& DPoint::operator= (const DSize& size)
{
    setPoint(size.width, size.height);
    return *this;
}

DPoint DPoint::operator+(const DPoint& right) const
{
    return DPoint(this->x + right.x, this->y + right.y);
}

DPoint DPoint::operator-(const DPoint& right) const
{
    return DPoint(this->x - right.x, this->y - right.y);
}

DPoint DPoint::operator-() const
{
	return DPoint(-x, -y);
}

DPoint DPoint::operator*(float a) const
{
    return DPoint(this->x * a, this->y * a);
}

DPoint DPoint::operator/(float a) const
{
	CCAssert(a, "DPoint division by 0.");
    return DPoint(this->x / a, this->y / a);
}

void DPoint::setPoint(float x, float y)
{
    this->x = x;
    this->y = y;
}

bool DPoint::equals(const DPoint& target) const
{
    return (fabs(this->x - target.x) < FLT_EPSILON)
        && (fabs(this->y - target.y) < FLT_EPSILON);
}

bool DPoint::fuzzyEquals(const DPoint& b, float var) const
{
    return (x - var <= b.x && b.x <= x + var)
        && (y - var <= b.y && b.y <= y + var);
}

float DPoint::getLength() const
{
    return sqrtf(x*x + y*y);
};

float DPoint::getLengthSq() const
{
    return dot(*this); //x*x + y*y;
};

float DPoint::getDistanceSq(const DPoint& other) const
{
    return (*this - other).getLengthSq();
};

float DPoint::getDistance(const DPoint& other) const
{
    return (*this - other).getLength();
};

float DPoint::getAngle() const
{
    return atan2f(y, x);
};

float DPoint::getAngle(const DPoint& other) const
{
    DPoint a2 = normalize();
    DPoint b2 = other.normalize();
    float angle = atan2f(a2.cross(b2), a2.dot(b2));
    if( fabs(angle) < FLT_EPSILON ) return 0.f;
    return angle;
}

float DPoint::dot(const DPoint& other) const
{
    return x*other.x + y*other.y;
};

float DPoint::cross(const DPoint& other) const
{
    return x*other.y - y*other.x;
};

DPoint DPoint::getPerp() const
{
    return DPoint(-y, x);
};

DPoint DPoint::getRPerp() const
{
    return DPoint(y, -x);
};

DPoint DPoint::project(const DPoint& other) const
{
    return other * (dot(other)/other.dot(other));
};

DPoint DPoint::rotate(const DPoint& other) const
{
    return DPoint(x*other.x - y*other.y, x*other.y + y*other.x);
};

DPoint DPoint::unrotate(const DPoint& other) const
{
    return DPoint(x*other.x + y*other.y, y*other.x - x*other.y);
};

DPoint DPoint::normalize() const
{
    float length = getLength();
    if(length == 0.) return DPoint(1.f, 0);
    return *this / getLength();
};

DPoint DPoint::lerp(const DPoint& other, float alpha) const
{
    return *this * (1.f - alpha) + other * alpha;
};

DPoint DPoint::rotateByAngle(const DPoint& pivot, float angle) const
{
    return pivot + (*this - pivot).rotate(DPoint::forAngle(angle));
}

DPoint DPoint::forAngle(const float a)
{
    return DPoint(cosf(a), sinf(a));
}


NS_CC_END

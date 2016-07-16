
#include "CASize.h"
#include "CAPoint.h"
#include "ccMacros.h"
#include "basics/CAApplication.h"

// implementation of DPoint
NS_CC_BEGIN

DSize::DSize(void) : width(0), height(0)
{
}

DSize::DSize(float width, float height)
{
    setSize(width, height);
}

DSize::DSize(const DSize& other)
{
    setSize(other.width, other.height);
}

DSize::DSize(const DPoint& point)
{
    setSize(point.x, point.y);
}

DSize& DSize::operator= (const DSize& other)
{
    setSize(other.width, other.height);
    return *this;
}

DSize& DSize::operator= (const DPoint& point)
{
    setSize(point.x, point.y);
    return *this;
}

DSize DSize::operator+(const DSize& right) const
{
    return DSize(this->width + right.width, this->height + right.height);
}

DSize DSize::operator-(const DSize& right) const
{
    return DSize(this->width - right.width, this->height - right.height);
}

DSize DSize::operator*(float a) const
{
    return DSize(this->width * a, this->height * a);
}

DSize DSize::operator/(float a) const
{
	CCAssert(a, "DSize division by 0.");
    return DSize(this->width / a, this->height / a);
}

void DSize::setSize(float width, float height)
{
    this->width = width;
    this->height = height;
}

bool DSize::equals(const DSize& target) const
{
    return (fabs(this->width  - target.width)  < FLT_EPSILON)
        && (fabs(this->height - target.height) < FLT_EPSILON);
}

bool DSize::fuzzyEquals(const DSize& b, float var) const
{
    return (width - var <= b.width && b.width <= width + var)
        && (height - var <= b.height && b.height <= height + var);
}

NS_CC_END

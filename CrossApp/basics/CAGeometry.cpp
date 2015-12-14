

#include "CAGeometry.h"
#include "ccMacros.h"
#include "basics/CAApplication.h"

// implementation of DPoint
NS_CC_BEGIN

float fround(float x)//double round
{
    float y = 100;
    int xx = x > FLT_EPSILON ? (x * y + 0.5) : (x * y - 0.5);
    return xx/y;
}

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
    this->x = fround(x);
    this->y = fround(y);
}

bool DPoint::equals(const DPoint& target) const
{
    return (fabs(this->x - target.x) < 0.01f)
        && (fabs(this->y - target.y) < 0.01f);
}

bool DPoint::fuzzyEquals(const DPoint& b, float var) const
{
    if(x - var <= b.x && b.x <= x + var)
        if(y - var <= b.y && b.y <= y + var)
            return true;
    return false;
}

float DPoint::getAngle(const DPoint& other) const
{
    DPoint a2 = normalize();
    DPoint b2 = other.normalize();
    float angle = atan2f(a2.cross(b2), a2.dot(b2));
    if( fabs(angle) < 0.01f ) return 0.f;
    return angle;
}

DPoint DPoint::rotateByAngle(const DPoint& pivot, float angle) const
{
    return pivot + (*this - pivot).rotate(DPoint::forAngle(angle));
}

// implementation of DSize

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
    this->width = fround(width);
    this->height = fround(height);
}

bool DSize::equals(const DSize& target) const
{
    return (fabs(this->width  - target.width)  < 0.01f)
        && (fabs(this->height - target.height) < 0.01f);
}

// implementation of DRect

DRect::DRect(void)
:m_bCenter(false)
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

DRect::DRect(float x, float y, float width, float height)
:m_bCenter(false)
{
    setRect(x, y, width, height);
}

DRect::DRect(const DRect& other)
:m_bCenter(other.isCenter())
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

void DRect::setRect(float x, float y, float width, float height)
{
    origin.setPoint(x, y);
    size.setSize(width, height);
}

DRect& DRect::operator= (const DRect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    m_bCenter = other.isCenter();
    return *this;
}

DRect DRect::operator*(float a) const
{
    return DRect(this->origin.x * a,
                  this->origin.y * a,
                  this->size.width * a,
                  this->size.height * a);;
}

DRect DRect::operator/(float a) const
{
    return DRect(this->origin.x / a,
                  this->origin.y / a,
                  this->size.width / a,
                  this->size.height / a);;
}

bool DRect::equals(const DRect& rect) const
{
    if (fabsf(this->getMinX() - rect.getMinX()) >= 0.01f)
    {
        return false;
    }
    
    if (fabsf(this->getMaxX() - rect.getMaxX()) >= 0.01f)
    {
        return false;
    }
    
    if (fabsf(this->getMinY() - rect.getMinY()) >= 0.01f)
    {
        return false;
    }
    
    if (fabsf(this->getMaxY() - rect.getMaxY()) >= 0.01f)
    {
        return false;
    }
    
    return true;
}

float DRect::getMaxX() const
{
    return m_bCenter ? (float)(origin.x + size.width / 2) : (float)(origin.x + size.width);
}

float DRect::getMidX() const
{
    return m_bCenter ? (float)origin.x : (float)(origin.x + size.width / 2);
}

float DRect::getMinX() const
{
    return m_bCenter ? (float)(origin.x - size.width / 2) : (float)origin.x;
}

float DRect::getMaxY() const
{
    return m_bCenter ? (float)(origin.y + size.height / 2) : (float)(origin.y + size.height);
}

float DRect::getMidY() const
{
    return m_bCenter ? (float)origin.y : (float)(origin.y + size.height / 2);
}

float DRect::getMinY() const
{
    return m_bCenter ? (float)(origin.y - size.height / 2) : (float)origin.y;
}

bool DRect::containsPoint(const DPoint& point) const
{
    bool bRet = false;

    if (   point.x >= getMinX()
        && point.x <= getMaxX()
        && point.y >= getMinY()
        && point.y <= getMaxY())
    {
        bRet = true;
    }

    return bRet;
}

bool DRect::intersectsRect(const DRect& rect) const
{
    return !(   getMaxX() < rect.getMinX()
             || rect.getMaxX() < getMinX()
             || getMaxY() < rect.getMinY()
             || rect.getMaxY() < getMinY());
}


void DRect::InflateRect(float v)
{
	size.width += 2 * v;
	size.height += 2 * v;

	if (!m_bCenter)
	{
		origin.x -= v;
		origin.y -= v;
	}
}

NS_CC_END

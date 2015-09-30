

#include "CAGeometry.h"
#include "ccMacros.h"
#include "basics/CAApplication.h"

// implementation of CCPoint
NS_CC_BEGIN

float fround(float x)//double round
{
    float y = 10000;
    int xx = x > FLT_EPSILON ? (x * y + 0.5) : (x * y - 0.5);
    return xx/y;
}

CCPoint::CCPoint(void) : x(0), y(0)
{
}

CCPoint::CCPoint(float x, float y)
{
    setPoint(x, y);
}

CCPoint::CCPoint(const CCPoint& other)
{
    setPoint(other.x, other.y);
}

CCPoint::CCPoint(const CCSize& size)
{
    setPoint(size.width, size.height);
}

CCPoint& CCPoint::operator= (const CCPoint& other)
{
    setPoint(other.x, other.y);
    return *this;
}

CCPoint& CCPoint::operator= (const CCSize& size)
{
    setPoint(size.width, size.height);
    return *this;
}

CCPoint CCPoint::operator+(const CCPoint& right) const
{
    return CCPoint(this->x + right.x, this->y + right.y);
}

CCPoint CCPoint::operator-(const CCPoint& right) const
{
    return CCPoint(this->x - right.x, this->y - right.y);
}

CCPoint CCPoint::operator-() const
{
	return CCPoint(-x, -y);
}

CCPoint CCPoint::operator*(float a) const
{
    return CCPoint(this->x * a, this->y * a);
}

CCPoint CCPoint::operator/(float a) const
{
	CCAssert(a, "CCPoint division by 0.");
    return CCPoint(this->x / a, this->y / a);
}

void CCPoint::setPoint(float x, float y)
{
    this->x = fround(x);
    this->y = fround(y);
}

bool CCPoint::equals(const CCPoint& target) const
{
    return (fabs(this->x - target.x) < FLT_EPSILON)
        && (fabs(this->y - target.y) < FLT_EPSILON);
}

bool CCPoint::fuzzyEquals(const CCPoint& b, float var) const
{
    if(x - var <= b.x && b.x <= x + var)
        if(y - var <= b.y && b.y <= y + var)
            return true;
    return false;
}

float CCPoint::getAngle(const CCPoint& other) const
{
    CCPoint a2 = normalize();
    CCPoint b2 = other.normalize();
    float angle = atan2f(a2.cross(b2), a2.dot(b2));
    if( fabs(angle) < FLT_EPSILON ) return 0.f;
    return angle;
}

CCPoint CCPoint::rotateByAngle(const CCPoint& pivot, float angle) const
{
    return pivot + (*this - pivot).rotate(CCPoint::forAngle(angle));
}

// implementation of CCSize

CCSize::CCSize(void) : width(0), height(0)
{
}

CCSize::CCSize(float width, float height)
{
    setSize(width, height);
}

CCSize::CCSize(const CCSize& other)
{
    setSize(other.width, other.height);
}

CCSize::CCSize(const CCPoint& point)
{
    setSize(point.x, point.y);
}

CCSize& CCSize::operator= (const CCSize& other)
{
    setSize(other.width, other.height);
    return *this;
}

CCSize& CCSize::operator= (const CCPoint& point)
{
    setSize(point.x, point.y);
    return *this;
}

CCSize CCSize::operator+(const CCSize& right) const
{
    return CCSize(this->width + right.width, this->height + right.height);
}

CCSize CCSize::operator-(const CCSize& right) const
{
    return CCSize(this->width - right.width, this->height - right.height);
}

CCSize CCSize::operator*(float a) const
{
    return CCSize(this->width * a, this->height * a);
}

CCSize CCSize::operator/(float a) const
{
	CCAssert(a, "CCSize division by 0.");
    return CCSize(this->width / a, this->height / a);
}

void CCSize::setSize(float width, float height)
{
    this->width = fround(width);
    this->height = fround(height);
}

bool CCSize::equals(const CCSize& target) const
{
    return (fabs(this->width  - target.width)  < FLT_EPSILON)
        && (fabs(this->height - target.height) < FLT_EPSILON);
}

// implementation of CCRect

CCRect::CCRect(void)
:m_bCenter(false)
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

CCRect::CCRect(float x, float y, float width, float height)
:m_bCenter(false)
{
    setRect(x, y, width, height);
}

CCRect::CCRect(const CCRect& other)
:m_bCenter(other.isCenter())
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

void CCRect::setRect(float x, float y, float width, float height)
{
    origin.setPoint(x, y);
    size.setSize(width, height);
}

CCRect& CCRect::operator= (const CCRect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    m_bCenter = other.isCenter();
    return *this;
}

CCRect CCRect::operator*(float a) const
{
    return CCRect(this->origin.x * a,
                  this->origin.y * a,
                  this->size.width * a,
                  this->size.height * a);;
}

CCRect CCRect::operator/(float a) const
{
    return CCRect(this->origin.x / a,
                  this->origin.y / a,
                  this->size.width / a,
                  this->size.height / a);;
}

bool CCRect::equals(const CCRect& rect) const
{
    if (fabsf(this->getMinX() - rect.getMinX()) >= FLT_EPSILON)
    {
        return false;
    }
    
    if (fabsf(this->getMaxX() - rect.getMaxX()) >= FLT_EPSILON)
    {
        return false;
    }
    
    if (fabsf(this->getMinY() - rect.getMinY()) >= FLT_EPSILON)
    {
        return false;
    }
    
    if (fabsf(this->getMaxY() - rect.getMaxY()) >= FLT_EPSILON)
    {
        return false;
    }
    
    return true;
}

float CCRect::getMaxX() const
{
    return m_bCenter ? (float)(origin.x + size.width/2) : (float)(origin.x + size.width);
}

float CCRect::getMidX() const
{
    return m_bCenter ? (float)origin.x : (float)(origin.x + size.width / 2.0);
}

float CCRect::getMinX() const
{
    return m_bCenter ? (float)(origin.x - size.width/2) : (float)origin.x;
}

float CCRect::getMaxY() const
{
    return m_bCenter ? (float)(origin.y + size.height/2) : (float)(origin.y + size.height);
}

float CCRect::getMidY() const
{
    return m_bCenter ? (float)origin.y : (float)(origin.y + size.height / 2.0);
}

float CCRect::getMinY() const
{
    return m_bCenter ? (float)(origin.y - size.height/2) : (float)origin.y;
}

bool CCRect::containsPoint(const CCPoint& point) const
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

bool CCRect::intersectsRect(const CCRect& rect) const
{
    return !(   getMaxX() < rect.getMinX()
             || rect.getMaxX() < getMinX()
             || getMaxY() < rect.getMinY()
             || rect.getMaxY() < getMinY());
}


void CCRect::InflateRect(float v)
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

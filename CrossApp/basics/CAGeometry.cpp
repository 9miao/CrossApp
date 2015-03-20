

#include "CAGeometry.h"
#include "ccMacros.h"
#include "basics/CAApplication.h"

// implementation of CCPoint
NS_CC_BEGIN

CCPoint::CCPoint(void) : x(0), y(0)
{
}

CCPoint::CCPoint(float x, float y) : x(x), y(y)
{
}

CCPoint::CCPoint(const CCPoint& other) : x(other.x), y(other.y)
{
}

CCPoint::CCPoint(const CCSize& size) : x(size.width), y(size.height)
{
}

CCPoint::CCPoint(const CADipPoint& other)
: x(_px(other.x))
, y(_px(other.y))
{
}

CCPoint::CCPoint(const CADipSize& size)
: x(_px(size.width))
, y(_px(size.height))
{
}

CCPoint& CCPoint::operator= (const CADipPoint& other)
{
    setPoint(_px(other.x), _px(other.y));
    return *this;
}

CCPoint& CCPoint::operator= (const CADipSize& other)
{
    setPoint(_px(other.width), _px(other.height));
    return *this;
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
    this->x = x;
    this->y = y;
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

CCSize::CCSize(float width, float height) : width(width), height(height)
{
}

CCSize::CCSize(const CCSize& other) : width(other.width), height(other.height)
{
}

CCSize::CCSize(const CCPoint& point) : width(point.x), height(point.y)
{
}

CCSize::CCSize(const CADipPoint& other)
: width(_px(other.x))
, height(_px(other.y))
{
}

CCSize::CCSize(const CADipSize& size)
: width(_px(size.width))
, height(_px(size.height))
{
}

CCSize& CCSize::operator= (const CADipPoint& other)
{
    setSize(_px(other.x), _px(other.y));
    return *this;
}

CCSize& CCSize::operator= (const CADipSize& other)
{
    setSize(_px(other.width), _px(other.height));
    return *this;
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
    this->width = width;
    this->height = height;
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

CCRect::CCRect(const CADipRect& other)
:m_bCenter(other.isCenter())
{
    origin = other.origin;
    size = other.size;
}

void CCRect::setRect(float x, float y, float width, float height)
{
    origin.x = x;
    origin.y = y;

    size.width = width;
    size.height = height;
}

CCRect& CCRect::operator= (const CCRect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    m_bCenter = other.isCenter();
    return *this;
}

CCRect& CCRect::operator= (const CADipRect& other)
{
    origin = other.origin;
    size = other.size;
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

    if (point.x >= getMinX()
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


CADipPoint::CADipPoint()
:x(0.0f)
,y(0.0f)
{
    
}

CADipPoint::CADipPoint(float x, float y)
:x(x)
,y(y)
{
    
}

CADipPoint::CADipPoint(const CADipPoint& other)
:x(other.x)
,y(other.y)
{
    
}

CADipPoint::CADipPoint(const CADipSize& other)
:x(other.width)
,y(other.height)
{
    
}

CADipPoint::CADipPoint(const CCPoint& other)
:x(_dip(other.x))
,y(_dip(other.y))
{
    
}

CADipPoint::CADipPoint(const CCSize& other)
:x(_dip(other.width))
,y(_dip(other.height))
{
    
}

CADipPoint& CADipPoint::operator= (const CADipPoint& other)
{
    this->x = other.x;
    this->y = other.y;
    return *this;
}

CADipPoint& CADipPoint::operator= (const CADipSize& other)
{
    this->x = other.width;
    this->y = other.height;
    return *this;
}

CADipPoint& CADipPoint::operator= (const CCPoint& other)
{
    this->x = _dip(other.x);
    this->y = _dip(other.y);
    return *this;
}

CADipPoint& CADipPoint::operator= (const CCSize& other)
{
    this->x = _dip(other.width);
    this->y = _dip(other.height);
    return *this;
}

CADipPoint CADipPoint::operator*(float a) const
{
    return CADipPoint(this->x * a, this->y * a);
}


CADipPoint CADipPoint::operator/(float a) const
{
    return CADipPoint(this->x / a, this->y / a);
}

CADipPoint CADipPoint::operator+(const CADipPoint& right) const
{
    return CADipPoint(this->x + right.x, this->y + right.y);
}

CADipPoint CADipPoint::operator-(const CADipPoint& right) const
{
    return CADipPoint(this->x - right.x, this->y - right.y);
}

bool CADipPoint::equals(const CADipPoint& target) const
{
    return (this->x == target.x && this->y == target.y);
}



CADipSize::CADipSize()
:width(0.0f)
,height(0.0f)
{
    
}

CADipSize::CADipSize(float width, float height)
:width(width)
,height(height)
{
    
}

CADipSize::CADipSize(const CADipPoint& other)
:width(other.x)
,height(other.y)
{
    
}

CADipSize::CADipSize(const CADipSize& other)
:width(other.width)
,height(other.height)
{
    
}

CADipSize::CADipSize(const CCPoint& other)
:width(_dip(other.x))
,height(_dip(other.y))
{
    
}

CADipSize::CADipSize(const CCSize& other)
:width(_dip(other.width))
,height(_dip(other.height))
{
    
}

CADipSize& CADipSize::operator= (const CADipSize& other)
{
    this->width = other.width;
    this->height = other.height;
    return *this;
}

CADipSize& CADipSize::operator= (const CADipPoint& other)
{
    this->width = other.x;
    this->height = other.y;
    return *this;
}

CADipSize& CADipSize::operator= (const CCPoint& other)
{
    this->width = _dip(other.x);
    this->height = _dip(other.y);
    return *this;
}

CADipSize& CADipSize::operator= (const CCSize& other)
{
    this->width = _dip(other.width);
    this->height = _dip(other.height);
    return *this;
}

CADipSize CADipSize::operator*(float a) const
{
    return CADipSize(this->width * a, this->height * a);
}

CADipSize CADipSize::operator/(float a) const
{
    return CADipSize(this->width / a, this->height / a);
}

CADipSize CADipSize::operator+(const CADipSize& right) const
{
    return CADipSize(this->width + right.width, this->height + right.height);
}

CADipSize CADipSize::operator-(const CADipSize& right) const
{
    return CADipSize(this->width - right.width, this->height - right.height);
}

bool CADipSize::equals(const CADipSize& target) const
{
    return (this->width == target.width && this->height == target.height);
}


CADipRect::CADipRect(void)
:m_bCenter(false)
,origin(CADipPointZero)
,size(CADipSizeZero)
{

}

CADipRect::CADipRect(float x, float y, float width, float height)
:m_bCenter(false)
{
    origin = CADipPoint(x, y);
    size = CADipSize(width, height);
}

CADipRect::CADipRect(const CADipRect& other)
:m_bCenter(other.isCenter())
{
    origin = other.origin;
    size = other.size;
}

CADipRect::CADipRect(const CCRect& other)
:m_bCenter(other.isCenter())
{
    origin = other.origin;
    size = other.size;
}

CADipRect& CADipRect::operator= (const CADipRect& other)
{
    origin = other.origin;
    size = other.size;
    m_bCenter = other.isCenter();
    return *this;
}

CADipRect& CADipRect::operator= (const CCRect& other)
{
    origin = other.origin;
    size = other.size;
    m_bCenter = other.isCenter();
    return *this;
}

CADipRect CADipRect::operator*(float a) const
{
    return CADipRect(this->origin.x * a,
                  this->origin.y * a,
                  this->size.width * a,
                  this->size.height * a);;
}

CADipRect CADipRect::operator/(float a) const
{
    return CADipRect(this->origin.x / a,
                  this->origin.y / a,
                  this->size.width / a,
                  this->size.height / a);;
}

bool CADipRect::equals(const CADipRect& rect) const
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

float CADipRect::getMaxX() const
{
    return m_bCenter ? (float)(origin.x + size.width/2) : (float)(origin.x + size.width);
}

float CADipRect::getMidX() const
{
    return m_bCenter ? (float)origin.x : (float)(origin.x + size.width / 2.0);
}

float CADipRect::getMinX() const
{
    return m_bCenter ? (float)(origin.x - size.width/2) : (float)origin.x;
}

float CADipRect::getMaxY() const
{
    return m_bCenter ? (float)(origin.y + size.height/2) : (float)(origin.y + size.height);
}

float CADipRect::getMidY() const
{
    return m_bCenter ? (float)origin.y : (float)(origin.y + size.height / 2.0);
}

float CADipRect::getMinY() const
{
    return m_bCenter ? (float)(origin.y - size.height/2) : (float)origin.y;
}

bool CADipRect::containsPoint(const CADipPoint& point) const
{
    bool bRet = false;
    
    if (point.x >= getMinX()
        && point.x <= getMaxX()
        && point.y >= getMinY()
        && point.y <= getMaxY())
    {
        bRet = true;
    }
    
    return bRet;
}

bool CADipRect::intersectsRect(const CADipRect& rect) const
{
    return !(   getMaxX() < rect.getMinX()
             || rect.getMaxX() < getMinX()
             || getMaxY() < rect.getMinY()
             || rect.getMaxY() < getMinY());
}


NS_CC_END

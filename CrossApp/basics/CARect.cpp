

#include "CARect.h"

#include "ccMacros.h"
#include "basics/CAApplication.h"

// implementation of DPoint
NS_CC_BEGIN

DRect::DRect(void)
:m_eType(DRect::Type::Frame)
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

DRect::DRect(const DPoint& orgin, const DSize& size)
:m_eType(DRect::Type::Frame)
{
    setRect(orgin.x, orgin.y, size.width, size.height);
}


DRect::DRect(float x, float y, float width, float height)
:m_eType(DRect::Type::Frame)
{
    setRect(x, y, width, height);
}

DRect::DRect(const DRect& other)
:m_eType(other.m_eType)
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
    m_eType = other.m_eType;
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

float DRect::getMaxX() const
{
    return m_eType == DRect::Type::Center ? (float)(origin.x + size.width / 2) : (float)(origin.x + size.width);
}

float DRect::getMidX() const
{
    return m_eType == DRect::Type::Center ? (float)origin.x : (float)(origin.x + size.width / 2);
}

float DRect::getMinX() const
{
    return m_eType == DRect::Type::Center ? (float)(origin.x - size.width / 2) : (float)origin.x;
}

float DRect::getMaxY() const
{
    return m_eType == DRect::Type::Center ? (float)(origin.y + size.height / 2) : (float)(origin.y + size.height);
}

float DRect::getMidY() const
{
    return m_eType == DRect::Type::Center ? (float)origin.y : (float)(origin.y + size.height / 2);
}

float DRect::getMinY() const
{
    return m_eType == DRect::Type::Center ? (float)(origin.y - size.height / 2) : (float)origin.y;
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

	if (m_eType == DRect::Type::Frame)
	{
		origin.x -= v;
		origin.y -= v;
	}
}

void DRect::InflateRect(float hor, float ver)
{
    size.width += hor;
    size.height += ver;
    if (m_eType == DRect::Type::Frame)
    {
        origin.x -= hor/2;
        origin.y -= ver/2;
    }
}

void DRect::InflateRect(float l, float t, float r, float b)
{
	size.width += (l+r);
	size.height += (t+b);
	if (m_eType == DRect::Type::Frame)
	{
		origin.x -= l;
		origin.y -= t;
	}
}


NS_CC_END

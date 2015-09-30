

#include "support/CAPointExtension.h"
#include "CATouch.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

// returns the current touch location in OpenGL coordinates
DPoint CATouch::getLocation() const
{ 
    return m_point;
}

// returns the previous touch location in OpenGL coordinates
DPoint CATouch::getPreviousLocation() const
{ 
    return m_prevPoint;
}

// returns the start touch location in OpenGL coordinates
DPoint CATouch::getStartLocation() const
{ 
    return m_startPoint;  
}

// returns the delta position between the current location and the previous location in OpenGL coordinates
DPoint CATouch::getDelta() const
{     
    return ccpSub(getLocation(), getPreviousLocation()); 
}

DPoint CATouch::getDeltaFromAToZ() const
{
    return ccpSub(getLocation(), getStartLocation());
}

bool CATouch::isDelta() const
{
    return m_bDelta;
}

void CATouch::setTouchInfo(int id, float x, float y)
{
    m_nId = id;
    m_prevPoint = m_point.equals(DPoint(0xffffffff, 0xffffffff)) ? DPoint(x, y) : m_point;
    m_point.x   = x;
    m_point.y   = y;
    if (!m_startPointCaptured)
    {
        m_startPoint = m_point;
        m_startPointCaptured = true;
    }
    if ((m_startPoint - m_point).getLength() >= _px(32))
    {
        m_bDelta = true;
    }
}

NS_CC_END


#ifndef __CC_TOUCH_H__
#define __CC_TOUCH_H__

#include "basics/CAObject.h"
#include "basics/CAGeometry.h"

NS_CC_BEGIN

/**
 * @addtogroup input
 * @{
 */

class CC_DLL CATouch : public CAObject
{
public:
    /**
     * @js ctor
     */
    CATouch()
        : m_nId(0),
        m_startPointCaptured(false)
    {}

    /** returns the current touch location in OpenGL coordinates */
    CCPoint getLocation() const;
    /** returns the previous touch location in OpenGL coordinates */
    CCPoint getPreviousLocation() const;
    /** returns the start touch location in OpenGL coordinates */
    CCPoint getStartLocation() const;
    /** returns the delta of 2 current touches locations in screen coordinates */
    CCPoint getDelta() const;
    
    void setTouchInfo(int id, float x, float y)
    {
        m_nId = id;
        m_prevPoint = m_point;
        m_point.x   = x;
        m_point.y   = y;
        if (!m_startPointCaptured)
        {
            m_startPoint = m_point;
            m_startPointCaptured = true;
        }
    }
    /**
     *  @js getId
     */
    int getID() const
    {
        return m_nId;
    }

private:
    int m_nId;
    bool m_startPointCaptured;
    CCPoint m_startPoint;
    CCPoint m_point;
    CCPoint m_prevPoint;
};

class CC_DLL CAEvent : public CAObject
{
};

// end of input group
/// @}

NS_CC_END

#endif  // __PLATFORM_TOUCH_H__

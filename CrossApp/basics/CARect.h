

#ifndef __DRect_H__
#define __DRect_H__

#include "CAPoint.h"
#include "CASize.h"

NS_CC_BEGIN

class CC_DLL DRect
{
public:
    
    DPoint origin;
    
    DSize  size;

public:
    
    DRect();
    
    DRect(float x, float y, float width, float height);

    DRect(const DRect& other);
    
    void setRect(float x, float y, float width, float height);
    
    DRect& operator= (const DRect& other);
    
    DRect operator*(float a) const;

    DRect operator/(float a) const;
    
    float getMinX() const; /// return the leftmost x-value of current rect
    float getMidX() const; /// return the midpoint x-value of current rect
    float getMaxX() const; /// return the rightmost x-value of current rect
    float getMinY() const; /// return the bottommost y-value of current rect
    float getMidY() const; /// return the midpoint y-value of current rect
    float getMaxY() const; /// return the topmost y-value of current rect
    bool equals(const DRect& rect) const;
    bool containsPoint(const DPoint& point) const;
    bool intersectsRect(const DRect& rect) const;
    void InflateRect(float v);
	void InflateRect(float l, float t, float r, float b);
    
    typedef enum
    {
        Frame,
        Center
    }Type;
    
    CC_SYNTHESIZE_IS(Type, m_eType, Type);
};

const DRect DRectZero = DRect();

NS_CC_END

#endif // __DRect_H__



#ifndef __CCGEMETRY_H__
#define __CCGEMETRY_H__

#include "platform/CCPlatformMacros.h"
#include "CAObject.h"
#include <math.h>
#include <float.h>

NS_CC_BEGIN

class CC_DLL DSize;

class CC_DLL DPoint
{
public:
    float x;
    float y;

public:
    
    DPoint();
    
    DPoint(float x, float y);
    /**
     * @lua NA
     */
    DPoint(const DPoint& other);
    /**
     * @lua NA
     */
    DPoint(const DSize& size);
    /**
     * @lua NA
     */
    DPoint& operator= (const DPoint& other);
    /**
     * @lua NA
     */
    DPoint& operator= (const DSize& size);
    /**
     * @lua NA
     */
    DPoint operator+(const DPoint& right) const;
    /**
     * @lua NA
     */
    DPoint operator-(const DPoint& right) const;
    /**
     * @lua NA
     */
    DPoint operator-() const;
    /**
     * @lua NA
     */
    DPoint operator*(float a) const;
    /**
     * @lua NA
     */
    DPoint operator/(float a) const;
    /**
     * @lua NA
     */
    void setPoint(float x, float y);
    bool equals(const DPoint& target) const;
    
    /** @returns if points have fuzzy equality which means equal with some degree of variance.
     * @since v2.1.4
     * @lua NA
     */
    bool fuzzyEquals(const DPoint& target, float variance) const;

    /** Calculates distance between point an origin
     * @return float
     * @since v2.1.4
     * @lua NA
     */
    inline float getLength() const {
        return sqrtf(x*x + y*y);
    };

    /** Calculates the square length of a DPoint (not calling sqrt() )
     * @return float
     * @since v2.1.4
     * @lua NA
     */
    inline float getLengthSq() const {
        return dot(*this); //x*x + y*y;
    };

    /** Calculates the square distance between two points (not calling sqrt() )
     @return float
     @since v2.1.4
    */
    inline float getDistanceSq(const DPoint& other) const {
        return (*this - other).getLengthSq();
    };

    /** Calculates the distance between two points
     @return float
     @since v2.1.4
     */
    inline float getDistance(const DPoint& other) const {
        return (*this - other).getLength();
    };

    /** @returns the angle in radians between this vector and the x axis
     @since v2.1.4
    */
    inline float getAngle() const {
        return atan2f(y, x);
    };

    /** @returns the angle in radians between two vector directions
     @since v2.1.4
    */
    float getAngle(const DPoint& other) const;

    /** Calculates dot product of two points.
     @return float
     @since v2.1.4
     */
    inline float dot(const DPoint& other) const {
        return x*other.x + y*other.y;
    };

    /** Calculates cross product of two points.
     @return float
     @since v2.1.4
     */
    inline float cross(const DPoint& other) const {
        return x*other.y - y*other.x;
    };

    /** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
     @return DPoint
     @since v2.1.4
     */
    inline DPoint getPerp() const {
        return DPoint(-y, x);
    };

    /** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
     @return DPoint
     @since v2.1.4
     */
    inline DPoint getRPerp() const {
        return DPoint(y, -x);
    };

    /** Calculates the projection of this over other.
     @return DPoint
     @since v2.1.4
     */
    inline DPoint project(const DPoint& other) const {
        return other * (dot(other)/other.dot(other));
    };

    /** Complex multiplication of two points ("rotates" two points).
     @return DPoint vector with an angle of this.getAngle() + other.getAngle(),
     and a length of this.getLength() * other.getLength().
     @since v2.1.4
     */
    inline DPoint rotate(const DPoint& other) const {
        return DPoint(x*other.x - y*other.y, x*other.y + y*other.x);
    };

    /** Unrotates two points.
     @return DPoint vector with an angle of this.getAngle() - other.getAngle(),
     and a length of this.getLength() * other.getLength().
     @since v2.1.4
     */
    inline DPoint unrotate(const DPoint& other) const {
        return DPoint(x*other.x + y*other.y, y*other.x - x*other.y);
    };

    /** Returns point multiplied to a length of 1.
     * If the point is 0, it returns (1, 0)
     @return DPoint
     @since v2.1.4
     */
    inline DPoint normalize() const {
        float length = getLength();
        if(length == 0.) return DPoint(1.f, 0);
        return *this / getLength();
    };

    /** Linear Interpolation between two points a and b
     @returns
        alpha == 0 ? a
        alpha == 1 ? b
        otherwise a value between a..b
     @since v2.1.4
     */
    inline DPoint lerp(const DPoint& other, float alpha) const {
        return *this * (1.f - alpha) + other * alpha;
    };

    /** Rotates a point counter clockwise by the angle around a pivot
     @param pivot is the pivot, naturally
     @param angle is the angle of rotation ccw in radians
     @returns the rotated point
     @since v2.1.4
     */
    DPoint rotateByAngle(const DPoint& pivot, float angle) const;

    static inline DPoint forAngle(const float a)
    {
    	return DPoint(cosf(a), sinf(a));
    }
};

/**
 * @js NA
 */
class CC_DLL DSize
{
public:
    
    float width;
    float height;

public:
    
    DSize();
    
    DSize(float width, float height);
    /**
     * @lua NA
     */
    DSize(const DSize& other);
    /**
     * @lua NA
     */
    DSize(const DPoint& point);
    /**
     * @lua NA
     */
    DSize& operator= (const DSize& other);
    /**
     * @lua NA
     */
    DSize& operator= (const DPoint& point);
    /**
     * @lua NA
     */
    DSize operator+(const DSize& right) const;
    /**
     * @lua NA
     */
    DSize operator-(const DSize& right) const;
    /**
     * @lua NA
     */
    DSize operator*(float a) const;
    /**
     * @lua NA
     */
    DSize operator/(float a) const;
    /**
     * @lua NA
     */
    void setSize(float width, float height);
    /**
     * @lua NA
     */
    bool equals(const DSize& target) const;
};

/**
 * @js NA
 */
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

const DPoint DPointZero = DPoint();
const DSize DSizeZero = DSize();
const DRect DRectZero = DRect();

CC_DEPRECATED_ATTRIBUTE typedef DPoint CCPoint;
CC_DEPRECATED_ATTRIBUTE typedef DPoint CADipPoint;
CC_DEPRECATED_ATTRIBUTE typedef DPoint CCPointMake;
CC_DEPRECATED_ATTRIBUTE typedef DPoint ccp;
CC_DEPRECATED_ATTRIBUTE typedef DSize CCSize;
CC_DEPRECATED_ATTRIBUTE typedef DSize CADipSize;
CC_DEPRECATED_ATTRIBUTE typedef DSize CCSizeMake;
CC_DEPRECATED_ATTRIBUTE typedef DRect CCRect;
CC_DEPRECATED_ATTRIBUTE typedef DRect CADipRect;
CC_DEPRECATED_ATTRIBUTE typedef DRect CCRectMake;

#define CCPointZero DPointZero
#define CCSizeZero DSizeZero
#define CCRectZero DRectZero
#define CADipPointZero DPointZero
#define CADipSizeZero DSizeZero
#define CADipRectZero DRectZero


NS_CC_END

#endif // __CCGEMETRY_H__

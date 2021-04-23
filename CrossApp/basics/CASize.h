

#ifndef __DSize_H__
#define __DSize_H__

#include "platform/CCPlatformMacros.h"
#include "CAObject.h"
#include <math.h>
#include <float.h>

NS_CC_BEGIN

class CC_DLL DPoint;
class CC_DLL DSize
{
public:
    
    float width;
    float height;

public:
    
    DSize();
    
    DSize(float width, float height);

    DSize(const DSize& other);

    DSize(const DPoint& point);

    DSize& operator= (const DSize& other);

    DSize& operator= (const DPoint& point);

    DSize operator+(const DSize& right) const;

    DSize operator-(const DSize& right) const;

    DSize operator*(float a) const;

    DSize operator/(float a) const;

    void setSize(float width, float height);

    bool equals(const DSize& target) const;
    
    bool fuzzyEquals(const DSize& target, float variance) const;
};

const DSize DSizeZero = DSize();

NS_CC_END

#endif // __DSize_H__



#ifndef __CACOLOR_H__
#define __CACOLOR_H__

#include "CAObject.h"
#include <math.h>
#include "CCGL.h"

NS_CC_BEGIN

class CC_DLL CAColor4F;

class CC_DLL CAColor4B
{
public:
    
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;

public:
    
    CAColor4B();
    
    CAColor4B(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a);
    
    explicit CAColor4B(unsigned int rgba);
    
    CAColor4B(const CAColor4B& other);
    
    explicit CAColor4B(const CAColor4F& color);
    
    inline void set(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
    
    void setUInt32(unsigned int rgba);
    
    unsigned int getUInt32() const;
    
    bool operator==(const CAColor4B& right) const;
    bool operator==(const CAColor4F& right) const;
    bool operator!=(const CAColor4B& right) const;
    bool operator!=(const CAColor4F& right) const;
    
    CAColor4B operator*(float var) const;
    
    static const CAColor4B WHITE;
    static const CAColor4B YELLOW;
    static const CAColor4B BLUE;
    static const CAColor4B GREEN;
    static const CAColor4B RED;
    static const CAColor4B MAGENTA;
    static const CAColor4B BLACK;
    static const CAColor4B ORANGE;
    static const CAColor4B GRAY;
    static const CAColor4B CLEAR;
};

class CC_DLL CAColor4F
{
public:
    
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
    
public:
    
    CAColor4F();
    
    CAColor4F(float _r, float _g, float _b, float _a);
    
    CAColor4F(const CAColor4F& other);
    
    explicit CAColor4F(const CAColor4B& color);
    
    inline void set(float _r, float _g, float _b, float _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
    
    CAColor4F operator*(float var) const;
    
    bool operator==(const CAColor4F& right) const;
    bool operator==(const CAColor4B& right) const;
    bool operator!=(const CAColor4F& right) const;
    bool operator!=(const CAColor4B& right) const;
    
    static const CAColor4F WHITE;
    static const CAColor4F YELLOW;
    static const CAColor4F BLUE;
    static const CAColor4F GREEN;
    static const CAColor4F RED;
    static const CAColor4F MAGENTA;
    static const CAColor4F BLACK;
    static const CAColor4F ORANGE;
    static const CAColor4F GRAY;
    static const CAColor4F CLEAR;
};


#define ccc4(R, G, B, A)                CAColor4B(R, G, B, A)
#define ccc4Int(RGBA)                   CAColor4B(RGBA)
#define getUIntFormColor4B(_COLOR_)     _COLOR_.getUInt()
#define getIntFormColor4B(_COLOR_)      (int)(_COLOR_.getUInt())
#define ccc4Mult(_COLOR_, _VAR_)        (_COLOR_ * _VAR_)
#define CAColor4BEqual(_A_, _B_)        (_A_ == _B_)

CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_white   = {255, 255, 255, 255};
CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_yellow  = {255, 255,   0, 255};
CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_green   = {  0, 255,   0, 255};
CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_blue    = {  0,   0, 255, 255};
CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_red     = {255,   0,   0, 255};
CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_magenta = {255,   0, 255, 255};
CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_black   = {  0,   0,   0, 255};
CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_orange  = {255, 127,   0, 255};
CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_gray    = {166, 166, 166, 255};
CC_DEPRECATED_ATTRIBUTE static const CAColor4B  CAColor_clear   = {255, 255, 255,   0};

NS_CC_END

#endif // __CACOLOR_H__



#ifndef __CALAYOUT_H__
#define __CALAYOUT_H__

#include "basics/CAObject.h"
#include <math.h>
#include <float.h>

NS_CC_BEGIN

class CC_DLL DHorizontalLayout
{
public:
    
    typedef enum
    {
        L_R = 0,
        L_W,
        L_C,
        R_W,
        R_C,
        W_C = 5
    }
    Type;
    
public:
    
    float left;
    
    float right;
    
    float width;
    
    float center;
    
public:
    
    DHorizontalLayout();
    
    DHorizontalLayout(const DHorizontalLayout& other);
    
    DHorizontalLayout(float var1, float var2, const Type& type);
    
    DHorizontalLayout& operator= (const DHorizontalLayout& other);
    
    bool equals(const DHorizontalLayout& target) const;
};

/**
 * @js NA
 */
class CC_DLL DVerticalLayout
{
public:
    
    typedef enum
    {
        T_B = 0,
        T_H,
        T_C,
        B_H,
        B_C,
        H_C
    }
    Type;
    
public:
    
    float top;
    
    float bottom;
    
    float height;
    
    float center;
    
public:
    
    DVerticalLayout();
    
    DVerticalLayout(const DVerticalLayout& other);
    
    DVerticalLayout(float var1, float var2, const Type& type);
    
    DVerticalLayout& operator= (const DVerticalLayout& other);
    
    bool equals(const DVerticalLayout& target) const;
};

/**
 * @js NA
 */
class CC_DLL DLayout
{
public:
    
    DHorizontalLayout horizontal;
    
    DVerticalLayout vertical;
    
public:
    
    DLayout();
    
    DLayout(const DLayout& other);
    
    DLayout(const DHorizontalLayout& horizontal, const DVerticalLayout& vertical);
    
    DLayout& operator= (const DLayout& other);
    
    bool equals(const DLayout& target) const;
};

static inline DHorizontalLayout DHorizontalLayout_L_R(float left, float right)
{
    return DHorizontalLayout(left, right, DHorizontalLayout::L_R);
}

static inline DHorizontalLayout DHorizontalLayout_L_W(float left, float width)
{
    return DHorizontalLayout(left, width, DHorizontalLayout::L_W);
}

static inline DHorizontalLayout DHorizontalLayout_L_C(float left, float center)
{
    return DHorizontalLayout(left, center, DHorizontalLayout::L_C);
}

static inline DHorizontalLayout DHorizontalLayout_R_W(float right, float width)
{
    return DHorizontalLayout(right, width, DHorizontalLayout::R_W);
}

static inline DHorizontalLayout DHorizontalLayout_R_C(float right, float center)
{
    return DHorizontalLayout(right, center, DHorizontalLayout::R_C);
}

static inline DHorizontalLayout DHorizontalLayout_W_C(float width, float center)
{
    return DHorizontalLayout(width, center, DHorizontalLayout::W_C);
}


static inline DVerticalLayout DVerticalLayout_T_B(float top, float bottom)
{
    return DVerticalLayout(top, bottom, DVerticalLayout::T_B);
}

static inline DVerticalLayout DVerticalLayout_T_H(float top, float height)
{
    return DVerticalLayout(top, height, DVerticalLayout::T_H);
}

static inline DVerticalLayout DVerticalLayout_T_C(float top, float center)
{
    return DVerticalLayout(top, center, DVerticalLayout::T_C);
}

static inline DVerticalLayout DVerticalLayout_B_H(float bottom, float height)
{
    return DVerticalLayout(bottom, height, DVerticalLayout::B_H);
}

static inline DVerticalLayout DVerticalLayout_B_C(float bottom, float center)
{
    return DVerticalLayout(bottom, center, DVerticalLayout::B_C);
}

static inline DVerticalLayout DVerticalLayout_H_C(float height, float center)
{
    return DVerticalLayout(height, center, DVerticalLayout::H_C);
}

const DHorizontalLayout DHorizontalLayouttZero  = DHorizontalLayout();
const DVerticalLayout   DVerticalLayoutZero     = DVerticalLayout();
const DLayout           DLayoutZero             = DLayout();
const DHorizontalLayout DHorizontalLayoutFill   = DHorizontalLayout_L_R(0, 0);
const DVerticalLayout   DVerticalLayoutFill     = DVerticalLayout_T_B(0, 0);
const DLayout           DLayoutFill             = DLayout(DHorizontalLayoutFill, DVerticalLayoutFill);

NS_CC_END

#endif // __CALAYOUT_H__

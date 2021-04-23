

#ifndef __CALAYOUT_H__
#define __CALAYOUT_H__

#include "basics/CAObject.h"
#include <math.h>
#include <float.h>

NS_CC_BEGIN

class CC_DLL DHorizontalLayout
{
public:
    
    enum class Type
    {
        L_R = 0,
        L_W,
        R_W,
        W_C,
        NW_C = 4
    };
    
public:
    
    float left;
    
    float right;
    
    float width;
    
    float center;
    
    float normalizedWidth;
    
public:
    
    DHorizontalLayout();
    
    DHorizontalLayout(const DHorizontalLayout& other);
    
    DHorizontalLayout(float var1, float var2, DHorizontalLayout::Type type);
    
    DHorizontalLayout& operator= (const DHorizontalLayout& other);
    
    bool equals(const DHorizontalLayout& target) const;
};

/**
 * @js NA
 */
class CC_DLL DVerticalLayout
{
public:
    
    enum class Type
    {
        T_B = 0,
        T_H,
        B_H,
        H_C,
        NH_C = 4
    };
    
public:
    
    float top;
    
    float bottom;
    
    float height;
    
    float center;
    
    float normalizedHeight;
    
public:
    
    DVerticalLayout();
    
    DVerticalLayout(const DVerticalLayout& other);
    
    DVerticalLayout(float var1, float var2, DVerticalLayout::Type type);
    
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
    return DHorizontalLayout(left, right, DHorizontalLayout::Type::L_R);
}

static inline DHorizontalLayout DHorizontalLayout_L_W(float left, float width)
{
    return DHorizontalLayout(left, width, DHorizontalLayout::Type::L_W);
}

static inline DHorizontalLayout DHorizontalLayout_R_W(float right, float width)
{
    return DHorizontalLayout(right, width, DHorizontalLayout::Type::R_W);
}

static inline DHorizontalLayout DHorizontalLayout_W_C(float width, float center)
{
    return DHorizontalLayout(width, center, DHorizontalLayout::Type::W_C);
}

static inline DHorizontalLayout DHorizontalLayout_NW_C(float normalizedWidth, float center)
{
    return DHorizontalLayout(normalizedWidth, center, DHorizontalLayout::Type::NW_C);
}



static inline DVerticalLayout DVerticalLayout_T_B(float top, float bottom)
{
    return DVerticalLayout(top, bottom, DVerticalLayout::Type::T_B);
}

static inline DVerticalLayout DVerticalLayout_T_H(float top, float height)
{
    return DVerticalLayout(top, height, DVerticalLayout::Type::T_H);
}

static inline DVerticalLayout DVerticalLayout_B_H(float bottom, float height)
{
    return DVerticalLayout(bottom, height, DVerticalLayout::Type::B_H);
}

static inline DVerticalLayout DVerticalLayout_H_C(float height, float center)
{
    return DVerticalLayout(height, center, DVerticalLayout::Type::H_C);
}

static inline DVerticalLayout DVerticalLayout_NH_C(float normalizedHeight, float center)
{
    return DVerticalLayout(normalizedHeight, center, DVerticalLayout::Type::NH_C);
}

const DHorizontalLayout DHorizontalLayoutZero  = DHorizontalLayout();
const DVerticalLayout   DVerticalLayoutZero     = DVerticalLayout();
const DLayout           DLayoutZero             = DLayout();
const DHorizontalLayout DHorizontalLayoutFill   = DHorizontalLayout_L_R(0, 0);
const DVerticalLayout   DVerticalLayoutFill     = DVerticalLayout_T_B(0, 0);
const DLayout           DLayoutFill             = DLayout(DHorizontalLayoutFill, DVerticalLayoutFill);

NS_CC_END

#endif // __CALAYOUT_H__

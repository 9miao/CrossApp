

#include "basics/CAColor.h"

// implementation of DPoint
NS_CC_BEGIN

const CAColor4B CAColor4B::WHITE  (255, 255, 255, 255);
const CAColor4B CAColor4B::YELLOW (255, 255,   0, 255);
const CAColor4B CAColor4B::GREEN  (  0, 255,   0, 255);
const CAColor4B CAColor4B::BLUE   (  0,   0, 255, 255);
const CAColor4B CAColor4B::RED    (255,   0,   0, 255);
const CAColor4B CAColor4B::MAGENTA(255,   0, 255, 255);
const CAColor4B CAColor4B::BLACK  (  0,   0,   0, 255);
const CAColor4B CAColor4B::ORANGE (255, 127,   0, 255);
const CAColor4B CAColor4B::GRAY   (166, 166, 166, 255);
const CAColor4B CAColor4B::CLEAR  (255, 255, 255,   0);

const CAColor4F CAColor4F::WHITE  (    1,     1,     1, 1);
const CAColor4F CAColor4F::YELLOW (    1,     1,     0, 1);
const CAColor4F CAColor4F::GREEN  (    0,     1,     0, 1);
const CAColor4F CAColor4F::BLUE   (    0,     0,     1, 1);
const CAColor4F CAColor4F::RED    (    1,     0,     0, 1);
const CAColor4F CAColor4F::MAGENTA(    1,     0,     1, 1);
const CAColor4F CAColor4F::BLACK  (    0,     0,     0, 1);
const CAColor4F CAColor4F::ORANGE (    1,  0.5f,     0, 1);
const CAColor4F CAColor4F::GRAY   (0.65f, 0.65f, 0.65f, 1);
const CAColor4F CAColor4F::CLEAR  (    1,     1,     1, 0);

CAColor4B::CAColor4B()
: r(0)
, g(0)
, b(0)
, a(0)
{}

CAColor4B::CAColor4B(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a)
: r(_r)
, g(_g)
, b(_b)
, a(_a)
{}

CAColor4B::CAColor4B(unsigned int rgba)
{
    setUInt32(rgba);
}

CAColor4B::CAColor4B(const CAColor4B& other)
: r(other.r)
, g(other.g)
, b(other.b)
, a(other.a)
{}


CAColor4B::CAColor4B(const CAColor4F& color)
: r(color.r * 255)
, g(color.g * 255)
, b(color.b * 255)
, a(color.a * 255)
{}

void CAColor4B::setUInt32(unsigned int rgba)
{
    b = rgba % 0x100;
    rgba /= 0x100;
    g = rgba % 0x100;
    rgba /= 0x100;
    r = rgba % 0x100;
    rgba /= 0x100;
    a = rgba % 0x100;
}

unsigned int CAColor4B::getUInt32() const
{
    return (b + g * 0x100 + r * 0x10000 + a * 0x1000000);
}

bool CAColor4B::operator==(const CAColor4B& right) const
{
    return (r == right.r && g == right.g && b == right.b && a == right.a);
}

bool CAColor4B::operator==(const CAColor4F& right) const
{
    return (*this == CAColor4B(right));
}

bool CAColor4B::operator!=(const CAColor4B& right) const
{
    return !(*this == right);
}

bool CAColor4B::operator!=(const CAColor4F& right) const
{
    return !(*this == right);
}

CAColor4B CAColor4B::operator*(float var) const
{
    CAColor4B color;
    color.r = static_cast<GLubyte>(r * var);
    color.g = static_cast<GLubyte>(g * var);
    color.b = static_cast<GLubyte>(b * var);
    color.a = a;
    return color;
}




CAColor4F::CAColor4F()
: r(0)
, g(0)
, b(0)
, a(0)
{}

CAColor4F::CAColor4F(float _r, float _g, float _b, float _a)
: r(_r)
, g(_g)
, b(_b)
, a(_a)
{}


CAColor4F::CAColor4F(const CAColor4F& other)
: r(other.r)
, g(other.g)
, b(other.b)
, a(other.a)
{}


CAColor4F::CAColor4F(const CAColor4B& color)
: r(color.r / 255.f)
, g(color.g / 255.f)
, b(color.b / 255.f)
, a(color.a / 255.f)
{}

bool CAColor4F::operator==(const CAColor4F& right) const
{
    return (r == right.r && g == right.g && b == right.b && a == right.a);
}

bool CAColor4F::operator==(const CAColor4B& right) const
{
    return (*this == CAColor4F(right));
}

bool CAColor4F::operator!=(const CAColor4F& right) const
{
    return !(*this == right);
}

bool CAColor4F::operator!=(const CAColor4B& right) const
{
    return !(*this == right);
}

CAColor4F CAColor4F::operator*(float var) const
{
    CAColor4F color;
    color.r = r * var;
    color.g = g * var;
    color.b = b * var;
    color.a = a;
    return color;
}



NS_CC_END

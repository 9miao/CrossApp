#ifndef CAFont_h__
#define CAFont_h__

#include "ccTypes.h"
#include "basics/CAObject.h"

using std::string;

NS_CC_BEGIN


enum class CAVerticalTextAlignment
{
    Top = 0,
    Center = 1,
    Bottom = 2,
};

enum class CATextAlignment
{
    Left = 0,
    Center = 1,
    Right = 2,
};

const CAColor4B CAFontShadowDefaultColor = CAColor4B(0, 0, 0, 88);

class CC_DLL CAFontShadow
{
public:

    bool       shadowEnabled;
    DSize      shadowOffset;
    float      shadowBlur;
    CAColor4B  shadowColor;
    
public:
    
    CAFontShadow()
    : shadowEnabled(false)
    , shadowOffset(DSizeZero)
    , shadowBlur(0)
    , shadowColor(CAFontShadowDefaultColor)
    {
        
    }
};

class CC_DLL CAFontStroke
{
public:

    bool        strokeEnabled;
    CAColor4B   strokeColor;
    float       strokeSize;
    
public:
    
    CAFontStroke()
    : strokeEnabled(false)
    , strokeColor(CAColor4B::BLACK)
    , strokeSize(0)
    {}
    
};

class CC_DLL CAFont
{
public:

    bool                    bold;
    
    bool                    underLine;
    /*
    CAColor4B               underLineColor;
    */
    bool                    deleteLine;
    /*
    CAColor4B               deleteLineColor;
    */
    bool                    italics;
    
    float                   italicsValue;
    
    bool                    wordWrap;
    
    std::string             fontName;
    
    float                   fontSize;
    
    float                   lineSpacing;
    
    CAColor4B               color;

    CAFontShadow            shadow;
    
    CAFontStroke            stroke;
    
public:
    
    CAFont()
    : bold(false)
    , underLine(false)
    /*
    , underLineColor(CAColor4B::CLEAR)
     */
    , deleteLine(false)
    /*
    , deleteLineColor(CAColor4B::CLEAR)
     */
    , italics(false)
    , italicsValue(0.25f)
    , wordWrap(true)
    , fontSize(24)
    , fontName("")
    , lineSpacing(0)
    , color(CAColor4B::BLACK)
    {
        
    }
    
    CAFont(const CAFont& other);
    
    CAFont& operator= (const CAFont& other);
};

NS_CC_END

#endif // CAFont_h__

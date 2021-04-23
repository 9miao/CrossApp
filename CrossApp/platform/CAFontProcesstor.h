
#ifndef __CAFontProcesstor_H__
#define __CAFontProcesstor_H__

#include "CADensityDpi.h"
#include "view/CARichLabel.h"
NS_CC_BEGIN

class CAImage;
class CC_DLL CAFontProcesstor
{
public:
    
    static CAImage* imageForRichText(const std::vector<CARichLabel::Element>& elements, DSize& dimensions, CATextAlignment textAlignment);
    
    static CAImage* imageForText(const std::string& text, CAFont font, DSize& dimensions, CATextAlignment textAlignment);
    
    static float heightForFont(const CAFont& font);
    
    static float heightForTextAtWidth(const std::string& text, const CAFont& font, float width);
    
    static float widthForTextAtOneLine(const std::string& text, const CAFont& font);
};
NS_CC_END
#endif // __CAFontProcesstor_H__

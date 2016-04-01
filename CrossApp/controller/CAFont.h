#ifndef CAFont_h__
#define CAFont_h__

#include "ccTypes.h"
#include "basics/CAObject.h"

using std::string;

NS_CC_BEGIN

class CC_DLL CAFont
{
public:

    bool bold;
    
    bool underLine;
    
    bool deleteLine;
    
    bool italics;
    
    unsigned int fontSize;
    
    CAColor4B color;
    
    std::string fontName;
    
    char hyperlink;
    
public:
    
    CAFont();
    
    CAFont(const CAFont& other);
    
    CAFont& operator= (const CAFont& other);
};


#define g_CAFontCache CAFontCache::sharedStyleCache()

NS_CC_END

#endif // CAFont_h__

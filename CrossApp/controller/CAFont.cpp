#include "CAFont.h"
#include "platform/CAFTFontCache.h"

NS_CC_BEGIN

CAFont::CAFont()
: fontSize(24)
, fontName("")
, color(CAColor_black)
, hyperlink(false)
, italics(false)
, bold(false)
, underLine(false)
, deleteLine(false)
{
    
}

CAFont::CAFont(const CAFont& other)
: fontSize(other.fontSize)
, fontName(other.fontName)
, color(other.color)
, hyperlink(other.hyperlink)
, italics(other.italics)
, bold(other.bold)
, underLine(other.underLine)
, deleteLine(other.deleteLine)
{

}

CAFont& CAFont::operator= (const CAFont& other)
{
    fontSize = other.fontSize;
    fontName = other.fontName;
    color = other.color;
	hyperlink = other.hyperlink;
    italics = other.italics;
    bold = other.bold;
    underLine = other.underLine;
    deleteLine = other.deleteLine;
    return *this;
}

NS_CC_END
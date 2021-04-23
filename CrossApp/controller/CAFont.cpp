#include "CAFont.h"

NS_CC_BEGIN

CAFont::CAFont(const CAFont& other)
: bold(other.bold)
, underLine(other.underLine)
//, underLineColor(other.underLineColor)
, deleteLine(other.deleteLine)
//, deleteLineColor(other.deleteLineColor)
, italics(other.italics)
, italicsValue(other.italicsValue)
, wordWrap(other.wordWrap)
, fontSize(other.fontSize)
, fontName(other.fontName)
, lineSpacing(other.lineSpacing)
, color(other.color)
, shadow(other.shadow)
, stroke(other.stroke)
{

}

CAFont& CAFont::operator= (const CAFont& other)
{
    this->bold                  = other.bold;
    this->underLine             = other.underLine;
    //this->underLineColor        = other.underLineColor;
    this->deleteLine            = other.deleteLine;
    //this->deleteLineColor       = other.deleteLineColor;
    this->italics               = other.italics;
    this->italicsValue          = other.italicsValue;
    this->wordWrap              = other.wordWrap;
    this->fontSize              = other.fontSize;
    this->fontName              = other.fontName;
    this->lineSpacing           = other.lineSpacing;
    this->color                 = other.color;
    this->shadow                = other.shadow;
    this->stroke                = other.stroke;
    return *this;
}

NS_CC_END

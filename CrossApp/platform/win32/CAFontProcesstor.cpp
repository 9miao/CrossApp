

#include "../CAFontProcesstor.h"
#include "CAFTFontCache.h"
#include "CAFreeTypeFont.h"
#include "CAFTFontCache.h"
#include "CAFTRichFont.h"
NS_CC_BEGIN

CAImage* CAFontProcesstor::imageForRichText(const std::vector<CARichLabel::Element>& elements, DSize& dimensions, CATextAlignment textAlignment)
{
	CAImage* image = nullptr;
	if (!elements.empty())
	{
		image = g_AFTRichFont.initWithString(elements, dimensions, elements.front().font.color, elements.front().font.color);
		dimensions = image->getContentSize();
	}
	return image;
}


CAImage* CAFontProcesstor::imageForText(const std::string& text, const CAFont& font, DSize& dimensions, CATextAlignment textAlignment)
{
	CAImage* image =  g_AFTFontCache.initWithString(text.c_str(),
		font.color,
		font.fontName.c_str(),
		font.fontSize,
		dimensions.width,
		dimensions.height,
		textAlignment,
		CAVerticalTextAlignment::Center,
		font.wordWrap, 
		font.lineSpacing,
		font.bold,
		font.italics,
		font.underLine, 
		font.deleteLine,
		nullptr);

	if (image)
	{
		dimensions = image->getContentSize();
	}
		
	return image;
}

float CAFontProcesstor::heightForFont(const CAFont& font)
{
	return (float)g_AFTFontCache.getFontHeight(font.fontName.c_str(), font.fontSize);
}

float CAFontProcesstor::heightForTextAtWidth(const std::string& text, const CAFont& font, float width)
{
    
	return (float)g_AFTFontCache.getStringHeight(font.fontName.c_str(), font.fontSize, text, width, font.lineSpacing, font.wordWrap);
}

float CAFontProcesstor::widthForTextAtOneLine(const std::string& text, const CAFont& font)
{
    
	return (float)g_AFTFontCache.getStringWidth(font.fontName.c_str(), font.fontSize, text);
}
NS_CC_END

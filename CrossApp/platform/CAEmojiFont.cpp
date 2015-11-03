#include "CAEmojiFont.h"
#include "basics/CAApplication.h"
#include "platform/CCFileUtils.h"
#include "support/ccUTF8.h"
#include "CATempTypeFont.h"
#include <string.h>
#include "CADevice.h"

using namespace std;

NS_CC_BEGIN


CAEmojiFont::CAEmojiFont()
: m_CurFontFace(NULL)
{
	FT_Init_FreeType(&m_gFtLibrary);
	initEmojiFont();
}

CAEmojiFont::~CAEmojiFont()
{
	finiEmojiFont();
	FT_Done_FreeType(m_gFtLibrary);
}

CAEmojiFont* CAEmojiFont::getInstance()
{
	static CAEmojiFont ins;
	return &ins;
}

unsigned char* CAEmojiFont::loadEmojiFontBuffer(unsigned long& size)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	const char* fontName = "";

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    
	const char* fontName = "/System/Library/Fonts/Apple Color Emoji.ttf";

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    const char* fontName = NULL;
    float version = atof(CADevice::getSystemVersionWithIOS());
    
    if (version >= 9.0f)
    {
        fontName = "/System/Library/Fonts/Core/AppleColorEmoji@2x.ttf";
    }
    else if (version >= 8.0f)
    {
        fontName = "/System/Library/Fonts/Core/AppleColorEmoji@2x.ttf";
    }
    else
    {
        fontName = "/System/Library/Fonts/Cache/AppleColorEmoji@2x.ttf";
    }

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	const char* fontName = "/system/fonts/NotoColorEmoji.ttf";
    
#endif

	return CCFileUtils::sharedFileUtils()->getFileData(fontName, "rb", &size);
}

bool CAEmojiFont::initEmojiFont()
{
	unsigned long size = 0;
	unsigned char* pBuffer = loadEmojiFontBuffer(size);
	if (pBuffer == NULL)
		return false;

	FT_Error error = FT_New_Memory_Face(m_gFtLibrary, pBuffer, size, 0, &m_CurFontFace);
	if (error)
		return false;

	FT_Select_Charmap(m_CurFontFace, FT_ENCODING_UNICODE);

	return true;
}

void CAEmojiFont::finiEmojiFont()
{
	FT_Done_Face(m_CurFontFace);
}

bool CAEmojiFont::isEmojiCodePoint(unsigned int codepoint)
{
	uint32_t glyph_index = FT_Get_Char_Index(m_CurFontFace, codepoint);
	return (glyph_index > 0);
}

CAImage* CAEmojiFont::getEmojiImage(unsigned int codepoint, unsigned long nFontSize)
{
	int best_match = 0;
	int diff = std::abs((int)(nFontSize - m_CurFontFace->available_sizes[0].width));
	for (int i = 1; i < m_CurFontFace->num_fixed_sizes; ++i) {
		int ndiff = std::abs((int)(nFontSize - m_CurFontFace->available_sizes[i].width));
		if (ndiff < diff) { best_match = i; diff = ndiff; }
	}
	FT_Select_Size(m_CurFontFace, best_match);

	uint32_t glyph_index = FT_Get_Char_Index(m_CurFontFace, codepoint);
	if (glyph_index == 0)
		return NULL;

	FT_Error error = FT_Load_Glyph(m_CurFontFace, glyph_index, FT_LOAD_COLOR);
	if (error)
		return NULL;

	error = FT_Render_Glyph(m_CurFontFace->glyph, FT_RENDER_MODE_NORMAL);
	if (error)
		return NULL;

	int w = (m_CurFontFace->glyph->advance.x >> 6);
	int h = static_cast<uint32_t>(m_CurFontFace->glyph->metrics.height >> 6);

	int size = w * h * 4;
	std::vector<uint8_t> bitmap;
	bitmap.resize(size);
	bitmap.assign(size, 0x00);

	FT_GlyphSlot slot = m_CurFontFace->glyph;
	uint8_t* src = slot->bitmap.buffer;
	// FIXME: Should use metrics for drawing. (e.g. calculate baseline)
	int yoffset = h - slot->bitmap.rows;
	for (int y = 0; y < slot->bitmap.rows; ++y) {
		uint8_t* dest = &bitmap[((y + yoffset) * w) * 4];
		for (int x = 0; x < slot->bitmap.width; ++x) {
			uint8_t b = *src++, g = *src++, r = *src++, a = *src++;
			*dest++ = r; *dest++ = g; *dest++ = b; *dest++ = a;
		}
	}

	CAImage* image = new CAImage();
	if (!image->initWithRawData(&bitmap[0], CAImage::PixelFormat_RGBA8888, w, h))
	{
		delete image;
		return NULL;
	}
	image->autorelease();
	return image;
}

NS_CC_END


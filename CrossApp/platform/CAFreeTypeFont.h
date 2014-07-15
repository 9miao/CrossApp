//
//  CCSTLContainer.h
//  CrossApp
//
//  Created by Zhujian on 14-7-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CC_PLATFORM_CAFREETYPEFONT_H
#define __CC_PLATFORM_CAFREETYPEFONT_H

#include "platform/CCCommon.h"
#include "platform/CCImage.h"
#include "view/CAImageView.h"
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>


NS_CC_BEGIN

typedef struct TGlyph_
{
	FT_UInt    index;  // glyph index
    FT_Vector  pos;    // glyph origin on the baseline
	FT_Glyph   image;  // glyph image
} TGlyph, *PGlyph;

typedef struct FontBufferInfo
{
	unsigned char*  pBuffer;  
	unsigned long  size;  
} FontBufferInfo;

typedef struct FTWordInfo
{
	std::vector<TGlyph> glyphs; // glyphs for the word
	FT_BBox             bbox;   // bounding box containing all of the glyphs in the word
} FTWordInfo;


typedef struct FTLineInfo
{
	std::vector<TGlyph> glyphs;     // glyphs for the line text
	FT_BBox             bbox;       // bounding box containing all of the glyphs in the line
    unsigned int        width;      // width of the line     
    FT_Vector           pen;        // current pen position
} FTLineInfo;


class CC_DLL CAFreeTypeFont
{
public:

	CAFreeTypeFont();
	~CAFreeTypeFont();

	static int getFontHeight(const char* pFontName, unsigned long nSize);

	CAImage* initWithString(const char* pText, const char* pFontName, int nSize, int width, int height, 
		CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment);

private:
	unsigned char* loadFont(const char *pFontName, unsigned long *size);
	unsigned char* getBitmap(CCImage::ETextAlign eAlignMask, int* outWidth, int* outHeight);

	FT_Error initGlyphs(const char* text);
	FT_Error initWordGlyphs(std::vector<TGlyph>& glyphs, const std::string& text, FT_Vector& pen);

	void compute_bbox(std::vector<TGlyph>& glyphs, FT_BBox  *abbox);

    void drawText(FTLineInfo* pInfo, unsigned char* pBuffer, FT_Vector *pen);

    void draw_bitmap(unsigned char* pBuffer, FT_Bitmap*  bitmap,FT_Int x,FT_Int y);

    void initWords(const char* text);

    FT_Vector getPenForAlignment(FTLineInfo* pInfo, CCImage::ETextAlign eAlignMask, int lineNumber, int totalLines);

    FT_Error addWord(const std::string& word);
    void newLine();
    void endLine();

    const std::string m_space;
	FT_Face			m_face;
	std::vector<FTLineInfo*> m_lines;

    int             m_inWidth;      // requested width of text box
    int             m_inHeight;     // requested height of text box
    int             m_width;        // final bitMap width
    int             m_height;       // final bitMap height
    int             m_textWidth;    // width of text text after word wrapping and line breaks    
    int             m_textHeight;   // height of text text after word wrapping and line breaks  
    int             m_lineHeight;   // height of a line for the font size
    int             m_windowWidth;  // the width of the window

    FTLineInfo*     m_currentLine;  // the current line object to add words to.
};

NS_CC_END

#endif
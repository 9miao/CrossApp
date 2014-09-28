//
//  CAFreeTypeFont.h
//  CrossApp
//
//  Created by Zhujian on 14-7-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CC_PLATFORM_CAFREETYPEFONT_H
#define __CC_PLATFORM_CAFREETYPEFONT_H

#include "platform/CCCommon.h"
#include "platform/CCImage.h"
#include "images/CAImage.h"
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


typedef struct _TextAttribute
{
    int charSize;
    float charlength;
    
}TextAttribute;

typedef struct _TextViewLineInfo
{
	std::vector<TextAttribute> TextAttrVect;
	int iStartCharPos;
	int iEndCharPos;
    
}TextViewLineInfo;

typedef struct TGlyph_
{
	FT_UInt    index;  // glyph index
    FT_Vector  pos;    // glyph origin on the baseline
	FT_Glyph   image;  // glyph image
	FT_ULong   c;
} TGlyph, *PGlyph;

typedef struct FontBufferInfo
{
	unsigned char*  pBuffer;  
	unsigned long  size;  
} FontBufferInfo;

typedef struct FTLineInfo
{
	std::vector<TGlyph> glyphs;     // glyphs for the line text
	FT_BBox             bbox;       // bounding box containing all of the glyphs in the line
    unsigned int        width;      // width of the line     
    FT_Vector           pen;        // current pen position
} FTLineInfo;


class CC_DLL CAFreeTypeFont
{
	friend class CAFTFontCache;
public:

	CAFreeTypeFont();
	virtual ~CAFreeTypeFont();

	CAImage* initWithString(const char* pText, const char* pFontName, int nSize, int inWidth, int inHeight, 
		CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment, bool bWordWrap=true, int iLineSpacing = 0);

	CAImage* initWithStringEx(const char* pText, const char* pFontName, int nSize, int inWidth, int inHeight, std::vector<TextViewLineInfo>& linesText, bool bWordWrap=true);

	static void destroyAllFontBuff();
protected:
	bool initFreeTypeFont(const char* pFontName, unsigned long nSize);
	void finiFreeTypeFont();
	unsigned char* loadFont(const char *pFontName, unsigned long *size);
	unsigned char* getBitmap(CCImage::ETextAlign eAlignMask, int* outWidth, int* outHeight);
	int getFontHeight();
	int getStringWidth(const std::string& text);
	int getStringHeight(const std::string& text, int iLimitWidth, int iLineSpace, bool bWordWrap);
	void destroyAllLines();

	FT_Error initGlyphs(const char* text);
	FT_Error initGlyphsLine(const std::string& line);
	FT_Error initWordGlyphs(std::vector<TGlyph>& glyphs, const std::string& text, FT_Vector& pen);
	FT_Error initTextView(const char* pText, std::vector<TextViewLineInfo>& linesText);
	
	void compute_bbox(std::vector<TGlyph>& glyphs, FT_BBox  *abbox);
	void compute_bbox2(TGlyph& glyph, FT_BBox& bbox);

    void drawText(FTLineInfo* pInfo, unsigned char* pBuffer, FT_Vector *pen);

    void draw_bitmap(unsigned char* pBuffer, FT_Bitmap*  bitmap,FT_Int x,FT_Int y);

    FT_Vector getPenForAlignment(FTLineInfo* pInfo, CCImage::ETextAlign eAlignMask, int lineNumber, int totalLines);

    void calcuMultiLines(std::vector<TGlyph>& glyphs);
    
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
	int				m_lineSpacing;
    int             m_windowWidth;  // the width of the window

    FTLineInfo*     m_currentLine;  // the current line object to add words to.

	bool m_bWordWrap;
};

NS_CC_END

#endif
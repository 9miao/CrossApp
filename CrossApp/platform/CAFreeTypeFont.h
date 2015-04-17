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
	FT_Bool	   isOpenType;
} TGlyph, *PGlyph;

typedef struct FontBufferInfo
{
	unsigned char*  pBuffer;  
	unsigned long  size;
	int face_index;
	bool isOpenTypeFont;
} FontBufferInfo;

typedef struct FTLineInfo
{
	std::vector<TGlyph> glyphs;     // glyphs for the line text
	FT_BBox             bbox;       // bounding box containing all of the glyphs in the line
    unsigned int        width;      // width of the line     
    FT_Vector           pen;        // current pen position
	bool				includeRet;
} FTLineInfo;

typedef enum
{
    kAlignCenter        = 0x33, ///< Horizontal center and vertical center.
    kAlignTop           = 0x13, ///< Horizontal center and vertical top.
    kAlignTopRight      = 0x12, ///< Horizontal right and vertical top.
    kAlignRight         = 0x32, ///< Horizontal right and vertical center.
    kAlignBottomRight   = 0x22, ///< Horizontal right and vertical bottom.
    kAlignBottom        = 0x23, ///< Horizontal center and vertical bottom.
    kAlignBottomLeft    = 0x21, ///< Horizontal left and vertical bottom.
    kAlignLeft          = 0x31, ///< Horizontal left and vertical center.
    kAlignTopLeft       = 0x11, ///< Horizontal left and vertical top.
}ETextAlign;

class CC_DLL CAFreeTypeFont
{
	friend class CAFTFontCache;
public:

	CAFreeTypeFont();
	virtual ~CAFreeTypeFont();

	CAImage* initWithString(const char* pText, const char* pFontName, int nSize, int inWidth, int inHeight,
		CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment, bool bWordWrap = true, int iLineSpacing = 0, bool bBold = false, bool bItalics = false, bool bUnderLine = false);

	CAImage* initWithStringEx(const char* pText, const char* pFontName, int nSize, int inWidth, int inHeight, 
		std::vector<TextViewLineInfo>& linesText, int iLineSpace = 0, bool bWordWrap = true);

	static void destroyAllFontBuff();
protected:
	bool initFreeTypeFont(const char* pFontName, unsigned long nSize);
	void finiFreeTypeFont();
	unsigned char* loadFont(const char *pFontName, unsigned long *size, int& ttfIndex);
	unsigned char* getBitmap(ETextAlign eAlignMask, int* outWidth, int* outHeight);
	int getFontHeight();
	int getStringWidth(const std::string& text, bool bBold = false, bool bItalics = false);
    int cutStringByWidth(const std::string& text, int iLimitWidth, int& cutWidth);
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
	void draw_line(unsigned char* pBuffer, FT_Int x1, FT_Int y1, FT_Int x2, FT_Int y2);

    FT_Vector getPenForAlignment(FTLineInfo* pInfo, ETextAlign eAlignMask, int lineNumber, int totalLines);

    void calcuMultiLines(std::vector<TGlyph>& glyphs);
    
    FT_Error addWord(const std::string& word);
    void newLine();
    void endLine();

    const std::string m_space;
	FT_Face			m_face;
	std::vector<FTLineInfo*> m_lines;
	FT_Matrix		m_ItalicMatrix;

    int             m_inWidth;      // requested width of text box
    int             m_inHeight;     // requested height of text box
    int             m_width;        // final bitMap width
    int             m_height;       // final bitMap height
    int             m_textWidth;    // width of text text after word wrapping and line breaks    
    int             m_textHeight;   // height of text text after word wrapping and line breaks  
    int             m_lineHeight;   // height of a line for the font size
	int				m_lineSpacing;

	FTLineInfo*     m_currentLine;  // the current line object to add words to.

	bool m_bWordWrap;
	bool m_bBold;
	bool m_bItalics;
	bool m_bUnderLine;
	bool m_bOpenTypeFont;
};

NS_CC_END

#endif
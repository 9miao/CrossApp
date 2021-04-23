#include "CAFreeTypeFont.h"
#include "basics/CAApplication.h"
#include "platform/CAFileUtils.h"
#include "support/ccUTF8.h"
#include "CATempTypeFont.h"
#include <string.h>
#include "device/CADevice.h"


NS_CC_BEGIN

struct StrICmpLess
{
	bool operator()(const std::string& _Left, const std::string& _Right) const { 
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
		return strcasecmp(_Left.c_str(), _Right.c_str()) < 0;
#else
		return stricmp(_Left.c_str(), _Right.c_str()) < 0;
#endif
	}
};
static std::map<std::string, FontBufferInfo, StrICmpLess> s_fontsNames;
static FT_Library s_FreeTypeLibrary = NULL;


CAFreeTypeFont::CAFreeTypeFont()
:m_space(" ")
,m_currentLine(NULL)
,m_face(NULL)
,m_inWidth(0)
,m_inHeight(0)
,m_inFontSize(0)
,m_width(0)
,m_height(0)
,m_textWidth(0)
,m_textHeight(0)
,m_lineHeight(0)
, m_lineSpacing(0)
, m_bWordWrap(true)
, m_bBold(false)
, m_bItalics(false)
, m_bUnderLine(false)
{
	m_ItalicMatrix.xx = 0x10000L;
	m_ItalicMatrix.xy = ITALIC_LEAN_VALUE * 0x10000L;
	m_ItalicMatrix.yx = 0;
	m_ItalicMatrix.yy = 0x10000L;

}

CAFreeTypeFont::~CAFreeTypeFont() 
{
	finiFreeTypeFont();
	destroyAllLines();
}

void CAFreeTypeFont::destroyAllFontBuff()
{
	std::map<std::string, FontBufferInfo>::iterator it = s_fontsNames.begin();
	for (; it != s_fontsNames.end(); it++)
	{
		delete[]it->second.pBuffer;
	}
	s_fontsNames.clear();
}


CAImage* CAFreeTypeFont::initWithString(const std::string& pText, const CAColor4B& fontColor, const std::string& pFontName, int nSize, int inWidth, int inHeight,
	CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment, bool bWordWrap, int iLineSpacing, bool bBold, bool bItalics, bool bUnderLine, bool bDeleteLine, std::vector<TextViewLineInfo>* pLinesText)
{
	if (pText.empty())
		return NULL;
	
	std::u32string cszTemp;
	std::string cszNewText = pText;

	CATempTypeFont::getInstance().initTempTypeFont(nSize);

_AgaginInitGlyphs:
	m_inWidth = inWidth;
	m_inHeight = inHeight;
	m_lineSpacing = iLineSpacing;
	m_bWordWrap = bWordWrap;
	m_bBold = bBold;
	m_bItalics = bItalics;
	m_bUnderLine = bUnderLine;
	m_bDeleteLine = bDeleteLine;
	m_cFontColor = fontColor;

	FT_Error error = initGlyphs(cszNewText.c_str());
	if (error) return NULL;

	if (pLinesText != NULL)
	{
		initTextView(*pLinesText);
	}
	else if (m_inHeight < m_textHeight)
	{
		if (cszTemp.empty())
		{
			int totalLines = m_inHeight / m_lineHeight;

			for (int i = 0; i < m_lines.size(); i++, cszTemp+='\n')
			{
				if (i < totalLines)
				{
					std::vector<TGlyph>& v = m_lines[i]->glyphs;
					for (int j = 0; j < v.size(); j++)
					{
						cszTemp += v[j].c;
					}
				}
				else break;
			}
		}
		if (cszTemp.empty())
		{
			vAlignment = CAVerticalTextAlignment::Top;
		}
		else
		{
			cszTemp.erase(cszTemp.end() - 1);

			cszNewText.clear();
			StringUtils::UTF32ToUTF8(cszTemp, cszNewText);
			if (!cszNewText.empty())
			{
				cszNewText += "...";
			}
			destroyAllLineFontGlyph();
			goto _AgaginInitGlyphs;
		}
	}

	ETextAlign eAlign;
	if (CAVerticalTextAlignment::Top == vAlignment)
	{
		eAlign = (CATextAlignment::Center == hAlignment) ? kAlignTop
        : (CATextAlignment::Left == hAlignment) ? kAlignTopLeft : kAlignTopRight;
	}
	else if (CAVerticalTextAlignment::Center == vAlignment)
	{
		eAlign = (CATextAlignment::Center == hAlignment) ? kAlignCenter
        : (CATextAlignment::Left == hAlignment) ? kAlignLeft : kAlignRight;
	}
	else if (CAVerticalTextAlignment::Bottom == vAlignment)
	{
		eAlign = (CATextAlignment::Center == hAlignment) ? kAlignBottom
        : (CATextAlignment::Left == hAlignment) ? kAlignBottomLeft : kAlignBottomRight;
	}
	else
	{
		eAlign = kAlignTopLeft;
	}

	int width = 0, height = 0;
	unsigned char* pData = getBitmap(eAlign, &width, &height);
	if (pData == NULL)
	{
		return NULL;
	}
	m_lineSpacing = 0;
	m_bWordWrap = false;
	m_bBold = false;
	m_bItalics = false;
	m_bUnderLine = false;

	CAData* data = CAData::create();
	data->fastSet(pData, width * height * 4);
	CAImage* image = new CAImage();
	if (!image->initWithRawData(data, CAImage::PixelFormat::RGBA8888, width, height))
	{
        CC_SAFE_RELEASE_NULL(image);
		return nullptr;
	}
	image->autorelease();
	return image;
}

unsigned char* CAFreeTypeFont::getBitmap(ETextAlign eAlignMask, int* outWidth, int* outHeight)
{
    int lineNumber = 0;
    int totalLines = (int)m_lines.size();

    m_width = m_inWidth ? m_inWidth : m_textWidth;
    m_height = m_inHeight ? m_inHeight : m_textHeight;
    
    unsigned int size = m_width * m_height * 4;
	unsigned char* pBuffer = new unsigned char[size];
    if(!pBuffer)
    {
		return NULL;
    }
	memset(pBuffer, 0, size);

    std::vector<FTLineInfo*>::iterator line;
	for (line = m_lines.begin(); line != m_lines.end(); ++line)
    {
        FT_Vector pen = getPenForAlignment(*line, eAlignMask, lineNumber, totalLines);
		drawText(*line, pBuffer, &pen);
		if (m_bUnderLine)
		{
			draw_line(pBuffer, (FT_Int)pen.x, (FT_Int)pen.y, (FT_Int)(pen.x + (*line)->width), (FT_Int)pen.y);
		}
		if (m_bDeleteLine)
		{
			draw_line(pBuffer, (FT_Int)pen.x, (FT_Int)(pen.y - m_inFontSize*0.3f), (FT_Int)(pen.x + (*line)->width), (FT_Int)(pen.y - m_inFontSize*0.3f));
		}
        lineNumber++;
    }
    *outWidth = m_width;
    *outHeight = m_height;

    return pBuffer;
}

int CAFreeTypeFont::getFontHeight()
{
	if (m_face != NULL)
	{
		return (int)(((m_face->size->metrics.ascender) >> 6) - ((m_face->size->metrics.descender) >> 6));
	}
	return 0;
}

// text encode with utf8
int CAFreeTypeFont::getStringWidth(const std::string& text, bool bBold, bool bItalics)
{
	int iStrWidth = 0;
	std::vector<TGlyph> glyphs;

	FT_Vector vt;
	memset(&vt, 0, sizeof(vt));

	m_bBold = bBold;
	m_bItalics = bItalics;
	if (0 == initWordGlyphs(glyphs, text, vt))
	{
		iStrWidth = (int)vt.x;
	}
	m_bBold = false;
	m_bItalics = false;
	m_bUnderLine = false;

	destroyFontGlyph(glyphs);
	return iStrWidth;
}

int CAFreeTypeFont::cutStringByWidth(const std::string& text, int iLimitWidth, int& cutWidth)
{
    std::vector<TGlyph> glyphs;
    
    FT_Vector vt;
    memset(&vt, 0, sizeof(vt));
    if (0 != initWordGlyphs(glyphs, text, vt))
        return 0;
    
    FT_BBox bbox;
    FT_BBox glyph_bbox;
    FT_GlyphSlot slot = m_face->glyph;
    
    /* initialize string bbox to "empty" values */
    bbox.xMin = 32000;
    bbox.xMax = -32000;
    
    // use the max and min values for the entire font face
    bbox.yMin = (m_face->size->metrics.descender) >> 6;
    bbox.yMax = (m_face->size->metrics.ascender) >> 6;
    
    /* for each glyph image, compute its bounding box, */
    /* translate it, and grow the string bbox          */
    
    int nWordPos = 0;
    cutWidth = 0;
    for (std::vector<TGlyph>::iterator glyph = glyphs.begin(); glyph != glyphs.end(); ++glyph)
    {
        FT_Glyph_Get_CBox(glyph->image, ft_glyph_bbox_pixels, &glyph_bbox);
        
		if (glyph->image == NULL)
		{
			glyph_bbox.xMax = (glyph_bbox.xMin + m_inFontSize);
		}
        if (glyph_bbox.xMin == glyph_bbox.xMax)
        {
            glyph_bbox.xMax = glyph_bbox.xMin + (slot->advance.x >> 6) + DEFAULT_SPACE_VALUE;
        }
        glyph_bbox.xMin += glyph->pos.x;
        glyph_bbox.xMax += glyph->pos.x;
        glyph_bbox.yMin += glyph->pos.y;
        glyph_bbox.yMax += glyph->pos.y;
        
        if (glyph_bbox.xMin < bbox.xMin)
            bbox.xMin = glyph_bbox.xMin;
        
        if (glyph_bbox.yMin < bbox.yMin)
            bbox.yMin = glyph_bbox.yMin;
        
        if (glyph_bbox.xMax > bbox.xMax)
            bbox.xMax = glyph_bbox.xMax;
        
        if (glyph_bbox.yMax > bbox.yMax)
            bbox.yMax = glyph_bbox.yMax;
        
        int width = (int)(bbox.xMax - bbox.xMin);
		cutWidth = (int)(glyph->pos.x - bbox.xMin + (slot->advance.x >> 6) + DEFAULT_SPACE_VALUE);
        if (width > iLimitWidth)
        {
            cutWidth = (int)(glyph->pos.x - bbox.xMin);
            break;
        }
        
        nWordPos++;
    }
    
    int nCharPos = 0;
    for (int i = 0; i < nWordPos; i++)
    {
        char ch = text[nCharPos];
        if ((ch & 0x80) == 0x00)			// 0xxxxxxx
            nCharPos += 1;
        else if ((ch & 0xE0) == 0xC0)		// 110xxxxx
            nCharPos += 2;
        else if ((ch & 0xF0) == 0xE0)		// 1110xxxx
            nCharPos += 3;
        else
            nCharPos += 4;
    }
    
	destroyFontGlyph(glyphs);
    return nCharPos;
}

int CAFreeTypeFont::cutStringByDSize(std::string& text, const DSize& lableSize, const std::string& pFontName, unsigned long nFontSize, bool bWordWrap, int iLineSpacing, bool bBold, bool bItalics)
{
	std::u32string cszTemp;
	std::string cszNewText = text;

	CATempTypeFont::getInstance().initTempTypeFont(nFontSize);

	m_inWidth = lableSize.width;
	m_inHeight = lableSize.height;
	m_lineSpacing = iLineSpacing;
	m_bWordWrap = bWordWrap;
	m_bBold = bBold;
	m_bItalics = bItalics;
	m_bUnderLine = false;

	FT_Error error = initGlyphs(cszNewText.c_str());
	if (error) return -1;

	if (m_inHeight < m_textHeight)
	{
		if (cszTemp.empty())
		{
			int totalLines = m_inHeight / m_lineHeight;

			for (int i = 0; i < m_lines.size(); i++)
			{
				if (i < totalLines)
				{
					std::vector<TGlyph>& v = m_lines[i]->glyphs;
					for (int j = 0; j < v.size(); j++)
					{
						cszTemp += v[j].c;
					}
					if (m_lines[i]->includeRet)
					{
						cszTemp += '\n';
					}
				}
				else break;
			}
		}
		cszNewText.clear();
		StringUtils::UTF32ToUTF8(cszTemp, cszNewText);
	}
	destroyAllLineFontGlyph();
	text = cszNewText;
	return 0;
}

// text encode with utf8
int CAFreeTypeFont::getStringHeight(const std::string& text, int iLimitWidth, int iLineSpace, bool bWordWrap)
{
	m_inWidth = iLimitWidth;
	m_inHeight = 0;
	m_lineSpacing = iLineSpace;
	m_bWordWrap = bWordWrap;

	initGlyphs(text.c_str());

	for (int i = 0; i < m_lines.size(); i++)
	{
		destroyFontGlyph(m_lines[i]->glyphs);
	}
	destroyAllLines();
	return m_textHeight;
}

void CAFreeTypeFont::destroyAllLines()
{
	for (int i = 0; i < m_lines.size(); i++)
	{
		delete m_lines[i];
	}
	m_lines.clear();
	m_currentLine = NULL;
}

void CAFreeTypeFont::destroyFontGlyph(std::vector<TGlyph>& v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		if (v[i].image)
		{
			FT_Done_Glyph(v[i].image);
		}
		v[i].image = 0;
	}
}

void CAFreeTypeFont::destroyAllLineFontGlyph()
{
	for (int i = 0; i < m_lines.size(); i++)
	{
		destroyFontGlyph(m_lines[i]->glyphs);
	}
}

FT_Vector CAFreeTypeFont::getPenForAlignment(FTLineInfo* pInfo, ETextAlign eAlignMask,int lineNumber, int totalLines)
{
    FT_Vector pen;
    
    int top;
    int stringWidth  = (int)(pInfo->bbox.xMax - pInfo->bbox.xMin);
    int maxLineNumber = totalLines - 1;
    pen.x = 0;
    pen.y = 0;

    switch(eAlignMask)
    {
        case kAlignTop: // Horizontal center and vertical top.
            pen.x = ((m_width  - stringWidth) / 2) - pInfo->bbox.xMin;
            pen.y = pInfo->bbox.yMax + (lineNumber * m_lineHeight);	
 		    break;
			
        case kAlignTopLeft: // Horizontal left and vertical top.
            pen.x -=pInfo->bbox.xMin;
            pen.y = pInfo->bbox.yMax + (lineNumber * m_lineHeight);		    
 		    break;

	    case kAlignTopRight: // Horizontal right and vertical top.
            pen.x = m_width - stringWidth - pInfo->bbox.xMin;
            pen.y = pInfo->bbox.yMax + (lineNumber * m_lineHeight);		    
		    break;
 
	    case kAlignBottomRight: // Horizontal right and vertical bottom.
            pen.x = m_width - stringWidth - pInfo->bbox.xMin;
            pen.y = m_height + pInfo->bbox.yMin - ((maxLineNumber - lineNumber) * m_lineHeight);
		    break;

	    case kAlignBottom: // Horizontal center and vertical bottom.
            pen.x = ((m_width  - stringWidth) / 2) - pInfo->bbox.xMin;
            pen.y = m_height + pInfo->bbox.yMin - ((maxLineNumber - lineNumber) * m_lineHeight);
		    break;

	    case kAlignBottomLeft: // Horizontal left and vertical bottom.
            pen.x -=pInfo->bbox.xMin;
            top = (m_height - m_textHeight) / 2;
            pen.y = m_height + pInfo->bbox.yMin - ((maxLineNumber - lineNumber) * m_lineHeight);
  		    break;

	    case kAlignCenter: // Horizontal center and vertical center
            pen.x = ((m_width  - stringWidth) / 2) - pInfo->bbox.xMin;
            top = (m_height - m_textHeight) / 2;
            pen.y = top + (lineNumber * m_lineHeight) + pInfo->bbox.yMax;		    
			pen.y += m_lineSpacing / 2;
            break;

	    case kAlignRight: // Horizontal right and vertical center.
            pen.x = m_width - stringWidth - pInfo->bbox.xMin;
            top = (m_height - m_textHeight) / 2;
            pen.y = top + (lineNumber * m_lineHeight) + pInfo->bbox.yMax;		    
			pen.y += m_lineSpacing / 2;
  		    break;

	    case kAlignLeft: // Horizontal left and vertical center.
	    default:
            pen.x -=pInfo->bbox.xMin;
            top = (m_height - m_textHeight) / 2;
            pen.y = top + (lineNumber * m_lineHeight) + pInfo->bbox.yMax;
			pen.y += m_lineSpacing / 2;
  		    break;
    }
    return pen;
}
static int s_fontOffsetType = 0;

static int getWordOffset(FT_ULong c, int lineHeight)
{
    int value = 0;
    if (s_fontOffsetType == 0)
    {
        value = 0;
    }
    else if (s_fontOffsetType == 1)
    {
        value = (c > 0x80) ? 0 : (lineHeight / 12);
    }
    return value;
}

static int getEmojiOffset(int lineHeight)
{
    int value = 0;
    if (s_fontOffsetType == 0)
    {
        value = lineHeight / 12;
    }
    else if (s_fontOffsetType == 1)
    {
        value = 0;
    }
    return value;
}

void CAFreeTypeFont::drawText(FTLineInfo* pInfo, unsigned char* pBuffer, FT_Vector *pen)
{
	std::vector<TGlyph>& glyphs = pInfo->glyphs;
	for (std::vector<TGlyph>::reverse_iterator glyph = glyphs.rbegin(); glyph != glyphs.rend(); ++glyph)
    {
        FT_Glyph image = glyph->image;
		if (image == NULL)
		{
			continue;
		}
		
        FT_Error error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, 0, 1);
        if (!error)
        {
            FT_BitmapGlyph  bit = (FT_BitmapGlyph)image;

			int dtValue = getWordOffset(glyph->c, m_lineHeight);
            FT_Int x = (FT_Int)(pen->x + glyph->pos.x + bit->left);
			FT_Int y = (FT_Int)(pen->y - bit->top + dtValue);
			draw_bitmap(pBuffer, &bit->bitmap, x, y);
            FT_Done_Glyph(image);
        }
    }
}

void CAFreeTypeFont::draw_bitmap(unsigned char* pBuffer, FT_Bitmap*  bitmap, FT_Int x, FT_Int y)
{
    FT_Int  i, j, p, q;
    FT_Int  x_max = x + bitmap->width;
    FT_Int  y_max = y + bitmap->rows;

    for (i = x, p = 0; i < x_max; i++, p++)
    {
        for ( j = y, q = 0; j < y_max; j++, q++ )
        {
            if (i < 0 || j < 0 || i >= m_width || j >= m_height)
                continue;

			FT_Int index = (j * m_width * 4) + (i * 4);

			unsigned char value = bitmap->buffer[q * bitmap->width + p];
			if (value > 0)
			{
                pBuffer[index++] = m_cFontColor.r * value / 255.0f;
				pBuffer[index++] = m_cFontColor.g * value / 255.0f;
				pBuffer[index++] = m_cFontColor.b * value / 255.0f;
				pBuffer[index++] = value;
			}
        }
    }  
}

void CAFreeTypeFont::draw_line(unsigned char* pBuffer, FT_Int x1, FT_Int y1, FT_Int x2, FT_Int y2)
{
	for (FT_Int i = x1; i <= x2; i++)
	{
		for (FT_Int j = y1; j <= y2; j++)
		{
			FT_Int index = (j * m_width * 4) + (i * 4);
			for (int k = 0; k < 4; k++)
			{
				pBuffer[index++] = m_cFontColor.r;
				pBuffer[index++] = m_cFontColor.g;
				pBuffer[index++] = m_cFontColor.b;
				pBuffer[index++] = 0xff;
			}
		}
	}
}

void CAFreeTypeFont::endLine() 
{
    if(m_currentLine)
    {
        m_lines.push_back(m_currentLine);
		m_textWidth = MAX(m_textWidth, (int)(m_currentLine->bbox.xMax - m_currentLine->bbox.xMin));
        m_textHeight += m_lineHeight;
    }
}


void CAFreeTypeFont::newLine() 
{
    m_currentLine = new FTLineInfo();
    m_currentLine->width = 0;
    m_currentLine->pen.x = 0;
    m_currentLine->pen.y = 0;
	m_currentLine->includeRet = false;
}


void CAFreeTypeFont::calcuMultiLines(std::vector<TGlyph>& glyphs)
{
	FT_GlyphSlot slot = m_face->glyph;
	int maxWidth = m_inWidth ? m_inWidth : 0xFFFF;
	
	m_currentLine->bbox.xMin = 32000;
	m_currentLine->bbox.xMax = -32000;
	m_currentLine->bbox.yMin = (m_face->size->metrics.descender) >> 6;
	m_currentLine->bbox.yMax = (m_face->size->metrics.ascender) >> 6;
    
	int i = 0;
	for (; i < glyphs.size(); i++)
	{
		FT_BBox glyph_bbox;
		FT_Glyph_Get_CBox(glyphs[i].image, ft_glyph_bbox_pixels, &glyph_bbox);
        
		if (glyphs[i].image == NULL)
		{
			glyph_bbox.xMax = (glyph_bbox.xMin + m_inFontSize);
		}
		if (glyph_bbox.xMin == glyph_bbox.xMax)
		{
			glyph_bbox.xMax = glyph_bbox.xMin + (slot->advance.x >> 6) + DEFAULT_SPACE_VALUE;
		}

		glyph_bbox.xMin += glyphs[i].pos.x;
		glyph_bbox.xMax += glyphs[i].pos.x;
		glyph_bbox.yMin += glyphs[i].pos.y;
		glyph_bbox.yMax += glyphs[i].pos.y;
        
		if (m_inWidth == 0xFFFF || 
			(glyph_bbox.xMax <= maxWidth && m_currentLine->bbox.xMax - m_currentLine->bbox.xMin <= maxWidth))
		{
			m_currentLine->glyphs.push_back(glyphs[i]);
		}
		else
		{
			break;
		}

		if (glyph_bbox.xMin < m_currentLine->bbox.xMin)
			m_currentLine->bbox.xMin = glyph_bbox.xMin;

		if (glyph_bbox.yMin < m_currentLine->bbox.yMin)
			m_currentLine->bbox.yMin = glyph_bbox.yMin;

		if (glyph_bbox.xMax > m_currentLine->bbox.xMax)
			m_currentLine->bbox.xMax = glyph_bbox.xMax;

		if (glyph_bbox.yMax > m_currentLine->bbox.yMax)
			m_currentLine->bbox.yMax = glyph_bbox.yMax;
	}
    
	if (i <= 0)
	{
		return;
	}
	glyphs.erase(glyphs.begin(), glyphs.begin() + i);
	m_currentLine->width = (unsigned int)(m_currentLine->bbox.xMax - m_currentLine->bbox.xMin);
	m_currentLine->pen.x = m_currentLine->bbox.xMax;

	if (!glyphs.empty())
	{
		endLine();
		newLine();

		FT_Pos iTruncted = glyphs[0].pos.x;
		for (int i = 0; i < glyphs.size(); i++)
		{
			if (glyphs[i].index == 0 && !glyphs[i].isEmoji)
				continue;
			
			glyphs[i].pos.x -= iTruncted;
		}
		calcuMultiLines(glyphs);
	}
}

FT_Error CAFreeTypeFont::addWord(const std::string& word) 
{
	std::vector<TGlyph> glyphs; // glyphs for the word
	FT_BBox             bbox;   // bounding box containing all of the glyphs in the word
    int maxWidth = m_inWidth ? m_inWidth : 0xFFFFFF;
    std::string newWord;

    if (word.empty()) 
    {
        newWord = ' ' + word;
    } 
    else
    {
        newWord = word;
    }

    FT_Error error = initWordGlyphs(glyphs, newWord, m_currentLine->pen);
    if(!error) 
	{
        compute_bbox(glyphs, &bbox);
		if(m_currentLine->width == 0 || bbox.xMax <= maxWidth)
        {
			if (m_currentLine->width == 0 && bbox.xMax-bbox.xMin>maxWidth)
			{
				calcuMultiLines(glyphs);
			}
			else
			{
				m_currentLine->glyphs.insert(m_currentLine->glyphs.end(), glyphs.begin(), glyphs.end());
				if (m_currentLine->width == 0)
				{
					m_currentLine->bbox = bbox;
				}
				else
				{
					m_currentLine->bbox.xMax = bbox.xMax;
				}
				m_currentLine->width = (unsigned int)m_currentLine->bbox.xMax - (unsigned int)m_currentLine->bbox.xMin;
			}
        }
        else 
        {
            endLine();
            newLine();
            addWord(word);
        }
    }
    return error;
}

FT_Error CAFreeTypeFont::initGlyphs(const std::string& text)
{
    std::string line(text);
    std::vector<std::string> lines;
    std::vector<std::string> words;

	size_t pos = 0;
    m_textWidth = 0;
    m_textHeight = 0;
    // the height of a line of text based on the max height of a glyph in the font size
    m_lineHeight = (int)(((m_face->size->metrics.ascender) >> 6) - ((m_face->size->metrics.descender) >> 6));
	//m_lineSpacing += m_lineHeight * 0.15;
	m_lineHeight += m_lineSpacing;

	destroyAllLines();

	if (!line.empty())
	{
		size_t first = line.find('\n');
		while (first != std::string::npos)
		{
			initGlyphsLine(line.substr(pos, first - pos));
			m_currentLine->includeRet = true;

			pos = first + 1;
			first = line.find('\n', pos);
		}
		initGlyphsLine(line.substr(pos, line.size() - pos));
    }
    return 0;
}

FT_Error CAFreeTypeFont::initGlyphsLine(const std::string& line)
{
	newLine();

	if (!line.empty())
	{
		if (!m_bWordWrap)
		{
			addWord(line);
		}
		else
		{
			std::size_t prev = 0, pos;
			while ((pos = line.find_first_of(" ", prev)) != std::string::npos)
			{
				if (pos >= prev)
				{
					addWord(line.substr(prev, pos - prev));
				}
				prev = pos > prev ? pos : pos + 1;
			}
			if (prev < line.length())
			{
				addWord(line.substr(prev, std::string::npos));
			}
		}
	}

	endLine();

	return 0;
}


FT_Error CAFreeTypeFont::initWordGlyphs(std::vector<TGlyph>& glyphs, const std::string& text, FT_Vector& pen) 
{
	FT_UInt			glyph_index;
	FT_UInt			previous = 0;
	FT_Error		error = 0;
	PGlyph			glyph;
    unsigned int    numGlyphs = 0;
	unsigned int	italicsDt = m_lineHeight * tan(ITALIC_LEAN_VALUE * 0.15 * M_PI);

	std::u32string utf32String;
	if (!StringUtils::UTF8ToUTF32(text, utf32String))
		return -1;

	glyphs.clear();
	glyphs.reserve(utf32String.size());

	FT_Bool useKerning = FT_HAS_KERNING(m_face);
	for (int n = 0; n < utf32String.size(); n++)
	{
		FT_ULong c = utf32String[n];

		if (c == '\r' || c == '\n')
			continue;
		
		/* convert character code to glyph index */
		glyphs.resize(glyphs.size() + 1);
		glyph = &glyphs[numGlyphs];
		glyph->c = c;
		glyph_index = FT_Get_Char_Index(m_face, c);
		glyph->isEmoji = false;

		FT_Bool isOpenType = (glyph_index == 0);
		if (glyph_index == 0)
		{
			glyph_index = FT_Get_Char_Index(CATempTypeFont::getInstance().m_CurFontFace, c);
		}
        glyph->index = glyph_index;
		if (glyph_index == 0)
		{
			glyphs.resize(glyphs.size() - 1);
			continue;
		}

		FT_Face curFace = isOpenType ? CATempTypeFont::getInstance().m_CurFontFace : m_face;
		if (curFace==NULL)
        {
            numGlyphs++;
            continue;
        }
		
		FT_GlyphSlot slot = curFace->glyph;
 		if (useKerning && previous && glyph_index)
		{
			FT_Vector  delta;
			FT_Get_Kerning(curFace, previous, glyph_index, FT_KERNING_DEFAULT, &delta);
			pen.x += delta.x >> 6;
		}

		/* store current pen position */
		glyph->pos = pen;

		if (glyph->isEmoji)
		{
			pen.x += m_inFontSize;
			numGlyphs++;
			continue;
		}

		/* load glyph image into the slot without rendering */
		error = FT_Load_Glyph(curFace, glyph_index, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP);
		if (error)
			continue;  /* ignore errors, jump to next glyph */

		if (m_bBold)
		{
			FT_Outline_Embolden(&slot->outline, 48);
		}

		if (c == 10)
		{
			continue;
		}

		/* extract glyph image and store it in our table */
		error = FT_Get_Glyph(curFace->glyph, &glyph->image);
		if (error)
			continue;  /* ignore errors, jump to next glyph */

		FT_Matrix* pFTMat = m_bItalics ? &m_ItalicMatrix : NULL;

		 /* translate the glyph image now */
		FT_Glyph_Transform(glyph->image, pFTMat, NULL);

		/* increment pen position */
		pen.x += (slot->advance.x >> 6) + DEFAULT_SPACE_VALUE;
		if (pFTMat)
		{
			pen.x += (italicsDt/5.0f);
		}

		/* record current glyph index */
		previous = glyph_index;

		numGlyphs++;
	}
	return error;
}


void CAFreeTypeFont::initTextView(std::vector<TextViewLineInfo>& linesText)
{
	linesText.clear();
	bool bIncludeReturn = false;

	int iCurCharPos = 0;
	for (int i = 0; i < m_lines.size(); i++)
	{
		TextViewLineInfo cTextViewLine;
		cTextViewLine.iStartCharPos = iCurCharPos;

		FT_BBox bbox;
		bbox.xMin = 32000;
		bbox.xMax = -32000;
		bbox.yMin = (m_face->size->metrics.descender) >> 6;
		bbox.yMax = (m_face->size->metrics.ascender ) >> 6;

		if (bIncludeReturn)
		{
			TextAttribute v;
			v.charSize = 1; v.charlength = 0;
			cTextViewLine.TextAttrVect.push_back(v);
			iCurCharPos += v.charSize;
		}

		for (int j = 0; j < m_lines[i]->glyphs.size(); j++)
		{
			TGlyph& g = m_lines[i]->glyphs[j];
			
			FT_BBox temp_bbox = bbox;
			compute_bbox2(g, bbox);

			TextAttribute v;
			if (j==0)
			{
				int iStartPos = (int)bbox.xMin;
				temp_bbox = bbox;
				temp_bbox.xMin -= iStartPos;
				temp_bbox.xMax -= iStartPos;
				v.charlength = bbox.xMax - bbox.xMin;
			}
			else
			{
				v.charlength = bbox.xMax - temp_bbox.xMax;
			}

			std::u32string cstr;
			cstr += g.c; std::string s;
			StringUtils::UTF32ToUTF8(cstr, s);
			v.charSize = (int)s.size();
			cTextViewLine.TextAttrVect.push_back(v);
			iCurCharPos += v.charSize;
		}
		cTextViewLine.iEndCharPos = iCurCharPos;
		linesText.push_back(cTextViewLine);

		bIncludeReturn = m_lines[i]->includeRet;
	}
}

void  CAFreeTypeFont::compute_bbox(std::vector<TGlyph>& glyphs, FT_BBox  *abbox)
{
    FT_BBox bbox;
    FT_BBox glyph_bbox;
	FT_GlyphSlot slot = m_face->glyph;

    /* initialize string bbox to "empty" values */
    bbox.xMin = 32000;
    bbox.xMax = -32000;

    // use the max and min values for the entire font face
    bbox.yMin = (m_face->size->metrics.descender) >> 6;
    bbox.yMax = (m_face->size->metrics.ascender) >> 6;

    /* for each glyph image, compute its bounding box, */
    /* translate it, and grow the string bbox          */
	for (std::vector<TGlyph>::iterator glyph = glyphs.begin(); glyph != glyphs.end(); ++glyph)
    {
		FT_Glyph_Get_CBox(glyph->image, ft_glyph_bbox_pixels, &glyph_bbox);

		if (glyph->image == NULL)
		{
			glyph_bbox.xMax = (glyph_bbox.xMin + m_inFontSize);
		}
		if (glyph_bbox.xMin == glyph_bbox.xMax)
		{
			glyph_bbox.xMax = glyph_bbox.xMin + (slot->advance.x >> 6) + DEFAULT_SPACE_VALUE;
		}
        
        glyph_bbox.xMin += glyph->pos.x;
        glyph_bbox.xMax += glyph->pos.x;
        glyph_bbox.yMin += glyph->pos.y;
        glyph_bbox.yMax += glyph->pos.y;

        if (glyph_bbox.xMin < bbox.xMin)
            bbox.xMin = glyph_bbox.xMin;

        if (glyph_bbox.yMin < bbox.yMin)
            bbox.yMin = glyph_bbox.yMin;

        if (glyph_bbox.xMax > bbox.xMax)
            bbox.xMax = glyph_bbox.xMax;

        if (glyph_bbox.yMax > bbox.yMax)
            bbox.yMax = glyph_bbox.yMax;
    }

    /* check that we really grew the string bbox */
    if (bbox.xMin > bbox.xMax)
    {
        bbox.xMin = 0;
        bbox.yMin = 0;
        bbox.xMax = 0;
        bbox.yMax = 0;
    }
  
    /* return string bbox */
    *abbox = bbox;
}

void CAFreeTypeFont::compute_bbox2(TGlyph& glyph, FT_BBox& bbox)
{
	FT_BBox glyph_bbox;
	FT_GlyphSlot slot = m_face->glyph;
	FT_Glyph_Get_CBox(glyph.image, ft_glyph_bbox_pixels, &glyph_bbox);

	if (glyph.image == NULL)
	{
		glyph_bbox.xMax = (glyph_bbox.xMin + m_inFontSize);
	}
	if (glyph_bbox.xMin == glyph_bbox.xMax)
	{
		glyph_bbox.xMax = glyph_bbox.xMin + (slot->advance.x >> 6) + DEFAULT_SPACE_VALUE;
	}
	glyph_bbox.xMin += glyph.pos.x;
	glyph_bbox.xMax += glyph.pos.x;
	glyph_bbox.yMin += glyph.pos.y;
	glyph_bbox.yMax += glyph.pos.y;

	if (glyph_bbox.xMin < bbox.xMin)
		bbox.xMin = glyph_bbox.xMin;

	if (glyph_bbox.yMin < bbox.yMin)
		bbox.yMin = glyph_bbox.yMin;

	if (glyph_bbox.xMax > bbox.xMax)
		bbox.xMax = glyph_bbox.xMax;

	if (glyph_bbox.yMax > bbox.yMax)
		bbox.yMax = glyph_bbox.yMax;

	if (bbox.xMin > bbox.xMax)
	{
		bbox.xMin = 0;
		bbox.yMin = 0;
		bbox.xMax = 0;
		bbox.yMax = 0;
	}
}

bool CAFreeTypeFont::initFreeTypeFont(const std::string& pFontName, unsigned long nSize)
{
	m_face = initFreeType(pFontName, nSize);
	m_inFontSize = nSize;
	return (m_face != NULL);
}

void CAFreeTypeFont::finiFreeTypeFont()
{
	if (m_face)
	{
		finiFreeType(m_face);
	}
	m_face = NULL;
}

FT_Face CAFreeTypeFont::initFreeType(const std::string& pFontName, unsigned long nSize)
{
	unsigned long size = 0; int face_index = 0;
	unsigned char* pBuffer = loadFont(pFontName, &size, face_index);
	if (pBuffer == NULL)
		return NULL;
	
	FT_Error error = 0;
	if (!s_FreeTypeLibrary)
	{
		error = FT_Init_FreeType(&s_FreeTypeLibrary);
	}

	FT_Face face = NULL;
	if (!error)
	{
		error = FT_New_Memory_Face(s_FreeTypeLibrary, pBuffer, size, face_index, &face);
	}

	if (!error)
	{
		FT_Select_Charmap(face, FT_ENCODING_UNICODE);
		FT_Set_Char_Size(face, nSize << 6, nSize << 6, 72, 72);
		return face;
	}
	else
	{
		finiFreeType(face);
		return NULL;
	}
}

void CAFreeTypeFont::finiFreeType(FT_Face face)
{
	if (face)
	{
		FT_Done_Face(face);
	}
	face = NULL;	
}

unsigned char* CAFreeTypeFont::loadFont(const std::string& pFontName, unsigned long *size, int& ttfIndex)
{
	std::string path;
	std::string lowerCase(pFontName);
	for (unsigned int i = 0; i < lowerCase.length(); ++i)
	{
		lowerCase[i] = tolower(lowerCase[i]);
	}

	if (std::string::npos == lowerCase.find("fonts/"))
	{
		path = "fonts/";
		path += lowerCase;
	}
	else
	{
		path = lowerCase;
	}

	std::map<std::string, FontBufferInfo>::iterator ittFontNames = s_fontsNames.find(path.c_str());
	if (ittFontNames != s_fontsNames.end())
	{
		ttfIndex = ittFontNames->second.face_index;
		*size = ittFontNames->second.size;
        s_fontOffsetType = ittFontNames->second.font_offset_type;
		return ittFontNames->second.pBuffer;
	}

    s_fontOffsetType = 0;
	ttfIndex = 0;

	for (unsigned int i = 6; i < path.length(); ++i)
	{
		path[i] = tolower(path[i]);
	}
    unsigned char* pBuffer = NULL;
    
    if (path.compare("fonts/") != 0)
    {
        pBuffer = FileUtils::getInstance()->getFileData(path.c_str(), "rb", size);
        if (pBuffer == NULL)
        {
            for (unsigned int i = 6; i < path.length(); ++i)
            {
                path[i] = toupper(path[i]);
            }
            pBuffer = FileUtils::getInstance()->getFileData(path.c_str(), "rb", size);
        }
    }
    
    if (pBuffer == NULL)
    {
        const char* fontName = "fonts/Regular.ttf";
        pBuffer = FileUtils::getInstance()->getFileData(fontName, "rb", size);
        ttfIndex = 0;
    }
    
	if (pBuffer == NULL)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        char sTTFont[256];
        GetWindowsDirectoryA(sTTFont,255);
        strcat(sTTFont,"\\fonts\\simhei.ttf");
		const char* pFontName = sTTFont;

        pBuffer = FileUtils::getInstance()->getFileData(pFontName, "rb", size);
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        
        const char* fontName = "/System/Library/Fonts/STHeiti Light.ttc";
        pBuffer = FileUtils::getInstance()->getFileData(fontName, "rb", size);
		ttfIndex = 1;
        s_fontOffsetType = 1;
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        const char* fontName = NULL;
        
        float version = atof(CADevice::getSystemVersionWithIOS());
        
        if (version >= 9.0f)
        {
            fontName = "/System/Library/Fonts/LanguageSupport/PingFang.ttc";
            ttfIndex = 2;
        }
        else if (version >= 8.2f)
        {
            fontName = "/System/Library/Fonts/Core/STHeiti-Light.ttc";
            ttfIndex = 1;
            s_fontOffsetType = 1;
        }
        else
        {
            fontName = "/System/Library/Fonts/Cache/STHeiti-Light.ttc";
            ttfIndex = 1;
            s_fontOffsetType = 1;
        }
        
        pBuffer = FileUtils::getInstance()->getFileData(fontName, "rb", size);

        if (pBuffer == NULL)
        {
            fontName = "/System/Library/Fonts/STHeiti Light.ttc";
            pBuffer = FileUtils::getInstance()->getFileData(fontName, "rb", size);
            ttfIndex = 1;
            s_fontOffsetType = 1;
        }

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        const char* fontName = "/system/fonts/DroidSansFallback.ttf";
        pBuffer = FileUtils::getInstance()->getFileData(fontName, "rb", size);
        
        if (pBuffer == NULL)
        {
            fontName = "/system/fonts/NotoSansHans-Regular.otf";
            pBuffer = FileUtils::getInstance()->getFileData(fontName, "rb", size);
        }
#endif
	}

	

	FontBufferInfo info;
	info.pBuffer = pBuffer;
	info.size = *size;
	info.face_index = ttfIndex;
    info.font_offset_type = s_fontOffsetType;
	s_fontsNames[path] = info;
	return pBuffer;
}

NS_CC_END


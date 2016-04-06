#include "CAFTRichFont.h"
#include "support/ccUtils.h"
#include "view/CARichLabel.h"
#include "platform/CCFileUtils.h"
#include "support/ccUTF8.h"
#include "CATempTypeFont.h"
#include "CAEmojiFont.h"


NS_CC_BEGIN

CAFTRichFont g_AFTRichFont;

CAFTRichFont::CAFTRichFont()
: m_pCurrentLine(NULL)
{
	m_ItalicMatrix.xx = 0x10000L;
	m_ItalicMatrix.xy = ITALIC_LEAN_VALUE * 0x10000L;
	m_ItalicMatrix.yx = 0;
	m_ItalicMatrix.yy = 0x10000L;
}


CAFTRichFont::~CAFTRichFont()
{
	destroyAllLines();
}


CAImage* CAFTRichFont::initWithString(std::vector<LabelElement>& labels, const DSize& sz, const CAColor4B& linkCol, const CAColor4B& linkVisitedCol)
{
	destroyAllLines();

	m_inSize = sz;
	m_textSize = DSizeZero;
	m_linkColor = linkCol;
	m_linkVisitedColor = linkVisitedCol;
	m_HyperlinkRect = DRectZero;
	m_vHyperlinkRects.clear();
	initGlyphs(labels);

	int width = 0, height = 0;
	unsigned char* pData = getBitmap(&width, &height);
	if (pData == NULL)
	{
		return NULL;
	}

	for (int i = 0, j = 0; i < labels.size(); i++)
	{
		if (labels[i].nHyperlink == 0)
			continue;
		labels[i].vHyperlinkRects = m_vHyperlinkRects[j++];
	}

	CAImage* image = new CAImage();
	if (!image->initWithRawData(pData, CAImage::PixelFormat_RGBA8888, width, height))
	{
		CC_SAFE_RELEASE_NULL(image);
	}
	delete[]pData;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	image->releaseData();
#endif
	image->autorelease();
	return image;
}


void CAFTRichFont::newLine()
{
	m_pCurrentLine = new FTLineInfoEx();
	m_pCurrentLine->width = 0;
	m_pCurrentLine->height = 0;
	m_pCurrentLine->pen.x = 0;
	m_pCurrentLine->pen.y = 0;
}

void CAFTRichFont::endLine()
{
	if (m_pCurrentLine)
	{
		m_lines.push_back(m_pCurrentLine);
		m_textSize.width = MAX(m_textSize.width, (int)(m_pCurrentLine->bbox.xMax - m_pCurrentLine->bbox.xMin));

		int lineHeight = 0;
		for (int i = 0; i < m_pCurrentLine->glyphs.size(); i++)
		{
			if (m_pCurrentLine->glyphs[i].fontSize > lineHeight)
			{
				lineHeight = m_pCurrentLine->glyphs[i].fontSize;
			}
		}
		m_pCurrentLine->height = lineHeight;
		m_textSize.height += lineHeight;
	}
}

void CAFTRichFont::destroyAllLines()
{
	for (int i = 0; i < m_lines.size(); i++)
	{
		delete m_lines[i];
	}
	m_lines.clear();
	m_pCurrentLine = NULL;
}

void CAFTRichFont::initGlyphs(const std::vector<LabelElement>& labels)
{
	std::vector<LabelElement> labeltexts;
	for (int i = 0; i < labels.size(); i++)
	{
		const LabelElement& label = labels[i];

		const std::string& line = label.cszText;

		size_t pos = 0;
		size_t first = line.find('\n');
		while (first != std::string::npos)
		{
			labeltexts.push_back(LabelElement(line.substr(pos, first - pos), label.font, label.nHyperlink));
			initGlyphsLine(labeltexts);
			labeltexts.clear();

			pos = first + 1;
			first = line.find('\n', pos);
		}
		labeltexts.push_back(LabelElement(line.substr(pos, line.size()), label.font, label.nHyperlink));
	}
	initGlyphsLine(labeltexts);
}

void CAFTRichFont::initGlyphsLine(const std::vector<LabelElement>& labels)
{
	if (!labels.empty())
	{
		newLine();

		initGlyphsLineEx(labels);

		endLine();
	}
}


void CAFTRichFont::initGlyphsLineEx(const std::vector<LabelElement>& labels)
{
	FT_BBox bbox;   // bounding box containing all of the glyphs in the word
	std::vector<TGlyphEx> glyphs; // glyphs for the word
	int maxWidth = m_inSize.width ? m_inSize.width : 0xFFFFFF;

	FT_Error error = initWordGlyphs(labels, glyphs, m_pCurrentLine->pen);
	if (!error)
	{
		compute_bbox(glyphs, &bbox);
		if (m_pCurrentLine->width == 0 || bbox.xMax <= maxWidth)
		{
			if (m_pCurrentLine->width == 0 && bbox.xMax - bbox.xMin>maxWidth)
			{
				calcuMultiLines(glyphs);
			}
			else
			{
				m_pCurrentLine->glyphs.insert(m_pCurrentLine->glyphs.end(), glyphs.begin(), glyphs.end());
				if (m_pCurrentLine->width == 0)
				{
					m_pCurrentLine->bbox = bbox;
				}
				else
				{
					m_pCurrentLine->bbox.xMax = bbox.xMax;
					if (bbox.yMax > m_pCurrentLine->bbox.yMax)
					{
						m_pCurrentLine->bbox.yMax = bbox.yMax;
					}
				}
				m_pCurrentLine->width = (unsigned int)m_pCurrentLine->bbox.xMax - (unsigned int)m_pCurrentLine->bbox.xMin;
			}
		}
		else
		{
			endLine();
			newLine();
			initGlyphsLineEx(labels);
		}
	}
}

FT_Error CAFTRichFont::initWordGlyphs(const std::vector<LabelElement>& labels, std::vector<TGlyphEx>& glyphs, FT_Vector& pen)
{
	glyphs.clear();
	for (int i = 0; i < labels.size(); i++)
	{
		const LabelElement& label = labels[i];

		if (-1 == initWordGlyph(label, glyphs, pen))
			return -1;
	}
	return 0;
}

FT_Error CAFTRichFont::initWordGlyph(const LabelElement& label, std::vector<TGlyphEx>& glyphs, FT_Vector& pen)
{
	const CAFont& ft = label.font;
	const std::string& szText = label.cszText;
	FT_Face face = convertToSPFont(ft);
	CAColor4B col = ft.color;
	bool bBold = ft.bold;
	bool bItalics = ft.italics;
	bool bDeleteLine = ft.deleteLine;
	bool bUnderLine = ft.underLine;
	int iHyperlink = label.nHyperlink;
	int iFontSize = ft.fontSize;
	int iLineHeight = (int)(((face->size->metrics.ascender) >> 6) - ((face->size->metrics.descender) >> 6));
	int	italicsDt = iLineHeight * tan(ITALIC_LEAN_VALUE * 0.15 * M_PI);
	iLineHeight += iLineHeight / 5;
	FT_UInt previous = 0;
	FT_Error error = 0;

	std::u32string utf32String;
	if (!StringUtils::UTF8ToUTF32(szText, utf32String))
		return -1;

	FT_Bool useKerning = FT_HAS_KERNING(face);
	for (int n = 0; n < utf32String.size(); n++)
	{
		FT_ULong c = utf32String[n];

		if (c == '\r' || c == '\n')
			continue;

		/* convert character code to glyph index */
		glyphs.resize(glyphs.size() + 1);
		PGlyphEx glyph = &glyphs[glyphs.size() - 1];
		glyph->c = c;
		FT_UInt glyph_index = FT_Get_Char_Index(face, c);
		glyph->isEmoji = false;
		glyph->face = face;
		glyph->fontSize = iFontSize;
		if (iHyperlink==1)
		{
			glyph->col = m_linkColor;
		}
		else if (iHyperlink==2)
		{
			glyph->col = m_linkVisitedColor;
		}
		else
		{
			glyph->col = col;
		}
		glyph->deleteLine = bDeleteLine;
		glyph->underLine = (iHyperlink>0) ? true : bUnderLine;
		glyph->hyperlink = (iHyperlink>0);

		FT_Bool isOpenType = (glyph_index == 0);
		if (glyph_index == 0)
		{
			glyph_index = FT_Get_Char_Index(CATempTypeFont::getInstance().m_CurFontFace, c);
		}
		glyph->index = glyph_index;
		if (glyph_index == 0)
		{
			if (CAEmojiFont::getInstance()->isEmojiCodePoint(c))
			{
				isOpenType = false;
				glyph->isEmoji = true;
			}
			else
			{
				glyphs.resize(glyphs.size() - 1);
				continue;
			}
		}

		FT_Face curFace = isOpenType ? CATempTypeFont::getInstance().m_CurFontFace : face;
		if (curFace == NULL)
		{
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
			pen.x += iFontSize;
			continue;
		}

		/* load glyph image into the slot without rendering */
		error = FT_Load_Glyph(curFace, glyph_index, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP);
		if (error)
			continue;  /* ignore errors, jump to next glyph */

		if (bBold)
		{
			FT_Outline_Embolden(&slot->outline, 1 << 6);
		}

		if (c == 10)
		{
			continue;
		}

		/* extract glyph image and store it in our table */
		error = FT_Get_Glyph(curFace->glyph, &glyph->image);
		if (error)
			continue;  /* ignore errors, jump to next glyph */

		FT_Matrix* pFTMat = bItalics ? &m_ItalicMatrix : NULL;

		/* translate the glyph image now */
		FT_Glyph_Transform(glyph->image, pFTMat, NULL);

		/* increment pen position */
		pen.x += (slot->advance.x >> 6) + DEFAULT_SPACE_VALUE;
		if (pFTMat)
		{
			pen.x += (italicsDt/3.0f);
		}

		/* record current glyph index */
		previous = glyph_index;
	}
	return error;
}

FT_Face CAFTRichFont::convertToSPFont(const CAFont& ft)
{
	g_AFTFontCache.setCurrentFontData(ft.fontName.c_str(), ft.fontSize);
	return g_AFTFontCache.m_pCurFontData->ftFont.m_face;
}

void CAFTRichFont::compute_bbox(std::vector<TGlyphEx>& glyphs, FT_BBox *abbox)
{
	FT_BBox bbox;
	FT_BBox glyph_bbox;

	/* initialize string bbox to "empty" values */
	bbox.xMin = 32000;
	bbox.xMax = -32000;
	getLineYBBox(glyphs, bbox.yMin, bbox.yMax);

	for (std::vector<TGlyphEx>::iterator glyph = glyphs.begin(); glyph != glyphs.end(); ++glyph)
	{
		FT_GlyphSlot slot = glyph->face->glyph;

		FT_Glyph_Get_CBox(glyph->image, ft_glyph_bbox_pixels, &glyph_bbox);

		if (glyph->image == NULL)
		{
			glyph_bbox.xMax = (glyph_bbox.xMin + glyph->fontSize);
		}
		if (glyph_bbox.xMin == glyph_bbox.xMax)
		{
			glyph_bbox.xMax = glyph_bbox.xMin + (slot->advance.x >> 6) + DEFAULT_SPACE_VALUE;
		}
		glyph->width = glyph_bbox.xMax - glyph_bbox.xMin;

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

void CAFTRichFont::getLineYBBox(std::vector<TGlyphEx>& glyphs, FT_Pos& yPosMin, FT_Pos& yPosMax)
{
	yPosMin = 0;
	yPosMax = 0;

	FT_Face face = NULL;
	for (int i = 0, fs = -1; i < glyphs.size(); i++)
	{
		int ft = glyphs[i].fontSize;
		if (ft>fs)
		{
			face = glyphs[i].face;
			fs = ft;
		}
	}
	if (face != NULL)
	{
		yPosMin = (face->size->metrics.descender) >> 6;
		yPosMax = (face->size->metrics.ascender) >> 6;
	}
}

void CAFTRichFont::calcuMultiLines(std::vector<TGlyphEx>& glyphs)
{
	int maxWidth = m_inSize.width ? m_inSize.width : 0xFFFF;

	m_pCurrentLine->bbox.xMin = 32000;
	m_pCurrentLine->bbox.xMax = -32000;
	getLineYBBox(glyphs, m_pCurrentLine->bbox.yMin, m_pCurrentLine->bbox.yMax);

	int i = 0;
	for (; i < glyphs.size(); i++)
	{
		FT_BBox glyph_bbox;
		FT_Glyph_Get_CBox(glyphs[i].image, ft_glyph_bbox_pixels, &glyph_bbox);

		FT_GlyphSlot slot = glyphs[i].face->glyph;
		if (glyphs[i].image == NULL)
		{
			glyph_bbox.xMax = (glyph_bbox.xMin + glyphs[i].fontSize);
		}
		if (glyph_bbox.xMin == glyph_bbox.xMax)
		{
			glyph_bbox.xMax = glyph_bbox.xMin + (slot->advance.x >> 6) + DEFAULT_SPACE_VALUE;
		}

		glyph_bbox.xMin += glyphs[i].pos.x;
		glyph_bbox.xMax += glyphs[i].pos.x;
		glyph_bbox.yMin += glyphs[i].pos.y;
		glyph_bbox.yMax += glyphs[i].pos.y;

		if (maxWidth == 0xFFFF ||
			(glyph_bbox.xMax <= maxWidth && m_pCurrentLine->bbox.xMax - m_pCurrentLine->bbox.xMin <= maxWidth))
		{
			m_pCurrentLine->glyphs.push_back(glyphs[i]);
		}
		else
		{
			break;
		}

		if (glyph_bbox.xMin < m_pCurrentLine->bbox.xMin)
			m_pCurrentLine->bbox.xMin = glyph_bbox.xMin;

		if (glyph_bbox.yMin < m_pCurrentLine->bbox.yMin)
			m_pCurrentLine->bbox.yMin = glyph_bbox.yMin;

		if (glyph_bbox.xMax > m_pCurrentLine->bbox.xMax)
			m_pCurrentLine->bbox.xMax = glyph_bbox.xMax;

		if (glyph_bbox.yMax > m_pCurrentLine->bbox.yMax)
			m_pCurrentLine->bbox.yMax = glyph_bbox.yMax;
	}

	if (i <= 0)
	{
		return;
	}
	glyphs.erase(glyphs.begin(), glyphs.begin() + i);
	m_pCurrentLine->width = (unsigned int)(m_pCurrentLine->bbox.xMax - m_pCurrentLine->bbox.xMin);
	m_pCurrentLine->pen.x = m_pCurrentLine->bbox.xMax;

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

unsigned char* CAFTRichFont::getBitmap(int* outWidth, int* outHeight)
{
	int width, height;
	getTextSize(width, height);

	unsigned int size = width * height * 4;
	unsigned char* pBuffer = new unsigned char[size];
	if (!pBuffer)
	{
		return NULL;
	}
	memset(pBuffer, 0, size);

	m_HyperlinkRect = DRectZero;
	std::vector<CCRect> cc;

	FT_Vector pen;
	pen.x = pen.y = 0;
	for (int i = 0; i < m_lines.size(); i++)
	{
		FTLineInfoEx* line = m_lines[i];

		pen.x = - line->bbox.xMin;
		pen.y += line->height;

		drawText(line, pBuffer, &pen);

		calcuHyperlinkRects(line, &pen, cc);
		if (!m_HyperlinkRect.equals(DRectZero))
		{
			cc.push_back(m_HyperlinkRect);
		}
		m_HyperlinkRect = DRectZero;
	}
	*outWidth = width;
	*outHeight = height;

	if (!m_HyperlinkRect.equals(DRectZero))
	{
		cc.push_back(m_HyperlinkRect);
	}
	if (!cc.empty())
	{
		m_vHyperlinkRects.push_back(cc);
	}
	return pBuffer;
}

void CAFTRichFont::drawText(FTLineInfoEx* pInfo, unsigned char* pBuffer, FT_Vector *pen)
{
	std::vector<TGlyphEx>& glyphs = pInfo->glyphs;
	for (std::vector<TGlyphEx>::reverse_iterator glyph = glyphs.rbegin(); glyph != glyphs.rend(); ++glyph)
	{
		FT_Glyph image = glyph->image;
		if (image == NULL)
		{
			int iEmojiSize = (glyph->fontSize % 2) ? (glyph->fontSize - 1) : glyph->fontSize;

			CAImage* pEmoji = CAEmojiFont::getInstance()->getEmojiImage((unsigned int)glyph->c, iEmojiSize);
			if (pEmoji)
			{
				pEmoji = CAImage::scaleToNewImageWithImage(pEmoji, DSize(iEmojiSize, iEmojiSize));
			}

			if (pEmoji)
			{
				glyph->x = (FT_Int)(pen->x + glyph->pos.x);
				glyph->y = (FT_Int)(pen->y - iEmojiSize);
				draw_emoji(pBuffer, pEmoji, glyph->x, glyph->y, iEmojiSize);
			}
			continue;
		}

		FT_Error error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, 0, 1);
		if (!error)
		{
			FT_BitmapGlyph bit = (FT_BitmapGlyph)image;

			glyph->x = (FT_Int)(pen->x + glyph->pos.x + bit->left);
			glyph->y = (FT_Int)(pen->y - bit->top);
			draw_bitmap(pBuffer, &bit->bitmap, glyph->col, glyph->x, glyph->y);
			FT_Done_Glyph(image);

			if (glyph->underLine)
			{
				draw_line(pBuffer, glyph->col, glyph->x, pen->y + 2, glyph->x + glyph->width, pen->y + 2);
			}
			if (glyph->deleteLine)
			{
				draw_line(pBuffer, glyph->col, glyph->x, pen->y - glyph->fontSize / 3, glyph->x + glyph->width, pen->y - glyph->fontSize / 3);
			}
		}
	}
}

void CAFTRichFont::draw_emoji(unsigned char* pBuffer, CAImage* pEmoji, FT_Int x, FT_Int y, int iEmojiSize)
{
	FT_Int  x_max = x + iEmojiSize;
	FT_Int  y_max = y + iEmojiSize;

	int width, height;
	getTextSize(width, height);

	uint8_t* src = pEmoji->m_pData;
	for (FT_Int i = y; i < y_max; i++)
	{
		for (FT_Int j = x; j < x_max; j++)
		{
			if (i < 0 || j < 0 || j >= width || i >= height)
				continue;
		
			FT_Int index = (i * width * 4) + (j * 4);
			for (int k = 0; k < 4; k++)
			{
				pBuffer[index + k] = *src++;
			}
		}
	}
}

void CAFTRichFont::draw_bitmap(unsigned char* pBuffer, FT_Bitmap* bitmap, const CAColor4B& col, FT_Int x, FT_Int y)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	int width, height;
	getTextSize(width, height);

	for (i = x, p = 0; i < x_max; i++, p++)
	{
		for (j = y, q = 0; j < y_max; j++, q++)
		{
			if (i < 0 || j < 0 || i >= width || j >= height)
				continue;

			FT_Int index = (j * width * 4) + (i * 4);

			unsigned char value = bitmap->buffer[q * bitmap->width + p];
			if (value > 0)
			{
				pBuffer[index++] = col.r * value / 255.0f;
				pBuffer[index++] = col.g * value / 255.0f;
				pBuffer[index++] = col.b * value / 255.0f;
				pBuffer[index++] = value;
			}
		}
	}
}

void CAFTRichFont::draw_line(unsigned char* pBuffer, const CAColor4B& col, FT_Int x1, FT_Int y1, FT_Int x2, FT_Int y2)
{
	int width, height;
	getTextSize(width, height);
	for (FT_Int i = x1; i <= x2; i++)
	{
		for (FT_Int j = y1; j <= y2; j++)
		{
			if (i < 0 || j < 0 || i >= width || j >= height)
				continue;

			FT_Int index = (j * width * 4) + (i * 4);
			for (int k = 0; k < 4; k++)
			{
				pBuffer[index++] = col.r;
				pBuffer[index++] = col.g;
				pBuffer[index++] = col.b;
				pBuffer[index++] = 0xff;
			}
		}
	}
}

void CAFTRichFont::getTextSize(int& width, int& height)
{
	int w = MIN(m_inSize.width, m_textSize.width);
	int h = MIN(m_inSize.height, m_textSize.height);

	width  = MIN(1.5f*w, m_inSize.width);
	height = MIN(1.5f*h, m_inSize.height);
}

void CAFTRichFont::calcuHyperlinkRects(FTLineInfoEx* pInfo, FT_Vector *pen, std::vector<DRect>& cc)
{
	std::vector<TGlyphEx>& glyphs = pInfo->glyphs;
	for (std::vector<TGlyphEx>::iterator glyph = glyphs.begin(); glyph != glyphs.end(); ++glyph)
	{
		FT_Glyph image = glyph->image;

		if (glyph->hyperlink && image != NULL)
		{
			if (m_HyperlinkRect.equals(DRectZero))
			{
				int dt = 2;
				m_HyperlinkRect.origin.x = glyph->x;
				m_HyperlinkRect.origin.y = glyph->y - dt;
				m_HyperlinkRect.size.width = glyph->width;
				m_HyperlinkRect.size.height = glyph->fontSize + 2 * dt;
			}
			else
			{
				m_HyperlinkRect.size.width = glyph->x + glyph->width - m_HyperlinkRect.origin.x;
			}
		}
		else
		{
			if (!m_HyperlinkRect.equals(DRectZero))
			{
				cc.push_back(m_HyperlinkRect);
				m_vHyperlinkRects.push_back(cc);
				cc.clear();
			}
			m_HyperlinkRect = DRectZero;
		}
	}
}

NS_CC_END
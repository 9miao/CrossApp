#include "CAFreeTypeFont.h"
#include "basics/CAApplication.h"
#include "platform/CCFileUtils.h"
#include "support/ccUTF8.h"


using namespace std;

NS_CC_BEGIN

static map<std::string, FontBufferInfo> s_fontsNames;
static FT_Library s_FreeTypeLibrary = NULL;


CAFreeTypeFont::CAFreeTypeFont()
:m_space(" ")
,m_currentLine(NULL)
,m_face(NULL)
,m_inWidth(0)
,m_inHeight(0)
,m_width(0)
,m_height(0)
,m_textWidth(0)
,m_textHeight(0)
,m_windowWidth(0)
,m_lineHeight(0)
, m_isForTextField(false)
{
    CCSize size = CAApplication::getApplication()->getWinSizeInPixels();
    m_windowWidth = (int)size.width;
}

CAFreeTypeFont::~CAFreeTypeFont() 
{
	finiFreeTypeFont();
}


CAImage* CAFreeTypeFont::initWithString(const char* pText, const char* pFontName, int nSize, int inWidth, int inHeight, CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment)
{
	FT_Error error = 0;
	unsigned long size = 0;
	unsigned char* pBuffer = NULL;
    
	if (pText == NULL || pFontName == NULL)
		return NULL;
    
	m_inWidth = inWidth;
	m_inHeight = inHeight;
    
	// attempt to load font from Resources fonts folder
	pBuffer = loadFont(pFontName, &size);
    
	if (!pBuffer) // font not found!
		return NULL;
    
	if (!s_FreeTypeLibrary)
	{
		error = FT_Init_FreeType(&s_FreeTypeLibrary);
	}
    
	if (!error && !m_face)
	{
		error = FT_New_Memory_Face(s_FreeTypeLibrary, pBuffer, size, 0, &m_face);
	}
    
	if (!error)
	{
		error = FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
	}
    
	if (!error)
		error = FT_Set_Char_Size(m_face, nSize << 6, nSize << 6, 72, 72);
    
	if (!error)
		error = initGlyphs(pText);
    
    
	CCImage::ETextAlign eAlign;
    
	if (inHeight == nSize && m_inWidth < m_textWidth)
	{
		hAlignment = CATextAlignmentLeft;
	}
	if (m_inHeight < m_textHeight)
	{
		vAlignment = CAVerticalTextAlignmentTop;
	}
    
	if (CAVerticalTextAlignmentTop == vAlignment)
	{
		eAlign = (CATextAlignmentCenter == hAlignment) ? CCImage::kAlignTop
        : (CATextAlignmentLeft == hAlignment) ? CCImage::kAlignTopLeft : CCImage::kAlignTopRight;
	}
	else if (CAVerticalTextAlignmentCenter == vAlignment)
	{
		eAlign = (CATextAlignmentCenter == hAlignment) ? CCImage::kAlignCenter
        : (CATextAlignmentLeft == hAlignment) ? CCImage::kAlignLeft : CCImage::kAlignRight;
	}
	else if (CAVerticalTextAlignmentBottom == vAlignment)
	{
		eAlign = (CATextAlignmentCenter == hAlignment) ? CCImage::kAlignBottom
        : (CATextAlignmentLeft == hAlignment) ? CCImage::kAlignBottomLeft : CCImage::kAlignBottomRight;
	}
	else
	{
		CCAssert(false, "Not supported alignment format!");
		return NULL;
	}

	int width = 0, height = 0;
	unsigned char* pData = getBitmap(eAlign, &width, &height);
	if (pData == NULL)
	{
		return NULL;
	}

	CCImage* pImage = new CCImage();
	if (!pImage->initWithImageData(pData, width*height * 4, CCImage::kFmtRawData, width, height))
	{
		delete []pData;
		delete pImage;
		return NULL;
	}
	delete[]pData;
	
	CAImage* pCAImage = new CAImage();
	if (!pCAImage->initWithImage(pImage))
	{
		delete pImage;
		delete pCAImage;
		return NULL;
	}
    pImage->release();
    pCAImage->autorelease();
	return pCAImage;
}

unsigned char* CAFreeTypeFont::getBitmap(CCImage::ETextAlign eAlignMask, int* outWidth, int* outHeight)
{
    int lineNumber = 0;
    int totalLines = m_lines.size();

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
		return ((m_face->size->metrics.ascender) >> 6) - ((m_face->size->metrics.descender) >> 6);
	}
	return 0;
}

// text encode with utf8
int CAFreeTypeFont::getStringWidth(const std::string& text)
{
	std::vector<TGlyph> glyphs;

	FT_Vector vt;
	memset(&vt, 0, sizeof(vt));
	if (0 != initWordGlyphs(glyphs, text, vt))
		return 0;

	FT_BBox bbox;
	compute_bbox(glyphs, &bbox);
	return bbox.xMax - bbox.xMin;
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

FT_Vector CAFreeTypeFont::getPenForAlignment(FTLineInfo* pInfo, CCImage::ETextAlign eAlignMask,int lineNumber, int totalLines)
{
    FT_Vector pen;
    
    int top;
    int stringWidth  = pInfo->bbox.xMax - pInfo->bbox.xMin;
    int maxLineNumber = totalLines - 1;
    pen.x = 0;
    pen.y = 0;

    switch(eAlignMask)
    {
        case CCImage::kAlignTop: // Horizontal center and vertical top.
            pen.x = ((m_width  - stringWidth) / 2) - pInfo->bbox.xMin;
            pen.y = pInfo->bbox.yMax + (lineNumber * m_lineHeight);		    
 		    break;
			
        case CCImage::kAlignTopLeft: // Horizontal left and vertical top.
            pen.x -=pInfo->bbox.xMin;
            pen.y = pInfo->bbox.yMax + (lineNumber * m_lineHeight);		    
 		    break;

	    case CCImage::kAlignTopRight: // Horizontal right and vertical top.
            pen.x = m_width - stringWidth - pInfo->bbox.xMin;
            pen.y = pInfo->bbox.yMax + (lineNumber * m_lineHeight);		    
		    break;
 
	    case CCImage::kAlignBottomRight: // Horizontal right and vertical bottom.
            pen.x = m_width - stringWidth - pInfo->bbox.xMin;
            pen.y = m_height + pInfo->bbox.yMin - ((maxLineNumber - lineNumber) * m_lineHeight);
		    break;

	    case CCImage::kAlignBottom: // Horizontal center and vertical bottom.
            pen.x = ((m_width  - stringWidth) / 2) - pInfo->bbox.xMin;
            pen.y = m_height + pInfo->bbox.yMin - ((maxLineNumber - lineNumber) * m_lineHeight);
		    break;

	    case CCImage::kAlignBottomLeft: // Horizontal left and vertical bottom.
            pen.x -=pInfo->bbox.xMin;
            top = (m_height - m_textHeight) / 2;
            pen.y = m_height + pInfo->bbox.yMin - ((maxLineNumber - lineNumber) * m_lineHeight);
  		    break;

	    case CCImage::kAlignCenter: // Horizontal center and vertical center
            pen.x = ((m_width  - stringWidth) / 2) - pInfo->bbox.xMin;
            top = (m_height - m_textHeight) / 2;
            pen.y = top + (lineNumber * m_lineHeight) + pInfo->bbox.yMax;		    
            break;

	    case CCImage::kAlignRight: // Horizontal right and vertical center.
            pen.x = m_width - stringWidth - pInfo->bbox.xMin;
            top = (m_height - m_textHeight) / 2;
            pen.y = top + (lineNumber * m_lineHeight) + pInfo->bbox.yMax;		    
  		    break;

	    case CCImage::kAlignLeft: // Horizontal left and vertical center.
	    default:
            pen.x -=pInfo->bbox.xMin;
            top = (m_height - m_textHeight) / 2;
            pen.y = top + (lineNumber * m_lineHeight) + pInfo->bbox.yMax;		    
  		    break;
    }

    return pen;
}

void  CAFreeTypeFont::drawText(FTLineInfo* pInfo, unsigned char* pBuffer, FT_Vector *pen)
{
	std::vector<TGlyph>& glyphs = pInfo->glyphs;
	for (std::vector<TGlyph>::iterator glyph = glyphs.begin(); glyph != glyphs.end(); ++glyph)
    {
        FT_Glyph image = glyph->image;
        FT_Error error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, 0, 1);
        if (!error)
        {
            FT_BitmapGlyph  bit = (FT_BitmapGlyph)image;
            draw_bitmap(pBuffer, &bit->bitmap, pen->x + glyph->pos.x + bit->left,pen->y - bit->top);
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

            unsigned char value =  bitmap->buffer[q * bitmap->width + p];

            if(value > 0)
            {
                FT_Int index = (j * m_width * 4) + (i * 4);
                pBuffer[index++] = 0xff;
                pBuffer[index++] = 0xff;
                pBuffer[index++] = 0xff;
                pBuffer[index++] = value;
           }
        }
    }  
}

void CAFreeTypeFont::endLine() 
{
    if(m_currentLine)
    {
        m_lines.push_back(m_currentLine);
		m_textWidth = MAX(m_textWidth, m_currentLine->bbox.xMax - m_currentLine->bbox.xMin);
        m_textHeight += m_lineHeight;
    }
}

void CAFreeTypeFont::newLine() 
{
    m_currentLine = new FTLineInfo();
    m_currentLine->width = 0;
    m_currentLine->pen.x = 0;
    m_currentLine->pen.y = 0;
}


void CAFreeTypeFont::calcuMultiLines(std::vector<TGlyph>& glyphs)
{
	FT_BBox glyph_bbox, global_bbox;
	int maxWidth = m_inWidth ? m_inWidth : 0xFFFF;
	
	m_currentLine->bbox.xMin = 32000;
	m_currentLine->bbox.xMax = -32000;
	m_currentLine->bbox.yMin = (m_face->size->metrics.descender) >> 6;
	m_currentLine->bbox.yMax = (m_face->size->metrics.ascender) >> 6;
    
	int i = 0;
	for (; i < glyphs.size(); i++)
	{
		FT_Glyph_Get_CBox(glyphs[i].image, ft_glyph_bbox_pixels, &glyph_bbox);
        
		glyph_bbox.xMin += glyphs[i].pos.x;
		glyph_bbox.xMax += glyphs[i].pos.x;
		glyph_bbox.yMin += glyphs[i].pos.y;
		glyph_bbox.yMax += glyphs[i].pos.y;
        
		int dtValue = glyph_bbox.xMax - glyph_bbox.xMin;
		if (m_inWidth == 0xFFFF || 
			(glyphs[i].pos.x + dtValue <= maxWidth && m_currentLine->bbox.xMax - m_currentLine->bbox.xMin<=maxWidth))
		{
			m_currentLine->glyphs.push_back(glyphs[i]);
		}
		else break;

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
	m_currentLine->width = m_currentLine->bbox.xMax - m_currentLine->bbox.xMin;

	if (!glyphs.empty())
	{
		endLine();
		newLine();


		int start_pos = glyphs[0].pos.x;
		for (int i = 0; i < glyphs.size(); i++)
		{
			glyphs[i].pos.x -= start_pos;
		}
		calcuMultiLines(glyphs);
	}
}

FT_Error CAFreeTypeFont::addWord(const std::string& word) 
{
	std::vector<TGlyph> glyphs; // glyphs for the word
	FT_BBox             bbox;   // bounding box containing all of the glyphs in the word
    int maxWidth = m_inWidth ? m_inWidth : m_windowWidth;
    std::string newWord;

    if(m_currentLine->width > 0) 
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
				m_currentLine->width = m_currentLine->bbox.xMax - m_currentLine->bbox.xMin;
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

FT_Error CAFreeTypeFont::initGlyphs(const char* text) 
{
    FT_Error error = 0;
    std::stringstream stringStream(text);
    std::string line;
    vector<std::string> lines;
    vector<std::string> words;

    m_textWidth = 0;
    m_textHeight = 0;
    // the height of a line of text based on the max height of a glyph in the font size
    m_lineHeight = ((m_face->size->metrics.ascender) >> 6) - ((m_face->size->metrics.descender) >> 6);

	destroyAllLines();

	if (m_isForTextField)
	{
		newLine();
		addWord(text);
		endLine();
		return error;
	}

    while(std::getline(stringStream, line) && !error) 
    {
        newLine();

        std::size_t prev = 0, pos;
        while ((pos = line.find_first_of(" ", prev)) != std::string::npos)
        {
            if (pos > prev)
            {
                addWord(line.substr(prev, pos-prev));
            }
            prev = pos + 1;
        }
        if (prev < line.length())
        {
            addWord(line.substr(prev, std::string::npos));
        }
        endLine();
    }

    return error;
}

// if linewidth > m_inWidth
// get words for line

void CAFreeTypeFont::initWords(const char* text) 
{
    std::stringstream stringStream(text);
    std::string line;
    vector<std::string> lines;
    vector<std::string> words;

    while(std::getline(stringStream, line)) 
    {
        lines.push_back(line);
    }

	for (vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        std::size_t prev = 0, pos;
        while ((pos = it->find_first_of(" ';", prev)) != std::string::npos)
        {
            if (pos > prev)
                words.push_back(it->substr(prev, pos-prev));
            prev = pos+1;
        }
        if (prev < it->length())
            words.push_back(it->substr(prev, std::string::npos));
    }

	for (vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
    {
        std::string foo(*it);
    }
}


FT_Error CAFreeTypeFont::initWordGlyphs(std::vector<TGlyph>& glyphs, const std::string& text, FT_Vector& pen) 
{
	FT_GlyphSlot	slot = m_face->glyph; 
	FT_UInt			glyph_index;
	FT_UInt			previous = 0;
	FT_Error		error = 0;
	PGlyph			glyph;
    unsigned int    numGlyphs = 0;
	

	std::u16string utf16String;
	if (!StringUtils::UTF8ToUTF16(text, utf16String))
		return -1;

	glyphs.clear();
	FT_Bool useKerning = FT_HAS_KERNING(m_face);

	for (int n = 0; n < utf16String.size(); n++)
	{
		FT_ULong c = utf16String[n];

		/* convert character code to glyph index */
		glyphs.resize(glyphs.size() + 1);
		glyph = &glyphs[numGlyphs];

		
		glyph_index = FT_Get_Char_Index(m_face, c);

 		if (useKerning && previous && glyph_index)
		{
			FT_Vector  delta;
			FT_Get_Kerning(m_face, previous, glyph_index, FT_KERNING_DEFAULT, &delta);
			pen.x += delta.x >> 6;
		}

		/* store current pen position */
		glyph->pos = pen;
		glyph->index = glyph_index;
		

		/* load glyph image into the slot without rendering */
		error = FT_Load_Glyph(m_face, glyph_index, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP);
		if (error)
			continue;  /* ignore errors, jump to next glyph */

		/* extract glyph image and store it in our table */
		error = FT_Get_Glyph(m_face->glyph, &glyph->image);
		if (error)
			continue;  /* ignore errors, jump to next glyph */

		 /* translate the glyph image now */
		FT_Glyph_Transform(glyph->image, 0, &glyph->pos);

		/* increment pen position */
		pen.x += slot->advance.x >> 6;

		/* record current glyph index */
		previous = glyph_index;

		numGlyphs++;
	}

	return error;
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

		if (glyph_bbox.xMin == glyph_bbox.xMax)
		{
			glyph_bbox.xMax = glyph_bbox.xMin + slot->advance.x >> 6;
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

bool CAFreeTypeFont::initFreeTypeFont(const char* pFontName, unsigned long nSize)
{
	unsigned long size = 0;
	unsigned char* pBuffer = loadFont(pFontName, &size);
	if (pBuffer == NULL)
		return false;
	
	FT_Error error = 0;
	if (!s_FreeTypeLibrary)
	{
		error = FT_Init_FreeType(&s_FreeTypeLibrary);
	}

	if (!error && !m_face)
	{
		error = FT_New_Memory_Face(s_FreeTypeLibrary, pBuffer, size, 0, &m_face);
	}

	if (!error)
	{
		error = FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
	}

	if (!error)
		error = FT_Set_Char_Size(m_face, nSize << 6, nSize << 6, 72, 72);

	return (error==0);
}

void CAFreeTypeFont::finiFreeTypeFont()
{
	if (m_face)
	{
		FT_Done_Face(m_face);
	}
	m_face = NULL;

	destroyAllLines();
}

unsigned char* CAFreeTypeFont::loadFont(const char *pFontName, unsigned long *size)
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

    if (std::string::npos == lowerCase.find(".ttf"))
    {
        path += ".ttf";
    }

	std::map<std::string, FontBufferInfo>::iterator ittFontNames = s_fontsNames.find(path.c_str());
	if (ittFontNames != s_fontsNames.end())
	{
		*size = ittFontNames->second.size;
		return ittFontNames->second.pBuffer;
	}

	std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(path.c_str());
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "rb", size);
	if (pBuffer == NULL)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        char sTTFont[256];
        GetWindowsDirectoryA(sTTFont,255);
        strcat(sTTFont,"\\fonts\\simhei.ttf");
        pFontName = sTTFont;
        pBuffer = CCFileUtils::sharedFileUtils()->getFileData(pFontName, "rb", size);
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        pFontName = "/System/Library/Fonts/Cache/STHeiti-Light.ttc";
        pBuffer = CCFileUtils::sharedFileUtils()->getFileData(pFontName, "rb", size);
        
        if (pBuffer == NULL)
        {
            pFontName = "/System/Library/Fonts/STHeiti Light.ttc";
            pBuffer = CCFileUtils::sharedFileUtils()->getFileData(pFontName, "rb", size);
        }
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        pFontName = "/system/fonts/DroidSansFallback.ttf";
        pBuffer = CCFileUtils::sharedFileUtils()->getFileData(pFontName, "rb", size);
        
#endif
        
	}

	FontBufferInfo info;
	info.pBuffer = pBuffer;
	info.size = *size;
	s_fontsNames[path] = info;
	return pBuffer;
}

NS_CC_END


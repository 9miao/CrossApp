#include "CAFTFontCache.h"
#include "basics/CAApplication.h"
#include "platform/CAFileUtils.h"
#include "CAFTRichFont.h"
#include "support/ccUTF8.h"


NS_CC_BEGIN


CAFTFontCache::CAFTFontCache()
: m_pCurFontData(NULL)
{
	initDefaultFont();
}

CAFTFontCache::~CAFTFontCache()
{
	destroyAllFontData();
	CAFreeTypeFont::destroyAllFontBuff();
}

CAFTFontCache& CAFTFontCache::getAFTFontCache()
{
	static CAFTFontCache instance;
	return instance;
}

void CAFTFontCache::initDefaultFont()
{
	if (!m_FontDataVect.empty())
		return;

	setCurrentFontData("", 18);
}

void CAFTFontCache::destroyAllFontData()
{
	for (int i = 0; i < m_FontDataVect.size(); i++)
	{
		FontDataTable* t = m_FontDataVect[i];
		t->ftFont.finiFreeTypeFont();
		delete t;
	}
	m_FontDataVect.clear();
}


void CAFTFontCache::setCurrentFontData(const char* pFontName, int nSize)
{
	CCAssert(pFontName != NULL, "");
	if (m_pCurFontData)
	{
		if (m_pCurFontData->szFontName == pFontName && m_pCurFontData->iFontSize == nSize)
			return;
	}

	for (int i = 0; i < m_FontDataVect.size(); i++)
	{
		FontDataTable* v = m_FontDataVect[i];
		if (v->szFontName == pFontName && v->iFontSize == nSize)
		{
			m_pCurFontData = v;
			return;
		}
	}

	FontDataTable* fData = new FontDataTable();
	fData->szFontName = pFontName;
	fData->iFontSize = nSize;
	fData->ftFont.initFreeTypeFont(pFontName, nSize);
	fData->iFontHeight = fData->ftFont.getFontHeight();
	m_FontDataVect.push_back(fData);
	m_pCurFontData = m_FontDataVect.back();
}

int CAFTFontCache::getFontHeight(const char* pFontName, unsigned long nSize)
{
	setCurrentFontData(pFontName, (int)nSize);
	return m_pCurFontData->iFontHeight;
}

int CAFTFontCache::getStringWidth(const char* pFontName, unsigned long nSize, const std::string& text, bool bBold, bool bItalics)
{
	setCurrentFontData(pFontName, (int)nSize);
	return m_pCurFontData->ftFont.getStringWidth(text, bBold, bItalics);
}

int CAFTFontCache::cutStringByWidth(const char* pFontName, unsigned long nSize, const std::string& text, int iLimitWidth, int& cutWidth, bool bBold, bool bItalics)
{
	setCurrentFontData(pFontName, (int)nSize);
	return m_pCurFontData->ftFont.cutStringByWidth(text, iLimitWidth, cutWidth);
}

int CAFTFontCache::cutStringByDSize(std::string& text, const DSize& lableSize, const char* pFontName, unsigned long nSize, bool bWordWrap, int iLineSpacing, bool bBold, bool bItalics)
{
	setCurrentFontData(pFontName, (int)nSize);
	return m_pCurFontData->ftFont.cutStringByDSize(text, lableSize, pFontName, nSize, bWordWrap, iLineSpacing, bBold, bItalics);
}

int CAFTFontCache::getStringHeight(const char* pFontName, unsigned long nSize, const std::string& text, int iLimitWidth, int iLineSpace, bool bWordWrap)
{
	setCurrentFontData(pFontName, (int)nSize);
	return m_pCurFontData->ftFont.getStringHeight(text, iLimitWidth, iLineSpace, bWordWrap);
}

CAImage* CAFTFontCache::initWithString(const char* pText, const CAColor4B& fontColor, const char* pFontName, int nSize, int width, int height,
	CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment, bool bWordWrap, int iLineSpacing, bool bBold, bool bItalics, bool bUnderLine, bool bDeleteLine, std::vector<TextViewLineInfo>* pLinesText)
{
	if (pText == NULL || pFontName == NULL)
		return NULL;

	setCurrentFontData(pFontName, nSize);
	CAImage* pImage = m_pCurFontData->ftFont.initWithString(pText, fontColor, pFontName, nSize, width, height, hAlignment, vAlignment, bWordWrap, iLineSpacing, bBold, bItalics, bUnderLine, bDeleteLine, pLinesText);
	return pImage;
}

NS_CC_END


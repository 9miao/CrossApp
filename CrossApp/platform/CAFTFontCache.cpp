#include "CAFTFontCache.h"
#include "basics/CAApplication.h"
#include "platform/CCFileUtils.h"
#include "support/ccUTF8.h"


using namespace std;

NS_CC_BEGIN


CAFTFontCache::CAFTFontCache()
: m_pCurFontData(NULL)
, m_isForTextField(false)
{
	initDefaultFont();
}

CAFTFontCache::~CAFTFontCache()
{
	destroyAllFontData();
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
    while (!m_FontDataVect.empty())
    {
        FontDataTable* t = m_FontDataVect.back();
        t->ftFont.finiFreeTypeFont();
        delete t;
        m_FontDataVect.pop_back();
    }
}


void CAFTFontCache::setCurrentFontData(const char* pFontName, int nSize)
{
	CCAssert(pFontName != NULL, "");
	if (m_pCurFontData)
	{
		if (m_pCurFontData->szFontName == pFontName && m_pCurFontData->iFontSize == nSize)
			return;
	}

    for (std::vector<FontDataTable *>::iterator it = m_FontDataVect.begin(); it != m_FontDataVect.end(); ++it)
	{
		FontDataTable* v = *it;
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
	setCurrentFontData(pFontName, nSize);
	return m_pCurFontData->iFontHeight;
}

int CAFTFontCache::getStringWidth(const char* pFontName, unsigned long nSize, const std::string& text)
{
	setCurrentFontData(pFontName, nSize);
	return m_pCurFontData->ftFont.getStringWidth(text);
}

CAImage* CAFTFontCache::initWithString(const char* pText, const char* pFontName, int nSize, int width, int height, CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment)
{
	if (pText == NULL || pFontName == NULL)
		return NULL;

	setCurrentFontData(pFontName, nSize);
	CAImage* pImage = m_pCurFontData->ftFont.initWithString(pText, pFontName, nSize, width, height, hAlignment, vAlignment);
	setForTextField(false);
	return pImage;
}

NS_CC_END


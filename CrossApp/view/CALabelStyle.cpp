#include "CALabelStyle.h"
#include "platform/CAFTFontCache.h"

NS_CC_BEGIN

CALabelStyle::CALabelStyle()
: m_nFontSize(20)
, m_nLineSpace(5)
, m_cFontColor(CAColor_black)
, m_bBold(false)
, m_bItalics(false)
, m_bWordWrap(false)
{

}

CALabelStyle* CALabelStyle::create()
{
	CALabelStyle* pStyle = new CALabelStyle();
	pStyle->autorelease();
	return pStyle;
}

int CALabelStyle::getFontHeight() const
{
	return g_AFTFontCache.getFontHeight(m_sFontName.c_str(), m_nFontSize);
}

int CALabelStyle::getStringWidth(const std::string& text) const
{
	return g_AFTFontCache.getStringWidth(m_sFontName.c_str(), m_nFontSize, text, m_bBold, m_bItalics);
}

int CALabelStyle::cutStringByWidth(const std::string& text, int iLimitWidth, int& cutWidth) const
{
	return g_AFTFontCache.cutStringByWidth(m_sFontName.c_str(), m_nFontSize, text, iLimitWidth, cutWidth, m_bBold, m_bItalics);
}

int CALabelStyle::getStringHeight(const std::string& text, int iLimitWidth) const
{
	return g_AFTFontCache.getStringHeight(m_sFontName.c_str(), m_nFontSize, text, iLimitWidth, m_nLineSpace, m_bWordWrap);
}

void CALabelStyleCache::addStyle(const string& sStyleName, const string& sFontName, int nFontSize, int nLineSpace, 
	const CAColor4B& cFontColor, bool bBold, bool bItalics, bool bWordWrap)
{
	CALabelStyle* pStyle = CALabelStyle::create();
	pStyle->setStyleName(sStyleName);
	pStyle->setFontName(sFontName);
	pStyle->setFontSize(nFontSize);
	pStyle->setFontColor(cFontColor);
	pStyle->setLineSpace(nLineSpace);
	pStyle->setBold(bBold);
	pStyle->setItalics(bItalics);
	pStyle->setWordWrap(bWordWrap);

	if (m_StyleMap.contains(sStyleName))
	{
		m_StyleMap.erase(sStyleName);
	}
	
	m_StyleMap.insert(sStyleName, pStyle);
	
}

const CALabelStyle* CALabelStyleCache::getStyle(const string& sStyleName)
{
	return m_StyleMap.getValue(sStyleName);
}

CALabelStyleCache::CALabelStyleCache()
{

}

CALabelStyleCache::~CALabelStyleCache()
{
	m_StyleMap.clear();
}

static CALabelStyleCache *g_sharedLabelStyleCache = NULL;

CALabelStyleCache * CALabelStyleCache::sharedStyleCache()
{
	if (!g_sharedLabelStyleCache)
	{
		g_sharedLabelStyleCache = new CALabelStyleCache();
	}
	return g_sharedLabelStyleCache;
}

void CALabelStyleCache::purgeSharedStyleCache()
{
	CC_SAFE_DELETE(g_sharedLabelStyleCache);
}

NS_CC_END
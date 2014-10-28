#ifndef CALabelStyle_h__
#define CALabelStyle_h__

#include "CAView.h"
#include <map>
using std::string;
using std::map;

NS_CC_BEGIN

class CC_DLL CALabelStyle : public CAObject
{
public:
	CALabelStyle();

	static const CALabelStyle* getStyle(const string& sStyleName);

	int getFontHeight() const;

	int getStringWidth( const std::string& text) const;

	int cutStringByWidth(const std::string& text, int iLimitWidth, int& cutWidth) const;

	int getStringHeight(const std::string& text, int iLimitWidth) const;

protected:
	CC_SYNTHESIZE_PASS_BY_REF(string, m_sStyleName, StyleName);
	CC_SYNTHESIZE_PASS_BY_REF(string, m_sFontName, FontName);
	CC_SYNTHESIZE(int, m_nFontSize, FontSize);
	CC_SYNTHESIZE(int, m_nLineSpace, LineSpace);
	CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_cFontColor, FontColor);
	CC_SYNTHESIZE_IS(bool, m_bBold, Bold);
	CC_SYNTHESIZE_IS(bool, m_bItalics, Italics);
	CC_SYNTHESIZE_IS(bool, m_bWordWrap, WordWrap);
};

class CC_DLL CALabelStyleCache
{
public:
	void addStyle(const string& sStyleName, const string& sFontName, int nFontSize, int nLineSpace, const CAColor4B& cFontColor,
		bool bBold, bool bItalics, bool bWordWrap);

	const CALabelStyle* getStyle(const string& sStyleName);

	static CALabelStyleCache * sharedStyleCache();

	static void purgeSharedStyleCache();

private:
	CALabelStyleCache();
	~CALabelStyleCache();

	map<string, CALabelStyle*> m_StyleMap;
};

NS_CC_END

#endif // CALabelStyle_h__

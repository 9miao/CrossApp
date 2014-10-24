#ifndef CALabelStyle_h__
#define CALabelStyle_h__

#include "CAView.h"
using std::string;

NS_CC_BEGIN

class CALabelStyle : public CAObject
{
public:
	CALabelStyle();

public:
	CC_SYNTHESIZE_IS_PASS_BY_REF(string, m_sFontName, FontName);
	CC_SYNTHESIZE(int, m_nFontSize, FontSize);
	CC_SYNTHESIZE(int, m_nLineSpace, LineSpace);
	CC_SYNTHESIZE_IS_PASS_BY_REF(CAColor4B, m_cFontColor, FontColor);
	CC_SYNTHESIZE_IS(bool, m_bBold, Bold);
	CC_SYNTHESIZE_IS(bool, m_bItalics, Italics);
	CC_SYNTHESIZE_IS(bool, m_bWordWrap, WordWrap);
};

NS_CC_END

#endif // CALabelStyle_h__
